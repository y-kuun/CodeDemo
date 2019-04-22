#include <windows.h>

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
global_variable HBITMAP BitmapHandle;
global_variable HDC BitmapDeviceContext;

internal void
Win32ResizeDIBSection(int Width, int Height)
{
    // Device Independent Buff
    // TODO bulletproof this
#if 1    
    if(BitmapHandle)
    {
        // clear lasttime generated handle to prevent leakage
        DeleteObject(BitmapHandle);
    }
    
    if(!BitmapDeviceContext)
    {
        // TODO(ykdu): Should we recreate these under certain special circumstances, e.g., new monitor pluged
        // Device Conetext diierent
        // Memory Device Contenxt: To enable applications to place output in memory rather than sending it to an actual device, use a special device context for bitmap operations called a memory device context. 
        BitmapDeviceContext = CreateCompatibleDC(0);
    }

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = Width;
    BitmapInfo.bmiHeader.biHeight = Height;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;
    // belows are automatic initialized to zero
    BitmapInfo.bmiHeader.biSizeImage = 0;
    BitmapInfo.bmiHeader.biXPelsPerMeter = 0;
    BitmapInfo.bmiHeader.biYPelsPerMeter = 0;
    BitmapInfo.bmiHeader.biClrUsed = 0;
    BitmapInfo.bmiHeader.biClrImportant = 0;

    BitmapHandle = CreateDIBSection(
        BitmapDeviceContext, &BitmapInfo, DIB_RGB_COLORS, &BitmapMemory, 0, 0);
#endif
}

internal void
Win32UpdateWindow(HDC DeviceContext, int X, int Y, int Width, int Height)
{
#if 1
    // rectangle to rectangel copy, this might the output of the render pipe line
    StretchDIBits(DeviceContext,
		  X, Y, Width, Height,
		  X, Y, Width, Height,		      
		  BitmapMemory,
		  &BitmapInfo,
		  DIB_RGB_COLORS, SRCCOPY);    
#endif
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
        OutputDebugStringA("WM_SIZE\n");
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
        local_persist DWORD op = WHITENESS;
        PAINTSTRUCT Paint;
        // DC表保存着相关的Graphic Objects以及相关的参数， BeginPaint只在WM_PAINT时候调用
        HDC DeviceContext = BeginPaint(hwnd, &Paint);
        int X = Paint.rcPaint.left;
        int Y = Paint.rcPaint.top;
        LONG Width = Paint.rcPaint.right - Paint.rcPaint.left;
        LONG Height = Paint.rcPaint.bottom - Paint.rcPaint.top;

        Win32UpdateWindow(DeviceContext, X, Y, Width, Height);
        // nightmare of thread safty, debug exclusive
        #if 0    
        PatBlt(DeviceContext, X, Y, Width, Height, op);
        if(op == WHITENESS){
            op = BLACKNESS;
        } else {
            op = WHITENESS;
        }

        #endif
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
#if 0
	    BOOL GetMessageA
		(
            LPMSG lpMsg,
            HWND  hWnd,
            UINT  wMsgFilterMin,
            UINT  wMsgFilterMax
		);
#endif
	    MSG message;
	    // while(1) will cause warning from the compiler
	    Running = true;
	    while(Running)
	    {
		// more realtime process message peekmeesage, if not message get by GetMessage process will be hangged up
		BOOL messageResult = GetMessageA(&message, 0, 0, 0);
		if(messageResult > 0)
		{
		    // handle keyboard messages
		    TranslateMessage(&message);
		    // windows prefer to dispatch the message its self
		    DispatchMessageA(&message); 
		}
		else
		{
		    break;
		}
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
