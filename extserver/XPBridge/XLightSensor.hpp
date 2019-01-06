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


#ifndef __XLIGHTSENSOR_H__
#define __XLIGHTSENSOR_H__


#include "XPBridge.hpp"


#define XLightSensor_API_getLuminance 		(1)	/*!< uint8_t getLuminance() */

/*	pdata
 *		- register: great threshold value
 *		- notify: nouse
 */
#define XLightSensor_EVT_greatThreshold		(1)

/*	pdata
 *		- register: little threshold value
 *		- notify: nouse
 */
#define XLightSensor_EVT_littleThreshold	(2)


class XLightSensor : public XPBlock {
public:
    XLightSensor();
    ~XLightSensor();

    uint8_t getLuminance();

	int registerEvent(uint8_t evt, uint8_t value);
	bool queryEventStatus();

private:
    void notifyHandler(uint8_t evt, const uint8_t *param, uint8_t psize);

    uint8_t mEvtMark;
	uint8_t mEvtStatus;
};


#endif //__XLIGHTSENSOR_H__
