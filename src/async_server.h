#ifndef ASYNC_SERVER_H
#define ASYNC_SERVER_H

#include "command_handler.h"

#include <boost/asio.hpp>
#include <array>

namespace bio = boost::asio;

class session : public std::enable_shared_from_this<session>
{
    using read_cb_signature = std::function<void(boost::system::error_code ec, std::size_t length)>;
public:
    session(bio::ip::tcp::socket socket,
            std::shared_ptr<command_handler> cmd_handler);
    void start();

private:
    void read();
    void handle_request(size_t length);
    void finish_handling();

private:
    read_cb_signature create_read_lambda();

private:
    bio::ip::tcp::socket _socket;
    std::shared_ptr<command_handler> _cmd_handler;

    std::string _str_id;
    std::array<char, 1500> _buffer;
};

class server
{
    using accept_cb_signature = std::function<void(boost::system::error_code, bio::ip::tcp::socket)>;
public:
    server(bio::io_context& io_context, unsigned short port,
           std::shared_ptr<command_handler> cmd_handler);

private:
    void accept();

    // callback lambdas
private:
    accept_cb_signature create_accept_lambda();

private:
    bio::ip::tcp::acceptor _acceptor;
    std::shared_ptr<command_handler> _cmd_handler;
};
#endif // ASYNC_SERVER_H
