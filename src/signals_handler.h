#ifndef SIGNALS_HANDLER_H
#define SIGNALS_HANDLER_H

#include "command_handler.h"
#include "base/base_workers_keeper.h"

#include <memory>

/**
 * @brief Класс содержащий колбек для обработки сигнала ОС
 * а также контекст приложения
 */
class signals_handler
{
public:
    /**
     * @brief Установка контекста приложения для корректного его завершения по сигналу
     * @param handler - обработчик команд консоли
     * @param wk - worker'ы выполняющие работу subscriber'ов
     */
    static void set_state(std::shared_ptr<command_handler> handler,
                          std::vector<std::weak_ptr<base_workers_keeper>> wk);

    /**
     * @brief Обработчик сигнала (SIGINT)
     */
    static void handle_sigint(int);

private:
    inline static std::weak_ptr<command_handler> _handler;
    inline static std::vector<std::weak_ptr<base_workers_keeper>> _workers_keepers;
};

#endif // SIGNALS_HANDLER_H
