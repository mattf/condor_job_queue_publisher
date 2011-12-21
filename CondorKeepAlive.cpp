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

#include "CondorKeepAlive.h"

#include <iostream>

#include <syslog.h>

namespace JobQueuePublisher {
  CondorKeepAlive::CondorKeepAlive() {
    FILE *fp = NULL;

    fp = popen("/usr/bin/condor_config_val LOG 2>&1", "r" );
    char buff[50];
    
    // Get the default value for CONCURRENCY_LIMIT_DEFAULT
    if (fp != NULL) {
		if (!fgets( buff, sizeof buff, fp )) {
			syslog(LOG_INFO, "fgets on condor_config_val pipe did not return data");
		}
      std::string response = buff;
      if ( response.find("Not defined") != std::string::npos ) {
	std::string msg;
	msg = "LOG - Not set. Using default /var/log/condor";
//	JobQueuePublisher::Logger::getInstance().log(JobQueuePublisher::Logger::INFO, msg);
	syslog(LOG_INFO, msg.c_str());
	addressDir_ = "/var/log/condor";
      } else if ( response.find("Can't") != std::string::npos ) {
	std::string msg;
	msg = "LOG - Not set. Using default /var/log/condor";
//	JobQueuePublisher::Logger::getInstance().log(JobQueuePublisher::Logger::INFO, msg);
	syslog(LOG_INFO, msg.c_str());
	addressDir_ = "/var/log/condor";
      } else {
//	JobQueuePublisher::Logger::getInstance().log(JobQueuePublisher::Logger::INFO, std::string("Reading MASTER_ADDRESS_FILE from directory  = ").append(response));
	syslog(LOG_INFO, std::string("Reading MASTER_ADDRESS_FILE from directory  = ").append(response).c_str());
	boost::trim_if(response, boost::is_any_of("\n"));
	boost::trim(response);
	addressDir_ = response;
      }
      pclose( fp );
    } else {
//      JobQueuePublisher::Logger::getInstance().log(JobQueuePublisher::Logger::INFO, std::string("Cannot exec condor_config_val ...using default dir /var/log/condor"));
      syslog(LOG_INFO, "Cannot exec condor_config_val ...using default dir /var/log/condor");
      addressDir_ = "/var/log/condor";
    }

    addressDir_ = "/var/log/condor";
    addressDir_.append("/.master_address");

    std::ifstream file; // indata is like cin
    std::string line;
    
//    JobQueuePublisher::Logger::getInstance().log(JobQueuePublisher::Logger::INFO, std::string("Opening file: ").append(addressDir_));
    syslog(LOG_INFO, std::string("Opening file: ").append(addressDir_).c_str());
    std::cout << "File: " << addressDir_ << std::endl;
    file.open(addressDir_.c_str()); // opens the file
    std::string inputline;

    while(getline(file, line)) { 

      // FORMAT for .master_address that we want to look for. <192.168.70.159:33858>
      if ( line.find("<")  != std::string::npos ) {
	std::vector<std::string> key_val;
	boost::split(key_val, line, boost::is_any_of(":"));
	address_ = key_val[0];
	port_    = key_val[1];

	address_ = boost::trim_copy_if( address_, boost::is_any_of("<>") );
	port_= boost::trim_copy_if( port_, boost::is_any_of("<>") );
	boost::trim(address_);
	boost::trim(port_);
	break; // we are done.
      }
    }

    file.close();

    short pid = getpid();
    // KeepAlive message
    kamessage_[0] = 0;
    kamessage_[1] = 0;
    kamessage_[2] = 0;
    kamessage_[3] = htons((short)60008);
    kamessage_[4] = 0;
    kamessage_[5] = 0;
    kamessage_[6] = 0;
    kamessage_[7] = htons(pid);
    kamessage_[8] = 0;
    kamessage_[9] = 0;
    kamessage_[10] = 0;
    kamessage_[11] = htons((short)30);
 
    //sprintf(kamessage_, "%8d%8d%8d", htons(DC_CHILDALIVE), htons((short)getpid()), htons((short)15));
    sockfd_=socket(AF_INET,SOCK_DGRAM,0);
    bzero(&servaddr_,sizeof(servaddr_));
    servaddr_.sin_family = AF_INET;
    servaddr_.sin_addr.s_addr=inet_addr(address_.c_str());
    std::cout << "Address = " << address_.c_str() << std::endl;
    std::cout << "Port = " << port_.c_str() << std::endl;
    int port = atoi(port_.c_str());
    servaddr_.sin_port=htons(port);

  }

  CondorKeepAlive::~CondorKeepAlive() {
    
  }


  void CondorKeepAlive::sendKeepAlive() {

    //sendto(sockfd_,message.c_str(),sizeof(kamessage_),0,
    sendto(sockfd_,kamessage_,sizeof(kamessage_),0,
	   (struct sockaddr *)&servaddr_,sizeof(servaddr_));
    
  }

};
