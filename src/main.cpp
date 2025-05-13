#include <Laser.h>
#include <Display.h>
#include <Encoder.h>
#include <Gyroscope.h>
#include <Websocket.h>
#include <WiFi.h>

#define GYRO_CALIBRATION_PIN 12
#define LASER_SWITCH_PIN 10
#define LASER_PIN 11
#define TRIGGER_PIN 13

Dn_Laser laser;
Dn_Display display;
Dn_Gyroscope gyroscope;
Dn_Websocket websocket;

/* --- Connection Settings --- */
const char *websocketTarget = "baby_alligator_1";
const char *ssid = "Timeout Studio 2.4G";
const char *password = "Timeout500";
const char *websocketServer = "192.168.0.100";
const uint16_t websocketPort = 8765;
const char *websocketPath = "/ws";
const char *websocketUsername = "baby_alligator_2";
/* --- End of Connection Settings --- */

unsigned long lastTriggerCheckTime = 0;
const unsigned long triggerCheckInterval = 50;
volatile bool triggerPressed = false;
volatile bool triggerHandled = false;

void IRAM_ATTR isrTrigger();

void setup()
{
  Serial.begin(115200);

  pinMode(TRIGGER_PIN, INPUT_PULLUP);

  // Initializing
  display.init();
  display.setBacklight(127);
  attachInterrupt(digitalPinToInterrupt(TRIGGER_PIN), isrTrigger, CHANGE);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) // Timeout after 10 seconds
  {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("\nWiFi connection failed. Continuing without WiFi.");
  }

  // Initialize WebSocket connection in its own thread
  websocket.begin(websocketServer, websocketPort, websocketPath, websocketUsername);
}

void loop()
{
  display.routine();
  display.displayUpdateLoop(atoi(websocket.lastMessage));

  if (millis() - lastTriggerCheckTime >= triggerCheckInterval)
  {
    if (triggerPressed)
    {
      if (websocket.isWebSocketConnected())
      {
        websocket.sendMessage(websocketTarget, "trigger");
        Serial.println("Trigger activated");
      }
      triggerPressed = false;
    }

    lastTriggerCheckTime = millis();
  }
}

void IRAM_ATTR isrTrigger()
{
  if (digitalRead(TRIGGER_PIN) == LOW)
  {
    if (!triggerHandled)
    {
      triggerPressed = true;
      triggerHandled = true;
    }
  }
  else
  {
    triggerHandled = false;
  }
}
