// ESPN_Scores.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include < stdio.h >
#include < stdlib.h >
#include < vcclr.h >
#include "stdafx.h"

#include "ESPNDll.h"
//#using "ESPNDLL.dll"
#using <mscorlib.dll>
#using "System.dll"
#using "System.Xml.dll"

using namespace System;
using namespace System::Net;
using namespace System::Xml;

#pragma comment(lib, "ESPNDll.lib")

using namespace std;
//using namespace ESPN;
int stupidNumber = 19;
int counter = 1;

int findEnd(char* data)
{
	int num = 0;
	int returnVal=0;
	string s = data;
	string search = "</TeamRecord>";
	//printf("\ndata: %s\n\n", data);
	printf("counter: %d\n", counter);
	num = s.find(search);
	char t = s[num];
	if(s[num+12] == '>')
	{
		if(counter == 2)
		{
			num += 12;
			//stupidNumber++;
		}
		else if(counter == 20)
		{
			num += stupidNumber + 8;
		}
		else
		{
			num += stupidNumber;
			stupidNumber++;
		}
		
	}
	counter++;
	return num;
}


int main(array<System::String ^> ^args)
{				
	ESPN::SportsScores::SetUp();
	//ESPN::SportsScores::ReadData();
	
	//http://api.sportsdatallc.org/nba-t1/standings?season_id=2011&api_key=sjuubg782chjuuppth7nxgnk
	System::Net::HttpWebRequest^ myRequest = dynamic_cast<HttpWebRequest^>(WebRequest::Create( "http://api.sportsdatallc.org/nba-t1/standings?season_id=2011&api_key=54exz7sw2tqszdffjxcqy5gx" ));
	System::Net::WebResponse^ myResponse = myRequest->GetResponse();

	System::IO::Stream^ ReceiveStream = myResponse->GetResponseStream();

	System::Text::Encoding^ encode = System::Text::Encoding::GetEncoding( "utf-8" );

	// Pipe the stream to a higher level stream reader with the required encoding format.
	System::IO::StreamReader^ readStream = gcnew System::IO::StreamReader( ReceiveStream,encode );
	Console::WriteLine( "\nResponse stream received" );
	

	int next = 533;
	int test = 0;
	array<Char>^ read = gcnew array<Char>(600);
	// Read 256 charcters at a time.
	int count = readStream->Read( read, 0, 569 );
	Console::WriteLine( "HTML...\r\n" );
	bool firstPass = true;
	while ( count > 0 && counter<29)
	{
	   // Dump the 256 characters on a string and display the string onto the console.
	   String^ str = gcnew String( read,0,count );
	   pin_ptr<const wchar_t> wch = PtrToStringChars(str);
	   size_t sizeInBytes = ((str->Length + 1) *2);
	   char* data = (char* )malloc(sizeInBytes);
	   errno_t err = 0;
	   size_t convertedChars = 0;
	   err = wcstombs_s(&convertedChars, data, sizeInBytes, wch, sizeInBytes);
	   if(err != 0)
		   printf("copying failed!\n");

	   if(!firstPass)
	   {
			
			ESPN::SportsScores::ParseData(data, next);
			next = findEnd(data);
			test++;
	   }
	   else
		   firstPass = false;
		//printf("counter: %d\n", counter);
	   //if(counter<14)
		count = readStream->Read( read, 0, next);
	}
	
	ESPN::SportsScores::PrintTeams(counter);

	Console::WriteLine( "" );
	// Release the resources of stream object.
	readStream->Close();

	// Release the resources of response object.
	myResponse->Close();
	
	

	cin.get();
	return 0;
}

