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


#ifndef __XIRTRACKING_H__
#define __XIRTRACKING_H__


#include "XPBridge.hpp"


#define XIRTracking_API_getStatus	(1)	/*!< uint8_t getStatus() */

/*	pdata
 *		- register: equal value to compare
 *		- notify: nouse
 */
#define XIRTracking_EVT_equal		(1)

/*	pdata
 *		- register: diff value to compare
 *		- notify: nouse
 */
#define XIRTracking_EVT_diff		(2)


class XIRTracking : public XPBlock  {
public:
	XIRTracking();
	~XIRTracking();

    uint8_t getStatus();
	
	int registerEvent(uint8_t evt, uint8_t status);
	bool queryEventStatus();

private:
    void notifyHandler(uint8_t evt, const uint8_t *param, uint8_t psize);

    uint8_t mEvtMark;
	uint8_t mEvtStatus;
};


#endif //__XIRTRACKING_H__
