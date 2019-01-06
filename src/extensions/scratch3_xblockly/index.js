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
const blockIconURI = 'data:image/png;base64,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';

// TODO: Needs comment
const XBridgeTimeout = 4500; // TODO: might need tweaking based on how long the peripheral takes to start sending data

/**
 * A time interval to wait (in milliseconds) while a block that sends a XBridge message is running.
 * @type {number}
 */
const XBridgeSendInterval = 100;

/**
 * Manage communication with a XBlockly peripheral over a Scrath Link client socket.
 */
class XBlockly {

    /**
     * Construct a XBlockly communication object.
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

        /**
         * ID for a timeout which is used to clear the busy flag if it has been
         * true for a long time.
         */
        this._busyTimeoutID = null;

        this.disconnect = this.disconnect.bind(this);
        this._onConnect = this._onConnect.bind(this);
        this._onReceive = this._onReceive.bind(this);
    }

    isOnBoardBlock(port) {
        return isNaN(port);
    }

    constructBlock(type, model, port) {
        const block = {
            type,
            model,
            port
        };
        this._blocks[port] = block;
        //放到服务端构建，即调用具体模块方法时传模块构建模块所需参数到服务端
        // if(!this.isOnBoardBlock(port)) { //非板载模块需要主动构建
        //     this.rpc('constructBlock', block
        //     , (result) => {
        //         /*switch(type) 
        //         {
        //         case 'XButton':
        //         {
        //             this.rpc('XButton_registerEvent', block);
        //         }
        //         break;
        //         }*/
        //     }
        //     );
        // }
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
        const server = {
            type,
            ports
        };
        this._services[key] = server;
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

    /*XButton_registerEvent(port) {
        var block = this.findOrConstructBlock('XButton', '', port);
        if (block) {
            if(!block.isRegister) {
                block.isRegister = true;
                block.isPressed = false;
                const params = {
                    type: block.type,
                    model: block.model,
                    port: block.port
                };
                this.rpc("XButton_registerEvent", params);
            }
        }
    }*/

    XButton_isPressed(port) {
        const block = this.findOrConstructBlock('XButton', XButtonModels.MODEL1, port);
        if (block) {
            return block.isPressed;
        } 
        return false;
    }
    
    XSegDisplay_showNumber(port, num) {
        const block = this.findOrConstructBlock('XSegDisplay', XSegDisplayModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: XSegDisplayModels.MODEL1,
                port: block.port,
                num
            };
            this.rpc("XSegDisplay_showNumber", params);
        }
    }

    XSegDisplay_clear(port) {
        const block = this.findOrConstructBlock('XSegDisplay', XSegDisplayModels.MODEL1, port);
        if (block) {
            const params = {
                type: block.type,
                model: XSegDisplayModels.MODEL1,
                port: block.port
            };
            this.rpc("XSegDisplay_clear", params);
        }
    }
    
    /*HQRCarDriver_showNumber(sgd, num) {
        const block = this.findOrConstructBlock('XButton', XButtonModels.MODEL1, port);
        const server = this.findOrConstructServer('XSegDisplay', sgd);
        if (server) {
            const params = {
                type: server.type,
                ports: server.ports,
                num
            };
            this.rpc("XSegDisplay_showNumber", params);
        }
    }*/

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
     * Disconnect from the xblockly.
     */
    disconnect () {
        this._blocks = {};
        this._services = {};
        window.clearInterval(this._timeoutID);
        if (this._xbridge) {
            this._xbridge.disconnect();
        }
    }

    /**
     * Return true if connected to the xblockly.
     * @return {boolean} - whether the xblockly is connected.
     */
    isConnected () {
        let connected = false;
        if (this._xbridge) {
            connected = this._xbridge.isConnected();
        }
        return connected;
    }

    rpc (method, params, callback) {
        if (!this.isConnected()) return;
        if (this._busy) return;

        // Set a busy flag so that while we are sending a message and waiting for
        // the response, additional messages are ignored.
        this._busy = true;

        // Set a timeout after which to reset the busy flag. This is used in case
        // a XBridge message was sent for which we never received a response, because
        // e.g. the peripheral was turned off after the message was sent. We reset
        // the busy flag after a while so that it is possible to try again later.
        this._busyTimeoutID = window.setTimeout(() => {
            this._busy = false;
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

        return new Promise(resolve => {
            setTimeout(() => {
                resolve();
            }, XBridgeSendInterval);
        });
    }

    /**
     * Starts reading data from peripheral after XBridge has connected to it.
     * @private
     */
    _onConnect () {
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
        case 'XButton_notifyEvent':
            {
                block = this.findBlock(params.port);
                //delete this._blocks[params.port];
                if (block) {
                    //block.isRegister = false;
                    block.isPressed = true;
                }
            }
            break;
        }
    }
}

/**
 * Enum for XBlockly.
 */
const XBlocklyPorts = {
    PORT1: '1',
    PORT2: '2',
    PORT3: '3',
    PORT4: '4'
};

const XBlocklyTypes = {
    TYPE1: 'XButton',
    TYPE2: 'XSegDisplay'
};

const XBlocklyModels = {
    MODEL1: 'BTN3200',
    MODEL2: 'SGD4300'
};

/**
 * Enum for XButton.
 */

const XButtonPorts = {
    PORT1: 'BTN'
};

const XButtonModels = {
    MODEL1: 'BTN3200'
};

/**
 * Enum for XSegDisplay.
 */

const XSegDisplayModels = {
    MODEL1: 'SGD4300'
};

/**
 * Scratch 3.0 blocks to interact with a XBlockly peripheral.
 */
class Scratch3XBlocklyBlocks {

    /**
     * @return {string} - the name of this extension.
     */
    static get EXTENSION_NAME () {
        return 'xblockly';
    }

    /**
     * @return {string} - the ID of this extension.
     */
    static get EXTENSION_ID () {
        return 'xblockly';
    }

    /**
     * @return {array} - text and values for each buttons menu element
     */
    get PORTS_MENU () {
        return [
            {
                text: XBlocklyPorts.PORT1,
                value: XBlocklyPorts.PORT1
            },
            {
                text: XBlocklyPorts.PORT2,
                value: XBlocklyPorts.PORT2
            },
            {
                text: XBlocklyPorts.PORT3,
                value: XBlocklyPorts.PORT3
            },
            {
                text: XBlocklyPorts.PORT4,
                value: XBlocklyPorts.PORT4
            }
        ];
    }
    
    get TYPES_MENU () {
        return [
            {
                text: 'XButton',
                value: XBlocklyTypes.TYPE1
            },
            {
                text: 'XSegDisplay',
                value: XBlocklyTypes.TYPE2
            }
        ];
    }

    get MODELS_MENU () {
        return [
            {
                text: XBlocklyModels.MODEL1,
                value: XBlocklyModels.MODEL1
            }
        ];
    }

    /**
     * @return {array} - text and values for each buttons menu element
     */
    get BUTTONS_MENU () {
        return [
            {
                text: XButtonPorts.PORT1,
                value: XButtonPorts.PORT1
            },
            {
                text: XBlocklyPorts.PORT1,
                value: XBlocklyPorts.PORT1
            },
            {
                text: XBlocklyPorts.PORT2,
                value: XBlocklyPorts.PORT2
            },
            {
                text: XBlocklyPorts.PORT3,
                value: XBlocklyPorts.PORT3
            },
            {
                text: XBlocklyPorts.PORT4,
                value: XBlocklyPorts.PORT4
            }
        ];
    }

    /**
     * Construct a set of XBlockly blocks.
     * @param {Runtime} runtime - the Scratch 3.0 runtime.
     */
    constructor (runtime) {
        /**
         * The Scratch 3.0 runtime.
         * @type {Runtime}
         */
        this.runtime = runtime;

        // Create a new XBlockly peripheral instance
        this._peripheral = new XBlockly(this.runtime, Scratch3XBlocklyBlocks.EXTENSION_ID);
    }

    /**
     * @returns {object} metadata for this extension and its blocks.
     */
    getInfo () {
        return {
            id: Scratch3XBlocklyBlocks.EXTENSION_ID,
            name: Scratch3XBlocklyBlocks.EXTENSION_NAME,
            blockIconURI: blockIconURI,
            showStatusButton: true,
            blocks: [
                /*{
                    opcode: 'constructBlock',
                    text: formatMessage({
                        id: 'xblockly.constructBlock',
                        default: '[PORT] connect block [TYPE] model [MODEL]',
                        description: 'connect type and model block with port'
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XBlocklyPorts.PORT1
                        },
                        TYPE: {
                            type: ArgumentType.STRING,
                            menu: 'types',
                            defaultValue: XBlocklyTypes.TYPE1
                        },
                        MODEL: {
                            type: ArgumentType.STRING,
                            menu: 'models',
                            defaultValue: XBlocklyModels.MODEL1
                        }
                    }
                },
                '---',
                {
                    opcode: 'XButton_registerEvent',
                    text: formatMessage({
                        id: 'xblockly.XButton_registerEvent',
                        default: 'register [PORT] button pressed event',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'buttons',
                            defaultValue: XButtonPorts.PORT1
                        }
                    }
                },*/
                {
                    opcode: 'XButton_isPressed',
                    text: formatMessage({
                        id: 'xblockly.XButton_isPressed',
                        default: '[PORT] button pressed?',
                        description: 'is the selected button on the xblockly pressed?'
                    }),
                    blockType: BlockType.BOOLEAN,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'buttons',
                            defaultValue: XButtonPorts.PORT1
                        }
                    }
                },
                '---',
                {
                    opcode: 'XSegDisplay_showNumber',
                    text: formatMessage({
                        id: 'xblockly.XSegDisplay_showNumber',
                        default: 'SegDisplay[PORT] show number [NUM]',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XBlocklyPorts.PORT1
                        },
                        NUM: {
                            type: ArgumentType.NUMBER,
                            defaultValue: 123
                        }
                    }
                },
                {
                    opcode: 'XSegDisplay_clear',
                    text: formatMessage({
                        id: 'xblockly.XSegDisplay_clear',
                        default: 'SegDisplay[PORT] clear',
                        description: ''
                    }),
                    blockType: BlockType.COMMAND,
                    arguments: {
                        PORT: {
                            type: ArgumentType.STRING,
                            menu: 'ports',
                            defaultValue: XBlocklyPorts.PORT1
                        }
                    }
                }
            ],
            menus: {
                ports: this.PORTS_MENU,
                types: this.TYPES_MENU,
                models: this.MODELS_MENU,
                buttons: this.BUTTONS_MENU
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
    }

    XButton_registerEvent (args) {
        this._peripheral.XButton_registerEvent(args.PORT);

        return new Promise(resolve => {
            setTimeout(() => {
                resolve();
            }, XBridgeSendInterval);
        });
    }*/

    XButton_isPressed (args) {
        return this._peripheral.XButton_isPressed(args.PORT);
    }

    XSegDisplay_showNumber (args) {
        this._peripheral.XSegDisplay_showNumber(args.PORT, args.NUM);

        return new Promise(resolve => {
            setTimeout(() => {
                resolve();
            }, XBridgeSendInterval);
        });
    }

    XSegDisplay_clear (args) {
        this._peripheral.XSegDisplay_clear(args.PORT);

        return new Promise(resolve => {
            setTimeout(() => {
                resolve();
            }, XBridgeSendInterval);
        });
    }
}

module.exports = Scratch3XBlocklyBlocks;
