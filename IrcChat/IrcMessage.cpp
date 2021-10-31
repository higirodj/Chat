//
//  IrcMessage.cpp
//  IrcChat
//
//  Created by Julius Higiro on 10/30/21.
//

#include "IrcMessage.h"

using namespace chat::irc;

const std::string IrcMessage::IRC_MESSAGE_TERMINATION = "\r\n";
const std::string IrcMessage::IRC_WILD_CARD_SYMBOL = "*";
const std::string IrcMessage::IRC_PREFIX_SYMBOL = ":";
const std::string IrcMessage::IRC_AT_SYMBOL = "@";
const std::string IrcMessage::IRC_BANG_SYMBOL = "!";


IrcMessage::IrcMessage() {}

IrcMessage::~IrcMessage() {}

std::vector<std::string> IrcMessage::split(const std::string& msg, char sep)
{
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;
    while ((end = msg.find(sep, start)) != std::string::npos)
    {
        tokens.push_back(msg.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(msg.substr(start));
    return tokens;
}

void IrcMessage::parsePrefix(const std::string& msg)
{
    std::vector<std::string> tokens;
    
    mPrefix.fullPrefix = msg.substr(1, msg.find(" ") - 1);

    if (mPrefix.fullPrefix.find(IRC_AT_SYMBOL) != std::string::npos)
    {
        tokens = split(mPrefix.fullPrefix, AT_DELIMITER);
        mPrefix.host = tokens.at(1);
        if (tokens.at(0).find(IRC_BANG_SYMBOL) != std::string::npos)
        {
            tokens = split(tokens.at(0), BANG_DELIMITER);
            mPrefix.nick = tokens.at(0);
            mPrefix.user = tokens.at(1);
        }
    }
}

void IrcMessage::parseMessage(std::string& msg)
{
    if (!msg.empty())
    {
        size_t pos = 0;
        // Parse the message prefix.
        if (*msg.begin() == COLON_DELIMITER)
        {
            parsePrefix(msg);
            pos = static_cast<int>(mPrefix.fullPrefix.size() + 2);
        }
        
        // Parse the message command.
        auto endPos = msg.find(" ", pos);
        if (endPos != std::string::npos)
        {
            mCommand = msg.substr(pos, endPos-pos);
            std::transform(mCommand.begin(), mCommand.end(),mCommand.begin(), ::toupper);
            msg = msg.substr(endPos+1);
        }
        else
        {
            // Take the rest of the string. Assumes there are no parameters.
            mCommand = msg.substr(pos);
            msg = "";
        }// Message is malformed if it doesn't include a command.
        
        // Parse the message parameters.
        if (!msg.empty())
        {
            if (*msg.begin() == COLON_DELIMITER)
            {
                mParameters.push_back(msg.substr(1));
            }
            else
            {
                size_t pos1 = 0, pos2 = 0;
                while ((pos2 = msg.find(" ", pos1)) != std::string::npos)
                {
                    if (*(msg.begin()+pos1) == COLON_DELIMITER)
                    {
                        ++pos1;
                        break;
                    }
                    mParameters.push_back(msg.substr(pos1, pos2 - pos1));
                    pos1 = pos2 + 1;
                }
                pos1 = (*(msg.begin()+pos1) == COLON_DELIMITER)?pos1+1 : pos1;
                mParameters.push_back(msg.substr(pos1));
            }
        }
    }
}

const IrcMessage::Prefix IrcMessage::IrcMessage::getPrefix() const
{
    return mPrefix;
}

const std::string IrcMessage::IrcMessage::getCommand() const
{
    return mCommand;
}

const std::vector<std::string> IrcMessage::getParameters() const
{
    return mParameters;
}
