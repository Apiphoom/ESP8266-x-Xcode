#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//WIFI
const char* ssid = "Apiphoom's AirPort Extreme";  //YourWIFI
const char* password = "!!@*#&^%$#$%^&*()"; //YOURPASSWORD

//ESPSERVER
ESP8266WebServer server(80);

const int led1_pin = D1;
const int led2_pin = D2;

void handleRoot() {
 server.send(200, "text/plain", String("Now we connect from ESP8266") +
              String(" /setleds?led1=XX&led2=YY changing XX/YY to ON or OFF."));
}


void handleSetLeds() {
  
  String led1_status = server.arg("led1");
  String led2_status = server.arg("led2");
  
  
  bool url_check = false;
  if((led1_status == "ON")||(led1_status == "OFF")||(led2_status == "ON")||(led2_status == "OFF"))
    url_check = true;

 
  if (led1_status == "ON")
    digitalWrite(led1_pin, HIGH);
  else if (led1_status == "OFF")
    digitalWrite(led1_pin, LOW);
  if (led2_status == "ON")
    digitalWrite(led2_pin, HIGH);
  else if (led2_status == "OFF")
    digitalWrite(led2_pin, LOW);
  if (url_check)
   
    server.send(200, "text/plain", "LEDs' status changed! (" + String(millis()) + ")");
  else
    server.send(200, "text/plain", "LEDs' status unchanged! (" + String(millis()) + ")");
}


void handleNotFound() {
  
  String message = "File Not Found\n\n";
 
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  
  server.send(404, "text/plain", message);
}


void setup(void) {
  
  pinMode(led1_pin, OUTPUT);
  pinMode(led2_pin, OUTPUT);
  digitalWrite(led1_pin, LOW);
  digitalWrite(led2_pin, LOW);

  
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("");

  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

 
  server.on("/", HTTP_GET, handleRoot);
  server.on("/setleds", HTTP_GET, handleSetLeds);
  server.onNotFound(handleNotFound);

   server.begin();
  Serial.println("HTTP server started");
}


void loop(void) {
  server.handleClient();
}
