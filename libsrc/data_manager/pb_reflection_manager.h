//
// Created by xinnjie on 2019/9/24.
//

#ifndef LITTLEB_PB_REFLECTION_MANAGER_H
#define LITTLEB_PB_REFLECTION_MANAGER_H
#include <memory>
#include <map>
#include <google/protobuf/message.h>

namespace littleB {
class PbReflectionManager {
public:
    /*方法元数据*/
    struct MethodData {
        std::unique_ptr<google::protobuf::Message> request_proto;   // protobuf请求数据类型
        std::unique_ptr<google::protobuf::Message> response_proto;  // protobuf响应数据类型
    };
    bool AddReflection(uint32_t cmd_id, std::unique_ptr<google::protobuf::Message> request,
                       std::unique_ptr<google::protobuf::Message> response);
    bool IsValidCmd(uint32_t cmd_id);
    google::protobuf::Message &GetRequestReflection(uint32_t cmd_id);
    google::protobuf::Message &GetResponseReflection(uint32_t cmd_id);


private:
    std::map<uint32_t, MethodData> reflection_data_;
};
}  // namespace littleB
#endif  // LITTLEB_PB_REFLECTION_MANAGER_H
