#include <dolphin.h>
#include <stddef.h>


// program sort_idxtbl(...) to make a permuted array of indices
#include <vector>
#include <algorithm>


///////////////////declare new and delete
#define HEAP_ID 0


static BOOL IsHeapInitialized = FALSE;


static void CPPInit()
{
    void*    arenaLo;
    void*    arenaHi;


    if (IsHeapInitialized)
    {
        return;
    }



    arenaLo = OSGetArenaLo();
    arenaHi = OSGetArenaHi();


    // Create a heap
    // OSInitAlloc should only ever be invoked once.
    arenaLo = OSInitAlloc(arenaLo, arenaHi, 1); // 1 heap
    OSSetArenaLo(arenaLo);


    // Ensure boundaries are 32B aligned
    arenaLo = (void*)OSRoundUp32B(arenaLo);
    arenaHi = (void*)OSRoundDown32B(arenaHi);


    // The boundaries given to OSCreateHeap should be 32B aligned
    OSSetCurrentHeap(OSCreateHeap(arenaLo, arenaHi));
    // From here on out, OSAlloc and OSFree behave like malloc and free
    // respectively
    OSSetArenaLo(arenaLo=arenaHi);
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
    OSFreeToHeap(HEAP_ID, block);
}


void operator delete[]   ( void* block )
{
    OSFreeToHeap(HEAP_ID, block);
}
///////////////////////


template <class RAIter>
struct sort_idxtbl_pair
   {
       RAIter it;
       int i;
       bool operator < (const sort_idxtbl_pair& s)
       { return (*it) < (*(s.it)); }
       void set(const RAIter& _it, int _i) { it=_it; i=_i; }
       sort_idxtbl_pair() {}
   } ;


template <class RAIter>
void sort_idxtbl(RAIter first, RAIter last, int* pidxtbl)
{
    int iDst = last-first;
    typedef std::vector< sort_idxtbl_pair<RAIter> > V;
    V v(iDst);


    int i=0;
    RAIter it = first;
    V::iterator vit = v.begin();
    for (i=0; it<last; it++, vit++, i++)
       (*vit).set(it,i);


    std::sort(v.begin(), v.end());


    int *pi = pidxtbl;
    vit = v.begin();
    for (; vit<v.end(); pi++, vit++)
        *pi = (*vit).i;
}


main()
{
 OSInit();


   int ai[10] = { 15,12,13,14,18,11,10,17,16,19 };


   printf("#################\n");
   std::vector<int> vecai(ai, ai+10);
   int aidxtbl[10];
   sort_idxtbl(vecai.begin(), vecai.end(), aidxtbl);


   for (int i=0; i<10; i++)
 printf("i=%i, aidxtbl[i]=%i, ai[aidxtbl[i]]=%i\n", i, aidxtbl[i],
ai[aidxtbl[i]]);


 printf("#################\n");


 OSHalt("End of stl demo");


}