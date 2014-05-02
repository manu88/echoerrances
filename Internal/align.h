//
//  align.cpp
//
//  Created by Grégoire Payen de La Garanderie and Paul Payen de La Garanderie on december 2012.
//  Copyright (c) 2012 Gregoire Payen de La Garanderie and Paul Payen de La Garanderie. All rights reserved.
//

#ifndef ALIGN_H
#define ALIGN_H

#include <stdint.h>
#include <stdlib.h>

//#ifndef __linux
#define INTERNAL_MEMALIGN
//#endif

#ifdef INTERNAL_MEMALIGN
void* aligned_alloc(size_t boundary, size_t size);
void aligned_free(void* ptr);
#else
#define aligned_free free
#endif

#endif // ALIGN_H
