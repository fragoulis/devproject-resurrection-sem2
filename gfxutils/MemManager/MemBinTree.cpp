//*****************************************************************************
//**
//**   File:               MemBinTree.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#include "MemBinTree.h"
#include "../ConfParser/ConfParser.h"
#include "../Misc/utils.h"


FixedMemTree :: FixedMemTree(const ParserSection * parsec,
							 const unsigned maxsize,
							 const unsigned minsize,char * data) // Init MemMgrAny<TreeNode> in constructor 
:_leafSize(minsize),
_root(NULL)
{
	MemMgrAny<TreeNode>::init("FixedMemTree",parsec);
	_root = new TreeNode(maxsize,NULL,data);
	_info.maxBytes = maxsize;
	_leafSize = minsize;
}

FixedMemTree :: ~FixedMemTree()	// Delete MemMgrAny<TreeNode> in destructor
{
	clear();
	MemMgrAny<TreeNode>::destroy();
}

char * FixedMemTree :: allocate(const unsigned size)
{
	assert(size < _root->size);

	//_cs.enter();

	const unsigned actual_size = nearestPOT(size);
#ifdef _DEBUG
	_info.totalAlloc += actual_size;
	_info.allocatedBytes += actual_size;
	_info.peakAlloc = MAX(_info.peakAlloc, _info.totalAlloc - _info.totalFree);
#endif
	char * ptr =  _root->allocate(actual_size,_leafSize);
	//_cs.leave();

	return ptr;
}

void FixedMemTree :: free(char * p)
{
	assert((p >= _root->data) && (p < &_root->data[_root->size]));
	//_cs.enter();
	const unsigned freed = _root->free(p);
	//_cs.leave();
#ifdef _DEBUG
	_info.totalFree += freed;
	_info.allocatedBytes -= freed;
#endif
	
}

void FixedMemTree :: clear()
{
	//_cs.enter();
	char * dat = _root->data;
	unsigned sz = _root->size;
	MemMgrAny<TreeNode>::instance()->clear();
	_root = NULL;
	_root = new TreeNode(sz,NULL,dat);
	//_cs.leave();
}

MemTreeInfo& FixedMemTree :: getStatus() // Return MemStatus struct, similar to StackStatus but adds wasted
{
	// Traverse & collect data
	_collectInfo();
	return _info;
} 

void FixedMemTree :: clearStatus()
{
	const unsigned maxbytes = _info.maxBytes;
	_info = MemTreeInfo();
	_info.maxBytes = maxbytes;
}

void FixedMemTree :: _collectInfo()
{
	_info.nodeNum = 0;
	_info.maxDepth = unsigned(-1);	// just set a max value
	_root->collectInfo(_info);
	_info.maxDepth = unsigned(math_const<float>::log2(float(_info.maxBytes / _info.maxDepth)));
}


/********************* TreeNode **************************/

char * TreeNode :: allocate(const unsigned sz,const unsigned minsize)
{
	if(status >= OCCUPIED)
		return 0;
	else
	{ 
		const bool need_split = ((size>>1) >= sz);
		if (status == FREE)
		{
			if((size != minsize) && need_split)
			{
				split();
				status = SPLIT;
				return left->allocate(sz,minsize);
			}
			else
			{
				status = OCCUPIED;
				return data;
			}
		}
		else
			if(! need_split)
				return 0;
		assert(status != FREE);
		char * mem = left->allocate(sz,minsize);
		assert(status != FREE);
		if(!mem)
			mem = right->allocate(sz,minsize);
		assert(status != FREE);
		if((left->status >= OCCUPIED) && 
		  (right->status >= OCCUPIED))
			status = CH_OCCUPIED;
		return mem;	
	}
}

unsigned TreeNode :: free(char * ptr)
{
	// Traverse down to free
	unsigned freed = 0;
	TreeNode * curnode = this;
	while(true)
	{
		assert(curnode->status != FREE);
		if(curnode->status == OCCUPIED)
		{
			freed += curnode->size;
			curnode->status = FREE;
			curnode = curnode->parent;
			break;
		}
		else
		{
			if (ptr < curnode->right->data)
			{
				curnode = curnode->left;
				assert(curnode->status != FREE);
			}
			else 
			{
				curnode = curnode->right;
				assert(curnode->status != FREE);
			}
		}
	}
	// Now traverse up to merge
	while(curnode)
	{
		if((curnode->left->status == curnode->right->status) &&
			(curnode->left->status == FREE))
		{
			assert(curnode->left->status == FREE);
			curnode->status = FREE;
			delete curnode->left;
			curnode->left = 0;
			delete curnode->right;
			curnode->right = 0;
			curnode = curnode->parent;
		}
		else
		{
			if(curnode->status != SPLIT)
			{
				curnode->status = SPLIT;
				curnode = curnode->parent;
			}
			else
				break;
		}
	}
	return freed;
}

void TreeNode :: split()
{
	const unsigned halfsz = size>>1;
	left = new TreeNode(halfsz,this,data);
	right = new TreeNode(halfsz,this,&data[halfsz]);
}

void TreeNode :: collectInfo(MemTreeInfo& info) const
{
	++info.nodeNum;
	if(!left)
		info.maxDepth = MIN(info.maxDepth,size);
	else
	{
		left->collectInfo(info);
		right->collectInfo(info);
	}
}