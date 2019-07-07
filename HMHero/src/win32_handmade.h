#ifndef __WIN32_HANDEMADE_H__

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

typedef struct win32_sound_output
{
    int SamplesPerSecond;
    int BytesPerSample;
    int SecondaryBufferSize;
    int LatencySampleCount;
    uint32_t RunningSampleIndex;
    float tSine;
} win32_sound_output;

#define __WIN32_HANDEMADE_H__
#endif
