#pragma once

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

class Display
{
public:
    virtual void Init() = 0;
    virtual int16_t Width() = 0;
    virtual int16_t Height() = 0;
    virtual void Clear() = 0;
    virtual void SendBuffer() = 0;
    virtual int16_t FontMaxCharWidth() = 0;
    virtual int16_t FontHeight() = 0;
    virtual void DrawString(int16_t x, int16_t y, const char *s, size_t len) = 0;
    virtual void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1) = 0;

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

#if HELTEC

#include <U8g2lib.h>

class Display_U8G2_SSD1306 : public Display
{
private:
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C m_OLED;

public:
    Display_U8G2_SSD1306() : m_OLED(U8G2_R0, OLED_RESET, OLED_CLOCK, OLED_DATA)
    {
    }

    virtual void Init()
    {
        m_OLED.begin();
        m_OLED.clear();
        m_OLED.setFont(u8g2_font_profont10_tf);
        m_OLED.setFontPosTop();
    }

    virtual int16_t Width()
    {
        return m_OLED.getDisplayWidth();
    }

    virtual int16_t Height()
    {
        return m_OLED.getDisplayHeight();
    }

    virtual void Clear()
    {
        m_OLED.clearBuffer();
    }

    virtual void SendBuffer()
    {
        m_OLED.sendBuffer();
    }

    virtual int16_t FontMaxCharWidth()
    {
        return m_OLED.getMaxCharWidth();
    }

    virtual int16_t FontHeight()
    {
        return m_OLED.getFontAscent() - m_OLED.getFontDescent();
    }

    virtual void DrawString(int16_t x, int16_t y, const char *s, size_t len)
    {
        m_OLED.setCursor(x, y);
        m_OLED.write((uint8_t*)s, len);
    }

    virtual void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
    {
        m_OLED.drawLine(x0, y0, x1, y1);
    }
};

#elif TTGO

#include <TFT_eSPI.h>

class Display_TFT_eSPI : public Display
{
private:
    TFT_eSPI m_TFT;
    TFT_eSprite m_Buffer;

public:
    Display_TFT_eSPI() : m_TFT(), m_Buffer(&m_TFT)
    {
    }

    virtual void Init()
    {
        m_TFT.begin();
        m_TFT.setRotation(3);
        m_Buffer.setColorDepth(16);
        m_Buffer.createSprite(m_TFT.width(), m_TFT.height());
        m_Buffer.setTextColor(TFT_WHITE, TFT_BLACK);
        m_Buffer.setTextFont(2);
    }

    virtual int16_t Width()
    {
        return m_TFT.width();
    }

    virtual int16_t Height()
    {
        return m_TFT.height();
    }

    virtual void Clear()
    {
        m_Buffer.fillSprite(TFT_BLACK);
    }

    virtual void SendBuffer()
    {
        m_Buffer.pushSprite(0, 0);
    }

    virtual int16_t FontMaxCharWidth()
    {
        return m_Buffer.textWidth("M");
    }

    virtual int16_t FontHeight()
    {
        return m_Buffer.fontHeight();
    }

    virtual void DrawString(int16_t x, int16_t y, const char *s, size_t len)
    {
        m_Buffer.drawString(s, x, y);
    }

    virtual void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
    {
        m_Buffer.drawLine(x0, y0, x1, y1, TFT_WHITE);
    }
};

#elif WROVERKIT

class Display_Adafruit_ILI9341 : public Display
{
public:
    Display_Adafruit_ILI9341()
    {
    }

    virtual void Init()
    {
    }

    virtual int16_t Width()
    {
        return 0;
    }

    virtual int16_t Height()
    {
        return 0;
    }

    virtual void Clear()
    {
    }

    virtual void SendBuffer()
    {
    }

    virtual int16_t FontMaxCharWidth()
    {
        return 0;
    }

    virtual int16_t FontHeight()
    {
        return 0;
    }

    virtual void DrawString(int16_t x, int16_t y, const char *s, size_t len)
    {
    }

    virtual void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
    {
    }
};

#else

#error Unknown board!

#endif
