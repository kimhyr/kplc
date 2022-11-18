#pragma once
#ifndef UTILS_DYNARRAY_HPP
#define UTILS_DYNARRAY_HPP

template<typename TData>
struct Dynarray {
    UInt64 space;
    UInt64 size;
    TData *data;
};

#endif  // UTILS_DYNARRAY_HPP
