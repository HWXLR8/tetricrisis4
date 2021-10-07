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
#ifndef AUDIO
#define AUDIO

class Audio
{
public:

    Audio(void);
    virtual ~Audio(void);

    bool AudioWorking;

    char Mix_Init_Error[256];

    #define MusicTotal      26
    Mix_Music *MusicTrack[MusicTotal];
    int MusicVolume;
    Uint8 CurrentlySelectedMusicTrack;
    #define SoundTotal      15
    Mix_Chunk *SoundFX[SoundTotal];
    int SoundVolume;

    void SetupAudio(void);
    void PlayMusic(Uint8 musicIndex, int loop);
    void PlayDigitalSoundFX(Uint8 soundIndex, int loop);
};

#endif
