//
//  align.cpp
//
//  Created by Grégoire Payen de La Garanderie and Paul Payen de La Garanderie on december 2012.
//  Copyright (c) 2012 Gregoire Payen de La Garanderie and Paul Payen de La Garanderie. All rights reserved.
//

#include "align.h"

#include <cassert>

#ifdef INTERNAL_MEMALIGN

// Our own version of aligned_alloc. We allocate a block large enough to contain
// the requested aligned memory and a 8-bit integer which allow us to recover
// the original pointer in aligned_free
// ------------------------------------
// |junk|8-bit diff|aligned memory ...|
// ------------------------------------
// |<--- diff ---->|
void* aligned_alloc(size_t boundary, size_t size)
{
    assert(boundary < 255);

    intptr_t ptr = (intptr_t)malloc(size + boundary);

    intptr_t mask = boundary-1;
    uint8_t diff = boundary - (ptr&mask);

    *(uint8_t*)(ptr + diff - 1) = diff;
    return (void*)(ptr + diff);
}

void aligned_free(void* ptr)
{
    intptr_t diff = *((uint8_t*)ptr - 1);
    free((uint8_t*)ptr - diff);
}
#else
#define aligned_free free
#endif
