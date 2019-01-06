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


#ifndef __XDualDCMotor_H__
#define __XDualDCMotor_H__


#include "XPBridge.hpp"


#define XDualDCMotor_API_setMotorSpeed	(0x01)	/*!< void setMotorSpeed(uint8_t motor, int8_t speed) */
#define XDualDCMotor_API_setAllSpeed	(0x02)	/*!< void setAllSpeed(int8_t speed1, int8_t speed2) */
#define XDualDCMotor_API_stopMotor		(0x03)	/*!< void stopMotor(uint8_t motor) */
#define XDualDCMotor_API_stopAllMotor	(0x04)	/*!< void stopAllMotor() */


class XDualDCMotor : public XPBlock {
public:
	XDualDCMotor();
	~XDualDCMotor();

	void setMotorSpeed(uint8_t motor, int8_t speed);

	void setAllSpeed(int8_t speed1, int8_t speed2);

	void stopMotor(uint8_t motor);

	void stopAllMotor();
};


#endif //__XDualDCMotor_H__
