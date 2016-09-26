#ifndef VBOXH
#define VBOXH

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>

bool VBoxInit(const char *VMName);
bool VBoxStartVM();
bool VBoxRunDialog();
bool VBoxNotepad();
bool VBoxPutRandomText();
bool VBoxSaveFile();

#endif