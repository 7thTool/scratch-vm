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


#include "XIRReceiver.hpp"


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


XIRReceiver::XIRReceiver() :
    XPBlock(XBLOCK_TYPE_XIRReceiver),
    mEvtMark(0),
    mKey(0xFF)
{
    LOGI(("XIRReceiver::XIRReceiver()\n"));
}

XIRReceiver::~XIRReceiver()
{
    LOGI(("XIRReceiver::~XIRReceiver()\n"));
}

uint8_t XIRReceiver::receive()
{
	LOGI(("XIRReceiver::receive()\n"));
	return mKey;
}

int XIRReceiver::registerEvent(uint8_t evt)
{
	LOGI(("XIRReceiver::registerEvent(%d)\n", evt));
	
	if (evt == XIRReceiver_EVT_received) {
        mEvtMark = (1 << XIRReceiver_EVT_received);
        mEvtStatus = 0;
		mKey = 0xFF;
		if (notifyRegister(XIRReceiver_EVT_received, NULL, 0) != 0) {
			LOGE(("notifyRegister XIRReceiver_EVT_received failed\n"));
            mEvtMark = 0;
            mEvtStatus = 0;
			return -1;
		}
	}
	
	return 0;
}

bool XIRReceiver::queryEventStatus()
{
	LOGI(("XIRReceiver::queryEventStatus()\n"));
	
	if (mEvtMark == mEvtStatus) {
		mEvtMark = 0;
		mEvtStatus = 0;
		LOGV(("XIRReceiver Query Event OK\n"));
		return true;
	} else {
		return false;
	}
}

void XIRReceiver::notifyHandler(uint8_t evt, const uint8_t *param, uint8_t psize)
{
    LOGI(("XIRReceiver::notifyHandler(%d)\n", evt));

    if ((evt == XIRReceiver_EVT_received) && (mEvtMark & (1<<XIRReceiver_EVT_received))) {
		mEvtStatus = 1 << XIRReceiver_EVT_received;
        fetchU8(param, &mKey);
        LOGV(("** wakeup signal **\n"));
    }
}
