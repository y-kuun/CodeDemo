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

inline uint32_t
SafeTruncateUInt64(uint64_t Value)
{
    Assert(Value <= 0xFFFFFFFF);
    uint32_t Result = (uint32_t)Value;
    return(Result);
}

#if HANDMADE_INTERNAL
typedef struct debug_read_file_result
{
    uint32_t ContentsSize;
    void *Contents;
} debug_read_file_result;

internal debug_read_file_result DEBUGPlatformReadEntireFile(char *Filename);
internal void DEBUGPlatformFreeFileMemory(void *Mermoy);
internal bool DEBUGPlatformWriteEntireFile(char *Filename, uint32_t MemorySize, void *Memory);
#endif

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
    bool IsConnected;
    bool IsAnalog;    
    float StickAverageX;
    float StickAverageY;
    
    union
    {
        game_button_state Buttons[12];
        struct
        {
            game_button_state MoveUp;
            game_button_state MoveDown;
            game_button_state MoveLeft;
            game_button_state MoveRight;
            
            game_button_state ActionUp;
            game_button_state ActionDown;
            game_button_state ActionLeft;
            game_button_state ActionRight;
            
            game_button_state LeftShoulder;
            game_button_state RightShoulder;

            game_button_state Back;
            game_button_state Start;
            
            game_button_state Terminator;
        };
    };

} game_controller_input;

typedef struct game_input
{
    game_controller_input Controllers[5];
} game_input;

inline game_controller_input *GetController(game_input *Input, int unsigned ControllerIndex)
{
    Assert(ControllerIndex < ArrayCount(Input->Controllers));
    
    game_controller_input *Result = &Input->Controllers[ControllerIndex];
    return(Result);
}

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
    
internal void
GameUpdateAndRender(game_memory *Memory, game_input *Input, game_offscreen_buffer *Buffer);

#define __HANDMADE_H__
#endif
