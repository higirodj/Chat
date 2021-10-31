//
//  IrcChatClient.hpp
//  IrcChat
//
//  Created by Julius Higiro on 10/30/21.
//

#pragma once
#include <stdio.h>
#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "Session.hpp"

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::system;

namespace chat
{
    namespace irc
    {
        class IrcChatClient
        {
        public:
            IrcChatClient(io_context& io_context);
            virtual ~IrcChatClient();
            /**
             * A handler for the asynchronous read operation.
             * Param 1: message - message.
             * Param 2: ec - error code.
             * Return none.
             */
            void readHandler(std::string& message, const error_code& ec);
            /**
             * A handler for the asynchronous write operation
             * Param 1: isConnected - bool flag that signifies connection.
             * Param 2: message - message.
             * Return none.
             */
            void writeHandler(const bool& isConnected, const std::string& message);
            /**
             * A handler for the asynchronous connection operation
             * Param 1: iterator - used to resolved hostname.
             * Param 2: ec - error code.
             * Return none.
             */
            void connectionHandler(const tcp::resolver::iterator& iterator, const error_code& ec);
            /**
             * Establishes an asynchronous TCP connection to the server.
             * Param none.
             * Return none.
             */
            void establishConnection();
            /**
             * Performs a connection reset.
             * Param none.
             * Return none.
             */
            void resetConnection();
            /**
             * Allows client to send a message using IRC commands
             * Param message - message.
             * Return none.
             */
            void sendMsg(const std::string& message);
            /**
             * Allows client to send a private message.
             * Param 1: msgtarget - message destination.
             * Param 2: message - message.
             * Return none.
             */
            void sendPrivMsg(const std::string& msgtarget, const std::string& message);
            /**
             * Allows client to send a ping message.
             * Param msgtarget - message destination.
             * Return none.
             */
            void sendPing(const std::string& msgtarget);
            /**
             * Allows client to send a pong message.
             * Param msgtarget - message destination.
             * Return none.
             */
            void sendPong(const std::string& msgtarget);
            /**
             * Allows the client to join a channel or multiple channels.
             * Param  chan - name of channel or list of channels.
             * Return none.
             */
            void joinChannel(const std::string& chan);
            /**
             * Allows the client to leave a channel or multiple channels.
             * Param chan - name of channel or list of channels.
             * Return none.
             */
            void leaveChannel(const std::string& chan);
            /**
             * Allows the client to quit the nework.
             * Param message - a message to the server.
             * Return none.
             */
            void quitNetwork(const std::string& message);
            /**
             * Disconnects client connection.
             * Param none.
             * Return none.
             */
            void disconnect();
            /**
             * Getter that returns user nick.
             * Param none.
             * Return nick.
             */
            std::string getNick() { return mNick;}
            
        private:
            io_context& mIoContext;
            std::unique_ptr<Session> session;
            steady_timer mResetTimer;
            std::string mNick = "kai";
            std::string mUser = "kai";
            std::string mUserMode = "8";
            std::string channel = "#test";
            const std::string mHostName = "localhost";
            const std::string mPortNumber = "6667";
            static const int RECONNECT_TIME;
        };
    }
}

