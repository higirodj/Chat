//
//  main.cpp
//  IrcChat
//
//  Created by Julius Higiro on 10/30/21.
//

#include <iostream>
#include <exception>
#include <thread>
#include <boost/asio.hpp>
#include "Session.hpp"

using namespace chat::irc;

int main(int argc, const char * argv[]) {
    try
    {
        io_context io_context;
        // Prevent the io context's run loop from ending before async calls.
        io_context::work work(io_context);
        // Run io context event processing loop on another thread.
        std::thread io_thread([&io_context]() { io_context.run(); });

        Session ss(io_context);

        std::string hostname("localhost");
        std::string port("6667");

        ss.asyncConnect(hostname, port,
            [&ss](const tcp::resolver::iterator& iterator, const error_code& ec)
        {
            std::cout << "Connected to: " << iterator->host_name() << std::endl;
            ss.asyncWrite("NICK kai\r\n");
            ss.asyncWrite("USER kai 8 * :kai\r\n");
            ss.asyncRead([](const std::string & read, const error_code& ec)
            {
                std::cout << read << "\n";
            });
        });
    

        ss.close();
        io_thread.join();
    }
    catch (std::exception & e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
