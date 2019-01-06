#pragma once

#ifndef _H_JSONRPC_HPP_
#define _H_JSONRPC_HPP_

#include <map>
#include <memory>
#include <future>
#include <thread>
#include <mutex>
//#define BOOST_THREAD_PROVIDES_FUTURE
//#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
//#define BOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY
//#include <boost/thread/future.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

template<class T>
class Jsonrpc 
{
public:
	class Packet
	{
	public:
        Packet(const std::shared_ptr<boost::property_tree::ptree>& d):json(d),id((size_t)-1) {}
        Packet(const std::shared_ptr<boost::property_tree::ptree>& d, size_t x):json(d),id(x) {}
		std::shared_ptr<boost::property_tree::ptree> json;
		size_t id;
	};
	class Response : public Packet
	{
        typedef Packet Base;
	public:
        Response(bool e, const std::shared_ptr<boost::property_tree::ptree>& data, size_t id):Base(data, id) {
            if(e) {
                error = json;
            } else {
                result = json;
            }
        }
		std::shared_ptr<boost::property_tree::ptree> error;
        std::shared_ptr<boost::property_tree::ptree> result;
	};
	class Request : public Packet
	{
        typedef Packet Base;
	public:
        Request(const std::string& m, const std::shared_ptr<boost::property_tree::ptree>& data, size_t id
        , std::shared_ptr<std::promise<std::shared_ptr<Response>>> r):Base(data, id), method(m),response(r) { 
            params = json;
        }
		std::string method;
		std::shared_ptr<boost::property_tree::ptree> params;
        std::shared_ptr<std::promise<std::shared_ptr<Response>>> response;
	};
private:
	size_t _requestID;
	std::map<size_t,std::shared_ptr<Request>> _openRequests;
    std::mutex mutex_;
public:
	Jsonrpc () {
        _requestID = 0;
        //_openRequests;
    }

    void clear()
    {
		std::unique_lock<std::mutex> lock(mutex_);
        _openRequests.clear();
        _requestID = 0;
    }

    /**
     * Make an RPC request and retrieve the result.
     * @param {string} method - the remote method to call.
     * @param {object} params - the parameters to pass to the remote method.
     * @returns {Promise} - a promise for the result of the call.
     */
    //std::shared_ptr<std::promise<std::shared_ptr<Response>>> 
    std::future<std::shared_ptr<Response>>
    sendRemoteRequest (const std::string& method, const std::shared_ptr<boost::property_tree::ptree>& params) {
        std::unique_lock<std::mutex> lock(mutex_);
        const size_t requestID = _requestID++;

        std::shared_ptr<std::promise<std::shared_ptr<Response>>> promise = std::make_shared<std::promise<std::shared_ptr<Response>>>();
        std::shared_ptr<Request> request = std::make_shared<Request>(method, params, requestID, promise);
        _openRequests[requestID] = request;
        _sendRequest(method, params, requestID);
        
        /*const promise = new Promise((resolve, reject) => {
            _openRequests[requestID] = {resolve, reject};
        });

        _sendRequest(method, params, requestID);*/

        return promise->get_future();
    }

    /**
     * Make an RPC notification with no expectation of a result or callback.
     * @param {string} method - the remote method to call.
     * @param {object} params - the parameters to pass to the remote method.
     */
    void sendRemoteNotification (const std::string& method, const std::shared_ptr<boost::property_tree::ptree>& params) {
        _sendRequest(method, params, (size_t)-1);
    }

    void sendRemoteNotification (const std::string& msg) {
        _sendMessage(msg);
    }

    /**
     * Handle an RPC request from remote, should return a result or Promise for result, if appropriate.
     * @param {string} method - the method requested by the remote caller.
     * @param {object} params - the parameters sent with the remote caller's request.
     */
    virtual bool didReceiveCall (const std::string& method, const std::shared_ptr<boost::property_tree::ptree>& params, std::shared_ptr<boost::property_tree::ptree>& result) {
        //throw new Error('Must override didReceiveCall');
        return nullptr;
    }

protected:
	bool json_to_str(const boost::property_tree::ptree &json, std::string &str)
	{
		try
		{
			std::ostringstream ss;
			boost::property_tree::write_json(ss, json);
			str = ss.str();
#if _TEST
			printf("send json: \n%s\n", str.c_str());
#endif
			return true;
		}
		catch (...)
		{
		}
		return false;
	}

	bool str_to_json(const std::string &str, boost::property_tree::ptree &json)
	{
		try
		{
#if _TEST
//			printf("recv json: \n%s\n", str.c_str());
#endif
			std::istringstream ss;
			ss.str(str);
			boost::property_tree::json_parser::read_json(ss, json);
			return true;
		}
		catch (...)
		{
		}
		return false;
	}
    //-1:error 0:request 1:result
    inline int parse_packet(const boost::property_tree::ptree& json, std::string& method, boost::property_tree::ptree& data, size_t& id)
	{
        int ret = -1;
		method = json.get<std::string>("method", "");
		if(!method.empty()) {
			auto opt = json.get_child_optional("params");
			if (opt) {
				data = opt.get();
			}
            ret = 0;
		} else {
			auto opt = json.get_child_optional("result");
			if (opt) {
				data = opt.get();
                ret = 1;
			} else {
                auto opt = json.get_child_optional("error");
                if (opt) {
                    data = opt.get();
                    ret = -1;
                }
            }
        }
		id = json.get<size_t>("id", (size_t)-1);
		return ret;
	}
	inline void build_request(boost::property_tree::ptree& json, const std::string& method, const boost::property_tree::ptree& params, size_t id)
	{
		json.put("jsonrpc", "2.0");
		json.put("method", method);
		json.put_child("params", params);
		json.put("id", id);
	}
	inline void build_request(std::string& str, const std::string& method, const boost::property_tree::ptree& params, size_t id)
	{
		boost::property_tree::ptree json;
		build_request(json, method, params, id);
		json_to_str(json, str);
	}
	inline void build_response(boost::property_tree::ptree& json, const boost::property_tree::ptree& result, size_t id)
	{
		json.put("jsonrpc", "2.0");
		json.put_child("result", result);
		json.put("id", id);
	}
	inline void build_error_response(boost::property_tree::ptree& json, const boost::property_tree::ptree& error, size_t id)
	{
		json.put("jsonrpc", "2.0");
		json.put_child("error", error);
		json.put("id", id);
	}

    virtual void _sendMessage (const std::string& msg) {
        //throw new Error('Must override _sendMessage');
    }

    virtual void _sendRequest (const std::string& method, std::shared_ptr<boost::property_tree::ptree> params, size_t id) {
        std::string msg;
        boost::property_tree::ptree json;
        build_request(json, method, params?*params:boost::property_tree::ptree(), id);
        json_to_str(json, msg);
        _sendMessage(msg);
    }

    virtual void _handleMessage (const std::string& msg) {
        boost::property_tree::ptree json;
		if(str_to_json(msg, json)) {
			std::string method;
			std::shared_ptr<boost::property_tree::ptree> data = std::make_shared<boost::property_tree::ptree>();
			size_t id;
            switch(parse_packet(json, method, *data, id))
            {
            case 0:
			{
                //丢到service所在线程执行，这样保持和RPC都在一个线程执行
                T* pT = static_cast<T*>(this);
                pT->service().post(boost::bind(&T::_handleRequest, pT, method, data, id));
                //_handleRequest(method, data, id);
            }
            break;
            case 1:
			{
                _handleResponse(nullptr, data, id);
            }
            break;
            default:
            {
                _handleResponse(data, nullptr, id);
            }
            break;
            }
        }
        /*if (json.jsonrpc !== '2.0') {
            throw new Error(`Bad or missing JSON-RPC version in message: ${json}`);
        }
        if (json.hasOwnProperty('method')) {
            _handleRequest(json);
        } else {
            _handleResponse(json);
        }*/
    }

    virtual void _sendResponse (size_t id, const std::shared_ptr<boost::property_tree::ptree>& data, bool error) {
        std::string msg;
        boost::property_tree::ptree json;
        if(error) {
            build_error_response(json, data?*data:boost::property_tree::ptree(), id);
        } else {
            build_response(json, data?*data:boost::property_tree::ptree(), id);
        }
        json_to_str(json, msg);
        _sendMessage(msg);
        /*const response = {
            jsonrpc: '2.0',
            id
        };
        if (error) {
            response.error = error;
        } else {
            response.result = result || null;
        }
        _sendMessage(response);*/
    }

    virtual void _handleResponse (const std::shared_ptr<boost::property_tree::ptree>& error, const std::shared_ptr<boost::property_tree::ptree>& result, size_t id) {
        std::unique_lock<std::mutex> lock(mutex_);
        auto it = _openRequests.find(id);
        if(it != _openRequests.end()) {
            const std::shared_ptr<Request> openRequest = it->second;
            if(openRequest) {
                if(error) {
                    openRequest->response->set_value(std::make_shared<Response>(true,error,id));
                } else {
                    openRequest->response->set_value(std::make_shared<Response>(false,result,id));
                }
            }
        }
        /*const {result, error, id} = json;
        const openRequest = _openRequests[id];
        delete _openRequests[id];
        if (openRequest) {
            if (error) {
                openRequest.reject(error);
            } else {
                openRequest.resolve(result);
            }
        }*/
    }

    virtual void _handleRequest (const std::string& method, const std::shared_ptr<boost::property_tree::ptree>& params, size_t id) {
        std::shared_ptr<boost::property_tree::ptree> result;
        bool ret = didReceiveCall(method, params, result);
        if (id != (size_t)-1) {
            _sendResponse(id, result, !ret);
        }
        /*const {method, params, id} = json;
        const rawResult = didReceiveCall(method, params);
        if (id) {
            Promise.resolve(rawResult).then(
                result => {
                    _sendResponse(id, result);
                },
                error => {
                    _sendResponse(id, null, error);
                }
            );
        }*/
    }
};

#endif//_H_JSONRPC_HPP_
