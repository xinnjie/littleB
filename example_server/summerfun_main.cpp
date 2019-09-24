//
// Created by xinnjie on 2019/9/24.
//
#include <wangle/bootstrap/ServerBootstrap.h>
#include <wangle/channel/AsyncSocketHandler.h>
#include <wangle/codec/LineBasedFrameDecoder.h>
#include <wangle/codec/StringCodec.h>

using namespace folly;
using namespace wangle;

using EchoPipeline = Pipeline<IOBufQueue&, std::string> ;

// the main logic of our echo server; receives a string and writes it straight
// back
class EchoHandler : public HandlerAdapter<std::string> {
public:
    void read(Context* ctx, std::string msg) override {
        std::cout << "handling " << msg << std::endl;
        write(ctx, msg + "\r\n");
    }
};

// where we define the chain of handlers for each messeage received
class EchoPipelineFactory : public PipelineFactory<EchoPipeline> {
public:
    EchoPipeline::Ptr newPipeline(
        std::shared_ptr<AsyncTransportWrapper> sock) override {
        auto pipeline = EchoPipeline::create();
        pipeline->addBack(AsyncSocketHandler(sock));
        pipeline->addBack(LineBasedFrameDecoder(8192));
        pipeline->addBack(StringCodec());
        pipeline->addBack(EchoHandler());
        pipeline->finalize();
        return pipeline;
    }
};

int main(int argc, char** argv) {
    ServerBootstrap<EchoPipeline> server;
    server.childPipeline(std::make_shared<EchoPipelineFactory>());
    server.bind(8009);
    server.waitForStop();

    return 0;
}