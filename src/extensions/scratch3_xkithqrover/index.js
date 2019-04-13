const ArgumentType = require('../../extension-support/argument-type');
const BlockType = require('../../extension-support/block-type');
const log = require('../../util/log');
const cast = require('../../util/cast');
const formatMessage = require('format-message');
const XBridge = require('../../io/xbridge');
const Base64Util = require('../../util/base64-util');

/**
 * Icon png to be displayed at the left edge of each extension block, encoded as a data URI.
 * @type {string}
 */
// eslint-disable-next-line max-len
const blockIconURI = 'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACgAAAAoCAYAAACM/rhtAAAI7UlEQVRYw+2YeVRT2R3HHwgeqaLGsVbaqWNt6bEKiDIo4gIossQIiKiRJYSETbaAEAXZI5skEIJgAAkhQAQREBxUQIZlEMERHMWlbigtTqdVx30Zis63L88DlSN1tO3M+Ae/c77n3dz7ct/n97v3d+99jyDGbMzGbMz+s5k3axDMgiUEU76DVBxZDiI2ySzI8uyfGcxcY5xTWqb6ZtkACYMhqW2SDk5wiLxiytku+Vm4pjKlQ5FRixKJm72jBWDyo/sm2fHaJ9ptO7slIqlZocjvbzi0v+UnAUrpspyS1mbLijtqtco9S1EQlVv1wlha9pwQKRB5oAKiUiUWZBeB/P1Cb1fRfb60/HlDdSmiipUtRJoilhAWcgmR3JjIzdV8vd/p1sE6U5zjvP8rqMxO28npbda89DbbAtft1n0OsRt6l61d83JZVDI6OpvwaX6pCgg6mRTYsL7sbMafu1qHgEdKWPhCLXnftSn+UfwZdmEGv3BNGCA8hCDYqYbvDcjgMLYxox1umG91fGxstAL6FgwsWmyOieGpKDuugB4JkFOXBwtFASZnFEFNJMdCuQCJjaHIbg6CQYDnG4CaO9PxEYd/eoadp2C8W+K9iSxBsQYr5VuCK/rk3cnE8qlkZxm/9Y8YXL3aBguXWsLQdA2m+4RDPTkfsmN58Dsgp6AW5e1HyiEFLPJKMD06CWtCXWEVz8ZiUzPYu9hjdkoaBTYuNgsT3UPPNzTUFtdXlzrRnGO+UndP2T3VJbZVnZWS/M5sU/jJVkRqwSNVpxpJ+/BxYCRmePFHROH3e4pQ0ZgPlqIUObUlKGsQU/W/5CdA5ZChnRNE21bBi2sLYaQtOCFO92lr3deq+r/c1XogRyYLnMBK7NLZHJ47zj255L2GVYfu4TNnk891nS2BA9ouPExk86HlH4sJAXEYH7abjMQeqCXlvzm3SGkJsqDL5WHuJg/Y8TaiZs/C50kxFp1FqfSm3CymhEiqMjt/qumyZ+D2v3h5+b909vAbXGfndHXzZud3TxIjIyPNQqmkpCgv69GnJibmusssHGeZM3izVq0L/rXlBuE0e269NjPkrvaWUGhv2YZJzBBoM3nQduZhEukQjbz+gR0KjbBczBVl43bVb2C5KwZhAneoJx6CSCyBj7c/uBxfrGPYQ5GTCT4v8KWhofnUH4QzNTW1N1+56rGF+WqMKrNVWLnCDMuXrcDy5StgQl7n26zHLOcAaG3PBUECRFccw+G6I1R5WkoxOhQG8IlkY2l8Emjx+yk4lRwcnHC2rR53es+i9VgluGx24ds3hOXmlnSrdeS88YFMJkNiQiLiYuPAD90OO/p6MGzsh7Vlowv8twbCm+sLhq09LC2syKRYDq0dedhV+QpwvKAMQXFs+MZwoBslIoGrsCQsdRiQH8zDo/5LlNobqlGYI7nz9iXF2s43PlaA/v5+qOzhw4coKCiARCJBgF8QBaaCr66uxsWLF1FRUUG1CeIFlAPWlrb42DUEmWR9U10FsqKtII62hU5sDhVNdUEFNviGfqeCc3F2x15x6jDgmdZj2C+T4q2AdBv7oLLSMgqu7LPjMHbgoLOzk4KIjoqhAJuamqj2azf70fvXryGVSql2XmAI1W7naImm8nS0HFYgNJYF2q5iCk4rSok5PiJwvAKo6G1w3IwT9dVoqZahXJGDM211KC0gHZmb22qlLz953UB+sk+voH3EJk63tvdsaW6hANj8XdBZwsDZngsUwM7wSLiRXvf29uJoSwf0bV1RfuRztLa2jnCA7miDymoFlFUHoJZQBSKhamDcdvkXk7jCe1oeu0fMPxVg3UEpasrk+LL5CMoKcwYJfUXnY/3CTszLboDh/jPQk7cnDmfvnsPJCYJECrC9qwdB8elUBDMyMsBy8YADwxGXLl3Cye7zMGP64e69B8OAEeE7KUAd7xgskx7G0r01IAQHbxFbs/9GbmE3iGhlueYogEND3HKkArvjY3oJA2UX5kmPn58ZLp+9oKQLC5TdMFSeaTWsuXJjSc8/YV3SjPaODgwMDFAw+/btgyfHezg5FIoiyoG+W9/gydNnUCqVrxxw84AFJ/C5fsvfMa/p69s0SeMdgiO8RESX7iT89goJfuEt1X7L8gocMcQquG+uduP44TL4eXllE/ry9vsGJBgZyUeqCA5pUV0fVIDM+j6YnX5MXnuwKSVnROaqxCSzt1BeSEU2n8z0iGwZ6M291H+H5HbuIdl/BzT8s1QHgEMEWzyV2FF8mPBMf0b3DP1eBahyqKpYRgF2f3EMTZ+Vw8TEZDbxpz31fqrIvQ5HAda+esjBGAUyxbWvHtb1BBbCIliTOwOd4TQMabuRDctwIUw67w9Dba26gMZtWVSZXd0NWmgeaKo56JE6SMQf3Eh4issIb8kNE8+IrSTgIDXMduvRerQSzbUH4evp+e/tTldYlWBQdGoE4MKaqyOiMCTXkk4s/er5G/Xrjl0f8dum/VsKknHuGYwS9kObJwWNzF6CLWwlIpSNBCf13tBxypcbsMzby7+GXGq61qyxuW1qujLujSVFN/nAlvk5jU+oOaiCrLgwKiCj7gZ2xhchILMWW/fWITS5HOX+qSgOzRz1fq/Pr0PNRQDt4BxMiSsHwUmrJ0Lyb5Jwuu9/GvXO1dRNrdytt6/t5QIS0rj76agPXXJuAPSGPjjUXqGAa3gZUAZnvHHf5u77+CSMXMucBaCFk0excPIA4SHsI5zFc/6no/ys8L00XWGleH7BiWsk5N1RIV/TylMPYHHi7oi69WefQI+cvyo4lT6KL3s50zftIuEmnPF/fe+YfRMTjM8PbCKjJlzc/bzN6PTTpz8EbNbzHdblH/9+CE4lTR9x5o/+kjTeMVY+bqPkjoZTOjScxND0LIU2vxaTww7hV8GZD/4ob/EiAY1EPXdCZgZl4XVAwkWg/6MDTrFy+x1tLZs5jc6Km7aWJSBPxLU0OquFlHKyNXfa0H2Snn/s1vJIGYYbz0o++0F9UMjoub1RNyJ/CPB7gilY8cF99QhrvMzVi1VUfpBwYzZmY/YT2r8Act99/XWq6mIAAAAASUVORK5CYII=';

// TODO: Needs comment
const XBridgeTimeout = 4500; // TODO: might need tweaking based on how long the peripheral takes to start sending data

/**
 * A time interval to wait (in milliseconds) while a block that sends a XBridge message is running.
 * @type {number}
 */
const XBridgeSendInterval = 20;

const BYTE_INVALID_VALUE = 0xff;
const SHORT_INVALID_VALUE = 0xffff;
const FLOAT_INVALID_VALUE = 999;

/**
 * Manage communication with a XKitHqrover peripheral over a Scrath Link client socket.
 */
class XKitHqrover {

    /**
     * Construct a XKitHqrover communication object.
     * @param {Runtime} runtime - the Scratch 3.0 runtime
     * @param {string} extensionId - the id of the extension
     */
    constructor (runtime, extensionId) {

        /**
         * The Scratch 3.0 runtime used to trigger the green flag button.
         * @type {Runtime}
         * @private
         */
        this._runtime = runtime;

        /**
         * The BluetoothLowEnergy connection socket for reading/writing peripheral data.
         * @type {XBridge}
         * @private
         */
        this._xbridge = null;
        this._runtime.registerPeripheralExtension(extensionId, this);

        /**
         * The id of the extension this peripheral belongs to.
         */
        this._extensionId = extensionId;

        this._blocks = {};
        this._services = {};

        /**
         * Interval ID for data reading timeout.
         * @type {number}
         * @private
         */
        this._timeoutID = null;

        /**
         * A flag that is true while we are busy sending data to the XBridge socket.
         * @type {boolean}
         * @private
         */
        this._busy = false;

        this._request = null;

        /**
         * ID for a timeout which is used to clear the busy flag if it has been
         * true for a long time.
         */
        this._busyTimeoutID = null;

        this.disconnect = this.disconnect.bind(this);
        this._onConnect = this._onConnect.bind(this);
        this._onReceive = this._onReceive.bind(this);
    }

    clearAll() {
        this._blocks = {};
        this._services = {};
        this._request = null;
        this._busy = false;
        if(this._timeoutID)
            window.clearInterval(this._timeoutID);
    }

    isOnBoardBlock(port) {
        return isNaN(port);
    }

    constructBlock(type, model, port) {
        var block = {
            type,
            model,
            port
        };
        switch (type) {
            case 'XButton': {
                block.status = BYTE_INVALID_VALUE;
            }
            break;
        case 'XIRReceiver': {
                block.key = BYTE_INVALID_VALUE;
            }
            break;
        case 'XIRTracking': {
                block.status = BYTE_INVALID_VALUE;
            }
            break;
        case 'XLightSensor': {
                block.luminance = BYTE_INVALID_VALUE;
            }
            break;
        case 'XSoundSensor': {
                block.volume = BYTE_INVALID_VALUE;
            }
            break;
        case 'XUltrasonic': {
                block.distance = FLOAT_INVALID_VALUE;
            }
            break;
        }
        this._blocks[port] = block;
        const params = {
            type,
            model,
            port
        };
        this.send("constructBlock", params, (result) => {
            switch (type) {
                case 'XButton': {
                    if(result) 
                        block.status = result.status;
                    this._request.resolve(block.status);
                }
                break;
            case 'XIRReceiver': {
                    if(result) 
                        block.key = result.key;
                    this._request.resolve(block.key);
                }
                break;
            case 'XIRTracking': {
                    if(result) 
                        block.status = result.status;
                    this._request.resolve(block.status);
                }
                break;
            case 'XLightSensor': {
                    if(result) 
                        block.luminance = result.luminance;
                    this._request.resolve(block.luminance);
                }
                break;
            case 'XSoundSensor': {
                    if(result) 
                        block.volume = result.volume;
                    this._request.resolve(block.volume);
                }
                break;
            case 'XUltrasonic': {
                    if(result) 
                        block.distance = result.distance;
                    this._request.resolve(block.distance);
                }
                break;
            }
        });
        return block;
    }

    findBlock(port) {
        return this._blocks[port];
    }
    
    findOrConstructBlock(type, model, port) {
        var block = this._blocks[port];
        if (!block) {
            block = this.constructBlock(type, model, port);
        } 
        return block;
    }

    constructServer(type, ports) {
        var key = type + ',' + ports;
        var server = {
            type,
            ports
        };
        this._services[key] = server;
        const params = {
            type,
            ports
        };
        this.send("constructServer", params, (result) => {
            //
        });
        return server;
    }
    
    findServer(type, ports) {
        var key = type + ',' + ports;
        return this._services[key];
    }

    findOrConstructServer(type, ports) {
        var key = type + ',' + ports;
        var server = this._services[key];
        if (!server) {
            server = this.constructServer(type, ports);
        } 
        return server;
    }
    
    HQRCarDriver_forward(speed) {
        const block = this.findOrConstructBlock('XDualDCMotor', '', XDualDCMotorPorts.PORT1);
        if (block) {
            const server = this.findOrConstructServer('HQRCarDriver', XDualDCMotorPorts.PORT1);
            if (server) {
                const params = {
                    type: server.type,
                    ports: server.ports,
                    ddm_model: block.model,
                    ddm_port: block.port,
                    speed
                };
                this.send("HQRCarDriver_forward", params, (result) => {
                    if(this._request)
                        this._request.resolve();
                });
            }
        }
    }

    HQRCarDriver_backward(speed) {
        const block = this.findOrConstructBlock('XDualDCMotor', '', XDualDCMotorPorts.PORT1);
        if (block) {
            const server = this.findOrConstructServer('HQRCarDriver', XDualDCMotorPorts.PORT1);
            if (server) {
                const params = {
                    type: server.type,
                    ports: server.ports,
                    ddm_model: block.model,
                    ddm_port: block.port,
                    speed
                };
                this.send("HQRCarDriver_backward", params, (result) => {
                    if(this._request)
                        this._request.resolve();
                });
            }
        }
    }

    HQRCarDriver_turn(action, speed, angle) {
        const block = this.findOrConstructBlock('XDualDCMotor', '', XDualDCMotorPorts.PORT1);
        if (block) {
            const server = this.findOrConstructServer('HQRCarDriver', XDualDCMotorPorts.PORT1);
            if (server) {
                const params = {
                    type: server.type,
                    ports: server.ports,
                    ddm_model: block.model,
                    ddm_port: block.port,
                    action,
                    speed,
                    angle
                };
                this.send("HQRCarDriver_turn", params, (result) => {
                    if(this._request)
                        this._request.resolve();
                });
            }
        }
    }

    HQRCarDriver_stop() {
        const block = this.findOrConstructBlock('XDualDCMotor', '', XDualDCMotorPorts.PORT1);
        if (block) {
            const server = this.findOrConstructServer('HQRCarDriver', XDualDCMotorPorts.PORT1);
            if (server) {
                const params = {
                    type: server.type,
                    ports: server.ports,
                    ddm_model: block.model,
                    ddm_port: block.port
                };
                this.send("HQRCarDriver_stop", params, (result) => {
                    if(this._request)
                        this._request.resolve();
                });
            }
        }
    }

    HQRCarDriver_autoLineTracking(irt_port,speed) {
        const block = this.findOrConstructBlock('XDualDCMotor', '', XDualDCMotorPorts.PORT1);
        if (block) {
            const server = this.findOrConstructServer('HQRCarDriver', XDualDCMotorPorts.PORT1);
            if (server) {
                const irt_block = this.findOrConstructBlock('XIRTracking', XIRTrackingModels.MODEL1, irt_port);
                if (irt_block) {
                    const params = {
                        type: server.type,
                        ports: server.ports,
                        ddm_model: block.model,
                        ddm_port: block.port,
                        irt_model: block.model,
                        irt_port,
                        speed
                    };
                    this.send("HQRCarDriver_autoLineTracking", params, (result) => {
                        if(this._request)
                            this._request.resolve();
                    });
                }
            }
        }
    }

    HQRCarDriver_autoObstacleAvoidance(uls_port,speed) {
        const block = this.findOrConstructBlock('XDualDCMotor', '', XDualDCMotorPorts.PORT1);
        if (block) {
            const server = this.findOrConstructServer('HQRCarDriver', XDualDCMotorPorts.PORT1);
            if (server) {
                const uls_block = this.findOrConstructBlock('XUltrasonic', XUltrasonicModels.MODEL1, uls_port);
                if (uls_block) {
                    const params = {
                        type: server.type,
                        ports: server.ports,
                        ddm_model: block.model,
                        ddm_port: block.port,
                        uls_model: uls_block.model,
                        uls_port,
                        speed
                    };
                    this.send("HQRCarDriver_autoObstacleAvoidance", params, (result) => {
                        if(this._request)
                            this._request.resolve();
                    });
                }
            }
        }
    }

    XSoundSensor_getVolume (port) {
        const block = this.findOrConstructBlock('XSoundSensor', '', XSoundSensorPorts.PORT1);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port
            };
            this.send("XSoundSensor_getVolume", params, (result) => {
                if(result) 
                    block.volume = result.volume;
                if(this._request)
                    this._request.resolve(block.volume);
            });
        }
    }

    XLightSensor_getLuminance (port) {
        const block = this.findOrConstructBlock(' XLightSensor', '', XLightSensorPorts.PORT1);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port
            };
            this.send("XLightSensor_getLuminance", params, (result) => {
                if(result)
                    block.luminance = result.luminance;
                if(this._request)
                    this._request.resolve(block.luminance);
            });
        }
    }

    XButton_isPressed(port) {
        const block = this.findOrConstructBlock('XButton', '', port);
        if (block) {
            return block.status == 1;
        } 
        return false;
    }
    
    XIRTracking_getStatus(port) {
        const block = this.findOrConstructBlock('XIRTracking', XIRTrackingModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port
            };
            this.send("XIRTracking_getStatus", params, (result) => {
                if(result)
                    block.status = result.status;
                if(this._request)
                    this._request.resolve(block.status);
            });
        }
    }
    
    XUltrasonic_getDistance(port) {
        const block = this.findOrConstructBlock('XUltrasonic', XUltrasonicModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port
            };
            this.send("XUltrasonic_getDistance", params, (result) => {
                if(result)
                    block.distance = result.distance;
                if(this._request)
                    this._request.resolve(block.distance);
            });
        }
    }

    XIRAvoiding_getStatus(port) {
        const block = this.findOrConstructBlock('XIRAvoiding', XIRAvoidingModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port
            };
            this.send("XIRAvoiding_getStatus", params, (result) => {
                if(result)
                    block.status = result.status;
                if(this._request)
                    this._request.resolve(block.status);
            });
        }
    }

    XSegDisplay_showNumber(port, num) {
        const block = this.findOrConstructBlock('XSegDisplay', XSegDisplayModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port,
                num
            };
            this.send("XSegDisplay_showNumber", params, (result) => {
                this._request.resolve();
            });
        }
    }

    XSegDisplay_showCharacter (port, index, char) {
        const block = this.findOrConstructBlock('XSegDisplay', XSegDisplayModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port,
                index,
                char
            };
            this.send("XSegDisplay_showCharacter", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }

    XSegDisplay_showSegment (port, index, seg) {
        const block = this.findOrConstructBlock('XSegDisplay', XSegDisplayModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port,
                index,
                seg
            };
            this.send("XSegDisplay_showSegment", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }

    XSegDisplay_clearSegment (port, index, seg) {
        const block = this.findOrConstructBlock('XSegDisplay', XSegDisplayModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port,
                index,
                seg
            };
            this.send("XSegDisplay_clearSegment", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }

    XSegDisplay_clear(port) {
        const block = this.findOrConstructBlock('XSegDisplay', XSegDisplayModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port
            };
            this.send("XSegDisplay_clear", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }
    
    HQRLightShow_showColor (rgb_port, index, red, green, blue) {
        const block = this.findOrConstructBlock('XRGBLed', '', rgb_port);
        if (block) {
            const server = this.findOrConstructServer('HQRLightShow', rgb_port);
            if (server) {
                const params = {
                    type: server.type,
                    ports: server.ports,
                    rgb_model: block.model,
                    rgb_port,
                    index,
                    red,
                    green,
                    blue,
                    clearOthers: true
                };
                this.send("HQRLightShow_showColor", params, (result) => {
                    if(this._request)
                        this._request.resolve();
                });
            }
        }
    }

    HQRLightShow_showBreath (rgb_port, index, speed, red, green, blue) {
        const block = this.findOrConstructBlock('XRGBLed', '', rgb_port);
        if (block) {
            const server = this.findOrConstructServer('HQRLightShow', rgb_port);
            if (server) {
                const params = {
                    type: server.type,
                    ports: server.ports,
                    rgb_model: block.model,
                    rgb_port,
                    index,
                    speed,
                    red,
                    green,
                    blue
                };
                this.send("HQRLightShow_showBreath", params, (result) => {
                    if(this._request)
                        this._request.resolve();
                });
            }
        }
    }

    HQRLightShow_showMeteor (rgb_port, speed, red, green, blue) {
        const block = this.findOrConstructBlock('XRGBLed', '', rgb_port);
        if (block) {
            const server = this.findOrConstructServer('HQRLightShow', rgb_port);
            if (server) {
                const params = {
                    type: server.type,
                    ports: server.ports,
                    rgb_model: block.model,
                    rgb_port,
                    speed,
                    red,
                    green,
                    blue
                };
                this.send("HQRLightShow_showMeteor", params, (result) => {
                    if(this._request)
                        this._request.resolve();
                });
            }
        }
    }

    HQRLightShow_clear (rgb_port, index) {
        const block = this.findOrConstructBlock('XRGBLed', '', rgb_port);
        if (block) {
            const server = this.findOrConstructServer('HQRLightShow', rgb_port);
            if (server) {
                const params = {
                    type: server.type,
                    ports: server.ports,
                    rgb_model: block.model,
                    rgb_port,
                    index,
                };
                this.send("HQRLightShow_clear", params, (result) => {
                    if(this._request)
                        this._request.resolve();
                });
            }
        }
    }

    HQRAudioPlayer_setNoteParameter (port,beatTime) {
        const server = this.findOrConstructServer('HQRAudioPlayer', port);
        if (server) {
            const params = {
                type: server.type,
                ports: server.ports,
                beatTime
            };
            this.send("HQRAudioPlayer_setNoteParameter", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }

    HQRAudioPlayer_playNote (port, note, scale, tone, beat) {
        const server = this.findOrConstructServer('HQRAudioPlayer', port);
        if (server) {
            const params = {
                type: server.type,
                ports: server.ports,
                note,
                scale,
                tone,
                beat
            };
            this.send("HQRAudioPlayer_playNote", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }

    HQRAudioPlayer_playMusic (port, music) {
        const server = this.findOrConstructServer('HQRAudioPlayer', port);
        if (server) {
            const params = {
                type: server.type,
                ports: server.ports,
                music
            };
            this.send("HQRAudioPlayer_playMusic", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }

    HQRAudioPlayer_playSound (port, sound) {
        const server = this.findOrConstructServer('HQRAudioPlayer', port);
        if (server) {
            const params = {
                type: server.type,
                ports: server.ports,
                sound
            };
            this.send("HQRAudioPlayer_playSound", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }

    HQRAudioPlayer_stop (port) {
        const server = this.findOrConstructServer('HQRAudioPlayer', port);
        if (server) {
            const params = {
                type: server.type,
                ports: server.ports
            };
            this.send("HQRAudioPlayer_stop", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }
    
    XVoiceBroadcast_reportObject (port, index, value) {
        const block = this.findOrConstructBlock('XVoiceBroadcast', XVoiceBroadcastModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port,
                index,
                value
            };
            this.send("XVoiceBroadcast_reportObject", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }

    XVoiceBroadcast_reportTime (port, hour, minute, second) {
        const block = this.findOrConstructBlock('XVoiceBroadcast', XVoiceBroadcastModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port,
                hour,
                minute,
                second
            };
            this.send("XVoiceBroadcast_reportTime", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }

    XVoiceBroadcast_reportDate (port, year, month, day) {
        const block = this.findOrConstructBlock('XVoiceBroadcast', XVoiceBroadcastModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port,
                year,
                month,
                day
            };
            this.send("XVoiceBroadcast_reportDate", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }

    XVoiceBroadcast_reportOperator (port, operator) {
        const block = this.findOrConstructBlock('XVoiceBroadcast', XVoiceBroadcastModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port,
                operator
            };
            this.send("XVoiceBroadcast_reportOperator", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }

    XVoiceBroadcast_reportSond (port, which, repeat) {
        const block = this.findOrConstructBlock('XVoiceBroadcast', XVoiceBroadcastModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port,
                which,
                repeat
            };
            this.send("XVoiceBroadcast_reportSound", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }

    XVoiceBroadcast_stop (port) {
        const block = this.findOrConstructBlock('XVoiceBroadcast', XVoiceBroadcastModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port
            };
            this.send("XVoiceBroadcast_stop", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }

    XLEDMatrix_setEffect (port, effect, speed) {
        const block = this.findOrConstructBlock('XLEDMatrix', XLEDMatrixModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port,
                effect,
                speed
            };
            this.send("XLEDMatrix_showNumber", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }

    XLEDMatrix_showNumber (port, num) {
        const block = this.findOrConstructBlock('XLEDMatrix', XLEDMatrixModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port,
                num
            };
            this.send("XLEDMatrix_showNumber", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }

    XLEDMatrix_showNumberPair (port, A, B) {
        const block = this.findOrConstructBlock('XLEDMatrix', XLEDMatrixModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port,
                A,
                B
            };
            this.send("XLEDMatrix_showNumberPair", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }

    XLEDMatrix_showEmoticon (port, emot) {
        const block = this.findOrConstructBlock('XLEDMatrix', XLEDMatrixModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port,
                emot
            };
            this.send("XLEDMatrix_showEmoticon", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }

    XLEDMatrix_showFlag (port, flag) {
        const block = this.findOrConstructBlock('XLEDMatrix', XLEDMatrixModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port,
                flag
            };
            this.send("XLEDMatrix_showFlag", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }

    XLEDMatrix_clear (port) {
        const block = this.findOrConstructBlock('XLEDMatrix', XLEDMatrixModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port
            };
            this.send("XLEDMatrix_clear", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }

    XIRReceiver_enableLongPress (port, key) {
        const block = this.findOrConstructBlock('XIRReceiver', '', port);
        if (block) {
            const params = {
                type: block.type,
                model: block.model,
                port: block.port,
                key
            };
            this.send("XIRReceiver_enableLongPress", params, (result) => {
                if(this._request)
                    this._request.resolve();
            });
        }
    }

    XIRReceiver_checkMessage (port, key) {
        const block = this.findOrConstructBlock('XIRReceiver', '', port);
        if (block) {
            return block.key == key;
        } 
        return false;
    }

    /**
     * Called by the runtime when user wants to scan for a peripheral.
     */
    scan () {
        if (this._xbridge) {
            this._xbridge.disconnect();
        }
        this._xbridge = new XBridge(this._runtime, this._extensionId, {
            filters: [
                //{services: [XBridgeUUID.service]}
            ]
        }, this._onConnect, this._onReceive);
    }

    /**
     * Called by the runtime when user wants to connect to a certain peripheral.
     * @param {number} id - the id of the peripheral to connect to.
     */
    connect (id) {
        if (this._xbridge) {
            this._xbridge.connectPeripheral(id);
        }
    }

    /**
     * Disconnect from the xkithqrover.
     */
    disconnect () {
        this.clearAll();
        if (this._xbridge) {
            this._xbridge.disconnect();
        }
    }

    /**
     * Return true if connected to the xkithqrover.
     * @return {boolean} - whether the xkithqrover is connected.
     */
    isConnected () {
        let connected = false;
        if (this._xbridge) {
            connected = this._xbridge.isConnected();
        }
        return connected;
    }

    // send (method, params) {
    //     return this._xbridge.rpc(method, params);
    // }

    isBusy() {
        return this._busy;
    }

    send (method, params, callback) {
        // Set a busy flag so that while we are sending a message and waiting for
        // the response, additional messages are ignored.
        this._busy = true;

        // Set a timeout after which to reset the busy flag. This is used in case
        // a XBridge message was sent for which we never received a response, because
        // e.g. the peripheral was turned off after the message was sent. We reset
        // the busy flag after a while so that it is possible to try again later.
        this._busyTimeoutID = window.setTimeout(() => {
            this._busy = false;
            if(callback) {
                callback(null);
            }
        }, 5000);

        this._xbridge.rpc(method, params).then(
            (result) => {
                this._busy = false;
                window.clearTimeout(this._busyTimeoutID);
                if(callback) {
                    callback(result);
                }
            }
        );
    }

    /**
     * Starts reading data from peripheral after XBridge has connected to it.
     * @private
     */
    _onConnect () {
        this.clearAll();
        //this._xbridge.read(XBridgeUUID.service, XBridgeUUID.rxChar, true, this._onMessage);
        //this._timeoutID = window.setInterval(this.disconnect, XBridgeTimeout);
    }

    /**
     * Handle a received call from the socket.
     * @param {string} method - a received method label.
     * @param {object} params - a received list of parameters.
     * @return {object} - optional return value.
     */
    _onReceive (method, params) {
        switch (method) {
        case 'XButton_notifyEvent': {
                block = this.findBlock(params.port);
                if (block) {
                    block.status = params.status;
                }
            }
            break;
        case 'XIRReceiver_notifyEvent': {
                block = this.findBlock(params.port);
                if (block) {
                    block.key = params.key;
                }
            }
            break;
        case 'XIRTracking_notifyEvent': {
                block = this.findBlock(params.port);
                if (block) {
                    block.status = params.status;
                }
            }
            break;
        case 'XLightSensor_notifyEvent': {
                block = this.findBlock(params.port);
                if (block) {
                    block.luminance = params.luminance;
                }
            }
            break;
        case 'XSoundSensor_notifyEvent': {
                block = this.findBlock(params.port);
                if (block) {
                    block.volume = params.volume;
                }
            }
            break;
        case 'XUltrasonic_notifyEvent': {
                block = this.findBlock(params.port);
                if (block) {
                    block.distance = params.distance;
                }
            }
            break;
        }
    }
}

/**
 * Enum for Compare.
 */

const EqualOper = {
    EQUAL: 1,
    NOT_EQUAL: 2
};

const CompareOper = {
    GREATER: 1,
    SMALLER: 2
};

/**
 * Enum for Speed.
 */

const XKitHqroverSpeeds = {
    SLOW: 0,
    NORMAL: 1,
    FAST: 2
};

/**
 * Enum for XKitHqrover.
 */
const XKitHqroverPorts = {
    PORT1: '1',
    PORT2: '2',
    PORT3: '3',
    PORT4: '4'
};

/**
 * Enum for XSoundSensor.
 */

const XSoundSensorPorts = {
    PORT1: 'SND'
};

/**
 * Enum for XLightSensor.
 */

const XLightSensorPorts = {
    PORT1: 'LIG'
};

/**
 * Enum for XButton.
 */

const XButtonPorts = {
    PORT1: 'BTN'
};

/**
 * Enum for XDualDCMotor.
 */

const XDualDCMotorPorts = {
    PORT1: 'DDM'
};

/**
 * Enum for XIRTracking.
 */

const XIRTrackingModels = {
    MODEL1: 'IRT3320'
};

/**
 * Enum for XUltrasonic.
 */

const XUltrasonicModels = {
    MODEL1: 'ULS3600'
};

/**
 * Enum for XIRAvoiding.
 */

const XIRAvoidingModels = {
    MODEL1: 'IRA3200'
};

/**
 * Enum for HQRCarDriver.
 */

const HQRCarDriverDirs = {
    FORWARD: 0,
    BACKWARD: 1
};

const HQRCarDriverActions = {
    LEFTFORWARD: 0,
    RIGHTFORWARD: 1,
    LEFTBACKWARD: 2,
    RIGHTBACKWARD: 3
};

/**
 * Enum for XRGBLed.
 */

const XRGBLedPorts = {
    PORT1: 'RGB'
};

/**
 * Enum for XBuzzer.
 */

const XBuzzerPorts = {
    PORT1: 'BUZ'
};

const XBuzzerMusics = {
    HAPPYBIRTHDAY: 1,
    LITTLESTAR: 2,
    SONGOFJOY: 3,
    LITTLEAPPLE: 4,
    GOTOSCHOOL: 5
};

const XBuzzerSounds = {
    AMBULANCE: 1,
    FIRE_ENGINE: 2,
    POLICE_WAGON: 3,
    CAR_WHISTLING: 4
};

/**
 * Enum for XSegDisplay.
 */

const XSegDisplayModels = {
    MODEL1: 'SGD4300'
};

/**
 * Enum for XVoiceBroadcast.
 */

const XVoiceBroadcastModels = {
    MODEL1: 'VBC3300'
};

const XVoiceBroadcastObjects = {
    VALUE: 0,
    LIGHT: 1,
    SOUND: 2,
    TEMPERATURE: 3,
    HUMIDITY: 4,
    DISTANCE: 5,
    SPEED: 6,
    STATUS: 7
};

const XVoiceBroadcastOperators = {
    ADD: 61,
    MINUS: 62,
    MULTI: 63,
    DIVIDE: 64,
    EQUAL: 65
};

/**
 * Enum for XLEDMatrix.
 */

const XLEDMatrixModels = {
    MODEL1: 'LMT3300'
};

const XLEDMatrixEffects = {
    STATIC: 0,
    ROLL_UP: 1,
    ROLL_DWON: 2,
    ROLL_LEFT: 3,
    ROLL_RIGHT: 4,
    FLICKER: 5
};

const XLEDMatrixEmots = {
    COOL: 0,
    SMILE: 1,
    LAUGH: 2,
    GRIEVANCE: 3,
    ANGRY: 4,
    ANGER: 5,
    CRY: 6,
    NAUGHTY: 7,
    LOVE: 8
};

const XLEDMatrixFlags = {
    X: 0,
    RECTANGLE: 1,
    TRIGANLE: 2,
    CIRCLE: 3,
    UP: 4,
    DOWN: 5,
    LEFT: 6,
    RIGHT: 7,
    STOP: 8,
    FORWARD_AND_TURN_LEFT: 9,
    FORWARD_AND_TURN_RIGHT: 10,
    BACKWARD_AND_TURN_LEFT: 11,
    BACKWARD_AND_TURN_RIGHT: 12,
    CUP: 13,
    BANNER: 14
};

/**
 * Enum for XIRReceiver.
 */

const XIRReceiverPorts = {
    PORT1: 'IRR'
};

/**
 * Scratch 3.0 blocks to interact with a XKitHqrover peripheral.
 */
class Scratch3XKitHqroverBlocks {

    /**
     * @return {string} - the name of this extension.
     */
    static get EXTENSION_NAME () {
        return formatMessage({
            id: 'xkithqrover.hqrover',
            default: 'hqrover',
            description: 'xkit hqrover'
        })
    }

    /**
     * @return {string} - the ID of this extension.
     */
    static get EXTENSION_ID () {
        return 'xkithqrover';
    }

    /**
     * @return {array} - text and values for each compare menu element
     */
    get COMPARE_OPER_MENU () {
        return [
            {
                text: '>',
                value: CompareOper.GREATER
            },
            {
                text: '<',
                value: CompareOper.OPER2
            }
        ];
    }

    /**
     * @return {array} - text and values for each speeds menu element
     */
    get SPEEDS_MENU () {
        return [
            {
                text: formatMessage({
                    id: 'xkithqrover.Speeds.SLOW',
                    default: 'slow',
                    description: ''
                }),
                value: XKitHqroverSpeeds.SLOW
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.Speeds.NORMAL',
                    default: 'middle',
                    description: ''
                }),
                value: XKitHqroverSpeeds.NORMAL
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.Speeds.FAST',
                    default: 'fast',
                    description: ''
                }),
                value: XKitHqroverSpeeds.FAST
            }
        ];
    }

    /**
     * @return {array} - text and values for each repeat menu element
     */
    get REPEATS_MENU () {
        return [
            {
                text: formatMessage({
                    id: 'xkithqrover.Repeat.FALSE',
                    default: 'not repeat',
                    description: ''
                }),
                value: 0
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.Repeat.TRUE',
                    default: 'repeat',
                    description: ''
                }),
                value: 1
            }
        ];
    }

    /**
     * @return {array} - text and values for each port menu element
     */
    get PORTS_MENU () {
        return [
            {
                text: XKitHqroverPorts.PORT1,
                value: XKitHqroverPorts.PORT1
            },
            {
                text: XKitHqroverPorts.PORT2,
                value: XKitHqroverPorts.PORT2
            },
            {
                text: XKitHqroverPorts.PORT3,
                value: XKitHqroverPorts.PORT3
            },
            {
                text: XKitHqroverPorts.PORT4,
                value: XKitHqroverPorts.PORT4
            }
        ];
    }

    /**
     * @return {array} - text and values for each soundsensor port menu element
     */
    get XSOUNDSENSOR_PORTS_MENU () {
        return [
            {
                text: XSoundSensorPorts.PORT1,
                value: XSoundSensorPorts.PORT1
            },
            {
                text: XKitHqroverPorts.PORT1,
                value: XKitHqroverPorts.PORT1
            },
            {
                text: XKitHqroverPorts.PORT2,
                value: XKitHqroverPorts.PORT2
            },
            {
                text: XKitHqroverPorts.PORT3,
                value: XKitHqroverPorts.PORT3
            },
            {
                text: XKitHqroverPorts.PORT4,
                value: XKitHqroverPorts.PORT4
            }
        ];
    }

    /**
     * @return {array} - text and values for each lightsensor port menu element
     */
    get XLIGHTSENSOR_PORTS_MENU () {
        return [
            {
                text: XLightSensorPorts.PORT1,
                value: XLightSensorPorts.PORT1
            },
            {
                text: XKitHqroverPorts.PORT1,
                value: XKitHqroverPorts.PORT1
            },
            {
                text: XKitHqroverPorts.PORT2,
                value: XKitHqroverPorts.PORT2
            },
            {
                text: XKitHqroverPorts.PORT3,
                value: XKitHqroverPorts.PORT3
            },
            {
                text: XKitHqroverPorts.PORT4,
                value: XKitHqroverPorts.PORT4
            }
        ];
    }

    /**
     * @return {array} - text and values for each button port menu element
     */
    get XBUTTON_PORTS_MENU () {
        return [
            {
                text: XButtonPorts.PORT1,
                value: XButtonPorts.PORT1
            },
            {
                text: XKitHqroverPorts.PORT1,
                value: XKitHqroverPorts.PORT1
            },
            {
                text: XKitHqroverPorts.PORT2,
                value: XKitHqroverPorts.PORT2
            },
            {
                text: XKitHqroverPorts.PORT3,
                value: XKitHqroverPorts.PORT3
            },
            {
                text: XKitHqroverPorts.PORT4,
                value: XKitHqroverPorts.PORT4
            }
        ];
    }

    /**
     * @return {array} - text and values for each irtracking's status menu element
     */
    get XIRTRACKING_STATUS_MENU () {
        return [
            {
                text: '■■',
                value: 3
            },
            {
                text: '■□',
                value: 1
            },
            {
                text: '□■',
                value: 2
            },
            {
                text: '□□',
                value: 0
            }
        ];
    }
    
    /**
     * @return {array} - text and values for each hqrcardriver dir menu element
     */
    get HQRCARDRIVER_DIRS_MENU () {
        return [
            {
                text: formatMessage({
                    id: 'xkithqrover.HQRCarDriver_Dirs.FORWARD',
                    default: 'forward',
                    description: ''
                }),
                value: HQRCarDriverDirs.FORWARD
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.HQRCarDriver_Dirs.BACKWARD',
                    default: 'backward',
                    description: ''
                }),
                value: HQRCarDriverDirs.BACKWARD
            }
        ];
    }

    /**
     * @return {array} - text and values for each hqrcardriver dir menu element
     */
    get HQRCARDRIVER_ACTIONS_MENU () {
        return [
            {
                text: formatMessage({
                    id: 'xkithqrover.HQRCarDriver_Actions.LEFTFORWARD',
                    default: 'forward and turn left',
                    description: ''
                }),
                value: HQRCarDriverActions.LEFTFORWARD
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.HQRCarDriver_Actions.RIGHTFORWARD',
                    default: 'forward and turn right',
                    description: ''
                }),
                value: HQRCarDriverActions.RIGHTFORWARD
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.HQRCarDriver_Actions.LEFTBACKWARD',
                    default: 'backward and turn left',
                    description: ''
                }),
                value: HQRCarDriverActions.LEFTBACKWARD
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.HQRCarDriver_Actions.RIGHTBACKWARD',
                    default: 'backward and turn right',
                    description: ''
                }),
                value: HQRCarDriverActions.RIGHTBACKWARD
            }
        ];
    }

    /**
     * @return {array} - text and values for each xsegdisplay index menu element
     */
    get XSEGDISPLAY_INDEXS_MENU () {
        return [
            {
                text: '1',
                value: 1
            },
            {
                text: '2',
                value: 2
            },
            {
                text: '3',
                value: 3
            },
            {
                text: '4',
                value: 4
            }
        ];
    }

    /**
     * @return {array} - text and values for each xsegdisplay segment menu element
     */
    get XSEGDISPLAY_SEGMENTS_MENU () {
        return [
            {
                text: 'a',
                value: 'a'
            },
            {
                text: 'b',
                value: 'b'
            },
            {
                text: 'c',
                value: 'c'
            },
            {
                text: 'd',
                value: 'd'
            },
            {
                text: 'e',
                value: 'e'
            },
            {
                text: 'f',
                value: 'f'
            },
            {
                text: 'g',
                value: 'g'
            },
            {
                text: '.',
                value: '.'
            }
        ];
    }

    /**
     * @return {array} - text and values for each rgbled port menu element
     */
    get XRGBLED_PORTS_MENU () {
        return [
            {
                text: XRGBLedPorts.PORT1,
                value: XRGBLedPorts.PORT1
            },
            {
                text: XKitHqroverPorts.PORT1,
                value: XKitHqroverPorts.PORT1
            },
            {
                text: XKitHqroverPorts.PORT2,
                value: XKitHqroverPorts.PORT2
            },
            {
                text: XKitHqroverPorts.PORT3,
                value: XKitHqroverPorts.PORT3
            },
            {
                text: XKitHqroverPorts.PORT4,
                value: XKitHqroverPorts.PORT4
            }
        ];
    }

    /**
     * @return {array} - text and values for each rgbled index menu element
     */
    get XRGBLED_INDEXS_MENU () {
        return [
            {
                text: formatMessage({
                    id: 'xkithqrover.XRGBLed_Indexs.ALL',
                    default: 'all',
                    description: ''
                }),
                value: 0
            },
            {
                text: '1',
                value: 1
            },
            {
                text: '2',
                value: 2
            },
            {
                text: '3',
                value: 3
            },
            {
                text: '4',
                value: 4
            },
            {
                text: '5',
                value: 5
            },
            {
                text: '6',
                value: 6
            }
        ];
    }

    /**
     * @return {array} - text and values for each xbuzzer port menu element
     */
    get XBUZZER_PORTS_MENU () {
        return [
            {
                text: XBuzzerPorts.PORT1,
                value: XBuzzerPorts.PORT1
            },
            {
                text: XKitHqroverPorts.PORT1,
                value: XKitHqroverPorts.PORT1
            },
            {
                text: XKitHqroverPorts.PORT2,
                value: XKitHqroverPorts.PORT2
            },
            {
                text: XKitHqroverPorts.PORT3,
                value: XKitHqroverPorts.PORT3
            },
            {
                text: XKitHqroverPorts.PORT4,
                value: XKitHqroverPorts.PORT4
            }
        ];
    }

    /**
     * @return {array} - text and values for each xbuzzer note menu element
     */
    get XBUZZER_NOTES_MENU () {
        return [
            {
                text: 'do',
                value: 1
            },
            {
                text: 're',
                value: 2
            },
            {
                text: 'me',
                value: 3
            },
            {
                text: 'fa',
                value: 4
            },
            {
                text: 'sol',
                value: 5
            },
            {
                text: 'la',
                value: 6
            },
            {
                text: 'si',
                value: 7
            },
            {
                text: '-',
                value: 0
            }
        ];
    }

    /**
     * @return {array} - text and values for each xbuzzer scale menu element
     */
    get XBUZZER_SCALES_MENU () {
        return [
            {
                text: formatMessage({
                    id: 'xkithqrover.XBuzzer_Scales.LOW',
                    default: 'low',
                    description: ''
                }),
                value: 0
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XBuzzer_Scales.MEDIUM',
                    default: 'medium',
                    description: ''
                }),
                value: 1
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XBuzzer_Scales.HIGH',
                    default: 'high',
                    description: ''
                }),
                value: 2
            }
        ];
    }

    /**
     * @return {array} - text and values for each xbuzzer tone menu element
     */
    get XBUZZER_TONES_MENU () {
        return [
            {
                text: 'C',
                value: 0
            },
            {
                text: 'D',
                value: 1
            },
            {
                text: 'E',
                value: 2
            },
            {
                text: 'F',
                value: 3
            },
            {
                text: 'G',
                value: 4
            },
            {
                text: 'A',
                value: 5
            },
            {
                text: 'B',
                value: 6
            }
        ];
    }

    /**
     * @return {array} - text and values for each xbuzzer tone menu element
     */
    get XBUZZER_BEATS_MENU () {
        return [
            {
                text: '1/8',
                value: 1
            },
            {
                text: '1/4',
                value: 2
            },
            {
                text: '1/2',
                value: 4
            },
            {
                text: '1',
                value: 8
            },
            {
                text: '2',
                value: 16
            }
        ];
    }

    /**
     * @return {array} - text and values for each xbuzzer music menu element
     */
    get XBUZZER_MUSICS_MENU () {
        return [
            {
                text: formatMessage({
                    id: 'xkithqrover.XBuzzer_Musics.HAPPYBIRTHDAY',
                    default: 'happy brithday',
                    description: ''
                }),
                value: XBuzzerMusics.HAPPYBIRTHDAY
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XBuzzer_Musics.LITTLESTAR',
                    default: 'little star',
                    description: ''
                }),
                value: XBuzzerMusics.LITTLESTAR
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XBuzzer_Musics.SONGOFJOY',
                    default: 'song of joy',
                    description: ''
                }),
                value: XBuzzerMusics.SONGOFJOY
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XBuzzer_Musics.LITTLEAPPLE',
                    default: 'little apple',
                    description: ''
                }),
                value: XBuzzerMusics.LITTLEAPPLE
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XBuzzer_Musics.GOTOSCHOOL',
                    default: 'go to school',
                    description: ''
                }),
                value: XBuzzerMusics.GOTOSCHOOL
            }
        ];
    }

    /**
     * @return {array} - text and values for each xbuzzer sound menu element
     */
    get XBUZZER_SOUNDS_MENU () {
        return [
            {
                text: formatMessage({
                    id: 'xkithqrover.XBuzzer_Sounds.AMBULANCE',
                    default: 'ambulance',
                    description: ''
                }),
                value: XBuzzerSounds.AMBULANCE
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XBuzzer_Sounds.FIRE_ENGINE',
                    default: 'fire engine',
                    description: ''
                }),
                value: XBuzzerSounds.FIRE_ENGINE
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XBuzzer_Sounds.POLICE_WAGON',
                    default: 'police wagon',
                    description: ''
                }),
                value: XBuzzerSounds.POLICE_WAGON
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XBuzzer_Sounds.CAR_WHISTLING',
                    default: 'car whistling',
                    description: ''
                }),
                value: XBuzzerSounds.CAR_WHISTLING
            }
        ];
    }

    /**
     * @return {array} - text and values for each xvoicebroadcast object menu element
     */
    get XVOICEBROADCAST_OBJECTS_MENU () {
        return [
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Objects.VALUE',
                    default: 'value',
                    description: ''
                }),
                value: XVoiceBroadcastObjects.VALUE
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Objects.LIGHT',
                    default: 'light',
                    description: ''
                }),
                value: XVoiceBroadcastObjects.LIGHT
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Objects.SOUND',
                    default: 'sound',
                    description: ''
                }),
                value: XVoiceBroadcastObjects.SOUND
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Objects.TEMPERATURE',
                    default: 'temperature',
                    description: ''
                }),
                value: XVoiceBroadcastObjects.TEMPERATURE
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Objects.HUMIDITY',
                    default: 'humidity',
                    description: ''
                }),
                value: XVoiceBroadcastObjects.HUMIDITY
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Objects.DISTANCE',
                    default: 'distance',
                    description: ''
                }),
                value: XVoiceBroadcastObjects.DISTANCE
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Objects.SPEED',
                    default: 'speed',
                    description: ''
                }),
                value: XVoiceBroadcastObjects.SPEED
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Objects.STATUS',
                    default: 'status',
                    description: ''
                }),
                value: XVoiceBroadcastObjects.STATUS
            }
        ];
    }

    /**
     * @return {array} - text and values for each xvoicebroadcast operator menu element
     */
    get XVOICEBROADCAST_OPERATORS_MENU () {
        return [
            {
                text: '+',
                value: XVoiceBroadcastOperators.ADD
            },
            {
                text: '-',
                value: XVoiceBroadcastOperators.MINUS
            },
            {
                text: '*',
                value: XVoiceBroadcastOperators.MULTI
            },
            {
                text: '/',
                value: XVoiceBroadcastOperators.DIVIDE
            }
        ];
    }

    /**
     * @return {array} - text and values for each xvoicebroadcast action menu element
     */
    get XVOICEBROADCAST_SOUND_ACTIONS_MENU () {
        return [
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.70',
                    default: 'tick tock',
                    description: ''
                }),
                value: 70
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.71',
                    default: 'whole bell',
                    description: ''
                }),
                value: 71
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.72',
                    default: 'jo',
                    description: ''
                }),
                value: 72
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.73',
                    default: 'jojo',
                    description: ''
                }),
                value: 73
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.74',
                    default: 'jojojo',
                    description: ''
                }),
                value: 74
            }
        ];
    }

    /**
     * @return {array} - text and values for each xvoicebroadcast alarm menu element
     */
    get XVOICEBROADCAST_SOUND_ALARMS_MENU () {
        return [
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.78',
                    default: 'police',
                    description: ''
                }),
                value: 78
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.79',
                    default: 'fire engine',
                    description: ''
                }),
                value: 79
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.80',
                    default: 'ambulance',
                    description: ''
                }),
                value: 80
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.81',
                    default: 'bell',
                    description: ''
                }),
                value: 81
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.82',
                    default: 'guard',
                    description: ''
                }),
                value: 82
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.83',
                    default: 'red alert',
                    description: ''
                }),
                value: 83
            }
        ];
    }

    /**
     * @return {array} - text and values for each xvoicebroadcast science menu element
     */
    get XVOICEBROADCAST_SOUND_SCIENCES_MENU () {
        return [
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.86',
                    default: 'machine run',
                    description: ''
                }),
                value: 86
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.87',
                    default: 'robot move',
                    description: ''
                }),
                value: 87
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.88',
                    default: 'servo motor',
                    description: ''
                }),
                value: 88
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.89',
                    default: 'light saber',
                    description: ''
                }),
                value: 89
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.90',
                    default: 'flight',
                    description: ''
                }),
                value: 90
            }
        ];
    }

    /**
     * @return {array} - text and values for each xvoicebroadcast warn menu element
     */
    get XVOICEBROADCAST_SOUND_WARNS_MENU () {
        return [
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.94',
                    default: 'punched-card',
                    description: ''
                }),
                value: 94
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.95',
                    default: 'ting ting',
                    description: ''
                }),
                value: 95
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.96',
                    default: 'water-drop',
                    description: ''
                }),
                value: 96
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.97',
                    default: 'dang',
                    description: ''
                }),
                value: 97
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.98',
                    default: 'telephone ringer',
                    description: ''
                }),
                value: 98
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.99',
                    default: 'take a picture',
                    description: ''
                }),
                value: 99
            }
        ];
    }

    /**
     * @return {array} - text and values for each xvoicebroadcast outstar menu element
     */
    get XVOICEBROADCAST_SOUND_OUTSTARS_MENU () {
        return [
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.102',
                    default: 'electric wave',
                    description: ''
                }),
                value: 102
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.103',
                    default: 'speak',
                    description: ''
                }),
                value: 103
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.104',
                    default: 'signal',
                    description: ''
                }),
                value: 104
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.105',
                    default: 'spaceship landed',
                    description: ''
                }),
                value: 105
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.106',
                    default: 'call',
                    description: ''
                }),
                value: 106
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.107',
                    default: 'hiahia',
                    description: ''
                }),
                value: 107
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.108',
                    default: 'warn',
                    description: ''
                }),
                value: 108
            }
        ];
    }

    /**
     * @return {array} - text and values for each xvoicebroadcast mood menu element
     */
    get XVOICEBROADCAST_SOUND_MOODS_MENU () {
        return [
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.110',
                    default: 'cheer',
                    description: ''
                }),
                value: 110
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.111',
                    default: 'surprise',
                    description: ''
                }),
                value: 111
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.112',
                    default: 'happy',
                    description: ''
                }),
                value: 112
            }
        ];
    }

    /**
     * @return {array} - text and values for each xvoicebroadcast music menu element
     */
    get XVOICEBROADCAST_SOUND_MUSICS_MENU () {
        return [
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.118',
                    default: 'summer',
                    description: ''
                }),
                value: 118
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.119',
                    default: 'always with me',
                    description: ''
                }),
                value: 119
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.120',
                    default: 'jingle bells',
                    description: ''
                }),
                value: 120
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.121',
                    default: 'waiting for the wind',
                    description: ''
                }),
                value: 121
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XVoiceBroadcast_Whichs.122',
                    default: 'doraemon',
                    description: ''
                }),
                value: 122
            }
        ];
    }

    /**
     * @return {array} - text and values for each xledmatrix effect menu element
     */
    get XLEDMATRIX_EFFECTS_MENU () {
        return [
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Effects.STATIC',
                    default: 'static',
                    description: ''
                }),
                value: XLEDMatrixEffects.STATIC
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Effects.ROLL_UP',
                    default: 'roll up',
                    description: ''
                }),
                value: XLEDMatrixEffects.ROLL_UP
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Effects.ROLL_DWON',
                    default: 'roll down',
                    description: ''
                }),
                value: XLEDMatrixEffects.ROLL_DWON
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Effects.ROLL_LEFT',
                    default: 'roll left',
                    description: ''
                }),
                value: XLEDMatrixEffects.ROLL_LEFT
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Effects.ROLL_RIGHT',
                    default: 'roll right',
                    description: ''
                }),
                value: XLEDMatrixEffects.ROLL_RIGHT
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Effects.FLICKER',
                    default: 'flicker',
                    description: ''
                }),
                value: XLEDMatrixEffects.FLICKER
            }
        ];
    }

    /**
     * @return {array} - text and values for each xledmatrix emot menu element
     */
    get XLEDMATRIX_EMOTS_MENU () {
        return [
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Emots.COOL',
                    default: 'cool',
                    description: ''
                }),
                value: XLEDMatrixEmots.COOL
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Emots.SMILE',
                    default: 'smile',
                    description: ''
                }),
                value: XLEDMatrixEmots.SMILE
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Emots.LAUGH',
                    default: 'laugh',
                    description: ''
                }),
                value: XLEDMatrixEmots.LAUGH
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Emots.GRIEVANCE',
                    default: 'grievance',
                    description: ''
                }),
                value: XLEDMatrixEmots.GRIEVANCE
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Emots.ANGRY',
                    default: 'angry',
                    description: ''
                }),
                value: XLEDMatrixEmots.ANGRY
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Emots.ANGER',
                    default: 'anger',
                    description: ''
                }),
                value: XLEDMatrixEmots.ANGER
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Emots.CRY',
                    default: 'cry',
                    description: ''
                }),
                value: XLEDMatrixEmots.CRY
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Emots.NAUGHTY',
                    default: 'naughty',
                    description: ''
                }),
                value: XLEDMatrixEmots.NAUGHTY
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Emots.LOVE',
                    default: 'love',
                    description: ''
                }),
                value: XLEDMatrixEmots.LOVE
            }
        ];
    }

    /**
     * @return {array} - text and values for each xledmatrix flag menu element
     */
    get XLEDMATRIX_FLAGS_MENU () {
        return [
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Flags.X',
                    default: 'X',
                    description: ''
                }),
                value: XLEDMatrixFlags.X
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Flags.RECTANGLE',
                    default: 'rectangle',
                    description: ''
                }),
                value: XLEDMatrixFlags.RECTANGLE
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Flags.TRIGANLE',
                    default: 'triganle',
                    description: ''
                }),
                value: XLEDMatrixFlags.TRIGANLE
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Flags.CIRCLE',
                    default: 'circle',
                    description: ''
                }),
                value: XLEDMatrixFlags.CIRCLE
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Flags.UP',
                    default: 'up',
                    description: ''
                }),
                value: XLEDMatrixFlags.UP
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Flags.DOWN',
                    default: 'down',
                    description: ''
                }),
                value: XLEDMatrixFlags.DOWN
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Flags.LEFT',
                    default: 'left',
                    description: ''
                }),
                value: XLEDMatrixFlags.LEFT
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Flags.RIGHT',
                    default: 'right',
                    description: ''
                }),
                value: XLEDMatrixFlags.RIGHT
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Flags.STOP',
                    default: 'stop',
                    description: ''
                }),
                value: XLEDMatrixFlags.STOP
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Flags.FORWARD_AND_TURN_LEFT',
                    default: 'forward and turn left',
                    description: ''
                }),
                value: XLEDMatrixFlags.FORWARD_AND_TURN_LEFT
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Flags.FORWARD_AND_TURN_RIGHT',
                    default: 'forward and turn right',
                    description: ''
                }),
                value: XLEDMatrixFlags.FORWARD_AND_TURN_RIGHT
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Flags.BACKWARD_AND_TURN_LEFT',
                    default: 'backward and turn left',
                    description: ''
                }),
                value: XLEDMatrixFlags.BACKWARD_AND_TURN_LEFT
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Flags.BACKWARD_AND_TURN_RIGHT',
                    default: 'backward and turn right',
                    description: ''
                }),
                value: XLEDMatrixFlags.BACKWARD_AND_TURN_RIGHT
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Flags.CUP',
                    default: 'cup',
                    description: ''
                }),
                value: XLEDMatrixFlags.CUP
            },
            {
                text: formatMessage({
                    id: 'xkithqrover.XLEDMatrix_Flags.BANNER',
                    default: 'banner',
                    description: ''
                }),
                value: XLEDMatrixFlags.BANNER
            }
        ];
    }

    /**
     * @return {array} - text and values for each xirreceiver port menu element
     */
    get XIRRECEIVER_PORTS_MENU () {
        return [
            {
                text: XIRReceiverPorts.PORT1,
                value: XIRReceiverPorts.PORT1
            },
            {
                text: XKitHqroverPorts.PORT1,
                value: XKitHqroverPorts.PORT1
            },
            {
                text: XKitHqroverPorts.PORT2,
                value: XKitHqroverPorts.PORT2
            },
            {
                text: XKitHqroverPorts.PORT3,
                value: XKitHqroverPorts.PORT3
            },
            {
                text: XKitHqroverPorts.PORT4,
                value: XKitHqroverPorts.PORT4
            }
        ];
    }

    /**
     * Construct a set of XKitHqrover blocks.
     * @param {Runtime} runtime - the Scratch 3.0 runtime.
     */
    constructor (runtime) {
        /**
         * The Scratch 3.0 runtime.
         * @type {Runtime}
         */
        this.runtime = runtime;

        // Create a new XKitHqrover peripheral instance
        this._peripheral = new XKitHqrover(this.runtime, Scratch3XKitHqroverBlocks.EXTENSION_ID);

        //
        this._intervalID = 0;
    }

    /**
     * @returns {object} metadata for this extension and its blocks.
     */
    getInfo () {
        return {
            id: Scratch3XKitHqroverBlocks.EXTENSION_ID,
            name: Scratch3XKitHqroverBlocks.EXTENSION_NAME,
            blockIconURI: blockIconURI,
            showStatusButton: true,
            blocks: [
                {
                    opcode: 'HQRCarDriver_move',
                    text: formatMessage({
                        id: 'xkithqrover.HQRCarDriver_move',
                        default: 'car [DIR] speed[SPEED]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        DIR: {
                            type: ArgumentType.NUMBER,
                            menu: 'hqrcardriver_dirs',
                            defaultValue: HQRCarDriverDirs.FORWARD
                        },
                        SPEED: {
                            type: ArgumentType.NUMBER,
                            defaultValue: 25
                        }
                    }
                },
                {
                    opcode: 'HQRCarDriver_turn',
                    text: formatMessage({
                        id: 'xkithqrover.HQRCarDriver_turn',
                        default: 'car [ACTION] speed[SPEED] angle[ANGLE]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        ACTION: {
                            type: ArgumentType.NUMBER,
                            menu: 'hqrcardriver_actions',
                            defaultValue: HQRCarDriverActions.LEFTFORWARD
                        },
                        SPEED: {
                            type: ArgumentType.NUMBER,
                            defaultValue: 25
                        },
                        ANGLE: {
                            type: ArgumentType.NUMBER,
                            defaultValue: 15
                        }
                    }
                },
                {
                    opcode: 'HQRCarDriver_stop',
                    text: formatMessage({
                        id: 'xkithqrover.HQRCarDriver_stop',
                        default: 'car stop',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                    }
                },
                {
                    opcode: 'HQRCarDriver_autoLineTracking',
                    text: formatMessage({
                        id: 'xkithqrover.HQRCarDriver_autoLineTracking',
                        default: 'car auto line tracking with[IRT_PORT] speed[SPEED]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        IRT_PORT: {
                            type: ArgumentType.NUMBER,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT4
                        },
                        SPEED: {
                            type: ArgumentType.NUMBER,
                            defaultValue: 25
                        }
                    }
                },
                {
                    opcode: 'HQRCarDriver_autoObstacleAvoidance',
                    text: formatMessage({
                        id: 'xkithqrover.HQRCarDriver_autoObstacleAvoidance',
                        default: 'car auto obstacle avoidance with[ULS_PORT] speed[SPEED]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        ULS_PORT: {
                            type: ArgumentType.NUMBER,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT1
                        },
                        SPEED: {
                            type: ArgumentType.NUMBER,
                            defaultValue: 25
                        }
                    }
                },
                '---',
                {
                    opcode: 'XSoundSensor_getVolume',
                    text: formatMessage({
                        id: 'xkithqrover.XSoundSensor_getVolume',
                        default: 'sound sensor[PORT] volume(0~100)',
                        description: ''
                    }),
                    blockType: BlockType.REPORTER,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'xsoundsensor_ports',
                            defaultValue: XSoundSensorPorts.PORT1
                        }
                    }
                },
                {
                    opcode: 'XLightSensor_getLuminance',
                    text: formatMessage({
                        id: 'xkithqrover.XLightSensor_getLuminance',
                        default: 'light sensor[PORT] luminance(0~100)',
                        description: ''
                    }),
                    blockType: BlockType.REPORTER,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'xlightsensor_ports',
                            defaultValue: XLightSensorPorts.PORT1
                        }
                    }
                },
                {
                    opcode: 'XButton_isPressed',
                    text: formatMessage({
                        id: 'xkithqrover.XButton_isPressed',
                        default: 'button[PORT] is pressed',
                        description: ''
                    }),
                    blockType: BlockType.BOOLEAN,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'xbutton_ports',
                            defaultValue: XButtonPorts.PORT1
                        }
                    }
                },
                {
                    opcode: 'XIRTracking_checkStatus',
                    text: formatMessage({
                        id: 'xkithqrover.XIRTracking_checkStatus',
                        default: 'IR tracking sensor[PORT] status[STATUS]',
                        description: ''
                    }),
                    blockType: BlockType.BOOLEAN,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT4
                        },
                        STATUS: {
                            type: ArgumentType.STRING,
                            menu: 'irtracking_status',
                            defaultValue: 3
                        }
                    }
                },
                {
                    opcode: 'XUltrasonic_getDistance',
                    text: formatMessage({
                        id: 'xkithqrover.XUltrasonic_getDistance',
                        default: 'ultrasonic[PORT] distance',
                        description: ''
                    }),
                    blockType: BlockType.REPORTER,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT1
                        }
                    }
                },
                {
                    opcode: 'XIRAvoiding_checkStatus',
                    text: formatMessage({
                        id: 'xkithqrover.XIRAvoiding_checkStatus',
                        default: 'IR avoiding sensor[PORT] status',
                        description: ''
                    }),
                    blockType: BlockType.BOOLEAN,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT1
                        }
                    }
                },
                '---',
                {
                    opcode: 'XSegDisplay_showNumber',
                    text: formatMessage({
                        id: 'xkithqrover.XSegDisplay_showNumber',
                        default: 'segment LED[PORT] show number[NUM]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT2
                        },
                        NUM: {
                            type: ArgumentType.NUMBER,
                            defaultValue: 123
                        }
                    }
                },
                {
                    opcode: 'XSegDisplay_showCharacter',
                    text: formatMessage({
                        id: 'xkithqrover.XSegDisplay_showCharacter',
                        default: 'segment LED[PORT] show[INDEX] character[CHAR](0~9,A~F)',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT2
                        },
                        INDEX: {
                            type: ArgumentType.NUMBER,
                            menu: 'xsegdisplay_indexs',
                            defaultValue: 1
                        },
                        CHAR: {
                            type: ArgumentType.STRING,
                            defaultValue: 'B'
                        }
                    }
                },
                {
                    opcode: 'XSegDisplay_showSegment',
                    text: formatMessage({
                        id: 'xkithqrover.XSegDisplay_showSegment',
                        default: 'segment LED[PORT] show[INDEX] segment[SEG]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT2
                        },
                        INDEX: {
                            type: ArgumentType.NUMBER,
                            menu: 'xsegdisplay_indexs',
                            defaultValue: 1
                        },
                        SEG: {
                            type: ArgumentType.STRING,
                            menu: 'xsegdisplay_segments',
                            defaultValue: 'a'
                        }
                    }
                },
                {
                    opcode: 'XSegDisplay_clearSegment',
                    text: formatMessage({
                        id: 'xkithqrover.XSegDisplay_clearSegment',
                        default: 'segment LED[PORT] clear[INDEX] segment[SEG]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT2
                        },
                        INDEX: {
                            type: ArgumentType.NUMBER,
                            menu: 'xsegdisplay_indexs',
                            defaultValue: 1
                        },
                        SEG: {
                            type: ArgumentType.STRING,
                            menu: 'xsegdisplay_segments',
                            defaultValue: 'a'
                        }
                    }
                },
                {
                    opcode: 'XSegDisplay_clear',
                    text: formatMessage({
                        id: 'xkithqrover.XSegDisplay_clear',
                        default: 'segment LED[PORT] clear',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT2
                        }
                    }
                },
                {
                    opcode: 'HQRLightShow_showColor',
                    text: formatMessage({
                        id: 'xkithqrover.HQRLightShow_showColor',
                        default: 'RGB LED[PORT] light[INDEX] color[COLOR]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'rgb_ports',
                            defaultValue: XRGBLedPorts.PORT1
                        },
                        INDEX: {
                            type: ArgumentType.NUMBER,
                            menu: 'rgb_indexs',
                            defaultValue: 0
                        },
                        COLOR: {
                            type: ArgumentType.COLOR
                        }
                    }
                },
                {
                    opcode: 'HQRLightShow_showBreath',
                    text: formatMessage({
                        id: 'xkithqrover.HQRLightShow_showBreath',
                        default: 'RGB LED[PORT] breath[INDEX] speed[SPEED] color[COLOR]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'rgb_ports',
                            defaultValue: XRGBLedPorts.PORT1
                        },
                        INDEX: {
                            type: ArgumentType.NUMBER,
                            menu: 'rgb_indexs',
                            defaultValue: 0
                        },
                        SPEED: {
                            type: ArgumentType.NUMBER,
                            menu: 'speeds',
                            defaultValue: XKitHqroverSpeeds.NORMAL
                        },
                        COLOR: {
                            type: ArgumentType.COLOR
                        }
                    }
                },
                {
                    opcode: 'HQRLightShow_showMeteor',
                    text: formatMessage({
                        id: 'xkithqrover.HQRLightShow_showMeteor',
                        default: 'RGB LED[PORT] meteor speed[SPEED] color[COLOR]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'rgb_ports',
                            defaultValue: XRGBLedPorts.PORT1
                        },
                        SPEED: {
                            type: ArgumentType.NUMBER,
                            menu: 'speeds',
                            defaultValue: XKitHqroverSpeeds.NORMAL
                        },
                        COLOR: {
                            type: ArgumentType.COLOR
                        }
                    }
                },
                {
                    opcode: 'HQRLightShow_clear',
                    text: formatMessage({
                        id: 'xkithqrover.HQRLightShow_clear',
                        default: 'RGB LED[PORT] clear[INDEX]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'rgb_ports',
                            defaultValue: XRGBLedPorts.PORT1
                        },
                        INDEX: {
                            type: ArgumentType.NUMBER,
                            menu: 'rgb_indexs',
                            defaultValue: 0
                        }
                    }
                },
                {
                    opcode: 'HQRAudioPlayer_setNoteParameter',
                    text: formatMessage({
                        id: 'xkithqrover.HQRAudioPlayer_setNoteParameter',
                        default: 'buzzer[PORT] set beat time[BEATTIME]secs(0.1~2)',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'xbuzzer_ports',
                            defaultValue: XBuzzerPorts.PORT1
                        },
                        BEATTIME: {
                            type: ArgumentType.NUMBER,
                            defaultValue: 0.8
                        }
                    }
                },
                {
                    opcode: 'HQRAudioPlayer_playNote',
                    text: formatMessage({
                        id: 'xkithqrover.HQRAudioPlayer_playNote',
                        default: 'buzzer[PORT] play note[NOTE] [SCALE]scale [TONE]tone [BEAT]beat',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'xbuzzer_ports',
                            defaultValue: XBuzzerPorts.PORT1
                        },
                        NOTE: {
                            type: ArgumentType.NUMBER,
                            menu: 'xbuzzer_notes',
                            defaultValue: 1
                        },
                        SCALE: {
                            type: ArgumentType.NUMBER,
                            menu: 'XBuzzer_Scales',
                            defaultValue: 0
                        },
                        TONE: {
                            type: ArgumentType.NUMBER,
                            menu: 'xbuzzer_tones',
                            defaultValue: 0
                        },
                        BEAT: {
                            type: ArgumentType.NUMBER,
                            menu: 'xbuzzer_beats',
                            defaultValue: 1
                        }
                    }
                },
                {
                    opcode: 'HQRAudioPlayer_playMusic',
                    text: formatMessage({
                        id: 'xkithqrover.HQRAudioPlayer_playMusic',
                        default: 'buzzer[PORT] play music[MUSIC]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'xbuzzer_ports',
                            defaultValue: XBuzzerPorts.PORT1
                        },
                        MUSIC: {
                            type: ArgumentType.NUMBER,
                            menu: 'XBuzzer_Musics',
                            defaultValue: XBuzzerMusics.HAPPYBIRTHDAY
                        }
                    }
                },
                {
                    opcode: 'HQRAudioPlayer_playSound',
                    text: formatMessage({
                        id: 'xkithqrover.HQRAudioPlayer_playSound',
                        default: 'buzzer[PORT] play sound[SOUND]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'xbuzzer_ports',
                            defaultValue: XBuzzerPorts.PORT1
                        },
                        SOUND: {
                            type: ArgumentType.NUMBER,
                            menu: 'xbuzzer_sounds',
                            defaultValue: XBuzzerSounds.AMBULANCE
                        }
                    }
                },
                {
                    opcode: 'HQRAudioPlayer_stop',
                    text: formatMessage({
                        id: 'xkithqrover.HQRAudioPlayer_stop',
                        default: 'buzzer[PORT] stop',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'xbuzzer_ports',
                            defaultValue: XBuzzerPorts.PORT1
                        }
                    }
                },
                {
                    opcode: 'XVoiceBroadcast_reportObject',
                    text: formatMessage({
                        id: 'xkithqrover.XVoiceBroadcast_reportObject',
                        default: 'voice broadcast[PORT] report[OBJECT] value[VALUE]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT3
                        },
                        OBJECT: {
                            type: ArgumentType.NUMBER,
                            menu: 'xvoicebroadcast_objects',
                            defaultValue: XVoiceBroadcastObjects.VALUE
                        },
                        VALUE: {
                            type: ArgumentType.NUMBER,
                            defaultValue: 0
                        }
                    }
                },
                {
                    opcode: 'XVoiceBroadcast_reportTime',
                    text: formatMessage({
                        id: 'xkithqrover.XVoiceBroadcast_reportTime',
                        default: 'voice broadcast[PORT] report time [HOUR]hour [MINUTE]minute [SECOND]secs',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT3
                        },
                        HOUR: {
                            type: ArgumentType.NUMBER,
                            defaultValue: 11
                        },
                        MINUTE: {
                            type: ArgumentType.NUMBER,
                            defaultValue: 11
                        },
                        SECOND: {
                            type: ArgumentType.NUMBER,
                            defaultValue: 11
                        }
                    }
                },
                {
                    opcode: 'XVoiceBroadcast_reportDate',
                    text: formatMessage({
                        id: 'xkithqrover.XVoiceBroadcast_reportDate',
                        default: 'voice broadcast[PORT] report date [YEAR]year [MONTH]month [DAY]day',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT3
                        },
                        YEAR: {
                            type: ArgumentType.NUMBER,
                            defaultValue: 2018
                        },
                        MONTH: {
                            type: ArgumentType.NUMBER,
                            defaultValue: 11
                        },
                        DAY: {
                            type: ArgumentType.NUMBER,
                            defaultValue: 11
                        }
                    }
                },
                {
                    opcode: 'XVoiceBroadcast_reportOperator',
                    text: formatMessage({
                        id: 'xkithqrover.XVoiceBroadcast_reportOperator',
                        default: 'voice broadcast[PORT] report operator [OPERATOR]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT3
                        },
                        OPERATOR: {
                            type: ArgumentType.NUMBER,
                            menu: 'xvoicebroadcast_operators',
                            defaultValue: XVoiceBroadcastOperators.ADD
                        }
                    }
                },
                {
                    opcode: 'XVoiceBroadcast_reportAction',
                    text: formatMessage({
                        id: 'xkithqrover.XVoiceBroadcast_reportAction',
                        default: 'voice broadcast[PORT] report action [ACTION] [REPEAT]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT3
                        },
                        ACTION: {
                            type: ArgumentType.NUMBER,
                            menu: 'xvoicebroadcast_actions',
                            defaultValue: 70
                        },
                        REPEAT: {
                            type: ArgumentType.NUMBER,
                            menu: 'repeats',
                            defaultValue: 0
                        }
                    }
                },
                {
                    opcode: 'XVoiceBroadcast_reportAlarm',
                    text: formatMessage({
                        id: 'xkithqrover.XVoiceBroadcast_reportAlarm',
                        default: 'voice broadcast[PORT] report alarm [ALARM] [REPEAT]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT3
                        },
                        ALARM: {
                            type: ArgumentType.NUMBER,
                            menu: 'xvoicebroadcast_alarms',
                            defaultValue: 78
                        },
                        REPEAT: {
                            type: ArgumentType.NUMBER,
                            menu: 'repeats',
                            defaultValue: 0
                        }
                    }
                },
                {
                    opcode: 'XVoiceBroadcast_reportScience',
                    text: formatMessage({
                        id: 'xkithqrover.XVoiceBroadcast_reportScience',
                        default: 'voice broadcast[PORT] report science [SCIENCE] [REPEAT]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT3
                        },
                        SCIENCE: {
                            type: ArgumentType.NUMBER,
                            menu: 'xvoicebroadcast_sciences',
                            defaultValue: 86
                        },
                        REPEAT: {
                            type: ArgumentType.NUMBER,
                            menu: 'repeats',
                            defaultValue: 0
                        }
                    }
                },
                {
                    opcode: 'XVoiceBroadcast_reportWarn',
                    text: formatMessage({
                        id: 'xkithqrover.XVoiceBroadcast_reportWarn',
                        default: 'voice broadcast[PORT] report warn [WARN] [REPEAT]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT3
                        },
                        WARN: {
                            type: ArgumentType.NUMBER,
                            menu: 'xvoicebroadcast_warns',
                            defaultValue: 94
                        },
                        REPEAT: {
                            type: ArgumentType.NUMBER,
                            menu: 'repeats',
                            defaultValue: 0
                        }
                    }
                },
                {
                    opcode: 'XVoiceBroadcast_reportOutstar',
                    text: formatMessage({
                        id: 'xkithqrover.XVoiceBroadcast_reportOutstar',
                        default: 'voice broadcast[PORT] report outstar [OUTSTAR] [REPEAT]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT3
                        },
                        OUTSTAR: {
                            type: ArgumentType.NUMBER,
                            menu: 'xvoicebroadcast_outstars',
                            defaultValue: 102
                        },
                        REPEAT: {
                            type: ArgumentType.NUMBER,
                            menu: 'repeats',
                            defaultValue: 0
                        }
                    }
                },
                {
                    opcode: 'XVoiceBroadcast_reportMood',
                    text: formatMessage({
                        id: 'xkithqrover.XVoiceBroadcast_reportMood',
                        default: 'voice broadcast[PORT] report mood [MOOD] [REPEAT]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT3
                        },
                        MOOD: {
                            type: ArgumentType.NUMBER,
                            menu: 'xvoicebroadcast_moods',
                            defaultValue: 110
                        },
                        REPEAT: {
                            type: ArgumentType.NUMBER,
                            menu: 'repeats',
                            defaultValue: 0
                        }
                    }
                },
                {
                    opcode: 'XVoiceBroadcast_reportMusic',
                    text: formatMessage({
                        id: 'xkithqrover.XVoiceBroadcast_reportMusic',
                        default: 'voice broadcast[PORT] report music [MUSIC] [REPEAT]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT3
                        },
                        MUSIC: {
                            type: ArgumentType.NUMBER,
                            menu: 'xvoicebroadcast_musics',
                            defaultValue: 118
                        },
                        REPEAT: {
                            type: ArgumentType.NUMBER,
                            menu: 'repeats',
                            defaultValue: 0
                        }
                    }
                },
                {
                    opcode: 'XVoiceBroadcast_stop',
                    text: formatMessage({
                        id: 'xkithqrover.XVoiceBroadcast_stop',
                        default: 'voice broadcast[PORT] stop',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT3
                        }
                    }
                },
                {
                    opcode: 'XLEDMatrix_setEffect',
                    text: formatMessage({
                        id: 'xkithqrover.XLEDMatrix_setEffect',
                        default: 'LED matrix[PORT] show effect[EFFECT] speed[SPEED]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT1
                        },
                        EFFECT: {
                            type: ArgumentType.NUMBER,
                            menu: 'xledmatrix_effects',
                            defaultValue: XLEDMatrixEffects.GREATER
                        },
                        SPEED: {
                            type: ArgumentType.NUMBER,
                            menu: 'speeds',
                            defaultValue: XKitHqroverSpeeds.SLOW
                        }
                    }
                },
                {
                    opcode: 'XLEDMatrix_showNumber',
                    text: formatMessage({
                        id: 'xkithqrover.XLEDMatrix_showNumber',
                        default: 'LED matrix[PORT] show number[NUM]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT1
                        },
                        NUM: {
                            type: ArgumentType.NUMBER,
                            defaultValue: 1.23
                        }
                    }
                },
                {
                    opcode: 'XLEDMatrix_showNumberPair',
                    text: formatMessage({
                        id: 'xkithqrover.XLEDMatrix_showNumberPair',
                        default: 'LED matrix[PORT] show score[A]:[B]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT1
                        },
                        A: {
                            type: ArgumentType.NUMBER,
                            defaultValue: 0
                        },
                        B: {
                            type: ArgumentType.NUMBER,
                            defaultValue: 0
                        }
                    }
                },
                {
                    opcode: 'XLEDMatrix_showEmoticon',
                    text: formatMessage({
                        id: 'xkithqrover.XLEDMatrix_showEmoticon',
                        default: 'LED matrix[PORT] show emoticon[EMOT]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT1
                        },
                        EMOT: {
                            type: ArgumentType.NUMBER,
                            menu: 'xledmatrix_emots',
                            defaultValue: XLEDMatrixEmots.COOL
                        }
                    }
                },
                {
                    opcode: 'XLEDMatrix_showFlag',
                    text: formatMessage({
                        id: 'xkithqrover.XLEDMatrix_showFlag',
                        default: 'LED matrix[PORT] show flag[FLAG]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT1
                        },
                        FLAG: {
                            type: ArgumentType.NUMBER,
                            menu: 'xledmatrix_flags',
                            defaultValue: XLEDMatrixFlags.X
                        }
                    }
                },
                {
                    opcode: 'XLEDMatrix_clear',
                    text: formatMessage({
                        id: 'xkithqrover.XLEDMatrix_clear',
                        default: 'LED matrix[PORT] clear',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XKitHqroverPorts.PORT1
                        }
                    }
                },
                '---',
                {
                    opcode: 'XIRReceiver_enableLongPress',
                    text: formatMessage({
                        id: 'xkithqrover.XIRReceiver_enableLongPress',
                        default: 'IR receiver[PORT] enable long press[KEY]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'irr_ports',
                            defaultValue: XIRReceiverPorts.PORT1
                        },
                        KEY: {
                            type: ArgumentType.STRING,
                            defaultValue: '1'
                        }
                    }
                },
                {
                    opcode: 'XIRReceiver_checkMessage',
                    text: formatMessage({
                        id: 'xkithqrover.XIRReceiver_checkMessage',
                        default: 'IR receiver[PORT] received message is[KEY]',
                        description: ''
                    }),
                    blockType: BlockType.BOOLEAN,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'irr_ports',
                            defaultValue: XIRReceiverPorts.PORT1
                        },
                        KEY: {
                            type: ArgumentType.STRING,
                            defaultValue: '1'
                        }
                    }
                }
            ],
            menus: {
                compare_oper: this.COMPARE_OPER_MENU,
                ports: this.PORTS_MENU,
                speeds: this.SPEEDS_MENU,
                repeats: this.REPEATS_MENU,
                xsoundsensor_ports: this.XSOUNDSENSOR_PORTS_MENU,
                xlightsensor_ports: this.XLIGHTSENSOR_PORTS_MENU,
                xbutton_ports: this.XBUTTON_PORTS_MENU,
                irtracking_status: this.XIRTRACKING_STATUS_MENU,
                hqrcardriver_dirs: this.HQRCARDRIVER_DIRS_MENU,
                hqrcardriver_actions: this.HQRCARDRIVER_ACTIONS_MENU,
                xsegdisplay_indexs: this.XSEGDISPLAY_INDEXS_MENU,
                xsegdisplay_segments: this.XSEGDISPLAY_SEGMENTS_MENU,
                rgb_ports: this.XRGBLED_PORTS_MENU,
                rgb_indexs: this.XRGBLED_INDEXS_MENU,
                xbuzzer_ports: this.XBUZZER_PORTS_MENU,
                xbuzzer_notes: this.XBUZZER_NOTES_MENU,
                XBuzzer_Scales: this.XBUZZER_SCALES_MENU,
                xbuzzer_tones: this.XBUZZER_TONES_MENU,
                xbuzzer_beats: this.XBUZZER_BEATS_MENU,
                XBuzzer_Musics: this.XBUZZER_MUSICS_MENU,
                xbuzzer_sounds: this.XBUZZER_SOUNDS_MENU,
                xvoicebroadcast_objects: this.XVOICEBROADCAST_OBJECTS_MENU,
                xvoicebroadcast_operators: this.XVOICEBROADCAST_OPERATORS_MENU,
                xvoicebroadcast_actions: this.XVOICEBROADCAST_SOUND_ACTIONS_MENU,
                xvoicebroadcast_alarms: this.XVOICEBROADCAST_SOUND_ALARMS_MENU,
                xvoicebroadcast_sciences: this.XVOICEBROADCAST_SOUND_SCIENCES_MENU,
                xvoicebroadcast_warns: this.XVOICEBROADCAST_SOUND_WARNS_MENU,
                xvoicebroadcast_outstars: this.XVOICEBROADCAST_SOUND_OUTSTARS_MENU,
                xvoicebroadcast_moods: this.XVOICEBROADCAST_SOUND_MOODS_MENU,
                xvoicebroadcast_musics: this.XVOICEBROADCAST_SOUND_MUSICS_MENU,
                xledmatrix_effects: this.XLEDMATRIX_EFFECTS_MENU,
                xledmatrix_emots: this.XLEDMATRIX_EMOTS_MENU,
                xledmatrix_flags: this.XLEDMATRIX_FLAGS_MENU,
                irr_ports: this.XIRRECEIVER_PORTS_MENU
            }
        };
    }

    /*constructBlock (args) {
        var block = this._peripheral.findBlock(args.PORT);
        if (!block) {
            block = this._peripheral.constructBlock(args.TYPE, args.MODEL, args.PORT);
            
            return new Promise(resolve => {
                setTimeout(() => {
                    resolve();
                }, XBridgeSendInterval);
            });
        } 
    }*/

    ///Move
    
    HQRCarDriver_move (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        if(args.DIR==HQRCarDriverDirs.FORWARD) {
            this._peripheral.HQRCarDriver_forward(args.SPEED);
        } else {
            this._peripheral.HQRCarDriver_backward(args.SPEED);
        }

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    HQRCarDriver_turn (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.HQRCarDriver_turn(args.ACTION, args.SPEED, args.ANGLE);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    HQRCarDriver_stop (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.HQRCarDriver_stop();

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }
    
    HQRCarDriver_autoLineTracking(args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.HQRCarDriver_autoLineTracking(args.IRT_PORT, args.SPEED);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    HQRCarDriver_autoObstacleAvoidance(args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.HQRCarDriver_autoObstacleAvoidance(args.ULS_PORT, args.SPEED);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    //Servo 不要

    ///Sense

    XSoundSensor_getVolume (args) {
        if (!this._peripheral.isConnected()) return 0;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        var block = this._peripheral.findBlock(args.PORT);
        if(block) {
            //return block.volume;
            this._peripheral.XSoundSensor_getVolume(args.PORT);
        } else {
            block = this._peripheral.constructBlock('XSoundSensor', '', args.PORT);
        }

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve(block.volume);
                }
            }, XBridgeSendInterval);
        });*/
    }

    XLightSensor_getLuminance (args) {
        if (!this._peripheral.isConnected()) return 0;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        var block = this._peripheral.findBlock(args.PORT);
        if(block) {
            //return block.luminance;
            this._peripheral.XLightSensor_getLuminance(args.PORT);
        } else {
            block = this._peripheral.constructBlock('XLightSensor', '', args.PORT);
        }

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve(block.luminance);
                }
            }, XBridgeSendInterval);
        });*/
    }

    // XButton_whenPressed (args) {
    //     if (!this._peripheral.isConnected()) return false;

    //     return this._peripheral.XButton_isPressed(args.PORT);
    // }

    XButton_isPressed (args) {
        if (!this._peripheral.isConnected()) return false;

        var block = this._peripheral.findBlock(args.PORT);
        if(block) {
            return block.status == 1;
        } else {
            const promise = new Promise((resolve, reject) => {
                this._peripheral._request = {resolve, reject};
            });

            block = this._peripheral.constructBlock('XButton', '', args.PORT);

            return promise;
        }
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve(block.status == 1);
                }
            }, XBridgeSendInterval);
        });*/
        //return this._peripheral.XButton_isPressed(args.PORT);
    }

    XIRTracking_checkStatus (args) {
        if (!this._peripheral.isConnected()) return false;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        var block = this._peripheral.findBlock(args.PORT);
        if(block) {
            //return block.status == args.STATUS;
            this._peripheral.XIRTracking_getStatus(args.PORT);
        } else {
            block = this._peripheral.constructBlock('XIRTracking', XIRTrackingModels.MODEL1, args.PORT);
        }

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve(block.status == args.STATUS);
                }
            }, XBridgeSendInterval);
        });*/
        //return this._peripheral.XIRTracking_getStatus(args.PORT) == args.STATUS;
    }

    XUltrasonic_getDistance (args) {
        if (!this._peripheral.isConnected()) return 0;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        var block = this._peripheral.findBlock(args.PORT);
        if(block) {
            //return block.distance;
            this._peripheral.XUltrasonic_getDistance(args.PORT);
        } else {
            block = this._peripheral.constructBlock('XUltrasonic', XUltrasonicModels.MODEL1, args.PORT);
        }

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve(block.distance/10);
                }
            }, XBridgeSendInterval);
        });*/
        //return this._peripheral.XUltrasonic_getDistance(args.PORT);
    }

    XIRAvoiding_checkStatus (args) {
        if (!this._peripheral.isConnected()) return false;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        var block = this._peripheral.findBlock(args.PORT);
        if(block) {
            //return block.status == args.STATUS;
            this._peripheral.XIRAvoiding_getStatus(args.PORT);
        } else {
            block = this._peripheral.constructBlock('XIRAvoiding', XIRAvoidingModels.MODEL1, args.PORT);
        }

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve(block.status == args.STATUS);
                }
            }, XBridgeSendInterval);
        });*/
        //return this._peripheral.XIRAvoiding_getStatus(args.PORT) == args.STATUS;
    }

    ///Show

    XSegDisplay_showNumber (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XSegDisplay_showNumber(args.PORT, args.NUM);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XSegDisplay_showCharacter (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XSegDisplay_showCharacter(args.PORT, args.INDEX, args.CHAR);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XSegDisplay_showSegment (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XSegDisplay_showSegment(args.PORT, args.INDEX, args.SEG);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XSegDisplay_clearSegment (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XSegDisplay_clearSegment(args.PORT, args.INDEX, args.SEG);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XSegDisplay_clear (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XSegDisplay_clear(args.PORT);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    HQRLightShow_showColor (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        const rgb = cast.toRgbColorObject(args.COLOR);
        //const hsv = Color.rgbToHsv(rgb);
        this._peripheral.HQRLightShow_showColor(args.PORT, args.INDEX, rgb.r, rgb.g, rgb.b);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    HQRLightShow_showBreath (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        const rgb = cast.toRgbColorObject(args.COLOR);
        //const hsv = Color.rgbToHsv(rgb);
        this._peripheral.HQRLightShow_showBreath(args.PORT, args.INDEX, args.SPEED, rgb.r, rgb.g, rgb.b);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    HQRLightShow_showMeteor (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        const rgb = cast.toRgbColorObject(args.COLOR);
        //const hsv = Color.rgbToHsv(rgb);
        this._peripheral.HQRLightShow_showMeteor(args.PORT, args.SPEED, rgb.r, rgb.g, rgb.b);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    HQRLightShow_clear (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.HQRLightShow_clear(args.PORT, args.INDEX);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    HQRAudioPlayer_setNoteParameter (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        //0.1秒就是100ms, 2秒就是2000，对应beatTime分别就是10和200, 然后转换为一个字节
        if(args.BEATTIME < 0.1) {
            args.BEATTIME = 0.1;
        }
        else if(args.BEATTIME > 2.0) {
            args.BEATTIME = 2.0;
        }
        this._peripheral.HQRAudioPlayer_setNoteParameter(args.PORT, args.BEATTIME*100);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    HQRAudioPlayer_playNote (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.HQRAudioPlayer_playNote(args.PORT, args.NOTE, args.SCALE, args.TONE, args.BEAT);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    HQRAudioPlayer_playMusic (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.HQRAudioPlayer_playMusic(args.PORT, args.MUSIC);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    HQRAudioPlayer_playSound (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.HQRAudioPlayer_playSound(args.PORT, args.SOUND);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    HQRAudioPlayer_stop (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.HQRAudioPlayer_stop(args.PORT);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XVoiceBroadcast_reportObject (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XVoiceBroadcast_reportObject(args.PORT, args.OBJECT);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XVoiceBroadcast_reportTime (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XVoiceBroadcast_reportTime(args.PORT, args.HOUR, args.MINUTE, args.SECOND);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XVoiceBroadcast_reportDate (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XVoiceBroadcast_reportDate(args.PORT, args.YEAR, args.MONTH, args.DAY);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XVoiceBroadcast_reportOperator (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XVoiceBroadcast_reportOperator(args.PORT, args.OPERATOR);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XVoiceBroadcast_reportAction (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XVoiceBroadcast_reportSond(args.PORT, args.ACTION, args.REPEAT);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XVoiceBroadcast_reportAlarm (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XVoiceBroadcast_reportSond(args.PORT, args.ALARM, args.REPEAT);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XVoiceBroadcast_reportScience (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XVoiceBroadcast_reportSond(args.PORT, args.SCIENCE, args.REPEAT);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XVoiceBroadcast_reportWarn (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XVoiceBroadcast_reportSond(args.PORT, args.WARN, args.REPEAT);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XVoiceBroadcast_reportOutstar (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XVoiceBroadcast_reportSond(args.PORT, args.OUTSTAR, args.REPEAT);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XVoiceBroadcast_reportMood (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XVoiceBroadcast_reportSond(args.PORT, args.MOOD, args.REPEAT);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XVoiceBroadcast_reportMusic (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XVoiceBroadcast_reportSond(args.PORT, args.MUSIC, args.REPEAT);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XVoiceBroadcast_stop (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XVoiceBroadcast_stop(args.PORT);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XLEDMatrix_setEffect (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XLEDMatrix_setEffect(args.PORT, args.EFFECT, args.SPEED);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XLEDMatrix_showNumber (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XLEDMatrix_showNumber(args.PORT, args.NUM);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XLEDMatrix_showNumberPair (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XLEDMatrix_showNumberPair(args.PORT, args.A, args.B);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XLEDMatrix_showEmoticon (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XLEDMatrix_showEmoticon(args.PORT, args.EMOT);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XLEDMatrix_showFlag (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XLEDMatrix_showFlag(args.PORT, args.FLAG);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XLEDMatrix_clear (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XLEDMatrix_clear(args.PORT);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    //Message

    XIRReceiver_enableLongPress (args) {
        if (!this._peripheral.isConnected()) return;

        const promise = new Promise((resolve, reject) => {
            this._peripheral._request = {resolve, reject};
        });

        this._peripheral.XIRReceiver_enableLongPress(args.PORT, args.KEY);

        return promise;
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    resolve();
                }
            }, XBridgeSendInterval);
        });*/
    }

    XIRReceiver_checkMessage (args) {
        if (!this._peripheral.isConnected()) return false;

        switch (args.KEY) {
            case 'A':
                args.KEY = '10';
                break;
            case 'B':
                args.KEY = '11';
                break;
            case 'C':
                args.KEY = '12';
                break;
            case 'D':
                args.KEY = '13';
                break;
            case 'E':
                args.KEY = '14';
                break;
            case 'F':
                args.KEY = '15';
                break;
            case '↑':
                args.KEY = '16';
                break;
            case '↓':
                args.KEY = '17';
                break;
            case '←':
                args.KEY = '18';
                break;
            case '→':
                args.KEY = '19';
                break;
            case 'OK':
                args.KEY = '20';
                break;
            default:
                break;
        }
        var block = this._peripheral.findBlock(args.PORT);
        if(block) {
            var ret = block.key == args.KEY;
            block.key = BYTE_INVALID_VALUE;
            return ret;
        } else {
            const promise = new Promise((resolve, reject) => {
                this._peripheral._request = {resolve, reject};
            });

            block = this._peripheral.constructBlock('XIRReceiver', '', args.PORT);

            return promise;
        }
        /*return new Promise(resolve => {
            this._intervalID = window.setInterval(() => {
                if(!this._peripheral.isBusy()) {
                    window.clearInterval(this._intervalID);
                    var ret = block.key == args.KEY;
                    block.key = BYTE_INVALID_VALUE;
                    resolve(ret);
                }
            }, XBridgeSendInterval);
        });*/
        //return this._peripheral.XIRReceiver_checkMessage(args.PORT) == args.KEY;
    }

}

module.exports = Scratch3XKitHqroverBlocks;
