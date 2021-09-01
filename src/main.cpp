#include <Arduino.h>

#define OLED_CLOCK 15
#define OLED_DATA 4
#define OLED_RESET 16

const uint32_t BLINK_DELAY_MILLIS = 500;

void setup()
{
  if(BLINK_DELAY_MILLIS > 0)
  {
    pinMode(LED_BUILTIN, OUTPUT);
  }
}

void loop()
{
  static uint32_t lastMillis = 0;
  uint32_t thisMillis = millis();
  uint32_t deltaMillis = thisMillis - lastMillis;
  lastMillis = thisMillis;

  if(BLINK_DELAY_MILLIS > 0)
  {
    void blinkLED(uint32_t deltaMillis);
    blinkLED(deltaMillis);
  }
}

void blinkLED(uint32_t deltaMillis)
{
    static bool ledState = HIGH;
    static uint32_t ledMillis = 0;

    digitalWrite(LED_BUILTIN, ledState);
    ledMillis += deltaMillis;
    if(ledMillis >= BLINK_DELAY_MILLIS)
    {
      ledState = !ledState;
      ledMillis -= BLINK_DELAY_MILLIS;
    }
}