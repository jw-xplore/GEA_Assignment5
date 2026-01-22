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

	MemoryPool* pool = MemoryManager::CreateMemoryPool();

	if (app.Open())
	{
		int* val = (int*)MemoryManager::Alloc<int>(pool);
		std::cout << "Val address: " << val << "\n";

		app.Run();
		app.Close(); 
	}
	app.Exit();
	
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	//_CrtDumpMemoryLeaks();

	MemoryManager::ReportMemoryAllocation();
}