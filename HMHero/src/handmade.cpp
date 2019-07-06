#include "handmade.h"

internal void
GameOutputSound(game_sound_output_buffer *SoundBuffer, int ToneHz)
{
    local_persist float tSine;
    int16_t ToneVolume = 3000;
    int WavePeriod = SoundBuffer->SamplesPerSecond/ToneHz;
    int16_t *SampleOut = SoundBuffer->Samples;
    for(int SampleIndex = 0;
        SampleIndex < SoundBuffer->SampleCount;
        ++SampleIndex)
    {
        // TODO(casey): Draw this out for people
        float SineValue = sinf(tSine);
        int16_t SampleValue = (int16_t)(SineValue * ToneVolume);
        *SampleOut++ = SampleValue;
        *SampleOut++ = SampleValue;

        tSine += 2.0f*Pi32*1.0f/(float)WavePeriod;
    }
}
    

internal void
RenderWeirdGradient(game_offscreen_buffer *Buffer, int BlueOffset, int GreenOffset)
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
GameUpdateAndRender(game_offscreen_buffer *Buffer,
                    game_sound_output_buffer *SoundBuffer, int ToneHz)
{
    int BlueOffset = 0;
    int GreenOffset = 0;
    GameOutputSound(SoundBuffer, ToneHz);
    RenderWeirdGradient(Buffer, BlueOffset, GreenOffset);
}
