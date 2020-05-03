#include "async_server.h"

#include <iostream>

session::session(bio::ip::tcp::socket socket,
                 std::shared_ptr<command_handler> cmd_handler) :
    _socket(std::move(socket)),
    _cmd_handler(cmd_handler)
{
    auto ep = _socket.remote_endpoint();
    _str_id = ep.address().to_string() + ":" + std::to_string(ep.port());
}

void session::start()
{
    read();
}

void session::read()
{
    auto self(shared_from_this());
    _socket.async_read_some(boost::asio::buffer(_buffer), create_read_lambda());
}

void session::handle_request(size_t length)
{
    _cmd_handler->add_command(_str_id, std::string(_buffer.data(), length));
}

void session::finish_handling()
{
    _cmd_handler->stop_handling(_str_id);
}

session::read_cb_signature session::create_read_lambda()
{
    auto self(shared_from_this());
    return [this, self](boost::system::error_code ec, std::size_t length)
    {
        if(ec == boost::asio::error::eof || ec == boost::asio::error::connection_reset)
            finish_handling();
        else
            handle_request(length);
    };
}


server::server(bio::io_context& io_context, unsigned short port,
               std::shared_ptr<command_handler> cmd_handler) :
    _acceptor(io_context, bio::ip::tcp::endpoint(bio::ip::tcp::v4(), port)),
    _cmd_handler(cmd_handler)
{
    accept();
}

void server::accept()
{
    _acceptor.async_accept(create_accept_lambda());
}

server::accept_cb_signature server::create_accept_lambda()
{
    return [this](boost::system::error_code ec, bio::ip::tcp::socket socket)
    {
        if (!ec == boost::system::errc::success)
            std::make_shared<session>(std::move(socket))->start();

        accept();
    };
}
