#ifndef NUP_ANY_H
#define NUP_ANY_H

#include "type.h"

namespace nup {

class AnyWrapper {
public:
    AnyWrapper() { }
    virtual ~AnyWrapper() = 0;
    virtual bool is(TYPE_INFO type_info) const = 0;
};

template <class T>
class AnyWrapperT : public AnyWrapper {
public:
    AnyWrapperT(const T& obj)
    : obj_(obj)
    {
    }
    virtual ~AnyWrapperT() { }

    T get() const { return obj_; }

    virtual bool is(TYPE_INFO type_info) const override
    {
        return typeid(T) == type_info;
    }

private:
    T obj_;
};

class Any {
public:
    Any() { }
    Any(Ptr<AnyWrapper> wrapper)
    : wrapper_(wrapper)
    {
    }
    ~Any() { }

    bool has_value() const { return wrapper_ != nullptr; }

    template <class T>
    T to() const
    {
        auto ptr = dynamic_cast<AnyWrapperT<T>*>(wrapper_.get());
        if (!ptr) {
            NUP_ABORT("any to invalid type");
        }
        return ptr->get();
    }

    template <class T>
    bool is() const
    {
        return wrapper_ && wrapper_->is(typeid(T));
    }

private:
    Ptr<AnyWrapper> wrapper_;
};

template <class T>
Any to_any(const T& obj)
{
    return Any(NUP_MAKE_PTR(AnyWrapperT<T>, obj));
}

template <class T>
T any_to(const Any& any)
{
    return any.to<T>();
}

} // namespace nup

#endif