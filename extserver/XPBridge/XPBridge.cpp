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


#include <assert.h>
#include "XPBridge.hpp"


#if 1
#define LOGV(x)      printf x
#define LOGI(x)      printf x
#define LOGW(x)      printf x
#define LOGE(x)      printf x
#else
#define LOGV(x)
#define LOGI(x)
#define LOGW(x)
#define LOGE(x)
#endif


/**
 *  Define a global standalone XBridge.
 */
XPBridge XBridge;


uint8_t * fetchU8(const uint8_t *buf, uint8_t *val)
{
    *val = *buf;
    return (uint8_t *)buf + 1;
}

uint8_t * fetchU16(const uint8_t *buf, uint16_t *val)
{
    *val = (buf[0] << 8) | buf[1];
    return (uint8_t *)buf + 2;
}

uint8_t * fetchU32(const uint8_t *buf, uint32_t *val)
{
    *val = ((uint32_t)buf[0] << 24) |
    ((uint32_t)buf[1] << 16) |
    ((uint32_t)buf[2] << 8) |
    (uint32_t)buf[3];
    return (uint8_t *)buf + 4;
}

uint8_t * fetchString(const uint8_t *buf, char **s)
{
    *s = (char *)buf;
    return (uint8_t *)buf + strlen(*s) + 1;
}

uint8_t * fetchFloat(const uint8_t *buf, float *val)
{
    uint32_t v = ((uint32_t)buf[0] << 24) |
    ((uint32_t)buf[1] << 16) |
    ((uint32_t)buf[2] << 8) |
    (uint32_t)buf[3];
    *val = v / 1000.0;
    return (uint8_t *)buf + 4;
}

uint8_t * fillU8(uint8_t *buf, uint8_t val)
{
    *buf = val;
    return buf + 1;
}

uint8_t * fillU16(uint8_t *buf, uint16_t val)
{
    buf[0] = (uint8_t)((val >> 8) & 0xff);
    buf[1] = (uint8_t)(val & 0xff);
    return buf + 2;
}

uint8_t * fillU32(uint8_t *buf, uint32_t val)
{
    buf[0] = (uint8_t)((val >> 24) & 0xff);
    buf[1] = (uint8_t)((val >> 16) & 0xff);
    buf[2] = (uint8_t)((val >> 8) & 0xff);
    buf[3] = (uint8_t)(val & 0xff);
    return buf + 4;
}

uint8_t * fillString(uint8_t *buf, const char *s)
{
    strcpy((char *)buf, s);
    return buf + strlen(s) + 1;
}

uint8_t * fillFloat(uint8_t *buf, float val)
{
    uint32_t v = val * 1000;
    buf[0] = (v >> 24) & 0xff;
    buf[1] = (v >> 16) & 0xff;
    buf[2] = (v >> 8) & 0xff;
    buf[3] = v & 0xff;
    return buf + 4;
}


int XPBlock::setup(const char *model, const char *port)
{
    LOGI(("XPBlock::setup(%s, %s)\n", model, port));
    return XBridge.constructExternalBlock(this, model, port);
}

int XPBlock::setup(const char *label)
{
    LOGI(("XPBlock::setup(%s)\n", label));
    return XBridge.constructOnboardBlock(this, label);
}

int XPBlock::access(uint8_t api, const uint8_t *param, uint8_t psize, uint8_t *result, uint8_t *rsize, int timeout, bool noResponse)
{
    LOGI(("XPBlock::access(%d, %p, %d, %p, %p, %d, %d)\n", api, param, psize, result, rsize, timeout, noResponse));
    return XBridge.accessBlock(uid(), api, param, psize, result, rsize, timeout, noResponse);
}

int XPBlock::notifyRegister(uint8_t evt, const uint8_t *param, uint8_t psize)
{
    LOGI(("XPBlock::notifyRegister(%d, %p, %d)\n", evt, param, psize));
    return XBridge.registerBlockNotify(uid(), evt, param, psize);
}


int XPServer::setup(const uint8_t *param, uint8_t psize)
{
    LOGI(("XPServer::setup(%p, %d)\n", param, psize));
    return XBridge.constructServer(this, param, psize);
}

int XPServer::access(uint8_t api, const uint8_t *param, uint8_t psize, uint8_t *result, uint8_t *rsize, int timeout, bool noResponse)
{
    LOGI(("XPServer::access(%d, %p, %d, %p, %p, %d, %d)\n", api, param, psize, result, rsize, timeout, noResponse));
    return XBridge.accessServer(uid(), api, param, psize, result, rsize, timeout, noResponse);
}

int XPServer::notifyRegister(uint8_t evt, const uint8_t *param, uint8_t psize)
{
    LOGI(("XPServer::notifyRegister(%d, %p, %d)\n", evt, param, psize));
    return XBridge.registerServerNotify(uid(), evt, param, psize);
}


BridgeLinklayer::BridgeLinklayer()
:mLinkListener(NULL)
,mTranslayer(NULL)
{
    LOGI(("BridgeLinklayer::BridgeLinklayer()\n"));
}

BridgeLinklayer::~BridgeLinklayer()
{
    LOGI(("BridgeLinklayer::~BridgeLinklayer()\n"));
}

int BridgeLinklayer::setup(BridgeLinklayer::LinkListener *listener, BridgeTranslayer* translayer)
{
	LOGI(("BridgeLinklayer::setup(%p %p)\n", listener, translayer));
	
	mLinkListener = listener;
    mTranslayer = translayer;

	return 0;
}

void BridgeLinklayer::release()
{
    
}

BridgeTranslayer::BridgeTranslayer(BridgeLinklayer* linklayer)
:mLinklayer(linklayer)
,mTransListener(NULL)
,mLock(NULL)
,mSignal(NULL)
{
    LOGI(("BridgeTranslayer::BridgeTranslayer()\n"));
}

BridgeTranslayer::~BridgeTranslayer()
{
    LOGI(("BridgeTranslayer::~BridgeTranslayer()\n"));
    
    release();
}

int BridgeTranslayer::setup(BridgeTranslayer::TransListener *listener)
{
	LOGI(("BridgeTranslayer::setup(%p)\n", listener));
	
	mTransListener = listener;

    do
    {
        mSignal = signal_init();
        if (!mSignal) {
            LOGE(("signal_init() failed\n"));
            break;
        }

        mLock = lock_init();
        if (!mLock) {
            LOGE(("lock_init() failed\n"));
            break;
        }

        break;
    } while(true);

	mResponsePacket.clear();
    mNotifyPacket.clear();
	return 0;
}

void BridgeTranslayer::release()
{
	LOGI(("BridgeTranslayer::stop()\n"));
	
    if (mSignal) {
        signal_deinit(mSignal);
		mSignal = NULL;
    }
    if (mLock) {
        lock_deinit(mLock);
		mLock = NULL;
    }
}

int BridgeTranslayer::recv(XBridgePacket *pkt, int timeout)
{	
    int ret;
    
    LOGI(("BridgeTranslayer::recv(%p, %d)\n", pkt, timeout));
    
    lock_get(mLock);
    do {
        ret = signal_wait(mSignal, mLock, timeout);
        if (ret != 0) {
            LOGE(("signal_wait() failed, ret=%d\n", ret));
            lock_put(mLock);
            
            return ret;
        }
    } while (mResponsePacket.isEmpty());

    pkt->copy(&mResponsePacket);
    mResponsePacket.clear();
    lock_put(mLock);
    
    return 0;
}

int BridgeTranslayer::send(XBridgePacket *pkt)
{
    uint8_t sbuf[XBRIDGE_MAX_PSIZE + 4]/*2B("XB") + 1B(plen) + 1B(checkSum)*/, size=0;
    
    LOGI(("BridgeTranslayer::send(%p)\n", pkt));
    
    /*!< start bytes */
    sprintf((char *)sbuf, "XB");
    size += 2;
    
    /*!< psize */
    sbuf[size] = pkt->psize();
    size += 1;
    
    /*!< pdata */
    memcpy(&sbuf[size], pkt->pbuf(), pkt->psize());
    size += pkt->psize();
    
    /*!< checksum */
    uint8_t checksum, i;
    for (checksum=0, i=0; i<size; i++) {
        checksum += sbuf[i];
    }
    sbuf[size] = checksum;
    size += 1;
    
    LOGV(("Send:"));
    for (uint8_t i=2; i<size; i++) {
        LOGV(("%2x", sbuf[i]));
        if (i < size-1) {
            LOGV((","));
        }
    }
    LOGV(("\n"));
    
    return sendBytes(sbuf, size);
}

int BridgeTranslayer::fetchPacket(XBridgePacket *pkt)
{
#define TIMEOUT		(400)	// 400ms

    char startChar=0;
    int rsize;
    uint8_t psize;
    uint8_t *pbuf = pkt->pbuf();
    uint8_t checkSum;
    unsigned long timeout;
    
    /*!< First check if there has any data */
    rsize = available();
    if (rsize == -1) {
        goto breakout;
    } else if (rsize == 0) {
        return 0;
    }
    
    /*!< First check if received minimum data enough to parse. */
    /*!< 8Bytes: 2B("XB") + 1B(PSize) + 1B(SNo) + 1B(Action) + 1B(Key1) + 1B(Key2) + 0B(Data) + 1B(CheckSum) */
    rsize = available();
    if (rsize < 8) {
        timeout = 0ul;
        do {
            sleep(100);
            timeout += 100;
            if (timeout > TIMEOUT) { // timeout.
                LOGV(("rsize=%d, rsize < 8 check timeout\n", rsize));
                goto cleanup;
            }
            rsize = available();
            if (rsize == -1) {
                LOGV(("failed to wait rsize\n"));
                goto breakout;
            }
        } while (rsize < 8);
    }
    
    /*!< Get start bytes of "XB" */
    if (recvByte((uint8_t*)&startChar) != 0) {
        LOGV(("failed to get 'X'\n"));
        goto breakout;
    }
    if (startChar != 'X') {
        LOGV(("1st startchar is not 'X', is %2x\n", startChar));
        goto cleanup;
    }
    if (recvByte((uint8_t*)&startChar) != 0) {
        LOGV(("failed to get 'B'\n"));
        goto breakout;
    }
    if (startChar != 'B') {
        LOGV(("2nd startchar is not 'B', is %2x\n", startChar));
        goto cleanup;
    }
    
    rsize -= 2;
    
    /*!< Get psize */
    if (recvByte(&psize) != 0) {
        LOGV(("failed to get psize\n"));
        goto breakout;
    }
    LOGV(("psize=%d\n", psize));
    
    // 1B(SNo) + 1B(Action) + 1B(Key1) + 1B(Key2) + 0B(Data)
    if ((psize < 4) || (psize > XBRIDGE_MAX_PSIZE)) {
        LOGV(("psize error\n"));
        goto cleanup;
    }
    rsize -= 1;
    
    /*!< read SNo, Action, Key1, Key2 */
    
    if ((recvByte(&pbuf[0]) != 0) || // SNo
        (recvByte(&pbuf[1]) != 0) || // Action
        (recvByte(&pbuf[2]) != 0) || // Key1
        (recvByte(&pbuf[3]) != 0)) { // Key2
        LOGV(("failed to get SNo, Action, Key1, Key2\n"));
        goto breakout;
    }
    LOGV(("SNo:%2x, Action:%2x, Key1:%2x, Key2:%2x\n", pbuf[0], pbuf[1], pbuf[2], pbuf[3]));
    rsize -= 4;
    psize -= 4;
    
    /*!< wait and receive packet "data" */
    if (rsize < psize + 1/*checkSum*/) {
        timeout = 0;
        do {
            sleep(100);
            timeout += 100;
            if (timeout > 600) { // timeout.
                LOGV(("rsize=%d, rsize < (psize + 1) check timeout\n", rsize));
                goto cleanup;
            }
            
            rsize = available();
            if (rsize == -1) {
                LOGV(("failed to wait pdata\n"));
                goto breakout;
            }
        } while (rsize < psize + 1/*checkSum*/);
    }
    if (psize > 0) {
        if (recvBytes(&pbuf[4], psize) != 0) {
            LOGV(("failed to get pdata\n"));
            goto breakout;
        }
        LOGV(("pdata:"));
        for (rsize = 0; rsize < psize; rsize++) {
            LOGV(("%2x,", pbuf[4+rsize]));
        }
        LOGV(("\n"));
    }
    
    /*!< CheckSum */
    
    if (recvByte(&checkSum) != 0) {
        LOGV(("failed to get checksum\n"));
        goto breakout;
    }
    LOGV(("checkSum=%2x\n", checkSum));
    
    uint8_t i, l;
    l = 'X' + 'B' + (psize + 4);
    for (i=0; i<(psize+4); i++) {
        l += pbuf[i];
    }
    if (checkSum != l) {
        LOGE(("checkSum failed %2x, should be %2x\n", l, checkSum));
        goto cleanup;
    }
    
    /*!< complete */
    pkt->set(psize); // dsize, except "SNo, Action, Key1, Key2" 4 bytes
    return 1;
cleanup:
    flush();
    return 0;
breakout:
    return -1;
}

void BridgeTranslayer::notifyAvailable()
{
    int ret;
    XBridgePacket pkt;
    uint8_t action;

    ret = fetchPacket(&pkt);
    if (ret < 0) {
        LOGE(("failed to fetchPacket, ret=%d\n", ret));
        return;
    } else if (ret == 0) {
        //LOGV(("fetchPacket timeout\n"));
        return;
    }
    LOGV(("fetchPacket, SNo:%2x, Action:%2x, Key1:%2x, Key2:%2x\n", pkt.sno(), pkt.action(), pkt.key1(), pkt.key2()));

    action = pkt.action();
    if (action >= XBRIDGE_ACTION_RESPONSE)
    {
        lock_get(mLock);
        mResponsePacket.copy(&pkt);
        signal_wakeup(mSignal);
        lock_put(mLock);
    }
    else if ((action == XBRIDGE_ACTION_BLOCK_NOTIFY) ||
             (action == XBRIDGE_ACTION_SERVER_NOTIFY))
    {
        mNotifyPacket.copy(&pkt);
        if (mTransListener)
        {
            mTransListener->notifyHandler(&mNotifyPacket);
            mNotifyPacket.clear();
        }
    }
}


XPBridge::XPBridge() :
	mTrans(NULL),
	mSNo(0),
	mStatus(0)
{
    LOGI(("XPBridge::XPBridge()\n"));
}

XPBridge::~XPBridge()
{
    LOGI(("XPBridge::~XPBridge()\n"));
    
    if (mTrans) {
        reset();
        //delete mTrans;
        mTrans = NULL;
    }
}

int XPBridge::setup(BridgeTranslayer *trans)
{
    LOGI(("XPBridge::setup(%p)\n", trans));

    if (!trans) {
        LOGE(("Invalid parameter of 'trans'\n"));
        return -1;
    }

    mTrans = trans;
    if (mTrans->setup(this) != 0) {
        LOGE(("trans->setup() failed\n"));
        mTrans = NULL;
        return -1;
    }
    
    reset();
    return 0;
}

void XPBridge::release()
{
    if (mTrans) {
		if (isConnected()) {
			disconnect();
		}
        mTrans->release();
    }

	reset();
}

void XPBridge::reset()
{
    LOGI(("XPBridge::reset()\n"));
    
    mSNo = 0;
    mStatus = 0;
    mBlockList.clear();
    mServerList.clear();
    mRPkt.clear();
    mSPkt.clear();
}

XPBlock * XPBridge::getBlockByUid(uint8_t uid)
{
    XPBlock *blk;
    
    List::Node *node = mBlockList.begin();
    while (node) {
        blk = (XPBlock *)node->obj();
        if (blk->uid() == uid) {
            return blk;
        }
        node = node->next();
    }
    
    return NULL;
}

XPBlock * XPBridge::getBlockByPos(uint8_t pos)
{
    XPBlock *blk;
    
    List::Node *node = mBlockList.begin();
    while (node) {
        if (pos == 0) {
            blk = (XPBlock *)node->obj();
            return blk;
        }
        node = node->next();
        pos--;
    }
    
    return NULL;
}

XPServer * XPBridge::getServerByUid(uint8_t uid)
{
    XPServer *svr;
    
    List::Node *node = mServerList.begin();
    while (node) {
        svr = (XPServer *)node->obj();
        if (svr->uid() == uid) {
            return svr;
        }
        node = node->next();
    }
    
    return NULL;
}

XPServer * XPBridge::getServerByPos(uint8_t pos)
{
    XPServer *svr;
    
    List::Node *node = mServerList.begin();
    while (node) {
        if (pos == 0) {
            svr = (XPServer *)node->obj();
            return svr;
        }
        node = node->next();
        pos--;
    }
    
    return NULL;
}

int XPBridge::transceive(int timeout, bool noResponse)
{
    int retry = 0;//默认值是1
    
    LOGI(("XPBridge::transceive(%d, %d)\n", timeout, noResponse));
    
    mRPkt.clear();  // first clear to receive
    
    do {
        mTrans->send(&mSPkt);
		if (noResponse) {
			goto Quit;
		}

        if (mTrans->recv(&mRPkt, (timeout == 0) ? 1000 : timeout) < 0) {
            LOGE(("transceive has not responsed, retry=%d\n", retry));
            retry--;
        } else {
            break;
        }
    } while (retry >= 0);
    
    if (retry < 0) {
        LOGE(("transceive has not responsed\n"));
        return -1;
    }
    
    if ((mRPkt.key2() != 0) ||
        (mRPkt.action() != (mSPkt.action() + XBRIDGE_ACTION_RESPONSE))/* ||
        (mRPkt.key1() != mSPkt.sno())*/)
    {
        LOGE(("response packet failed, key2:%d, action:%2x, key1:%2x\n", mRPkt.key2(), mRPkt.action(), mRPkt.key1()));
        return -1;
    }

Quit:
    mSPkt.clear();  /*!< clear mSPkt */
    return 0;
}

// const char * XPBridge::queryDeviceName()
// {
//     LOGI(("XPBridge::queryDeviceName()\n"));
    
//     mSPkt.set(genSNo(), XBRIDGE_ACTION_DEVICE_QUERY, 0x01, 0, 0);
//     if (transceive() != 0) {
//         LOGE(("transceive XBRIDGE_ACTION_DEVICE_QUERY (device name) failed\n"));
//         return NULL;
//     }
//     if (strlen((const char *)mRPkt.dbuf()) > XBRIDGE_DEVNAME_MAX_SIZE) {
//         LOGE(("invalid device name\n"));
//         return NULL;
//     }
    
//     LOGV(("device name: %s\n", (const char *)mRPkt.dbuf()));
//     return (const char *)mRPkt.dbuf();
// }

// int XPBridge::queryDeviceSN(uint8_t sn[4])
// {
//     uint32_t l;
    
//     LOGI(("XPBridge::queryDeviceSN()\n"));
    
//     mSPkt.set(genSNo(), XBRIDGE_ACTION_DEVICE_QUERY, 0x02, 0, 0);
//     if (transceive() != 0) {
//         LOGE(("transceive XBRIDGE_ACTION_DEVICE_QUERY (device sn) failed\n"));
//         return -1;
//     }
    
//     fetchU32((const uint8_t *)mRPkt.dbuf(), &l);
//     LOGV(("sn: %08x\n", l));
//     sn[0] = (l >> 24) & 0xFF;
//     sn[1] = (l >> 16) & 0xFF;
//     sn[2] = (l >> 8) & 0xFF;
//     sn[3] = l & 0xFF;
//     return 0;
// }

// const char * XPBridge::queryDeviceBoard()
// {
//     LOGI(("XPBridge::queryDeviceBoard()\n"));
    
//     mSPkt.set(genSNo(), XBRIDGE_ACTION_DEVICE_QUERY, 0x03, 0, 0);
//     if (transceive() != 0) {
//         LOGE(("transceive XBRIDGE_ACTION_DEVICE_QUERY (device board) failed\n"));
//         return NULL;
//     }
//     if (strlen((const char *)mRPkt.dbuf()) > XBRIDGE_BOARD_MAX_SIZE) {
//         LOGE(("invalid device board\n"));
//         return NULL;
//     }
    
//     LOGV(("device board: %s\n", (const char *)mRPkt.dbuf()));
//     return (const char *)mRPkt.dbuf();
// }

int XPBridge::queryDeviceFirmwareVersion(uint8_t *major, uint8_t *minor, uint8_t *patch)
{   
    const uint8_t *buf=NULL;
    
    LOGI(("XPBridge::queryDeviceFrimwareVersion()\n"));
    
    mSPkt.set(genSNo(), XBRIDGE_ACTION_DEVICE_QUERY, 0x05, 0, 0);
    if (transceive() != 0) {
        LOGE(("transceive XBRIDGE_ACTION_DEVICE_QUERY (device firmware version) failed\n"));
        return -1;
    }
    
    buf = (const uint8_t *)mRPkt.dbuf();
    buf = fetchU8(buf, major);
    buf = fetchU8(buf, minor);
    buf = fetchU8(buf, patch);
    LOGV(("firmware version: %d.%d.%d\n", *major, *minor, *patch));
    return 0;
}

int XPBridge::queryDeviceBID(uint8_t BID[5])
{   
    const uint8_t *buf=NULL;
    
    LOGI(("XPBridge::queryDeviceBID()\n"));
    
    mSPkt.set(genSNo(), XBRIDGE_ACTION_DEVICE_QUERY, 0x04, 0, 0);
    if (transceive() != 0) {
        LOGE(("transceive XBRIDGE_ACTION_DEVICE_QUERY (device BID) failed\n"));
        return -1;
    }
    
    buf = (const uint8_t *)mRPkt.dbuf();
    buf = fetchU8(buf, &BID[0]);
    buf = fetchU8(buf, &BID[1]);
    buf = fetchU8(buf, &BID[2]);
    buf = fetchU8(buf, &BID[3]);
    buf = fetchU8(buf, &BID[4]);
    LOGV(("BID[0~4]: %02d,%02d,%02d,%02d,%02d\n", BID[0], BID[1], BID[2], BID[3], BID[4]));
    return 0;
}

int XPBridge::handshake()
{
    LOGI(("XPBridge::handshake()\n"));
    
    mSPkt.set(genSNo(), XBRIDGE_ACTION_DEVICE_SETUP, 0x01, 0, 0);
    if (transceive() != 0) {
        LOGE(("transceive XBRIDGE_ACTION_DEVICE_SETUP (handshake) failed\n"));
        return -1;
    }
    LOGV(("handshake OK\n"));
    return 0;
}

int XPBridge::keepAlive(uint8_t timeout)
{
    LOGI(("XPBridge::keepAlive(%d)\n", timeout));
    
    mSPkt.set(genSNo(), XBRIDGE_ACTION_DEVICE_SETUP, 0x02, timeout, 0);
    if (transceive() != 0) {
        LOGE(("transceive XBRIDGE_ACTION_DEVICE_SETUP (keep alive) failed\n"));
        return -1;
    }
    LOGV(("keepAlive OK\n"));
    return 0;
}

int XPBridge::connect()
{
    LOGI(("XPBridge::connect()\n"));
    
    mSPkt.set(genSNo(), XBRIDGE_ACTION_SESSION_SETUP, 0x01, 0, 0);
    if (transceive() != 0) {
        LOGE(("transceive XBRIDGE_ACTION_SESSION_SETUP (connect) failed\n"));
        return -1;
    }
    mStatus |= XBRIDGE_STATUS_CONNECTED;
    LOGV(("connect OK\n"));
    return 0;
}

int XPBridge::disconnect()
{
    LOGI(("XPBridge::disconnect()\n"));
    
    mSPkt.set(genSNo(), XBRIDGE_ACTION_SESSION_SETUP, 0x02, 0, 0);
    if (transceive() != 0) {
        LOGE(("transceive XBRIDGE_ACTION_SESSION_SETUP (disconnect) failed\n"));
        return -1;
    }
    reset();    /*!< Reset */
    LOGV(("disconnect OK\n"));
    return 0;
}

int XPBridge::constructOnboardBlock(XPBlock *blk, const char *label)
{
    uint8_t uid = 0;
    
    LOGI(("XPBridge::constructOnboardBlock(%p, %s)\n", blk, label));
    assert(blk);
    assert(label);
    
    mSPkt.set(genSNo(), XBRIDGE_ACTION_BLOCK_CONSTRUCT, blk->type(), 1, strlen(label)+1, (const uint8_t *)label);
    if (transceive() != 0) {
        LOGE(("transceive XBRIDGE_ACTION_BLOCK_CONSTRUCT (type:0x%2x, label:%s) failed\n", blk->type(), label));
        return -1;
    }
    uid = mRPkt.dbuf()[0];
    blk->setUID(uid);
    
    List::Node *node = new List::Node(&mBlockList, (void *)blk);
    mBlockList.pushBack(node);
    
    LOGV(("uid: %d\n", uid));
    return 0;
}

int XPBridge::constructExternalBlock(XPBlock *blk, const char *model, const char *port)
{
    uint8_t uid = 0;
    char *dbuf = (char *)mSPkt.dbuf();
    
    LOGI(("XPBridge::constructExternalBlock(%p, %s, %s)\n", blk, model, port));
    assert(blk);
    assert(model);
    assert(port);
    
    memcpy(dbuf, model, strlen(model));
    dbuf += strlen(model) + 1;
    memcpy(dbuf, port, strlen(port));
    mSPkt.set(genSNo(), XBRIDGE_ACTION_BLOCK_CONSTRUCT, blk->type(), 2, strlen(model) + strlen(port) + 2);
    if (transceive() != 0) {
        LOGE(("transceive XBRIDGE_ACTION_BLOCK_CONSTRUCT (type:0x%x, model:%s, port:%s) failed\n", blk->type(), model, port));
        return -1;
    }
    uid = mRPkt.dbuf()[0];
    blk->setUID(uid);
    
    List::Node *node = new List::Node(&mBlockList, (void *)blk);
    mBlockList.pushBack(node);
    
    LOGV(("uid: %d\n", uid));
    return 0;
}

int XPBridge::constructServer(XPServer *svr, const uint8_t *param, uint8_t psize)
{
    uint8_t uid;
    
    LOGI(("XPBridge::constructServer(%p, %p, %d)\n", svr, param, psize));
    assert(svr);
    
    mSPkt.set(genSNo(), XBRIDGE_ACTION_SERVER_CONSTRUCT, svr->type(), 0, psize, param);
    if (transceive() != 0) {
        LOGE(("transceive XBRIDGE_ACTION_SERVER_CONSTRUCT (type:0x%x) failed\n", svr->type()));
        return -1;
    }
    uid = mRPkt.dbuf()[0];
    svr->setUID(uid);
    
    List::Node *node = new List::Node(&mServerList, (void *)svr);
    mServerList.pushBack(node);
    
    LOGV(("uid: %d\n", uid));
    return 0;
}

int XPBridge::sendUserMessage(uint8_t msg)
{
    LOGI(("XPBridge::sendUserMessage(%d)\n", msg));
    
    mSPkt.set(genSNo(), XBRIDGE_ACTION_USER_MESSAGE, msg, 0, 0);
    if (transceive() != 0) {
        LOGE(("transceive XBRIDGE_ACTION_USER_MESSAGE failed\n"));
        return -1;
    }
    LOGV(("sendUserMessage OK\n"));
    return 0;
}

int XPBridge::accessBlock(uint8_t uid, uint8_t api, const uint8_t *param, uint8_t psize, uint8_t *result, uint8_t *rsize, int timeout, bool noResponse)
{
    LOGI(("XPBridge::accessBlock(%d, %d, %p, %d, %p, %p, %d, %d)\n", uid, api, param, psize, result, rsize, timeout, noResponse));
    
    mSPkt.set(genSNo(), XBRIDGE_ACTION_BLOCK_ACCESS, noResponse ? (uid | 0x80) : uid, api, psize, param);
    if (transceive((timeout == 0) ? 0 : (1000 + timeout), noResponse) != 0) {
        LOGE(("transceive XBRIDGE_ACTION_BLOCK_ACCESS (uid:%d, api:%d) failed\n", uid, api));
        return -1;
    }

    if (!noResponse && result && rsize && (mRPkt.dsize() <= *rsize)) {
        memcpy(result, mRPkt.dbuf(), mRPkt.dsize());
        *rsize = mRPkt.dsize();
    }

    LOGV(("accessBlock OK\n"));
    return 0;
}

int XPBridge::accessServer(uint8_t uid, uint8_t api, const uint8_t *param, uint8_t psize, uint8_t *result, uint8_t *rsize, int timeout, bool noResponse)
{
    LOGI(("XPBridge::accessServer(%d, %d, %p, %d, %p, %p, %d, %d)\n", uid, api, param, psize, result, rsize, timeout, noResponse));
    
    mSPkt.set(genSNo(), XBRIDGE_ACTION_SERVER_ACCESS, noResponse ? (uid | 0x80) : uid, api, psize, param);
    if (transceive((timeout == 0) ? 0 : (1000 + timeout), noResponse) != 0) {
        LOGE(("transceive XBRIDGE_ACTION_SERVER_ACCESS (uid:%d, api:%d) failed\n", uid, api));
        return -1;
    }
    
    if (!noResponse &&result && rsize && (mRPkt.dsize() <= *rsize)) {
        memcpy(result, mRPkt.dbuf(), mRPkt.dsize());
        *rsize = mRPkt.dsize();
    }
    LOGV(("accessServer OK\n"));
    return 0;
}

int XPBridge::registerBlockNotify(uint8_t uid, uint8_t evt, const uint8_t *param, uint8_t psize)
{
    LOGI(("XPBridge::registerBlockNotify(%d, %d, %p, %d)\n", uid, evt, param, psize));
    
    mSPkt.set(genSNo(), XBRIDGE_ACTION_BLOCK_NOTIFY_REGISTER, uid, evt, psize, param);
    if (transceive() != 0) {
        LOGE(("transceive XBRIDGE_ACTION_BLOCK_NOTIFY_REGISTER (uid:%d, evt:%d) failed\n", uid, evt));
        return -1;
    }
    LOGV(("registerBlockNotify OK\n"));
    return 0;
}

int XPBridge::registerServerNotify(uint8_t uid, uint8_t evt, const uint8_t *param, uint8_t psize)
{
    LOGI(("XPBridge::registerServerNotify(%d, %d, %p, %d)\n", uid, evt, param, psize));
    
    mSPkt.set(genSNo(), XBRIDGE_ACTION_SERVER_NOTIFY_REGISTER, uid, evt, psize, param);
    if (transceive() != 0) {
        LOGE(("transceive XBRIDGE_ACTION_SERVER_NOTIFY_REGISTER (uid:%d, evt:%d) failed\n", uid, evt));
        return -1;
    }
    
    LOGV(("registerServerNotify OK\n"));
    return 0;
}

void XPBridge::notifyHandler(XBridgePacket *pkt)
{
    LOGI(("XPBridge::notifyHandler(%p)\n", pkt));
    assert(pkt);
    
    LOGV(("action:0x%x, key1:%d, key2:%d\n", pkt->action(), pkt->key1(), pkt->key2()));
    
    if (pkt->action() == XBRIDGE_ACTION_BLOCK_NOTIFY) {
        XPBlock *blk = getBlockByUid(pkt->key1());
        if (blk) {
            blk->notifyHandler(pkt->key2(), pkt->dbuf(), pkt->dsize());
        }
    } else if (pkt->action() == XBRIDGE_ACTION_SERVER_NOTIFY) {
        XPServer *svr = getServerByUid(pkt->key1());
        if (svr) {
            svr->notifyHandler(pkt->key2(), pkt->dbuf(), pkt->dsize());
        }
    }
}
