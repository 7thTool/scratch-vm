#pragma once

#ifndef _H_TONGLE_H_
#define _H_TONGLE_H_

#include "serialport.h"

namespace com {

    static const char err[] = "+ERR";
    static const char ok[] = "+OK";
    static const char dev[] = "+DEV";
    static const char scan_start[] = "AT+SCAN1";
    static const char scan_stop[] = "+SCAN:STOP";
    static const char disc[] = "AT+DISC";

class tongle : public serialport
{
	typedef serialport base;
    
public:
    tongle():connected_(false)
    {

    }

	bool open(const std::string& com)
	{
		if(base::open(com)) {
			com_ = com;
        	base::set_baudrate(115200);
        	base::async_read();
			/*discover();
            if(!devs_.empty()) {
			    return connect(devs_.front());
            }*/
            return true;
		}
		return false;
	}
    
	void close()
	{
        disconnect();
        com_.clear();
        devs_.clear();
        base::close();
    }

	const std::string& com() { return com_; }

	const std::vector<std::string>& devs() { return devs_; }

    bool connected() { return connected_; }

    virtual void on_discover(const std::string& dev) { }

	void discover()
	{
        base::async_write(scan_start, sizeof(scan_start)-1);
        do {
			std::string data;
            if(0 != wait_for(data, 30000)) {
                break;
            }
            char mac[64] = {0};
            int maclen = -1;
			char* buf = 0;
        	int buflen = 0;
            buf = (char *)data.data();
            buflen = (int)data.size();
            if (buflen >= sizeof(err) && memcmp(buf, err, sizeof(err) - 1) == 0)
            {
                set_last_error_message(std::string(buf, buflen));
                break;
            }
            else if (buflen >= sizeof(ok) && memcmp(buf, ok, sizeof(ok) - 1) == 0)
            {
                continue;
            }
            else if (buflen >= sizeof(scan_stop) && memcmp(buf, scan_stop, sizeof(scan_stop) - 1) == 0)
            {
                break;
            }
            else if (buflen > sizeof(dev) && memcmp(buf, dev, sizeof(dev) - 1) == 0)
            {
                int i = 0, j = 0;
                for (i = 0, j = buflen; i < j; i++)
                {
                    char ch = buf[i];
                    if (ch == '=')
                    {
                        maclen = 0;
                    }
                    else if (ch == '\r' || ch == '\n')
                    {
                        if (maclen >= 0)
                        {
                            break;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else if (maclen >= 0)
                    {
                        mac[maclen++] = ch;
                    }
                }
            }
            if (maclen > 0)
            {
                devs_.emplace_back(mac, maclen);
                on_discover(devs_.back());
            }
        } while(true);
	}

    bool connect(const std::string& dev)
    {
        char connect[128] = {0};
        int connectlen = sprintf(connect, "AT+CONNET%s", dev.c_str());
        base::async_write(connect, connectlen);
        do
        {
			std::string data;
            if (0 != wait_for(data, 30000)) {
                break;
            }
			char* buf = 0;
        	int buflen = 0;
            buf = (char *)data.data();
            buflen = (int)data.size();
            if (buflen >= sizeof(err) && memcmp(buf, err, sizeof(err) - 1) == 0)
            {
                set_last_error_message(std::string(buf, buflen));
                break;
            }
            else if (buflen >= sizeof(ok) && memcmp(buf, ok, sizeof(ok) - 1) == 0)
            {
                connected_ = true;
                return connected_;
            }
            else
            {
                continue;
            }
        } while (true);
        return false;
    }

    void disconnect()
    {
        if(!connected_) {
            return;
        }
        connected_ = false;
        base::async_write(disc, sizeof(disc) - 1);
        do
        {
			std::string data;
            if (0 != wait_for(data, 3000)) {
                break;
            }
			char* buf = 0;
        	int buflen = 0;
            buf = (char *)data.data();
            buflen = (int)data.size();
            if (buflen >= sizeof(err) && memcmp(buf, err, sizeof(err) - 1) == 0)
            {
                set_last_error_message(std::string(buf, buflen));
                break;
            }
            else if (buflen >= sizeof(ok) && memcmp(buf, ok, sizeof(ok) - 1) == 0)
            {
                break;
            }
            else
            {
                continue;
            }
        } while (true);
    }

protected:
	virtual int wait(int timeout) = 0;
	virtual int wait_for(std::string& data, int timeout) = 0;

	std::string com_;
	std::vector<std::string> devs_;
    bool connected_;
};

}

#endif//
