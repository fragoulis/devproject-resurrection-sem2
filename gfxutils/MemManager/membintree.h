#pragma once

#include "../MemManager/MemMgr_AnyData.h"
//#include <Threading/CriticalSection.h>


struct MemTreeInfo
{
	unsigned maxBytes,			// maximum allocatable bytes
			 allocatedBytes,	// currently allocated bytes
			 maxDepth,			// max tree depth
			 nodeNum,			// tree node number
			 totalAlloc,		// total byte allocation since last clear
			 totalFree,			// total freed bytes since last clear
			 peakAlloc;			// peak allocation 
	MemTreeInfo():maxBytes(0),allocatedBytes(0),maxDepth(0),
		nodeNum(0),totalAlloc(0),totalFree(0),peakAlloc(0){}
};

enum TreeNode_Status
{
	FREE = 0,	// completely free, no children
	SPLIT,		// somewhat free, has children
	OCCUPIED,	// occupied with mem, no children
	CH_OCCUPIED	// children occupied, has children
};

struct TreeNode
{
	TreeNode	  * parent,		// node parent
				  * left,		// left child
				  *	right;		// right child
	TreeNode_Status status;		// allocation status
	char 		  *	data;		// starting data address
	unsigned		size;		// allocated size ( only in occupied )

	TreeNode(const unsigned sz,TreeNode * par,char * dat):				// ctor
	size(sz),parent(par),left(0),right(0),status(FREE),data(dat){}

	char * allocate(const unsigned size,const unsigned minsize);		// allocate size bytes, minsize is minimum
	unsigned free(char * p);											// frees if OCCUPIED & with data = p

	void split();														// creates children
	void collectInfo(MemTreeInfo& info) const;							// recursive info-collection for status

	void * operator new (const unsigned sz)								// uses MemMgrAny for node dynamic mem management
	{
		return MemMgrAny<TreeNode>::instance()->allocate();
	}

	void operator delete (void * p)
	{
		MemMgrAny<TreeNode>::instance()->free((TreeNode *)p);
		p = 0;
	}
};

class ParserSection;
class FixedMemTree
{
	MemTreeInfo _info;		// status structure
	TreeNode *_root;		// tree root node

	unsigned _leafSize;		// minimum size a node can have ( no split after that )

	//CriticalSection _cs;

	void _collectInfo();	// fills _info

public :
	FixedMemTree(const ParserSection * parsec,			// ctor
				 const unsigned maxsize,
				 const unsigned minsize,char * data); 
	~FixedMemTree();									// dtor

	char * allocate(const unsigned size);				// allocates size bytes
	void free(char * p);								// frees data node p
	void clear();										// clears memory

	MemTreeInfo& getStatus(); // Return MemStatus struct, similar to StackStatus but adds wasted
	void clearStatus();		  // clears status
};

