#include <Laser.h>
#include <Display.h>
#include <Encoder.h>
#include <Gyroscope.h>

#define LASER_SWITCH_PIN 10
#define LASER_PIN 11
#define ENCODER_S1_PIN 6
#define ENCODER_S2_PIN 5
#define ENCODER_KEY_PIN 9

Dn_Laser laser;
Dn_Display display;
Dn_Encoder encoder;
Dn_Gyroscope gyroscope;

void setup()
{
  Serial.begin(115200);

  pinMode(LASER_SWITCH_PIN, INPUT);

  // initiallizing
  laser.init(LASER_PIN);
  display.init();
  display.setBacklight(127);
  encoder.init(ENCODER_S1_PIN, ENCODER_S2_PIN, ENCODER_KEY_PIN);
  gyroscope.init(123);
}

void loop()
{
  display.routine(); // lv_task_handler

  if (encoder.hasRotated())
  {
    Serial.print("Encoder Value: ");
    Serial.println(encoder.getValue());
  }

  if (encoder.isButtonPressed())
  {
    Serial.println("Button Pressed!");
  }

  if (digitalRead(LASER_SWITCH_PIN) == HIGH)
  {
    laser.toggle();
    Serial.print("Laser is ");
    Serial.println(laser.getStatus() ? "ON" : "OFF");
    delay(300);
  }

  gyroscope.readDegree();
}