#pragma once
#include <iostream>
#include "Shape.h"
using namespace std;

class Arena {
    private:
    char* memoryStart;
    size_t offset;
    size_t totalsize;

    public:
    Arena(size_t size){
        memoryStart = new char[size];
        offset = 0;
        totalsize = size;
    }

    ~Arena() {
        delete[] memoryStart;
    }

    void* alloc(size_t bytes) {
        if (offset + bytes > totalsize) return nullptr;
        void* ptr = memoryStart + offset;
        offset += bytes;
        return ptr;
    }
};