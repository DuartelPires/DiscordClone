#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "helloworld.pb.h"
#include "helloworld.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

using namespace std;

class GreeterClient {
public:
    GreeterClient(shared_ptr<Channel> channel)
        : stub_(Greeter::NewStub(channel)) {}

 
    int SayHello(const string& user) {
        HelloRequest request;
        request.set_name(user);

        HelloReply reply;
        ClientContext context;

        // Executa a chamada RPC
        Status status = stub_->SayHello(&context, request, &reply);

        if (status.ok()) {
            return reply.message();
        } else {
            cout << "Erro " << status.error_code() << ": " << status.error_message() << endl;
            return -1;
        }
    }

private:
    unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char** argv) {
    int userId;
    // Endereço IP e porta para conexão
    string alvo_conexao("127.0.0.1:50051");

    // Instancia o cliente criando o canal de comunicação inseguro
    GreeterClient greeter(grpc::CreateChannel(alvo_conexao, grpc::InsecureChannelCredentials()));
    
    string user("world");
    userId = greeter.SayHello(user);

 
    cout << "My userId: " << userId << endl;

    return 0;
}
