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


#ifndef __XUlTRASONIC_H__
#define __XUlTRASONIC_H__


#include "XPBridge.hpp"


#define XUltrasonic_API_getDistance			(1)	/*!< uint16_t getDistance() */

/*	pdata
 *		- register: great threshold value
 *		- notify: nouse
 */
#define XUltrasonic_EVT_greatThreshold		(1)

/*	pdata
 *		- register: little threshold value
 *		- notify: nouse
 */
#define XUltrasonic_EVT_littleThreshold		(2)


class XUltrasonic : public XPBlock {
public:
	XUltrasonic();
	~XUltrasonic();

	uint16_t getDistance();

	int registerEvent(uint8_t evt, uint8_t value);
	bool queryEventStatus();

private:
    void notifyHandler(uint8_t evt, const uint8_t *param, uint8_t psize);

    uint8_t mEvtMark;
	uint8_t mEvtStatus;
};


#endif //__XUlTRASONIC_H__
