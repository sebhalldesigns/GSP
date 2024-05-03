#include "GSPCore/GVector.h"
#include "def/GVectorDef.h"

#include <stdlib.h>
#include <string.h>

// initialises vector
GVector GVector_Init() {

    GVector vector = calloc(1, sizeof(GVectorDef));

    if (vector == NULL) {
        return NULL;
    }

    ((GVectorDef*)vector)->data = NULL;
    ((GVectorDef*)vector)->size = 0;

    return vector;
}

// frees vector
void GVector_Free(GVector vector) {

    free(vector);
}

// adds item to vector if it doesn't exist
void GVector_Add(GVector vector, GVectorItem item) {

    if (vector == NULL) {
        return;
    }

    void* data = ((GVectorDef*)vector)->data;
    size_t size = ((GVectorDef*)vector)->size;

    void* newData = realloc(data, sizeof(uintptr_t) * (size + 1));

    if (newData == NULL) {
        return;
    }

    *((uintptr_t*)newData + (sizeof(uintptr_t) * size)) = (uintptr_t)item;

    ((GVectorDef*)vector)->size++;
    ((GVectorDef*)vector)->data = newData;
}

// removes item from vector if it exists
void GVector_Remove(GVector vector, GVectorItem item) {

    if (vector == NULL) {
        return;
    }

    void* data = ((GVectorDef*)vector)->data;
    size_t size = ((GVectorDef*)vector)->size;

    for (size_t i = 0; i < size; i++) {
        if (*((GVectorItem*)data + (sizeof(uintptr_t) * i)) == item) {
            // found item

            size_t itemsToMove = size - (i + 1);
            memcpy(data + (sizeof(uintptr_t) * i), data + (sizeof(uintptr_t) * (i + 1)), sizeof(uintptr_t) * itemsToMove);

            void* newData = realloc(data, sizeof(uintptr_t) * (size - 1));

            ((GVectorDef*)vector)->size--;

            if (newData == NULL) {
                return;
            }

            ((GVectorDef*)vector)->data = newData;

        }
    }

}

// gets size of vector
size_t GVector_Size(GVector vector) {
    
    if (vector == NULL) {
        return 0;
    }

    return ((GVectorDef*)vector)->size;
}

// returns item at index or NULL if the index is invalid.
GVectorItem GVector_Get(GVector vector, size_t index) {
    
    if (vector == NULL) {
        return NULL;
    }

    if (index >= ((GVectorDef*)vector)->size) {
        return NULL;
    }

    return (GVectorItem)*((uintptr_t*)((GVectorDef*)vector)->data + (sizeof(uintptr_t) * index));
}

// sets item if it is a valid index
void GVector_Set(GVector vector, size_t index, GVectorItem item) {

    if (vector == NULL) {
        return;
    }

    if (index >= ((GVectorDef*)vector)->size) {
        return;
    }

    *((uintptr_t*)((GVectorDef*)vector)->data + (sizeof(uintptr_t) * index)) = (uintptr_t)item;

}

// returns true if vector contains item, false if it doesn't
bool GVector_Contains(GVector vector, GVectorItem item) {

    if (vector == NULL) {
        return false;
    }

    void* data = ((GVectorDef*)vector)->data;
    size_t size = ((GVectorDef*)vector)->size;

    for (size_t i = 0; i < size; i++) {
        if (*((GVectorItem*)data + (sizeof(uintptr_t) * i)) == item) {
            return true;
        }
    }

    return false;
}

// returns index of item if it is in vector, or GVector Size (last element + 1) 
// if it is not in the vector.
size_t GVector_IndexOf(GVector vector, GVectorItem item) {

    if (vector == NULL) {
        return SIZE_MAX;
    } 

    void* data = ((GVectorDef*)vector)->data;
    size_t size = ((GVectorDef*)vector)->size;

    for (size_t i = 0; i < size; i++) {
        if (*((GVectorItem*)data + (sizeof(uintptr_t) * i)) == item) {
            return i;
        }
    }

    return size + 1;
}

