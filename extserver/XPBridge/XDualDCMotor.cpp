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


#include "XDualDCMotor.hpp"


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


XDualDCMotor::XDualDCMotor() :
    XPBlock(XBLOCK_TYPE_XDualDCMotor)
{
    LOGI(("XDualDCMotor::XDualDCMotor()\n"));
}

XDualDCMotor::~XDualDCMotor()
{
    LOGI(("XDualDCMotor::~XDualDCMotor()\n"));
}

void XDualDCMotor::setMotorSpeed(uint8_t motor, int8_t speed)
{
    uint8_t param[2], *p=param;

    LOGI(("XDualDCMotor::setMotorSpeed(%d, %d)\n", motor, speed));

    p = fillU8(p, motor);
    p = fillU8(p, speed);
    access(XDualDCMotor_API_setMotorSpeed, (const uint8_t *)param, 2, NULL, 0);
}

void XDualDCMotor::setAllSpeed(int8_t speed1, int8_t speed2)
{
    uint8_t param[2], *p=param;

    LOGI(("XDualDCMotor::setAllSpeed(%d, %d)\n", speed1, speed2));

    p = fillU8(p, speed1);
    p = fillU8(p, speed2);
    access(XDualDCMotor_API_setAllSpeed, (const uint8_t *)param, 2, NULL, 0);
}

void XDualDCMotor::stopMotor(uint8_t motor)
{
    LOGI(("XDualDCMotor::stopMotor(%d)\n", motor));

    access(XDualDCMotor_API_stopMotor, (const uint8_t *)&motor, 1, NULL, 0);
}

void XDualDCMotor::stopAllMotor()
{
    LOGI(("XDualDCMotor::stopAllMotor()\n"));

    access(XDualDCMotor_API_stopAllMotor, NULL, 0, NULL, 0);
}
