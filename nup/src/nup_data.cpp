#include "nup_data.h"

namespace nup {

MemoryAreaRef empty_memory_area_ref = { nullptr, 0 };

Data::Data()
: mem_(nullptr)
, mem_ref_(empty_memory_area_ref)
, weak_(true)
{
}

Data::Data(Ptr<MemoryBlock> m)
: mem_(m)
, mem_ref_(empty_memory_area_ref)
, weak_(false)
{
}
Data::Data(MemoryAreaRef r)
: mem_(nullptr)
, mem_ref_(r)
, weak_(true)
{
}

Data::~Data() { }

void* Data::ptr() const
{
    if (weak_)
        return mem_ref_.base;
    else
        return mem_ ? mem_->data() : nullptr;
}

size_t Data::size() const
{
    if (weak_)
        return mem_ref_.size;
    else
        return mem_ ? mem_->size() : 0;
}

} // namespace nup