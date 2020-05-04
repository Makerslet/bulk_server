#include "subscriber_mock.h"
#include "commands_factory.h"
#include "command_handler.h"
#include "commands.h"

#include <gtest/gtest.h>

TEST(FACTORY, OPEN_SCOPE_COMMAND)
{
    std::string str = "{";
    commands_factory factory;
    auto cmd = factory.create_command(str);

    ASSERT_EQ(cmd->type(), command_type::open_scope);
}

TEST(FACTORY, CLOSE_SCOPE_COMMAND)
{
    std::string str = "}";
    commands_factory factory;
    auto cmd = factory.create_command(str);

    ASSERT_EQ(cmd->type(), command_type::close_scope);
}

TEST(FACTORY, TEXT_COMMAND)
{
    std::string str = "abrakadabra";
    commands_factory factory;
    auto cmd = factory.create_command(str);

    ASSERT_EQ(cmd->type(), command_type::text);
    ASSERT_EQ(dynamic_cast<text_command*>(cmd.get())->info(), str);
}

TEST(COMMAND_HANDLER, BASE_CHECK)
{
    auto subscriber = std::make_shared<subscriber_mock>();
    command_handler cmd_handler(1);
    cmd_handler.subscribe(subscriber);

    std::string command_text("hello_world");
    std::string client_id = "client1";
    cmd_handler.add_command(client_id, command_text);

    std::vector<std::string> expected;
    expected.push_back(command_text);
    ASSERT_TRUE(expected == subscriber->output());
}

TEST(COMMAND_HANDLER, INPUT_ENDED)
{
    auto subscriber = std::make_shared<subscriber_mock>();
    command_handler cmd_handler(2);
    cmd_handler.subscribe(subscriber);


    std::string command_text("hello_world");
    std::string client_id = "client1";
    cmd_handler.add_command(client_id, command_text);
    cmd_handler.stop_handling_client(client_id);
    cmd_handler.stop();

    std::vector<std::string> expected;
    expected.push_back(command_text);
    ASSERT_TRUE(expected == subscriber->output());
}

TEST(COMMAND_HANDLER, CLIENT_STARTED_INDIVIDUAL_SCOPE_AND_FINISHED)
{
    auto subscriber = std::make_shared<subscriber_mock>();
    command_handler cmd_handler(2);
    cmd_handler.subscribe(subscriber);

    std::string command_text("hello_world");
    std::string client_id = "client1";
    cmd_handler.add_command(client_id, command_text);
    cmd_handler.add_command(client_id, "{");
    cmd_handler.stop_handling_client(client_id);
    cmd_handler.stop();

    std::vector<std::string> expected;
    expected.push_back(command_text);
    ASSERT_TRUE(expected == subscriber->output());
}

TEST(COMMAND_HANDLER, CLIENT_INDIVIDUAL_SCOPE)
{
    auto subscriber = std::make_shared<subscriber_mock>();
    command_handler cmd_handler(5);
    cmd_handler.subscribe(subscriber);

    std::string command_text("hello_world");
    std::string client_id = "client1";
    cmd_handler.add_command(client_id, "{");
    cmd_handler.add_command(client_id, command_text);
    cmd_handler.add_command(client_id, "}");

    std::vector<std::string> expected;
    expected.push_back(command_text);
    ASSERT_TRUE(expected == subscriber->output());
}


TEST(COMMAND_HANDLER, MULTIPLE_NESTED_INDIVIDUAL_SCOPES)
{
    auto subscriber = std::make_shared<subscriber_mock>();
    command_handler cmd_handler(5);
    cmd_handler.subscribe(subscriber);

    std::string command1_text("hello_world");
    std::string command2_text("hello_world");
    std::string command3_text("hello_world");
    std::string client_id = "client1";

    cmd_handler.add_command(client_id, "{");
    cmd_handler.add_command(client_id, command1_text);

    cmd_handler.add_command(client_id, "{");
    cmd_handler.add_command(client_id, command2_text);
    cmd_handler.add_command(client_id, "}");

    cmd_handler.add_command(client_id, command3_text);
    cmd_handler.add_command(client_id, "}");

    std::vector<std::string> expected;
    expected.push_back(command1_text);
    expected.push_back(command2_text);
    expected.push_back(command3_text);
    ASSERT_TRUE(expected == subscriber->output());
}

TEST(COMMAND_HANDLER, FORCED_CLOSURE_INDIVIDUAL_SCOPE)
{
    auto subscriber = std::make_shared<subscriber_mock>();
    command_handler cmd_handler(2);
    cmd_handler.subscribe(subscriber);

    std::string command_text("hello_world");
    std::string client_id = "client1";
    cmd_handler.add_command(client_id, "{");
    cmd_handler.add_command(client_id, command_text);
    cmd_handler.stop_handling_client(client_id);

    std::vector<std::string> expected;
    ASSERT_TRUE(expected == subscriber->output());
}


TEST(COMMAND_HANDLER, MULTIPLE_CLIENTS_IN_COMMON_SCOPE)
{
    auto subscriber = std::make_shared<subscriber_mock>();
    command_handler cmd_handler(3);
    cmd_handler.subscribe(subscriber);

    std::string command_text1("hello_world1");
    std::string client_id1 = "client1";
    cmd_handler.add_command(client_id1, command_text1);

    std::string command_text2("hello_world2");
    std::string client_id2 = "client2";
    cmd_handler.add_command(client_id2, command_text2);

    std::string command_text3("hello_world3");
    std::string client_id3 = "client3";
    cmd_handler.add_command(client_id3, command_text3);


    cmd_handler.stop_handling_client(client_id1);
    cmd_handler.stop_handling_client(client_id2);
    cmd_handler.stop_handling_client(client_id3);
    cmd_handler.stop();

    std::vector<std::string> expected;
    expected.push_back(command_text1);
    expected.push_back(command_text2);
    expected.push_back(command_text3);
    ASSERT_TRUE(expected == subscriber->output());
}


TEST(COMMAND_HANDLER, MULTIPLE_CLIENTS_IN_DIFFERENT_SCOPES)
{
    auto subscriber = std::make_shared<subscriber_mock>();
    command_handler cmd_handler(3);
    cmd_handler.subscribe(subscriber);

    std::string command_text1("hello_world1");
    std::string client_id1 = "client1";
    cmd_handler.add_command(client_id1, command_text1);

    std::string command_text2("hello_world2");
    std::string client_id2 = "client2";
    cmd_handler.add_command(client_id2, command_text2);

    std::string command_text3("hello_world3");
    std::string client_id3 = "client3";
    cmd_handler.add_command(client_id3, "{");
    cmd_handler.add_command(client_id3, command_text3);

    std::string command_text4("hello_world4");
    cmd_handler.add_command(client_id2, command_text4);

    cmd_handler.stop_handling_client(client_id1);
    cmd_handler.stop_handling_client(client_id2);
    cmd_handler.stop_handling_client(client_id3);
    cmd_handler.stop();

    std::vector<std::string> expected;
    expected.push_back(command_text1);
    expected.push_back(command_text2);
    expected.push_back(command_text4);
    ASSERT_TRUE(expected == subscriber->output());
}


