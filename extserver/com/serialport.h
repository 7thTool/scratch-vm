#pragma once

#ifndef _H_SERIALPORT_H_
#define _H_SERIALPORT_H_

#include <list>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <codecvt>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <boost/locale.hpp>
#include <boost/timer.hpp>
#include <boost/random.hpp>
#include <boost/any.hpp>
#include <boost/assign.hpp>
#include <boost/asio/serial_port.hpp>

#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/creation_tags.hpp>
#include <boost/interprocess/shared_memory_object.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#ifdef _WINDOWS
#include <windows.h>
#include <tchar.h>
#include <Setupapi.h>
#include <devguid.h>
#pragma comment(lib, "setupapi.lib")
#endif

//#include <openssl/md5.h>
//#include <openssl/sha.h>
//#include <openssl/des.h>
//#include <openssl/rsa.h>
//#include <openssl/pem.h>
//#include <boost/asio/ssl.hpp>

namespace com {

class serialport
{
private:
	static std::string copySubstring(char *someString, int n)
	{
		return std::string(someString, n);
	}
public:
	struct ListResultItem {
		std::string comName;
		std::string manufacturer;
		std::string serialNumber;
		std::string pnpId;
		std::string locationId;
		std::string vendorId;
		std::string productId;
	};

	static std::vector<ListResultItem> enum_serialport()
	{
		std::vector<ListResultItem> comlist;
#ifdef _WINDOWS
		GUID *guidDev = (GUID *)&GUID_DEVCLASS_PORTS;
		HDEVINFO hDevInfo = SetupDiGetClassDevs(guidDev, NULL, NULL, DIGCF_PRESENT | DIGCF_PROFILE);
		if (hDevInfo)
		{
			int memberIndex = 0;
			while (true)
			{

				SP_DEVINFO_DATA deviceInfoData;
				DWORD dwSize = 0, dwPropertyRegDataType = 0;
				TCHAR szBuffer[400] = {0};
				TCHAR *p = nullptr;
				bool isCom = false;

				std::string pnpId;
				std::string vendorId;
				std::string productId;
				std::string name;
				std::string manufacturer;
				std::string locationId;

				ZeroMemory(&deviceInfoData, sizeof(SP_DEVINFO_DATA));
				deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

				if (SetupDiEnumDeviceInfo(hDevInfo, memberIndex, &deviceInfoData) == FALSE)
				{
					if (GetLastError() == ERROR_NO_MORE_ITEMS)
					{
						break;
					}
				}

				dwSize = sizeof(szBuffer);
				SetupDiGetDeviceInstanceId(hDevInfo, &deviceInfoData, szBuffer, dwSize, &dwSize);
				szBuffer[dwSize] = _T('\0');
				pnpId = szBuffer;

				p = _tcsstr(szBuffer, _T("VID_"));
				if (p)
				{
					p += 4;
					vendorId = copySubstring(p, 4);
				}
				p = _tcsstr(szBuffer, _T("PID_"));
				if (p)
				{
					p += 4;
					productId = copySubstring(p, 4);
				}

				if (SetupDiGetDeviceRegistryProperty(hDevInfo, &deviceInfoData, SPDRP_LOCATION_INFORMATION, &dwPropertyRegDataType, (BYTE *)szBuffer, sizeof(szBuffer), &dwSize))
				{
					locationId = szBuffer;
				}
				if (SetupDiGetDeviceRegistryProperty(hDevInfo, &deviceInfoData, SPDRP_MFG, &dwPropertyRegDataType, (BYTE *)szBuffer, sizeof(szBuffer), &dwSize))
				{
					manufacturer = szBuffer;
				}

				HKEY hkey = SetupDiOpenDevRegKey(hDevInfo, &deviceInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);
				if (hkey != INVALID_HANDLE_VALUE)
				{
					dwSize = sizeof(szBuffer);
					if (RegQueryValueEx(hkey, _T("PortName"), NULL, NULL, (LPBYTE)&szBuffer, &dwSize) == ERROR_SUCCESS)
					{
						szBuffer[dwSize] = _T('\0');
						name = szBuffer;
						isCom = _tcsstr(szBuffer, _T("COM")) != NULL;
					}
				}
				if (isCom)
				{
					ListResultItem resultItem;
					resultItem.comName = name;
					resultItem.manufacturer = manufacturer;
					resultItem.pnpId = pnpId;
					resultItem.vendorId = vendorId;
					resultItem.productId = productId;
					resultItem.locationId = locationId;
					comlist.push_back(resultItem);
				}

				RegCloseKey(hkey);
				memberIndex++;
			}
			SetupDiDestroyDeviceInfoList(hDevInfo);
		}
#endif//
		return comlist;
	}
private:
	std::vector<ListResultItem> com_list_;
	std::string last_error_;
	boost::asio::io_service service_;
	boost::asio::io_service::work service_work_;
	std::shared_ptr<std::thread> thread_;
	boost::atomics::atomic_bool close_flag_;
	boost::asio::serial_port sp_;
	std::vector<std::string> write_buffer_[2];
	boost::atomics::atomic_bool write_complete_;
	std::array<char, 1024> read_buffer_;
	boost::atomics::atomic_bool read_complete_;
	boost::recursive_mutex mutex_;
	//std::condition_variable cv_;
  public:
	serialport() : com_list_(), sp_(service_), service_work_(service_), close_flag_(true), write_complete_(true), read_complete_(true)
	{
	}

	~serialport()
	{
	}

	void init()
	{
		update_serialport();
	}

	void term()
	{
	}

	void update_serialport()
	{
		boost::any comlist = enum_serialport();
		if (!comlist.empty())
		{
			com_list_ = boost::any_cast<std::vector<ListResultItem>>(comlist);
		}
	}

	const std::vector<ListResultItem> &get_com_list(bool update = false)
	{
		if (update)
		{
			update_serialport();
		}
		return com_list_;
	}

	typedef std::array<unsigned int, 14> baud_rate_list;
	const baud_rate_list &get_baudrate_list()
	{
		static const baud_rate_list list = {
		300,
		1200,
		2400,
		4800,
		9600,
		19200,
		38400,
		74880,
		115200,
		230400,
		250000,
		500000,
		1000000,
		2000000};
		return list;
	}

	bool open(const std::string& com)
	{
		boost::system::error_code ec;
		sp_.open(com, ec);
		if (sp_.is_open())
		{
			close_flag_ = false;
			thread_ = std::shared_ptr<std::thread>(new std::thread(
				boost::bind(&boost::asio::io_service::run, &service_)));
		}
		return sp_.is_open();
	}

	void close()
	{
		boost::system::error_code ec;
		if (!close_flag_)
		{
			close_flag_ = true;
			sp_.cancel(ec);
			service_.stop();
			thread_->join();
			thread_.reset();
			service_.reset();
			sp_.close(ec);
		}
	}

	bool is_open()
	{
		return sp_.is_open();
	}

	void set_baudrate(unsigned int baud_rate)
	{
		BOOST_ASSERT(sp_.is_open());
		sp_.set_option(boost::asio::serial_port::baud_rate(baud_rate)); //波特率，构造参数为unsigned int
		sp_.set_option(boost::asio::serial_port::flow_control());		//流量控制，构造参数为serial_port::flow_control::type，enum类型，可以是none software hardware
		sp_.set_option(boost::asio::serial_port::parity());				//奇偶校验，构造参数为serial_port::parity::type，enum类型，可以是none, odd, even
		sp_.set_option(boost::asio::serial_port::stop_bits());			//停止位，构造参数为serial_port::stop_bits::type，enum类型，可以是one onepointfive two
		sp_.set_option(boost::asio::serial_port::character_size(8));	//字符大小，构造参数为unsigned int
	}

	void async_write(const char *buf, size_t len)
	{
		{
			boost::recursive_mutex::scoped_lock lock(mutex_);
			write_buffer_[0].emplace_back(buf, len);
		}
		post_write();
	}

	void async_read()
	{
		{
			//boost::recursive_mutex::scoped_lock lock(mutex_);
		}
		post_read();
	}

	std::string get_last_error_message()
	{
		return last_error_;
	}

	void set_last_error_message(const std::string &msg)
	{
		last_error_ = msg;
	}

protected:
	virtual void on_write(boost::system::error_code ec, size_t len, const char* buf)
	{
	
	}

	virtual void on_read(boost::system::error_code ec, size_t len, const char* buf)
	{
		//std::lock_guard<std::mutex> lock(mutex_);
		//cv_.notify_one();
	}

private:
	void post_read()
	{
		if (close_flag_)
		{
			return;
		}
		boost::recursive_mutex::scoped_lock lock(mutex_);
		if (!read_complete_)
		{
			return;
		}
		read_complete_ = false;
		sp_.async_read_some(boost::asio::buffer(read_buffer_), [&](boost::system::error_code ec, std::size_t bytes_transferred) {
			on_read(ec, bytes_transferred, read_buffer_.data());
			boost::recursive_mutex::scoped_lock lock(mutex_);
			read_buffer_.fill(0);
			read_complete_ = true;
			if(!ec)
			{
				post_read();
			}
		});
	}

	void post_write()
	{
		if (close_flag_)
		{
			return;
		}
		boost::recursive_mutex::scoped_lock lock(mutex_);
		if (!write_complete_)
		{
			return;
		}
		if (write_buffer_[0].empty())
		{
			return;
		}
		write_complete_ = false;
		write_buffer_[1].swap(write_buffer_[0]);
		// Write the serialized data to the socket. We use "gather-write" to send
		// both the header and the data in a single write operation.
		std::vector<boost::asio::const_buffer> buffers;
		for (auto &buf : write_buffer_[1])
		{
			buffers.push_back(boost::asio::buffer(buf));
			//buffers.push_back(boost::asio::buffer(buf.c_str(),buf.size()));
		}
		boost::asio::async_write(sp_, buffers, [&](boost::system::error_code ec, std::size_t bytes_transferred) {
			for (auto buf : write_buffer_[1])
			{
				on_write(ec, buf.size(), buf.c_str());
			}
			boost::recursive_mutex::scoped_lock lock(mutex_);
			write_buffer_[1].clear();
			write_complete_ = true;
			if(!ec)
			{
				post_write();
			}
		});
	}
};

}

#endif//_H_SERIALPORT_H_
