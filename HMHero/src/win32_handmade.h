
#ifndef __WIN32_HANDEMADE_H__

typedef struct win32_offscreen_buffer
{
    BITMAPINFO Info;
    void *Memory;
    int Width;
    int Height;
    int Pitch;
    int BytesPerPixel;
} win32_offscreen_buffer;

typedef struct win32_window_dimension
{
    int Width;
    int Height;
} win32_window_dimension;

typedef struct win32_sound_output
{
    int SamplesPerSecond;
    int BytesPerSample;
    DWORD SecondaryBufferSize;
    DWORD SafetyBytes;
    int LatencySampleCount;
    uint32_t RunningSampleIndex;
    float tSine;
} win32_sound_output;

typedef struct win32_debug_time_marker
{
    DWORD OutputPlayCursor;
    DWORD OutputWriteCursor;
    DWORD OutputLocation;
    DWORD OutputByteCount;
    DWORD ExpectedFlipPlayCursor;
    DWORD FlipPlayCursor;
    DWORD FlipWriteCursor;    

} win32_debug_time_marker;

#define __WIN32_HANDEMADE_H__
#endif
