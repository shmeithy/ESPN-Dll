// ESPNDll.cpp : Defines the exported functions for the DLL application.
//

#include <iostream>
#include "stdafx.h"
#include < stdio.h >
#include < stdlib.h >
#include < vcclr.h >
#include <stdexcept>
#include "ESPNDll.h"
//#using "ESPNDll.dll"
#using "System.dll"
#using "System.Xml.dll"

using namespace System;
using namespace System::Net;
using namespace System::Xml;
using namespace std;

//#pragma comment(lib, "ESPNDll.lib")

struct Team
{
	string conference;
	string name;
	string wins;
	string losses;
	string percentage;
	string gamesBack;
};

int currentTeam = 0;
int counter = 0;
int stupidNumber = 19;
bool isOnTeam = false;
char* currentSearch;
Team teams[30];


namespace ESPN
{
	public ref class Globals
	{
	public:
		static HttpWebRequest^ myRequest;
		static System::Net::WebResponse^ myResponse;

		static System::IO::Stream^ ReceiveStream;

		static System::Text::Encoding^ encode;

		// Pipe the stream to a higher level stream reader with the required encoding format.
		static System::IO::StreamReader^ readStream;

	};
	void SportsScores::SetUp()
	{
		printf("Set shit up");
		
		currentSearch = (char*)malloc(128*sizeof(char));
		currentSearch = "name";

	}

	
		int findEnd(char* data)
		{
			int num = 0;
			int returnVal=0;
			string s = data;
			string search = "</TeamRecord>";
			printf("\ndata: %s\n\n", data);
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
	/*
	void SportsScores::ReadData()
	{
		Globals::myRequest = dynamic_cast<HttpWebRequest^>(WebRequest::Create( "http://api.sportsdatallc.org/nba-t1/standings?season_id=2011&api_key=54exz7sw2tqszdffjxcqy5gx" ));
		Globals::myResponse = Globals::myRequest->GetResponse();

		Globals::ReceiveStream = Globals::myResponse->GetResponseStream();

		Globals::encode = System::Text::Encoding::GetEncoding( "utf-8" );

		// Pipe the stream to a higher level stream reader with the required encoding format.
		Globals::readStream = gcnew System::IO::StreamReader( Globals::ReceiveStream,Globals::encode );
		Console::WriteLine( "\nResponse stream received" );
	

		int next = 533;
		int test = 0;
		array<Char>^ read = gcnew array<Char>(600);
		// Read 256 charcters at a time.
		int count = Globals::readStream->Read( read, 0, 569 );
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
			
				ParseData(data, next);
				next = findEnd(data);
				test++;
		   }
		   else
			   firstPass = false;
			//printf("counter: %d\n", counter);
		   //if(counter<14)
			count = Globals::readStream->Read( read, 0, next);
		}

		// Release the resources of stream object.
		Globals::readStream->Close();

		// Release the resources of response object.
		Globals::myResponse->Close();
	}
	*/
	void SportsScores::ParseData(char* data, int start)
	{
		string s;
		size_t l = s.max_size();
		string line = data;
		//printf("\ndata: %s\n", data);
		size_t currentPlace = 569;
		size_t total = 0;
		//int size = sizeof(data);
		int size = 0;
		//for(int i =0; i < 30; i++)
		//{
		
		//System::Xml::XmlTextReader^ *reader = gcnew XmlTextReader(ReceiveStream);
		//while(reader->Read())
		//{
			//printf("reading\n");
		//}
			total = FindString(data, size, currentPlace);
			//printf("currentPlace: %d\n", currentPlace);
			currentPlace += total;
			
		//}
	}

	string findWord(string word, int index)
	{
		char* val = (char*)malloc(128*sizeof(char));
		//index += (sizeof(currentSearch) * 2);
		int n = index;
		
		int count =0;
		for(int i =0; i < word.length(); i++)
		{
			//printf("\nval: %c\nascii: %d", word[i + index], word[i + index]);
			if(word[i+index] == '\"' || word[i+index] == '<')
			{
				n = i + index;
				break;
			}
			else
			{
				val[count] = word[i + index];
				count++;
			}
		}
		val[count] = '\0';

		string returnVal = val;

		return returnVal;
	}

	int SportsScores::FindString(char* data,int size,  size_t currentPlace)
	{
		//char* val = new char(size + 1);
		string temp = data;
		//temp = temp.substr(currentPlace, temp.size() - currentPlace);
		//printf("temp: %s\n\n", temp.c_str());
		size_t found = temp.find("\" name=");
		string end;
		//if(found < 1000)
		//{
		
		string conf, wins, losses, percentage, gamesBack;

		isOnTeam = true;
		end = findWord(temp, found+8);
		teams[currentTeam].name = end;
			
		found = temp.find("\" conference=");
		conf = findWord(temp, found + 14);
		teams[currentTeam].conference = conf;

		//string temp = data;
		found = temp.find("<Won>");
		wins = findWord(temp, found+5);
		teams[currentTeam].wins = wins;

		found = temp.find("<Lost>");
		losses = findWord(temp, found+6);
		teams[currentTeam].losses = losses;

		found = temp.find("<PCT>");
		percentage = findWord(temp, found+5);
		teams[currentTeam].percentage = percentage;

		found = temp.find("<GB>");
		gamesBack = findWord(temp, found+4);
		teams[currentTeam].gamesBack = gamesBack;

		found = temp.find("</TeamRecord");
		found+=12;

		currentTeam++;

		
		return found;

	}

	void SportsScores::ParseTeam(char* data)
	{
		//printf("\ndata: %s\n", data);
		char* val = (char*)malloc(128*sizeof(char));		
		string temp = data;
		size_t found = temp.find("<Won>");
		string end = findWord(temp, found+5);
		teams[currentTeam].wins = end;

		found = temp.find("<Lost>");
		end = findWord(temp, found+6);
		teams[currentTeam].losses = end;

		found = temp.find("<PCT>");
		end = findWord(temp, found+5);
		teams[currentTeam].percentage = end;

		found = temp.find("<GB>");
		end = findWord(temp, found+4);
		teams[currentTeam].gamesBack = end;

		currentTeam++;
		isOnTeam = false;

		//printf("%s\t%s\t%s\t%s\t%s\n", &teams[currentTeam].name, &teams[currentTeam].wins, &teams[currentTeam].losses, &teams[currentTeam].percentage, &teams[currentTeam].gamesBack);
		//printf("\ndata: %s\n\n", data);
		

	}


	void SportsScores::PrintTeams(int counter)
	{
		printf("\n\nTeam			|	Wins	|	Losses	|	Win %\t|GB\n");
		for(int i =0; i < counter-1; i++)
		{
			if(teams[i].name.size() >= 16)
			{
				printf("%s\t|\t%s\t|\t%s\t|\t%s\t|%s\n", teams[i].name.c_str(), teams[i].wins.c_str(), teams[i].losses.c_str(), teams[i].percentage.c_str(), teams[i].gamesBack.c_str());
			}
			else
				printf("%s\t\t|\t%s\t|\t%s\t|\t%s\t|%s\n", teams[i].name.c_str(), teams[i].wins.c_str(), teams[i].losses.c_str(), teams[i].percentage.c_str(), teams[i].gamesBack.c_str());
		}
	}


	

}




