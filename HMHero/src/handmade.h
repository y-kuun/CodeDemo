#ifndef __HANDMADE_H__


/*
  HANDMADE_INTERNAL:
  0 - Build for public release
  1 - Build for developer only

  HANDMADE_SLOW:
  0 - Not slow code allowed
  1 - Slow code welcome
 */

#if HANDMADE_SLOW
#define Assert(Expression) if(!(Expression)) {*(int*)0 = 0;}
#else
#define Assert(Expression)
#endif

#define Kilobytes(Value) ((Value) * 1024LL)
#define Megabytes(Value) (Kilobytes(Value) * 1024LL)
#define Gigabytes(Value) (Megabytes(Value) * 1024LL)
#define Terabytes(Value) (Gigabytes(Value) * 1024LL)

// FOUR THINGS: timing, controller/ keyboard input, bitmap to use, sound buffer to use

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

typedef struct game_offscreen_buffer
{
    void *Memory;
    int Width;
    int Height;
    int Pitch;
} game_offscreen_buffer;

typedef struct game_sound_output_buffer
{
    int SamplesPerSecond;
    int SampleCount;
    int16_t *Samples;
} game_sound_output_buffer;

typedef struct game_button_state
{
    int HalfTransitionCount;
    bool EndedDown;
} game_button_state;

typedef struct game_controller_input
{
    bool IsAnalog;
    
    float StartX;
    float StartY;
    
    float MinX;
    float MinY;

    float MaxX;
    float MaxY;

    float EndX;
    float EndY;
    
    union
    {
        game_button_state Buttons[6];
        struct
        {
            game_button_state Up;
            game_button_state Down;
            game_button_state Left;
            game_button_state Right;
            game_button_state LeftShoulder;
            game_button_state RightShoulder;
        };
    };

} game_controller_input;

typedef struct game_input
{
    game_controller_input Controllers[4];
} game_input;

typedef struct game_memory
{
    bool IsInitialized;
    uint64_t PermanetStorageSize;
    void *PermanetStorage;

    uint64_t TransientStorageSize;
    void *TransientStorage;
} game_memory;

typedef struct game_state
{
    int ToneHz;
    int GreenOffset;
    int BlueOffset;
} game_state;
    
internal void GameUpdateAndRender(game_memory *Memory, game_input *Input, game_offscreen_buffer *Buffer,
                                  game_sound_output_buffer *SoundBuffer);

#define __HANDMADE_H__
#endif
