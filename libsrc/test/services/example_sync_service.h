//
// Created by xinnjie on 2019/9/23.
//

#ifndef LITTLEB_EXAMPLE_SYNC_SERVICE_H
#define LITTLEB_EXAMPLE_SYNC_SERVICE_H
#include "example_service.pb.h"
#include "service_interface.h"
class ExampleSyncService : public littleB::SyncServiceInterface<ExampleReq, ExampleResp> {
public:
    explicit ExampleSyncService(int exampleInt) : example_int(exampleInt) {}
    ExampleResp operator()(RoleInfo& role, const ExampleReq& request) override;
    int GetExampleInt() const { return example_int; }

private:
    int example_int;
};

#endif  // LITTLEB_EXAMPLE_SYNC_SERVICE_H
