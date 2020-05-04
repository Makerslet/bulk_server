#include "signals_handler.h"

signals_handler::sig_handler_signature signals_handler::create_sigint_handler(server& srv)
{
    return [&srv](const boost::system::error_code&, int) {
        srv.stop_accepting();
        std::exit(0);
    };
}
