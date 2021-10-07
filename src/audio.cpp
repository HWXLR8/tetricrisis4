/*
	TetriCrisis 4 110% A.I. Turbo - Puzzle game
	Copyright (C) 2016  Jesse Palser

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software Foundation,
	Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/
#include <stdio.h>
#include <cstring>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "audio.h"

#include "screens.h"
#include "visuals.h"

extern Screens* screens;
extern Visuals* visuals;

//-------------------------------------------------------------------------------------------------
Audio::Audio(void)
{
    MusicVolume = 64;
    SoundVolume = 64;
    CurrentlySelectedMusicTrack = 0;

    for (int soundIndex = 0; soundIndex < SoundTotal; soundIndex++)  SoundFX[soundIndex] = NULL;
    for (int musicIndex = 0; musicIndex < MusicTotal; musicIndex++)  MusicTrack[musicIndex] = NULL;

    AudioWorking = false;
}

//-------------------------------------------------------------------------------------------------
Audio::~Audio(void)
{
    if (AudioWorking == false)  return;

    Mix_HaltChannel(-1);
    for (int soundIndex = 0; soundIndex < SoundTotal; soundIndex++)
    {
        if (SoundFX[soundIndex] != NULL)  Mix_FreeChunk(SoundFX[soundIndex]);
        SoundFX[soundIndex] = NULL;
    }
    Mix_AllocateChannels(0);
    printf("Unloaded all music from memory.\n");

    Mix_HaltMusic();
    for (int musicIndex = 0; musicIndex < MusicTotal; musicIndex++)
    {
        if (MusicTrack[musicIndex] != NULL)  Mix_FreeMusic(MusicTrack[musicIndex]);
        MusicTrack[musicIndex] = NULL;
    }
    printf("Unloaded all sounds from memory.\n");

    Mix_CloseAudio();
    Mix_Quit();
    printf("SDL_mixer closed.\n");
}

//-------------------------------------------------------------------------------------------------
void Audio::SetupAudio(void)
{
    strcpy(Mix_Init_Error, "SDL2_Mixer: OK");

    int flags = MIX_INIT_OGG|MIX_INIT_MODPLUG;
    int initted = Mix_Init(flags);
    if ( (initted&flags) != flags )
    {
        printf("Mix_Init: Failed to init required ogg and mod support!\n");
        printf("Mix_Init: %s\n", Mix_GetError());

        strcpy(Mix_Init_Error, "SDL2_Mixer ERROR: ");
        strcat( Mix_Init_Error, Mix_GetError() );

        AudioWorking = false;
        return;
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1/*MIX_DEFAULT_CHANNELS*/, 2048)==-1)
    {
        printf("ERROR: SDL2_Mixer init failed: %s\n", Mix_GetError());

        strcpy(Mix_Init_Error, "SDL2_Mixer ERROR: ");
        strcat( Mix_Init_Error, Mix_GetError() );

        AudioWorking = false;
        return;
    }
    else printf("SDL2_Mixer initialized.\n");

    Mix_AllocateChannels(16);
    for (int channel = 0; channel < 16; channel++)
    {
        Mix_Volume(channel, SoundVolume);
    }

    for (int index = 0; index < MusicTotal; index++)
    {
        switch(index)
        {
            case 0:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Title-BGM.ogg");
                break;

            case 1:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-1-BGM.mod");
                break;

            case 2:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-2-BGM.mod");
                break;

            case 3:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-3-BGM.mod");
                break;

            case 4:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-4-BGM.mod");
                break;

            case 5:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-5-BGM.mod");
                break;

            case 6:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-6-BGM.mod");
                break;

            case 7:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-7-BGM.mod");
                break;

            case 8:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-8-BGM.mod");
                break;

            case 9:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-9-BGM.mod");
                break;

            case 10:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-10-BGM.mod");
                break;

            case 11:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-11-BGM.mod");
                break;

            case 12:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-12-BGM.mod");
                break;

            case 13:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-13-BGM.mod");
                break;

            case 14:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-14-BGM.mod");
                break;

            case 15:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-15-BGM.mod");
                break;

            case 16:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-16-BGM.mod");
                break;

            case 17:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-17-BGM.mod");
                break;

            case 18:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-18-BGM.mod");
                break;

            case 19:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-19-BGM.mod");
                break;

            case 20:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-20-BGM.ogg");
                break;

            case 21:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-21-BGM.ogg");
                break;

            case 22:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-22-BGM.ogg");
                break;

            case 23:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Track-23-BGM.ogg");
                break;

            case 24:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Crisis-I-Mode-Level7-BGM.mod");
                break;

            case 25:
                MusicTrack[index] = Mix_LoadMUS("data/audio/Crisis-I-Mode-Ending-BGM.ogg");
                break;

            default:
                break;
        }

        if(!MusicTrack[index] && MusicTotal != 0)
        {
            printf("ERROR: Couldn't load music %s\n", Mix_GetError());
        }
        else  Mix_VolumeMusic(MusicVolume);
    }
    printf("Loaded all music into memory.\n");

    for (int index = 0; index < SoundTotal; index++)
    {
        switch(index)
        {
            case 0:
                SoundFX[index] = Mix_LoadWAV("data/audio/Menu-MoveWav.ogg");
                break;

            case 1:
                SoundFX[index] = Mix_LoadWAV("data/audio/Menu-ClickWav.ogg");
                break;

            case 2:
                SoundFX[index] = Mix_LoadWAV("data/audio/Move-PieceWav.ogg");
                break;

            case 3:
                SoundFX[index] = Mix_LoadWAV("data/audio/Piece-CollisionWav.ogg");
                break;

            case 4:
                SoundFX[index] = Mix_LoadWAV("data/audio/Piece-DropWav.ogg");
                break;

            case 5:
                SoundFX[index] = Mix_LoadWAV("data/audio/Piece-RotateWav.ogg");
                break;

            case 6:
                SoundFX[index] = Mix_LoadWAV("data/audio/Line-ClearedWav.ogg");
                break;

            case 7:
                SoundFX[index] = Mix_LoadWAV("data/audio/Tetri-ClearedWav.ogg");
                break;

            case 8:
                SoundFX[index] = Mix_LoadWAV("data/audio/Level-UpWav.ogg");
                break;

            case 9:
                SoundFX[index] = Mix_LoadWAV("data/audio/Must-Think-In-RussianWav.ogg");
                break;

            case 10:
                SoundFX[index] = Mix_LoadWAV("data/audio/Incoming-LineWav.ogg");
                break;

            case 11:
                SoundFX[index] = Mix_LoadWAV("data/audio/Game-OverWav.ogg");
                break;

            case 12:
                SoundFX[index] = Mix_LoadWAV("data/audio/CrackWav.ogg");
                break;

            case 13:
                SoundFX[index] = Mix_LoadWAV("data/audio/Shall-We-Play-A-GameWav.ogg");
                break;

            case 14:
                SoundFX[index] = Mix_LoadWAV("data/audio/SwordWav.ogg");
                break;

            default:
                break;
        }

        if(!SoundFX[index])
        {
            printf("ERROR: Couldn't load sound effect %s\n", Mix_GetError());
        }
        else  Mix_VolumeChunk(SoundFX[index], MIX_MAX_VOLUME);
    }
    printf("Loaded all sounds into memory.\n");

    AudioWorking = true;
}

//-------------------------------------------------------------------------------------------------
void Audio::PlayMusic(Uint8 musicIndex, int loop)
{
    if (AudioWorking == false)  return;

    if (MusicTrack[musicIndex] == NULL)  return;

    if (musicIndex >= MusicTotal)  return;

    if (Mix_PlayingMusic() == 1)  Mix_HaltMusic();

    Mix_VolumeMusic(MusicVolume);

    CurrentlySelectedMusicTrack = musicIndex;

    if(Mix_PlayMusic(MusicTrack[musicIndex], loop)==-1)
    {
        printf( "Mix_PlayMusic: %s\n", Mix_GetError() );
    }

    switch(CurrentlySelectedMusicTrack)
    {
        case 0:
            SDL_SetWindowTitle(visuals->Window, "t.A.T.u. - ''All About Us''");
            break;

        case 1:
            SDL_SetWindowTitle(visuals->Window, "D.J. Fading Twilight - ''T-Crisis 3 100% A.I. InGame Track 00''");
            break;

        case 2:
            SDL_SetWindowTitle(visuals->Window, "D.J. Fading Twilight - ''T-Crisis 3 100% A.I. InGame Track 01''");
            break;

        case 3:
            SDL_SetWindowTitle(visuals->Window, "D.J. Fading Twilight - ''T-Crisis 3 100% A.I. InGame Track 02''");
            break;

        case 4:
            SDL_SetWindowTitle(visuals->Window, "D.J. Fading Twilight - ''T-Crisis 3 100% A.I. InGame Track 03''");
            break;

        case 5:
            SDL_SetWindowTitle(visuals->Window, "D.J. Fading Twilight - ''T-Crisis 3 100% A.I. InGame Track 04''");
            break;

        case 6:
            SDL_SetWindowTitle(visuals->Window, "D.J. Fading Twilight - ''T-Crisis 3 100% A.I. InGame Track 05''");
            break;

        case 7:
            SDL_SetWindowTitle(visuals->Window, "D.J. Fading Twilight - ''T-Crisis 3 100% A.I. InGame Track 06''");
            break;

        case 8:
            SDL_SetWindowTitle(visuals->Window, "D.J. Fading Twilight - ''T-Crisis 3 100% A.I. InGame Track 07''");
            break;

        case 9:
            SDL_SetWindowTitle(visuals->Window, "D.J. Fading Twilight - ''T-Crisis 3 100% A.I. InGame Track 08''");
            break;

        case 10:
            SDL_SetWindowTitle(visuals->Window, "D.J. Fading Twilight - ''T-Crisis 3 100% A.I. InGame Track 09''");
            break;

        case 11:
            SDL_SetWindowTitle(visuals->Window, "D.J. Fading Twilight - ''T-Crisis 3 100% A.I. InGame Track 10''");
            break;

        case 12:
            SDL_SetWindowTitle(visuals->Window, "D.J. Fading Twilight - ''T-Crisis 3 100% A.I. InGame Track 11''");
            break;

        case 13:
            SDL_SetWindowTitle(visuals->Window, "D.J. Fading Twilight - ''T-Crisis 3 100% A.I. InGame Track 12''");
            break;

        case 14:
            SDL_SetWindowTitle(visuals->Window, "D.J. Fading Twilight - ''T-Crisis 3 100% A.I. InGame Track 13''");
            break;

        case 15:
            SDL_SetWindowTitle(visuals->Window, "D.J. Fading Twilight - ''T-Crisis 3 100% A.I. InGame Track 14''");
            break;

        case 16:
            SDL_SetWindowTitle(visuals->Window, "D.J. Fading Twilight - ''T-Crisis 3 100% A.I. InGame Track 15''");
            break;

        case 17:
            SDL_SetWindowTitle(visuals->Window, "D.J. Fading Twilight - ''T-Crisis 3 100% A.I. InGame Track 16''");
            break;

        case 18:
            SDL_SetWindowTitle(visuals->Window, "D.J. Fading Twilight - ''T-Crisis 3 100% A.I. InGame Track 17''");
            break;

        case 19:
            SDL_SetWindowTitle(visuals->Window, "D.J. Fading Twilight - ''T-Crisis 3 100% A.I. InGame Track 18''");
            break;

        case 20:
            SDL_SetWindowTitle(visuals->Window, "Pajama Party - '' Yo No Se' ''(D.J. Fading Twilight Remix)");
            break;

        case 21:
            SDL_SetWindowTitle(visuals->Window, "Aria - ''Without You''(Russian Lyrics)");
            break;

        case 22:
            SDL_SetWindowTitle(visuals->Window, "Europe - ''The Final Countdown''");
            break;

        case 23:
            SDL_SetWindowTitle(visuals->Window, "Milli Vanilli - '' Girl You Know It's True ''(Numarx Remix)");
            break;

        case 24:
            SDL_SetWindowTitle(visuals->Window, "D.J. Fading Twilight - ''T-Crisis 3 100% A.I. Crisis+Mode Level 7''");
            break;

        case 25:
            SDL_SetWindowTitle(visuals->Window, "Sweet Sensation - ''Hooked On You''");
            break;

        default:
            break;
    }

    if (MusicVolume == 0)  SDL_SetWindowTitle(visuals->Window, "''TetriCrisis 4 110% A.I. Turbo'' - (C)2016 By 16BitSoft Inc.");
}

//-------------------------------------------------------------------------------------------------
void Audio::PlayDigitalSoundFX(Uint8 soundIndex, int loops)
{
    if (AudioWorking == false)  return;

    if (SoundFX[soundIndex] == NULL)  return;

    if (soundIndex >= SoundTotal)  return;

    if (SoundVolume == 0)  return;

    for (int index = 1; index < 16; index++)
        Mix_Volume( index, SoundVolume );

    for (int indexTwo = 0; indexTwo < SoundTotal; indexTwo++)
        Mix_VolumeChunk(SoundFX[indexTwo], SoundVolume);

    if(Mix_PlayChannel(-1, SoundFX[soundIndex], loops)==-1)
    {
        printf( "Mix_PlayChannel: %s\n",Mix_GetError() );
    }
}
//-------------------------------------------------------------------------------------------------
