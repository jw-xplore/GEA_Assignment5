#pragma once

#include <memory>
#include <iostream>
#include <mutex>
#include <thread>

//-------------------------------------------------------------------------
// Base memory manager
//-------------------------------------------------------------------------

const size_t POOL_SIZE = 65536;

class Arena
{
private:
	char* buffer;
	size_t size;
	unsigned int allocated;

	Arena* prevArena = nullptr;
	Arena* nextArena = nullptr;
public:

	Arena(size_t s = POOL_SIZE)
	{
		buffer = new char[s];
		size = s;
		allocated = 0;

		std::cout << "Buffer: " << &buffer << "\n";
		std::cout << "This: " << this << "\n";
		std::cout << "-------------------------------------\n";
	}
	
	~Arena()
	{
		delete nextArena;
	}

	template<class T>
	void* Alloc(unsigned int count = 1)
	{
		// Check element size out of bounce
		size_t elSize = sizeof(T);
		size_t completeSize = elSize * count;

		if (completeSize > this->size)
		{
			throw std::invalid_argument("Memory block is larger than arena size");
			return nullptr;
		}

		// Check element allignment
		size_t alignment = alignof(T);
		int allignmentOverflow = allocated % alignment;

		if (allignmentOverflow != 0)
		{
			allignmentOverflow = size - allignmentOverflow;
			allocated += allignmentOverflow;
		}

		// Check element out of bounce
		int endPos = allocated + elSize * count;
		void* start = this + allocated;

		if (endPos > this->size)
		{
			if (nextArena == nullptr)
			{
				// Create next pool
				nextArena = new Arena(this->size);
				nextArena->prevArena = this;
				start = this;
			}

			// Get next free pool
			return nextArena->Alloc<T>(count);
		}

		allocated += elSize;
		return start;
	}

	void Reset(bool removeLinks = true)
	{
		allocated = 0;

		if (removeLinks)
		{
			// Delete all linked arenas
			if (nextArena != nullptr)
				delete nextArena;

			nextArena = nullptr;
			return;
		}

		// Keep linked arenas and restert their position
		if (nextArena != nullptr)
			nextArena->Reset(removeLinks);
	}
};

/*
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
}
*/

//-------------------------------------------------------------------------
// Tracking memory override
//-------------------------------------------------------------------------

/*
std::mutex allocMtx;

void* operator new(std::size_t size)
{
	std::unique_lock<std::mutex> allocLock(allocMtx);

	if (void* p = std::malloc(size))
	{
		MemoryManager::objectsCount++;
		MemoryManager::allocatedMemoryForObjects += size;
		return p;
	}

	throw std::bad_alloc{};

	allocLock.unlock();
}

void operator delete(void* p) noexcept
{
	MemoryManager::objectsCount--;
	MemoryManager::allocatedMemoryForObjects -= sizeof(p);
	std::free(p);
}

void* operator new[](std::size_t size) throw(std::bad_alloc)
{
	std::unique_lock<std::mutex> allocLock(allocMtx);

	if (void* p = std::malloc(size))
	{
		MemoryManager::arraysCount++;
		MemoryManager::allocatedMemoryForArrays += size;
		return p;
	}

	throw std::bad_alloc{};

	allocLock.unlock();
}
void operator delete[](void* p) throw()
{
	MemoryManager::arraysCount--;
	MemoryManager::allocatedMemoryForObjects -= sizeof(p);
	std::free(p);
}
*/

//-------------------------------------------------------------------------
// Memory pool allocator
//-------------------------------------------------------------------------

template <typename T>
class PoolAllocator
{
public:
	T* buffer;
	//size_t size;
	size_t elementSize;
	size_t elementsCount;

	size_t usedCount = 0;
	T** handles;

	PoolAllocator(size_t count)
	{
		usedCount = 0;
		elementsCount = count;
		elementSize = sizeof(T);

		//size = elementsCount * elementSize;
		buffer = new T[elementsCount];
		handles = new T*[elementsCount];

		for (size_t i = 0; i < elementsCount; i++)
		{
			T* element = buffer + i;
			handles[i] = element;
		}
	}

	T* Allocate()
	{
		if (usedCount >= elementsCount)
			return nullptr;

		T* pos = handles[usedCount];
		usedCount++;

		return pos;
	}

	void Remove(T* element)
	{
		// Find element
		int pos = -1;
		for (size_t i = 0; i < elementsCount; i++)
		{
			if (handles[i] == element)
			{
				pos = i;
				break;
			}
		}

		if (pos == -1)
			return;

		// Decrease used and switch element position
		usedCount--;

		if (pos < usedCount)
		{
			T* temp = handles[pos];
			handles[pos] = handles[usedCount];
			handles[usedCount] = temp;
		}
	}

	const T& operator[](std::size_t idx) const
	{
		return handles[idx];
	}

	T& at(size_t idx)
	{
		return *handles[idx];
	}
};