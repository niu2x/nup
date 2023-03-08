#ifndef NUP_DATA_H
#define NUP_DATA_H

#include "type.h"

namespace nup {

struct MemoryAreaRef {
    void* base;
    size_t size;
};

extern MemoryAreaRef empty_memory_area_ref;

class Data {
public:
    Data();
    Data(Ptr<MemoryBlock>);
    Data(MemoryAreaRef);

    ~Data();

    Data(const Data&) = default;
    Data& operator=(const Data&) = default;

    void* ptr() const;
    size_t size() const;

private:
    Ptr<MemoryBlock> mem_;
    MemoryAreaRef mem_ref_;
    bool weak_;
};

} // namespace nup

#endif