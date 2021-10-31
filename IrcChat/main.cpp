//
//  main.cpp
//  IrcChat
//
//  Created by Julius Higiro on 10/30/21.
//

#include <iostream>
#include <exception>
#include <thread>
#include "IrcChatClient.hpp"
using namespace chat::irc;

int main(int argc, const char * argv [])
{
    try
    {
        io_context io_context;
        // Prevent the io context's run loop from ending before async calls.
        io_context::work work(io_context);
        // Run io context event processing loop on another thread.
        std::thread io_thread([&io_context]() { io_context.run(); });

        IrcChatClient client(io_context);
        client.establishConnection();
        std::string input;
        while (input != "quit")
        {
            std::getline(std::cin, input);
            if (!input.empty() && input != "quit")
            {
                client.sendPrivMsg("#test", input);
            }
        }
        if (input == "quit")
        {
            client.quitNetwork("I'm out");
        }
        client.disconnect();
        io_thread.join();
    }
    catch (std::exception & e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}

