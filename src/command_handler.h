#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "base/base_command.h"
#include "base/base_publisher.h"
#include "commands_factory.h"

#include <memory>
#include <unordered_map>

/**
 * @brief Класс статистики обработки команд
 * comand_handler'ом
 */
struct command_handler_statistic
{
    /**
     * @brief Количество строк
     */
    size_t num_lines = 0;
    /**
     * @brief Количество блоков
     */
    size_t num_blocks = 0;
    /**
     * @brief Количество команд
     */
    size_t num_commands = 0;
};

/**
 * @brief Класс обработки команд
 */
class command_handler : public base_publisher
{
    /**
     * @brief Команды принадлежащие определенному scope
     */
    using commands = std::vector<std::string>;
    /**
     * @brief Временная метка первой команды с scope и список команд
     */
    using commands_description = std::pair<uint64_t, commands>;

    /**
     * @brief Структура контекста пользователя для обработки индивидеальных команд
     */
    struct client_context
    {
        size_t nested_level = 0;
        commands_description individual_commands;
    };

    /**
     * @brief Итератор на контекст пользователя
     */
    using context_iter = std::unordered_map<std::string, client_context>::iterator;

public:
    /**
     * @brief Конструктор обработчика команд
     * @param bulk_length - размер пакета команд
     */
    command_handler(std::size_t bulk_length);

    /**
     * @brief Метод запуска команды в обработку
     * @param command - команда
     */
    void add_command(const std::string& client, const std::string& str);

    /**
     * @brief Метод обработки завершения обработки команд от клиента
     */
    void stop_handling_client(const std::string &client);

    /**
     * @brief Метод глобальной остановки обработки комманд
     */
    void stop();

private:
    /**
     * @brief Метод обработки оповещения подписчиков
     * @param timestamp - временная метка первой команды
     * @param string - информационная строка
     */
    void notify(uint64_t timestamp, const commands &cmds);

    /**
     * @brief Метод обработки команды открытия scope
     */
    void handle_open_scope(context_iter client_context_iter);

    /**
     * @brief Метод обработки команды закрытия scope
     */
    void handle_close_scope(context_iter client_context_iter);

    /**
     * @brief Метод обработки текстовой команды
     * @param timestamp - временная метка
     * @param str - текст команды
     */
    void handle_text_command(context_iter client_context_iter, uint64_t timestamp, const std::string& str);

    /**
     * @brief Метод создания контекста пользователя, в случае если он не существует
     * @param client - текстовый идентификатор клиента
     * @return Итератор на контекст клиента
     */
    context_iter create_if_not_exists(const std::string& client);

private:
    std::size_t _bulk_length;
    commands_description _common_commands;
    std::unordered_map<std::string, client_context> _clients_contexts;

    commands_factory _factory;
};

#endif // COMMAND_HANDLER_H
