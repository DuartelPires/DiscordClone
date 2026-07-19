#include <iostream>
#include <memory>
#include <string>

#include "domain/serverState.cpp"
#include <grpcpp/grpcpp.h>
#include "helloworld.pb.h"
#include "helloworld.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

using namespace std;

class GreeterServiceImpl final : public Greeter::Service {
private:
    ServerState* currentState;

public:
    // Construtor
    explicit GreeterServiceImpl(ServerState* currentState) : currentState(currentState) {}

    Status SayHello(ServerContext* context, const HelloRequest* request,
                    HelloReply* reply) override {

        reply->set_message(currentState->getLatestUserId());
        return Status::OK;
    }
};

void RunServer(ServerState& currentState) {
    // Escuta todas as interfaces da rede na porta 50051
    string server_address("0.0.0.0:50051");
    GreeterServiceImpl service(&currentState);

    ServerBuilder builder;
    // Configura a porta sem autenticação (Insecure)
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Regista o serviço que criamos acima
    builder.RegisterService(&service);

    // Inicializa o servidor
    unique_ptr<Server> server(builder.BuildAndStart());
    cout << "Servidor rodando no endereço: " << server_address << endl;

    // Mantém o servidor ativo aguardando conexões
    server->Wait();
}

int main(int argc, char** argv) {
    ServerState currentState;
    RunServer(currentState);
    return 0;
}
