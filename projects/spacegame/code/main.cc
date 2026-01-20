//------------------------------------------------------------------------------
// main.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "spacegameapp.h"
#include <iostream>
#include <new>
#include <map>

//-------------------------------------------------------------------------
// Memory manager test
//-------------------------------------------------------------------------

uint objectsCount;
uint allocatedMemoryForObjects;
uint arraysCount;
uint allocatedMemoryForArrays;

void* operator new(std::size_t size)
{
	if (void* p = std::malloc(size))
	{
		objectsCount++;
		allocatedMemoryForObjects += size;
		return p;
	}

	throw std::bad_alloc{};
}

void operator delete(void* p) noexcept
{
	objectsCount--;
	allocatedMemoryForObjects -= sizeof(p);
	std::free(p);
}

void* operator new[](std::size_t size) throw(std::bad_alloc)
{
	if (void* p = std::malloc(size))
	{
		arraysCount++;
		allocatedMemoryForArrays += size;
		return p;
	}

	throw std::bad_alloc{};
}
void operator delete[](void* p) throw()
{
	arraysCount--;
	allocatedMemoryForObjects -= sizeof(p);
	std::free(p);
}

//-------------------------------------------------------------------------
// Main
//-------------------------------------------------------------------------

int main(int argc, const char** argv)
{
	Game::SpaceGameApp app;
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();
	
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	//_CrtDumpMemoryLeaks();

	std::cout << "----------------------------------------------------" << "\n";
	std::cout << "Leaked objects: " << objectsCount << ", size: " << allocatedMemoryForObjects << "\n";
	std::cout << "Leaked arrays: " << arraysCount << ", size: " << allocatedMemoryForArrays;
}