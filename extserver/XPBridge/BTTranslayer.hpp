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


#ifndef __BTTRANSLAYER_H__
#define __BTTRASNLAYER_H__


#include <XHelper.h>
#include "../com/ble.h"
#include "../com/tongle.h"
#include "XPBridge.hpp"


class BTLinkLayer : public com::tongle, public BridgeLinklayer {
	typedef com::tongle combase;
public:
	BTLinkLayer();
	~BTLinkLayer();

	// 建立连接 hqrover;A518161D;
	int setup(BridgeLinklayer::LinkListener *listener, BridgeTranslayer* translayer);
	// 停止连接
	void release();

	inline bool is_setup() { return combase::is_open(); }

	void on_discover(const std::string& dev);
	
protected:
	int wait(int timeout);
	int wait_for(std::string& data, int timeout);

	void on_write(boost::system::error_code ec, size_t len, const char* buf);
	void on_read(boost::system::error_code ec, size_t len, const char* buf);

private:
	void flush();
	int available();
	int recvByte(uint8_t *buf);
	int recvBytes(uint8_t *buf, uint8_t size);

	int recvBytesUntil(uint8_t *buf, uint8_t size, uint8_t spec);
	//int recvUntil(uint8_t *buf, uint8_t size, uint8_t* spec, uint8_t speclen);
	int recvAllBytes(uint8_t *buf, uint8_t size);
	
	int sendBytes(uint8_t *buf, uint8_t size);

	int mLastLinkPos;
	std::string mBuffer;
    lock_handler mLock;
    signal_handler mSignal;
};

#endif // __BTTRANSLAYER_H__
