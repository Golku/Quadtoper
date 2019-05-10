#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

// Constants
const char* ssid = "Jason's Happy Place";
const char* password = "heWhoHeals15!";

int incomingCommand = 0;
DynamicJsonDocument readDoc(1024);
DynamicJsonDocument writeDoc(1024);
bool connected;
int outputType;
bool power;
int speed;
long timer;

// Globals
WebSocketsServer webSocket = WebSocketsServer(80);


void sendJson(){

  writeDoc["outputType"] = "1";
  writeDoc["powerOn"]   = true;
  writeDoc["speed"]   = speed;

  size_t outputSize = 256;
  char* output = new char[outputSize];
  
  serializeJson(writeDoc, output, outputSize);

  uint8_t * dataOut = (uint8_t *)output;

  webSocket.sendTXT(0, dataOut);
}

void receiveJson(uint8_t * data){

  deserializeJson(readDoc, data);

  outputType = readDoc["outputType"];
  power = readDoc["power"];
  Serial.println(outputType);
  Serial.println(power);

  if(outputType == 1){
    Serial.println("Ai");
  }else{
    Serial.println("Nai");
  }

  if(power){
    Serial.println("power on");
  }else{
    Serial.println("power off");
  }

  //sendJson();
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
      Serial.printf("[%u] Text: %s\n", num, payload);
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

  // Start WebSocket server and assign callback
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
}

void loop() {

  // Look for and handle WebSocket data
  webSocket.loop();

  if(connected){
    if(millis() > timer + 1000){
      timer = millis();
      speed = random(100);
      //Serial.println(speed);
      //sendJson();
    }
  }
  
}