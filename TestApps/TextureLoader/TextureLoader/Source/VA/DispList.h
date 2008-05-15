#ifndef __DISPLIST_H__
#define __DISPLIST_H__

#include <dolphin.h>

#define DLIST_MEM (u32)65536

class DispList
{
	void * m_data;
	u32 m_size;
	bool m_filled;

public :

	DispList():m_data(0),m_size(0),m_filled(false)
	{
		m_data = OSAlloc(DLIST_MEM);
	}

	~DispList()
	{
		OSFree(m_data);
	}

	void start() 
	{
		DCInvalidateRange(m_data,DLIST_MEM);
		GXBeginDisplayList(m_data,DLIST_MEM);
	}

	void end() 
	{
		m_size = GXEndDisplayList();
		m_filled = true;
		void * real_data = OSAlloc(m_size);
		ASSERTMSG(real_data != NULL, "error allocating actual data");
		memcpy( real_data, m_data, m_size);
		DCFlushRange( real_data, m_size);
		OSFree(m_data);
		m_data = real_data;
	}

	void call() const
	{
		GXCallDisplayList(m_data,m_size);
	}

	bool isFilled() const {return m_filled;}

	const void * data() const {return m_data;}
};

#endif