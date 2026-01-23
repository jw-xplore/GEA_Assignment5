//------------------------------------------------------------------------------
// main.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "spacegameapp.h"
#include <iostream>
#include "../engine/core/MemoryManager.h"

//-------------------------------------------------------------------------
// Main
//-------------------------------------------------------------------------

int main(int argc, const char** argv)
{
	Game::SpaceGameApp app;

	MemoryPool pool = MemoryPool();

	if (app.Open())
	{
		bool* val1 = (bool*)MemoryManager::Alloc<bool>(pool);
		bool* val2 = (bool*)MemoryManager::Alloc<bool>(pool);
		bool* val3 = (bool*)MemoryManager::Alloc<bool>(pool);
		bool* val4 = (bool*)MemoryManager::Alloc<bool>(pool);
		bool* val5 = (bool*)MemoryManager::Alloc<bool>(pool);
		float* f = (float*)MemoryManager::Alloc<float>(pool);
		MemoryManager::FreeMemoryPool(pool);
		float* f2 = (float*)MemoryManager::Alloc<float>(pool);

		//int* val2 = (int*)MemoryManager::Alloc<int>(pool);
		//int* val3 = (int*)MemoryManager::Alloc<int>(pool);

		app.Run();
		app.Close(); 
	}
	app.Exit();

	//delete pool;
	MemoryManager::ReportMemoryAllocation();
}