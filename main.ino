#include <WiFi.h>
#include <WebServer.h>
#include <secrets.cpp>

// Enter your home Wi-Fi network details here
const char* ssid = SSID;
const char* password = PASSWORD;

int counter = 0;

WebServer server(80);
String currentStatus = "IDLE";
const int triggerPin = 1;

// NEW: This keeps track of whether we have already triggered a sound
bool soundTriggered = false; 

// Sends the current status (IDLE or PLAY) to your iPhone app
void handleStatus() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", currentStatus);
}

// Resets the status back to IDLE AND clears our trigger flag
void handleReset() {
    server.sendHeader("Connection", "close");
    currentStatus = "IDLE";
    soundTriggered = false; // ALLOWS THE ESP32 TO TRIGGER THE NEXT SOUND
    server.send(200, "text/plain", "RESET_OK");
}

void setup() {
    // Start serial communication at 115200 baud rate
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)

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
    
    // HIGH means the button is NOT pressed (idle state)
    if (digitalRead(triggerPin) == HIGH) {
        
        // Only change the status if we haven't already sent a play command
        if (!soundTriggered) {
            
            if (counter < 100) {
                currentStatus = "PLAY1";
            } else if (counter < 200) {
                currentStatus = "PLAY2";
            } else if (counter < 300) {
                currentStatus = "PLAY3";
            } else if (counter < 400) {
                currentStatus = "PLAY4";
            } else if (counter < 500) {
                currentStatus = "PLAY5";
            } else {
                currentStatus = "PLAY6";
            }
            
            Serial.print("Button is NOT pressed! Status set to: ");
            Serial.println(currentStatus);
            
            // Lock it down so it doesn't spam. 
            // It stays locked until the app calls /reset
            soundTriggered = true; 
        }
    } else {
        // Optional: If they DO press the button, reset the tracker 
        // or change the status if you want a "pressed" behavior.
    }

    // Cycle the counter in the background to mix up the sounds
    counter++;
    if (counter >= 600) {
        counter = 0; 
    }
    
    delay(50); 
}
