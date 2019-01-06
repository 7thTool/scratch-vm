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


#ifndef __XPRIDGE_H__
#define __XPRIDGE_H__


#include "common.hpp"


/**
 *  Definition of block type.
 */
#define XBLOCK_TYPE_XButton       	(0x01)
#define XBLOCK_TYPE_XDualDCMotor	(0x02)
#define XBLOCK_TYPE_XUltrasonic    	(0x03)
#define XBLOCK_TYPE_XIRTracking    	(0x04)
#define XBLOCK_TYPE_XLightSensor   	(0x05)
#define XBLOCK_TYPE_XBuzzer        	(0x06)
#define XBLOCK_TYPE_XDHT           	(0x07)
#define XBLOCK_TYPE_XRGBLed        	(0x08)
#define XBLOCK_TYPE_XAcceGyro      	(0x09)
#define XBLOCK_TYPE_XSoundSensor   	(0x0A)
#define XBLOCK_TYPE_XPIR           	(0x0B)
#define XBLOCK_TYPE_XSegDisplay    	(0x0C)
#define XBLOCK_TYPE_XIRSender      	(0x0D)
#define XBLOCK_TYPE_XIRReceiver    	(0x0E)
#define XBLOCK_TYPE_XIPC    		(0x0F)
#define XBLOCK_TYPE_XServo			(0x10)


/**
 *
 */
uint8_t * fetchU8(const uint8_t *buf, uint8_t *val);
uint8_t * fetchU16(const uint8_t *buf, uint16_t *val);
uint8_t * fetchU32(const uint8_t *buf, uint32_t *val);
uint8_t * fetchString(const uint8_t *buf, char **s);
uint8_t * fetchFloat(const uint8_t *buf, float *val);

uint8_t * fillU8(uint8_t *buf, uint8_t val);
uint8_t * fillU16(uint8_t *buf, uint16_t val);
uint8_t * fillU32(uint8_t *buf, uint32_t val);
uint8_t * fillString(uint8_t *buf, const char *s);
uint8_t * fillFloat(uint8_t *buf, float val);

/**
 *  XBridge is a Global standalone object.
 */
class XPBridge;

extern XPBridge XBridge;


/**
 *  Base class of Block Proxy.
 */
class XPBlock {
public:
    XPBlock(uint8_t type) : mType(type), mUID(0) {}
    virtual ~XPBlock() {}
    
    /**
     * \brief  Construct external block.
     * \param  model,
     * \param  port,
     * \return 0 in succeed, else failed.
     */
    int setup(const char *model, const char *port);
    
    /**
     * \brief  Construct onboard block.
     * \param  label,
     * \return 0 in succeed, else failed.
     */
    int setup(const char *label);
    
    uint8_t type() const { return mType; }
    uint8_t uid() const { return mUID; }
    
    /**
     *  \brief  API access.
     */
    virtual int access(uint8_t api, const uint8_t *param, uint8_t psize, uint8_t *result, uint8_t *rsize, int timeout=0, bool noResponse=false);
    
    virtual int notifyRegister(uint8_t evt, const uint8_t *param, uint8_t psize);
    
    
    friend class XPBridge;
protected:
    void setUID(uint8_t uid) { mUID = uid; }
    virtual void notifyHandler(uint8_t evt, const uint8_t *param, uint8_t psize) {
        return;
    }
    
private:
    uint8_t mType;
    uint8_t	mUID;
};


class XPServer : public XPBlock {
public:
    XPServer(uint8_t type) : XPBlock(type) {}
    virtual ~XPServer() {}
    
    /**
     * \brief   Construct server.
     * \param   param,
     * \param   psize,
     * \return 0 in succeed, else failed.
     */
    int setup(const uint8_t *param, uint8_t psize);
    
    int access(uint8_t api, const uint8_t *param, uint8_t psize, uint8_t *result, uint8_t *rsize, int timeout=0, bool noResponse=false);
    int notifyRegister(uint8_t evt, const uint8_t *param, uint8_t psize);
};


/**
 *
 */
class List {
public:
    List() :
    mLength(0),
    mHead(this, NULL) {
        mHead.mPrev = mHead.mNext = &mHead;
    }
    ~List() { clear(); }
    
    class Node {
    public:
        Node(List *lst, void *obj) :
        mList(lst),
        mObj(obj) {
            mPrev = mNext = NULL;
        }
        ~Node() {}
        
        void * obj() {
            return mObj;
        }
        void remove() { mList->remove(this); }
        Node * prev() { return (mPrev == mList->head()) ? NULL : mPrev; }
        Node * next() { return (mNext == mList->head()) ? NULL : mNext; }
        
    protected:
        friend class List;
        Node    *mPrev;
        Node    *mNext;
        
    private:
        List    *mList;
        void    *mObj;
    };
    
    uint8_t length() { return mLength; }
    Node * head() { return &mHead; }
    Node * begin() { return (mLength == 0) ? NULL : mHead.mNext; }
    Node * end() { return (mLength == 0) ? NULL : mHead.mPrev; }
    
    void pushBack(Node *node) {
        node->mPrev = mHead.mPrev;
        node->mNext = &mHead;
        mHead.mPrev->mNext = node;
        mHead.mPrev = node;
        mLength++;
    }
    void pushFront(Node *node) {
        node->mPrev = &mHead;
        node->mNext = mHead.mNext;
        mHead.mNext->mPrev = node;
        mHead.mNext = node;
        mLength++;
    }
    void remove(Node *node) {
        if (mLength > 0) {
            node->mNext->mPrev = node->mPrev;
            node->mPrev->mNext = node->mNext;
            mLength--;
        }
    }
    void clear() {
        Node *node;
        while (mHead.mNext != &mHead) {
            node = mHead.mNext; // backup to delete.
            remove(mHead.mNext);
            delete node;
        }
    }
    
private:
    uint8_t mLength;
    Node    mHead;
};


/**
 *  XBridge Protocol Packet.
 */
#define XBRIDGE_ACTION_DEVICE_QUERY     		(0x01)  //!< Query device name, SN, mainboard type, BID, version
#define XBRIDGE_ACTION_DEVICE_SETUP   			(0x02)  //!< Setup device, key1: 0x01--handshake, 0x02--heartbeat
#define XBRIDGE_ACTION_SESSION_SETUP			(0x03)  //!< Setup session, key1: 0x01--start, 0x02--stop
#define XBRIDGE_ACTION_BLOCK_CONSTRUCT  		(0x10)  //!< Construct block, key1 is block type, key2: 0x01--onboard, 0x02--external
#define XBRIDGE_ACTION_BLOCK_ACCESS     		(0x12)  //!< Access block, key1 is uid, key2 is api.
#define XBRIDGE_ACTION_BLOCK_NOTIFY     		(0x1A)  //!< Notify block, key1 is uid, key2 is evt.
#define XBRIDGE_ACTION_BLOCK_NOTIFY_REGISTER	(0x1B)  //!< Block register notify, key1 is uid, key2 is evt.
#define XBRIDGE_ACTION_BLOCK_NOTIFY_UNREGISTER	(0x1C)  //!< Block unregister notify, key1 is uid, key2 is evt.
#define XBRIDGE_ACTION_SERVER_CONSTRUCT 		(0x20)  //!< Construct server, key1 is server type.
#define XBRIDGE_ACTION_SERVER_ACCESS    		(0x22)  //!< Access server, key1 is uid, key2 is api.
#define XBRIDGE_ACTION_SERVER_NOTIFY    		(0x2A)  //!< Notify server, key1 is uid, key2 is evt.
#define XBRIDGE_ACTION_SERVER_NOTIFY_REGISTER	(0x2B)  //!< Server register notify, key1 is uid, key2 is evt.
#define XBRIDGE_ACTION_SERVER_NOTIFY_UNREGISTER	(0x2C)  //!< Server unregister notify, key1 is uid, key2 is evt.
#define XBRIDGE_ACTION_USER_MESSAGE    			(0x40)  //!< User message, key1 is message code.
#define XBRIDGE_ACTION_RESPONSE         		(0x80)  //!< Actual is 0x80+ACTION, key1 is the action's SNo, key2 is status.

#define XBRIDGE_MAX_DSIZE	12
#define XBRIDGE_MAX_PSIZE	16

class XBridgePacket {
public:
#define OFFSET_SNO		0
#define OFFSET_ACT		1
#define OFFSET_KEY1		2
#define OFFSET_KEY2		3
#define OFFSET_DATA		4
    
    XBridgePacket() :
    mEmpty(1),
    mDSize(0) {
        memset((void *)mBuffer, 0, XBRIDGE_MAX_PSIZE);
    }
    ~XBridgePacket() {}
    
    /*!< Call set to load the packet */
    void set(uint8_t sno, uint8_t action,
             uint8_t key1, uint8_t key2, uint8_t dsize, const uint8_t *dbuf) {
        mBuffer[OFFSET_SNO] = sno;
        mBuffer[OFFSET_ACT] = action;
        mBuffer[OFFSET_KEY1] = key1;
        mBuffer[OFFSET_KEY2] = key2;
        mDSize = (dsize > XBRIDGE_MAX_DSIZE) ? XBRIDGE_MAX_DSIZE : dsize;
        memcpy(&mBuffer[OFFSET_DATA], dbuf, mDSize);
        mEmpty = 0;
    }
    void set(uint8_t sno, uint8_t action,
             uint8_t key1, uint8_t key2, uint8_t dsize) {
        mBuffer[OFFSET_SNO] = sno;
        mBuffer[OFFSET_ACT] = action;
        mBuffer[OFFSET_KEY1] = key1;
        mBuffer[OFFSET_KEY2] = key2;
        mDSize = (dsize > XBRIDGE_MAX_DSIZE) ? XBRIDGE_MAX_DSIZE : dsize;
        mEmpty = 0;
    }
    void set(uint8_t dsize) {
        mDSize = dsize;
        mEmpty = 0;
    }
    
    void copy(XBridgePacket *pkt) {
        memcpy(mBuffer, pkt->pbuf(), XBRIDGE_MAX_PSIZE);
        set(pkt->dsize());
    }
    
    /*!< Call clear to give back the packet */
    void clear() {
        mDSize = 0;
        mEmpty = 1;
        memset((void *)mBuffer, 0, XBRIDGE_MAX_PSIZE);
    }
    
    bool isEmpty() { return mEmpty ? true : false; }
    uint8_t sno() const { return mBuffer[OFFSET_SNO]; }
    uint8_t action() const { return mBuffer[OFFSET_ACT]; }
    uint8_t key1() const { return mBuffer[OFFSET_KEY1]; }
    uint8_t key2() const { return mBuffer[OFFSET_KEY2]; }
    uint8_t dlen() const { return XBRIDGE_MAX_DSIZE; }
    uint8_t * dbuf() const { return (uint8_t *)&mBuffer[OFFSET_DATA]; }
    uint8_t dsize() const { return mDSize; }
    uint8_t plen() const { return XBRIDGE_MAX_PSIZE; }
    uint8_t *pbuf() const { return (uint8_t *)mBuffer; }
    uint8_t psize() const { return mDSize + 4; }
    
private:
    uint8_t		mEmpty;
    uint8_t     mBuffer[XBRIDGE_MAX_PSIZE];
    uint8_t     mDSize;
};

class BridgeLinklayer;
//class BridgeLinklayer::LinkListener;
class BridgeTranslayer;
//class BridgeTranslayer::TransListener;

class BridgeLinklayer {
    friend class BridgeTranslayer;
public:
	// 应用响应链接消息
	class LinkListener {
	public:
		#define BTLINK_EVT_DEV_DISCOVER	    1	/*!< 扫描到设备 */
	
		virtual void notifyHandler(int evt, void *param) { }
	};

    BridgeLinklayer();
	virtual ~BridgeLinklayer();

	// 开始扫描
	//void startScan();
	// 停止扫描
	//void stopScan();
	// 建立连接
	int setup(BridgeLinklayer::LinkListener *listener, BridgeTranslayer* translayer);
	// 释放连接
	void release();

protected:
    // 清除输入输出缓存
	virtual void flush() = 0;
	// 返回接收到的数据长度
	virtual int available() = 0;
	// 读一个字节的数据，0表示成功
	virtual int recvByte(uint8_t *buf) = 0;
	// 读[size]个字节的数据，0表示成功
	virtual int recvBytes(uint8_t *buf, uint8_t size) = 0;
	// 写[size]个字节的数据，0表示成功
	virtual int sendBytes(uint8_t *buf, uint8_t size) = 0;
    
	BridgeLinklayer::LinkListener	*mLinkListener;
	BridgeTranslayer* mTranslayer;
};

class BridgeTranslayer {
     friend class BridgeLinklayer;
public:
	// 设备通知的回调接口
    class TransListener {
    public:
        virtual void notifyHandler(XBridgePacket *pkt) {}
    };

    BridgeTranslayer(BridgeLinklayer* linklayer);
    virtual ~BridgeTranslayer();
	
	// 初始化传输层。返回0表示成功，负数表示失败
	virtual int setup(BridgeTranslayer::TransListener *listener);
	// 释放传输层
    virtual void release();
    
    /**
     *  \brief  Receive [Response] packet.
     *  \param  pkt, pkt will to be fill.
     *  \param  timeout, unit is millisecond.
     *  \return -1--failed, -2--timeout, 0--succeed.
     */
    virtual int recv(XBridgePacket *pkt, int timeout);
    
    // 通知收到数据，连接层调用
    virtual void notifyAvailable();
    
    /**
     *  \brief  Send packet.
     *  \param  pkt, pkt want to be send.
     *  \return 0 in succeed, else failed.
     */
    virtual int send(XBridgePacket *pkt);
    
protected:
    int fetchPacket(XBridgePacket *pkt);

	// 清除输入输出缓存
	inline void flush() { mLinklayer->flush(); };
	// 返回接收到的数据长度
	inline int available() { return mLinklayer->available(); };
	// 读一个字节的数据，0表示成功
	inline int recvByte(uint8_t *buf) { return mLinklayer->recvByte(buf); };
	// 读[size]个字节的数据，0表示成功
	inline int recvBytes(uint8_t *buf, uint8_t size) { return mLinklayer->recvBytes(buf, size); };
	// 写[size]个字节的数据，0表示成功
	inline int sendBytes(uint8_t *buf, uint8_t size) { return mLinklayer->sendBytes(buf, size); };

	BridgeLinklayer* mLinklayer;
	BridgeTranslayer::TransListener * mTransListener;
    XBridgePacket   mResponsePacket;
    XBridgePacket   mNotifyPacket;
    lock_handler 	mLock;
    signal_handler	mSignal;
};


/**
 *  Bridge Proxy.
 */
#define XBRIDGE_DEVNAME_MAX_SIZE	(11)
#define XBRIDGE_BOARD_MAX_SIZE	    (11)

#define XBRIDGE_STATUS_CONNECTED	(0x01)

class XPBridge : public BridgeTranslayer::TransListener {
public:
    XPBridge();
    ~XPBridge();
    
    /**
     *  \brief  Setup XBridge Object.
     *  \return 0--succeed, else failed.
     */
    int setup(BridgeTranslayer *trans);
    void release();

    /*!< Check if session connected */
    bool isConnected() { return (mStatus & XBRIDGE_STATUS_CONNECTED) ? true : false; }
    
    /**
     *  \brief  Query device name.
     *  \param  None.
     *  \return Device name in succeed, else NULL.
     */
    //const char * queryDeviceName();
    
    /**
     *  \brief  Query device Serial Number.
     *  \param  None.
     *  \return 0 is failed, else return SN.
     */
    //int queryDeviceSN(uint8_t sn[4]);
    /**
     *  \brief  Query device mainboard type.
     *  \param  None.
     *  \return Mainboard type in succeed, else NULL.
     */
    //const char * queryDeviceBoard();
    
    /**
     *  \brief  Query device firmware version.
     *  \param
     *  \return 0 in succeed, else failed.
     */
    int queryDeviceFirmwareVersion(uint8_t *major, uint8_t *minor, uint8_t *patch);

    /**
     *  \brief  Query device BID.
     *  \param
     *  \return 0 in succeed, else failed.
     */
    int queryDeviceBID(uint8_t BID[5]);
	
    /**
     *  \brief  Handshake with device.
     *  \param  None.
     *  \return 0 in succeed, else failed.
     */
    int handshake();
    
    /**
     *  \brief  Keep alive with device.
     *  \param  timeout, timeout in second.
     *  \return 0 in succeed, else failed.
     */
    int keepAlive(uint8_t timeout);
    
    /**
     *  \brief  Connect a session with device.
     *  \param  None.
     *  \return 0 in succeed, else failed.
     */
    int connect();
    
    /**
     *  \brief  Disonnect a session with device.
     *  \param  None.
     *  \return 0 in succeed, else failed.
     */
    int disconnect();
    
    
    friend class XPBlock;
    friend class XPServer;
protected:
    /**
     *  \brief  Construct onboard block.
     *  \param  blk, block want to constructed.
     *  \param  label, label of the block.
     *  \return 0 in succeed, else failed.
     */
    int constructOnboardBlock(XPBlock *blk, const char *label);
    
    /**
     *  \brief  Construct external block.
     *  \param  blk, block want to constructed.
     *  \param  model, model of the block.
     *  \param  port, port that the block want connect to.
     *  \return 0 in succeed, else failed.
     */
    int constructExternalBlock(XPBlock *blk, const char *model, const char *port);
    
    /**
     *  \brief  Construct server.
     *  \param  svr, server want to constructed.
     *  \param  param,
     *  \param  psize,
     *  \return 0 in succeed, else failed.
     */
    int constructServer(XPServer *svr, const uint8_t *param, uint8_t psize);
    
    /**
     *  \brief Send user message.
     *  \param  msg, message to send.
     *  \return 0 in succeed, else failed.
     */
    int sendUserMessage(uint8_t msg);
    
    int accessBlock(uint8_t uid, uint8_t api, const uint8_t *param, uint8_t psize, uint8_t *result, uint8_t *rsize, int timeout=0, bool noResponse=false);
    int accessServer(uint8_t uid, uint8_t api, const uint8_t *param, uint8_t psize, uint8_t *result, uint8_t *rsize, int timeout=0, bool noResponse=false);
    int registerBlockNotify(uint8_t uid, uint8_t evt, const uint8_t *param, uint8_t psize);
    int registerServerNotify(uint8_t uid, uint8_t evt, const uint8_t *param, uint8_t psize);
    
    
private:
    void reset();
    uint8_t genSNo() {
        if (++mSNo == 0) {
            mSNo = 1;
        }
        return mSNo;
    }
    
    int transceive(int timeout=3000, bool noResponse=false);
    
    XPBlock * getBlockByUid(uint8_t uid);
    XPBlock * getBlockByPos(uint8_t pos);
    XPServer * getServerByUid(uint8_t uid);
    XPServer * getServerByPos(uint8_t pos);
    
    /*!< Interface of BridgeTransLayer::NotifyListener */
    void notifyHandler(XBridgePacket *pkt);
    
    BridgeTranslayer 	* mTrans;
    uint8_t             mSNo;
    uint8_t             mStatus;
    List                mBlockList;
    List                mServerList;
    XBridgePacket 		mRPkt;
    XBridgePacket		mSPkt;
};


#endif	// __XPBRIDGE_H__
