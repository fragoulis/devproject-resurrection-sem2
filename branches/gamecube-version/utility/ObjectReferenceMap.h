#pragma once
#include <map>
#include <string>


/**
 * Helper class that wraps around std::map<string, T*>
 */

template< typename T >
class ObjectReferenceMap
{
public:
	ObjectReferenceMap() { }
	~ObjectReferenceMap() { }


	void addObjectReference(T* s, const std::string& name) {
		m_references.insert(ObjectPointerMap::value_type(name, s));
	}
	void removeObjectReference(const std::string& name) {
		m_references.erase(name);
	}

	T* findObjectReference(const std::string& name) {
		ObjectPointerMap::iterator i = m_references.find(name);
		if (i == m_references.end()) return NULL;
		return i->second;
	}
	const T* findObjectReference(const std::string& name) const {
		ObjectPointerMap::const_iterator i = m_references.find(name);
		if (i == m_references.end()) return NULL;
		return i->second;
	}

	void clear() { m_references.clear(); }



	template< typename ObjectVisitor >
	void visitObjects(ObjectVisitor ov)
	{
		for (ObjectPointerMapIt i = m_references.begin(); i != m_references.end(); i++)
		{
			ov(i->first, i->second);
		}
	}

	template< typename ObjectVisitor >
	void visitObjects(ObjectVisitor ov) const
	{
		for (ObjectPointerMap::const_iterator i = m_references.begin(); i != m_references.end(); i++)
		{
			ov(i->first, i->second);
		}
	}


private:
	typedef std::map<const std::string, T*> ObjectPointerMap;
	typedef typename ObjectPointerMap::iterator ObjectPointerMapIt;
	ObjectPointerMap m_references;
};
