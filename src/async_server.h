#ifndef ASYNC_SERVER_H
#define ASYNC_SERVER_H

#include <boost/asio.hpp>

namespace bio = boost::asio;

class session : public std::enable_shared_from_this<session>
{
public:
  session(bio::ip::tcp::socket socket);
  void start();

private:
  void do_read();
  void do_write(std::size_t length);

private:
  bio::ip::tcp::socket _socket;

  enum { max_length = 1024 };
  char _data[max_length];
};

class server
{
public:
  server(bio::io_context& io_context, unsigned short port);

private:
  void do_accept();

  bio::ip::tcp::acceptor _acceptor;
};
#endif // ASYNC_SERVER_H
