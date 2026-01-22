#pragma once

#include <memory>
#include <iostream>

//-------------------------------------------------------------------------
// Base memory manager
//-------------------------------------------------------------------------

struct MemoryPool
{
	//void* memory;
	size_t size;
	unsigned int allocated;

	MemoryPool(size_t s)
	{
		size = s;
		//memory = new void[size];
	}
};

namespace MemoryManager
{
	const size_t POOL_SIZE = 65536;

	unsigned int objectsCount;
	unsigned int allocatedMemoryForObjects;
	unsigned int arraysCount;
	unsigned int allocatedMemoryForArrays;

	void ReportMemoryAllocation()
	{
		std::cout << "----------------------------------------------------" << "\n";
		std::cout << "MEMORY MANAGER REPORT:\n";
		std::cout << "Allocated objects: " << objectsCount << ", size: " << allocatedMemoryForObjects << "\n";
		std::cout << "Allocated arrays: " << arraysCount << ", size: " << allocatedMemoryForArrays << "\n";
		std::cout << "----------------------------------------------------" << "\n";
	}

	//-------------------------------------------------------------------------
	// Memory pool
	//-------------------------------------------------------------------------

	MemoryPool* CreateMemoryPool(size_t size = POOL_SIZE)
	{
		MemoryPool* pool = new MemoryPool(size);
		return pool;
	}

	void FreeMemoryPool(MemoryPool* pool)
	{
		delete pool;
	}

	template<class T>
	void* Alloc(MemoryPool* pool)
	{
		void* start = pool + pool->allocated;
		pool->allocated += sizeof(T);

		return start;
	}

	void Clear(MemoryPool* pool)
	{
		pool->allocated = 0;
	}
}

//-------------------------------------------------------------------------
// Tracking memory override
//-------------------------------------------------------------------------

void* operator new(std::size_t size)
{
	if (void* p = std::malloc(size))
	{
		MemoryManager::objectsCount++;
		MemoryManager::allocatedMemoryForObjects += size;
		return p;
	}

	throw std::bad_alloc{};
}

void operator delete(void* p) noexcept
{
	MemoryManager::objectsCount--;
	MemoryManager::allocatedMemoryForObjects -= sizeof(p);
	std::free(p);
}

void* operator new[](std::size_t size) throw(std::bad_alloc)
{
	if (void* p = std::malloc(size))
	{
		MemoryManager::arraysCount++;
		MemoryManager::allocatedMemoryForArrays += size;
		return p;
	}

	throw std::bad_alloc{};
}
void operator delete[](void* p) throw()
{
	MemoryManager::arraysCount--;
	MemoryManager::allocatedMemoryForObjects -= sizeof(p);
	std::free(p);
}
