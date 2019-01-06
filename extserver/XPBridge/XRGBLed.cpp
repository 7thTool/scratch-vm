/**
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */


#include "XRGBLed.hpp"


#if 0
#define LOGV(x)     printf x
#define LOGI(x)     printf x
#define LOGW(x)     printf x
#define LOGE(x)     printf x
#else
#define LOGV(x)
#define LOGI(x)
#define LOGW(x)
#define LOGE(x)
#endif


XRGBLed::XRGBLed() :
    XPBlock(XBLOCK_TYPE_XRGBLed)
{
    LOGI(("XRGBLed::XRGBLed()\n"));
}

XRGBLed::~XRGBLed()
{
    LOGI(("XRGBLed::~XRGBLed()\n"));
}

void XRGBLed::setColor(uint8_t index, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t param[4], *p=param;

    LOGI(("XRGBLed::setColor(%d, %d, %d, %d)\n", index, red, green, blue));

    p = fillU8(p, index);
    p = fillU8(p, red);
    p = fillU8(p, green);
    p = fillU8(p, blue);
    access(XRGBLed_API_setColor, (const uint8_t *)param, 4, NULL, 0);
}

void XRGBLed::setAllColor(uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t param[3], *p=param;

    LOGI(("XRGBLed::setAllColor(%d, %d, %d)\n", red, green, blue));

    p = fillU8(p, red);
    p = fillU8(p, green);
    p = fillU8(p, blue);
    access(XRGBLed_API_setAllColor, (const uint8_t *)param, 3, NULL, 0);
}

void XRGBLed::show()
{
    access(XRGBLed_API_show, NULL, 0, NULL, 0);
}

void XRGBLed::showColor(uint8_t index, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t param[4], *p=param;

    LOGI(("XRGBLed::showColor(%d, %d, %d, %d)\n", index, red, green, blue));

    p = fillU8(p, index);
    p = fillU8(p, red);
    p = fillU8(p, green);
    p = fillU8(p, blue);
    access(XRGBLed_API_showColor, (const uint8_t *)param, 4, NULL, 0);
}

void XRGBLed::showAllColor(uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t param[3], *p=param;

    LOGI(("XRGBLed::showAllColor(%d, %d, %d)\n", red, green, blue));

    p = fillU8(p, red);
    p = fillU8(p, green);
    p = fillU8(p, blue);
    access(XRGBLed_API_showAllColor, (const uint8_t *)param, 3, NULL, 0);
}

void XRGBLed::clear()
{
    LOGI(("XRGBLed::clear()\n"));
    access(XRGBLed_API_clear, NULL, 0, NULL, 0);
}
