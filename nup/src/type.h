#ifndef NUP_TYPE_H
#define NUP_TYPE_H

#include <string>
#include <map>
#include <thread>
#include <mutex>
#include <functional>
#include <atomic>
#include <memory>
#include <exception>
#include <type_traits>

#include <boost/noncopyable.hpp>
#include <boost/preprocessor/cat.hpp>

#include "nup_alloc.h"

namespace nup {

template <class Param>
void unused(Param param)
{
    (void)param;
}

using string = std::string;

class Interator {
public:
    Interator() { }
    virtual ~Interator() { }
    virtual void next() = 0;
    virtual bool has_value() const = 0;
};

template <class KEY, class VALUE>
class KVIterator : public Interator {
public:
    virtual const KEY& key() = 0;
    virtual VALUE& value() = 0;
};

template <class VALUE>
class VIterator : public Interator {
public:
    virtual VALUE& value() = 0;
};

template <class KEY, class VALUE>
class Map {
public:
    class MapIterator : public KVIterator<KEY, VALUE> {
    public:
        using std_map_iterator = typename std::map<KEY, VALUE>::iterator;

        MapIterator(Map* map)
        {
            it_ = map->begin();
            end_ = map->end();
        }

        MapIterator(Map* map, std_map_iterator it)
        {
            it_ = it;
            end_ = map->end();
        }

        virtual ~MapIterator() { }
        virtual void next() override { ++it_; }

        virtual bool has_value() const override { return it_ != end_; }

        virtual KEY& key() override { return it_.first; }
        virtual VALUE& value() override { return it_.second; }

        void set(const KEY& key, const VALUE& value) { delegate_[key] = value; }

        MapIterator lookup(const KEY& key)
        {
            return MapIterator(&delegate_, delegate_.find(key));
        }

        void remove(const KEY& key) { delegate_.erase(key); }

    private:
        std_map_iterator it_;
        std_map_iterator end_;
    };

    Map() { }
    virtual ~Map() { }

    MapIterator iterator() { return MapIterator(&delegate_); }

    friend class MapIterator;

private:
    std::map<KEY, VALUE> delegate_;
};

template <class T>
class Size {
public:
    T width, height;
};

using IntSize = Size<int>;

template <class T>
using Ptr = std::shared_ptr<T>;

template <class T>
class Singleton : private boost::noncopyable {
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
    exception(const string& msg)
    : message_(msg)
    {
    }
    virtual ~exception() { }
    virtual const char* what() const noexcept { return message_.c_str(); }

private:
    string message_;
};

} // namespace nup

#define NUP_ABORT(message) NUP_THROW(exception(message))
#define NUP_THROW(e)       throw e;

#define NUP_CURRENT_THREAD_ID() std::this_thread::get_id()
#define NUP_THREAD_ID           std::decay_t<decltype(CURRENT_THREAD_ID())>

#define NUP_MUTEX(name) std::mutex name
#define NUP_SCOPE_LOCK(name)                                                   \
    std::lock_guard<std::mutex> BOOST_PP_CAT(nup_scope_lock_, __LINE__)(name);

#define NUP_MUTEX_DEFAULT      NUP_MUTEX(default_mutex_)
#define NUP_SCOPE_LOCK_DEFAULT NUP_SCOPE_LOCK(default_mutex_)

#endif