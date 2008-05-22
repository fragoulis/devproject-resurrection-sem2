#include <dolphin.h>
#include <libsn.h>
#include <stdlib.h>
#include <stdio.h>
#include "gc_new_delete.h"
#include <demo.h>
#define HEAP_ID 0

static BOOL IsHeapInitialized = FALSE;

static void CPPInit()
{
    if (IsHeapInitialized)
    {
        return;
    }

	DEMOInit(NULL);

    //arenaLo = OSGetArenaLo();
    //arenaHi = OSGetArenaHi();

    //// Create a heap
    //// OSInitAlloc should only ever be invoked once.
    //arenaLo = OSInitAlloc(arenaLo, arenaHi, 1); // 1 heap
    //OSSetArenaLo(arenaLo);

    //// Ensure boundaries are 32B aligned
    //arenaLo = (void*)OSRoundUp32B(arenaLo);
    //arenaHi = (void*)OSRoundDown32B(arenaHi);

    //// The boundaries given to OSCreateHeap should be 32B aligned
    //OSSetCurrentHeap(OSCreateHeap(arenaLo, arenaHi));
    //// From here on out, OSAlloc and OSFree behave like malloc and free
    //// respectively
    //OSSetArenaLo(arenaLo=arenaHi);
    IsHeapInitialized = TRUE;
}

void* operator new       ( size_t blocksize )
{
    if (!IsHeapInitialized)
    {
        CPPInit();
    }
    return OSAllocFromHeap(HEAP_ID, blocksize);
}

void* operator new[]     ( size_t blocksize )
{
    if (!IsHeapInitialized)
    {
        CPPInit();
    }
    return OSAllocFromHeap(HEAP_ID, blocksize);
}

void operator delete     ( void* block )
{
	if(block)
		OSFreeToHeap(HEAP_ID, block);
}

void operator delete[]   ( void* block )
{
	if(block)
		OSFreeToHeap(HEAP_ID, block);
}

void* mymalloc (size_t blocksize)
{
	if (!IsHeapInitialized)
	{
		CPPInit();
	}
	return OSAllocFromHeap(HEAP_ID, blocksize);
}

void myfree (void* ptr)
{
	if(ptr)
		OSFreeToHeap(HEAP_ID, ptr);
}

void free (void* ptr)
{
	if(ptr)
		OSFreeToHeap(HEAP_ID, ptr);
}

void* malloc (size_t blocksize)
{
	if (!IsHeapInitialized)
	{
		CPPInit();
	}
	return OSAllocFromHeap(HEAP_ID, blocksize);
}

void free_r (void* ptr)
{
	if(ptr)
		OSFreeToHeap(HEAP_ID, ptr);
}


//Very, very simple realloc, just enough to get things working.
void* realloc (void* ptr, size_t blocksize)
{
	if (!IsHeapInitialized)
	{
		printf("\nERROR: Cannot realloc if heap not initialised\n");
		return NULL;
	}
	
	OSFreeToHeap(HEAP_ID, ptr);
	return OSAllocFromHeap(HEAP_ID, blocksize);
}

// Thanks Rich from Steven!!
class GcGameInitOnce
{
public:
	GcGameInitOnce(void)
	{
		_register_malloc = mymalloc;
		_register_free = myfree;
		CPPInit();
	}
} __once_only__;