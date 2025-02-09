#include <Laser.h>
#include <Display.h>
#include <Encoder.h>

#define LASER_SWITCH_PIN 10
#define LASER_PIN 11
#define ENCODER_S1_PIN 6
#define ENCODER_S2_PIN 5
#define ENCODER_KEY_PIN 9

Dn_Laser laser;
Dn_Display display;
Dn_Encoder encoder;

void setup()
{
  Serial.begin(115200);

  pinMode(LASER_SWITCH_PIN, INPUT);

  // initiallizing
  laser.init(LASER_PIN);
  display.init();
  encoder.init(ENCODER_S1_PIN, ENCODER_S2_PIN, ENCODER_KEY_PIN);
}

void loop()
{
  display.routine(); // lv_task_handler

  // Serial.println("running");

  if (encoder.hasRotated())
  {
    Serial.print("Encoder Value: ");
    Serial.println(encoder.getValue());
  }

  if (encoder.isButtonPressed())
  {
    Serial.println("Button Pressed!");
  }

  delay(10); // Debounce delay
}