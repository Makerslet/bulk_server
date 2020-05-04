#include "command_handler.h"
#include "commands.h"

#include <iostream>

command_handler::command_handler(std::size_t bulk_length) :
    _bulk_length(bulk_length)
{}


void command_handler::add_command(const std::string& client,
        const std::string& str)
{
    context_iter iter = create_if_not_exists(client);

    try {
        auto cmd = _factory.create_command(str);
        switch (cmd->type()) {
            case command_type::open_scope: {
                handle_open_scope(iter);
                break;
            }
            case command_type::close_scope: {
                handle_close_scope(iter);
                break;
            }
            case command_type::text: {
                auto timestampt = cmd->timestamp();
                std::string str = dynamic_cast<text_command*>((cmd.get()))->info();
                handle_text_command(iter, timestampt, str);
                break;
            }
        }
    }
    catch(const std::logic_error& ex) {
        std::cout << ex.what() << std::endl;
    }
}


void command_handler::handle_open_scope(context_iter client_context_iter)
{
    auto& context = client_context_iter->second;
    ++context.nested_level;
}

void command_handler::handle_close_scope(context_iter client_context_iter)
{
    auto& context = client_context_iter->second;

    if(context.nested_level == 0)
        throw std::logic_error("Ignore semantically wrong command");

    if(context.nested_level == 1)
    {
        if(!context.individual_commands.second.empty())
        {
            notify(context.individual_commands.first,
                   context.individual_commands.second);

            context.individual_commands.second.clear();
        }
    }

    --context.nested_level;
}

void command_handler::stop_handling_client(const std::string& client)
{
    _clients_contexts.erase(client);
}

void command_handler::stop()
{
    if(!_common_commands.second.empty())
        notify(_common_commands.first, _common_commands.second);
}

void command_handler::handle_text_command(context_iter client_context_iter,
            uint64_t timestamp, const std::string& str)
{
    auto& context = client_context_iter->second;
    auto& commands = context.nested_level > 0 ? context.individual_commands : _common_commands;

    if(commands.second.empty())
        commands.first = timestamp;
    commands.second.push_back(str);

    if(context.nested_level == 0)
    {
        if(commands.second.size() == _bulk_length)
        {
            notify(commands.first, commands.second);
            commands.second.clear();
        }
    }
}

command_handler::context_iter command_handler::create_if_not_exists(const std::string& client)
{
    context_iter iter = _clients_contexts.find(client);
    if(iter == _clients_contexts.end())
    {
        auto [inserted_iter, inserted] = _clients_contexts.try_emplace(client);
        if(inserted)
            iter = inserted_iter;
    }

    return iter;
}

void command_handler::notify(uint64_t timestamp,const commands& cmds)
{
    auto task_ptr = std::make_shared<subscriber_task>(timestamp, cmds);

    for(auto subscriber : _subscribers)
    {
        auto shared_subscriber = subscriber.lock();
        if(shared_subscriber)
            shared_subscriber->update(task_ptr);
    }
}
