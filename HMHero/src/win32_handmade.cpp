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

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // wParam, lParam both depends on the value of uMsg
	LRESULT result = 0;

	switch(uMsg){
		case WM_SIZE:
		{
            // ANSI version of chars
			OutputDebugStringA("WM_SIZE\n");
		} break;
        
		case WM_DESTROY:
		{
            PostQuitMessage(0);
			OutputDebugStringA("WM_DESTROY\n");
		} break;

		case WM_CLOSE:
		{
			OutputDebugStringA("WM_CLOSE\n");
			DestroyWindow(hwnd);
		} break;

		case WM_ACTIVATEAPP:
		{
			OutputDebugStringA("WM_ACTIVATEAPP\n");
		} break;
	
		case WM_PAINT:
		{
			// 当发生size change的时候windows需要repaint
			static DWORD op = WHITENESS;
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(hwnd, &Paint);
			int X = Paint.rcPaint.left;
			int Y = Paint.rcPaint.top;
			LONG width = Paint.rcPaint.right - Paint.rcPaint.left;
			LONG height = Paint.rcPaint.bottom - Paint.rcPaint.top;
			// nightmare of thread safty, debug exclusive
			PatBlt(DeviceContext, X, Y, width, height, op);
			if(op == WHITENESS){
				op = BLACKNESS;
			} else {
				op = WHITENESS;
			}
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
	*/
	WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = WindowProc;
	WindowClass.hInstance = hInstance;
	// WindowClass.hIcon = ;
	WindowClass.lpszClassName = "HanmadeHeroWindowsClass";
    //MessageBox(0, "This is handmade hero!", "Handmade Hero", MB_OK | MB_ICONINFORMATION);

#if 0
	HWND CreateWindowExA(
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
    
	if(RegisterClass(&WindowClass)){
        // A might mean string are handle in unicode
		HWND WindowHandle = CreateWindowExA(
			0, WindowClass.lpszClassName, "Handmade Hero", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
		if(WindowHandle)
		{
#if 0
			BOOL GetMessageA(
			LPMSG lpMsg,
			HWND  hWnd,
			UINT  wMsgFilterMin,
			UINT  wMsgFilterMax
			);
#endif
			MSG message;
			for(;;){
                // more realtime process message peekmeesage
				BOOL messageResult = GetMessageA(&message, 0, 0, 0);
				if(messageResult){
                    // handle key word messages
					TranslateMessage(&message);
                    // windows prefer to dispatch the message its self
					DispatchMessage(&message); 
				} else {
					break;
				}
			}
		} else {
			// TODO (ykdu): error handle
		}
	} else {
		// TODO (ykdu): login	
	}

	return(0);
}
