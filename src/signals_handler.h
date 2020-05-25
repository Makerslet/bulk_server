#ifndef SIGNALS_HANDLER_H
#define SIGNALS_HANDLER_H

#include "base/base_workers_keeper.h"
#include "async_server.h"

#include <boost/system/error_code.hpp>
#include <functional>

/**
 * @brief Класс содержащий генератор колбека для обработки сигнала SIGINT
 */
class signals_handler
{
    using sig_handler_signature = std::function<void(const boost::system::error_code&, int)>;
public:
    /**
     * @brief Генератор обработчика сигнала (SIGINT)
     * @arg srv - сервер приложения
     */
    static sig_handler_signature create_sigint_handler(server& srv,
                    std::vector<std::weak_ptr<base_workers_keeper>> workers);
};

#endif // SIGNALS_HANDLER_H
