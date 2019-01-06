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


#ifndef __HQRLIGHTSHOW_H__
#define __HQRLIGHTSHOW_H__


#include "XPBridge.hpp"
#include "XRGBLed.hpp"
#include "HQRover.hpp"


#define HQRLightShow_API_showColor		(1)	/*!< void showColor(uint8_t index, uint8_t red, uint8_t green, uint8_t blue) */
#define HQRLightShow_API_showMeteor		(2)	/*!< void showMeteor(uint8_t speed, uint8_t red, uint8_t green, uint8_t blue) */
#define HQRLightShow_API_showBreath		(3)	/*!< void showBreath(uint8_t index, uint8_t speed, uint8_t red, uint8_t green, uint8_t blue) */
#define HQRLightShow_API_clear			(4)	/*!< void clear(uint8_t index) */


class HQRLightShow : public XPServer {
public:
	HQRLightShow();
	~HQRLightShow();

    int setup(XRGBLed *rgb);

	void showColor(uint8_t index, uint8_t red, uint8_t green, uint8_t blue, uint8_t clearOthers=0);

	void showMeteor(uint8_t speed, uint8_t red, uint8_t green, uint8_t blue);

	void showBreath(uint8_t index, uint8_t speed, uint8_t red, uint8_t green, uint8_t blue);

	void clear(uint8_t index);
};


#endif //__HQRLIGHTSHOW_H__
