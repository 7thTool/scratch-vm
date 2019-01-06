const JSONRPCWebSocket = require('../util/jsonrpc-web-socket');
const ScratchLinkWebSocket = 'ws://localhost:8080/scratch/xbridge';
// const log = require('../util/log');

class XBridge extends JSONRPCWebSocket {

    /**
     * A XBridge peripheral socket object.  It handles connecting, over web sockets, to
     * XBridge peripherals, and reading and writing data to them.
     * @param {Runtime} runtime - the Runtime for sending/receiving GUI update events.
     * @param {string} extensionId - the id of the extension using this socket.
     * @param {object} peripheralOptions - the list of options for peripheral discovery.
     * @param {object} connectCallback - a callback for connection.
     */
    constructor (runtime, extensionId, peripheralOptions, connectCallback, didReceiveCallback) {
        const ws = new WebSocket(ScratchLinkWebSocket);
        super(ws);

        this._ws = ws;
        this._ws.onopen = this.requestPeripheral.bind(this); // only call request peripheral after socket opens
        this._ws.onerror = this._sendRequestError.bind(this, 'ws onerror');
        this._ws.onclose = this._sendDisconnectError.bind(this, 'ws onclose');

        this._availablePeripherals = {};
        this._connectCallback = connectCallback;
        this._connected = false;
        this._didReceiveCallback = didReceiveCallback;
        this._characteristicDidChangeCallback = null;
        this._extensionId = extensionId;
        this._peripheralOptions = peripheralOptions;
        this._discoverTimeoutID = null;
        this._runtime = runtime;
    }

    /**
     * Request connection to the peripheral.
     * If the web socket is not yet open, request when the socket promise resolves.
     */
    requestPeripheral () {
        if (this._ws.readyState === 1) { // is this needed since it's only called on ws.onopen?
            this._availablePeripherals = {};
            if (this._discoverTimeoutID) {
                window.clearTimeout(this._discoverTimeoutID);
            }
            this._discoverTimeoutID = window.setTimeout(this._sendDiscoverTimeout.bind(this), 15000);
            this.sendRemoteRequest('discover', this._peripheralOptions)
                .catch(e => {
                    this._sendRequestError(e);
                });
        }
        // TODO: else?
    }

    /**
     * Try connecting to the input peripheral id, and then call the connect
     * callback if connection is successful.
     * @param {number} id - the id of the peripheral to connect to
     */
    connectPeripheral (id) {
        this.sendRemoteRequest('connect', {peripheralId: id})
            .then(() => {
                this._connected = true;
                this._runtime.emit(this._runtime.constructor.PERIPHERAL_CONNECTED);
                this._connectCallback();
            })
            .catch(e => {
                this._sendRequestError(e);
            });
    }

    /**
     * Close the websocket.
     */
    disconnect () {
        this._ws.close();
        if (this._discoverTimeoutID) {
            window.clearTimeout(this._discoverTimeoutID);
        }
    }

    /**
     * @return {bool} whether the peripheral is connected.
     */
    isConnected () {
        return this._connected;
    }

    /**
     * Start receiving notifications from the specified ble service.
     * @param {number} serviceId - the ble service to read.
     * @param {number} characteristicId - the ble characteristic to get notifications from.
     * @param {object} onCharacteristicChanged - callback for characteristic change notifications.
     * @return {Promise} - a promise from the remote startNotifications request.
     */
    startNotifications (serviceId, characteristicId, onCharacteristicChanged = null) {
        const params = {
            serviceId,
            characteristicId
        };
        this._characteristicDidChangeCallback = onCharacteristicChanged;
        return this.sendRemoteRequest('startNotifications', params)
            .catch(e => {
                this._sendDisconnectError(e);
            });
    }

    rpc (method, params) {
        return this.sendRemoteRequest(method, params)
            .catch(e => {
                this._sendDisconnectError(e);
            });
    }

    /**
     * Handle a received call from the socket.
     * @param {string} method - a received method label.
     * @param {object} params - a received list of parameters.
     * @return {object} - optional return value.
     */
    didReceiveCall (method, params) {
        switch (method) {
        case 'didDiscoverPeripheral':
            this._availablePeripherals[params.peripheralId] = params;
            this._runtime.emit(
                this._runtime.constructor.PERIPHERAL_LIST_UPDATE,
                this._availablePeripherals
            );
            if (this._discoverTimeoutID) {
                window.clearTimeout(this._discoverTimeoutID);
            }
            break;
        case 'ping':
            return 42;
        default:
            if(this._didReceiveCallback) {
                return this._didReceiveCallback(method, params);
            }
            break;
        }
    }

    _sendRequestError (/* e */) {
        // log.error(`XBridge error: ${JSON.stringify(e)}`);

        this._runtime.emit(this._runtime.constructor.PERIPHERAL_REQUEST_ERROR, {
            message: `Scratch lost connection to`,
            extensionId: this._extensionId
        });
    }

    _sendDisconnectError (/* e */) {
        // log.error(`XBridge error: ${JSON.stringify(e)}`);

        if (!this._connected) return;

        this._connected = false;

        this._runtime.emit(this._runtime.constructor.PERIPHERAL_DISCONNECT_ERROR, {
            message: `Scratch lost connection to`,
            extensionId: this._extensionId
        });
    }

    _sendDiscoverTimeout () {
        if (this._discoverTimeoutID) {
            window.clearTimeout(this._discoverTimeoutID);
        }
        this._runtime.emit(this._runtime.constructor.PERIPHERAL_SCAN_TIMEOUT);
    }
}

module.exports = XBridge;
