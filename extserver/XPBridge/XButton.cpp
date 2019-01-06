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


#include "XButton.hpp"


#if 1
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


XButton::XButton() :
    XPBlock(XBLOCK_TYPE_XButton),
    mEvtMark(0),
	mEvtStatus(0)
{
    LOGI(("XButton::XButton()\n"));
}

XButton::~XButton()
{
    LOGI(("XButton::~XButton()\n"));
}

uint8_t XButton::isPressed()
{
    uint8_t result;
    uint8_t rsize=1;

    LOGI(("XButton::isPressed()\n"));

    if (access(XButton_API_isPressed, NULL, 0, &result, &rsize) != 0) {
        LOGE(("access XButton_API_isPressed failed\n"));
        return 0;
    } else {
        LOGV(("access XButton_API_isPressed, result is %d\n", result));
        return result;
    }
}
	
uint8_t XButton::isKnocked()
{
    uint8_t result;
    uint8_t rsize=1;

    LOGI(("XButton::isKnocked()\n"));

    if (access(XButton_API_isKnocked, NULL, 0, &result, &rsize) != 0) {
        LOGE(("access XButton_API_isKnocked failed\n"));
        return 0;
    } else {
        LOGV(("access XButton_API_isKnocked, result is %d\n", result));
        return result;
    }
}

int XButton::registerEvent(uint8_t evt)
{
	LOGI(("XButton::registerEvent(%d)\n", evt));
	
	if (evt == XButton_EVT_knocked) {
		if (!(mEvtMark & XButton_EVT_knocked)) {
            mEvtMark = (1 << XButton_EVT_knocked);
            mEvtStatus = 0;
			if (notifyRegister(XButton_EVT_knocked, NULL, 0) != 0) {
				LOGE(("notifyRegister XButton_EVT_knocked failed\n"));
                mEvtMark = 0;
                mEvtStatus = 0;
				return -1;
			}
		}
		return 0;
	}
	
	return -1;
}

bool XButton::queryEventStatus()
{
    if(mEvtMark == 0 && mEvtStatus==0) {
        return false;
    }
	if (mEvtStatus == mEvtMark) {
		mEvtMark = 0;
		mEvtStatus = 0;
		LOGV(("XButton Query Event OK\n"));
		return true;
	} else {
		return false;
	}
}

void XButton::notifyHandler(uint8_t evt, const uint8_t *param, uint8_t psize)
{
    LOGI(("XButton::notifyHandler(%d)\n", evt));

    if ((evt == XButton_EVT_knocked) && (mEvtMark & (1 << XButton_EVT_knocked))) {
		mEvtStatus |= (1 << XButton_EVT_knocked);
        LOGV(("** wakeup signal **\n"));
    }
}
