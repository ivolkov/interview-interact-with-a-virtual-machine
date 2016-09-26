#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include "vbox.h"

int main(int argc, char* argv[])
{
			// constants for default values
			// default virtual machine name
	const char *DefVMName = "win7";

			// VM boot time in seconds
	const unsigned int VMStartDelaySec = 180;
		
			// determine virtual machine name
	#define VMNameLen 4096
	char VMName[VMNameLen];

	if (argc > 1)
		strcpy_s(VMName, argv[1]);
	else
		strcpy_s(VMName, DefVMName);

	if (!VBoxInit(VMName)) {
		#ifdef _DEBUG
		printf_s(_T("Could not initialize VirtualBox, exiting in 3 seconds..."));
		Sleep(3000);
		#endif
		return EXIT_FAILURE;
	}

			// start virtual machine
	if (!VBoxStartVM()) {
		#ifdef _DEBUG
		printf_s("Could not start VM, exiting in 3 seconds...");
		Sleep(3000);
		#endif

		return EXIT_FAILURE;
	}

			// wait for VM to boot
	#ifdef _DEBUG
	printf("Waiting %i seconds\n", VMStartDelaySec);
	#endif
			
	Sleep(VMStartDelaySec * 1000);

			// launch run dialog
	if (!VBoxRunDialog())
		return EXIT_FAILURE;

	Sleep(3000);

			// type in "notepad + Enter"
	if (!VBoxNotepad())
		return EXIT_FAILURE;

	Sleep(2000);

			// type "random text" in Notepad window
	if (!VBoxPutRandomText())
		return EXIT_FAILURE;

	Sleep(1000);

			// save text file
	if (!VBoxSaveFile())
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

