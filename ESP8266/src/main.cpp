#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

// Constants
const char* ssid = "Jason's Happy Place";
const char* password = "heWhoHeals15!";

int incomingCommand = 0;
DynamicJsonDocument readDoc(1024);
DynamicJsonDocument writeDoc(1024);
int status;

// Globals
WebSocketsServer webSocket = WebSocketsServer(80);

void receiveJson(uint8_t * data){

  deserializeJson(readDoc, data);

  status = readDoc["status"];
  Serial.println(status);

  if(status == 1){
    Serial.println("Ai");
  }else{
    Serial.println("Nai");
  }
}

uint8_t * sendJson(){

  writeDoc["sensor"] = "gps";
  writeDoc["time"]   = 1351824120;

  JsonArray data = writeDoc.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);

  char* output;
  size_t outputSize = 256;

  serializeJson(writeDoc, output, outputSize);

  uint8_t * h = (uint8_t *)atoi(output);

  return h;
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
      break;

    // New client has connected
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connection from ", num);
        Serial.println(ip.toString());
      }
      break;

    // Echo text message back to client
    case WStype_TEXT:
      Serial.printf("[%u] Text: %s\n", num, payload);

      webSocket.sendTXT(num, sendJson());
      break;

    // For everything else: do nothing
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    default:
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
}