#pragma once
#include <iostream>
#include <optional>
#include <memory>
#include <string>
#include <vector>
#include <thread>

#include "domain/serverState.cpp"
#include <grpcpp/grpcpp.h>
#include "chat.pb.h"
#include "chat.grpc.pb.h"

using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;

using chat::ChatService;
using chat::SendRequest;
using chat::SendResponse;
using chat::UserIdRequest;
using chat::UserIdResponse;
using chat::ChatMessage;

using namespace std;

class ChatServiceImpl final : public ChatService::Service {
private:
    ServerState* currentState;

public:
    //construtor
    explicit ChatServiceImpl(ServerState* currentState) : currentState(currentState) {}

    Status EnviarMensagem(ServerContext* context, const SendRequest* request,
                    SendResponse* reply) override {
        
        Message newMsg(request->message().user_id(), request->message().user_text());
        currentState->addMessage(newMsg);

        reply->set_success(true);
        reply->set_error_msg("");
        return Status::OK;
    }

    Status EnviarUserId(ServerContext* context, const UserIdRequest* request,
                    UserIdResponse* reply) override {

        reply->set_user_id(currentState->getLatestUserId());
        return Status::OK;
    }

    Status LerMensagens(ServerContext* context, const UserIdRequest* request,
                    ServerWriter<ChatMessage>* writer) override {

        int readerId = -1;

        try {
          readerId = stoi(request->name());
        } catch(...) {
        }

        size_t lastReadIndex = currentState->getMessagesCount();

        auto isCancelled = [context]() { return context->IsCancelled(); };

        while (!context->IsCancelled()) {
          vector<Message> newMessages = currentState->getMessagesFromIndex(lastReadIndex, isCancelled);

          if (newMessages.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
          }

          for (Message& msg : newMessages) {
            if(msg.getUserId() == readerId){
              continue;
            }

            ChatMessage chatMsg;
            chatMsg.set_user_id(msg.getUserId());
            chatMsg.set_user_text(msg.getUserText());
 
            if (!writer->Write(chatMsg)) {
                return Status::OK;
            }
          }
          lastReadIndex += newMessages.size();
        }
        
        return Status::OK;
    }
};
