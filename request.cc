#include "request.hpp"
#include <boost/tokenizer.hpp>
#include <sstream>
#include <iostream>

namespace http{
  namespace server{
    std::unique_ptr<Request> Request::Parse(const std::string& raw_request){
      
      std::unique_ptr<Request> req(new Request);
      req->raw_request_ = raw_request;
   
      std::istringstream iss(raw_request);

      std::string request_line;
      std::getline(iss, request_line);
      if (request_line[request_line.size()-1] == '\r')
	{
	  request_line.pop_back();
	}
      
      size_t method_end = request_line.find(" ");
      req->method_ = request_line.substr(0, method_end);

      size_t uri_end = request_line.find(" ", method_end+1);
      req->uri_ = request_line.substr(method_end+1, uri_end-method_end-1);

      req->version_ = request_line.substr(uri_end+1);
      
      std::string cur_header;
      size_t separator;
      std::string header_name;
      std::string header_value;
      while(std::getline(iss, cur_header))
	{
	  if (cur_header[cur_header.size()-1] == '\r') // get rid of extra carriage returns
	    {
	      cur_header.pop_back();
	    }
	  if (cur_header == "") // break once we reach the empty line separating the headers and the body
	    {
	      break;
	    }
	  separator = cur_header.find(": ");
	  header_name = cur_header.substr(0, separator);
	  header_value = cur_header.substr(separator+2);
	  req->headers_.push_back(std::make_pair(header_name, header_value));
	}
      
      std::string body;
      std::getline(iss, body);
      req->body_ = body;



      return req;
    }
    
    std::string Request::raw_request() const{
      return raw_request_;
    }
    std::string Request::method() const{
      return method_;
    }
    std::string Request::uri() const{
      return uri_;
    }
    std::string Request::version() const{
      return version_;
    }
    
    using Headers = std::vector<std::pair<std::string, std::string>>;
    Headers Request::headers() const {
      return headers_;
    }
    
    std::string Request::body() const {
      return body_;
    }

    void Request::set_raw_request(std::string raw_request)
    {
      raw_request_ = raw_request;
    }
    void Request::set_method(std::string method)
    {
      method_ = method;
    }
    void Request::set_uri(std::string uri)
    {
      uri_= uri;
    }
    void Request::set_version(std::string version)
    {
      version_ = version;
    }
    void Request::set_body(std::string body)
    {
      body_ = body;
    }
    void Request::set_headers(Headers headers)
    {
      headers_ = headers;
    }

    std::string Request::get_header(const std::string header_name)
    {
      for (const auto& header : headers_)
	{
	  if (header.first == header_name)
	    {
	      return header.second;
	    }
	}
      return "";
    }

    bool Request::accept_gzip() const
    {
      for (const auto& header : headers_)
	{
	  if (header.first == "Accept-Encoding")
	    {
	      if (header.second.find("gzip") != std::string::npos)
		{
		  return true;
		}
	      return false;
	    }
	}
      return false;
    }    
  }
}
