#include <windows.h>
#include <stdint.h>
#include <xinput.h>
#include <dsound.h>
#include <math.h>
#include <malloc.h>
#include "handmade_hero_debug.h"

/*
  TODO(ykdu): THIS IS NOT A FINAL PLATFORM LAYER!!!!
  * Saved game locations
  * Getting a handle to our own execitable file
  * Asset loading path
  * Threading (launch a thread)
  * Raw Input (support for multiple keyboards
  * sleep / time begin period
  * clip Cursor
  * Fullscreen support
  * WM_SETCURSOR control cursor visibility
  * QueryCancelAutoplay
  * WM_ACTIVATEAPP
  * Blit speed improvements
  * Hardware acceleration
  * GetKeyboardLayout

Just a partial list of stuff  !!!
*/

// NOTE(ykdu) more reasonable name for static
#define internal static
#define local_persist static
#define global_variable static 

#define Pi32 3.14159265359f

typedef uint32_t bool32_t;

global_variable int XOffset = 0;
global_variable int YOffset = 0;
global_variable LPDIRECTSOUNDBUFFER GlobalSecondaryBuffer;

// unity builds, compile as one single translation
#include "handmade.cpp"

void *PlatformLoadFile(char *FileName)
{
    return NULL;
}

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

typedef struct {
    // NOTE(ykdu): DirectSound Configurations
    uint32_t RunningSampleIndex;
    int SamplesPerSecond;
    float ToneHz;
    int16_t ToneVolume;
    int WavePeriod;
    int HalfWavePeriod;
    int BytesPerSample;
    int SecondaryBufferSize;
    float tSine;
    int LatencySampleCount;
} win32_sound_output;

global_variable BOOL GlobalRunning;
// NOTE(ykdu): we keep only one back buff all the time
global_variable win32_offscreen_buffer GlobalBackbuffer;
global_variable win32_sound_output GlobalSoundOutput = {};

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
        XInputLibrary = LoadLibrary("xinput1_0.dll");
    }
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
    else
    {
    }
}

#define DIRECT_SOUND_CREATE(name) _Check_return_ HRESULT WINAPI name(_In_opt_ LPCGUID pcGuidDevice, _Outptr_ LPDIRECTSOUND *ppDS, _Pre_null_ LPUNKNOWN pUnkOuter)
typedef DIRECT_SOUND_CREATE(direct_sound_create);
internal void
Win32InitDSound(HWND Window, int32_t SamplesPerSecond, int32_t BufferSize)
{
    // NOTE(ykdu): Load the library

    // NOTE(ykdu): Get a DirectSound
    HMODULE DSoundLibrary = LoadLibrary("dsound.dll");
    if(DSoundLibrary)
    {
        direct_sound_create *DirectSoundCreate = (direct_sound_create*)GetProcAddress(DSoundLibrary, "DirectSoundCreate");
        
        LPDIRECTSOUND DirectSound;
        if(DirectSoundCreate && SUCCEEDED(DirectSoundCreate(0, &DirectSound, 0)))
        {
            WAVEFORMATEX WaveFormat = {};
            WaveFormat.nChannels = 2;
            WaveFormat.wBitsPerSample = 16;
            WaveFormat.cbSize = 0;
            WaveFormat.wFormatTag = WAVE_FORMAT_PCM;                    
            WaveFormat.nSamplesPerSec = SamplesPerSecond;
                    
            WaveFormat.nBlockAlign  = (WaveFormat.nChannels * WaveFormat.wBitsPerSample) / 8;
            WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;

            if(SUCCEEDED(DirectSound->SetCooperativeLevel(Window, DSSCL_PRIORITY)))
            {
                {
                    DSBUFFERDESC BufferDescription = {};
                    BufferDescription.dwSize = sizeof(BufferDescription);
                    BufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;
                    // two second data for audio
                    LPDIRECTSOUNDBUFFER PrimaryBuffer;
                    if(SUCCEEDED(DirectSound->CreateSoundBuffer(&BufferDescription, &PrimaryBuffer, 0)))
                    {
                        if(SUCCEEDED(PrimaryBuffer->SetFormat(&WaveFormat)))
                        {
                            OutputDebugStringAFormat("DSound PrimaryBuffer Succeeded!\n");
                        }
                        else
                        {
                            // TODO(ykdu): Diagnostic
                        }
                    }
                }

                {
                    // NOTE(ykdu): casey may have problems reading the MSDN, it seems to me hard to use
                    DSBUFFERDESC BufferDescription = {};
                    BufferDescription.dwSize = sizeof(BufferDescription);
                    BufferDescription.dwFlags = 0;
                    BufferDescription.dwBufferBytes = BufferSize;
                    BufferDescription.lpwfxFormat = &WaveFormat;
                    // two second data for audio
                    if(SUCCEEDED(DirectSound->CreateSoundBuffer(&BufferDescription, &GlobalSecondaryBuffer, 0)))
                    {
                        // TODO(ykdu): Play Sound
                        OutputDebugStringAFormat("DSound GlobalSecondaryBuffer Succeeded!\n");                        
                    }
                    else
                    {
                        // TODO(ykdu): Diagnostic
                    }
                }
            }
            else
            {
                // TODO(ykdu): Diagnostic
            }
            // NOTE(ykdu): "Create" a primary/secondary buffer to comunicate with the sound card
            // (sound card can also do sould shader, to create new effects)
            OutputDebugStringAFormat("DSound Finished!\n");
        }
        else
        {
            // TODO(ykdu): Diagnostic
            OutputDebugStringAFormat("Diagnostic DSound!\n");
        }
    }
    return;
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
            if((BitMask & (1 << 29)) != 0)
            {
                GlobalRunning = false;
            }
        } break;

        case VK_UP:
        {
            // this canbe overflow
            GlobalSoundOutput.ToneHz *= 1.1f;
            GlobalSoundOutput.WavePeriod = GlobalSoundOutput.SamplesPerSecond / GlobalSoundOutput.ToneHz;
        } break;

        case VK_DOWN:
        {
            // NOTE(ykdu): this will be 0.000, after serveral times, however the next line will raise exception
            GlobalSoundOutput.ToneHz /= 1.1f;
            GlobalSoundOutput.WavePeriod = GlobalSoundOutput.SamplesPerSecond / GlobalSoundOutput.ToneHz;            
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

internal void
Win32ClearBuffer(win32_sound_output *SoundOutput)
{
    VOID *Region1, *Region2;
    DWORD Region1Size, Region2Size;
    if(SUCCEEDED(GlobalSecondaryBuffer->Lock(0, SoundOutput->SecondaryBufferSize,
                                             &Region1, &Region1Size, &Region2, &Region2Size, 0)))
    {
        uint8_t *DestSample = (uint8_t *)Region1;
        for(DWORD SampleIndex = 0; SampleIndex < Region1Size; SampleIndex++)
        {
            *DestSample++ = 0;
        }
                        
        DestSample = (uint8_t*)Region2;
        for(DWORD SampleIndex = 0; SampleIndex < Region2Size; SampleIndex++)
        {
            *DestSample++ = 0;
        }
        GlobalSecondaryBuffer->Unlock(Region1, Region1Size, Region2, Region2Size);    
    }
}

internal void
Win32FillSoundBuffer(win32_sound_output *SoundOutput, DWORD ByteToLock, DWORD BytesToWrite,
                     game_sound_output_buffer *SourceBuffer)
{
    // just copy data to the sound buffer
    VOID *Region1, *Region2;
    DWORD Region1Size, Region2Size;
    if(SUCCEEDED(GlobalSecondaryBuffer->Lock(ByteToLock, BytesToWrite, &Region1, &Region1Size, &Region2, &Region2Size, 0)))
    {
        int16_t *DstSample = (int16_t*)Region1;
        int16_t *SrcSample = SourceBuffer->Samples;
        DWORD Region1SampleCount = Region1Size / SoundOutput->BytesPerSample;
        for(DWORD SampleIndex = 0; SampleIndex < Region1SampleCount; SampleIndex++)
        {
            *DstSample++ = *SrcSample++;
            *DstSample++ = *SrcSample++;            
            ++SoundOutput->RunningSampleIndex;
        }
                        
        DstSample = (int16_t*)Region2;
        DWORD Region2SampleCount = Region2Size / SoundOutput->BytesPerSample;
        for(DWORD SampleIndex = 0; SampleIndex < Region2SampleCount; SampleIndex++)
        {
            *DstSample++ = *SrcSample++;
            *DstSample++ = *SrcSample++;            
            ++SoundOutput->RunningSampleIndex;
        }
        GlobalSecondaryBuffer->Unlock(Region1, Region1Size, Region2, Region2Size);    
    }
}

int CALLBACK
// wWinMain for receving unicode PWSTR lpCmdLine
WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd)
{
    LARGE_INTEGER PerfCountFrequencyResult;
    QueryPerformanceFrequency(&PerfCountFrequencyResult);
    int64_t PerfCountFrequency = PerfCountFrequencyResult.QuadPart;
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
        HWND WindowHandle = CreateWindowExA(
            0, WindowClass.lpszClassName, "Handmade Hero", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
        if(WindowHandle)
        {
            HDC DeviceContext = GetDC(WindowHandle);
            GlobalRunning = true;

            GlobalSoundOutput.RunningSampleIndex = 0;
            GlobalSoundOutput.SamplesPerSecond = 48000;
            GlobalSoundOutput.ToneHz = 256;
            GlobalSoundOutput.ToneVolume = 3000;
            GlobalSoundOutput.WavePeriod = GlobalSoundOutput.SamplesPerSecond / GlobalSoundOutput.ToneHz;
            GlobalSoundOutput.HalfWavePeriod = GlobalSoundOutput.WavePeriod / 2;
            GlobalSoundOutput.BytesPerSample = sizeof(int16_t) * 2;
            GlobalSoundOutput.SecondaryBufferSize = GlobalSoundOutput.SamplesPerSecond * GlobalSoundOutput.BytesPerSample;
            GlobalSoundOutput.LatencySampleCount = GlobalSoundOutput.SamplesPerSecond / 15;            
            Win32InitDSound(WindowHandle, GlobalSoundOutput.SamplesPerSecond, GlobalSoundOutput.SecondaryBufferSize);
            Win32ClearBuffer(&GlobalSoundOutput);
            GlobalSecondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);

            int16_t *Samples = (int16_t *)VirtualAlloc(0, GlobalSoundOutput.SecondaryBufferSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
            
            LARGE_INTEGER LastCounter;
            QueryPerformanceCounter(&LastCounter);
            uint64_t LastCycleCount = __rdtsc();
            
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

                DWORD ByteToLock = 0;
                DWORD TargetCursor = 0;
                DWORD BytesToWrite = 0;
                DWORD PlayCursor = 0;
                DWORD WriteCursor = 0;
                bool SoundIsValid = false;
                
                if(SUCCEEDED(GlobalSecondaryBuffer->GetCurrentPosition(&PlayCursor, &WriteCursor)))
                {
                    ByteToLock = (GlobalSoundOutput.RunningSampleIndex * GlobalSoundOutput.BytesPerSample) % GlobalSoundOutput.SecondaryBufferSize;
                    TargetCursor =
                        ((PlayCursor +
                          (GlobalSoundOutput.LatencySampleCount * GlobalSoundOutput.BytesPerSample)) %
                         GlobalSoundOutput.SecondaryBufferSize);
                    if(ByteToLock > TargetCursor)
                    {
                        BytesToWrite = (GlobalSoundOutput.SecondaryBufferSize - ByteToLock);
                        BytesToWrite += TargetCursor;
                    }
                    else
                    {
                        BytesToWrite = TargetCursor - ByteToLock;
                    }
                    SoundIsValid = true;
                }

                game_sound_output_buffer SoundBuffer = {};
                SoundBuffer.SamplesPerSecond = GlobalSoundOutput.SamplesPerSecond;
                SoundBuffer.SampleCount = BytesToWrite / GlobalSoundOutput.BytesPerSample;
                SoundBuffer.Samples = Samples;

                game_offscreen_buffer Buffer = {};
                Buffer.Memory = GlobalBackbuffer.Memory;
                Buffer.Width = GlobalBackbuffer.Width;
                Buffer.Height = GlobalBackbuffer.Height;
                Buffer.Pitch = GlobalBackbuffer.Pitch;
                // game update sound and render buffer
                GameUpdateAndRender(&Buffer, &SoundBuffer, GlobalSoundOutput.ToneHz);

                // win32 programs send those to device
                if(SoundIsValid)
                {
                    Win32FillSoundBuffer(&GlobalSoundOutput, ByteToLock, BytesToWrite, &SoundBuffer);
                }
                win32_window_dimension Dimension = Win32GetWindowDimension(WindowHandle);
                Win32DisplayBufferInWindow(&GlobalBackbuffer, DeviceContext,
                                           Dimension.Width, Dimension.Height);

                // ++XOffset;
                // ++YOffset;

                uint64_t EndCycleCount = __rdtsc();
                LARGE_INTEGER EndCounter;
                QueryPerformanceCounter(&EndCounter);
                
                uint64_t CyclesElapsed = EndCycleCount - LastCycleCount;
                int64_t CounterElapsed = EndCounter.QuadPart - LastCounter.QuadPart;
                double MSPerFrame = (((1000.0f*(double)CounterElapsed) / (double)PerfCountFrequency));
                double FPS = (double)PerfCountFrequency / (double)CounterElapsed;
                double MCPF = ((double)CyclesElapsed / (1000.0f * 1000.0f));

#if 0
                OutputDebugStringAFormat("%.02fms/f,  %.02ff/s, %.02fmc/f\n", MSPerFrame, FPS, MCPF);
#endif

                LastCounter = EndCounter;
                LastCycleCount = EndCycleCount;
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
