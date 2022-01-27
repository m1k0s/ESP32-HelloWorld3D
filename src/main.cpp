#include <Arduino.h>
#include <alloca.h>
#include "display.h"
#include "viewport.h"
#include "tetrahedron.h"
#include "hexahedron.h"
#include "octahedron.h"

#if HELTEC
Display_U8G2_SSD1306 g_Display = Display_U8G2_SSD1306();
#elif TTGO
Display_TFT_eSPI g_Display = Display_TFT_eSPI();
#elif WROVERKIT
Display_WROVER_KIT_LCD g_Display = Display_WROVER_KIT_LCD();
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
        #if LED_BUILTIN
        pinMode(LED_BUILTIN, OUTPUT);
        #else
        pinMode(LED_BUILTIN_RED, OUTPUT);
        pinMode(LED_BUILTIN_GREEN, OUTPUT);
        pinMode(LED_BUILTIN_BLUE, OUTPUT);
        #endif
    }

    g_Display.Init();
    g_ScreenWidth = g_Display.Width();
    g_ScreenHeight = g_Display.Height();

    g_LineHeight = g_Display.FontHeight();
    g_MaxCharWidth = g_Display.FontMaxCharWidth();

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

    g_Display.Clear();

    {
        void render(uint32_t deltaMillis);
        render(deltaMillis);
    }

    g_Display.PrintF(g_ScreenWidth - 3 * g_MaxCharWidth, 0, "%03.0f", fps);

    g_Display.SendBuffer();

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

    #if LED_BUILTIN
    digitalWrite(LED_BUILTIN, ledState);
    #else
    digitalWrite(LED_BUILTIN_RED, ledState);
    digitalWrite(LED_BUILTIN_GREEN, ledState);
    digitalWrite(LED_BUILTIN_BLUE, ledState);
    #endif
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
        g_Display.DrawLine(p0.x, p0.y, p1.x, p1.y);
    }
}
