//
//  ChatCommandHandler.hpp
//  IrcChat
//
//  Created by Julius Higiro on 10/31/21.
//

#pragma once

#include "IrcMessage.h"
#include <map>
#include <string>

namespace chat
{
    namespace irc
    {
        class ChatCommandHandler
        {
        public:
            typedef void (ChatCommandHandler::*FunctionPtr)(const IrcMessage&);
            
            ChatCommandHandler();
            virtual ~ChatCommandHandler();
            void handleNickUpdateMessage(const IrcMessage& ircmsg);
            void handleNoticeMessage(const IrcMessage& ircmsg);
            void handleJoinMessage(const IrcMessage& ircmsg);
            void handleLeaveMessage(const IrcMessage& ircmsg);
            void handlePrivMessage(const IrcMessage& ircmsg);
            void handleQuitMessage(const IrcMessage& ircmsg);
            void invokeCommandHandler(const IrcMessage& ircmsg);
            bool isNumericResponse(const std::string& response);
            void handleNumericReplies(const std::vector<std::string>& params, int reply);
            void prettyPrint(const std::vector<std::string>& params, int startPos);
            
        private:
            static const size_t NUMERIC_RESPONSE_LENGTH = 3;
            std::map<std::string, FunctionPtr> mCmdHandlerTable {
                { "PRIVMSG",    &ChatCommandHandler::handlePrivMessage             },
                { "NOTICE",     &ChatCommandHandler::handleNoticeMessage           },
                { "JOIN",       &ChatCommandHandler::handleJoinMessage             },
                { "PART",       &ChatCommandHandler::handleLeaveMessage            },
                { "NICK",       &ChatCommandHandler::handleNickUpdateMessage       },
                { "QUIT",       &ChatCommandHandler::handleQuitMessage             }
            };
        };
    }
}
