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
