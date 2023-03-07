#ifndef NUP_ALLOC_H
#define NUP_ALLOC_H

#define NUP_MALLOC(bytes)   (void*)new char[bytes]
#define NUP_ALLOC(T, count) (T*)new char[(count) * sizeof(T)]
#define NUP_FREE(ptr)       delete[](char*) ptr

#define NUP_NEW(T, ...)                 new T(__VA_ARGS__)
#define NUP_NEW_ARRAY(T, count)         new T[count]
#define NUP_DELETE(ptr)                 delete ptr
#define NUP_DELETE_ARRAY(ptr, count)    delete[] ptr
#define NUP_MAKE_PTR(T, ...)            Ptr<T>(new T(__VA_ARGS__))

#endif