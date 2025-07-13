
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

//------LED operations----------------------------------

void setLedOff() {
  digitalWrite(INDICATOR_LED_PIN, INDICATOR_LED_OFF);
}

void setLedOn() {
  digitalWrite(INDICATOR_LED_PIN, INDICATOR_LED_ON);
}

