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


#include "XBuzzer.hpp"


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


XBuzzer::XBuzzer() :
    XPBlock(XBLOCK_TYPE_XBuzzer)
{
    LOGI(("XBuzzer::XBuzzer()\n"));
}

XBuzzer::~XBuzzer ()
{
    LOGI(("XBuzzer::~XBuzzer()\n"));
}

void XBuzzer::playTone(uint16_t frequency, uint32_t duration)
{
    uint8_t param[6], *p=param;
    
    LOGI(("XBuzzer::playTone(%d, %d)\n", frequency, duration));

    p = fillU16(p, frequency);
    p = fillU32(p, duration);
    access(XBuzzer_API_playTone, (const uint8_t *)param, 6, NULL, 0, (int)duration);
}
