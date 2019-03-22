//requires installation of nircmd to C:\Windows
//when opening it up, you NEED TO have spotify paused, at which point you press the multimedia play/pause button to begin the program along with unpausing spotify itself. must run as administrator

#include "pch.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <time.h>

using namespace std;

bool isMute = 0;
bool isPaused = 1;
bool foundSpotify = 0;
HWND spotifyHandle = NULL;

void changeVol(bool wantsToMute)
{
	if (!wantsToMute && !isMute)						//ad is no longer playing and it's already unmuted
	{
		//cout << "SPOTIFY IS ALREADY UNMUTED...\n";
		return;
	}
	else if (wantsToMute && isMute)						//ad is playing and it's already muted
	{
		//cout << "SPOTIFY IS ALREADY MUTED...\n";
		return;
	}
	else if (wantsToMute && !isMute)					//ad is playing and it wants to mute
	{
		system("nircmd.exe muteappvolume spotify.exe 1");
		cout << "MUTING SPOTIFY...\n";
		isMute = 1;
		return;
	}
	else if (!wantsToMute && isMute)					//ad is no longer playing and it wants to unmute
	{
		system("nircmd.exe muteappvolume spotify.exe 0");
		cout << "UNMUTING SPOTIFY...\n";
		isMute = 0;
		return;
	}
}

void startLoop()
{
	while (1)
	{
		if (GetAsyncKeyState(VK_MEDIA_PLAY_PAUSE) & 1)
		{
			isPaused = !isPaused;
			if (isPaused)
			{
				cout << "PAUSING PROGRAM...\n";
			}
			else
			{
				cout << "UNPAUSING PROGRAM...\n";
			}
		}
		if (!isPaused)
		{
			char wnd_title[256];
			GetWindowText(spotifyHandle, wnd_title, sizeof(wnd_title));
			string temptitle = wnd_title;

			if (temptitle.find("-") != string::npos)							//all songs have a hyphen in their names
			{
				changeVol(0);			//it's a song
			}
			else
			{
				changeVol(1);			//it's an ad
			}
		}
		Sleep(1000);
	}
}


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	if (IsWindowVisible(hwnd)) // check whether window is visible
	{
		char wnd_title[256];
		GetWindowText(hwnd, wnd_title, sizeof(wnd_title));
		string temptitle = wnd_title;

		if (temptitle == "Spotify")
		{
			cout << "FOUND SPOTIFY WINDOW...\n";
			foundSpotify = 1;
			spotifyHandle = hwnd;

			cout << "PRESS THE PLAY/PAUSE MULTIMEDIA KEY TO BEGIN...\n";
			cout << "STARTING LOOP...\n";
			startLoop();
		}		
	}
	return true;			//function must return true if you want to continue enumeration
}

int main()
{
	cout << "MAKE SURE SPOTIFY IS PAUSED.\nPRESS ANY KEY TO BEGIN...\n";
	cin.ignore();		//ignores input so pressing a key does nothing but continue the execution
	EnumWindows(EnumWindowsProc, 0);
}
