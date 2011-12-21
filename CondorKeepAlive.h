/***************************************************************
 *
 * Copyright (C) 2010 Red Hat, Inc.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.  You may
 * obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************/

#ifndef __CONDORKEEPALIVE_H__
#define __CONDORKEEPALIVE_H__

#include <string>
#include <fstream>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include "Logger.h"
//#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

namespace JobQueuePublisher {
  class CondorKeepAlive {
  public:
    // Constructor
    CondorKeepAlive();

    //Destructor
    ~CondorKeepAlive();

    bool startSession(std::string ipAddress, int port);

    bool startSession();

    // sendKeepAlive()
    void sendKeepAlive();

  protected:
    unsigned char* build_message();
  private:

    int sockfd_;
    struct sockaddr_in servaddr_;
    unsigned short kamessage_[12];
    std::string addressDir_;
    std::string address_;
    std::string port_;
    static const short DC_CHILDALIVE=60008;
    
  };
};

#endif // __CONDORKEEPALIVE_H__
