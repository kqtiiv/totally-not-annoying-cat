#include <WiFi.h>
#include <WebServer.h>

// Enter your home Wi-Fi network details here
const char* ssid = "30Engate";
const char* password = "Engate-Guest#8923";

int counter = 0;

WebServer server(80);
String currentStatus = "IDLE";
const int triggerPin = 1;

// Sends the current status (IDLE or PLAY) to your iPhone app
void handleStatus() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", currentStatus);
}

// Resets the status back to IDLE after the iPhone plays the sound
void handleReset() {
    server.sendHeader("Connection", "close");
    currentStatus = "IDLE";
    server.send(200, "text/plain", "RESET_OK");
}

void setup() {
    // Start serial communication at 115200 baud rate
    Serial.begin(115200);
    
    // Configure the on-board button pin
    pinMode(triggerPin, INPUT_PULLUP);

    // Connect to your Wi-Fi network
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) { 
        delay(500); 
        Serial.print(".");
    }
    Serial.println("");
    
    // This is the IP address you need to copy into MIT App Inventor
    Serial.print("ESP32-S2 IP Address: ");
    Serial.println(WiFi.localIP()); 

    // Define the web endpoints the iPhone will check
    server.on("/status", handleStatus);
    server.on("/reset", handleReset);
    server.begin();
}

void loop() {
    server.handleClient();
    if (digitalRead(triggerPin) == LOW) {
        counter = 0;
        currentStatus = "PLAYPURR";
        Serial.println("Trigger activated! Status set to PLAYPURR.");
        return;
    }

    if (counter < 100) {
        currentStatus = "PLAY1";
        Serial.println("Trigger activated! Status set to PLAY1.");
    } else if (counter < 200) {
        currentStatus = "PLAY2";
        Serial.println("Trigger activated! Status set to PLAY2.");
    } else if (counter < 300) {
        currentStatus = "PLAY3";
        Serial.println("Trigger activated! Status set to PLAY3.");
    } else if (counter < 400) {
        currentStatus = "PLAY4";
        Serial.println("Trigger activated! Status set to PLAY4.");
    } else if (counter < 500) {
        currentStatus = "PLAY5";
        Serial.println("Trigger activated! Status set to PLAY5.");
    }   else {
        currentStatus = "PLAY6";
        Serial.println("Trigger activated! Status set to PLAY6.");
    }
    
    delay(200);

    counter++;
}
