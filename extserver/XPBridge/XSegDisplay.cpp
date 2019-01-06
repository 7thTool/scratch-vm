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


#include "XSegDisplay.hpp"


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


XSegDisplay::XSegDisplay() :
    XPBlock(XBLOCK_TYPE_XSegDisplay)
{
    LOGI(("XSegDisplay::XSegDisplay()\n"));
}

XSegDisplay::~XSegDisplay()
{
    LOGI(("XSegDisplay::~XSegDisplay()\n"));
}

void XSegDisplay::showNumber(float num)
{
    uint8_t param[4], *p=param;

    LOGI(("XSegDisplay::showNumber(%f)\n", num));

    p = fillFloat(p, num);
    access(XSegDisplay_API_showNumber, (const uint8_t *)param, 4, NULL, 0);
}

void XSegDisplay::clear()
{
    LOGI(("XSegDisplay::clear()\n"));
    access(XSegDisplay_API_clear, NULL, 0, NULL, 0);
}
