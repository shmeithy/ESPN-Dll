#pragma once
#include <iostream>
#include "stdafx.h"
#include < stdio.h >
#include < stdlib.h >
#include <stdexcept>
#include "ESPNDll.h"

#using "System.dll"
#using "System.Xml.dll"

using namespace System;
using namespace System::Net;
using namespace System::Xml;
using namespace std;
namespace ESPN
{
	
	class SportsScores
	{
	public:
		
		static __declspec(dllexport) void SetUp();
		//static __declspec(dllexport) void ReadData();
		static __declspec(dllexport) void PrintTeams(int counter);
		static __declspec(dllexport) void ParseData(char* data, int start);
		static __declspec(dllexport) void ParseTeam(char* data);
		static __declspec(dllexport) int FindString(char* data,int size, size_t currentPlace);



	};

}