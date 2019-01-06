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


#include "XLightSensor.hpp"


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


XLightSensor::XLightSensor() :
    XPBlock(XBLOCK_TYPE_XLightSensor),
    mEvtMark(0),
	mEvtStatus(0)
{
    LOGI(("XLightSensor::XLightSensor()\n"));
}

XLightSensor::~XLightSensor()
{
    LOGI(("XLightSensor::~XLightSensor()\n"));
}

uint8_t XLightSensor::getLuminance()
{
    uint8_t result;
    uint8_t rsize=1;
    
    LOGI(("XLightSensor::getLuminance()\n"));

    if (access(XLightSensor_API_getLuminance, NULL, 0, &result, &rsize) != 0) {
        LOGE(("access XLightSensor_API_getLuminance failed\n"));
        return 0;
    } else {
        LOGV(("access XLightSensor_API_getLuminance, result is %d\n", result));
        return result;
    }
}

int XLightSensor::registerEvent(uint8_t evt, uint8_t value)
{
	LOGI(("XLightSensor::registerEvent(%d, %d)\n", evt, value));
	
	if (evt == XLightSensor_EVT_greatThreshold) {
        mEvtMark = (1 << XLightSensor_EVT_greatThreshold);
        mEvtStatus = 0;
        if (notifyRegister(XLightSensor_EVT_greatThreshold, &value, 1) != 0) {
			LOGE(("notifyRegister XLightSensor_EVT_greatThreshold failed\n"));
            mEvtMark = 0;
            mEvtStatus = 0;
			return -1;
		}
	} else if (evt == XLightSensor_EVT_littleThreshold) {
        mEvtMark = (1 << XLightSensor_EVT_littleThreshold);
        mEvtStatus = 0;
		if (notifyRegister(XLightSensor_EVT_littleThreshold, &value, 1) != 0) {
			LOGE(("notifyRegister XLightSensor_EVT_littleThreshold failed\n"));
            mEvtMark = 0;
            mEvtStatus = 0;
			return -1;
		}
	}
	
	return 0;
}

bool XLightSensor::queryEventStatus()
{
	LOGI(("XLightSensor::queryEventStatus()\n"));
	
	if (mEvtMark == mEvtStatus) {
		mEvtMark = 0;
		mEvtStatus = 0;
		LOGV(("XLightSensor Query Event OK\n"));
		return true;
	} else {
		return false;
	}
}

void XLightSensor::notifyHandler(uint8_t evt, const uint8_t *param, uint8_t psize)
{
    LOGI(("XLightSensor::notifyHandler(%d)\n", evt));

    /*!< FIXME: */
    if ((evt == XLightSensor_EVT_greatThreshold) && (mEvtMark & (1<<XLightSensor_EVT_greatThreshold))) {
		mEvtStatus = 1 << XLightSensor_EVT_greatThreshold;
        LOGV(("** wakeup signal **\n"));
    }
    else if ((evt == XLightSensor_EVT_littleThreshold) && (mEvtMark & (1<<XLightSensor_EVT_littleThreshold))) {
		mEvtStatus = 1 << XLightSensor_EVT_littleThreshold;
        LOGV(("** wakeup signal **\n"));
    }
}
