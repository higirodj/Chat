//
//  IrcChatClient.hpp
//  IrcChat
//
//  Created by Julius Higiro on 10/30/21.
//
// https://tools.ietf.org/html/rfc1459 Internet Relay Chat Protocol
// https://tools.ietf.org/html/rfc2810 Internet Relay Chat:Architecture
// https://tools.ietf.org/html/rfc2811 Internet Relay Chat:Channel Management
// https://tools.ietf.org/html/rfc2812 Internet Relay Chat:Client Protocol
// https://tools.ietf.org/html/rfc2813 Internet Relay Chat:Server Protocol


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
            void readHandler(const std::string& message, const error_code& ec);
            void writeHandler(const bool& isConnected);
            void connectionHandler(const tcp::resolver::iterator& iterator, const error_code& ec);
            void establishConnection();
            void resetConnection();
            void sendMessage(const std::string& message);
            void joinChannel(const std::string& message);
            void leaveChannel(const std::string& message);
            void disconnect();
            
        private:
            io_context& mIoContext;
            std::unique_ptr<Session> session;
            steady_timer mResetTimer;
            std::string mNick = "kai";
            std::string mUser = "kai 8 * :kai";
            const std::string mHostName = "127.0.0.1";
            const std::string mPortNumber = "6667";
            static const int RECONNECT_TIME;
        };
    }
}

