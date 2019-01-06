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


#ifndef __XSEGDISPLAY_H__
#define __XSEGDISPLAY_H__


#include "XPBridge.hpp"


#define XSegDisplay_API_showNumber		(0x01)	/*!< void showNumber(float num) */
#define XSegDisplay_API_clear			(0x02)	/*!< void clear() */


class XSegDisplay : public XPBlock {
public:
   XSegDisplay();
   ~XSegDisplay();

   void showNumber(float num);
   void clear();
};


#endif	// __XSEGDISPLAY_H__
