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

void changeVol(bool wantsToMute)
{
	if (!wantsToMute && !isMute)						//ad is no longer playing and it's already unmuted
	{
		//cout << "SPOTIFY IS ALREADY UNMUTED...\n";
		isMute = 0;
		return;
	}
	else if (wantsToMute && isMute)						//ad is playing and it's already muted
	{
		//cout << "SPOTIFY IS ALREADY MUTED...\n";
		isMute = 1;
		return;
	}
	else if (wantsToMute && !isMute)					//ad is playing and it wants to mute
	{
		system("nircmd.exe changeappvolume spotify.exe -10");
		cout << "MUTING SPOTIFY...\n";
		isMute = 1;
		return;
	}
	else if (!wantsToMute && isMute)					//ad is no longer playing and it wants to unmute
	{
		system("nircmd.exe changeappvolume spotify.exe .02");
		cout << "UNMUTING SPOTIFY...\n";
		isMute = 0;
		return;
	}
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{

	if (IsWindowVisible(hwnd)) // check whether window is visible
	{
		char wnd_title[256];
		GetWindowText(hwnd, wnd_title, sizeof(wnd_title));
		string temptitle = wnd_title;

		if ((temptitle == "Spotify") || (temptitle == "Learn More") || (temptitle == "Learn more") || (temptitle == "Advertisement"))
		{
			cout << "AD IS PLAYING...\n";
			changeVol(1);
			//Sleep(30000);
			return false;					//prevents EnumWindows from enumerating again
		}
		else
		{
			changeVol(0);
		}
	}
	return true;							//function must return true if you want to continue enumeration
}

int main()
{
	cout << "MAKE SURE SPOTIFY IS PAUSED.\nPRESS THE PLAY/PAUSE MULTIMEDIA KEY TO BEGIN...\n";

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
			//run the actual program
			EnumWindows(EnumWindowsProc, 0);
		}
		Sleep(2000);
	}
}
