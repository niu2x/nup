#ifndef NUP_TYPE_H
#define NUP_TYPE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <map>
#include <type_traits>
#include <thread>
#include <mutex>
#include <functional>
#include <vector>
#include <atomic>
#include <memory>
#include <exception>
#include <typeinfo>
#include <type_traits>

#include <boost/noncopyable.hpp>
#include <boost/preprocessor/cat.hpp>

#include <Eigen/Dense>

#include "nup_alloc.h"

#define NUP_INLINE BOOST_FORCEINLINE
#define NUP_ENUM_COUNT(e) (int)(e::COUNT)

namespace nup {

template <bool B, class T>
using enable_if_t = std::enable_if_t<B, T>;

template <class B, class D>
constexpr bool is_base_of_v = std::is_base_of_v<B, D>;

using TYPE_INFO = const std::type_info&;

using noncopyable = boost::noncopyable;

template <class Param>
void unused(Param param)
{
    (void)param;
}

using TimePoint = uint64_t;
using TimeInterval = uint64_t;
using BitFlags = uint64_t;

using String = std::string;

class BaseInterator {
};

template <class T>
class Iterator : public BaseInterator {
public:
    Iterator() { }
    virtual ~Iterator() { }

    virtual const T& value() const = 0;
    virtual void next() = 0;
    virtual bool has_value() const = 0;

    operator bool() const { return has_value(); }
    const T& operator*() const { return value(); }
};

template <class T>
enable_if_t<is_base_of_v<BaseInterator, T>, T&> operator++(T& iter)
{
    iter.next();
    return iter;
}

template <class T>
enable_if_t<is_base_of_v<BaseInterator, T>, T> operator++(T& iter, int)
{
    auto ret = iter;
    iter.next();
    return ret;
}

template <class T>
class ArrayIterator : public Iterator<T> {
public:
    ArrayIterator(const T* base, int nr)
    : base_(base)
    , nr_(nr)
    , pos_(0)
    {
    }
    virtual ~ArrayIterator() { }

    virtual const T& value() const override { return base_[pos_]; }
    virtual void next() override { pos_++; }
    virtual bool has_value() const override { return pos_ < nr_; }

private:
    const T* base_;
    int nr_;
    int pos_;
};

template <class T>
class Vector {
public:
    Vector() { }
    ~Vector() { }

    T* data() { return delegate_.data(); }
    const T* data() const { return delegate_.data(); }

    void resize(size_t size) { delegate_.resize(size); }
    void reserve(size_t size) { delegate_.reserve(size); }
    size_t size() const { return delegate_.size(); }

    ArrayIterator<T> iterator() const
    {
        return ArrayIterator<T>(data(), size());
    };

    void push_back(const T& t) { delegate_.push_back(t); }

private:
    std::vector<T> delegate_;
};

using MemoryBlock = Vector<uint8_t>;

using offset_t = std::ptrdiff_t;

template <class T>
class Size {
public:
    T width, height;
};

using IntSize = Size<int>;

// class Point {
// public:
//     float x, y, z;
// };
//
//
using RGBA32 = uint32_t;

template <class T>
using Ptr = std::shared_ptr<T>;

template <class T>
class Singleton : private noncopyable {
public:
    using TYPE = T;

    static T* get() { return instance_; }

    template <class... Param>
    static void create_instance(Param... args)
    {
        if (!instance_)
            instance_ = NUP_NEW(TYPE, std::forward<Param>(args)...);
    }

    static void create_instance()
    {
        if (!instance_)
            instance_ = NUP_NEW(TYPE);
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

class exception : public std::exception {
public:
    exception(const String& msg)
    : message_(msg)
    {
    }
    virtual ~exception() { }
    virtual const char* what() const noexcept { return message_.c_str(); }

private:
    String message_;
};

struct MemoryAreaRef {
    void* base;
    size_t size;
};

extern const MemoryAreaRef empty_memory_area_ref;

enum class Error {
    OK = 0,
    END_OF_FILE,
};

using E = Error;

template <class T1, class T2>
auto min(const T1& t1, const T2& t2)
{
    return std::min(t1, t2);
}

template <class T1, class T2>
auto max(const T1& t1, const T2& t2)
{
    return std::max(t1, t2);
}

using Matrix4f = Eigen::Matrix4f;
using Transform = Matrix4f;

} // namespace nup

#define NUP_ABORT(message) NUP_THROW(exception(message))
#define NUP_THROW(e)       throw e;

#define NUP_ASSERT(cond, message)                                              \
    {                                                                          \
        if (!(cond)) {                                                         \
            NUP_ABORT(message);                                                \
        }                                                                      \
    }

#define NUP_CURRENT_THREAD_ID() std::this_thread::get_id()
#define NUP_THREAD_ID           std::decay_t<decltype(CURRENT_THREAD_ID())>

#define NUP_MUTEX(name) std::mutex name
#define NUP_SCOPE_LOCK(name)                                                   \
    std::lock_guard<std::mutex> BOOST_PP_CAT(nup_scope_lock_, __LINE__)(name);

#define NUP_MUTEX_DEFAULT      NUP_MUTEX(default_mutex_)
#define NUP_SCOPE_LOCK_DEFAULT NUP_SCOPE_LOCK(default_mutex_)

#define NUP_CHECK_GL_ERROR()                                                   \
    ({                                                                         \
        GLenum __error = glGetError();                                         \
        if (__error)                                                           \
            printf("OpenGL error 0x%04X in %s %d\n", __error, __FUNCTION__,    \
                __LINE__);                                                     \
    })

#endif