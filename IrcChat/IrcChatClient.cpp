//
//  IrcChatClient.cpp
//  IrcChat
//
//  Created by Julius Higiro on 10/30/21.
//

#include <iostream>
#include "IrcChatClient.hpp"
#include "IrcMessage.h"

using namespace chat::irc;

const int IrcChatClient::RECONNECT_TIME = 30;

IrcChatClient::IrcChatClient(io_context& io_context) : mIoContext(io_context), mResetTimer(io_context)
{
    session = std::unique_ptr<Session>(new Session(io_context));
}

IrcChatClient::~IrcChatClient()
{
    disconnect();
}

void IrcChatClient::readHandler(const std::string& message, const error_code& ec)
{
    if (ec == error::eof || ec == error::connection_reset)
    {
        std::cout << "Connection Failed during read. Attempting reconnect in 30 seconds...\n";
        mResetTimer.expires_after(boost::asio::chrono::seconds(RECONNECT_TIME));
        mResetTimer.async_wait(boost::bind(&IrcChatClient::resetConnection, this));
    }
    else
    {
        //IrcMessage msg(message);
        // if message is a ping
        // session.asyncWrite("PONG \r\n");
        // if registration complete,
        // join channels
        std::cout << message << "\n";
    }
}

void IrcChatClient::writeHandler(const bool& isConnected)
{
    if(!isConnected)
    {
        std::cout << "Connection Failed during write. Attempting reconnect in 30 seconds...\n";
        mResetTimer.expires_after(boost::asio::chrono::seconds(RECONNECT_TIME));
        mResetTimer.async_wait(boost::bind(&IrcChatClient::resetConnection, this));
    }
}

void IrcChatClient::connectionHandler(const tcp::resolver::iterator& iterator, const error_code& ec)
{
    if (ec == error::eof || ec == error::connection_reset)
    {
        std::cout << "Connection Failed. Attempting reconnect in 30 seconds...\n";
        mResetTimer.expires_after(boost::asio::chrono::seconds(RECONNECT_TIME));
        mResetTimer.async_wait(boost::bind(&IrcChatClient::resetConnection, this));
    }
    else
    {
        std::cout << "Connected to: " << iterator->host_name() << " " << iterator->service_name() << "\n";
        std::string nickMsg = "NICK " + mNick + IrcMessage::IRC_MESSAGE_TERMINATION;
        std::string userMsg = "USER " + mUser + IrcMessage::IRC_MESSAGE_TERMINATION;
        session->asyncWrite(nickMsg);
        session->asyncWrite(userMsg);
        session->asyncRead(boost::bind(&IrcChatClient::readHandler, this, _1, _2));
    }
}

void IrcChatClient::establishConnection()
{
    session->asyncConnect(mHostName, mPortNumber, boost::bind(&IrcChatClient::connectionHandler, this, _1, _2));
}

void IrcChatClient::resetConnection()
{
    session.reset(new Session(mIoContext));
    establishConnection();
}

void IrcChatClient::sendMessage(const std::string& message)
{
    session->asyncWrite(message, boost::bind(&IrcChatClient::writeHandler, this, _1));
}

void IrcChatClient::joinChannel(const std::string& message)
{
    std::string tmpMsg = "JOIN " + message + IrcMessage::IRC_MESSAGE_TERMINATION;
    session->asyncWrite(tmpMsg, boost::bind(&IrcChatClient::writeHandler, this, _1));
}

void IrcChatClient::leaveChannel(const std::string& message)
{
    std::string tmpMsg = "PART " + message + IrcMessage::IRC_MESSAGE_TERMINATION;
    session->asyncWrite(tmpMsg, boost::bind(&IrcChatClient::writeHandler, this, _1));
}

void IrcChatClient::disconnect()
{
    session->close();
}

