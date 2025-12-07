#include "pins_arduino.h"
#include <ArduinoJson.h>
#include <FS.h>
#include <LittleFS.h>
#include <esp-fs-webserver.h> // https://github.com/cotestatnt/esp-fs-webserver

#define FILESYSTEM LittleFS

#define LED D2

// Var labels (in /setup webpage)
#define LED_LABEL "The LED pin number"
#define BOOL_LABEL "A bool variable"
#define LONG_LABEL "A long variable"
#define FLOAT_LABEL "A float variable"
#define STRING_LABEL "A String variable"
#define DROPDOWN_LABEL "A dropdown listbox"

// Test "options" values
uint8_t ledPin = LED_BUILTIN;
bool boolVar = true;
uint32_t longVar = 1234567890;
float floatVar = 15.5F;
String stringVar = "Test option String";

// In order to show a dropdown list box in /setup page
// we need a list ef values and a variable to store the selected option
#define LIST_SIZE 7
const char *dropdownList[LIST_SIZE] = {"Monday",   "Tuesday", "Wednesday",
                                       "Thursday", "Friday",  "Saturday",
                                       "Sunday"};
String dropdownSelected;

FSWebServer myWebServer(FILESYSTEM, 80);
WebSocketsServer webSocket = WebSocketsServer(81);

void updateGpioList() {
  Serial.println(F("Updating GPIO list\n"));
  if (myWebServer.client()) {
    int read_pin = digitalRead(D8); // dummy read to avoid exception
    myWebServer.send(200, "text/plain", (read_pin ? "HIGHHHH" : "LOWWWW"));
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t len) {
  switch (type) {
  case WStype_DISCONNECTED:
    Serial.printf("[%u] Disconnected!\n", num);
    break;
  case WStype_CONNECTED:
    Serial.printf("[%u] Client connected\n", num);
    break;
  case WStype_TEXT: {
    Serial.printf("[%u] get Text: %s\n", num, payload);
    // Copy content of payload to a temporary char array
    String json = (char *)payload;
    parseMessage(json);
    break;
  }
  default:
    break;
  }
}

void parseMessage(String &json) {
#if ARDUINOJSON_VERSION_MAJOR > 6
  JsonDocument doc;
#else
  DynamicJsonDocument doc(512);
#endif
  DeserializationError error = deserializeJson(doc, json);

  if (!error) {
    // If this is a "writeOut" command, set the pin level to value
    const char *cmd = doc["cmd"];
    if (strcmp(cmd, "writeOut") == 0) {
      int pin = doc["pin"];
      int level = doc["level"];
      Serial.printf("Set pin %d to %d\n", pin, level);
    }
  }
  Serial.print(F("deserializeJson() failed: "));
  Serial.println(error.f_str());
}

#ifdef ESP32
void getFsInfo(fsInfo_t *fsInfo) {
  fsInfo->fsName = "LittleFS";
  fsInfo->totalBytes = LittleFS.totalBytes();
  fsInfo->usedBytes = LittleFS.usedBytes();
}
#else
void getFsInfo(fsInfo_t *fsInfo) { fsInfo->fsName = "LittleFS"; }
#endif

void setup() {
  Serial.begin(115200);

  // FILESYSTEM initialization
  if (!FILESYSTEM.begin()) {
    Serial.println("ERROR on mounting filesystem.");
    // FILESYSTEM.format();
    ESP.restart();
  }

  // Try to connect to stored SSID, start AP if fails after timeout
  myWebServer.setAP("ESP_AP", "123456789");
  IPAddress myIP = myWebServer.startWiFi(15000);

  if (WiFi.status() == WL_CONNECTED) {
    myIP = WiFi.localIP();
    Serial.print(F("\nConnected! IP address: "));
    Serial.println(myIP);
  }

  // Add custom page handlers
  myWebServer.on("/getGpioList", HTTP_GET, updateGpioList);

  // set /setup and /edit page authentication
  // myWebServer.setAuthentication("admin", "admin");

  // enable file system based code editor at /edit page
  myWebServer.enableFsCodeEditor(getFsInfo);

  myWebServer.addOptionBox("My Options");
  myWebServer.addOption(BOOL_LABEL, boolVar);
  myWebServer.addOption(LED_LABEL, ledPin);
  myWebServer.addOption(LONG_LABEL, longVar);
  myWebServer.addOption(FLOAT_LABEL, floatVar, 1.0, 100.0, 0.01);
  myWebServer.addOption(STRING_LABEL, stringVar);
  myWebServer.addDropdownList(DROPDOWN_LABEL, dropdownList, LIST_SIZE);
  // myWebServer.addHTML(save_btn_htm, "buttons");
  // myWebServer.addJavascript(button_script, "script");

  // Start webserver
  myWebServer.begin();
  Serial.println(F("ESP Web Server started.\n IP address: "));
  Serial.println(myIP);
  Serial.println(F("Open /setup page to configure optional parameters"));
  Serial.println(F("Open /edit page to view and edit files"));
  Serial.println(
      F("Open /update page to upload firmware and filesystem updates"));

  // Start WebSocket server on port 81
  Serial.println("Start WebSocket server");
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  myWebServer.run();
  webSocket.loop();
}
