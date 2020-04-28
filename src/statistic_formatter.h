#ifndef STATISTIC_FORMATTER_H
#define STATISTIC_FORMATTER_H

#include "base/base_workers_keeper.h"
#include "command_handler.h"

/**
 * @brief Класс осуществляющий форматирование статистики
 * обработки комманда
 */
class statistic_formatter
{
public:
    /**
     * @brief Преобразование статистики работы worker'а в строку
     * @arg stat - контекст worker'а
     * @return Строковое представление статистики
     */
    static std::string format(context_sptr stat);

    /**
     * @brief Преобразование статистики работы command_handler'а в строку
     * @arg stat - статистика работы command_handler'а
     * @return Строковое представление статистики
     */
    static std::string format(const command_handler_statistic& stat);
};

#endif // STATISTIC_FORMATTER_H
