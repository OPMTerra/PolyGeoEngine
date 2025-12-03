#pragma once
#include <iostream>
#include <cstddef>//For size_t, max_align_t
#include <cstdint>//for uintptr_t
#include "Shape.h"

class Arena {
    private:
    char* memoryStart;
    size_t offset;
    size_t totalsize;

    public:
    Arena(size_t size){
        //Allocating the block of memory of the given size
        memoryStart = new char[size];
        offset = 0;
        totalsize = size;
    }

    ~Arena() {
        delete[] memoryStart;
        cout << "Initialized memory area with 10 MB (freed)." << endl;
    }

    void* alloc(size_t bytes) {
        //using max_align_t gives the maximum alignment required
        size_t align = alignof(std::max_align_t);
        //Calculates the current address as a number
        uintptr_t current_ptr = reinterpret_cast<uintptr_t>(memoryStart + offset);
        //Padding logic
        size_t padding = (align - (current_ptr % align)) % align;
        
        if (offset + bytes + padding > totalsize) return nullptr;
        
        uintptr_t aligned_ptr = current_ptr + padding;
        
        offset += bytes + padding;
        
        return reinterpret_cast<void*>(aligned_ptr);
    }
};