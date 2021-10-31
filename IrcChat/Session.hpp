//
//  Session.h
//  IrcChat
//
//  Created by Julius Higiro on 10/30/21.
//
#pragma once

#include <string>
#include <queue>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::system;

namespace chat
{
    namespace irc
    {
        // Session represents the context for a single tcp client connection.
        class Session
        {
        public:
            Session(io_context& io_context);
            virtual ~Session();
            /**
             * Performs an asynchronous connection to the server.
             * Param 1: The hostname of listening server.
             * Param 2: The port number of listening server.
             * Param 3: A handler invoked during an asynchronous Session.
             * Return none.
             */
            void asyncConnect(const std::string& host,
                              const std::string& port,
                              const std::function<void(const tcp::resolver::iterator &, const error_code &)> & handler = {});
            /**
             * Sets the read handler for the asynchronous read operation.
             * Param 1: A handler invoked during an asynchronous read.
             * Return none.
             */
            void asyncRead(const std::function<void(std::string &, const error_code &)> & handler);
            /**
             * Performs an asynchronous write of messages to the server.
             * Param 1: A message to send to the server.
             * Param 2: A handler invoked during an asynchronous write.
             * Return none.
             */
            void asyncWrite(const std::string& message, const std::function<void(const bool &, const std::string &)> & handler = {});
            /**
             * Closes the socket
             * Param none.
             * Return none.
             */
            void close();

        private:
            io_context& mIoContext;
            tcp::resolver mResolver;
            tcp::socket mSocket;
            std::queue<std::string> mMessageQueue;
            boost::asio::streambuf mReadBuffer;
            std::function<void(std::string &, const error_code &)> mReadHandler;
            bool mIsResolved;
            bool mIsConnected;
            /**
             * Performs an asynchronous connection to the server.
             * Param 1: Resolver iterator to resolve hostname address.
             * Param 2: A completion handler that is invoked when the async operation is completed.
             * Return none.
             */
            void makeConnect(tcp::resolver::iterator iterator,
                            const std::function<void(const tcp::resolver::iterator &, const error_code &)> & handler = {});
            /**
             * Performs an asynchronous read  operation to receive messages from the server.
             * Param none.
             * Return none.
             */
            void makeRead();
            /**
             * Performs an asynchronous write operation that sends messages to the server.
             * Param none.
             * Return none.
             */
            void makeWrite();
        };
    }
}




