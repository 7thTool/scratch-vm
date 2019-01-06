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


#ifndef __HQRCARDRIVER_H__
#define __HQRCARDRIVER_H__


#include "XPBridge.hpp"
#include "XDualDCMotor.hpp"
#include "XIRTracking.hpp"
#include "XUltrasonic.hpp"
#include "HQRover.hpp"


#define HQRCarDriver_API_forward                (1)	/*!< void forward(uint8_t speed) */
#define HQRCarDriver_API_backward	            (2)	/*!< void backward(uint8_t speed) */
#define HQRCarDriver_API_turn                   (3)	/*!< void turn(uint8_t action, uint8_t speed, uint8_t angle) */
#define HQRCarDriver_API_stop                   (4)	/*!< void stop() */
#define HQRCarDriver_API_autoLineTracking       (5)	/*!< void autoLineTracking(XIRTracking *irt, uint8_t speed) */
#define HQRCarDriver_API_autoObstacleAvoidance  (6)	/*!< void autoObstacleAvoidance(XUltrasonic *uls, uint8_t speed) */


class HQRCarDriver : public XPServer {
public:
	HQRCarDriver();
	~HQRCarDriver();

    int setup(XDualDCMotor *ddm);

    void forward(uint8_t speed);

    void backward(uint8_t speed);

	void turn(uint8_t action, uint8_t speed, uint8_t angle);

    void stop();
	
    void autoLineTracking(XIRTracking *irt, uint8_t speed);
	
	void autoObstacleAvoidance(XUltrasonic *uls, uint8_t speed);
};


#endif //__HQRCARDRIVER_H__
