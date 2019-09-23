//
// Created by xinnjie on 2019/9/12.
//

#ifndef LITTLEB_EXAMPLE_ASYNC_SERVICE_H
#define LITTLEB_EXAMPLE_ASYNC_SERVICE_H
#include <service_interface.h>
#include <boost/pointer_cast.hpp>
#include "example_service.pb.h"
class ExampleService : public littleB::AsyncServiceInterface {
public:
    using ExampleRespPtr = std::unique_ptr<ExampleResp>;  // TODO: generate
    using ExampleReqPtr = std::unique_ptr<ExampleReq>;    // TODO: generate
    folly::Future<littleB::ResponsePtr> operator()(littleB::RequestPtr req) override { // TODO: generate
        return DoCall(boost::dynamic_pointer_cast<ExampleReq>(std::move(req)));
    }

    folly::Future<ExampleRespPtr> DoCall(ExampleReqPtr req);  // TODO: generate
};

class example_async_service {};

#endif  // LITTLEB_EXAMPLE_ASYNC_SERVICE_H
