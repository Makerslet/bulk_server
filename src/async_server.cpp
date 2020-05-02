#include "async_server.h"

#include <iostream>

session::session(bio::ip::tcp::socket socket)
  : _socket(std::move(socket))
{
}

void session::start()
{
  do_read();
}

void session::do_read()
{
  auto self(shared_from_this());
  _socket.async_read_some(boost::asio::buffer(_data, max_length),
      [this, self](boost::system::error_code ec, std::size_t length)
      {
        if (!ec)
        {
          std::cout << "receive " << length << "=" << std::string{_data, length} << std::endl;
          do_write(length);
        }
      });
}

void session::do_write(std::size_t length)
{
  auto self(shared_from_this());
  boost::asio::async_write(_socket, boost::asio::buffer(_data, length),
      [this, self](boost::system::error_code ec, std::size_t /*length*/)
      {
        if (!ec)
        {
          do_read();
        }
      });
}

server::server(bio::io_context& io_context, unsigned short port)
  : _acceptor(io_context, bio::ip::tcp::endpoint(bio::ip::tcp::v4(), port))
{
  do_accept();
}

void server::do_accept()
{
  _acceptor.async_accept(
      [this](boost::system::error_code ec, bio::ip::tcp::socket socket)
      {
        if (!ec)
        {
          std::make_shared<session>(std::move(socket))->start();
        }

        do_accept();
      });
}
