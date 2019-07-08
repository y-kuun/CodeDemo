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

        tSine += 2.0f * Pi32 * 1.0f / (float)WavePeriod;
        if(tSine > 2.0f * Pi32)
        {
            tSine -= 2.0f * Pi32;
        }
    }
}


internal void
RenderWeirdGradient(game_offscreen_buffer *Buffer, int BlueOffset, int GreenOffset)
{
    uint8_t *Row = (uint8_t*)Buffer->Memory;
    for(int Y = 0; Y < Buffer->Height; Y++)
    {
        uint32_t *Pixel = (uint32_t*)Row;
        for(int X = 0; X < Buffer->Width; X++)
        {
            uint8_t Blue = (uint8_t)(X + BlueOffset);
            uint8_t Green = (uint8_t)(Y + GreenOffset);
            *Pixel++ = ((Green << 8) | Blue);
        }
        Row += Buffer->Pitch;
    }
}

internal void
GameUpdateAndRender(game_memory *Memory, game_input *Input, game_offscreen_buffer *Buffer)
{
    Assert((&Input->Controllers[0].Terminator - &Input->Controllers[0].Buttons[0]) ==
           (ArrayCount(Input->Controllers[0].Buttons)));
    Assert(sizeof(game_state) <= Memory->PermanetStorageSize);

    game_state *GameState = (game_state*) Memory->PermanetStorage;
    if(!Memory->IsInitialized)
    {
        char *Filename = __FILE__;

        debug_read_file_result File = DEBUGPlatformReadEntireFile(Filename);
        if(File.Contents)
        {
            DEBUGPlatformWriteEntireFile("test.out", File.ContentsSize, File.Contents);
            DEBUGPlatformFreeFileMemory(File.Contents);
        }


        GameState->ToneHz = 512;
        Memory->IsInitialized = true;
    }

    for(int ControllerIndex = 0; ControllerIndex < ArrayCount(Input->Controllers); ++ControllerIndex)
    {
        game_controller_input *Controller = GetController(Input, ControllerIndex);
        if(Controller->IsAnalog)
        {
            // NOTE(casey): Use analog movement tuning
            GameState->BlueOffset += (int)(4.0f*Controller->StickAverageX);
            GameState->ToneHz = 512 + (int)(128.0f*Controller->StickAverageY);
        }
        else
        {
            if(Controller->MoveDown.EndedDown)
            {
                GameState->ToneHz = (GameState->ToneHz - 1);
                if(GameState->ToneHz < 128)
                {
                    GameState->ToneHz = 128;
                }
            }
            if(Controller->MoveUp.EndedDown)
            {
                GameState->ToneHz = (GameState->ToneHz + 1);
                if(GameState->ToneHz > 256 + 128)
                {
                    GameState->ToneHz = 256 + 128;
                }
            }
            if(Controller->MoveLeft.EndedDown)
            {
                GameState->BlueOffset += 1;
            }
            if(Controller->MoveRight.EndedDown)
            {
                GameState->BlueOffset -= 1;
            }
        }
    }

    // GameOutputSound(SoundBuffer, GameState->ToneHz);
    RenderWeirdGradient(Buffer, GameState->BlueOffset, GameState->GreenOffset);
}

internal void
GameGetSoundSamples(game_memory *Memory, game_sound_output_buffer *SoundBuffer)
{
    game_state *GameState = (game_state *)Memory->PermanetStorage;
    GameOutputSound(SoundBuffer, GameState->ToneHz);
}
