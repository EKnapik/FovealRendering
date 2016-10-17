#pragma once
#include <eyex/EyeX.h>

#pragma comment (lib, "Tobii.EyeX.Client.lib")


/*
	Get's an eye tracker and has methods to return the current
	viewing position on the screen.
*/
class EyeTracker
{
public:
	EyeTracker();
	~EyeTracker();
};

