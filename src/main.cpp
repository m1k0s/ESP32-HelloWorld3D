#include <Arduino.h>
#include <U8g2lib.h>

#define OLED_CLOCK 15
#define OLED_DATA 4
#define OLED_RESET 16

const uint32_t BLINK_DELAY_MILLIS = 500;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C g_OLED(U8G2_R0, OLED_RESET, OLED_CLOCK, OLED_DATA);
uint32_t g_ScreenWidth;
uint32_t g_ScreenHeight;
uint32_t g_LineHeight;
uint32_t g_MaxCharWidth;

void setup()
{
    if(BLINK_DELAY_MILLIS > 0)
    {
        pinMode(LED_BUILTIN, OUTPUT);
    }

    g_OLED.begin();
    g_OLED.clear();

    g_ScreenWidth = g_OLED.getWidth();
    g_ScreenHeight = g_OLED.getHeight();

    g_OLED.setFont(u8g2_font_profont10_tf);
    g_LineHeight = g_OLED.getFontAscent() - g_OLED.getFontDescent();
    g_MaxCharWidth = g_OLED.getMaxCharWidth();

}

void loop()
{
    static uint32_t lastMillis = 0;
    uint32_t thisMillis = millis();
    uint32_t deltaMillis = thisMillis - lastMillis;
    lastMillis = thisMillis;

    static float fps = 0.0f;
    if(deltaMillis > 0)
    {
        fps = fps * 0.9f + (1000.0f / deltaMillis) * 0.1f;
    }

    if(BLINK_DELAY_MILLIS > 0)
    {
        void blinkLED(uint32_t deltaMillis);
        blinkLED(deltaMillis);
    }

    g_OLED.clearBuffer();

    {
        void render(uint32_t deltaMillis);
        render(deltaMillis);
    }

    g_OLED.setCursor(g_ScreenWidth - 3 * g_MaxCharWidth, g_LineHeight);
    g_OLED.printf("%03.0f", fps);

    g_OLED.sendBuffer();
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

void render(uint32_t deltaMillis)
{
}