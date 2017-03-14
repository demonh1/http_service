#ifndef MSG_H
#define MSG_H

#include <cinttypes>

namespace utils {

enum Priority
{
    LOW,
    NORMAL,
    HIGH
};

class Message
{
public:
    explicit Message(int32_t id, Priority priority = Priority::NORMAL) : id_(id), priority_(priority),ref_count_(1) {}
    virtual ~Message() = default;

    int32_t id() const { return id_; }

    uint32_t AddRef(){ return ++ref_count_; }

    uint32_t Release()
    {
        uint32_t res = --ref_count_;
        if(res == 0)
            delete this;
        return res;
    }

    Priority priority() const { return priority_; }

private:
    const int32_t id_;
    Priority priority_;
    uint32_t ref_count_;
};

} // utils


#endif // MSG_H
