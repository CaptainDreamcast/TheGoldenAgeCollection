#include "mirk_titlescreengoof.h"

#include <stdio.h>
#include "mirk_titlescreengoofscreen.h"


#ifdef _WIN32
#include <windows.h>

void startMirkTitleScreenGoof() {
	ShellExecute(NULL, L"open", L"http://www.wikihow.com/Be-Nice", NULL, NULL, SW_SHOWNORMAL);
}

#elif defined __EMSCRIPTEN__ 

#include <emscripten.h>

void startMirkTitleScreenGoof() {
	emscripten_run_script("window.open(\"http://www.wikihow.com/Be-Nice\", \"_blank\"); ");
}

#else

void startMirkTitleScreenGoof() {
	setNewScreen(&MirkTitleScreenGoofScreen);
}

#endif
