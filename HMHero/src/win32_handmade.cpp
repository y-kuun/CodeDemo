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
global_variable LPDIRECTSOUNDBUFFER GlobalSecondaryBuffer;
global_variable int64_t GlobalPerfCountFrequency;

// unity builds, compile as one single translation
#include "handmade.cpp"
#include "win32_handmade.h"

void *PlatformLoadFile(char *FileName)
{
    return NULL;
}

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

internal debug_read_file_result
DEBUGPlatformReadEntireFile(char *Filename)
{
    debug_read_file_result Result = {};

    HANDLE FileHandle = CreateFileA(Filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if(FileHandle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER FileSize;
        if(GetFileSizeEx(FileHandle, &FileSize))
        {
            uint32_t FileSize32 = SafeTruncateUInt64(FileSize.QuadPart);
            Result.Contents = VirtualAlloc(0, FileSize32, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
            if(Result.Contents)
            {
                DWORD BytesRead;
                if(ReadFile(FileHandle, Result.Contents, FileSize32, &BytesRead, 0) &&
                   (FileSize32 == BytesRead))
                {
                    Result.ContentsSize = FileSize32;
                }
                else
                {
                    DEBUGPlatformFreeFileMemory(Result.Contents);
                    Result.Contents = 0;
                }
            }
            else
            {
            }
        }
        else
        {
        }
        CloseHandle(FileHandle);
    }
    else
    {
    }

    return Result;
}

internal void
DEBUGPlatformFreeFileMemory(void *Memory)
{
    if(Memory)
    {
        VirtualFree(Memory, 0, MEM_RELEASE);
    }
}

internal bool
DEBUGPlatformWriteEntireFile(char *Filename, uint32_t MemorySize, void *Memory)
{
    bool Result = false;

    HANDLE FileHandle = CreateFileA(Filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
    if(FileHandle != INVALID_HANDLE_VALUE)
    {
        DWORD BytesWritten;
        if(WriteFile(FileHandle, Memory, MemorySize, &BytesWritten, 0))
        {
            Result = (BytesWritten == MemorySize);
        }
        else
        {
        }

        CloseHandle(FileHandle);
    }
    else
    {
    }
    return Result;
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
        } break;

        case WM_KEYUP:
        {
            Assert(!"Keyboard input came in through a non-dispatch message!");
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

internal void
Win32ProcessKeyboardMessage(game_button_state *NewState, bool IsDown)
{
    Assert(NewState->EndedDown != IsDown);
    NewState->EndedDown = IsDown;
    ++NewState->HalfTransitionCount;
}

internal float
Win32ProcessXInputStickValue(short Value, short DeadZoneThreshold)
{
    float Result = 0;

    if(Value < -DeadZoneThreshold)
    {
        Result = (float)((Value + DeadZoneThreshold) / (32768.0f - DeadZoneThreshold));
    }
    else if(Value > DeadZoneThreshold)
    {
        Result = (float)((Value - DeadZoneThreshold) / (32767.0f - DeadZoneThreshold));
    }

    return Result;
}

internal void
Win32ProcessXInputDigitalButton(DWORD XInputButtonState,
                                game_button_state *OldState, DWORD ButtonBit,
                                game_button_state *NewState)
{
    NewState->EndedDown |= ((XInputButtonState & ButtonBit) == ButtonBit);
    NewState->HalfTransitionCount |= (OldState->EndedDown != NewState->EndedDown) ? 1 : 0;
}

internal void
Win32ProcessPendingMessages(game_controller_input *KeyboardController)
{
    MSG Message;
    while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
    {
        switch(Message.message)
        {
            case WM_QUIT:
            {
                GlobalRunning = false;
            } break;
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
                uint32_t VKCode = (uint32_t)Message.wParam;
                bool WasDown = ((Message.lParam & (1 << 30)) != 0);
                bool IsDown = ((Message.lParam & (1 << 31)) == 0);
                if(WasDown != IsDown)
                {
                    if(VKCode == 'W')
                    {
                        Win32ProcessKeyboardMessage(&KeyboardController->MoveUp, IsDown);
                    }
                    else if(VKCode == 'A')
                    {
                        Win32ProcessKeyboardMessage(&KeyboardController->MoveLeft, IsDown);
                    }
                    else if(VKCode == 'S')
                    {
                        Win32ProcessKeyboardMessage(&KeyboardController->MoveDown, IsDown);
                    }
                    else if(VKCode == 'D')
                    {
                        Win32ProcessKeyboardMessage(&KeyboardController->MoveRight, IsDown);
                    }
                    else if(VKCode == 'Q')
                    {
                        Win32ProcessKeyboardMessage(&KeyboardController->LeftShoulder, IsDown);
                    }
                    else if(VKCode == 'E')
                    {
                        Win32ProcessKeyboardMessage(&KeyboardController->RightShoulder, IsDown);
                    }
                    else if(VKCode == VK_UP)
                    {
                        Win32ProcessKeyboardMessage(&KeyboardController->ActionUp, IsDown);
                    }
                    else if(VKCode == VK_LEFT)
                    {
                        Win32ProcessKeyboardMessage(&KeyboardController->ActionLeft, IsDown);
                    }
                    else if(VKCode == VK_DOWN)
                    {
                        Win32ProcessKeyboardMessage(&KeyboardController->ActionDown, IsDown);
                    }
                    else if(VKCode == VK_RIGHT)
                    {
                        Win32ProcessKeyboardMessage(&KeyboardController->ActionRight, IsDown);
                    }
                    else if(VKCode == VK_ESCAPE)
                    {
                        Win32ProcessKeyboardMessage(&KeyboardController->Start, IsDown);
                    }
                    else if(VKCode == VK_SPACE)
                    {
                        Win32ProcessKeyboardMessage(&KeyboardController->Back, IsDown);
                    }
                }

                bool AltKeyWasDown = (Message.lParam & (1 << 29));
                if((VKCode == VK_F4) && AltKeyWasDown)
                {
                    GlobalRunning = false;
                }
            } break;

            default:
            {
                TranslateMessage(&Message);
                DispatchMessageA(&Message);
            }
        }
    }
}

inline LARGE_INTEGER
Win32GetWallClock(void)
{
    LARGE_INTEGER Result;
    QueryPerformanceCounter(&Result);
    return Result;
}

inline float
Win32GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End)
{
    float Result = ((float)(End.QuadPart - Start.QuadPart) /
                     (float)GlobalPerfCountFrequency);
    return Result;
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
    GlobalPerfCountFrequency = PerfCountFrequencyResult.QuadPart;
    unsigned int DesiredSchedulerMS = 1;
    bool SleepIsGranular = (timeBeginPeriod(DesiredSchedulerMS) == TIMERR_NOERROR);

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

    int MonitorRefreshHz = 60;
    int GameUpdateHz = MonitorRefreshHz / 2;
    float TargetSecondsPerFrame = 1.0f / (float)GameUpdateHz;

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
            GlobalSoundOutput.BytesPerSample = sizeof(int16_t) * 2;
            GlobalSoundOutput.SecondaryBufferSize = GlobalSoundOutput.SamplesPerSecond * GlobalSoundOutput.BytesPerSample;
            GlobalSoundOutput.LatencySampleCount = GlobalSoundOutput.SamplesPerSecond / 15;
            Win32InitDSound(WindowHandle, GlobalSoundOutput.SamplesPerSecond, GlobalSoundOutput.SecondaryBufferSize);
            Win32ClearBuffer(&GlobalSoundOutput);
            GlobalSecondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);

            int16_t *Samples = (int16_t *)VirtualAlloc(0, GlobalSoundOutput.SecondaryBufferSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#if HANDMADE_INTERNAL
            LPVOID BaseAddress = (LPVOID)Terabytes(2);
#else
            LPVOID BaseAddress = 0;
#endif
            game_memory GameMemory = {};
            GameMemory.PermanetStorageSize = Megabytes(4);
            GameMemory.TransientStorageSize = Gigabytes(1);

            uint64_t TotalSize = GameMemory.PermanetStorageSize + GameMemory.TransientStorageSize;
            GameMemory.PermanetStorage = VirtualAlloc(BaseAddress, (size_t)TotalSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
            GameMemory.TransientStorage = ((uint8_t*)GameMemory.PermanetStorage + GameMemory.PermanetStorageSize);

            // nesseccey memory already inited
            if(Samples && GameMemory.PermanetStorage && GameMemory.TransientStorage)
            {
                game_input Input[2] = {};
                game_input *NewInput = &Input[0];
                game_input *OldInput = &Input[1];

                LARGE_INTEGER LastCounter = Win32GetWallClock();
                uint64_t LastCycleCount = __rdtsc();

                while(GlobalRunning)
                {

                    game_controller_input *OldKeyboardController = GetController(OldInput, 0);
                    game_controller_input *NewKeyboardController = GetController(NewInput, 0);
                    *NewKeyboardController = {};
                    NewKeyboardController->IsConnected = true;

                    for(int ButtonIndex = 0;
                        ButtonIndex < ArrayCount(NewKeyboardController->Buttons);
                        ++ButtonIndex)
                    {
                        NewKeyboardController->Buttons[ButtonIndex].EndedDown =
                            OldKeyboardController->Buttons[ButtonIndex].EndedDown;
                    }

                    Win32ProcessPendingMessages(NewKeyboardController);

                    DWORD MaxControllerCount = XUSER_MAX_COUNT;
                    if(MaxControllerCount > (ArrayCount(NewInput->Controllers) - 1))
                    {
                        MaxControllerCount = (ArrayCount(NewInput->Controllers) - 1);
                    }
                    for(DWORD ControllerIndex=0; ControllerIndex < MaxControllerCount; ControllerIndex++)
                    {
                        DWORD OurControllerIndex = ControllerIndex + 1;
                        game_controller_input *OldController = GetController(OldInput, OurControllerIndex);
                        game_controller_input *NewController = GetController(NewInput, OurControllerIndex);

                        XINPUT_STATE ControllerState;
                        if(XInputGetState(ControllerIndex, &ControllerState) == ERROR_SUCCESS)
                        {
                            NewController->IsConnected = true;

                            XINPUT_GAMEPAD *Pad = &ControllerState.Gamepad;

                            NewController->StickAverageX = Win32ProcessXInputStickValue(Pad->sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
                            NewController->StickAverageY = Win32ProcessXInputStickValue(Pad->sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

                            if((NewController->StickAverageX != 0.0f) ||
                               (NewController->StickAverageY != 0.0f))
                            {
                                NewController->IsAnalog = true;
                            }

                            if(Pad->wButtons & XINPUT_GAMEPAD_DPAD_UP)
                            {
                                NewController->StickAverageY = 1.0f;
                                NewController->IsAnalog = false;
                            }

                            if(Pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
                            {
                                NewController->StickAverageY = -1.0f;
                                NewController->IsAnalog = false;
                            }

                            if(Pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
                            {
                                NewController->StickAverageX = -1.0f;
                                NewController->IsAnalog = false;
                            }

                            if(Pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
                            {
                                NewController->StickAverageX = 1.0f;
                                NewController->IsAnalog = false;
                            }

                            float Threshold = 0.5f;
                            Win32ProcessXInputDigitalButton((NewController->StickAverageX < -Threshold) ? 1 : 0,
                                &OldController->MoveLeft, 1,
                                &NewController->MoveLeft);
                            Win32ProcessXInputDigitalButton((NewController->StickAverageX > Threshold) ? 1 : 0,
                                &OldController->MoveRight, 1,
                                &NewController->MoveRight);
                            Win32ProcessXInputDigitalButton(
                                (NewController->StickAverageY < -Threshold) ? 1 : 0,
                                &OldController->MoveDown, 1,
                                &NewController->MoveDown);
                            Win32ProcessXInputDigitalButton((NewController->StickAverageY > Threshold) ? 1 : 0,
                                &OldController->MoveUp, 1,
                                &NewController->MoveUp);

                            Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                            &OldController->ActionDown, XINPUT_GAMEPAD_A,
                                                            &NewController->ActionDown);
                            Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                            &OldController->ActionRight, XINPUT_GAMEPAD_B,
                                                            &NewController->ActionRight);
                            Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                            &OldController->ActionLeft, XINPUT_GAMEPAD_X,
                                                            &NewController->ActionLeft);
                            Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                            &OldController->ActionUp, XINPUT_GAMEPAD_Y,
                                                            &NewController->ActionUp);
                            Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                            &OldController->LeftShoulder, XINPUT_GAMEPAD_LEFT_SHOULDER,
                                                            &NewController->LeftShoulder);
                            Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                            &OldController->RightShoulder, XINPUT_GAMEPAD_RIGHT_SHOULDER,
                                                            &NewController->RightShoulder);

                            Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                            &OldController->Start, XINPUT_GAMEPAD_START,
                                                            &NewController->Start);
                            Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                            &OldController->Back, XINPUT_GAMEPAD_BACK,
                                                            &NewController->Back);
                        }
                        else
                        {
                            NewController->IsConnected = false;

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
                    GameUpdateAndRender(&GameMemory, NewInput, &Buffer, &SoundBuffer);

                    // win32 programs send those to device
                    if(SoundIsValid)
                    {
                        Win32FillSoundBuffer(&GlobalSoundOutput, ByteToLock, BytesToWrite, &SoundBuffer);
                    }


                    LARGE_INTEGER WorkCounter = Win32GetWallClock();
                    float WorkSecondsElapsed = Win32GetSecondsElapsed(LastCounter, WorkCounter);

                    // TODO(casey): NOT TESTED YET!  PROBABLY BUGGY!!!!!
                    float SecondsElapsedForFrame = WorkSecondsElapsed;
                    if(SecondsElapsedForFrame < TargetSecondsPerFrame)
                    {
                        if(SleepIsGranular)
                        {
                            DWORD SleepMS = (DWORD)(1000.0f * (TargetSecondsPerFrame - SecondsElapsedForFrame));
                            if(SleepMS > 0)
                            {
                                // Sleep(SleepMS);
                            }
                        }

                        float TestSecondsElapsedForFrame = Win32GetSecondsElapsed(LastCounter, Win32GetWallClock());
                        // Assert(TestSecondsElapsedForFrame < TargetSecondsPerFrame);

                        while(SecondsElapsedForFrame < TargetSecondsPerFrame && false)
                        {
                            SecondsElapsedForFrame = Win32GetSecondsElapsed(LastCounter, Win32GetWallClock());
                        }
                    }
                    else
                    {
                        // TODO(ykdu): MISSED FRAME RATE!
                    }



                    win32_window_dimension Dimension = Win32GetWindowDimension(WindowHandle);
                    Win32DisplayBufferInWindow(&GlobalBackbuffer, DeviceContext,
                                               Dimension.Width, Dimension.Height);

                    game_input *Temp = NewInput;
                    NewInput = OldInput;
                    OldInput = Temp;

                    LARGE_INTEGER EndCounter = Win32GetWallClock();
                    float MSPerFrame = 1000.0f*Win32GetSecondsElapsed(LastCounter, EndCounter);
                    LastCounter = EndCounter;

                    uint64_t EndCycleCount = __rdtsc();
                    uint64_t CyclesElapsed = EndCycleCount - LastCycleCount;
                    LastCycleCount = EndCycleCount;

                    double FPS = 0.0f;
                    double MCPF = ((double)CyclesElapsed / (1000.0f * 1000.0f));

#if 1
                    char FPSBuffer[256];
                    _snprintf_s(FPSBuffer, sizeof(FPSBuffer),
                                "%.02fms/f,  %.02ff/s,  %.02fmc/f\n", MSPerFrame, FPS, MCPF);
                    OutputDebugStringA(FPSBuffer);
#endif
                }
            }
        }
        else
        {
        }
    }
    else
    {
    }
    return 0;
}
