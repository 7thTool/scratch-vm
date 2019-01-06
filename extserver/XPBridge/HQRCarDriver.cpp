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


#include "HQRCarDriver.hpp"


#if 0
#define LOGV(x)     printf x
#define LOGI(x)     printf x
#define LOGW(x)     printf x
#define LOGE(x)     printf x
#else
#define LOGV(x)
#define LOGI(x)
#define LOGW(x)
#define LOGE(x)
#endif


HQRCarDriver::HQRCarDriver() :
    XPServer(HQRSVR_TYPE_CARDRIVER)
{
    LOGI(("HQRCarDriver::HQRCarDriver()\n"));
}

HQRCarDriver::~HQRCarDriver()
{
    LOGI(("HQRCarDriver::~HQRCarDriver()\n"));
}

int HQRCarDriver::setup(XDualDCMotor *ddm)
{
    uint8_t param = ddm->uid();

    LOGI(("HQRCarDriver::setup(%d)\n", param));
    return XPServer::setup(&param, 1);
}

void HQRCarDriver::forward(uint8_t speed)
{
    LOGI(("HQRCarDriver::forward(%d)\n", speed));
    access(HQRCarDriver_API_forward, (const uint8_t *)&speed, 1, NULL, 0);
}

void HQRCarDriver::backward(uint8_t speed)
{
    LOGI(("HQRCarDriver::backward(%d)\n", speed));
    access(HQRCarDriver_API_backward, (const uint8_t *)&speed, 1, NULL, 0);
}

void HQRCarDriver::turn(uint8_t action, uint8_t speed, uint8_t angle)
{
    uint8_t param[3], *p=param;

    LOGI(("HQRCarDriver::turn(%d, %d, %d)\n", action, speed, angle));
    p = fillU8(p, action);
    p = fillU8(p, speed);
    p = fillU8(p, angle);
    access(HQRCarDriver_API_turn, (const uint8_t *)param, 3, NULL, 0);
}

void HQRCarDriver::stop()
{
    LOGI(("HQRCarDriver::stop()\n"));
    access(HQRCarDriver_API_stop, NULL, 0, NULL, 0);
}

void HQRCarDriver::autoLineTracking(XIRTracking *irt, uint8_t speed)
{
    uint8_t param[2], *p=param;

    LOGI(("HQRCarDriver::autoLineTracking(%d, %d)\n", irt->uid(), speed));
    p = fillU8(p, irt->uid());
    p = fillU8(p, speed);
    access(HQRCarDriver_API_autoLineTracking, (const uint8_t *)param, 2, NULL, 0);
}

void HQRCarDriver::autoObstacleAvoidance(XUltrasonic *uls, uint8_t speed)
{
    uint8_t param[2], *p=param;

    LOGI(("HQRCarDriver::autoObstacleAvoidance(%d, %d)\n", uls->uid(), speed));
    p = fillU8(p, uls->uid());
    p = fillU8(p, speed);
    access(HQRCarDriver_API_autoObstacleAvoidance, (const uint8_t *)param, 2, NULL, 0);
}
