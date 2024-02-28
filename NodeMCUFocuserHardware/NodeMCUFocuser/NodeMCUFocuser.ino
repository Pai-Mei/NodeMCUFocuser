//Includes the Arduino Stepper Library
#include <AccelStepper.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

#define Debug                   0 // set to 1 to see debug messages over serial
#define EEPROMStartAddress    0x0f

#define BOOT_MODE_BUTTON_PIN    0
#define INDICATOR_LED_PIN       2
#define INDICATOR_LED_ON        0
#define INDICATOR_LED_OFF       1
#define RESET_CONFIG_THRESHOLD  5000

//global settings stored in EEPROM
bool ServerEnabled;
bool CreateAccessPoint;
String ssid;
String pass;
IPAddress localIP;
IPAddress gateway;
IPAddress subnet;

// Define step constant (4 - full-step, 8 - half-step)
#define MotorInterfaceType  8
// Pins entered in sequence IN1-IN3-IN4-IN2 for proper step sequence
AccelStepper myStepper(MotorInterfaceType, 14, 13, 15, 12);

//global variables for stepper
int currentPosition = 0;
int targetPosition = 0;
bool isMoving = false;

//global variables for web server timeout
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 30000;

ESP8266WebServer server(80);

void SetDefaults()
{
  ServerEnabled = true;
  CreateAccessPoint = true;
  // replace with your wifi ssid and wpa2 key
  ssid = String("ASCOM NodeMCU WiFi Focuser"); 
  pass = String("1234567890");
  //set your IP configuration
  localIP = IPAddress(192, 168, 0, 1);
  gateway = IPAddress(192, 168, 0, 1);
  subnet = IPAddress(255, 255, 0, 0);
}

void setup() {
  Serial.begin(9600);
  InitSettings();
  SetupStepper();
  SetupWiFi();
  SetupServer();
}

void loop() {
  DoStep();
  CheckSerial();
  CheckServer();
}

//------Stepper----------------------------------

void SetupStepper() {
  myStepper.setMaxSpeed(200.0);
  myStepper.setAcceleration(20.0);
  myStepper.setSpeed(20);
  targetPosition = currentPosition = myStepper.currentPosition();
}

void DoStep() {
  if (isMoving) {
    isMoving = myStepper.run();
    currentPosition = myStepper.currentPosition();
    if (!isMoving) Serial.println(WithTermination("stoped"));
  }
  if (targetPosition != currentPosition) {
    myStepper.moveTo(targetPosition);
    isMoving = true;
  }
}

void SetMaxPosition(int steps) {
  targetPosition = currentPosition = steps / 2;
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

//------Serial communication----------------------------------

#define StopCommand "stop"
#define MoveCommand "move"
#define MoveToCommand "moveto"
#define SetMaxPositionCommand "set_max_position"
#define SaveSettings "save_settings"
#define GetSettings "get_settings"
#define Termination "#"

String WithTermination(String message) { return message + Termination; }

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
  } else if (command.startsWith(GetSettings)) {
    return WithTermination(CurrentSettings());
  } else if (command.startsWith(SaveSettings)) {
    if(StoreSettings(command.substring(strlen(SaveSettings) + 1)))
    {
      WriteToEEPROM();
      SetupWiFi();
      SetupServer();
      return WithTermination(String("saved"));      
    }
    else
      return WithTermination(String("failed"));
  } else
    return WithTermination(String("unknown command"));
}

void CheckSerial() {
  if (Serial.available() != 0) {
    while (Serial.available() == 0) {}  //wait for data available
    String teststr = Serial.readString();
    teststr.trim();
    String answer = ProcessCommand(teststr);
    Serial.println(answer);
    delay(100);
  }
}

String CurrentSettings()
{
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

bool StoreSettings(String json)
{
  //returns true if successfully stored
  DebugPrint("Store settings ");
  DebugPrintln(json);
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, json);
  if(error) return false;
  ServerEnabled = doc["serverEnabled"];
  CreateAccessPoint = doc["createAccessPoint"];
  ssid = String(doc["ssid"]);
  pass = String(doc["pass"]);
  localIP = FromString(doc["localIP"]);
  gateway = FromString(doc["gateway"]);
  subnet = FromString(doc["subnet"]);
  return true;
}

IPAddress FromString(String ipString)
{
  //returns IPAddress struct from XXX.XXX.XXX.XXX string respresentation
  uint8_t octets[4];  
  DebugPrint("Reseived IP address:");
  for(int i = 0, current = 0, next = 0; i < 4; i++)
  {
    next = ipString.indexOf(".",  current);
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

void setLedOff()
{
  digitalWrite(INDICATOR_LED_PIN, INDICATOR_LED_OFF);
}

void setLedOn()
{
  digitalWrite(INDICATOR_LED_PIN, INDICATOR_LED_ON);
}