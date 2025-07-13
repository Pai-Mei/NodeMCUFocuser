//Includes the Arduino Stepper Library
#include <AccelStepper.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <Servo.h>

#define Debug 0  // set to 1 to see debug messages over serial
#define EEPROMStartAddress 0x0f

#define BOOT_MODE_BUTTON_PIN 0
#define INDICATOR_LED_PIN 2
#define INDICATOR_LED_ON 0
#define INDICATOR_LED_OFF 1
#define RESET_CONFIG_THRESHOLD 5000

#define Status_Moving "moving"
#define Status_Stoped "stoped"

//global settings stored in EEPROM
bool ServerEnabled;
bool CreateAccessPoint;
String ssid;
String pass;
IPAddress localIP;
IPAddress gateway;
IPAddress subnet;

#define pinServo 5
#define pinLight 0//4
int brightness = 0;

// Define step constant (4 - full-step, 8 - half-step)
#define MotorInterfaceType 4
#define pinIN1 14
#define pinIN3 13
#define pinIN4 15
#define pinIN2 12
// Pins entered in sequence IN1-IN3-IN4-IN2 for proper step sequence
AccelStepper myStepper(MotorInterfaceType, pinIN1, pinIN3, pinIN4, pinIN2);
//servo falt box lid control
Servo servo;
//global variables for stepper
int currentPosition = 0;
int targetPosition = 0;
bool isMoving = false;

//global variables for web server timeout
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 30000;

ESP8266WebServer server(80);

void SetDefaults() {
  analogWriteRange(1024);
  analogWriteFreq(500);
}

void setup() {
  Serial.begin(9600);
  InitSettings();
  SetupStepper();
  SetupFlatBox();
  SetupWiFi();
  SetupServer();
  SetupTimer();
}

void loop() {
  if(isMoving)
    DoStep();
  else {
    CheckSerial();
    CheckServer();
  }
}

//------Timer----------------------------------

void ICACHE_RAM_ATTR onTimerISR(){  
  if(brightness > 0)
  {
    digitalWrite(pinLight,HIGH);
    delayMicroseconds(brightness);
    digitalWrite(pinLight,LOW);
  }      
  timer1_write(10000 - brightness);
}

void SetupTimer()
{
  //Initialize Ticker every 0.5s
    timer1_attachInterrupt(onTimerISR);
    //TIM_DIV1 = 0,   //80MHz (80 ticks/us - 104857.588 us max)
    //TIM_DIV16 = 1,  //5MHz (5 ticks/us - 1677721.4 us max)
    //TIM_DIV256 = 3 //312.5Khz (1 tick = 3.2us - 26843542.4 us max)
    timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
    timer1_write(10000); //2ms
}



//------Stepper----------------------------------

void SetupStepper() {
  myStepper.setMaxSpeed(500.0);
  myStepper.setAcceleration(50.0);
  myStepper.setSpeed(20);
  targetPosition = currentPosition = myStepper.currentPosition();
}

void DoStep() {
  if (isMoving) {
    isMoving = myStepper.run();
    currentPosition = myStepper.currentPosition();
    if (!isMoving && targetPosition == currentPosition) Serial.println(WithTermination(Status_Stoped));
  }
  if (targetPosition != currentPosition) {
    myStepper.moveTo(targetPosition);
    isMoving = true;
  }
}

void SetMaxPosition(int steps) {
  targetPosition = currentPosition = steps / 2;
  myStepper.setCurrentPosition(currentPosition);
  isMoving = false;
}


void Move(int steps) {
  targetPosition += steps;
  isMoving = true;
}

void MoveTo(int position) {
  targetPosition = position;
  isMoving = true;
}

void Stop() {
  myStepper.setSpeed(0);
  targetPosition = currentPosition;
  isMoving = false;
}

void StopBrake() {
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, LOW);
  digitalWrite(pinIN3, LOW);
  digitalWrite(pinIN4, LOW);
}

//-----flat panel control--------------------------------------


void SetupFlatBox()
{
  servo.attach(pinServo, 500, 2500); 
  servo.write(176);
}

void OpenCover() {
  servo.write(5);
}

void CloseCover() {
  servo.write(176);
}

void SetLight(int level) {
  analogWrite(pinLight, level);
}

//------Serial communication----------------------------------

#define statusCommand "status"
#define StopBrakeCommand "stopBrake"
#define StopCommand "stop"
#define MoveCommand "move"
#define MoveToCommand "moveto"
#define SetMaxPositionCommand "set_max_position"
#define SaveSettings "save_settings"
#define GetSettings "get_settings"
#define OpenCoverCmd "open_cover"
#define CloseCoverCmd "close_cover"
#define SetLightCmd "set_light"
#define Termination "#"

String WithTermination(String message) {
  return message + Termination;
}

String ProcessCommand(String command) {
  if (command.startsWith(SetMaxPositionCommand)) {
    int steps = command.substring(strlen(SetMaxPositionCommand) + 1).toInt();
    SetMaxPosition(steps);
    return WithTermination(String("yes"));
  } else if (command.startsWith(MoveCommand)) {
    int steps = command.substring(strlen(MoveCommand) + 1).toInt();
    Move(steps);
    return WithTermination(String("moving"));
  } else if (command.startsWith(MoveToCommand)) {
    int steps = command.substring(strlen(MoveCommand) + 1).toInt();
    Move(steps);
    return WithTermination(String("moving"));
  } else if (command.startsWith(StopCommand)) {
    Stop();
    return WithTermination(String("stoped"));
  } else if (command.startsWith(statusCommand)) {
    String status = isMoving ? Status_Moving : Status_Stoped;
    return WithTermination(String(status));
  } else if (command.startsWith(StopBrakeCommand)) {
    StopBrake();
    return WithTermination(String("brakeStoped"));
  } else if (command.startsWith(OpenCoverCmd)) {
    OpenCover();
    return WithTermination("opened");
  } else if (command.startsWith(CloseCoverCmd)) {
    CloseCover();
    return WithTermination("closed");
  } else if (command.startsWith(SetLightCmd)) {
    int level = command.substring(strlen(SetLightCmd) + 1).toInt();
    SetLight(level);
    return WithTermination("set");
  } else if (command.startsWith(GetSettings)) {
    return WithTermination(CurrentSettings());
  } else if (command.startsWith(SaveSettings)) {
    if (StoreSettings(command.substring(strlen(SaveSettings) + 1))) {
      WriteToEEPROM();
      SetupWiFi();
      SetupServer();
      return WithTermination(String("saved"));
    } else
      return WithTermination(String("failed"));
  } else
    return WithTermination(String("unknown command"));
}

String ProcessMoonliteCommand(String command) {
  int offset = command[1] == '2' ? 1 : 0;
  String cmd = command.substring(1 + offset, 2 + offset);
  if (cmd = "GV")
    return WithTermination(String("10"));
  else if (cmd == "GP")
    return WithTermination(String("0000"));
  else if (cmd == "GN")
    return WithTermination(String("0000"));
  else if (cmd == "GT" || cmd == "GD" || cmd == "GH" || cmd == "GI" || cmd == "GB")
    return WithTermination(String("0000"));

  return WithTermination(String(""));
}

void CheckSerial() {
  if (Serial.available() != 0) {
    while (Serial.available() == 0) {}  //wait for data available
    String teststr = Serial.readString();
    teststr.trim();
    String answer;
    if (teststr[0] == ':')
      answer = ProcessMoonliteCommand(teststr);
    else
      answer = ProcessCommand(teststr);
    Serial.println(answer);
    delay(100);
  }
}

String CurrentSettings() {
  //returns current settings in json
  String result = "{\r\n\"serverEnabled\":\"" + String(ServerEnabled) + "\",\r\n";
  result += "\"createAccessPoint\":\"" + String(CreateAccessPoint) + "\",\r\n";
  result += "\"ssid\":\"" + String(ssid) + "\",\r\n";
  result += "\"pass\":\"" + String(pass) + "\",\r\n";
  result += "\"localIP\":\"" + localIP.toString() + "\",\r\n";
  result += "\"gateway\":\"" + gateway.toString() + "\",\r\n";
  result += "\"subnet\":\"" + subnet.toString() + "\"\r\n}\r\n";
  return result;
}

bool StoreSettings(String json) {
  //returns true if successfully stored
  DebugPrint("Store settings ");
  DebugPrintln(json);
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, json);
  if (error) return false;
  ServerEnabled = doc["serverEnabled"];
  CreateAccessPoint = doc["createAccessPoint"];
  ssid = String(doc["ssid"]);
  pass = String(doc["pass"]);
  localIP = FromString(doc["localIP"]);
  gateway = FromString(doc["gateway"]);
  subnet = FromString(doc["subnet"]);
  return true;
}

IPAddress FromString(String ipString) {
  //returns IPAddress struct from XXX.XXX.XXX.XXX string respresentation
  uint8_t octets[4];
  DebugPrint("Reseived IP address:");
  for (int i = 0, current = 0, next = 0; i < 4; i++) {
    next = ipString.indexOf(".", current);
    octets[i] = ipString.substring(current, next).toInt();
    current = next + 1;
    DebugPrint(String(octets[i]));
    DebugPrint(".");
  }
  DebugPrintln("");
  return IPAddress(octets);
}

//------store settings----------------------------------

void InitSettings()
{
  RestoreFromEEPROM();
  delay(1000); // give the user a chance to press the boot-mode button after reset
  pinMode(BOOT_MODE_BUTTON_PIN, INPUT_PULLUP);
  if (digitalRead(BOOT_MODE_BUTTON_PIN) == LOW) {
        bool exceededThreshold = BlinkWhilePressingModeButton();
        if (exceededThreshold) {
            DebugPrintln("Resetting configuration to default");
            SetDefaults();
            WriteToEEPROM();
        } 
    }
}

bool BlinkWhilePressingModeButton() {
    // Return True if pressed longer than the threshold.
    unsigned long start_time = millis();
    while (digitalRead(BOOT_MODE_BUTTON_PIN) == LOW) {
        delay(50);
        if ((millis() - start_time) > RESET_CONFIG_THRESHOLD) {
            setLedOff();
            return true;
        }
    }
    setLedOff();
    return false;
}

void WriteToEEPROM()
{
  int address = EEPROMStartAddress;  
  EEPROM.write(address++, ServerEnabled);  
  EEPROM.write(address++, CreateAccessPoint);
  address = WriteIPAddress(address, localIP);
  address = WriteIPAddress(address, gateway);
  address = WriteIPAddress(address, subnet);
  address = WriteString(address, ssid);
  address = WriteString(address, pass);
  EEPROM.commit();
}

void RestoreFromEEPROM()
{
  DebugPrintln("Read sttings from EEPROM");
  EEPROM.begin(512);
  int address = EEPROMStartAddress;    
  ServerEnabled = EEPROM.read(address++) > 0;  
  CreateAccessPoint = EEPROM.read(address++) > 0;
  address = ReadIPAddress(address, &localIP);  
  address = ReadIPAddress(address, &gateway);  
  address = ReadIPAddress(address, &subnet);  
  address = ReadString(address, &ssid);  
  address = ReadString(address, &pass);   
}

int WriteIPAddress(int address, IPAddress value)
{  
  //returns next address      
  for(int i = 0; i < 4; i++)
    EEPROM.write(address++, value[i]);        
  return address;  
}

int ReadIPAddress(int address, IPAddress* value)
{  
  //returns next address
  uint8_t t[4];
  for(int i = 0; i < 4; i++)
    t[i] = EEPROM.read(address++);
  *value = IPAddress(t);
  return address; 
}

int WriteString(int address, String value)
{  
  //returns next address  
  EEPROM.write(address++, value.length());
  for(int i = 0; i <= value.length(); i++)
    EEPROM.write(address++, value[i]);
  return address;
}

int ReadString(int address, String* result)
{
  //returns next address  
  int lenght = EEPROM.read(address++);
  char* str = new char[lenght];
  for(int i = 0; i <= lenght; i++)
    str[i] = EEPROM.read(address++);
  *result = String(str);  
  return address;
}

//------Network setup----------------------------------

void SetupWiFi() {
  if (CreateAccessPoint)
    SetupAccessPoint();
  else
    ConnectToWiFi();
}

void ConnectToWiFi() {
  if (!WiFi.config(localIP, gateway, subnet)) {
    DebugPrintln("STA Failed to configure");
    return;
  }
  WiFi.begin(ssid, pass);
  DebugPrintln("Connecting to " + ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    DebugPrint(".");
  }
  DebugPrintln("");
  DebugPrintln("WiFi connected");
  DebugPrint("IP address:\t");
  DebugPrintln(WiFi.localIP().toString());
}

void SetupAccessPoint() {
  if (!ServerEnabled) return;
  WiFi.softAP(ssid, pass);
  DebugPrintln("Establish access point " + ssid);
  WiFi.softAPConfig(localIP, gateway, subnet);
  delay(1000);
  DebugPrint("Access Point \"");
  DebugPrint(ssid);
  DebugPrint("\" started ");
  DebugPrint("IP address:\t");
  DebugPrintln(WiFi.softAPIP().toString());
}

//------Web server implementation----------------------------------

void SetupServer() {
  server.on("/", handle);
  server.onNotFound(handleNotFound);
  server.begin();
}

void CheckServer() {
  server.handleClient();
}

void handle() {
  PerformAction();
  if (server.method() == HTTP_GET)
    server.send(200, "text/html", GetCientPage());
  else if (server.method() == HTTP_POST)
    server.send(200, "text/json", GetPostResponse());
  else
    server.send(405, "text/json", "{ \"error\": \"method not allowed\" }");
}

void handleNotFound() {
  if (server.method() == HTTP_GET)
    server.send(404, "text/html", Get404Page());
  else
    server.send(404, "text/plain", "Error: page not found");
}

String GetPostResponse() {
  //returns http response content
  String result = "{ \"position\":" + String(currentPosition) + ",\r\n";
  result += "\"targetPosition\":" + String(targetPosition) + ",\r\n";
  result += "\"moving\":" + String(isMoving) + "}\r\n";
  return result;
}

void PerformAction() {
  for (int i = 0; i < server.args(); i++) {
    if (server.argName(i) == MoveCommand) {
      int steps = String(server.arg(i)).toInt();
      if (steps != 0) Move(steps);
    } else if (server.argName(i) == MoveToCommand) {
      int position = String(server.arg(i)).toInt();
      MoveTo(position);
    } else if (server.argName(i) == SetMaxPositionCommand) {
      int position = String(server.arg(i)).toInt();
      SetMaxPosition(position);
    } else if (server.argName(i) == StopCommand) {
      int stop = String(server.arg(i)).toInt();
      if (stop > 0) Stop();
    } else if (server.argName(i) == StopBrakeCommand) {
      int stopBrake = String(server.arg(i)).toInt();
      if (stopBrake > 0) StopBrake();
    } else if (server.argName(i) == "opencover") {
      int level = String(server.arg(i)).toInt();
      if(level == 0)
        CloseCover();
      else
        OpenCover();
    } else if (server.argName(i) == "setlight") {
      int level = String(server.arg(i)).toInt();
      SetLight(level);
    }
  }
}

//------Web Pages----------------------------------

String Get404Page() {
  //returns html page for error 404
  String response = R"(<!DOCTYPE html><html>
  <head><meta name="viewport" content="width=device-width, initial-scale=1">
  <style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
  p {padding: 10px 20px; font-size: 30px;}
  </style></head>
  <body><h1>ASCOM NodeMCU WiFi Focuser</h1>
  <p>Page not found</p>  
  </body></html>)";
  return response;
}

String GetCientPage() {
  //returns html page for thin client
  String response = R"(<!DOCTYPE html><html>
  <head><meta name="viewport" content="width=device-width, initial-scale=1">
  <style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
  p {padding: 10px 20px; font-size: 30px;}
  input[type=text]{ font-size: 30px; margin: 2px;}
  input[type=submit]{ background-color: #195B6A; border: none; color: white; padding: 3px 10px;
  text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}
  .button { background-color: #195B6A; border: none; color: white; padding: 5px 10px;
  text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}
  .button2 {background-color: #77878A;}</style></head>
  <body><h1>ASCOM NodeMCU WiFi Focuser</h1>)";
  response += "<p>Focuser position:" + String(currentPosition) + "</p>\r\n";
  response += "<p>Target position:" + String(targetPosition) + "</p>\r\n";
  response += "<p>Moving:" + String(isMoving) + "</p>\r\n";
  response += R"(<p>
  <a href="/?move=-250"><button class="button">&#8666;</button></a>
  <a href="/?move=-25"><button class="button">&#8656;</button></a>  
  <a href="/?stop=1"><button class="button">&#x220E;</button></a>  
  <a href="/?move=25"><button class="button">&#8658;</button></a>
  <a href="/?move=250"><button class="button">&#8667;</button></a>
  </p>
  <form><p><input type="text" id="move" value="100" name="move" size="5"><input style="button" type="submit" value="Move"></p></form>  
  <p><a href="/?stopBrake=1"><button class="button">Stop brake</button></a></p>
  <p>Cover: <a href="/?opencover=1"><button class="button">Open</button></a><a href="/?opencover=0"><button class="button">Close</button></a></p>
  <p>Light: <a href="/?setlight=1"><button class="button">On</button></a><a href="/?setlight=0"><button class="button">Off</button></a></p>
  </body></html>)";
  return response;
}

//------Debug messages----------------------------------
void DebugPrint(String string) {
  if (Debug) Serial.print(string);
}

void DebugPrintln(String string) {
  if (Debug) Serial.println(string);
}

//------LED operations----------------------------------

void setLedOff() {
  digitalWrite(INDICATOR_LED_PIN, INDICATOR_LED_OFF);
}

void setLedOn() {
  digitalWrite(INDICATOR_LED_PIN, INDICATOR_LED_ON);
}
