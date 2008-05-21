//////////////////////////////////////////////////////////////
//
//	Vector2.cpp
//
//	SN-Dave: This program is a simple confidence test which tests the following
//	1. STL Include file setup.
//		a. lists
//		b. strings
//		c. vectors
//	2. STL I/O library.
//		a. I/O stream functions.
//		b. cout/cerr
//	3. libc I/O library.
//
//	Build line (if you have autolinking set up correctly):
//	ngccc vector.cpp -g -o vector.elf
//
//	Expected output in TTY Window:
//

/* 

now initialising the heap.
Heap init complete
The string: Are strings working ??.

floats: 3.14159
doubles: 2.71828

vecfloat1 = 3.14159 vecfloat2 = 2.71828
1 2 3 
: 1: 2: 3: 4: 4: 4

1 2 3 4 4 4 5 6 7 

My string is Are strings working ??

Member of vec 0 = 0
Member of vec 1 = 2
Member of vec 2 = 4
Member of vec 3 = 6
Member of vec 4 = 8
Member of vec 5 = 10
Member of vec 6 = 12
Member of vec 7 = 14
Member of vec 8 = 16
Member of vec 9 = 18
Member of vec 10 = 20
Member of vec 11 = 22
Member of vec 12 = 24
Member of vec 13 = 26
Member of vec 14 = 28
Member of vec 15 = 30
Member of vec 16 = 32
Member of vec 17 = 34
Member of vec 18 = 36
Member of vec 19 = 38
 Just read: Hello, what am I up to now?  ... So there you go
Program end


*/

#include <dolphin.h>
#include <stddef.h>
#include <libsn.h>
#include <stdlib.h>


#include <stdio.h>


#include <string>
#include <vector>
#include <memory>
#include <list>
#include <iostream>
#include <fstream>

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

extern "C" void free_r (void* ptr)
{
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



using namespace std;

int main (void)
{
	int t;
	long l;
	float f;
	double dd;
	_register_malloc = mymalloc;
	_register_free = myfree;
	list<int>::iterator i;
	int rgTest1[] = {5,6,7};
	int rgTest2[] = {10,11,12};
	int x;

	std::string str;
	std::vector<int> vec;
	list<int> klist;
	vector<float> vecfloat;

	PCinit();

	OSInit();
	cout << "\nnow initialising the heap.\n";
    	CPPInit();
	cout << "Heap init complete";


	str = "Are strings working ??";

	f = 3.141592653959;
	dd = 2.7182818284590452353602874713527;

	// Insert one at a time
	klist.insert (klist.begin(), 2);
	klist.insert (klist.begin(), 1);
	klist.insert (klist.end(), 3);

	cout << "\nThe string: " << str << ".\n";

	cout << "\nfloats: " << f << "\ndoubles: " << dd << "\n";

	vecfloat.push_back (f);
	vecfloat.push_back (dd);

	cout << "\nvecfloat1 = " << vecfloat[0] << " vecfloat2 = " << vecfloat[1] << "\n";


    // 1 2 3
    for (i = klist.begin(); i != klist.end(); ++i)
        printf("%d ",*i);
    printf("\n");

    // Insert 3 fours
    klist.insert (klist.end(), 3, 4);

    // 1 2 3 4 4 4
    for (i = klist.begin(); i != klist.end(); ++i)
	{
        cout << ": " << *i;
	}
    cout << "\n\n";


    // Insert an array in there
    klist.insert (klist.end(), rgTest1, rgTest1 + 3);

    // 1 2 3 4 4 4 5 6 7
    for (i = klist.begin(); i != klist.end(); ++i)
        printf("%d ",*i);
    printf("\n");

	//Populate a vector
	for (t=0;t<20;t++)
	{
		//x = new int;
		x = t*2;
		vec.push_back (x);
	}

	cout << "\nMy string is " << str << "\n";

	for (t=0;t<vec.size();t++)
	{
		cerr << "\nMember of vec " << t << " = " << vec[t];
	}

	//Open a file
	ofstream mystream("data.txt",ios::out);

	if (mystream.bad())
	{
		cerr << "\nI/O failure while opening data.txt";
	}
	else
	{
		str = "Hello, what am I up to now?\0";
		mystream.write( str.c_str(), str.size() );
		mystream.close();
	}

	//Open a file
	ofstream mystream2("data2.txt",ios::out);

	if (mystream2.bad())
	{
		cerr << "\nI/O failure while opening data2.txt";
	}
	else
	{
		mystream2.close();
	}

	//Now try loading the file back in and displaying the content
	ifstream instream("data.txt",ios::in);

	if (instream.bad())
	{
		cerr << "\nI/O failure while opening data.txt";
	}
	else 
	{
		char ch[50];
		memset (ch,0,50);
		str = "";
		instream.read( ch, 50 );
		instream.close();
		str = string(ch);
		//Stick a bit on the end
		str += "  ... So there you go";
		cout << "\n Just read: " << str;
	}

	printf("\nProgram end");

	for(;;)
	{
		snPollHost();
	}; //never return
	return 0; //Never get here.
}