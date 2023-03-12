#include "nup_data.h"

namespace nup {

const MemoryAreaRef empty_memory_area_ref = { nullptr, 0 };

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

uint8_t* Data::ptr() const
{
    if (weak_)
        return (uint8_t*)(mem_ref_.base);
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

void Data::resize(size_t size)
{
    mem_->resize(size);
    if (weak_) {
        memcpy(mem_->data(), mem_ref_.base, std::min(size, mem_ref_.size));
    }
    weak_ = false;
}

} // namespace nup