//
// Created by xinnjie on 2019/9/24.
//

#include "pb_reflection_manager.h"
namespace littleB {
bool littleB::PbReflectionManager::AddReflection(uint32_t cmd_id, std::unique_ptr<google::protobuf::Message> request,
                                                 std::unique_ptr<google::protobuf::Message> response) {
    assert(reflection_data_.count(cmd_id) == 0);
     return reflection_data_.emplace(cmd_id, MethodData{std::move(request), std::move(response)}).second;
}
google::protobuf::Message& PbReflectionManager::GetRequestReflection(uint32_t cmd_id) {
    assert(IsValidCmd(cmd_id));
    return *reflection_data_[cmd_id].request_proto;
}
bool PbReflectionManager::IsValidCmd(uint32_t cmd_id) { return reflection_data_.count(cmd_id) != 0; }
google::protobuf::Message& PbReflectionManager::GetResponseReflection(uint32_t cmd_id) {
    assert(IsValidCmd(cmd_id));
    return *reflection_data_[cmd_id].response_proto;
}
}  // namespace littleB
