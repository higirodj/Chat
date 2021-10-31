//
//  IrcMessage.cpp
//  IrcChat
//
//  Created by Julius Higiro on 10/30/21.
//

#include "IrcMessage.h"

using namespace chat::irc;

const std::string IrcMessage::IRC_MESSAGE_TERMINATION = "\r\n";

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
    
    mPrefix.rawPrefix = msg.substr(1, msg.find(" ") - 1);

    if (mPrefix.rawPrefix.find("@") != std::string::npos)
    {
        tokens = split(mPrefix.rawPrefix, AT_DELIMITER);
        mPrefix.host = tokens.at(1);
    }
    
    if (tokens.at(0).find("!") != std::string::npos)
    {
        tokens = split(tokens.at(0), BANG_DELIMITER);
        mPrefix.nick = tokens.at(0);
        mPrefix.user = tokens.at(1);
    }
}

void IrcMessage::parseMessage(std::string& msg)
{
    mPrefix.reset();
    std::string command;
    std::vector<std::string> parameters;
    int pos = 0;
    // Message starts with prefix. Otherwise, it starts with a command.
    if (*msg.begin() == COLON_DELIMITER)
    {
        parsePrefix(msg);
        pos = static_cast<int>(mPrefix.rawPrefix.size() + 2);
    }
    command = msg.substr(pos, msg.find(" "));
    std::transform(command.begin(), command.end(),command.begin(), ::toupper);
    mCommand = command;
    (msg.substr(pos).find(" ") != std::string::npos) ?
    msg = msg.substr(msg.find(" ")+1) : msg = "";
    /* msg contains the parameter portion of the message. Otherwise, it doesn't have parameters.
     Ex. msg --> JOIN #bar #club #food
     parameters: #bar #club #food
     */
    if (!msg.empty())
    {
        if (*msg.begin() == COLON_DELIMITER)
        {
            parameters.push_back(msg.substr(1));
        }
        else
        {
            // TODO: find rules for parameters. Remove tags
            // #bar #club #food
            size_t pos1 = 0, pos2;
            while ((pos2 = msg.find(" ", pos1)) != std::string::npos)
            {
                parameters.push_back(msg.substr(pos1, pos2 - pos1));
                pos1 = pos2 + 1;
                if (msg.substr(pos1, 1) == ":")
                {
                    parameters.push_back(msg.substr(pos1 + 1));
                    break;
                }
            }
            parameters.push_back(msg.substr(pos1));
        }
    }
    mParameters = parameters;
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
