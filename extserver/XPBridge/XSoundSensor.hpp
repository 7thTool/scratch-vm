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


#ifndef __XSOUNDSENSOR_H__
#define __XSOUNDSENSOR_H__


#include "XPBridge.hpp"


#define XSoundSensor_API_getVolume 			(1)	/*!< uint8_t getVolume() */

/*	pdata
 *		- register: great threshold value
 *		- notify: nouse
 */
#define XSoundSensor_EVT_greatThreshold		(1)

/*	pdata
 *		- register: little threshold value
 *		- notify: nouse
 */
#define XSoundSensor_EVT_littleThreshold	(2)


class XSoundSensor : public XPBlock {
public:
    XSoundSensor();
    ~XSoundSensor();

    uint8_t getVolume();

	int registerEvent(uint8_t evt, uint8_t value);
	bool queryEventStatus();

private:
    void notifyHandler(uint8_t evt, const uint8_t *param, uint8_t psize);

    uint8_t mEvtMark;
	uint8_t mEvtStatus;
};


#endif //__XSOUNDSENSOR_H__
