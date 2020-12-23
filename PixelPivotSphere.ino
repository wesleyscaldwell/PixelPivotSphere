#include <EEPROM.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#import "MainPageHtml.h"


//#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
//#include <WiFi.h>
//#include <WiFiServer.h>


  const char* ssid = "MyWifi";  //"CGOffice"; //
  const char* password = "3132143880";
  //ESP8266WebServer server(80);
  //WiFiServer server(80);
  WebServer server(80);


  const int BUTTON_PIN = 0;
  const int LED_PIN = 5;

  //const int led = 13;
  
  int eepromActionsStart = 0;
  int eepromActionsLen = 200;
  String PhotoSphereActions = "";


  String rotationInfo = "";
  boolean Direction = true;


  //uint8_t ConfigPin1 = 5; //D1;
  //uint8_t ConfigPin2 = 4; //D2;
  //uint8_t ConfigPin3 = 0; //D3;
  //uint8_t ConfigPin4 = 2; //D4;

  //uint8_t ConfigPin5 = 14; //D5;
  //uint8_t ConfigPin6 = 12; //D6;
  //uint8_t ConfigPin7 = 13; //D7;
  //uint8_t ConfigPin8 = 15; //D8;

  //uint8_t ConfigPin5Rev = 10; //D5;
  //uint8_t ConfigPin6Rev = 16; //D6;
  //uint8_t ConfigPin7Rev = 3; //D7;
  //uint8_t ConfigPin8Rev = 1; //D8;  


  uint8_t ConfigPin1 = 19; //D1;
  uint8_t ConfigPin2 = 23; //D2;
  uint8_t ConfigPin3 = 22; //D3;
  uint8_t ConfigPin4 = 18; //D4;

  uint8_t ConfigPin5 = 38; //D5;
  uint8_t ConfigPin6 = 34; //D6;
  uint8_t ConfigPin7 = 35; //D7;
  uint8_t ConfigPin8 = 39; //D8;

  uint8_t ConfigPin5Rev = 14; //D5;
  uint8_t ConfigPin6Rev = 27; //D6;
  uint8_t ConfigPin7Rev = 13; //D7;
  uint8_t ConfigPin8Rev = 12; //D8;  


  
  const int dirPin = ConfigPin4; 

  
  uint8_t IN1 = ConfigPin1;
  uint8_t IN2 = ConfigPin2;
  uint8_t IN3 = ConfigPin3;
  uint8_t IN4 = ConfigPin4;

  uint8_t IN5 = ConfigPin5;
  uint8_t IN6 = ConfigPin6;
  uint8_t IN7 = ConfigPin7;
  uint8_t IN8 = ConfigPin8;

  uint8_t IN5Rev = ConfigPin5Rev;
  uint8_t IN6Rev = ConfigPin6Rev;
  uint8_t IN7Rev = ConfigPin7Rev;
  uint8_t IN8Rev = ConfigPin8Rev;
  
  int Step28MotorSteps = 0;

void setup(void) {

  EEPROM.begin(512);
  Serial.begin(115200);

  delay(1000);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  
  pinMode(ConfigPin1, OUTPUT);    
  pinMode(ConfigPin2, OUTPUT);
  pinMode(ConfigPin3, OUTPUT);
  pinMode(ConfigPin4, OUTPUT);

  pinMode(ConfigPin5, OUTPUT);    
  pinMode(ConfigPin6, OUTPUT);
  pinMode(ConfigPin7, OUTPUT);
  pinMode(ConfigPin8, OUTPUT);

  pinMode(ConfigPin5Rev, OUTPUT);    
  pinMode(ConfigPin6Rev, OUTPUT);
  pinMode(ConfigPin7Rev, OUTPUT);
  pinMode(ConfigPin8Rev, OUTPUT);
  

  delay(1500);

  digitalWrite(LED_PIN, LOW); // LED off

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  delay(500);
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) //if (MDNS.begin("esp8266")) 
  {
    Serial.println("MDNS responder started");
  }
  
  
  //WiFi.persistent(false);
  //WiFi.disconnect(true);
  //WiFi.mode(WIFI_STA);
  //WiFi.begin(ssid, password);
  //Serial.println("");
  //delay(500);
  //Serial.println("");
  //Serial.print("Connected to ");
  //Serial.println(ssid);
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());

  PhotoSphereActions = EepromGet(eepromActionsStart,eepromActionsLen);

  server.on("/", handleRoot);
  server.on("/inline", []() {    server.send(200, "text/plain", "this works as well");  });
  server.on("/getMotorStatus", []() {    server.send(200, "text/plain", "Motor Status Result");  });
  server.on("/test", handleRoot);
  server.on("/paramtest", []() {    server.send(200, "text/plain", server.arg("PARAM1"));  });
  server.on("/SetMotorLocation", []() {   String msg = SetMotorTemp();  server.send(200, "text/plain", msg);  });
  server.on("/SetPhotoActions", []() { PhotoSphereActions = server.arg("ACTIONS");  PhotoSphereActions.replace("x", "+");   EepromSave(eepromActionsStart, eepromActionsLen, PhotoSphereActions);  server.send(200, "text/plain", PhotoSphereActions);  });

  server.on("/RunPhotoSphere", []() {  ProcessPictureSphere();  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");


}




void loop() {
  server.handleClient();
}

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
  String s = MAIN_page; //Read HTML contents

  String tempActions = PhotoSphereActions;
  tempActions.replace("+", "x");
  tempActions.trim();
  s.replace("%PhotoActions%", tempActions); 
  server.send(200, "text/html", s); //Send web page
}


void ProcessPictureSphere()
{
  String msg = "";
  Serial.println("starting sphere");
  msg += "starting sphere - 1 \n\n";

  //String eventList = "";
  String eventList = PhotoSphereActions;
  String eventListArray[99];
  int ActionCount = SplitString(eventList, eventListArray);

  Serial.println("Completed String Split");
  for (int i=0; i <= ActionCount; i++)   //sizeof(eventListArray)
  {
    Serial.print("Running Loop: ");
    Serial.println(i);
    String x = eventListArray[i];    
    x.trim();
    
    Serial.println(x);
    msg += "Line Value:  \n\n";
    msg += x;
    msg += "\n\n";

    if(x.length() > 0)
    {
      String rotationAxis = x.substring(0,1);
      String rotationClock = x.substring(1,2);  
      String rotationSteps = x.substring(2);  
      Serial.println("Step Info: ");
      Serial.print("Up / Down: ");
      Serial.print(rotationAxis);
      Serial.print(" | Rotation: ");
      Serial.print(rotationClock);
      Serial.print(" | Step: ");
      Serial.println(rotationSteps);
      
      msg += "Step Info:  \n";
      msg += "Up / Down:  ";
      msg += rotationAxis + " ";
      msg += " | Rotation:  ";
      msg += rotationClock + " ";
      msg += " | Step:  ";
      msg += rotationSteps + " ";

      bool clockWise = rotationClock == "-";
      int steps = rotationSteps.toInt();
      Direction = clockWise;    //!Direction;  
      
      if(rotationAxis == "v")
      {
        ProcessStep28MotorAction(steps, 10, false, 2);
      }
      if(rotationAxis == "h")
      {
        ProcessStep28MotorAction(steps, 10, false, 1);
      }
      delay(2500);
    }
  }

  TurnOffMotor(1);
  TurnOffMotor(2);
  
  msg.replace("\n", "<br />");
  server.send(200, "text/plain", msg);
}

int SplitString(String oneLine, String sa[])
{
  oneLine.trim();
  Serial.println("");
  Serial.println(oneLine);
  int ArrayCount = 0;
  for (int i=0; i < oneLine.length(); i++)
  {
    if(oneLine.charAt(i) == ';')
    {
      ArrayCount++;  
    }
  }
  Serial.print("Array Count: ");
  Serial.println(ArrayCount);
  
  //sa[ArrayCount];
  int r=0, t=0;
  int LineLength = oneLine.length();
  for (int i=0; i < oneLine.length(); i++)
  { 
    Serial.print("t: ");
    Serial.print(t);
    Serial.print("i: ");
    Serial.print(i);
    Serial.print("  r: ");
    Serial.print(r);
    Serial.print(" | Letter: ");
    Serial.print(oneLine.charAt(i));
    Serial.print(" | Word: ");
    Serial.println(oneLine.substring(r, i));
    
    if(oneLine.charAt(i) == ';') 
    { 
      String lineValue = oneLine.substring(r, i); 
      lineValue.trim();
      sa[t] = lineValue;
      r=(i+1); 
      t++; 
    }
    else
    {
        int checkVal = i + 1;
        if(checkVal == LineLength)
        {
          String lineValue = oneLine.substring(r, checkVal); 
          lineValue.trim();
          sa[t] = lineValue;
          r=(i+1); 
          t++; 
        }
    }
  }  
  return ArrayCount;
}


String SetMotorTemp()
{
  String msg = "";
  Serial.println("starting");
  
  rotationInfo = "";
  if (server.hasArg("STEPS") && server.hasArg("CLOCK")) 
  {
    msg += "Params Found - 1 \n\n";
    int motorNumber = 1;
    int steps;
    int delayValue = 100;
    bool turnOffMotor = false;
    bool clockWise;
    
    steps = server.arg("STEPS").toInt();
    clockWise = server.arg("CLOCK") == "1";
    Direction = clockWise;    //!Direction;  

    if (server.hasArg("TIMEDELAY")){
      delayValue = server.arg("TIMEDELAY").toInt();
    }

    if(server.hasArg("TURNOFFMOTOR")){
      msg += "Motor Param Found \n\n";
      turnOffMotor = server.arg("TURNOFFMOTOR") == "1";
    }

    if (server.hasArg("MOTORNUMBER")){
      motorNumber = server.arg("MOTORNUMBER").toInt();
    }
    msg += "Motor #: " + String(motorNumber) + " \n\n"; 
    msg += "Value A: " + String(clockWise) + " \n\n"; 
    msg += "Value B: " + String(steps) + " \n\n"; 
    msg += "Value D: " + String(Direction) + " \n\n"; 

    ProcessStep28MotorAction(steps, delayValue, turnOffMotor, motorNumber);

    
    msg += "Stage - 2 \n\n"; 
    rotationInfo = rotationInfo + "Starting Rotation Set" + "\n";

    
    
    msg += rotationInfo; 
  }
  else
  {
    msg += " not all params are found : STEPS /CLOCK  \n\n";
  }
  Serial.println("finished");
  Serial.println(msg);
  msg.replace("\n", "<br />");
  return msg;
}




void ProcessStep28MotorAction(int steps, int delayValue, bool turnOffMotor, int motorNum)
{

  if(motorNum == 1)
  {
    TurnMotorLeftRight(steps, delayValue);
  }
  else if(motorNum == 2)
  {
    TurnMotorUpDown(steps, delayValue);
  }

  if(turnOffMotor){
    rotationInfo = rotationInfo + "Done - Turning Off motors" + "\n";
    TurnOffMotor(motorNum);
  }
}


















void TurnOffMotor(int motorNum)
{
  if(motorNum == 1)
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
  else if(motorNum == 2)
  {
    digitalWrite(IN5, LOW);
    digitalWrite(IN6, LOW);
    digitalWrite(IN7, LOW);
    digitalWrite(IN8, LOW);
    digitalWrite(IN5Rev, LOW);
    digitalWrite(IN6Rev, LOW);
    digitalWrite(IN7Rev, LOW);
    digitalWrite(IN8Rev, LOW);
  }
}

void TurnMotorLeftRight(int xw, int delayValue) 
{
  int valueINa = IN1;
  int valueINb = IN2;
  int valueINc = IN3;
  int valueINd = IN4;
    
  for (int x = 0; x < xw; x++) 
  {
    switch (Step28MotorSteps) 
    {
      case 0:
        Serial.println("Step 0");
        digitalWrite(valueINa, LOW);
        digitalWrite(valueINb, LOW);
        digitalWrite(valueINc, LOW);
        digitalWrite(valueINd, HIGH);
        break;
      case 1:
        Serial.println("Step 1");
        digitalWrite(valueINa, LOW);
        digitalWrite(valueINb, LOW);
        digitalWrite(valueINc, HIGH);
        digitalWrite(valueINd, HIGH);
        break;
      case 2:
        Serial.println("Step 2");
        digitalWrite(valueINa, LOW);
        digitalWrite(valueINb, LOW);
        digitalWrite(valueINc, HIGH);
        digitalWrite(valueINd, LOW);
        break;
      case 3:
        Serial.println("Step 3");
        digitalWrite(valueINa, LOW);
        digitalWrite(valueINb, HIGH);
        digitalWrite(valueINc, HIGH);
        digitalWrite(valueINd, LOW);
        break;
      case 4:
        Serial.println("Step 4");
        digitalWrite(valueINa, LOW);
        digitalWrite(valueINb, HIGH);
        digitalWrite(valueINc, LOW);
        digitalWrite(valueINd, LOW);
        break;
      case 5:
        Serial.println("Step 5");
        digitalWrite(valueINa, HIGH);
        digitalWrite(valueINb, HIGH);
        digitalWrite(valueINc, LOW);
        digitalWrite(valueINd, LOW);
        break;
      case 6:
        Serial.println("Step 6");
        digitalWrite(valueINa, HIGH);
        digitalWrite(valueINb, LOW);
        digitalWrite(valueINc, LOW);
        digitalWrite(valueINd, LOW);
        break;
      case 7:
        Serial.println("Step 7");
        digitalWrite(valueINa, HIGH);
        digitalWrite(valueINb, LOW);
        digitalWrite(valueINc, LOW);
        digitalWrite(valueINd, HIGH);
        break;
      default:
        Serial.println("Step Default");
        digitalWrite(valueINa, LOW);
        digitalWrite(valueINb, LOW);
        digitalWrite(valueINc, LOW);
        digitalWrite(valueINd, LOW);
        break;
    }

    delay(delayValue);
    if (Direction == 1) {
      Step28MotorSteps++;
    }
    if (Direction == 0) {
      Step28MotorSteps--;
    }
    if (Step28MotorSteps > 7) {
      Step28MotorSteps = 0;
    }
    if (Step28MotorSteps < 0) {
      Step28MotorSteps = 7;
    }
    
  }
}


void TurnMotorUpDown(int xw, int delayValue) 
{
  int valueINa = IN5;
  int valueINb = IN6;
  int valueINc = IN7;
  int valueINd = IN8;
  int valueINaRev = IN5Rev;
  int valueINbRev = IN6Rev;
  int valueINcRev = IN7Rev;
  int valueINdRev = IN8Rev;
  
  for (int x = 0; x < xw; x++) 
  {
    switch (Step28MotorSteps) 
    {
      case 0:
        Serial.println("Step 0");
        digitalWrite(valueINaRev, HIGH);
        digitalWrite(valueINbRev, LOW);
        digitalWrite(valueINcRev, LOW);
        digitalWrite(valueINdRev, HIGH);
    
        digitalWrite(valueINa, LOW);
        digitalWrite(valueINb, LOW);
        digitalWrite(valueINc, LOW);
        digitalWrite(valueINd, HIGH);
        break;
      case 1:
        Serial.println("Step 1");
        digitalWrite(valueINaRev, HIGH);
        digitalWrite(valueINbRev, LOW);
        digitalWrite(valueINcRev, LOW);
        digitalWrite(valueINdRev, LOW);
    
        digitalWrite(valueINa, LOW);
        digitalWrite(valueINb, LOW);
        digitalWrite(valueINc, HIGH);
        digitalWrite(valueINd, HIGH);
        break;
      case 2:
        Serial.println("Step 2");
        digitalWrite(valueINaRev, HIGH);
        digitalWrite(valueINbRev, HIGH);
        digitalWrite(valueINcRev, LOW);
        digitalWrite(valueINdRev, LOW);
    
        digitalWrite(valueINa, LOW);
        digitalWrite(valueINb, LOW);
        digitalWrite(valueINc, HIGH);
        digitalWrite(valueINd, LOW);
        break;
      case 3:
        Serial.println("Step 3");
        digitalWrite(valueINaRev, LOW);
        digitalWrite(valueINbRev, HIGH);
        digitalWrite(valueINcRev, LOW);
        digitalWrite(valueINdRev, LOW);
    
        digitalWrite(valueINa, LOW);
        digitalWrite(valueINb, HIGH);
        digitalWrite(valueINc, HIGH);
        digitalWrite(valueINd, LOW);
        break;
      case 4:
        Serial.println("Step 4");
        digitalWrite(valueINaRev, LOW);
        digitalWrite(valueINbRev, HIGH);
        digitalWrite(valueINcRev, HIGH);
        digitalWrite(valueINdRev, LOW);
    
        digitalWrite(valueINa, LOW);
        digitalWrite(valueINb, HIGH);
        digitalWrite(valueINc, LOW);
        digitalWrite(valueINd, LOW);
        break;
      case 5:
        Serial.println("Step 5");
        digitalWrite(valueINaRev, LOW);
        digitalWrite(valueINbRev, LOW);
        digitalWrite(valueINcRev, HIGH);
        digitalWrite(valueINdRev, LOW);
    
        digitalWrite(valueINa, HIGH);
        digitalWrite(valueINb, HIGH);
        digitalWrite(valueINc, LOW);
        digitalWrite(valueINd, LOW);
        break;
      case 6:
        Serial.println("Step 6");
        digitalWrite(valueINaRev, LOW);
        digitalWrite(valueINbRev, LOW);
        digitalWrite(valueINcRev, HIGH);
        digitalWrite(valueINdRev, HIGH);
    
        digitalWrite(valueINa, HIGH);
        digitalWrite(valueINb, LOW);
        digitalWrite(valueINc, LOW);
        digitalWrite(valueINd, LOW);
        break;
      case 7:
        Serial.println("Step 7");
        digitalWrite(valueINaRev, LOW);
        digitalWrite(valueINbRev, LOW);
        digitalWrite(valueINcRev, LOW);
        digitalWrite(valueINdRev, HIGH);
    
        digitalWrite(valueINa, HIGH);
        digitalWrite(valueINb, LOW);
        digitalWrite(valueINc, LOW);
        digitalWrite(valueINd, HIGH);
        break;
      default:
        Serial.println("Step Default");
        digitalWrite(valueINaRev, LOW);
        digitalWrite(valueINbRev, LOW);
        digitalWrite(valueINcRev, LOW);
        digitalWrite(valueINdRev, LOW);
    
        digitalWrite(valueINa, LOW);
        digitalWrite(valueINb, LOW);
        digitalWrite(valueINc, LOW);
        digitalWrite(valueINd, LOW);
        break;
    }

    delay(delayValue);
    if (Direction == 1) {
      Step28MotorSteps++;
    }
    if (Direction == 0) {
      Step28MotorSteps--;
    }
    if (Step28MotorSteps > 7) {
      Step28MotorSteps = 0;
    }
    if (Step28MotorSteps < 0) {
      Step28MotorSteps = 7;
    }
    
  }
}



void handleNotFound() 
{
  //digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += ""; //server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  //digitalWrite(led, 0);
}



String EepromGet(int loc, int locLen){
  
  int maxLen = (loc + locLen);
  char rtnValue[locLen];
  String rtnString = "";

  for (int i = 0; i < locLen; i++) 
  {     
    char newVal;
    rtnValue[i] = newVal;   
  }
  
  for (int i = loc; i < maxLen; i++) {
    int col = i - loc;
    rtnValue[col] = EEPROM.read(i);
    rtnString += rtnValue[col];
  }
  return rtnString;
}

void EepromSave(int loc, int ValueLength, String ValueString)
{
  for (int i = loc; i < (loc + ValueLength); i++) {
    char emptyValue = ' ';
    EEPROM.write(i, emptyValue);
  }
  for (int i = loc; i < (loc + ValueString.length()); i++) {
    int charLoc = (i - loc);
    char val = ValueString.charAt(charLoc);
    EEPROM.write(i, val);
  }
  EEPROM.commit();
}
