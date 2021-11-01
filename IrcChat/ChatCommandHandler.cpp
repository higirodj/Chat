//
//  ChatCommandHandler.cpp
//  IrcChat
//
//  Created by Julius Higiro on 10/31/21.
//

#include <iostream>
#include "ChatCommandHandler.hpp"

using namespace chat::irc;

ChatCommandHandler::ChatCommandHandler() {}

ChatCommandHandler::~ChatCommandHandler() {}

void ChatCommandHandler::handleNickUpdateMessage(const IrcMessage &ircmsg)
{
   
}

void ChatCommandHandler::handleNoticeMessage(const IrcMessage &ircmsg)
{

}

void ChatCommandHandler::handleJoinMessage(const IrcMessage &ircmsg)
{
  
}

void ChatCommandHandler::handleLeaveMessage(const IrcMessage &ircmsg)
{
 
}

void ChatCommandHandler::handlePrivMessage(const IrcMessage &ircmsg)
{
    if (ircmsg.getCommand()== "PRIVMSG")
    {
        std::cout << ircmsg.getPrefix().nick << " ";
        /*for (auto param : ircmsg.getParameters())
        {
            std::cout << param << " ";
        }
        std::cout << "\n";*/
        std::cout << ircmsg.getParameters().back() << "\n";
    }
}

void ChatCommandHandler::handleQuitMessage(const IrcMessage &ircmsg)
{
    
}

void ChatCommandHandler::prettyPrint(const std::vector<std::string>& params, int startPos)
{
    std::vector<std::string>::const_iterator iter = params.begin();
    std::advance(iter, startPos);
 
    for(;iter != params.end(); ++iter)
    {
        std::cout << *iter << " ";
    }
    std::cout << "\n";
}

bool ChatCommandHandler::isNumericResponse(const std::string& response)
{
    if(response.length() == NUMERIC_RESPONSE_LENGTH)
    {
    return !response.empty() && std::find_if(response.begin(),
        response.end(), [](unsigned char c) { return !std::isdigit(c); }) == response.end();
        
    }
    return false;
}

void ChatCommandHandler::invokeCommandHandler(const IrcMessage &ircmsg)
{
    std::string key = ircmsg.getCommand();
    std::map<std::string, ChatCommandHandler::FunctionPtr>::iterator iter = mCmdHandlerTable.find(key);
    if (!key.empty() && iter != mCmdHandlerTable.end())
    {
        ChatCommandHandler::FunctionPtr &fp = mCmdHandlerTable[ircmsg.getCommand()];
        (this->*fp)(ircmsg);
    }
    else
    {
        if (isNumericResponse(key))
        {
            prettyPrint(ircmsg.getParameters(),1);
        }
    }
}
