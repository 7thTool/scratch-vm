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


#ifndef __XBUTTON_H__
#define __XBUTTON_H__


#include "XPBridge.hpp"


#define XButton_API_isPressed		(1)	/*!< uint8_t isPressed() */
#define XButton_API_isKnocked		(2)	/*!< uint8_t isKnocked() */

/*	pdata
 *		- register: nouse
 *		- notify: nouse
 */
#define XButton_EVT_knocked			(1)


class XButton : public XPBlock {
public:
	XButton();
	~XButton();

	/**
     *  \brief  Check if the key is pressed.
	 *  \param  none
	 *  \return 1 if key is pressed, 0 if not pressed.
	 */
	uint8_t isPressed();
	
    /**
     *  \brief  Check if the key is knocked.
	 *  \param  none
	 *  \return 1 if key is knocked, 0 if not knoeced.
	 */
	uint8_t isKnocked();


	int registerEvent(uint8_t evt);
	bool queryEventStatus();
	
private:
    void notifyHandler(uint8_t evt, const uint8_t *param, uint8_t psize);

    uint8_t         mEvtMark;
	uint8_t			mEvtStatus;
};


#endif	// __XBUTTON_H__
