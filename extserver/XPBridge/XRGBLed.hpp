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


#ifndef __XRGBLED_H__
#define __XRGBLED_H__


#include "XPBridge.hpp"


#define XRGBLed_API_setColor		(0x01)	/*!< void setColor(uint8_t index, uint8_t red, uint8_t green, uint8_t blue) */
#define XRGBLed_API_setAllColor		(0x02)	/*!< void setAllColor(uint8_t red, uint8_t green, uint8_t blue) */
#define XRGBLed_API_show			(0x03)	/*!< void show() */
#define XRGBLed_API_showColor		(0x04)	/*!< void showColor(uint8_t index, uint8_t red, uint8_t green, uint8_t blue) */
#define XRGBLed_API_showAllColor	(0x05)	/*!< void showAllColor(uint8_t red, uint8_t green, uint8_t blue) */
#define XRGBLed_API_clear			(0x06)	/*!< void clear() */


class XRGBLed : public XPBlock {
public:
	XRGBLed();
	~XRGBLed();

	void setColor(uint8_t index, uint8_t red, uint8_t green, uint8_t blue);

	void setAllColor(uint8_t red, uint8_t green, uint8_t blue);

	void show();

	void showColor(uint8_t index, uint8_t red, uint8_t green, uint8_t blue);

	void showAllColor(uint8_t red, uint8_t green, uint8_t blue);

	void clear();
};


#endif //__XRGBLED_H__
