#ifndef NUP_ALLOC_H
#define NUP_ALLOC_H

#define NUP_MALLOC(bytes)   (void*)new char[bytes]
#define NUP_ALLOC(T, count) (T*)new char[(count) * sizeof(T)]
#define NUP_FREE(ptr)       delete[](char*) ptr

#define NUP_NEW(T)                      new T
#define NUP_NEW_ARRAY(T, count)         new T[count]
#define NUP_DELETE(ptr, T)              delete ptr
#define NUP_DELETE_ARRAY(ptr, T, count) delete[] ptr

#endif