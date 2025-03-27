#include <Laser.h>
#include <Display.h>
#include <Encoder.h>
#include <Gyroscope.h>
#include <Websocket.h>
#include <WiFi.h>

#define GYRO_CALIBRATION_PIN 12
#define LASER_SWITCH_PIN 10
#define LASER_PIN 11
#define ENCODER_S1_PIN 6
#define ENCODER_S2_PIN 5
#define ENCODER_KEY_PIN 9

Dn_Laser laser;
Dn_Display display;
Dn_Encoder encoder;
Dn_Gyroscope gyroscope;
Dn_Websocket websocket;

const char *ssid = "Timeout Studio 2.4G";
const char *password = "Timeout500";
const char *websocketServer = "192.168.0.100";
const uint16_t websocketPort = 8765;
const char *websocketPath = "/ws";
const char *username = "baby_alligator_2";

void taskGyroscope(void *pvParameters)
{
  for (;;)
  {
    if (gyroscope.calibrationFactor == 0.0)
    {
      float angle = gyroscope.readAngle();
      Serial.print("Heading (degrees): ");
      Serial.println(angle);
    }
    else
    {
      float calibrated = gyroscope.readCalibratedAngle();
      Serial.print("Calibrated Heading (degrees): ");
      Serial.println(calibrated);
    }

    if (digitalRead(GYRO_CALIBRATION_PIN) == HIGH)
    {
      gyroscope.calibrate();
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void IRAM_ATTR isrLaser()
{
  laser.toggle();
  Serial.print("Laser is ");
  Serial.println(laser.getStatus() ? "ON" : "OFF");
  // delay(300);
}

void setup()
{
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize WebSocket connection in its own thread
  websocket.begin(websocketServer, websocketPort, websocketPath, username);

  pinMode(LASER_SWITCH_PIN, INPUT);
  pinMode(GYRO_CALIBRATION_PIN, INPUT);

  // initiallizing
  laser.init(LASER_PIN);
  display.init();
  display.setBacklight(127);
  encoder.init(ENCODER_S1_PIN, ENCODER_S2_PIN, ENCODER_KEY_PIN);
  gyroscope.init(123);
  attachInterrupt(digitalPinToInterrupt(LASER_SWITCH_PIN), isrLaser, RISING);
  xTaskCreate(taskGyroscope, "taskGyroscope", 2048, NULL, 1, NULL);
}

void loop()
{
  display.routine(uint16_t(gyroscope.readAngle())); // lv_task_handler

  static unsigned long lastTime = 0;
  if (millis() - lastTime > 10000)
  {
    lastTime = millis();

    if (websocket.isWebSocketConnected())
    {
      // Send a message to a specific user
      websocket.sendMessage("baby_alligator_1", "Hello from ESP32");
      Serial.println("Sent message to server");
    }
    else
    {
      Serial.println("Not connected, can't send message");
    }
  }

  // Other tasks can run here without being affected by WebSocket operations
  delay(5);
}