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


#include "HQRLightShow.hpp"


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


HQRLightShow::HQRLightShow() :
    XPServer(HQRSVR_TYPE_LIGHTSHOW)
{
    LOGI(("HQRLightShow::HQRLightShow()\n"));
}

HQRLightShow::~HQRLightShow()
{
    LOGI(("HQRLightShow::~HQRLightShow()\n"));
}

int HQRLightShow::setup(XRGBLed *rgb)
{
    uint8_t param = rgb->uid();

    LOGI(("HQRLightShow::setup(%d)\n", param));
    return XPServer::setup(&param, 1);
}

void HQRLightShow::showColor(uint8_t index, uint8_t red, uint8_t green, uint8_t blue, uint8_t clearOthers)
{
    uint8_t param[5], *p=param;

    LOGI(("HQRLightShow::showColor(%d, %d, %d, %d)\n", index, red, green, blue));

    p = fillU8(p, index);
    p = fillU8(p, red/3);
    p = fillU8(p, green/3);
    p = fillU8(p, blue/3);
    p = fillU8(p, clearOthers);
    access(HQRLightShow_API_showColor, (const uint8_t *)param, 5, NULL, 0);
}

void HQRLightShow::showMeteor(uint8_t speed, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t param[4], *p=param;

    LOGI(("HQRLightShow::showMetor(%d, %d, %d, %d)\n", speed, red, green, blue));

    p = fillU8(p, speed);
    p = fillU8(p, red/3);
    p = fillU8(p, green/3);
    p = fillU8(p, blue/3);
    access(HQRLightShow_API_showMeteor, (const uint8_t *)param, 4, NULL, 0);
}

void HQRLightShow::showBreath(uint8_t index, uint8_t speed, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t param[5], *p=param;

    LOGI(("HQRLightShow::showBreath(%d, %d, %d, %d, %d)\n", index, speed, red, green, blue));

    p = fillU8(p, index);
    p = fillU8(p, speed);
    p = fillU8(p, red/3);
    p = fillU8(p, green/3);
    p = fillU8(p, blue/3);
    access(HQRLightShow_API_showBreath, (const uint8_t *)param, 5, NULL, 0);
}

void HQRLightShow::clear(uint8_t index)
{
    LOGI(("HQRLightShow::clear(%d)\n", index));
    access(HQRLightShow_API_clear, (const uint8_t *)&index, 1, NULL, 0);
}
