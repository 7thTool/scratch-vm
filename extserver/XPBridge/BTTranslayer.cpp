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


#include "XPBridge.hpp"
#include "BTTranslayer.hpp"


#if 1
#define LOGV(format, ...) printf(format, ##__VA_ARGS__)
#define LOGI(format, ...) printf(format, ##__VA_ARGS__)
#define LOGW(format, ...) printf(format, ##__VA_ARGS__)
#define LOGE(format, ...) printf(format, ##__VA_ARGS__)
#else
#define LOGV(format, ...)
#define LOGI(format, ...)
#define LOGW(format, ...)
#define LOGE(format, ...)
#endif


BTLinkLayer::BTLinkLayer()
: mLastLinkPos(0)
, mLock(NULL)
, mSignal(NULL)
{
    mSignal = signal_init();
    if (!mSignal) {
        LOGE(("signal_init() failed\n"));
    }

    mLock = lock_init();
    if (!mLock) {
        LOGE(("lock_init() failed\n"));
    }
}

BTLinkLayer::~BTLinkLayer()
{
	release();
    
    if (mSignal) {
        signal_deinit(mSignal);
		mSignal = NULL;
    }
    if (mLock) {
        lock_deinit(mLock);
		mLock = NULL;
    }
}

int BTLinkLayer::setup(BridgeLinklayer::LinkListener *listener, BridgeTranslayer* translayer)
{
    int ret = BridgeLinklayer::setup(listener,translayer);
    mTranslayer = translayer;
    std::vector<ListResultItem> com_list = get_com_list(true);
    for(size_t i = mLastLinkPos, j = com_list.size() + mLastLinkPos; i < j; i++)
    {
        i %= com_list.size();
        mLastLinkPos = i; //记住最后一次的com
        if(com_list[i].manufacturer == "Prolific")
        {
            if(combase::open(com_list[i].comName)) 
            {
                return 0;
            }
            break;
        }
    }
    return -1;
}

void BTLinkLayer::release()
{
    flush();
    combase::close();
    flush();
    BridgeLinklayer::release();
}

void BTLinkLayer::on_discover(const std::string& dev)
{
    mLinkListener->notifyHandler(BTLINK_EVT_DEV_DISCOVER, (void*)dev.c_str());
}

int BTLinkLayer::wait(int timeout)
{
    int ret = 0;
        lock_get(mLock);
        ret = signal_wait(mSignal, mLock, timeout);
        if (ret != 0) {
            lock_put(mLock);
            return ret;
        }
        lock_put(mLock);
        return 0;
}

int BTLinkLayer::wait_for(std::string& data, int timeout)
{
    int ret = 0;
        lock_get(mLock);
        ret = signal_wait(mSignal, mLock, timeout);
        if (ret != 0) {
            lock_put(mLock);
            return ret;
        }
        data = mBuffer;
        mBuffer.clear();
        lock_put(mLock);
        return 0;
}

void BTLinkLayer::on_write(boost::system::error_code ec, size_t len, const char* buf)
{
    //
}

void BTLinkLayer::on_read(boost::system::error_code ec, size_t len, const char* buf)
{
    if(ec) {
        LOGI("%d:%s\n", ec.value(), ec.message().c_str());
        return;
    }

    lock_get(mLock);
    mBuffer.append(buf, len);
    lock_put(mLock);

    //保存数据
    if(!connected())
    {
        //设备连接
        LOGI("%.*s", len, buf);
        signal_wakeup(mSignal);
    }
    else 
    {
        //通知传输层数据到达
        mTranslayer->notifyAvailable();
    }
}

void BTLinkLayer::flush()
{
	lock_get(mLock);
    mBuffer.clear();
    lock_put(mLock);
}

int BTLinkLayer::available()
{
    return mBuffer.size();
}

int BTLinkLayer::recvByte(uint8_t *buf)
{
    lock_get(mLock);
    if(buf && mBuffer.size() > 0)
    {
        *buf = mBuffer[0];
        mBuffer.erase(mBuffer.begin());
        lock_put(mLock);
        return 0;
    }
    lock_put(mLock);
    return -1;
}

int BTLinkLayer::recvBytes(uint8_t *buf, uint8_t size)
{
    lock_get(mLock);
    if(buf && mBuffer.size() >= size)
    {
        memcpy(buf, mBuffer.data(), size);
        mBuffer.erase(mBuffer.begin(), mBuffer.begin() + size);
        lock_put(mLock);
        return 0;
    }
    lock_put(mLock);
    return -1;
}

int BTLinkLayer::recvBytesUntil(uint8_t *buf, uint8_t size, uint8_t spec)
{
    lock_get(mLock);
    if(buf && mBuffer.size() > 0)
    {
        uint8_t *src = (uint8_t *)mBuffer.data();
        int srclen = mBuffer.size();
        for (int i = 0; i < size && i < srclen; i++)
        {
            if (src[i] == spec) {
                memcpy(buf, src, i);
                mBuffer.erase(mBuffer.begin(), mBuffer.begin() + i);
                lock_put(mLock);
                return i;
                break;
            }
        }
    }
    lock_put(mLock);
    return 0;
}
/*
int BTLinkLayer::recvUntil(uint8_t *buf, uint8_t size, uint8_t* spec, uint8_t speclen)
{
    lock_get(mLock);
    if(buf && mBuffer.size() >= speclen)
    {
        uint8_t *src = (uint8_t *)mBuffer.data();
        int srclen = mBuffer.size();
        if (srclen == dstlen) {
            return (memcmp(src, dst, dstlen) == 0 ? (char*)src : 0);
        }
        else {
            for (int i = 0, j = srclen - dstlen; i <= j; i++)
            {
                if (memcmp(src + i, dst, dstlen) == 0) {
                    return (src + i);
                }
            }
            return (0);
        }
        return 0;
        lock_put(mLock);
        return 0;
    }
    lock_put(mLock);
    return -1;
}
*/
int BTLinkLayer::recvAllBytes(uint8_t *buf, uint8_t size)
{
    lock_get(mLock);
    if(buf && mBuffer.size() <= size)
    {
        uint8_t* src = (uint8_t*)mBuffer.data();
        uint8_t srclen = mBuffer.size();
        memcpy(buf, src, srclen);
        mBuffer.clear();
        lock_put(mLock);
        return srclen;
    }
    lock_put(mLock);
    return 0;
}

int BTLinkLayer::sendBytes(uint8_t *buf, uint8_t size)
{
	async_write((const char*)buf, size);
    return 0;
}
