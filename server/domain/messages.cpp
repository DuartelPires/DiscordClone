#include <iostream>
#include <string>

using namespace std;

class Message {
  public:
    int userId;
    string userText;

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
