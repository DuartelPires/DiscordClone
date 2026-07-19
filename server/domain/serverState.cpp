#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>
#include <functional>

#include "messages.cpp"

using namespace std;

class ServerState {
  private:
    int latestUserId = 0;
    vector<Message> allMessages;
    mutex mtx;
    condition_variable cv;

  public:

    void addMessage(Message newMessage){
      unique_lock<mutex> lock(mtx);
      allMessages.push_back(newMessage);
      cv.notify_all();
    }

    int getLatestUserId(){
      unique_lock<mutex> lock(mtx);
      latestUserId++;
      return latestUserId;
    }

    string getLatestMessage() {
      unique_lock<mutex> lock(mtx);
      if (allMessages.empty()) {
          return "";
      }
      return allMessages.back().getUserText();
    }
    
    //bloqueia ate haver novas msgs
    vector<Message> getMessagesFromIndex(size_t lastReadIndex, std::function<bool()> isCancelled) {
      while (true) {
        if (isCancelled()) {
            return {};
        }

        unique_lock<mutex> lock(mtx);

        //se o wait_for falha (retorna false porque o tempo esgotou)
        if (!cv.wait_for(lock, chrono::milliseconds(500), [this, lastReadIndex] {
            return allMessages.size() > lastReadIndex;
        })) {
            continue;
        }

        vector<Message> newMessages;

        for (size_t i = lastReadIndex; i < allMessages.size(); i++) {
            newMessages.push_back(allMessages[i]);
        }

        return newMessages;
      }
    }
 
    size_t getMessagesCount() {
      unique_lock<mutex> lock(mtx);
      return allMessages.size();
    }
};
