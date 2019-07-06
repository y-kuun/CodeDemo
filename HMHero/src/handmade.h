#ifndef __HANDMADE_H__

// style 1, virtual all platform related parts here
// disadvantages: game no need the handle those complexities of the platform layers

// style 2, game as service to the OS level
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

internal void GameUpdateAndRender(game_offscreen_buffer *Buffer,
                                  game_sound_output_buffer *SoundBuffer, int ToneHz);

#define __HANDMADE_H__
#endif
