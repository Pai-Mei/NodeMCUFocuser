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

//------Debug messages----------------------------------
void DebugPrint(String string) {
  if (Debug) Serial.print(string);
}

void DebugPrintln(String string) {
  if (Debug) Serial.println(string);
}