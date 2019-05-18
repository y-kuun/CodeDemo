#include <windows.h>
#include <stdint.h>
#include <xinput.h>
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

// NOTE(ykdu) more reasonable name for static
#define internal static
#define local_persist static
#define global_variable static

global_variable int XOffset = 0;
global_variable int YOffset = 0;

// NOTE(ykdu) go throught the all code when you refactory
typedef struct win32_offscreen_buffer
{
    BITMAPINFO Info;
    void *Memory;
    int Width;
    int Height;
    int Pitch;
} win32_offscreen_buffer;

typedef struct win32_window_dimension
{
    int Width;
    int Height;
} win32_window_dimension;

global_variable BOOL GlobalRunning;
// NOTE(ykdu): we keep only one back buff all the time
global_variable win32_offscreen_buffer GlobalBackbuffer;

// NOTE(ykdu):

#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD, XINPUT_VIBRATION*)
typedef X_INPUT_SET_STATE(x_input_set_state);
X_INPUT_SET_STATE(XInputSetStateStub)
{
    return(ERROR_DEVICE_NOT_CONNECTED);
}
global_variable x_input_set_state *XInputSetState_ = XInputSetStateStub;
#define XInputSetState XInputSetState_

#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD, XINPUT_STATE*)
typedef X_INPUT_GET_STATE(x_input_get_state);
X_INPUT_GET_STATE(XInputGetStateStub)
{
    return(ERROR_DEVICE_NOT_CONNECTED);    
}
global_variable x_input_get_state *XInputGetState_ = XInputGetStateStub;
#define XInputGetState XInputGetState_

internal void
win32LoadXInput(void)
{
    HMODULE XInputLibrary = LoadLibrary("xinput1_4.dll");
    if(!XInputLibrary)
    {
        XInputLibrary = LoadLibrary("xinput1_3.dll");
    }
    if(XInputLibrary)
    {
        XInputGetState = (x_input_get_state*)GetProcAddress(XInputLibrary, "XInputGetState");
        if(!XInputGetState) { XInputGetState = XInputGetStateStub;}
        
        XInputSetState = (x_input_set_state*)GetProcAddress(XInputLibrary, "XInputSetState");
        if(!XInputSetState) { XInputSetState = XInputSetStateStub;}
    }
}

win32_window_dimension
Win32GetWindowDimension(HWND Window)
{
    win32_window_dimension Result;

    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    Result.Width = ClientRect.right - ClientRect.left;
    Result.Height = ClientRect.bottom - ClientRect.top;
    
    return(Result);
}

internal void
RenderWeirdGradient(win32_offscreen_buffer *Buffer, int BlueOffset, int GreenOffset)
{
    // NOTE(ykdu) addition is the most cheapest operation fro 
    uint8_t *Row = (uint8_t*)Buffer->Memory;
    for(int Y = 0; Y < Buffer->Height; Y++)
    {
        uint32_t *Pixel = (uint32_t*)Row;
        for(int X = 0; X < Buffer->Width; X++)
        {
            uint8_t Blue =  (X + BlueOffset);
            uint8_t Green = (Y + GreenOffset);
            *Pixel++ = ((Green << 8) | Blue);
        }
        Row += Buffer->Pitch;
    }
}

internal void
Win32ResizeDIBSection(win32_offscreen_buffer *Buffer, int Width, int Height)
{
    if(Buffer->Memory)
    {
        VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
    }

    Buffer->Width = Width;
    Buffer->Height = Height;

    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biWidth = Buffer->Width;
    // NOTE(ykdu): plus bottom-top, minus for top-bottom
    Buffer->Info.bmiHeader.biHeight = -Buffer->Height;
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32;
    Buffer->Info.bmiHeader.biCompression = BI_RGB;

    // TODO(ykdu) allocate array which is DWORD aligned might work
    int BytesPerPixel = sizeof(uint32_t);
    // NOTE(ykdu) alloc at page level
    Buffer->Memory = VirtualAlloc(0,
        BytesPerPixel * Buffer->Width * Buffer->Height,
        MEM_COMMIT, PAGE_READWRITE);
    Buffer->Pitch = BytesPerPixel * Buffer->Width;
    
    // TODO(ykdu) should clear the memory to black(0)
}

internal void
Win32DisplayBufferInWindow(win32_offscreen_buffer *Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight)
{
    // NOTE(ykdu): if dest rect is larger/smaller than src rect,
    // StretchDIBits will enlarge or compact the data
    StretchDIBits(DeviceContext,
        0, 0, WindowWidth, WindowHeight, // NOTE(ykdu) left-top x,y and width,height
        0, 0, Buffer->Width, Buffer->Height,
        Buffer->Memory,
        &Buffer->Info,
        DIB_RGB_COLORS, SRCCOPY);
}

void win32KeyDownHandler(WPARAM Key, LPARAM BitMask)
{
    // move with hjkl
    // TODO(ykdu): use marcro instead of intermediate numbers
    // NOTE(ykdu): if you input two key at the same time, how ever only one VK message will be provoked,
    // may be we should using key down to handle this multiple key inputed at the same time, otherwise `GetKeyboardState` `GetAsyncKeyState`
    switch(Key)
    {
        case 0x4A: // j
        {
            YOffset -= 10;
        } break;

        case 0x4B: // k
        {
            YOffset += 10;
        } break;

        case 0x48: // h
        {
            XOffset += 10;
        } break;

        case 0x4C: // l
        {
            XOffset -= 10;
        } break;

        case VK_F4:
        {
            // on windows 10 WM_CLOSE message will be provoked before this alt-f4
            if(BitMask & (1 << 29))
            {
                GlobalRunning = false;
            }
        } break;
        
        default:
        {
            OutputDebugStringAFormat("WM_KEYDOWN %d %c\n", (int)Key, (char)Key);
        } break;
    }
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

        // NOTE(ykdu): when come to key message, lParam contains a bitmask for differen meannings, check to MSDN
        case WM_CHAR:
        {
            OutputDebugStringAFormat("WM_CHAR %d %c\n", (int)wParam, (char)wParam);            
        } break;
        
        case WM_KEYDOWN:
        {
            win32KeyDownHandler(wParam, lParam);
        } break;
        
        case WM_KEYUP:
        {
            OutputDebugStringAFormat("WM_KEYUP %d %c\n", (int)wParam, (char)wParam);
        } break;
        
        case WM_SIZE:
        {
            // TODO(ykdu): decided to keep a constant size backbuffer
            win32_window_dimension Dim = Win32GetWindowDimension(hwnd);
            OutputDebugStringAFormat("WM_Size: %d x %d\n", Dim.Width, Dim.Height);
        } break;
        
        case WM_DESTROY:
        {
            GlobalRunning = false;
            OutputDebugStringA("WM_DESTROY\n");
        } break;

        case WM_CLOSE:
        {
            // TODO(ykdu): Handle this with a message to the user?
            GlobalRunning = false;
            OutputDebugStringA("WM_CLOSE\n");
        } break;

        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;
	
        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(hwnd, &Paint);
            win32_window_dimension Dim = Win32GetWindowDimension(hwnd);
            Win32DisplayBufferInWindow(&GlobalBackbuffer, DeviceContext, Dim.Width, Dim.Height);
            EndPaint(hwnd, &Paint);
        } break;

        default:
        {
            // OutputDebugStringA("what ever\n");
            // NOTE(ykdu): if we don't handle this msg, then let windows do it
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
    Win32ResizeDIBSection(&GlobalBackbuffer, 1280, 720);
    win32LoadXInput();
    /*
      OWNDC, not need to share device context
      HREDRAW VREDRAW, Horizantal and vertical redraw when changed
    */
    WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = Win32WindowProc;
    WindowClass.hInstance = hInstance;
    // WindowClass.hIcon = ;
    WindowClass.lpszClassName = "HanmadeHeroWindowsClass";

    if(RegisterClass(&WindowClass))
    {
        // A might mean string are handle in unicode
        HWND WindowHandle = CreateWindowExA(
            0, WindowClass.lpszClassName, "Handmade Hero", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
        if(WindowHandle)
        {
            HDC DeviceContext = GetDC(WindowHandle);
            GlobalRunning = true;
            while(GlobalRunning)
            {
                MSG Message;
                while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
                {
                    if(Message.message == WM_QUIT)
                    {
                        GlobalRunning = false;
                    }
                    
                    // handle keyboard messages
                    TranslateMessage(&Message);
                    // windows prefer to dispatch the message its self
                    DispatchMessageA(&Message);
                }

                // NOTE(ykdu): I do not have a game pad
                // TODO(ykdu): should we poll this more frequently, now is after all message from windows
                for(DWORD ControllerIndex=0; ControllerIndex < XUSER_MAX_COUNT; ControllerIndex++)
                {
                    XINPUT_STATE ControllerState;
                    if(XInputGetState(ControllerIndex, &ControllerState) == ERROR_SUCCESS)
                    {
                        // NOTE(ykdu): controller is plugged in
                        XINPUT_GAMEPAD *Pad = &ControllerState.Gamepad;
                        bool Up = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
                        bool Down = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
                        bool Left = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
                        bool Right = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
                        bool StartButton = (Pad->wButtons & XINPUT_GAMEPAD_START);
                        bool BackButton = (Pad->wButtons & XINPUT_GAMEPAD_BACK);
                        bool LeftShoulder = (Pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
                        bool RightShoulder = (Pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
                        bool AButton = (Pad->wButtons & XINPUT_GAMEPAD_A);
                        bool BButton = (Pad->wButtons & XINPUT_GAMEPAD_B);
                        bool XButton = (Pad->wButtons & XINPUT_GAMEPAD_X);
                        bool YButton = (Pad->wButtons & XINPUT_GAMEPAD_Y);

                        // NOTE(ykdu): here means the thumb stick
                        uint16_t StickX = Pad->sThumbLX;
                        uint16_t StickY = Pad->sThumbLY;
                    }
                    else
                    {
                        // NOTE(ykdu); controller is not available
                    }
                }
                
                RenderWeirdGradient(&GlobalBackbuffer, XOffset, YOffset);
                win32_window_dimension Dim = Win32GetWindowDimension(WindowHandle);
                Win32DisplayBufferInWindow(&GlobalBackbuffer, DeviceContext, Dim.Width, Dim.Height);
                // ++XOffset;
                // ++YOffset;
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
