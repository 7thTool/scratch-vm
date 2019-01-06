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


#include "XUltrasonic.hpp"


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


XUltrasonic::XUltrasonic() :
    XPBlock(XBLOCK_TYPE_XUltrasonic),
    mEvtMark(0),
	mEvtStatus(0)
{
    LOGI(("XUltrasonic::XUltrasonic()\n"));
}

XUltrasonic::~XUltrasonic()
{
    LOGI(("XUltrasonic::~XUltrasonic()\n"));
}

uint16_t XUltrasonic::getDistance()
{
    uint16_t result, distance;
    uint8_t rsize=2;
    
    LOGI(("XUltrasonic::getDistance()\n"));
    
    if (access(XUltrasonic_API_getDistance, NULL, 0, (uint8_t *)&result, &rsize) != 0) {
        LOGE(("access XUltrasonic_API_getDistance failed\n"));
        return 0;
    } else {
        fetchU16((uint8_t *)&result, &distance);
        LOGV(("access XUltrasonic_API_getDistance, result is %d\n", distance));
        return distance;
    }
}

int XUltrasonic::registerEvent(uint8_t evt, uint8_t value)
{
	LOGI(("XUltrasonic::registerEvent(%d, %d)\n", evt, value));
	
	if (evt == XUltrasonic_EVT_greatThreshold) {
        mEvtMark = (1 << XUltrasonic_EVT_greatThreshold);
        mEvtStatus = 0;
		if (notifyRegister(XUltrasonic_EVT_greatThreshold, &value, 1) != 0) {
			LOGE(("notifyRegister XUltrasonic_EVT_greatThreshold failed\n"));
            mEvtMark = 0;
            mEvtStatus = 0;
			return -1;
		}
	} else if (evt == XUltrasonic_EVT_littleThreshold) {
        mEvtMark = (1 << XUltrasonic_EVT_littleThreshold);
        mEvtStatus = 0;
		if (notifyRegister(XUltrasonic_EVT_littleThreshold, &value, 1) != 0) {
			LOGE(("notifyRegister XUltrasonic_EVT_littleThreshold failed\n"));
            mEvtMark = 0;
            mEvtStatus = 0;
			return -1;
		}
	}
	
	return 0;
}

bool XUltrasonic::queryEventStatus()
{
	LOGI(("XUltrasonic::queryEventStatus()\n"));
	
	if (mEvtMark == mEvtStatus) {
		mEvtMark = 0;
		mEvtStatus = 0;
		LOGV(("XUltrasonic Query Event OK\n"));
		return true;
	} else {
		return false;
	}
}

void XUltrasonic::notifyHandler(uint8_t evt, const uint8_t *param, uint8_t psize)
{
    LOGI(("XUltrasonic::notifyHandler(%d)\n", evt));

    /*!< FIXME: */
    if ((evt == XUltrasonic_EVT_greatThreshold) && (mEvtMark & (1<<XUltrasonic_EVT_greatThreshold))) {
		mEvtStatus = 1 << XUltrasonic_EVT_greatThreshold;
        LOGV(("** wakeup signal **\n"));
    }
    else if ((evt == XUltrasonic_EVT_littleThreshold) && (mEvtMark & (1<<XUltrasonic_EVT_littleThreshold))) {
		mEvtStatus = 1 << XUltrasonic_EVT_littleThreshold;
        LOGV(("** wakeup signal **\n"));
    }
}
