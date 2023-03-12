#ifndef NUP_DATA_H
#define NUP_DATA_H

#include "nup_type.h"

namespace nup {

class Data {
public:
    Data();
    Data(Ptr<MemoryBlock>);
    Data(MemoryAreaRef);
    ~Data();
    Data(const Data&) = default;
    Data& operator=(const Data&) = default;

    uint8_t* ptr() const;
    size_t size() const;

    void resize(size_t size);

private:
    Ptr<MemoryBlock> mem_;
    MemoryAreaRef mem_ref_;
    bool weak_;
};

} // namespace nup

#endif