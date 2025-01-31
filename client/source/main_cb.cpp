#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include <condition_variable>
#include "grpcpp/grpcpp.h"
#include "helloworld.pb.h"
#include "helloworld.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloRequest;
using helloworld::HelloReply;

class GreeterClient {
public:
    GreeterClient(std::shared_ptr<Channel> channel)
        : stub_(Greeter::NewStub(channel)) {}

    void SayHello(const std::string& user) {
        std::unique_lock<std::mutex> lock(mutex_);

        HelloRequest request;
        request.set_name(user);

        stub_->async()->SayHello(&context_, &request, &reply_,
            [this](Status status) {
                std::lock_guard<std::mutex> lock(mutex_);
                if (status.ok()) {
                    std::cout << "Greeter received: " << reply_.message() << std::endl;
                } else {
                    std::cerr << "RPC failed: " << status.error_message() << std::endl;
                }
                done_ = true;
                cv_.notify_one();
            });

        // Wait for the RPC to complete
        cv_.wait(lock, [this] { return done_; });
    }

private:
    std::unique_ptr<Greeter::Stub> stub_;
    ClientContext context_;
    HelloReply reply_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool done_ = false;
};

int main(int argc, char** argv) {
    std::cout << APP_NAME << " v" << APP_VERSION_MAJOR << "." << APP_VERSION_MINOR << "." << APP_VERSION_PATCH << APP_VERSION_DIRTY << std::endl;

    GreeterClient greeter(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    std::string user("world");
    greeter.SayHello(user);

    // Sleep for a while to allow the callback to complete
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}