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


#ifndef __XBUZZER_H__
#define __XBUZZER_H__


#include "XPBridge.hpp"


#define XBuzzer_API_playTone 			(0x01)	/*!< void playTone(uint16_t frequency, uint32_t duration) */


class XBuzzer : public XPBlock {
public:
	XBuzzer();
	~XBuzzer();

	void playTone(uint16_t frequency, uint32_t duration);
};


#endif //__XBUZZER_H__
