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
            void readHandler(std::string& message, const error_code& ec);
            void writeHandler(const bool& isConnected, const std::string& message);
            void connectionHandler(const tcp::resolver::iterator& iterator, const error_code& ec);
            void establishConnection();
            void resetConnection();
            void sendMsg(const std::string& message);
            void sendPrivMsg(const std::string& msgtarget, const std::string& message);
            void sendPing(const std::string& message);
            void sendPong(const std::string& message);
            void joinChannel(const std::string& message);
            void leaveChannel(const std::string& message);
            void leaveNetwork(const std::string& message);
            void disconnect();
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

