#include <windows.h>
#include <stdint.h>
#include "handmade_hero_debug.h"

// refer to MSDN at least one time
/*
  LRESULT CALLBACK WindowProc(
  _In_ HWND   hwnd,
  _In_ UINT   uMsg,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
  );
*/

#define internal static
#define local_persist static
#define global_variable static

// TODO (ykdu) This is a global for now
global_variable BOOL Running;

global_variable BITMAPINFO BitmapInfo;
global_variable void *BitmapMemory;
// TODO (ykdu)
global_variable int BitmapWidth;
global_variable int BitmapHeight;
global_variable int BytesPerPixel = 4;

internal void
RenderWeirdGradient(int XOffset, int YOffset)
{
    int Pitch = BitmapWidth * BytesPerPixel;
    // int8_t *Row = (int8_t*)BitmapMemory;
    for(int Y = 0; Y < BitmapHeight; Y++)
    {
        // int32_t *Pixel = (int32_t*)Row;
        for(int X = 0; X < BitmapWidth; X++)
        {
            int32_t *Pixel = (int32_t*)((char*)BitmapMemory + (Pitch * Y + BytesPerPixel * X));
            // NOTE(ykdu) highest byte is not used  0x00RRGGBB
            // LITTLE ENDIAN ARCHITECHTURE, uint32 in memory  BB GG RR 00
            // if use int8_t here will cause overflow shit
            uint8_t Blue =  (X + XOffset);
            uint8_t Green = (Y + YOffset);
            *Pixel = ((Green << 8) | Blue);
            Pixel++;
        }
        // Row += Pitch;
    }
}

internal void
Win32ResizeDIBSection(int Width, int Height)
{
    if(BitmapMemory)
    {
        VirtualFree(BitmapMemory, 0, MEM_RELEASE);
    }

    BitmapWidth = Width;
    BitmapHeight = Height;

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = Width;
    // plus bottom-top, minus for top-bottom
    BitmapInfo.bmiHeader.biHeight = -Height;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    // TODO(ykdu) allocate array which is DWORD aligned might work
    BitmapMemory = VirtualAlloc(0, BytesPerPixel * Width * Height, MEM_COMMIT, PAGE_READWRITE);

    // RenderWeirdGradient(0, 0);
}

internal void
Win32UpdateWindow(HDC DeviceContext, RECT *WindowRect, int X, int Y, int Width, int Height)
{
    int WindowWidth = WindowRect->right - WindowRect->left;
    int WindowHeight = WindowRect->bottom - WindowRect->top;
    StretchDIBits(DeviceContext,
                  /*
                    X, Y, Width, Height,
                    X, Y, Width, Height,  
                  */
                  0, 0, BitmapWidth, BitmapHeight,
                  0, 0, WindowWidth, WindowHeight,
                  BitmapMemory,
                  &BitmapInfo,
                  DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK
Win32WindowProc(HWND hwnd,
	   UINT uMsg,
	   WPARAM wParam,
	   LPARAM lParam)
{
    // wParam, lParam both depend on the value of uMsg
    LRESULT result = 0;
    switch(uMsg){
        case WM_SIZE:
        {
            // ANSI version of chars
            RECT ClientRect;
            GetClientRect(hwnd, &ClientRect);
            int Width = ClientRect.right - ClientRect.left;
            int Height = ClientRect.bottom - ClientRect.top;
            Win32ResizeDIBSection(Width, Height);
            // OutputDebugStringA("WM_SIZE: %d x %d \n", Width, Height);
            OutputDebugStringAFormat("WM_Size: %d x %d\n", Width, Height);
        } break;
        
        case WM_DESTROY:
        {
            // TODO(ykdu): Handle this with a error?
            Running = false;
            OutputDebugStringA("WM_DESTROY\n");
        } break;

        case WM_CLOSE:
        {
            // TODO(ykdu): Handle this with a message to the user?
            Running = false;	
            OutputDebugStringA("WM_CLOSE\n");
        } break;

        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;
	
        case WM_PAINT:
        {
            // 当发生size change的时候windows需要repaint
            // nightmare of thread safty, debug exclusive
            local_persist DWORD op = WHITENESS;
            PAINTSTRUCT Paint;
            // DC表保存着相关的Graphic Objects以及相关的参数， BeginPaint只在WM_PAINT时候调用
            HDC DeviceContext = BeginPaint(hwnd, &Paint);
            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;
            LONG Width = Paint.rcPaint.right - Paint.rcPaint.left;
            LONG Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            OutputDebugStringA("WM_PAINT");
            Win32UpdateWindow(DeviceContext, &Paint.rcPaint, X, Y, Width, Height);
            EndPaint(hwnd, &Paint);
        } break;

        default:
        {
            //OutputDebugStringA("what ever\n");
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        } break;
    }

    return result;
}

int CALLBACK
// wWinMain for receving unicode PWSTR lpCmdLine
WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd)
{
    /*
      READ MSDN
      UINT      style;
      WNDPROC   lpfnWndProc;
      int       cbClsExtra;
      int       cbWndExtra;
      HINSTANCE hInstance;
      HICON     hIcon;
      HCURSOR   hCursor;
      HBRUSH    hbrBackground;
      LPCSTR    lpszMenuName;
      LPCSTR    lpszClassName;
    */
    // set all field to zero
    WNDCLASSA WindowClass = {0};	
    /*
      OWNDC, not need to share device context
      HREDRAW VREDRAW, Horizantal and vertical redraw when changed
    */
    WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = Win32WindowProc;
    WindowClass.hInstance = hInstance;
    // WindowClass.hIcon = ;
    WindowClass.lpszClassName = "HanmadeHeroWindowsClass";
    //MessageBox(0, "This is handmade hero!", "Handmade Hero", MB_OK | MB_ICONINFORMATION);

#if 0
    HWND CreateWindowExA
	(
        DWORD     dwExStyle,
        LPCSTR    lpClassName,
        LPCSTR    lpWindowName,
        DWORD     dwStyle,
        int       X,
        int       Y,
        int       nWidth,
        int       nHeight,
        HWND      hWndParent,
        HMENU     hMenu,
        HINSTANCE hInstance,
        LPVOID    lpParam
	);
#endif
    
    if(RegisterClass(&WindowClass))
    {
        // A might mean string are handle in unicode
        HWND WindowHandle = CreateWindowExA(
            0, WindowClass.lpszClassName, "Handmade Hero", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
        if(WindowHandle)
        {
            MSG message;
            // while(1) will cause warning from the compiler
            Running = true;
            int XOffset = 0;
            int YOffset = 0;
            while(Running)
            {
                MSG Message;
                // more realtime process message peekmeesage, if not message get by GetMessage process will be hangged up
                // NOTE(ykdu) last argument different with GetMessage
                while(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
                {
                    if(Message.message == WM_QUIT)
                    {
                        Running = false;
                    }
                    // handle keyboard messages
                    TranslateMessage(&message);
                    // windows prefer to dispatch the message its self
                    DispatchMessageA(&message);
                }

                HDC DeviceContext = GetDC(WindowHandle);
                RECT ClientRect;
                GetClientRect(WindowHandle, &ClientRect);
                RenderWeirdGradient(XOffset, YOffset);
                int X = ClientRect.left;
                int Y = ClientRect.top;
                LONG Width = ClientRect.right - ClientRect.left;
                LONG Height = ClientRect.bottom - ClientRect.top;
                Win32UpdateWindow(DeviceContext, &ClientRect, X, Y, Width, Height);
                ReleaseDC(WindowHandle, DeviceContext);
                ++XOffset;
                ++YOffset;
            }
        }
        else
        {
            // TODO (ykdu): error handle
        }
    }
    else
    {
        // TODO (ykdu): login	
    }

    return 0;
}
