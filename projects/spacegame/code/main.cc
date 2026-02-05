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

struct TestStruct
{
	bool a;
	bool b;
};

int main(int argc, const char** argv)
{
	Game::SpaceGameApp app;

	//Arena arena = Arena(10);

	if (app.Open())
	{
		app.Run();
		app.Close(); 
	}
	app.Exit();

	//delete pool;
	//MemoryManager::ReportMemoryAllocation();
}