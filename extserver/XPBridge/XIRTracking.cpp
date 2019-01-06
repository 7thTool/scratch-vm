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


#include "XIRTracking.hpp"


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


XIRTracking::XIRTracking() :
    XPBlock(XBLOCK_TYPE_XIRTracking),
    mEvtMark(0),
	mEvtStatus(0)
{
    LOGI(("XIRTracking::XIRTracking()\n"));
}

XIRTracking::~XIRTracking()
{
    LOGI(("XIRTracking::~XIRTracking()\n"));
}

uint8_t XIRTracking::getStatus()
{
    uint8_t result;
    uint8_t rsize=1;
    
    LOGI(("XIRTracking::getStatus()\n"));

    if (access(XIRTracking_API_getStatus, NULL, 0, &result, &rsize) != 0) {
        LOGE(("access XIRTracking_API_getStatus failed\n"));
        return 0;
    } else {
        LOGV(("access XIRTracking_API_getStatus, result is %d\n", result));
        return result;
    }
}

int XIRTracking::registerEvent(uint8_t evt, uint8_t status)
{
	LOGI(("XIRTracking::registerEvent(%d, %d)\n", evt, status));
	
	if (evt == XIRTracking_EVT_equal) {
        mEvtMark = (1 << XIRTracking_EVT_equal);
        mEvtStatus = 0;
		if (notifyRegister(XIRTracking_EVT_equal, &status, 1) != 0) {
			LOGE(("notifyRegister XIRTracking_EVT_equal failed\n"));
            mEvtMark = 0;
            mEvtStatus = 0;
			return -1;
		}
	} else if (evt == XIRTracking_EVT_diff) {
        mEvtMark = (1 << XIRTracking_EVT_diff);
        mEvtStatus = 0;
		if (notifyRegister(XIRTracking_EVT_diff, &status, 1) != 0) {
			LOGE(("notifyRegister XIRTracking_EVT_diff failed\n"));
            mEvtMark = 0;
            mEvtStatus = 0;
			return -1;
		}
	}
    return 0;
}

bool XIRTracking::queryEventStatus()
{
	LOGI(("XIRTracking::queryEventStatus()\n"));
	
	if (mEvtMark == mEvtStatus) {
		mEvtMark = 0;
		mEvtStatus = 0;
		LOGV(("XIRTracking Query Event OK\n"));
		return true;
	} else {
		return false;
	}
}

void XIRTracking::notifyHandler(uint8_t evt, const uint8_t *param, uint8_t psize)
{
    LOGI(("XIRTracking::notifyHandler(%d)\n", evt));

    /*!< FIXME: */
    if ((evt == XIRTracking_EVT_equal) && (mEvtMark & (1<<XIRTracking_EVT_equal))) {
		mEvtStatus = 1 << XIRTracking_EVT_equal;
        LOGV(("** wakeup signal **\n"));
    }
    else if ((evt == XIRTracking_EVT_diff) && (mEvtMark & (1<<XIRTracking_EVT_diff))) {
		mEvtStatus = 1 << XIRTracking_EVT_diff;
        LOGV(("** wakeup signal **\n"));
    }
}
