#pragma once

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

#if HELTEC
#include <U8g2lib.h>
#elif TTGO
#include <TFT_eSPI.h>
#elif WROVERKIT
#include <WROVER_KIT_LCD.h>
#else
#error Unknown board!
#endif

class Display
{
private:
#if HELTEC
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C m_OLED;
#elif TTGO
    TFT_eSPI m_TFT;
    TFT_eSprite m_Buffer;
#elif WROVERKIT
    WROVER_KIT_LCD m_TFT;
#endif

public:
    Display()
#if HELTEC
        : m_OLED(U8G2_R0, OLED_RESET, OLED_CLOCK, OLED_DATA)
#elif TTGO
        : m_TFT(), m_Buffer(&m_TFT)
#elif WROVERKIT
        : m_TFT()
#endif
    {
    }

    void Init()
    {
#if HELTEC
        m_OLED.begin();
        m_OLED.clear();
        m_OLED.setFont(u8g2_font_profont10_tf);
        m_OLED.setFontPosTop();
#elif TTGO
        m_TFT.begin();
        m_TFT.setRotation(3);
        m_Buffer.setColorDepth(16);
        m_Buffer.createSprite(m_TFT.width(), m_TFT.height());
        m_Buffer.setTextColor(TFT_WHITE, TFT_BLACK);
        m_Buffer.setTextFont(2);
#elif WROVERKIT
        m_TFT.begin();
        m_TFT.setRotation(3);
        m_TFT.setTextColor(WROVER_WHITE);
        m_TFT.setTextSize(1);
#endif
    }

    int16_t Width()
    {
#if HELTEC
        return m_OLED.getDisplayWidth();
#elif TTGO
        return m_TFT.width();
#elif WROVERKIT
        return m_TFT.width();
#endif
    }

    int16_t Height()
    {
#if HELTEC
        return m_OLED.getDisplayHeight();
#elif TTGO
        return m_TFT.height();
#elif WROVERKIT
        return m_TFT.height();
#endif
    }

    void Clear()
    {
#if HELTEC
        m_OLED.clearBuffer();
#elif TTGO
        m_Buffer.fillSprite(TFT_BLACK);
#elif WROVERKIT
        m_TFT.fillScreen(WROVER_BLACK);
#endif
    }

    void SendBuffer()
    {
#if HELTEC
        m_OLED.sendBuffer();
#elif TTGO
        m_Buffer.pushSprite(0, 0);
#elif WROVERKIT
#endif
    }

    int16_t FontMaxCharWidth()
    {
#if HELTEC
        return m_OLED.getMaxCharWidth();
#elif TTGO
        return m_Buffer.textWidth("M");
#elif WROVERKIT
        int16_t x1;
        int16_t y1;
        uint16_t w;
        uint16_t h;
        m_TFT.getTextBounds("M", 0, 0, &x1, &y1, &w, &h);
        return w;
#endif
    }

    int16_t FontHeight()
    {
#if HELTEC
        return m_OLED.getFontAscent() - m_OLED.getFontDescent();
#elif TTGO
        return m_Buffer.fontHeight();
#elif WROVERKIT
        int16_t x1;
        int16_t y1;
        uint16_t w;
        uint16_t h;
        m_TFT.getTextBounds("M", 0, 0, &x1, &y1, &w, &h);
        return h;
#endif
    }

    void DrawString(int16_t x, int16_t y, const char *s, size_t len)
    {
#if HELTEC
        m_OLED.setCursor(x, y);
        m_OLED.write((uint8_t*)s, len);
#elif TTGO
        m_Buffer.drawString(s, x, y);
#elif WROVERKIT
        m_TFT.setCursor(x, y);
        m_TFT.write(s, len);
#endif
    }

    void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
    {
#if HELTEC
        m_OLED.drawLine(x0, y0, x1, y1);
#elif TTGO
        m_Buffer.drawLine(x0, y0, x1, y1, TFT_WHITE);
#elif WROVERKIT
        m_TFT.drawLine(x0, y0, x1, y1, WROVER_WHITE);
#endif
    }

    void PrintF(int16_t x, int16_t y, const char *format, ...)
    {
        char buffer[256];
        va_list args;
        va_start(args, format);
        int len = vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        DrawString(x, y, buffer, len);
    }
};
