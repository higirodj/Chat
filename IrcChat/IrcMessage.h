//
//  IrcMessage.h
//  IrcChat
//
//  Created by Julius Higiro on 10/30/21.
//
#pragma once

#include <regex>
#include <string>

namespace chat {
    namespace irc {
        class IrcMessage
        {
        public:
            static const std::string IRC_MESSAGE_TERMINATION;
            struct Prefix
            {
                std::string rawPrefix;
                std::string nick;
                std::string user;
                std::string host;
                
                void reset()
                {
                    rawPrefix = "";
                    nick = "";
                    user = "";
                    host = "";
                }
            };
            
            IrcMessage();
            virtual ~IrcMessage();
            
            /**
             * Splits message into tokens based on delimeter.
             * Param:  msg the message that is tokenized..
             * Param 2: sep delimeter.
             */
            std::vector<std::string> split(const std::string& msg, char sep);
            /**
             * Parses the raw irc message into its components.
             * Param:  none.
             */
            void parseMessage(std::string& msg);
            /**
             * Parses the prefix into its components.
             * Param:  none.
             */
            void parsePrefix(const std::string& msg);
            /**
             * Returns the message prefix.
             * Param: none.
             */
            const Prefix getPrefix() const;
            
            /**
             * Returns the message command.
             * Param: none.
             */
            const std::string getCommand() const;
            
            /**
             * Returns the message parameters.
             * Param: none.
             */
            const std::vector<std::string> getParameters() const;
            
        private:
            std::string mCommand;
            std::vector<std::string> mParameters;
            Prefix mPrefix;
            const std::string mIrcMessage;
            static const char BANG_DELIMITER = 0x21;
            static const char AT_DELIMITER = 0x40;
            static const char COLON_DELIMITER = 0x3A;
        };    
    }
}
