#include <WiFi.h>
#include <WebServer.h>
#include <Servo.h>
#include <secrets.cpp>

// Enter your home Wi-Fi network details here
const char* ssid = SSID;
const char* password = PASSWORD;

int counter = 0;
bool thrown = false;

WebServer server(80);
String currentStatus = "IDLE";
Servo myServo;

const int triggerPin = 1;
int pos = 0;

bool soundTriggered = false;

void handleStatus() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", currentStatus);
}

void handleReset() {
    server.sendHeader("Connection", "close");
    currentStatus = "IDLE";
    soundTriggered = false;
    server.send(200, "text/plain", "RESET_OK");
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    Serial.begin(115200);
    
    pinMode(triggerPin, INPUT_PULLUP);
    myServo.attach(8);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) { 
        delay(500); 
        Serial.print(".");
    }
    Serial.println("");
    
    Serial.print("ESP32-S2 IP Address: ");
    Serial.println(WiFi.localIP()); 

    server.on("/status", handleStatus);
    server.on("/reset", handleReset);
    server.begin();
}

void loop() {
    server.handleClient();

    if (digitalRead(triggerPin) == HIGH) {
        if (counter < 100)       currentStatus = "PLAY1";
        else if (counter < 200)  currentStatus = "PLAY2";
        else if (counter < 300)  currentStatus = "PLAY3";
        else if (counter < 400)  currentStatus = "PLAY4";
        else if (counter < 500)  currentStatus = "PLAY5";
        else if (counter < 550)  currentStatus = "PLAY6";

        counter++;
    } else {
        currentStatus = "PLAYPURR";
        counter = 0;
    }

    // Trigger servo sweep when counter maxes out
if (counter >= 0 && !thrown) {
    myServo.write(180);  // spin one direction
    delay(500);          // tune this ms value until it rotates ~70°
    myServo.write(90);   // stop

    myServo.write(0);  // spin one direction
    delay(500);          // tune this ms value until it rotates ~70°
    myServo.write(90);   // stop
    thrown = true;
}

    Serial.println("Current status: " + currentStatus);

    delay(50);
}
