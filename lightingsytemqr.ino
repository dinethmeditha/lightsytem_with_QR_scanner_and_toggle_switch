#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

// ===== WiFi Access Point =====
const char* ssid = "ESP32-RELAY";
const char* password = "12345678";

// ===== Relay Pins (CHANGE IF NEEDED) =====
int relayPins[8] = {13, 12, 14, 27, 26, 25, 33, 32};

// ===== HTML UI =====
String webPage() {
  String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>ESP32 Relay Panel</title>
<style>
body { background:#0e0e0e; color:white; font-family:Arial; text-align:center; }
.card { background:#1c1c1c; padding:15px; margin:10px; border-radius:10px; }
button { padding:15px; margin:5px; font-size:16px; width:100px; }
.on { background:green; color:white; }
.off { background:red; color:white; }
</style>
</head>
<body>
<h1>ESP32 Relay Control</h1>
)rawliteral";

  const char* labels[8] = {"B","C","S","A","G","M","2","5"};

  for (int i = 0; i < 8; i++) {
    page += "<div class='card'>";
    page += "<h3>Relay " + String(i + 1) + " (" + labels[i] + ")</h3>";
    page += "<a href='/relay/" + String(i) + "/on'><button class='on'>ON</button></a>";
    page += "<a href='/relay/" + String(i) + "/off'><button class='off'>OFF</button></a>";
    page += "</div>";
  }

  page += "</body></html>";
  return page;
}

// ===== RELAY CONTROL =====
void relayOn(int id) {
  digitalWrite(relayPins[id], LOW);   // LOW = ON (most relay boards)
}

void relayOff(int id) {
  digitalWrite(relayPins[id], HIGH);  // HIGH = OFF
}

// ===== HANDLERS =====
void handleRoot() {
  server.send(200, "text/html", webPage());
}

void handleRelayOn() {
  int id = server.pathArg(0).toInt();
  relayOn(id);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleRelayOff() {
  int id = server.pathArg(0).toInt();
  relayOff(id);
  server.sendHeader("Location", "/");
  server.send(303);
}

// ===== QR PATH HANDLERS =====
void handleB() { relayOn(0); server.send(200, "text/plain", "Relay B ON"); }
void handleC() { relayOn(1); server.send(200, "text/plain", "Relay C ON"); }
void handleS() { relayOn(2); server.send(200, "text/plain", "Relay S ON"); }
void handleA() { relayOn(3); server.send(200, "text/plain", "Relay A ON"); }
void handleG() { relayOn(4); server.send(200, "text/plain", "Relay G ON"); }
void handleM() { relayOn(5); server.send(200, "text/plain", "Relay M ON"); }
void handle2() { relayOn(6); server.send(200, "text/plain", "Relay 2 ON"); }
void handle5() { relayOn(7); server.send(200, "text/plain", "Relay 5 ON"); }

// ===== SETUP =====
void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 8; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], HIGH); // OFF
  }

  WiFi.softAP(ssid, password);
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/relay/{}/on", handleRelayOn);
  server.on("/relay/{}/off", handleRelayOff);

  // CAPITAL PATHS
  server.on("/B", handleB);
  server.on("/C", handleC);
  server.on("/S", handleS);
  server.on("/A", handleA);
  server.on("/G", handleG);
  server.on("/M", handleM);
  server.on("/2", handle2);
  server.on("/5", handle5);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}
