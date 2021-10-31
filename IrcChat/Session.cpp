//
//  Session.cpp
//  IrcChat
//
//  Created by Julius Higiro on 10/30/21.
//

#include "Session.hpp"
#include "IrcMessage.h"

using namespace chat::irc;
using namespace boost::system;
using namespace boost::placeholders;

Session::Session(io_context& io_context)
    : mIoContext(io_context),
    mResolver(io_context),
    mSocket(io_context),
    mMessageQueue(),
    mReadBuffer(),
    mReadHandler(),
    mIsResolved(false),
    mIsConnected(false) {}

Session::~Session() { close(); }

void Session::asyncConnect(const std::string& host,
    const std::string& port,
    const std::function<void(const tcp::resolver::iterator& iterator, const error_code& error)> & handler)
{
    mResolver.async_resolve({ host, port },
        [this, handler](const error_code& error, tcp::resolver::iterator iterator)
    {
        switch (error.value())
        {
            case errc::success:
            {
                mIsResolved = true;
                makeConnect(iterator, handler);
                break;
            }
            default:
            {
                // resolve failed.
            }
        }
    });
}

void Session::close()
{
    mIoContext.post([this]() { mSocket.close(); });
}

void Session::asyncRead(const std::function<void(std::string &, const error_code &)>& handler)
{
    mReadHandler = handler;
}

void Session::asyncWrite(const std::string& message,
                             const std::function<void(const bool &, const std::string &)>& handler)
{
    mIoContext.post([this, message, handler]()
    {
        bool empty = mMessageQueue.empty();
        mMessageQueue.push(message);
        if (empty)
        {
            makeWrite();
        }
        if (handler)
        {
            handler(mIsConnected, message);
        }
    });
}

void Session::makeConnect(tcp::resolver::iterator iterator,
    const std::function<void(const tcp::resolver::iterator &, const error_code &)> & handler)
{
    async_connect(mSocket, iterator, [this, handler](const error_code& error, tcp::resolver::iterator iterator)
    {
        switch (error.value())
        {
            case errc::success:
            {
                mIsConnected = true;

                makeRead();

                if (!mMessageQueue.empty())
                {
                    makeWrite();
                }

                if (handler)
                {
                    handler(iterator, error);
                }
                break;
            }
            default:
            {
                mIsConnected = false;
                if (handler)
                {
                    handler(iterator, error);
                }
            }
        }
    });
}

void Session::makeRead()
{
    async_read_until(mSocket, mReadBuffer, IrcMessage::IRC_MESSAGE_TERMINATION,
        [this](const error_code& error, std::size_t bytes_transferred)
    {
        switch (error.value())
        {
            case errc::success:
            {
                  //Verify async_read_until read beyond the delimiter)
                  if(mReadBuffer.size() > bytes_transferred)
                  {
                      // Extract up to the first delimiter.
                      std::string read{
                        buffers_begin(mReadBuffer.data()),
                        buffers_begin(mReadBuffer.data()) + bytes_transferred
                          - IrcMessage::IRC_MESSAGE_TERMINATION.size()};
                      mReadBuffer.consume(bytes_transferred);
                      if (mReadHandler)
                      {
                          mReadHandler(read, error);
                      }
                  }
                else
                {
                    std::string read((std::istreambuf_iterator<char>(&mReadBuffer)),
                        std::istreambuf_iterator<char>());
                    if (mReadHandler)
                    {
                        mReadHandler(read, error);
                    }
                }
                makeRead();
                break;
            }
            default:
            {
                mIsConnected = false;
            }
        }
    });
}

void Session::makeWrite()
{
    if (mIsResolved && mIsConnected)
    {
        auto msg = mMessageQueue.front();
        async_write(mSocket, buffer(msg.c_str(), msg.size()),
            [this](const error_code & error, std::size_t)
        {
            switch (error.value())
            {
                case errc::success:
                {
                    mMessageQueue.pop();
                    if (!mMessageQueue.empty())
                    {
                        makeWrite();
                    }
                    break;
                }
                default:
                {
                    mIsConnected = false;
                }
            }
        });
    }
}
