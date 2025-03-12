#pragma once
#include <functional>
#include <string>

namespace wf::err {
    enum class eResultType {
        Ok = 0,
        Err = 1
    };

    struct Result {
        eResultType type = eResultType::Ok;
        std::string comment;
        using Callback = std::function<void(eResultType, const std::string&)>;
        Callback callback;

        explicit Result(const eResultType t = eResultType::Ok, std::string cmt = "", Callback cb = nullptr)
            : type(t), comment(std::move(cmt)), callback(std::move(cb)) {}

        void Notify() const {
            if (callback) {
                callback(type, comment);
            }
        }
    };
}