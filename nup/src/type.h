#ifndef NUP_TYPE_H
#define NUP_TYPE_H

#include <string>
#include <memory>

#include <boost/noncopyable.hpp>

namespace nup {

using string = std::string;

template <class T>
class Size {
public:
    T width, height;
};

using IntSize = Size<int>;

template <class T>
using Ptr<T> = std::shared_ptr<T>;

template <class T>
class Singleton : private boost::noncopyable {
public:
    static const T* get() const { return instance_; }
    static T* get() { return instance_; }
    static void make_instance()
    {
        if (!instance_)
            instance_ = T::make_instance();
    }

    static void destroy_instance()
    {
        if (instance_) {
            NUP_DELETE(instance_);
        }
    }

protected:
    Singleton() { }
    virtual ~Singleton() { }
    static T* instance_;
};

} // namespace nup

#endif