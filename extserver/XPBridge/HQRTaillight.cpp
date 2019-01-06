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


#include "HQRTaillight.hpp"

	
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


HQRTaillight::HQRTaillight() :
    XPServer(HQRSVR_TYPE_TAILLIGHT)
{
    LOGI(("HQRTaillight::HQRTaillight()\n"));
}

HQRTaillight::~HQRTaillight()
{
    LOGI(("HQRTaillight::~HQRTaillight()\n"));
}

int HQRTaillight::setup(XSegDisplay *sgd)
{
    uint8_t param = sgd->uid();
    
    LOGI(("HQRTaillight::setup(%d)\n", param));
    return XPServer::setup(&param, 1);
}

void HQRTaillight::showNumber(float num)
{
    uint8_t param[4], *p=param;
    
    LOGI(("HQRTaillight::showNumber(%f)\n", num));

    p = fillFloat(p, num);
    access(HQRTaillight_API_showNumber, (const uint8_t *)param, 4, NULL, 0);
}

void HQRTaillight::showEffect(uint8_t effect)
{
    LOGI(("HQRTaillight::showEffect(%d)\n", effect));
    access(HQRTaillight_API_showEffect, (const uint8_t *)&effect, 1, NULL, 0);
}

void HQRTaillight::clear()
{
    LOGI(("HQRTaillight::clear()\n"));
    access(HQRTaillight_API_clear, NULL, 0, NULL, 0);
}
