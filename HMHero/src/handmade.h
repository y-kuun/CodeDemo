#ifndef __HANDMADE_H__

// game as service to the OS level
/*
  OS level ->
  what to draw on the screen
  what to play on the sound device
  here is the user input
  
  read & write file
 */

/*
  TODO(ykdu):  Services that the platform layer provides to the game
 */

/*
  NOTE(ykdu): Services that the game provides to the platform layer
  (this may expand in the futur - sound on separete thread, etc.)
*/

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
    
internal void GameUpdateAndRender(game_input *Input, game_offscreen_buffer *Buffer,
                                  game_sound_output_buffer *SoundBuffer);

#define __HANDMADE_H__
#endif
