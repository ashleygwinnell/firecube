#pragma once

#include "Utils/utils.h"

namespace FireCube
{

class FIRECUBE_API RefCount
{
public:	
	RefCount();
	~RefCount();

	int refs;
	int weakRefs;
};

class FIRECUBE_API RefCounted
{
public:
	RefCounted();
	virtual ~RefCounted();
	void AddRef();
	void ReleaseRef();
	RefCount *GetRefCountObject();
private:
	RefCount *refCount;
};

template <class T>
class SharedPtr
{
public:
	SharedPtr() : ptr(nullptr)
	{
		
	}

	SharedPtr(T *ptr) : ptr(ptr)
	{
		AddRef();
	}

	SharedPtr(const SharedPtr<T> &rhs) : ptr(rhs.ptr)
	{
		AddRef();
	}

	~SharedPtr()
	{
		ReleaseRef();
	}

	SharedPtr<T> &operator = (const SharedPtr<T> rhs)
	{
		if (rhs.ptr == ptr)
			return *this;
		
		ReleaseRef();
		ptr = rhs.ptr;
		AddRef();
		return *this;
	}

	void Reset() 
	{ 
		ReleaseRef(); 
	}

	T* Get() const 
	{ 
		return ptr; 
	}

	RefCount *GetRefCountObject() const
	{
		return ptr ? ptr->GetRefCountObject() : nullptr;
	}

	template <class U> void StaticCast(const SharedPtr<U> &rhs)
	{
		ReleaseRef();
		ptr = static_cast<T*>(rhs.Get());
		AddRef();
	}
	
	template <class U> void DynamicCast(const SharedPtr<U> &rhs)
	{
		ReleaseRef();
		ptr = dynamic_cast<T*>(rhs.Get());
		AddRef();
	}

	T* operator -> () const 
	{
		return ptr; 
	}
	
	T& operator * () const 
	{ 
		return *ptr; 
	}
	
	T& operator [] (const int index) 
	{ 		
		return ptr[index]; 
	}
	
	bool operator < (const SharedPtr<T> &rhs) const 
	{ 
		return ptr < rhs.ptr; 
	}
	
	bool operator == (const SharedPtr<T> &rhs) const 
	{ 
		return ptr == rhs.ptr; 
	}
	
	bool operator != (const SharedPtr<T> &rhs) const 
	{ 
		return ptr != rhs.ptr; 
	}
	
	operator T* () const 
	{ 
		return ptr; 
	}
private:

	void AddRef()
	{
		if (ptr)
			ptr->AddRef();
	}

	void ReleaseRef()
	{
		if (ptr)
		{
			ptr->ReleaseRef();
			ptr = nullptr;
		}
	}

	T *ptr;
};

template <class T>
class WeakPtr
{
public:
	WeakPtr() : ptr(nullptr), refCount(nullptr)
	{

	}

	WeakPtr(T *ptr) : ptr(ptr), refCount(ptr ? ptr->GetRefCountObject() : nullptr)
	{
		AddRef();
	}

	~WeakPtr()
	{
		ReleaseRef();
	}

	bool Expired() const 
	{ 
		return refCount ? refCount->refs < 0 : true; 
	}

	T* Get() const
	{
		if (Expired())
			return nullptr;
		else
			return ptr;
	}

	SharedPtr<T> Lock() const
	{
		if (Expired())
			return SharedPtr<T>();
		else
			return SharedPtr<T>(ptr);
	}

	template <class U> void StaticCast(const WeakPtr<U> &rhs)
	{
		ReleaseRef();
		ptr = static_cast<T*>(rhs.Get());
		refCount = rhs.refCount;
		AddRef();
	}
	
	template <class U> void DynamicCast(const WeakPtr<U> &rhs)
	{
		ReleaseRef();
		ptr = dynamic_cast<T*>(rhs.Get());

		if (ptr)
		{
			refCount = rhs.refCount;
			AddRef();
		}
		else
			refCount = 0;
	}

	WeakPtr<T>& operator = (const SharedPtr<T> &rhs)
	{
		if (ptr == rhs.Get() && refCount == rhs.GetRefCountObject())
			return *this;

		ReleaseRef();
		ptr = rhs.Get();
		refCount = rhs.GetRefCountObject();
		AddRef();

		return *this;
	}

	WeakPtr<T>& operator = (T *ptr)
	{
		RefCount *refCount = ptr ? ptr->GetRefCountObject() : nullptr;

		if (this->ptr == ptr && this->refCount == refCount)
			return *this;

		ReleaseRef();
		this->ptr = ptr;
		this->refCount = refCount;
		AddRef();

		return *this;
	}

	operator T* () const 
	{ 
		return Get(); 
	}

	T* operator -> () const
	{		
		return Get();
	}
private:

	void AddRef()
	{
		if (refCount)
		{
			refCount->weakRefs++;
		}
	}

	void ReleaseRef()
	{
		if (refCount)
		{
			refCount->weakRefs--;
			if (Expired() && refCount->weakRefs == 0)
				delete refCount;
		}

		ptr = nullptr;
		refCount = nullptr;
	}

	T *ptr;
	RefCount *refCount;

};

}