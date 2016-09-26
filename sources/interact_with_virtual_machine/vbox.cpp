#include "vbox.h"

		// VBoxManage executable name
const char *VBoxManagerName = "VBoxManage.exe";

		// key scan codes definition
		//         win_down|  r  |win_up
const char *SCWinR = "e0 5b 13 93 e0 db";

		//                 n  |  o  |  t  |  e  |  p  |  a  |  d  |enter
const char *SCNotepad = "31 b1 18 98 14 94 12 92 19 99 1e 9e 20 a0 1c 9c";

		//					  r  |  a  |  n  |  d  |  o  |  m  |     |  t  |  e  |  x  |  t  
const char *SCRandomText = "13 93 1e 9e 31 b1 20 a0 18 98 32 b2 39 b9 14 94 12 92 2d ad 14 94";

		//	   LCtrl down|  s  | LCtrl up
const char *SCCtrlS = "1d 1f 9f 9d";

		//				  t  |  m  |  p  |  .  |  t  |  x  |  t
const char *SCTmpTxt = "14 94 32 b2 19 99 34 b4 14 94 2d ad 14 94";

		//			  Tab
const char *SCTab = "0f 8f";

		//			 Space bar
const char *SCSpace = "39 b9";

		//				   d  |  e  |  s  |  k  |  t  |  o  |  p  |  \  |enter
const char *SCDesktop = "20 a0 12 92 1f 9f 25 a5 14 93 18 98 19 99 2b ab 1c 9c";

		//	   LAlt down|  s  |LAlt up
const char *SCAltS = "38 1f 9f b8";
		
		//     LAlt down|  y  |LAlt up
const char *SCAltY = "38 15 95 b8";

		//		LAlt down|  F4 |LAlt up
const char *SCAltF4 = "38 3e be b8";

		// Current console handle
HWND VBoxConsoleHandle = NULL;

		// VirtualBox install directory
#define VBoxInstallDirLen 4096
char VBoxInstallDir[VBoxInstallDirLen];

		// VBoxManage command parameters
#define VBoxParamLen 4096
char VBoxParamStartVM[VBoxParamLen];
char VBoxParamNoAudio[VBoxParamLen];
char VBoxParamRunDialog[VBoxParamLen];
char VBoxParamNotepad[VBoxParamLen];
char VBoxParamRandomText[VBoxParamLen];
char VBoxParamCtrlS[VBoxParamLen];
char VBoxParamTmpTxt[VBoxParamLen];
char VBoxParamTab[VBoxParamLen];
char VBoxParamSpace[VBoxParamLen];
char VBoxParamDesktop[VBoxParamLen];
char VBoxParamAltS[VBoxParamLen];
char VBoxParamAltY[VBoxParamLen];
char VBoxParamAltF4[VBoxParamLen];
char VBoxParamEnter[VBoxParamLen];

		// new window show mode
#ifdef _DEBUG
const int VBoxShowMode = SW_NORMAL;
#else
const int VBoxShowMode = SW_HIDE;
#endif

bool VBoxCommand(const char *Params)
{
	HINSTANCE res = ShellExecute(VBoxConsoleHandle, "open", VBoxManagerName, Params, VBoxInstallDir, VBoxShowMode);
	if (res < (HINSTANCE)32) {
		#ifdef _DEBUG
		printf("Error while executing command (error %i)\n", res);
		#endif

		return false;
	}

	return true;
}

bool VBoxInit(const char *VMName)
{
	const char *sPutKey = "controlvm %s keyboardputscancode %s";

			// 1. obtain console handle
	#define ConsoleTitleLen 4096
	char ConsoleOrigTitle[ConsoleTitleLen];
	char ConsoleNewTitle[ConsoleTitleLen];

			// get original console title
	GetConsoleTitle(ConsoleOrigTitle, ConsoleTitleLen);

			// set new unique console title using TickCount and ProcessId
	wsprintf(ConsoleNewTitle, _T("%d/%d"), GetTickCount(), GetCurrentProcessId());
	SetConsoleTitle(ConsoleNewTitle);
	Sleep(20);

			// get console handle
	VBoxConsoleHandle = FindWindow(NULL, ConsoleNewTitle);

			// restore original console title
	SetConsoleTitle(ConsoleOrigTitle);

			// 2. hide console window in release build
	#ifndef _DEBUG
	ShowWindow(VBoxConsoleHandle, SW_HIDE);
	#endif

			// 3. obtain VirtualBox install directory
	if (GetEnvironmentVariable("VBOX_INSTALL_PATH", VBoxInstallDir, VBoxInstallDirLen) == 0)
		if (GetEnvironmentVariable("VBOX_MSI_INSTALL_PATH", VBoxInstallDir, VBoxInstallDirLen) == 0)
			return false;

	if (strlen(VBoxInstallDir) == 0)
		return false;

	VBoxInstallDir[strlen(VBoxInstallDir) - 1] = '\0';

			// 4. define VBoxManage run parameters
			// parameter "start virtual machine in debug build"
	#ifdef _DEBUG
	sprintf_s(VBoxParamStartVM, "startvm %s", VMName);
	#else
			// parameter "start virtual machine in release build ("headless mode")"
	sprintf_s(VBoxParamStartVM, "startvm %s --type headless", VMName);
	#endif

			// parameter "disable audio support"
	sprintf_s(VBoxParamNoAudio, "modifyvm %s --audio none", VMName);

			// parameter "put 'Win+R' keyboard keys"
	sprintf_s(VBoxParamRunDialog, sPutKey, VMName, SCWinR);

			// parameter "put 'notepad+Enter' keys"
	sprintf_s(VBoxParamNotepad, sPutKey, VMName, SCNotepad);

			// parameter "put 'random text' keys"
	sprintf_s(VBoxParamRandomText, sPutKey, VMName, SCRandomText);

			// parameter "put 'Ctrl + S' keys"
	sprintf_s(VBoxParamCtrlS, sPutKey, VMName, SCCtrlS);

			// parameter "put 'tmp.txt' keys"
	sprintf_s(VBoxParamTmpTxt, sPutKey, VMName, SCTmpTxt);

			// parameter "put 'Tab' key"
	sprintf_s(VBoxParamTab, sPutKey, VMName, SCTab);

			// parameter "put 'Scape' key"
	sprintf_s(VBoxParamSpace, sPutKey, VMName, SCSpace);

			// parameter "put 'desktop\' keys"
	sprintf_s(VBoxParamDesktop, sPutKey, VMName, SCDesktop);

			// parateter "put 'Alt+S" keys"
	sprintf_s(VBoxParamAltS, sPutKey, VMName, SCAltS);

			// parateter "put 'Alt+Y' keys"
	sprintf_s(VBoxParamAltY, sPutKey, VMName, SCAltY);

			// paramter "put 'Alt+F4' keys"
	sprintf_s(VBoxParamAltF4, sPutKey, VMName, SCAltF4);

	return true;
}

bool VBoxStartVM()
{
	VBoxCommand(VBoxParamNoAudio);
	return VBoxCommand(VBoxParamStartVM);
}

bool VBoxRunDialog()
{
	return VBoxCommand(VBoxParamRunDialog);
}

bool VBoxNotepad()
{
	return VBoxCommand(VBoxParamNotepad);
}

bool VBoxPutRandomText()
{
	return VBoxCommand(VBoxParamRandomText);
}

bool VBoxSaveFile()
{
			// press Ctrl+S
	if (!VBoxCommand(VBoxParamCtrlS))
		return false;

	Sleep(3000);

			// enter "tmp.txt"
	if (!VBoxCommand(VBoxParamTmpTxt))
		return false;

	Sleep(3000);

			// press Tab 6 times
	if (!VBoxCommand(VBoxParamTab))
		return false;

	if (!VBoxCommand(VBoxParamTab))
		return false;

	if (!VBoxCommand(VBoxParamTab))
		return false;

	if (!VBoxCommand(VBoxParamTab))
		return false;

	if (!VBoxCommand(VBoxParamTab))
		return false;

	if (!VBoxCommand(VBoxParamTab))
		return false;

	Sleep(3000);

			// press Space bar
	if (!VBoxCommand(VBoxParamSpace))
		return false;

	Sleep(3000);

			// enter "Desktop\"
	if (!VBoxCommand(VBoxParamDesktop))
		return false;

	Sleep(3000);

			// enter Alt+S to click on Save button
	if (!VBoxCommand(VBoxParamAltS))
		return false;

	Sleep(3000);

			// enter Alt+Y in case "Rewrite file?" dialog shows up
	if (!VBoxCommand(VBoxParamAltY))
		return false;

	Sleep(3000);

			// enter Alt+F4 to close notepad
	if (!VBoxCommand(VBoxParamAltF4))
		return false;

	return true;
}