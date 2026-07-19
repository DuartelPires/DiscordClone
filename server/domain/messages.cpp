#pragma once
#include <iostream>
#include <string>

using namespace std;

class Message {
  private:
    int userId;
    string userText;

  public:
    Message(int userId, string userText)
        : userId(userId), userText(userText){
    }

    int getUserId(){
      return userId;
    }

    string getUserText(){
      return userText;
    }
};
