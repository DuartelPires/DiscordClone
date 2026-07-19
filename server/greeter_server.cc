#include <iostream>
#include <memory>
#include <string>

#include "ChatServiceImpl.cpp"
#include <grpcpp/grpcpp.h>
#include "chat.pb.h"
#include "chat.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
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

void RunServer(ServerState& currentState) {
    //ouve todas as interfaces da rede na porta 50051
    string server_address("0.0.0.0:50051");
    ChatServiceImpl service(&currentState);

    ServerBuilder builder;
    
    //aumenta o limite de threads para suportar dezenas de clientes em streams síncronos
    grpc::ResourceQuota quota;
    quota.SetMaxThreads(500);
    builder.SetResourceQuota(quota);

    //insecure
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    //inicia o servidor
    unique_ptr<Server> server(builder.BuildAndStart());
    cout << "Servidor rodando no endereço: " << server_address << endl;

    //mantem o server ativo
    server->Wait();
}

int main(int argc, char** argv) {
    ServerState currentState;
    RunServer(currentState);
    return 0;
}
