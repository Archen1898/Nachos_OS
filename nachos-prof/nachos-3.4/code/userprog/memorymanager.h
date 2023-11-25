#ifndef MEMORY_H
#define MEMORY_H

#include "bitmap.h"
#include "synch.h"

class MemoryManager {

    public:
        MemoryManager();
        ~MemoryManager();

        //Gets the first clear page
        int AllocatePage();
        //Takes the index of a page and frees it
        int DeallocatePage(int which);
        unsigned int GetFreePageCount();

    private:
        //Used to track allocation by setting one bit per page
        BitMap *bitmap;

};



#endif // MEMORY_H
