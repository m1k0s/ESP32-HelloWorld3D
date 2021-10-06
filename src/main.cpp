#include <Arduino.h>
#include <alloca.h>
#include "viewport.h"
#include "tetrahedron.h"
#include "hexahedron.h"
#include "octahedron.h"

#if HELTEC
#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_F_HW_I2C g_OLED(U8G2_R0, OLED_RESET, OLED_CLOCK, OLED_DATA);
#elif TTGO
#include <TFT_eSPI.h>
TFT_eSPI g_TFT = TFT_eSPI();
TFT_eSprite g_Buffer(&g_TFT);
#else
#error Unknown board!
#endif
uint32_t g_ScreenWidth;
uint32_t g_ScreenHeight;
uint32_t g_LineHeight;
uint32_t g_MaxCharWidth;

Viewport g_Viewport;

const uint32_t BLINK_DELAY_MILLIS = 500;

void setup()
{
    if(BLINK_DELAY_MILLIS > 0)
    {
        pinMode(LED_BUILTIN, OUTPUT);
    }

#if HELTEC
    g_OLED.begin();
    g_OLED.clear();

    g_ScreenWidth = g_OLED.getWidth();
    g_ScreenHeight = g_OLED.getHeight();

    g_OLED.setFont(u8g2_font_profont10_tf);
    g_LineHeight = g_OLED.getFontAscent() - g_OLED.getFontDescent();
    g_MaxCharWidth = g_OLED.getMaxCharWidth();
#elif TTGO
    g_TFT.begin();
    g_TFT.setRotation(3);

    g_ScreenWidth = g_TFT.width();
    g_ScreenHeight = g_TFT.height();

    g_Buffer.setColorDepth(16);
    g_Buffer.createSprite(g_ScreenWidth, g_ScreenHeight);

    g_Buffer.setTextColor(TFT_WHITE, TFT_BLACK);
    g_Buffer.setTextFont(2);
    g_LineHeight = g_Buffer.fontHeight();
    g_MaxCharWidth = g_Buffer.textWidth("0");
#endif

    g_Viewport.halfWidth = 0.5f * g_ScreenWidth;
    g_Viewport.halfHeight = 0.5f * g_ScreenHeight;
    g_Viewport.fov = g_ScreenWidth < g_ScreenHeight ? g_ScreenWidth : g_ScreenHeight;
    g_Viewport.eyeDistance = 4.0f;
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

#if HELTEC
    g_OLED.clearBuffer();
#elif TTGO
    g_Buffer.fillSprite(TFT_BLACK);
#endif

    {
        void render(uint32_t deltaMillis);
        render(deltaMillis);
    }

#if HELTEC
    g_OLED.setCursor(g_ScreenWidth - 3 * g_MaxCharWidth, g_LineHeight);
    g_OLED.printf("%03.0f", fps);

    g_OLED.sendBuffer();
#elif TTGO
    g_Buffer.setCursor(g_ScreenWidth - 3 * g_MaxCharWidth, 0);
    g_Buffer.printf("%03.0f", fps);

    g_Buffer.pushSprite(0, 0);
#endif

    const uint32_t FRAME_TIME_CAP_MILLIS = 33;
    // Work out how long everything up till now took in this frame and delay appropriately
    uint32_t frameMillis = millis() - thisMillis;
    if (frameMillis < FRAME_TIME_CAP_MILLIS)
    {
        delay(FRAME_TIME_CAP_MILLIS - frameMillis);
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

void render(uint32_t deltaMillis)
{
    static float angleX = 0.0f;
    static float angleY = 0.0f;
    static float angleZ = 0.0f;

    void renderWireframe(const Mesh& mesh, const Viewport& viewport, float angleX, float angleY, float angleZ);
    renderWireframe(g_Hexahedron, g_Viewport, angleX, angleY, angleZ);

    float dt = deltaMillis / 1000.0f;

    angleX += dt * 1.0f / M_PI;
    angleY += dt * 2.0f / M_PI;
    angleZ += dt * 3.0f / M_PI;
}

void renderWireframe(const Mesh& mesh, const Viewport& viewport, float angleX, float angleY, float angleZ)
{
    Vector3* verts = (Vector3*)alloca(sizeof(Vector3) * mesh.verticiesSize);

    for(int i = 0; i < mesh.verticiesSize; ++i)
    {
        verts[i] = mesh.verticies[i].
            RotateX(angleX).
            RotateY(angleY).
            RotateZ(angleZ).
            Project(viewport);
    }

    for(int i = 0; i < mesh.edgesSize; i += 2)
    {
        const Vector3& p0 = verts[mesh.edges[i]];
        const Vector3& p1 = verts[mesh.edges[i + 1]];
#if HELTEC
        g_OLED.drawLine(p0.x, p0.y, p1.x, p1.y);
#elif TTGO
        g_Buffer.drawLine(p0.x, p0.y, p1.x, p1.y, TFT_YELLOW);
#endif
    }
}
