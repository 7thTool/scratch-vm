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


#include "XSoundSensor.hpp"


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


XSoundSensor::XSoundSensor() :
    XPBlock(XBLOCK_TYPE_XSoundSensor),
    mEvtMark(0),
	mEvtStatus(0)
{
    LOGI(("XSoundSensor::XSoundSensor()\n"));
}

XSoundSensor::~XSoundSensor()
{
    LOGI(("XSoundSensor::~XSoundSensor()\n"));
}

uint8_t XSoundSensor::getVolume()
{
    uint8_t result;
    uint8_t rsize=1;

    LOGI(("XSoundSensor::getVolume()\n"));

    if (access(XSoundSensor_API_getVolume, NULL, 0, &result, &rsize) != 0) {
        LOGE(("access XSoundSensor_API_getVolume failed\n"));
        return 0;
    } else {
        LOGV(("access XSoundSensor_API_getVolume, result is %d\n", result));
        return result;
    }
}

int XSoundSensor::registerEvent(uint8_t evt, uint8_t value)
{
	LOGI(("XSoundSensor::registerEvent(%d, %d)\n", evt, value));
	
	if (evt == XSoundSensor_EVT_greatThreshold) {
        mEvtMark = (1 << XSoundSensor_EVT_greatThreshold);
        mEvtStatus = 0;
		if (notifyRegister(XSoundSensor_EVT_greatThreshold, &value, 1) != 0) {
			LOGE(("notifyRegister XSoundSensor_EVT_greatThreshold failed\n"));
            mEvtMark = 0;
            mEvtStatus = 0;
			return -1;
		}

	} else if (evt == XSoundSensor_EVT_littleThreshold) {
        mEvtMark = (1 << XSoundSensor_EVT_littleThreshold);
        mEvtStatus = 0;
		if (notifyRegister(XSoundSensor_EVT_littleThreshold, &value, 1) != 0) {
			LOGE(("notifyRegister XSoundSensor_EVT_littleThreshold failed\n"));
            mEvtMark = 0;
            mEvtStatus = 0;
			return -1;
		}
	}
	
	return 0;
}

bool XSoundSensor::queryEventStatus()
{
	LOGI(("XSoundSensor::queryEventStatus()\n"));
	
	if (mEvtMark == mEvtStatus) {
		mEvtMark = 0;
		mEvtStatus = 0;
		LOGV(("XSoundSensor Query Event OK\n"));
		return true;
	} else {
		return false;
	}
}

void XSoundSensor::notifyHandler(uint8_t evt, const uint8_t *param, uint8_t psize)
{
    LOGI(("XSoundSensor::notifyHandler(%d)\n", evt));

    /*!< FIXME: */
    if ((evt == XSoundSensor_EVT_greatThreshold) && (mEvtMark & (1<<XSoundSensor_EVT_greatThreshold))) {
		mEvtStatus = 1 << XSoundSensor_EVT_greatThreshold;
        LOGV(("** wakeup signal **\n"));
    }
    else if ((evt == XSoundSensor_EVT_littleThreshold) && (mEvtMark & (1<<XSoundSensor_EVT_littleThreshold))) {
		mEvtStatus = 1 << XSoundSensor_EVT_littleThreshold;
        LOGV(("** wakeup signal **\n"));
    }
}
