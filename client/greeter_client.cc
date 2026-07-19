#include <iostream>
#include <string>
#include <thread>

#include "ChatClient.cpp"

using namespace std;

int main(int argc, char** argv) {
    int userId;
    string alvo_conexao("127.0.0.1:50051");

    ChatClient chatClient(grpc::CreateChannel(alvo_conexao, grpc::InsecureChannelCredentials()));
    
    string user;
    cout << "Como te chamas? ";
    getline(cin, user);
    
    userId = chatClient.EnviarUserId(user);
    if (userId == -1) {
        return 1;
    }
    
    cout << "Conectado! O teu userId é: " << userId << endl;
    cout << "Podes começar a escrever mensagens. (Escreve 'sair' para terminar)" << endl;
    
    //inicia thread para ler mensagens
    thread readerThread(&ChatClient::LerMensagens, &chatClient, userId);
    
    //loop para enviar mensagens
    string text;
    while (true) {
        cout << "> " << flush;
        getline(cin, text);
        if (text == "sair" || text == "exit") {
            break;
        }
        if (!text.empty()) {
            chatClient.EnviarMensagem(userId, text);
        }
    }

    readerThread.detach();

    return 0;
}
