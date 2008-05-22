#ifndef __RES_GC_NEW_DELETE_H__
#define __RES_GC_NEW_DELETE_H__

#include <libsn.h>
#include <stddef.h>

void* operator new       ( size_t blocksize );
void* operator new[]     ( size_t blocksize );
void operator delete     ( void* block );
void operator delete[]   ( void* block );

extern "C" void* mymalloc (size_t blocksize);
extern "C" void myfree (void* ptr);
extern "C" void free (void* ptr);
extern "C" void* malloc (size_t blocksize);
extern "C" void free_r (void* ptr);
//Very, very simple realloc, just enough to get things working.
extern "C" void* realloc (void* ptr, size_t blocksize);

#endif