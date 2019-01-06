#pragma once

#include "server.h"
#include "XServer.h"

#include "XPBridge/BTTranslayer.hpp"

#include "XPBridge/XButton.hpp"
#include "XPBridge/XBuzzer.hpp"
#include "XPBridge/XDualDCMotor.hpp"
#include "XPBridge/XIRReceiver.hpp"
#include "XPBridge/XIRTracking.hpp"
#include "XPBridge/XLightSensor.hpp"
#include "XPBridge/XRGBLed.hpp"
#include "XPBridge/XSegDisplay.hpp"
#include "XPBridge/XSoundSensor.hpp"
#include "XPBridge/XUltrasonic.hpp"

#include "XPBridge/HQRAudioPlayer.hpp"
#include "XPBridge/HQRCarDriver.hpp"
#include "XPBridge/HQRLightShow.hpp"
#include "XPBridge/HQRTaillight.hpp"

#include <algorithm>
#include <string>
#include <iosfwd>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "jsonrpc.hpp"

#define _TEST 1

//
/*
#connnect
GET /scratch/ble HTTP/1.1
Host: localhost:8080
Connection: Upgrade
Pragma: no-cache
Cache-Control: no-cache
User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36
Upgrade: websocket
Origin: http://localhost:8601
Sec-WebSocket-Version: 13
Accept-Encoding: gzip, deflate, br
Accept-Language: zh-CN,zh;q=0.9,en;q=0.8
Cookie: _ga=GA1.1.1217839775.1544668798; _gid=GA1.1.824628297.1545361860
Sec-WebSocket-Key: gXRpyQOOUZ9xE+XIf2Aejg==
Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits
//jsonrpc
//requet:{method, params, id}
//{"jsonrpc":"2.0","method":"discover","params":{},"id":0}
//response:{result, id} or {error, id}
//{"jsonrpc":"2.0","result":null,"id":1} or {"jsonrpc":"2.0","error":{message:"100"},"id":1}
//discover
{"jsonrpc":"2.0","method":"discover","params":{"filters":[{"services":[61445]}]},"id":0}
{"jsonrpc":"2.0","method":"didDiscoverPeripheral","params":{"peripheralId":"10:a1:40:11:e1:d4","rssi":8,"timestamp":11644480800000,"advType":"type","localName":"xblockly","serviceUuids":["0000ffe0-0000-1000-8000-00805f9b34fb"]},"error":"","id":0}
//connect
{"jsonrpc":"2.0","method":"connect","params":{"peripheralId":"10:a1:40:11:e1:d4"},"id":1}
{"jsonrpc":"2.0","result":{"peripheralId":"10:a1:40:11:e1:d4"},"error":0,"id":1}
{"jsonrpc":"2.0","error":"","id":1,"result":null}
//disconnect
//read
{"jsonrpc":"2.0","method":"read","params":{"serviceId":61445,"characteristicId":"5261da01-fa7e-42ab-850b-7c80220097cc","startNotifications":true},"id":2}
{"jsonrpc":"2.0","result":{"serviceId":61445,"characteristicId":"5261da01-fa7e-42ab-850b-7c80220097cc","startNotifications":true},"error":0,"id":2}
//write
{"jsonrpc":"2.0","result":{"serviceId":61445,"characteristicId":"5261da02-fa7e-42ab-850b-7c80220097cc","message":"gUhlbGxvIQ==","encoding":"base64"},"id":3}
//ble notify 
{"jsonrpc":"2.0","method":"characteristicDidChange","params":{"serviceId":61445,"characteristicId":"5261da02-fa7e-42ab-850b-7c80220097cc","message":"gUhlbGxvIQ==","encoding":"base64"},"id":0}
*/
class extserver
	: public server
	, public XNet::XServerT<extserver, extserver>
	, public XNet::XServerT<extserver, extserver>::Listener
	, public Jsonrpc<extserver>
	, public BTLinkLayer::LinkListener
	, public BridgeTranslayer::TransListener
{
	typedef server base;
	typedef XNet::XServerT<extserver, extserver> netbase;
	typedef Jsonrpc jsonrpc;
  private:
	std::shared_ptr<boost::asio::deadline_timer> timer_ptr_;
	size_t peer_;
	BTLinkLayer btl_;
	BridgeTranslayer btt_;
	bool bridge_connected_;
	struct BlockInfo
	{
		std::string type;
		std::string model;
		std::shared_ptr<XPBlock> block;
	};
	std::map<std::string, BlockInfo> port_blocks_;
	struct ServiceKey
	{
		std::string type;
		std::string ports;
	};
	struct ServiceKeyLess
	{
		bool operator()(const ServiceKey& x, const ServiceKey& y) const
		{
			int cmp = strcmp(x.type.c_str(), y.type.c_str());
			if(cmp < 0) {
				return true;
			} else if(cmp == 0) {
				return x.ports < y.ports;
			}
			return false;
		}
	};
	std::map<ServiceKey, std::shared_ptr<XPServer>, ServiceKeyLess> port_services_;

  public:
	extserver() : base(), netbase(this), peer_(0), btl_(), btt_(&btl_), bridge_connected_(false)
	{
	}

	const char *name()
	{
		return "extserver";
	}

	void init(int argc, char *argv[])
	{
		base::init(argc, argv);

		timer_ptr_ = std::make_shared<boost::asio::deadline_timer>(base::service());
		btl_.init();

		netbase::start(5);
		netbase::listen(8080);

		timer_ptr_->expires_from_now(boost::posix_time::milliseconds(100));
		timer_ptr_->async_wait(std::bind(&extserver::on_timer, this));
	}

	void term()
	{
		timer_ptr_.reset();
		base::term();
	}

	inline bool is_run()
	{
		return netbase::is_run();
	}

	inline bool is_setup() 
	{ 
		return btl_.is_setup();
	}

	inline bool is_device_connected()
	{
		return btl_.connected();
	}

	inline bool is_bridge_connected()
	{
		return bridge_connected_;
	}

  protected:
	virtual void on_term()
	{
		boost::system::error_code ec;

		peer_ = 0;
		netbase::stop();

		disconnect_device();
		release();

		timer_ptr_->cancel(ec);

		base::on_term();
	}

	void on_timer()
	{
		if (!is_run()) {
			return;
		}

#if 0
		if(!is_setup())
		{
			setup();
		}
		else if (!is_device_connected())
		{
			std::vector<std::string> devs = btl_.devs();
			if(!devs.empty()) {
				connect_device(devs[0]);
			}
		}
		else
		{
			if (is_bridge_connected())
			{
				PRINTFL("test...");
				//XButton
				std::shared_ptr<XButton> btn = std::static_pointer_cast<XButton>(find_block("BTN"));
				if (!btn)
				{
					btn = std::static_pointer_cast<XButton>(construct_block("XButton", "", "BTN"));
					if (btn)
					{
						btn->isPressed();
						btn->registerEvent(XButton_EVT_knocked);
					}
				}
				else
				{
					if (btn->queryEventStatus())
					{
						PRINTFL("test ok.");
						disconnect_device();
					}
				}
			}
		}
#endif

#if 0
		if(is_setup())
		{
			if (is_device_connected())
			{
				if (is_bridge_connected())
				{
					std::shared_ptr<XButton> btn = std::static_pointer_cast<XButton>(find_block("BTN"));
					if (!btn)
					{
						btn = std::static_pointer_cast<XButton>(construct_block("XButton", "", "BTN"));
						if (btn)
						{
							btn->registerEvent(XButton_EVT_knocked);
						}
					}
				}
			}
		}
#endif
		check_notify();

		timer_ptr_->expires_from_now(boost::posix_time::milliseconds(100));
		timer_ptr_->async_wait(std::bind(&extserver::on_timer, this));
	}

  public:
	void notifyHandler(int evt, void *param) 
	{ 
		switch(evt)
		{
			case BTLINK_EVT_DEV_DISCOVER:
			{
				on_discover((const char*)param);
			}
			break;
		}
	}

	int setup()
	{
		return btl_.setup(this, &btt_);
	}

	void release()
	{
		btl_.release();
	}

	void discover()
	{
		btl_.discover();
	}

	void connect_device(const std::string& dev)
	{
		// 建立蓝牙链接层
		if (!btl_.connect(dev)) {
			return;
		}

		connect_bridge();

		if (!is_bridge_connected()) {
			disconnect_device();
		}
	}

	void disconnect_device()
	{
		disconnect_bridge();

		btl_.disconnect();
	}

	void connect_bridge()
	{
		// 建立传输层
		XBridge.setup(&btt_);
		XBridge.handshake();
		XBridge.connect();
		bridge_connected_ = true;
	}

	void disconnect_bridge()
	{
		// 释放传输层
		XBridge.release();
		port_blocks_.clear();
		port_services_.clear();
		bridge_connected_ = false;
	}

	inline bool is_onboard_block(const std::string &port)
	{
		if(!port.empty())
			return std::isalpha(port[0]);
		return false;
	}

	std::shared_ptr<XPBlock> construct_block(const std::string &type, const std::string &model, const std::string &port)
	{
		std::shared_ptr<XPBlock> block;
		if (type == "XButton") {
			block = std::make_shared<XButton>();
		}
		else if (type == "XBuzzer") {
			block = std::make_shared<XBuzzer>();
		}
		else if (type == "XDualDCMotor") {
			block = std::make_shared<XDualDCMotor>();
		}
		else if (type == "XIRReceiver") {
			block = std::make_shared<XIRReceiver>();
		}
		else if (type == "XIRTracking") {
			block = std::make_shared<XIRTracking>();
		}
		else if (type == "XLightSensor") {
			block = std::make_shared<XLightSensor>();
		}
		else if (type == "XRGBLed") {
			block = std::make_shared<XRGBLed>();
		}
		else if (type == "XSegDisplay") {
			block = std::make_shared<XSegDisplay>();
		}
		else if (type == "XSoundSensor") {
			block = std::make_shared<XSoundSensor>();
		}
		else if (type == "XUltrasonic") {
			block = std::make_shared<XUltrasonic>();
		}
		if (block) {
			if (model.empty()) {
				block->setup(port.c_str()); //label
			} else {
				block->setup(model.c_str(), port.c_str()); //model port
			}
			port_blocks_[port] = {type, model, block};
		}
		return block;
	}

	inline std::shared_ptr<XPBlock> find_block(const std::string &id) //id is label or port
	{
		std::shared_ptr<XPBlock> block;
		auto it = port_blocks_.find(id);
		if (it != port_blocks_.end()) {
			block = it->second.block;
		}
		return block;
	}
	
	inline std::shared_ptr<XPBlock> find_or_construct_block(const std::string& type, const std::string& model, const std::string &id) //id is label or port
	{
		std::shared_ptr<XPBlock> block;
		auto it = port_blocks_.find(id);
		if (it != port_blocks_.end()) {
			block = it->second.block;
		} else {
			if(!model.empty() || is_onboard_block(id)) {
				block = construct_block(type, model, id);
			}
		}
		return block;
	}
	
	std::shared_ptr<XPServer> construct_server(const std::string &type, const std::string &ports)
	{
		ServiceKey key = {type, ports};
		std::shared_ptr<XPServer> service;
		if (type == "HQRAudioPlayer") {
			std::shared_ptr<XBuzzer> buz = std::static_pointer_cast<XBuzzer>(find_or_construct_block("XBuzzer", "", ports));
			if(buz) {
				std::shared_ptr<HQRAudioPlayer> hqr_ap = std::make_shared<HQRAudioPlayer>();
				hqr_ap->setup(buz.get());
				service = hqr_ap;
			}
		}
		else if (type == "HQRCarDriver") {
			std::shared_ptr<XDualDCMotor> ddm = std::static_pointer_cast<XDualDCMotor>(find_or_construct_block("XDualDCMotor", "", ports));
			if(ddm) {
				std::shared_ptr<HQRCarDriver> hqr_cd = std::make_shared<HQRCarDriver>();
				hqr_cd->setup(ddm.get());
				service = hqr_cd;
			}
		}
		else if (type == "HQRLightShow") {
			std::shared_ptr<XRGBLed> rgb = std::static_pointer_cast<XRGBLed>(find_or_construct_block("XRGBLed", "", ports));
			if(rgb) {
				std::shared_ptr<HQRLightShow> hqr_ls = std::make_shared<HQRLightShow>();
				hqr_ls->setup(rgb.get());
				service = hqr_ls;
			}
		}
		else if (type == "HQRTaillight") {
			std::shared_ptr<XSegDisplay> sgd = std::static_pointer_cast<XSegDisplay>(find_or_construct_block("XSegDisplay", "", ports));
			if(sgd) {
				std::shared_ptr<HQRTaillight> hqr_tl = std::make_shared<HQRTaillight>();
				hqr_tl->setup(sgd.get());
				service = hqr_tl;
			}
		}
		if (service) {
			port_services_[key] = service;
		}
		return service;
	}

	inline std::shared_ptr<XPServer> find_server(const std::string &type, const std::string &ports)
	{
		ServiceKey key = {type, ports};
		std::shared_ptr<XPServer> service;
		auto it = port_services_.find(key);
		if (it != port_services_.end()) {
			service = it->second;
		}
		return service;
	}

	inline std::shared_ptr<XPServer> find_or_construct_server(const std::string &type, const std::string &ports)
	{
		ServiceKey key = {type, ports};
		std::shared_ptr<XPServer> service;
		auto it = port_services_.find(key);
		if (it != port_services_.end()) {
			service = it->second;
		} else {
			service = construct_server(type, ports);
		}
		return service;
	}

	void check_notify()
	{
		auto it = port_blocks_.begin();
		for(; it != port_blocks_.end(); ++it)
		{
			const std::string& type = it->second.type;
			const std::string& model = it->second.model;
			const std::string& port = it->first;
			const std::shared_ptr<XPBlock>& block = it->second.block;
			if (type == "XButton") {
				std::shared_ptr<XButton> btn = std::static_pointer_cast<XButton>(block);
				if(btn) {
					if(btn->queryEventStatus()) {
						std::shared_ptr<boost::property_tree::ptree> params = std::make_shared<boost::property_tree::ptree>();
						params->put("type", type);
						params->put("model", model);
						params->put("port", port);
						sendRemoteNotification("XButton_notifyEvent", params);
					}
				}
			}
			else if (type == "XBuzzer") {
				std::shared_ptr<XBuzzer> buz = std::static_pointer_cast<XBuzzer>(block);
			}
			else if (type == "XDualDCMotor") {
				std::shared_ptr<XDualDCMotor> ddm = std::static_pointer_cast<XDualDCMotor>(block);
			}
			else if (type == "XIRReceiver") {
				std::shared_ptr<XIRReceiver> irr = std::static_pointer_cast<XIRReceiver>(block);
			}
			else if (type == "XIRTracking") {
				std::shared_ptr<XIRTracking> irt = std::static_pointer_cast<XIRTracking>(block);
			}
			else if (type == "XLightSensor") {
				std::shared_ptr<XLightSensor> lsr = std::static_pointer_cast<XLightSensor>(block);
			}
			else if (type == "XRGBLed") {
				std::shared_ptr<XRGBLed> rgb = std::static_pointer_cast<XRGBLed>(block);
			}
			else if (type == "XSegDisplay") {
				std::shared_ptr<XSegDisplay> sgd = std::static_pointer_cast<XSegDisplay>(block);
			}
			else if (type == "XSoundSensor") {
				std::shared_ptr<XSoundSensor> ssr = std::static_pointer_cast<XSoundSensor>(block);
			}
			else if (type == "XUltrasonic") {
				std::shared_ptr<XUltrasonic> ult = std::static_pointer_cast<XUltrasonic>(block);
			}
		}	
	}

  public:
//requet:{method, params, id}
//{"jsonrpc":"2.0","method":"discover","params":{},"id":0}
//response:{result, id} or {error, id}
//{"jsonrpc":"2.0","result":null,"id":1} or {"jsonrpc":"2.0","error":{message:"100"},"id":1}
	inline bool parse_response(const boost::property_tree::ptree& json, boost::property_tree::ptree& result, size_t& id)
	{
		auto opt = json.get_child_optional("result");
		if (opt) {
			result = opt.get();
			id = json.get<size_t>("id", 0);
			return true;
		}
		return false;
	}
	inline bool parse_error_response(const boost::property_tree::ptree& json, boost::property_tree::ptree& error, size_t& id)
	{
		auto opt = json.get_child_optional("error");
		if (opt) {
			error = opt.get();
			id = json.get<size_t>("id", 0);
			return true;
		}
		return false;
	}

	void on_setup(size_t peer)
	{
		if(peer_) {
			return;
		}
		peer_ = peer;
		setup();
	}

	void on_release(size_t peer)
	{
		if(peer_ != peer) {
			return;
		}
		peer_ = 0;
		jsonrpc::clear();
		disconnect_device();
		release();
	}

	inline bool didReceive_XKit_discover(const std::shared_ptr<boost::property_tree::ptree>& params, std::shared_ptr<boost::property_tree::ptree>& result) {
		//{"jsonrpc":"2.0","method":"discover","params":{"filters":[{"services":[61445]}]},"id":0}
		disconnect_device();
		discover();
		return true;
	}

	inline void on_discover(const std::string& dev)
	{
		if(!peer_) {
			return;
		}
		std::ostringstream ss;
		ss << R"({"jsonrpc":"2.0","method":"didDiscoverPeripheral","params":{"peripheralId":")" 
		<< dev << R"(", "name":"hqrover:)" << dev << R"(","rssi":8,"timestamp":11644480800000,"advType":"type","localName":"xblockly","serviceUuids":["0000ffe0-0000-1000-8000-00805f9b34fb"]},"id":0})";
		std::string send_data = ss.str();
		sendRemoteNotification(send_data);
		//std::future<std::shared_ptr<Response>> f = sendRemoteRequest("didDiscoverPeripheral", nullptr);
		//std::shared_ptr<Response> response = f.get();
		//if(response) {
		//	//
		//}
	}

	inline bool didReceive_XKit_connect (const std::shared_ptr<boost::property_tree::ptree>& params, std::shared_ptr<boost::property_tree::ptree>& result) {
		//{"jsonrpc":"2.0","method":"connect","params":{"peripheralId":"10:a1:40:11:e1:d4"},"id":1}
		const std::string& dev = params->get<std::string>("peripheralId", "");
		connect_device(dev);
		result = params;
		return is_bridge_connected();
	}

	inline bool didReceive_read (const std::shared_ptr<boost::property_tree::ptree>& params, std::shared_ptr<boost::property_tree::ptree>& result) {
		//{"jsonrpc":"2.0","method":"read","params":{"serviceId":61445,"characteristicId":"5261da01-fa7e-42ab-850b-7c80220097cc","startNotifications":true},"id":2}
		//send_data = R"({"jsonrpc":"2.0","result":{"serviceId":61445,"characteristicId":"5261da01-fa7e-42ab-850b-7c80220097cc","message":"ok"}, "id":2})";
		result = params;
		/*std::shared_ptr<XButton> btn = std::static_pointer_cast<XButton>(find_block("BTN"));
		if (!btn) {
			btn = std::static_pointer_cast<XButton>(construct_block("XButton", "", "BTN"));
			if (btn) {
				btn->isPressed();
				btn->registerEvent(XButton_EVT_knocked);
			}
		}*/
		return true;
	}

	inline bool didReceive_write (const std::shared_ptr<boost::property_tree::ptree>& params, std::shared_ptr<boost::property_tree::ptree>& result) {
		//{"jsonrpc":"2.0","method":"write","params":{"serviceId":61445,"characteristicId":"5261da02-fa7e-42ab-850b-7c80220097cc","message":"gUhlbGxvIQ==","encoding":"base64","withResponse":true},"id":3}
		if (params->get<bool>("withResponse", false)) {
			//send_data = R"({"jsonrpc":"2.0","result":{"serviceId":61445,"characteristicId":"5261da01-fa7e-42ab-850b-7c80220097cc","message":"ok"}, "id":3})";
			result = params;
		}
		return true;
	}

	inline bool didReceive_XKit_constructBlock(const std::shared_ptr<boost::property_tree::ptree>& params, std::shared_ptr<boost::property_tree::ptree>& result)
	{
		const std::string &type = params->get<std::string>("type", "");
		const std::string &model = params->get<std::string>("model", "");
		const std::string &port = params->get<std::string>("port", "");
		std::shared_ptr<XPBlock> block = find_or_construct_block(type, model, port);
		result = params;
		return true;
	}

	inline bool didReceive_XKit_constructServer(const std::shared_ptr<boost::property_tree::ptree>& params, std::shared_ptr<boost::property_tree::ptree>& result)
	{
		const std::string &type = params->get<std::string>("type", "");
		const std::string &ports = params->get<std::string>("ports", "");
		std::shared_ptr<XPServer> service = find_server(type, ports);
		if (!service) {
			service = construct_server(type, ports);
		}
		result = params;
		return true;
	}

	// inline bool didReceive_XButton_isPressed(const std::shared_ptr<boost::property_tree::ptree>& params, std::shared_ptr<boost::property_tree::ptree>& result)
	// {
	// 	const std::string &port = params->get<std::string>("port", "");
	// 	std::shared_ptr<XButton> btn = std::static_pointer_cast<XButton>(find_or_construct_block("XButton", "", port));
	// 	if (btn) {
	// 		result = params;
	// 		result->put("result", btn->isPressed());
	// 		return true;
	// 	}
	// 	return false;
	// }

	// inline bool didReceive_XButton_isKnocked(const std::shared_ptr<boost::property_tree::ptree>& params, std::shared_ptr<boost::property_tree::ptree>& result)
	// {
	// 	const std::string &port = params->get<std::string>("port", "");
	// 	std::shared_ptr<XButton> btn = std::static_pointer_cast<XButton>(find_or_construct_block("XButton", "", port));
	// 	if (btn) {
	// 		result = params;
	// 		result->put("result", btn->isKnocked());
	// 		return true;
	// 	}
	// 	return false;
	// }

	// inline bool didReceive_XButton_registerEvent(const std::shared_ptr<boost::property_tree::ptree>& params, std::shared_ptr<boost::property_tree::ptree>& result)
	// {
	// 	const std::string &port = params->get<std::string>("port", "");
	// 	std::shared_ptr<XButton> btn = std::static_pointer_cast<XButton>(find_or_construct_block("XButton", "", port));
	// 	if (btn) {
	// 		btn->registerEvent(XButton_EVT_knocked);
	// 		return true;
	// 	}
	// 	return false;
	// }

	// inline bool didReceive_HQRTaillight_showNumber(const std::shared_ptr<boost::property_tree::ptree>& params, std::shared_ptr<boost::property_tree::ptree>& result)
	// {
	// 	const std::string &ports = params->get<std::string>("ports", "");
	// 	std::shared_ptr<HQRTaillight> tll = std::static_pointer_cast<HQRTaillight>(find_or_construct_server("HQRTaillight", ports));
	// 	if (tll) {
	// 		tll->showNumber(params->get<float>("num", (float)0));
	// 		result = params;
	// 		return true;
	// 	}
	// 	return false;
	// }
	
	// inline bool didReceive_HQRTaillight_clear(const std::shared_ptr<boost::property_tree::ptree>& params, std::shared_ptr<boost::property_tree::ptree>& result)
	// {
	// 	const std::string &ports = params->get<std::string>("ports", "");
	// 	std::shared_ptr<HQRTaillight> tll = std::static_pointer_cast<HQRTaillight>(find_or_construct_server("HQRTaillight", ports));
	// 	if (tll) {
	// 		tll->clear();
	// 		result = params;
	// 		return true;
	// 	}
	// 	return false;
	// }

	inline bool didReceive_XSegDisplay_showNumber(const std::shared_ptr<boost::property_tree::ptree>& params, std::shared_ptr<boost::property_tree::ptree>& result)
	{
		const std::string &model = params->get<std::string>("model", "");
		const std::string &port = params->get<std::string>("port", "");
		std::shared_ptr<XSegDisplay> sgd = std::static_pointer_cast<XSegDisplay>(find_or_construct_block("XSegDisplay", model, port));
		if (sgd) {
			sgd->showNumber(params->get<float>("num", (float)0));
			return true;
		}
		return false;
	}
	
	inline bool didReceive_XSegDisplay_clear(const std::shared_ptr<boost::property_tree::ptree>& params, std::shared_ptr<boost::property_tree::ptree>& result)
	{
		const std::string &model = params->get<std::string>("model", "");
		const std::string &port = params->get<std::string>("port", "");
		std::shared_ptr<XSegDisplay> sgd = std::static_pointer_cast<XSegDisplay>(find_or_construct_block("XSegDisplay", model, port));
		if (sgd) {
			sgd->clear();
			return true;
		}
		return false;
	}

	void on_packet(size_t peer, std::string& buffer)
	{
		//if(peer_ != peer) {
		//	return;
		//}
		//
		//
		// std::string send_data;
		// boost::property_tree::ptree send_json;
		// boost::property_tree::ptree recv_json;
		// if(str_to_json(buffer, recv_json))
		// {
		// 	std::string method;
		// 	boost::property_tree::ptree params;
		// 	size_t id;
		// 	if(parse_request(recv_json, method, params, id))
		// 	{
		// 		if(method == "discover")
		// 		{
		// 			//{"jsonrpc":"2.0","method":"discover","params":{"filters":[{"services":[61445]}]},"id":0}
		// 			disconnect_device();
		// 			discover();
		// 			/*send_data = R"({"jsonrpc":"2.0","method":"didDiscoverPeripheral","params":{"peripheralId":"10:a1:40:11:e1:d4", "name":"hqrover","rssi":8,"timestamp":11644480800000,"advType":"type","localName":"xblockly","serviceUuids":["0000ffe0-0000-1000-8000-00805f9b34fb"]},"id":0})";
		// 			post_packet(peer, send_data.data(), send_data.size());
		// 			send_data = R"({"jsonrpc":"2.0","method":"didDiscoverPeripheral","params":{"peripheralId":"10:a1:40:11:e1:d5", "name":"hqrover","rssi":2,"timestamp":11644480800000,"advType":"type","localName":"xblockly","serviceUuids":["0000ffe0-0000-1000-8000-00805f9b34fb"]},"id":0})";
		// 			post_packet(peer, send_data.data(), send_data.size());*/
		// 			//send_data = R"({"jsonrpc":"2.0","result":null,"id":0})";
		// 			//post_packet(peer, send_data.data(), send_data.size());
		// 		}
		// 		else if(method == "connect")
		// 		{
		// 			//{"jsonrpc":"2.0","method":"connect","params":{"peripheralId":"10:a1:40:11:e1:d4"},"id":1}
		// 			connect_device(params.get<std::string>("peripheralId", ""));
		// 			if(is_bridge_connected()) {
		// 				//result
		// 				send_data = R"({"jsonrpc":"2.0","result":{"peripheralId":"10:a1:40:11:e1:d4","message":"ok"}, "id":1})";
		// 				build_response(send_json, params, id);
		// 				json_to_str(send_json, send_data);
		// 				post_packet(peer, send_data.data(), send_data.size());
		// 			} else {
		// 				//error
		// 				boost::property_tree::ptree error = params;
		// 				error.put("message", btl_.get_last_error_message());
		// 				build_error_response(send_json, error, id);
		// 				json_to_str(send_json, send_data);
		// 				post_packet(peer, send_data.data(), send_data.size());
		// 			}
		// 		}
		// 		else if(method == "read")
		// 		{
		// 			//{"jsonrpc":"2.0","method":"read","params":{"serviceId":61445,"characteristicId":"5261da01-fa7e-42ab-850b-7c80220097cc","startNotifications":true},"id":2}
		// 			//send_data = R"({"jsonrpc":"2.0","result":{"serviceId":61445,"characteristicId":"5261da01-fa7e-42ab-850b-7c80220097cc","message":"ok"}, "id":2})";
		// 			std::shared_ptr<XButton> btn = std::static_pointer_cast<XButton>(find_block("BTN"));
		// 			if (!btn)
		// 			{
		// 				btn = std::static_pointer_cast<XButton>(construct_block("XButton", "", "BTN"));
		// 				if (btn)
		// 				{
		// 					btn->isPressed();
		// 					btn->registerEvent(XButton_EVT_knocked);
		// 				}
		// 			}
		// 			build_response(send_json, params, id);
		// 			json_to_str(send_json, send_data);
		// 			post_packet(peer, send_data.data(), send_data.size());
		// 		}
		// 		else if(method == "write")
		// 		{
		// 			//{"jsonrpc":"2.0","method":"write","params":{"serviceId":61445,"characteristicId":"5261da02-fa7e-42ab-850b-7c80220097cc","message":"gUhlbGxvIQ==","encoding":"base64","withResponse":true},"id":3}
		// 			if(params.get<bool>("withResponse", false))
		// 			{
		// 				//send_data = R"({"jsonrpc":"2.0","result":{"serviceId":61445,"characteristicId":"5261da01-fa7e-42ab-850b-7c80220097cc","message":"ok"}, "id":3})";
		// 				build_response(send_json, params, id);
		// 				json_to_str(send_json, send_data);
		// 				post_packet(peer, send_data.data(), send_data.size());
		// 			}
		// 		}
		// 		else
		// 		{
		// 			build_response(send_json, params, id);
		// 			json_to_str(send_json, send_data);
		// 			post_packet(peer, send_data.data(), send_data.size());
		// 		}
		// 	}
		// 	else
		// 	{
		// 		//
		// 	}
		// }
	}

//protected:
    virtual void _sendMessage (const std::string& msg) {
		std::cout << "send: " << msg << std::endl;
		post_packet(peer_, msg.data(), msg.size());
    }

	virtual bool didReceiveCall (const std::string& method, const std::shared_ptr<boost::property_tree::ptree>& params, std::shared_ptr<boost::property_tree::ptree>& result) {
		//std::promise<std::shared_ptr<boost::property_tree::ptree>> promise;
		//base::service().post(boost::bind(&extserver::didReceive_Request, this, method, params));
        //result = promise->get_future().get();
		if (method == "discover") {
			return didReceive_XKit_discover(params,result);
		} else if (method == "connect") {
			return didReceive_XKit_connect(params,result);
		} else if (method == "constructBlock") {
			return didReceive_XKit_constructBlock(params,result);
		//} else if (method == "XButton_isPressed") {
		//	return didReceive_XButton_isPressed(params,result);
		//} else if (method == "XButton_isKnocked") {
		//	return didReceive_XButton_isKnocked(params,result);
		//} else if (method == "XButton_registerEvent") {
		//	return didReceive_XButton_registerEvent(params,result);
		//} else if (method == "HQRTaillight_showNumber") {
		//	return didReceive_HQRTaillight_showNumber(params,result);
		//} else if (method == "HQRTaillight_clear") {
		//	return didReceive_HQRTaillight_clear(params,result);
		} else if (method == "XSegDisplay_showNumber") {
			return didReceive_XSegDisplay_showNumber(params,result);
		} else if (method == "XSegDisplay_clear") {
			return didReceive_XSegDisplay_clear(params,result);
		} else {
			result = params;
			return true;
		}
		return false;
    }

#if XSERVER_PROTOTYPE_HTTP || XSERVER_PROTOTYPE_WEBSOCKET
	template <class Body, class Allocator>
	void
	on_ws_preaccept(ws_ptr peer_ptr, boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>> &&req)
	{
		std::cout << req << std::endl;
		base::service().post(boost::bind(&extserver::on_setup, this, peer_ptr->id()));
	}

	void on_ws_read(ws_ptr peer_ptr, std::string &buffer)
	{
		std::cout << "recv: " << buffer << std::endl;
		//base::service().post(boost::bind(&extserver::on_packet, this, peer_ptr->id(), buffer));
		if(peer_ != peer_ptr->id()) {
			return;
		}
		_handleMessage(buffer);
		peer_ptr->do_read();
	}
	
	void on_ws_close(ws_t *peer_ptr) 
	{
		std::cout << "peer closed : " << peer_ptr->id() << std::endl;
		if(peer_ != peer_ptr->id()) {
			return;
		}
		base::service().post(boost::bind(&extserver::on_release, this, peer_ptr->id()));
	}
#endif
};

