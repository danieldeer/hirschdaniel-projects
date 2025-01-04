#include <windows.h>
#include <stdio.h>
#include <unistd.h>

#define LINE_WIDTH 16
#define HEIGHT 400

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

int main()
{
    printf("Drawing pixels over screen ...\n");
    
    HDC screenDC = GetDC(NULL);
    HBRUSH hbrush = CreateSolidBrush(RGB(255,0,0));
    SelectObject(screenDC, hbrush);

    while(1)
    {
        Rectangle(screenDC, SCREEN_WIDTH/2-LINE_WIDTH/2, SCREEN_HEIGHT/2-HEIGHT/2, SCREEN_WIDTH/2+LINE_WIDTH/2, SCREEN_HEIGHT/2+HEIGHT/2);
        Rectangle(screenDC, SCREEN_WIDTH/2-HEIGHT/2, SCREEN_HEIGHT/2-LINE_WIDTH/2, SCREEN_WIDTH/2+HEIGHT/2, SCREEN_HEIGHT/2+LINE_WIDTH/2);
    }
}
