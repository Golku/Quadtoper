#include <WiFi.h>
#include <WebSocketsServer.h>
#include <serverController.h>
#include <controller.h>

// Constants
// const char* ssid = "Jason's Happy Place";
// const char* password = "heWhoHeals15!";

const char* ssid = "Jay jay";
const char* password = "12345678";

// const char* ssid = "JunoLog";
// const char* password = "Log-Juno";

// Globals
WebSocketsServer webSocket = WebSocketsServer(80);

bool connected;
long timer;

serverclass::serverclass(){

}

bool serverclass::isConnected(){
  return connected;
}

void receiveJson (uint8_t * payload){
  controller.dataIn(payload);
}

void serverclass::sendJson(uint8_t * dataOut){
  webSocket.sendTXT(0, dataOut);
}

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

void serverclass::connect(){
  
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

void serverclass::socketLoop(){

  // Look for and handle WebSocket data
  webSocket.loop();
}

serverclass server = serverclass();