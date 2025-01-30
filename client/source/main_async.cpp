#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include <grpcpp/alarm.h>
#include "proto/helloworld.pb.h"
#include "proto/helloworld.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloRequest;
using helloworld::HelloReply;

class GreeterClient {
public:
    GreeterClient(std::shared_ptr<Channel> channel)
        : stub_(Greeter::NewStub(channel)) {}

    void SayHello(const std::string& user) {
        HelloRequest request;
        request.set_name(user);

        AsyncClientCall* call = new AsyncClientCall;
        call->response_reader = stub_->AsyncSayHello(&call->context, request, &cq_);

        call->response_reader->Finish(&call->reply, &call->status, (void*)call);
    }

    void AsyncCompleteRpc() {
        void* got_tag;
        bool ok = false;

        while (cq_.Next(&got_tag, &ok)) {
            AsyncClientCall* call = static_cast<AsyncClientCall*>(got_tag);

            if (call->status.ok()) {
                std::cout << "Greeter received: " << call->reply.message() << std::endl;
            } else {
                std::cerr << "RPC failed" << std::endl;
            }

            delete call;
        }
    }

private:
    struct AsyncClientCall {
        HelloReply reply;
        ClientContext context;
        Status status;
        std::unique_ptr<grpc::ClientAsyncResponseReader<HelloReply>> response_reader;
    };

    std::unique_ptr<Greeter::Stub> stub_;
    CompletionQueue cq_;
};

int main(int argc, char** argv) {
    std::cout << APP_NAME << " v" << APP_VERSION_MAJOR << "." << APP_VERSION_MINOR << "." << APP_VERSION_PATCH << APP_VERSION_DIRTY << std::endl;

    GreeterClient greeter(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    std::string user("world");
    greeter.SayHello(user);
    greeter.AsyncCompleteRpc();

    return 0;
}