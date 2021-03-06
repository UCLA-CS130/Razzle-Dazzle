#ifndef SERVER_INFO_H
#define SERVER_INFO_H

#include "response.hpp"
#include "request.hpp"
#include <string>
#include <vector>
#include <mutex>

namespace http {
    namespace server {
        
        struct RequestInfo {
            std::string url;
            Response::ResponseCode rc;
        };
        
        struct HandlerInfo {
            std::string type_of_handler;
            std::string url_prefix;
        };
        
        class ServerInfo {
        public:

            static ServerInfo& getInstance();
            int get_number_of_requests();
            void append_request(std::string url, Response::ResponseCode rc);
            void append_handler(std::string type_of_handler, std::string url_prefix);
            void append_request(RequestInfo ri);
            void append_handler(HandlerInfo hi);
            std::vector<RequestInfo> ret_request_info() {return request_info;}
            std::vector<HandlerInfo> ret_handler_info() {return handler_info;}
            void lock();
            void unlock();
            
        private:
            std::vector<RequestInfo> request_info;
            std::vector<HandlerInfo> handler_info;
            ServerInfo() {}
            std::mutex mutex_;
        };
    }
}





#endif
