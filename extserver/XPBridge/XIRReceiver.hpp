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


#ifndef __XIRRECEIVER_H__
#define __XIRRECEIVER_H__


#include "XPBridge.hpp"


#define XIR_KEY_0		(0)		// '0'
#define XIR_KEY_1		(1)		// '1'
#define XIR_KEY_2		(2)		// '2'
#define XIR_KEY_3		(3)		// '3'
#define XIR_KEY_4		(4)		// '4'
#define XIR_KEY_5		(5)		// '5'
#define XIR_KEY_6		(6)		// '6'
#define XIR_KEY_7		(7)		// '7'
#define XIR_KEY_8		(8)		// '8'
#define XIR_KEY_9		(9)		// '9'
#define XIR_KEY_A		(10)	// 'A'
#define XIR_KEY_B		(11)	// 'B'
#define XIR_KEY_C		(12)	// 'C'
#define XIR_KEY_D		(13)	// 'D'
#define XIR_KEY_E		(14)	// 'E'
#define XIR_KEY_F		(15)	// 'F'
#define XIR_KEY_UP		(16)	// UP
#define XIR_KEY_DOWN	(17)	// DOWN
#define XIR_KEY_LEFT	(18)	// LEFT
#define XIR_KEY_RIGHT	(19)	// RIGHT
#define XIR_KEY_OK		(20)	// OK


#define XIRReceiver_API_receive		(2)	/*!< uint8_t receive() */

/*	pdata
 *		- register: nouse
 *		- notify: [0]--"uint8_t key"
 */
#define XIRReceiver_EVT_received	(1)


class XIRReceiver : public XPBlock  {
public:
	XIRReceiver();
	~XIRReceiver();

    uint8_t receive();

	int registerEvent(uint8_t evt);
	bool queryEventStatus();

private:
    void notifyHandler(uint8_t evt, const uint8_t *param, uint8_t psize);

    uint8_t mEvtMark;
    uint8_t mKey;
    uint8_t mEvtStatus;
};


#endif //__XIRRECEIVER_H__
