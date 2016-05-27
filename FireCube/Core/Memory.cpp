#include "Core/Memory.h"

using namespace FireCube;

RefCount::RefCount() : refs(0), weakRefs(0)
{

}

RefCount::~RefCount()
{
	refs = weakRefs = -1;
}


RefCounted::RefCounted() : refCount(new RefCount)
{
	refCount->weakRefs++;
}

RefCounted::~RefCounted()
{
	refCount->refs = -1;
	refCount->weakRefs--;
	if (refCount->weakRefs == 0)
		delete refCount;
}

void RefCounted::AddRef()
{
	refCount->refs++;
}

void RefCounted::ReleaseRef()
{
	refCount->refs--;
	if (refCount->refs == 0)
		delete this;

}

RefCount *RefCounted::GetRefCountObject()
{
	return refCount;
}

int RefCounted::GetRefCount() const
{
	return refCount->refs;
}
