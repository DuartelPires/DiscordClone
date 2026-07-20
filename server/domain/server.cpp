#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "channel.cpp"

using namespace std;

class Server {
  private:
    string serverName;
    string joinServerCode;
    vector<Channel> allServerChannels;

  public:
    Message(string serverName)
        : serverName(serverName){
    }
};

