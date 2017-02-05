#include "connection.hpp"
#include <utility>
#include <vector>

namespace http {
namespace server {

connection::connection(boost::asio::ip::tcp::socket socket)
  : socket_(std::move(socket))
{
}

void connection::start()
{
  try 
    {
      do_read();
    }
  catch(boost::system::error_code &e) 
    {
      throw e;
    }
}

void connection::stop()
{
  socket_.close();
}

void connection::do_read()
{
  auto self(shared_from_this());
  socket_.async_read_some(boost::asio::buffer(buffer_),
      [this, self](boost::system::error_code ec, std::size_t bytes)
      {
        reply_.content.append(buffer_.data(), buffer_.data() + bytes);
        if (reply_.content.substr(reply_.content.size() - 4, 4) == "\r\n\r\n" )
	  {
	    do_write();
	  }
        else
	  {
	    do_read();
	  }
  });
}
    
void connection::do_write()
{
  auto self(shared_from_this());
  /*
  std::vector<boost::asio::const_buffer> buffers;
  std::string sendString = "";
  sendString.append("HTTP/1.0 200 OK\r\n");

  sendString.append("Content-Length");
  sendString.append(": ");
  sendString.append(std::to_string(reply_.size()));
  sendString.append("\r\n");

  sendString.append("Content-Type");
  sendString.append(": ");
  sendString.append("text/plain\r\n\r\n");
  
  sendString.append(reply_);

  buffers.push_back(boost::asio::buffer(sendString));
  
  boost::asio::async_write(socket_, buffers,
			   [this, self](boost::system::error_code ec, std::size_t)
			   {
			     if (!ec)
			       {
				 boost::system::error_code ignored_ec;
				 stop();
			       }
			   });
         */
  reply_.status = reply::ok;
  header head0;
  head0.name = "Content-Length";
  head0.value = std::to_string(reply_.content.size());
  reply_.headers.push_back(head0);
        
  header head1;
  head1.name = "Content-Type";
  head1.value = "text/plain";
  reply_.headers.push_back(head1);

  boost::asio::async_write(socket_, reply_.to_buffers(),
      [this, self](boost::system::error_code ec, std::size_t)
      {
        if (!ec)
        {
          boost::system::error_code ignored_ec;
          stop();
        }
});
}

} // namespace server
} // namespace http
