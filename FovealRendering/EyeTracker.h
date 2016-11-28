/*
* Code used from Tobii Technology
* Copyright 2013-2014 Tobii Technology AB. All rights reserved.
*/

#ifndef TOBII_EYETRACKER
#define TOBII_EYETRACKER

#define TOLLERANCE 7
#define X_OFFSET 0
#define Y_OFFSET 0
#define ScreenHeight 1800
#define ScreenWidth 2880

#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <assert.h>
#include "eyex/EyeX.h"

#pragma comment (lib, "Tobii.EyeX.Client.lib")

// Initalization of global idetifieres for this eye tracker.
static const TX_STRING InteractorId = "Twilight Sparkle";
static TX_HANDLE g_hGlobalInteractorSnapshot = TX_EMPTY_HANDLE;
// Need global data passing
static float tobiiEyeEventXParam = 0;
static float tobiiEyeEventYParam = 0;
static float tobiiPrevXPos = 99999;
static float tobiiPrevYPos = 99999;

BOOL InitializeGlobalInteractorSnapshot(TX_CONTEXTHANDLE hContext);
void TX_CALLCONVENTION OnSnapshotCommitted(TX_CONSTHANDLE hAsyncData, TX_USERPARAM param);
void TX_CALLCONVENTION OnEngineConnectionStateChanged(TX_CONNECTIONSTATE connectionState, TX_USERPARAM userParam);
void OnGazeDataEvent(TX_HANDLE hGazeDataBehavior);
void TX_CALLCONVENTION HandleEvent(TX_CONSTHANDLE hAsyncData, TX_USERPARAM userParam);


class EyeTracker
{

public:
	EyeTracker(int width, int height);
	~EyeTracker();
	float GetXPos() { return (*curXPos / ScreenWidth * (float) width) + X_OFFSET; }
	float GetYPos() { return (*curYPos / ScreenHeight * (float) heigth) + Y_OFFSET; }

private:
	int width, heigth;
	TX_CONTEXTHANDLE hContext;
	float* curXPos;
	float* curYPos;
};

#endif // !TOBII_EYETRACKER