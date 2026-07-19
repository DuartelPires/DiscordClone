#pragma once
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "chat.pb.h"
#include "chat.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::Status;

using chat::ChatService;
using chat::SendRequest;
using chat::SendResponse;
using chat::UserIdRequest;
using chat::UserIdResponse;
using chat::ChatMessage;

using namespace std;

class ChatClient {
public:
    ChatClient(shared_ptr<Channel> channel)
        : stub_(ChatService::NewStub(channel)) {}

    int EnviarUserId(const string& user) {
        UserIdRequest request;
        request.set_name(user);

        UserIdResponse reply;
        ClientContext context;

        Status status = stub_->EnviarUserId(&context, request, &reply);

        if (status.ok()) {
            return reply.user_id();
        } else {
            cout << "Erro " << status.error_code() << ": " << status.error_message() << endl;
            return -1;
        }
    }

    void EnviarMensagem(int userId, const string& text) {
        SendRequest request;
        request.mutable_message()->set_user_id(userId);
        request.mutable_message()->set_user_text(text);

        SendResponse reply;
        ClientContext context;

        Status status = stub_->EnviarMensagem(&context, request, &reply);

        if (!status.ok() || !reply.success()) {
            cout << "Erro a enviar mensagem: " << status.error_message() << " " << reply.error_msg() << endl;
        }
    }

    void LerMensagens(int myUserId) {
        UserIdRequest request; 
        request.set_name(to_string(myUserId));
 
        ClientContext context;
        unique_ptr<ClientReader<ChatMessage>> reader(stub_->LerMensagens(&context, request));
 
        ChatMessage msg;
        while (reader->Read(&msg)) {
          cout << goToStart
               << cleanCurrentLine
               << "[User " << msg.user_id() << "]: " << msg.user_text() << "\n" 
               << promptChat << flush;
        }
 
        Status status = reader->Finish();
        if (!status.ok()) {
            cout << "Erro no stream de leitura: " << status.error_message() << endl;
        }
    }

private:
    unique_ptr<ChatService::Stub> stub_;
    const string goToStart         = "\r";
    const string cleanCurrentLine  = "\033[K";
    const string promptChat        = "> ";
};
