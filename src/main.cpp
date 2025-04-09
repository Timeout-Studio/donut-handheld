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

const char *ssid = "Timeout Studio 2.4G";
const char *password = "Timeout500";
const char *websocketServer = "192.168.0.100";
const uint16_t websocketPort = 8765;
const char *websocketPath = "/ws";
const char *username = "baby_alligator_2";
volatile int triggerState = LOW;
volatile int lastTriggerTime = 0;
// volatile int triggerState = LOW;
volatile int lastTriggerState = LOW;

// void taskGyroscope(void *pvParameters);
// void IRAM_ATTR isrLaser();
void IRAM_ATTR isrTrigger();

void setup()
{
  Serial.begin(115200);

  pinMode(LASER_SWITCH_PIN, INPUT);
  pinMode(GYRO_CALIBRATION_PIN, INPUT);
  pinMode(LASER_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);

  // initiallizing
  laser.init(LASER_PIN);
  display.init();
  display.setBacklight(127);
  gyroscope.init(123);
  // attachInterrupt(digitalPinToInterrupt(LASER_SWITCH_PIN), isrLaser, RISING);
  attachInterrupt(digitalPinToInterrupt(TRIGGER_PIN), isrTrigger, CHANGE);
  // xTaskCreate(taskGyroscope, "taskGyroscope", 2048, NULL, 1, NULL);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize WebSocket connection in its own thread
  websocket.begin(websocketServer, websocketPort, websocketPath, username);
}

void loop()
{
  char *ptr;

  display.routine(); // lv_task_handler
  // display.displayUpdateLoop(0);
  display.displayUpdateLoop((int)strtod(websocket.lastMessage, &ptr));
  // Serial.println((int)strtod(websocket.lastMessage, &ptr));
  // if (triggerState == HIGH)
  // {
  //   if (websocket.isWebSocketConnected())
  //   {
  //     websocket.sendMessage("baby_alligator_1", "trigger");
  //     Serial.println("Sent trigger to server");
  //   }
  // }

  if (lastTriggerState != triggerState)
  {
    lastTriggerState = triggerState;
    if (websocket.isWebSocketConnected())
    {
      websocket.sendMessage("baby_alligator_1", "trigger");
      Serial.println("Sent trigger to server");
    }
  }

  // Other tasks can run here without being affected by WebSocket operations
  delay(5);
}

// void taskGyroscope(void *pvParameters)
// {
//   for (;;)
//   {
//     if (gyroscope.calibrationFactor == 0.0)
//     {
//       float angle = gyroscope.readAngle();
//       Serial.print("Heading (degrees): ");
//       Serial.println(angle);
//     }
//     else
//     {
//       float calibrated = gyroscope.readCalibratedAngle();
//       Serial.print("Calibrated Heading (degrees): ");
//       Serial.println(calibrated);
//     }

//     if (digitalRead(GYRO_CALIBRATION_PIN) == HIGH)
//     {
//       gyroscope.calibrate();
//     }

//     vTaskDelay(100 / portTICK_PERIOD_MS);
//   }
// }

// void IRAM_ATTR isrLaser()
// {
//   laser.toggle();
//   Serial.print("Laser is ");
//   Serial.println(laser.getStatus() ? "ON" : "OFF");
// }

void IRAM_ATTR isrTrigger()
{
  // if (millis() - lastTriggerTime < 30)
  //   return;
  // lastTriggerTime = millis();

  // triggerState = digitalRead(TRIGGER_PIN);

  // if (triggerState == LOW)
  // {
  //   if (websocket.isWebSocketConnected())
  //   {
  //     websocket.sendMessage("baby_alligator_1", "trigger");
  //     Serial.println("Sent trigger to server");
  //   }
  // }

  triggerState = !triggerState; // Toggle trigger state
}