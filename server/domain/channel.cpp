#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Channel {
  private:
    string channelName;
    string channelDescription;
    vector<Message> allMessages;

  public:
    Message(string channelName, string channelDescriptionn)
        : channelName(channelName), channelDescription(channelDescription){
    }
};

