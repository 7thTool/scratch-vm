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


#ifndef __HQRTAILLIGHT_H__
#define __HQRTAILLIGHT_H__


#include "XPBridge.hpp"
#include "XSegDisplay.hpp"
#include "HQRover.hpp"


#define HQRTaillight_API_showNumber		(1)	/*!< void showNumber(float num) */
#define HQRTaillight_API_showEffect		(2)	/*!< void showEffect(uint8_t effect) */
#define HQRTaillight_API_clear			(3)	/*!< void clear() */


#define HQRTaillight_EFFECT_FORWARD		(1)
#define HQRTaillight_EFFECT_BACKWARD	(2)
#define HQRTaillight_EFFECT_AOA         (3)
#define HQRTaillight_EFFECT_ALT         (4)


class HQRTaillight : public XPServer {
public:
	HQRTaillight();
	~HQRTaillight();

    int setup(XSegDisplay *sgd);

	void showNumber(float num);

	void showEffect(uint8_t effect);

	void clear();
};


#endif //__HQRTAILLIGHT_H__
