#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

// Constants
const char* ssid = "Jason's Happy Place";
const char* password = "heWhoHeals15!";

// const char* ssid = "Jay jay";
// const char* password = "12345678";

DynamicJsonDocument readDoc(1024);
DynamicJsonDocument writeDoc(1024);

const int freq = 50;
const int channel1 = 0;
const int channel2 = 1;
const int channel3 = 2;
const int channel4 = 3;
const int resolution = 11;
const int motorPin1 = 33;
const int motorPin2 = 32;
const int motorPin3 = 25;
const int motorPin4 = 26;
const int multiplier = 101;

bool connected;
long timer;

int commandType;

bool powerOn;
bool pidOn;

int targetVal;

int motorIndex;
int motorVal;
int motorFLVal;
int motorFRVal;
int motorBLVal;
int motorBRVal;

// Globals
WebSocketsServer webSocket = WebSocketsServer(80);

/*
command:
  1 power (powerOn: true/false)
  2 pid   (pidOn: true/false)
  3 motor (motorIndex: 0,1,2,3 - motorVal: 0-100)
*/

void sendJson(){

  writeDoc["outputType"] = "1";
  writeDoc["motorFLVal"]   = motorFLVal;
  writeDoc["motorFRVal"]   = motorFRVal;
  writeDoc["motorBLVal"]   = motorBLVal;
  writeDoc["motorBRVal"]   = motorBRVal;

  size_t outputSize = 256;
  char* output = new char[outputSize];
  
  serializeJson(writeDoc, output, outputSize);

  uint8_t * dataOut = (uint8_t *)output;

  webSocket.sendTXT(0, dataOut);
}

void pidConfig(){
  targetVal = motorVal;
  Serial.print("targetVal: ");
  Serial.print(targetVal);
  
}

void changeMotorVal(){
  switch (motorIndex){
    case 0:
      motorFLVal = motorVal;
      Serial.print("motorFLVal: ");
      Serial.print(motorFLVal);
      ledcWrite(channel1, (motorFLVal+multiplier));
      break;
    case 1:
      motorFRVal = motorVal;
      Serial.print("motorFRVal: ");
      Serial.print(motorFRVal);
      ledcWrite(channel2, (motorFRVal+multiplier));
      break;
    case 2:
      motorBLVal = motorVal;
      Serial.print("motorBLVal: ");
      Serial.print(motorBLVal);
      ledcWrite(channel3, (motorBLVal+multiplier));
      break;  
    case 3:
      motorBRVal = motorVal;
      Serial.print("motorBRVal: ");
      Serial.print(motorBRVal);
      ledcWrite(channel4, (motorBRVal+multiplier));
      break;
    case 4:

      if(pidOn){
        pidConfig();
      }else{
        motorFLVal = motorVal;
        motorFRVal = motorVal;
        motorBLVal = motorVal;
        motorBRVal = motorVal;

        ledcWrite(channel1, (motorFLVal+multiplier));
        ledcWrite(channel2, (motorFRVal+multiplier));
        ledcWrite(channel3, (motorBLVal+multiplier));
        ledcWrite(channel4, (motorBLVal+multiplier));

        Serial.print("All motors vals: ");
        Serial.print(motorVal);
      }

      break;  
  }
}

void receiveJson(uint8_t * data){

  deserializeJson(readDoc, data);
  
  commandType = readDoc["commandType"];

  switch (commandType){
    case 1:
      powerOn = readDoc["powerOn"];
      Serial.print("Power: ");
      Serial.print(powerOn);
      break;
    case 2:
      pidOn = readDoc["pidOn"];
      Serial.print("pid: ");
      Serial.print(pidOn);
      break;
    case 3:
      motorIndex = readDoc["motorIndex"];
      motorVal = readDoc["motorVal"];
      changeMotorVal();
      break;
  }
  
  Serial.println(" ");
}

// Called when receiving any WebSocket message
void onWebSocketEvent(uint8_t num,
                      WStype_t type,
                      uint8_t * payload,
                      size_t length) {

  // Figure out the type of WebSocket event
  switch (type) {

    // Client has disconnected
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      connected = false;
      break;

    // New client has connected
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connection from ", num);
        Serial.println(ip.toString());
        connected = true;
        timer = millis();
      }
      break;

    // Echo text message back to client
    case WStype_TEXT:
      //Serial.printf("[%u] Text: %s\n", num, payload);
      receiveJson(payload);
      break;

    // For everything else: do nothing
    case WStype_BIN:
      Serial.printf("BIN");
      break;
    case WStype_ERROR:
      Serial.printf("error");
      break;
    case WStype_FRAGMENT_TEXT_START:
      Serial.printf("TEXT_start");
      break;
    case WStype_FRAGMENT_BIN_START:
      Serial.printf("BIN_start");
      break;
    case WStype_FRAGMENT:
      Serial.printf("feagment");
      break;
    case WStype_FRAGMENT_FIN:
      Serial.printf("fin");
      break;
    default:
      Serial.printf("default");
      break;
  }
}

void setup() {

  // Start Serial port
  Serial.begin(115200);

  // Connect to access point
  Serial.println("Connecting");
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }

  // Print our IP address
  Serial.println("Connected!");
  Serial.print("My IP address: ");
  Serial.println(WiFi.localIP());

  ledcSetup(channel1, freq, resolution);
  ledcSetup(channel2, freq, resolution);
  ledcSetup(channel3, freq, resolution);
  ledcSetup(channel4, freq, resolution);
  ledcAttachPin(motorPin1, channel1);
  ledcAttachPin(motorPin2, channel2);
  ledcAttachPin(motorPin3, channel3);
  ledcAttachPin(motorPin4, channel4);

  motorFLVal = 101;
  motorFRVal = 101;
  motorBLVal = 101;
  motorBRVal = 101;

  ledcWrite(channel1, motorFLVal);
  ledcWrite(channel2, motorFRVal);
  ledcWrite(channel3, motorBLVal);
  ledcWrite(channel4, motorBRVal);

  powerOn = false;
  pidOn = false;

  targetVal = 0;

  // Start WebSocket server and assign callback
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
}

void loop() {

  // Look for and handle WebSocket data
  webSocket.loop();

  // if(powerOn){
  //   if(pidOn){
  //     if(millis() > timer + 500){
  //       timer = millis();
  //       motorFLVal = random(100);
  //       motorFRVal = random(100);
  //       motorBLVal = random(100);
  //       motorBRVal = random(100);
  //       sendJson();
  //     }
  //   }
  // }

}