#pragma once

#include <memory>
#include <iostream>

//-------------------------------------------------------------------------
// Base memory manager
//-------------------------------------------------------------------------

const size_t POOL_SIZE = 65536;

struct MemoryPool
{
	char* buffer;
	size_t size;
	unsigned int allocated;

	MemoryPool* prevPool = nullptr;
	MemoryPool* nextPool = nullptr;

	MemoryPool(size_t s = POOL_SIZE)
	{
		buffer = new char[s];
		size = s;
		allocated = 0;
	}
	
	~MemoryPool()
	{
		delete nextPool;
	}
};

namespace MemoryManager
{
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

	template<class T>
	void* Alloc(MemoryPool& pool, unsigned int count = 1)
	{
		size_t size = sizeof(T);

		// Check size out of bounce
		if (sizeof(T) > pool.size)
		{
			throw std::invalid_argument("Object is larger than pool size");
			return nullptr;
		}

		// Check allignment
		size_t alignment = alignof(T);
		int allignmentOverflow = pool.allocated % alignment;
		if (allignmentOverflow != 0)
		{
			allignmentOverflow = size - allignmentOverflow;
			pool.allocated += allignmentOverflow;
		}

		void* start = &pool + pool.allocated;

		// Check element out of bounce
		int endPos = pool.allocated + size * count;

		if (endPos > pool.size)
		{
			if (pool.nextPool == nullptr)
			{
				// Create next pool
				pool.nextPool = new MemoryPool(pool.size);
				pool.nextPool->prevPool = &pool;
				start = &pool;
			}
			
			// Get next free pool
			return Alloc<T>(*pool.nextPool, count);
		}
		
		pool.allocated += sizeof(T);
		return start;
	}

	void FreeMemoryPool(MemoryPool& pool)
	{
		pool.allocated = 0;

		if (pool.nextPool != nullptr)
			FreeMemoryPool(*pool.nextPool);
	}

	void ResetMemoryPool(MemoryPool& pool)
	{
		pool.allocated = 0;

		if (pool.nextPool != nullptr)
			delete pool.nextPool;
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
