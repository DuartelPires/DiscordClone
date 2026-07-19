#include <iostream>
#include <vector>
#include <string>

#include "messages.cpp"

using namespace std;

class ServerState {
  public:
    int latestUserId = 0;
    vector<Message> allMessages;

    void addMessage(Message newMessage){
      allMessages.push_back(newMessage);
    }

    int getLatestUserId(){
      latestUserId++;
      return latestUserId;
    }
};
