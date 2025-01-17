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
#include <cstdlib>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "logic.h"

#include "audio.h"
#include "screens.h"
#include "input.h"
#include "visuals.h"

extern Audio* audio;
extern Screens* screens;
extern Input* input;
extern Visuals* visuals;

//-------------------------------------------------------------------------------------------------
Logic::Logic(void)
{
    GameMode = OriginalMode;

    CPUPlayerEnabled = 2;

    SelectedBackground = 0;
    SelectedMusicTrack = 19;
    NewGameGarbageHeight = 0;

    DisplayNextPiece = true;
    DisplayDropShadow = true;

    PressingUPAction = Rotate;

    for (int player = 0; player < NumberOfPlayers; player++)
    {
        PlayerData[player].PlayerInput = -1;

        PlayerData[player].UPActionTaken = false;
        PlayerData[player].RotateDirection = 0;

        PlayerData[player].Score = 0;
        PlayerData[player].DropBonus = 0;
        PlayerData[player].Level = 0;
        PlayerData[player].Lines = 0;
    }


    PlayingGameFrameLock = 33;

    TileSet = 0;

    GameWasJustPlayed = false;
    CrisisWon = false;

    ThinkRussianTimer = 0;

    TotalCPUPlayerLines = 0;
    NumberofCPUGames = 0;
    TotalOneLines = 0;
    TotalTwoLines = 0;
    TotalThreeLines = 0;
    TotalFourLines = 0;

    MaxRotationArray[0] = 0;
    MaxRotationArray[1] = 2;
    MaxRotationArray[2] = 2;
    MaxRotationArray[3] = 4;
    MaxRotationArray[4] = 4;
    MaxRotationArray[5] = 4;
    MaxRotationArray[6] = 1;
    MaxRotationArray[7] = 2;

    PieceDropStartHeight[0] = 0;
    PieceDropStartHeight[1] = 4;
    PieceDropStartHeight[2] = 4;
    PieceDropStartHeight[3] = 4;
    PieceDropStartHeight[4] = 4;
    PieceDropStartHeight[5] = 4;
    PieceDropStartHeight[6] = 3;
    PieceDropStartHeight[7] = 5;

	for (Uint8 player = 0; player < NumberOfPlayers; player++)
	{
		for (Uint8 indexTwo = 0; indexTwo < 4; indexTwo++)
		{
			PlayerData[player].PieceHistory[indexTwo] = ( rand()%7 + 1 );
		}
	}

	for (Uint8 player = 0; player < NumberOfPlayers; player++)
	{
		for (Uint8 indexTwo = 0; indexTwo < 4; indexTwo++)
		{
			for (Uint8 indexThree = 0; indexThree < 4; indexThree++)
			{
				if (indexThree != indexTwo)
				{
					while (PlayerData[player].PieceHistory[indexTwo] == PlayerData[player].PieceHistory[indexThree])
						PlayerData[player].PieceHistory[indexTwo] = ( rand()%7 + 1 );
				}
			}
		}
	}

    Player = 0;

    DelayAutoShift = 2;

    PAUSEgame = false;
}

//-------------------------------------------------------------------------------------------------
Logic::~Logic(void)
{

}

//-------------------------------------------------------------------------------------------------

/*  __________________
    \___  __  __  ___/
       / / / / / /                  TM
  T+B / / / / / / 100% PerfecT+EnginE   Version 10.0  [MultI+PlayeR A.I.]
   __/ /_/ /_/ /__
 /________________\

*/

//-------------------------------------------------------------------------------------------------
void Logic::InitializePieceData(void)
{
Uint8 piece, rotation, box;

    for (piece = 0; piece < 8; piece++)
        for (rotation = 0; rotation < 5; rotation++)
            for (box = 0; box < 17; box++)
                PieceData [piece] [rotation] [box] = 0;

    //RED "S Piece"...
    PieceData [1] [1] [10] = 1; // 01 02 03 04
    PieceData [1] [1] [11] = 1; // 05 06 07 08
    PieceData [1] [1] [13] = 1; // 09 [] [] 12
    PieceData [1] [1] [14] = 1; // [] [] 15 16

    PieceData [1] [2] [ 5] = 1;
    PieceData [1] [2] [ 9] = 1;
    PieceData [1] [2] [10] = 1;
    PieceData [1] [2] [14] = 1;

    PieceData [1] [3] [10] = 1;
    PieceData [1] [3] [11] = 1;
    PieceData [1] [3] [13] = 1;
    PieceData [1] [3] [14] = 1;

    PieceData [1] [4] [ 5] = 1;
    PieceData [1] [4] [ 9] = 1;
    PieceData [1] [4] [10] = 1;
    PieceData [1] [4] [14] = 1;

    //ORANGE "Z Piece"...
    PieceData [2] [1] [ 9] = 1;
    PieceData [2] [1] [10] = 1;
    PieceData [2] [1] [14] = 1;
    PieceData [2] [1] [15] = 1;

    PieceData [2] [2] [ 6] = 1;
    PieceData [2] [2] [ 9] = 1;
    PieceData [2] [2] [10] = 1;
    PieceData [2] [2] [13] = 1;

    PieceData [2] [3] [ 9] = 1;
    PieceData [2] [3] [10] = 1;
    PieceData [2] [3] [14] = 1;
    PieceData [2] [3] [15] = 1;

    PieceData [2] [4] [ 6] = 1;
    PieceData [2] [4] [ 9] = 1;
    PieceData [2] [4] [10] = 1;
    PieceData [2] [4] [13] = 1;

    //AQUA "T Piece"...
    PieceData [3] [1] [ 9] = 1;
    PieceData [3] [1] [10] = 1;
    PieceData [3] [1] [11] = 1;
    PieceData [3] [1] [14] = 1;

    PieceData [3] [2] [ 6] = 1;
    PieceData [3] [2] [ 9] = 1;
    PieceData [3] [2] [10] = 1;
    PieceData [3] [2] [14] = 1;

    PieceData [3] [3] [ 6] = 1;
    PieceData [3] [3] [ 9] = 1;
    PieceData [3] [3] [10] = 1;
    PieceData [3] [3] [11] = 1;

    PieceData [3] [4] [ 6] = 1;
    PieceData [3] [4] [10] = 1;
    PieceData [3] [4] [11] = 1;
    PieceData [3] [4] [14] = 1;

    //YELLOW "L Piece"...
    PieceData [4] [1] [ 9] = 1;
    PieceData [4] [1] [10] = 1;
    PieceData [4] [1] [11] = 1;
    PieceData [4] [1] [13] = 1;

    PieceData [4] [2] [ 5] = 1;
    PieceData [4] [2] [ 6] = 1;
    PieceData [4] [2] [10] = 1;
    PieceData [4] [2] [14] = 1;

    PieceData [4] [3] [ 7] = 1;
    PieceData [4] [3] [ 9] = 1;
    PieceData [4] [3] [10] = 1;
    PieceData [4] [3] [11] = 1;

    PieceData [4] [4] [ 6] = 1;
    PieceData [4] [4] [10] = 1;
    PieceData [4] [4] [14] = 1;
    PieceData [4] [4] [15] = 1;

    //GREEN "Backwards L Piece"...
    PieceData [5] [1] [ 9] = 1;
    PieceData [5] [1] [10] = 1;
    PieceData [5] [1] [11] = 1;
    PieceData [5] [1] [15] = 1;

    PieceData [5] [2] [ 6] = 1;
    PieceData [5] [2] [10] = 1;
    PieceData [5] [2] [13] = 1;
    PieceData [5] [2] [14] = 1;

    PieceData [5] [3] [ 5] = 1;
    PieceData [5] [3] [ 9] = 1;
    PieceData [5] [3] [10] = 1;
    PieceData [5] [3] [11] = 1;

    PieceData [5] [4] [ 6] = 1;
    PieceData [5] [4] [ 7] = 1;
    PieceData [5] [4] [10] = 1;
    PieceData [5] [4] [14] = 1;

    //BLUE "Box Piece"...
    PieceData [6] [1] [10] = 1;
    PieceData [6] [1] [11] = 1;
    PieceData [6] [1] [14] = 1;
    PieceData [6] [1] [15] = 1;

    PieceData [6] [2] [10] = 1;
    PieceData [6] [2] [11] = 1;
    PieceData [6] [2] [14] = 1;
    PieceData [6] [2] [15] = 1;

    PieceData [6] [3] [10] = 1;
    PieceData [6] [3] [11] = 1;
    PieceData [6] [3] [14] = 1;
    PieceData [6] [3] [15] = 1;

    PieceData [6] [4] [10] = 1;
    PieceData [6] [4] [11] = 1;
    PieceData [6] [4] [14] = 1;
    PieceData [6] [4] [15] = 1;

    //PURPLE "Line Piece"...
    PieceData [7] [1] [ 9] = 1;
    PieceData [7] [1] [10] = 1;
    PieceData [7] [1] [11] = 1;
    PieceData [7] [1] [12] = 1;

    PieceData [7] [2] [ 2] = 1;
    PieceData [7] [2] [ 6] = 1;
    PieceData [7] [2] [10] = 1;
    PieceData [7] [2] [14] = 1;

    PieceData [7] [3] [ 9] = 1;
    PieceData [7] [3] [10] = 1;
    PieceData [7] [3] [11] = 1;
    PieceData [7] [3] [12] = 1;

    PieceData [7] [4] [ 2] = 1;
    PieceData [7] [4] [ 6] = 1;
    PieceData [7] [4] [10] = 1;
    PieceData [7] [4] [14] = 1;
}

//-------------------------------------------------------------------------------------------------
void Logic::ClearPlayfieldsWithCollisionDetection(void)
{
	for (int player = 0; player < NumberOfPlayers; player++)
	{
		for (int y = 0; y < 26; y++)
			for (int x = 0; x < 15; x++)
				PlayerData[player].Playfield[x][y] = 255; // Collision detection value

        for (int y = 2; y < 5; y++)
            for (int x = 5; x < 9; x++)
                PlayerData[player].Playfield[x][y] = 0;

        for (int y = 5; y < 24; y++)
            for (int x = 2; x < 12; x++)
                PlayerData[player].Playfield[x][y] = 0;
	}
}

//-------------------------------------------------------------------------------------------------
Uint8 Logic::GetRandomPiece(void)
{
	Uint8 random = PlayerData[Player].PieceHistory[0];

	for (Uint8 index = 0; index < 3; index++)
	{
		PlayerData[Player].PieceHistory[index] = PlayerData[Player].PieceHistory[index+1];
	}

	Uint8 numberOfTries = 0;
	while ( (PlayerData[Player].PieceHistory[3] == PlayerData[Player].PieceHistory[0]
	      || PlayerData[Player].PieceHistory[3] == PlayerData[Player].PieceHistory[1]
	      || PlayerData[Player].PieceHistory[3] == PlayerData[Player].PieceHistory[2])
		  && (numberOfTries < 3) )
	{
		PlayerData[Player].PieceHistory[3] = ( rand()%7 + 1 );
		numberOfTries++;
	}

	return(random);
}

//-------------------------------------------------------------------------------------------------
int Logic::PieceCollision(void)
{
Sint8 x, y;
Uint8 box = 1;
int returnValue = CollisionNotTrue;

    for (y = 0; y < 4; y++)
        for (x = 0; x < 4; x++)
        {
            if ( (PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX + x ][ PlayerData[Player].PiecePlayfieldY + y ] > 1)
                &&(PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [box] > 0) )
                    returnValue = CollisionWithPlayfield;

            box++;
        }

    return(returnValue);
}

//-------------------------------------------------------------------------------------------------
int Logic::PieceCollisionDown(void)
{
Sint8 x, y;
Uint8 box = 1;
int returnValue = CollisionNotTrue;

    for (y = 1; y < 5; y++)
        for (x = 0; x < 4; x++)
        {
            if ( (PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX + x ][ PlayerData[Player].PiecePlayfieldY + y ] > 1)
                &&(PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [box] > 0) )
                    returnValue = CollisionWithPlayfield;

            box++;
        }

    return(returnValue);
}

//-------------------------------------------------------------------------------------------------
void Logic::AddPieceToPlayfieldMemory(int TempOrCurrentOrNextOrDropShadow)
{
    if ( (DisplayDropShadow == false)&&(TempOrCurrentOrNextOrDropShadow == DropShadow) )
        return;

	Uint8 TEMP_Piece = PlayerData[Player].Piece;
	Uint8 TEMP_PieceRotation = PlayerData[Player].PieceRotation;
	Uint8 TEMP_PiecePlayfieldX = PlayerData[Player].PiecePlayfieldX;
	Uint8 TEMP_PiecePlayfieldY = PlayerData[Player].PiecePlayfieldY;

	int value = PlayerData[Player].Piece+10;

    if (TempOrCurrentOrNextOrDropShadow == Next)
	{
		PlayerData[Player].Piece = PlayerData[Player].NextPiece;
		value = PlayerData[Player].NextPiece+10;
		PlayerData[Player].PieceRotation = 1;

        if      (Player == 0)  PlayerData[Player].PiecePlayfieldX = 5;
        else if (Player == 1)  PlayerData[Player].PiecePlayfieldX = 5;
        else if (Player == 2)  PlayerData[Player].PiecePlayfieldX = 5;

		PlayerData[Player].PiecePlayfieldY = 0;
	}
	else if (TempOrCurrentOrNextOrDropShadow == DropShadow)
	{
		for (int y = PlayerData[Player].PiecePlayfieldY; y < 23; y++)
		{
			PlayerData[Player].PiecePlayfieldY = y;
			if (PieceCollision() != CollisionNotTrue)
			{
				if (y - TEMP_PiecePlayfieldY > 4)
				{
					value = 1;
					PlayerData[Player].PiecePlayfieldY = y-1;
					y = 100;
				}
				else
				{
					PlayerData[Player].Piece = TEMP_Piece;
					PlayerData[Player].PieceRotation = TEMP_PieceRotation;
					PlayerData[Player].PiecePlayfieldX = TEMP_PiecePlayfieldX;
					PlayerData[Player].PiecePlayfieldY = TEMP_PiecePlayfieldY;
					return;
				}
			}
		}
	}

	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 1] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 2] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 3] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 4] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY ] = value;

	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 5] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY+1 ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 6] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY+1 ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 7] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY+1 ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 8] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY+1 ] = value;

	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 9] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY+2 ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [10] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY+2 ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [11] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY+2 ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [12] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY+2 ] = value;

	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [13] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY+3 ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [14] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY+3 ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [15] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY+3 ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [16] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY+3 ] = value;

	PlayerData[Player].Piece = TEMP_Piece;
	PlayerData[Player].PieceRotation = TEMP_PieceRotation;
	PlayerData[Player].PiecePlayfieldX = TEMP_PiecePlayfieldX;
	PlayerData[Player].PiecePlayfieldY = TEMP_PiecePlayfieldY;
}

//-------------------------------------------------------------------------------------------------
void Logic::DeletePieceFromPlayfieldMemory(int CurrentOrDropShadow)
{
    if ( (DisplayDropShadow == false)&&(CurrentOrDropShadow == DropShadow) )
        return;

    if (PlayerData[Player].PlayerStatus == FlashingCompletedLines || PlayerData[Player].PlayerStatus == ClearingCompletedLines)
        return;

	Uint8 TEMP_Piece = PlayerData[Player].Piece;
	Uint8 TEMP_PieceRotation = PlayerData[Player].PieceRotation;
	Uint8 TEMP_PiecePlayfieldX = PlayerData[Player].PiecePlayfieldX;
	Uint8 TEMP_PiecePlayfieldY = PlayerData[Player].PiecePlayfieldY;

	if (CurrentOrDropShadow == DropShadow)
	{
		for (int y = PlayerData[Player].PiecePlayfieldY; y < 23; y++)
		{
			PlayerData[Player].PiecePlayfieldY = y;
			if (PieceCollision() != CollisionNotTrue)
			{
				if (y - TEMP_PiecePlayfieldY > 4)
				{
					PlayerData[Player].PiecePlayfieldY = y-1;
					y = 100;
				}
				else
				{
					PlayerData[Player].Piece = TEMP_Piece;
					PlayerData[Player].PieceRotation = TEMP_PieceRotation;
					PlayerData[Player].PiecePlayfieldX = TEMP_PiecePlayfieldX;
					PlayerData[Player].PiecePlayfieldY = TEMP_PiecePlayfieldY;
					return;
				}
			}
		}
	}

	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 1] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 2] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 3] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 4] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY ] = 0;

	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 5] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY+1 ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 6] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY+1 ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 7] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY+1 ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 8] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY+1 ] = 0;

	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 9] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY+2 ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [10] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY+2 ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [11] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY+2 ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [12] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY+2 ] = 0;

	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [13] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY+3 ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [14] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY+3 ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [15] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY+3 ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [16] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY+3 ] = 0;

	PlayerData[Player].Piece = TEMP_Piece;
	PlayerData[Player].PieceRotation = TEMP_PieceRotation;
	PlayerData[Player].PiecePlayfieldX = TEMP_PiecePlayfieldX;
	PlayerData[Player].PiecePlayfieldY = TEMP_PiecePlayfieldY;
}

//-------------------------------------------------------------------------------------------------
void Logic::SetupNewPiece(void)
{
	PlayerData[Player].Piece = PlayerData[Player].NextPiece;
	PlayerData[Player].PieceMovementDelay = 0;
	PlayerData[Player].PieceRotation = 1;

    if (Player == 0)
        PlayerData[0].PiecePlayfieldX = 5;
    else if (Player == 1)
        PlayerData[1].PiecePlayfieldX = 5;
    else if (Player == 2)
        PlayerData[2].PiecePlayfieldX = 5;
    else if (Player == 3)
        PlayerData[3].PiecePlayfieldX = 5;

	PlayerData[Player].PiecePlayfieldY = 0;

	PlayerData[Player].NextPiece = GetRandomPiece();

	PlayerData[Player].PlayerStatus = NewPieceDropping;

	PlayerData[Player].PieceDropTimer = 0;

	PlayerData[Player].PieceRotated1 = false;
	PlayerData[Player].PieceRotated2 = false;

	PlayerData[Player].FlashCompletedLinesTimer = 0;
	PlayerData[Player].ClearCompletedLinesTimer = 0;

	PlayerData[Player].DropBonus = 0;

    for (int x = 0; x < 15; x++)
    {
        for (int rotation = 1; rotation < 5; rotation++)
        {
            PlayerData[Player].MoveOneBlockCavernHoles[x][rotation] = 0;
            PlayerData[Player].MoveCompletedLines[x][rotation] = 0;
            PlayerData[Player].MovePieceHeight[x][rotation] = 0;
            PlayerData[Player].MovePlayfieldBoxEdges[x][rotation] = 0;
            PlayerData[Player].MoveTrappedHoles[x][rotation] = 0;
        }
    }

    PlayerData[Player].BestMoveX = -1;
    PlayerData[Player].BestRotation = -1;
    PlayerData[Player].BestMoveCalculated = false;
    PlayerData[Player].MovedToBestMove = false;

    GameDisplayChanged = true;
}

//-------------------------------------------------------------------------------------------------
void Logic::CheckForCompletedLines(void)
{
int numberOfCompletedLines = 0;

    AddPieceToPlayfieldMemory(Current);

	for (int y = 5; y < 24; y++)
	{
		int boxTotal = 0;

		for (int x = 2; x < 12; x++)
		{
			if ( (PlayerData[Player].Playfield[x][y] > 9)&&(PlayerData[Player].Playfield[x][y] < 20) )
				boxTotal++;
		}

		if (boxTotal == 10)
            numberOfCompletedLines++;
	}

	if (numberOfCompletedLines > 0)
	{
		if (numberOfCompletedLines == 1)
		{
			PlayerData[Player].Score += (40 * (PlayerData[Player].Level+1) );
            TotalOneLines++;
		}
		else if (numberOfCompletedLines == 2)
		{
			PlayerData[Player].Score += (100 * (PlayerData[Player].Level+1) );
            TotalTwoLines++;
		}
		else if (numberOfCompletedLines == 3)
		{
			PlayerData[Player].Score += (300 * (PlayerData[Player].Level+1) );
            TotalThreeLines++;
		}
		else if (numberOfCompletedLines == 4)
		{
			PlayerData[Player].Score += (1200 * (PlayerData[Player].Level+1) );
            TotalFourLines++;
			audio->PlayDigitalSoundFX(7, 0);
		}

		PlayerData[Player].PlayerStatus = FlashingCompletedLines;
		PlayerData[Player].FlashCompletedLinesTimer = 1;
	}
	else  SetupNewPiece();
}

//-------------------------------------------------------------------------------------------------
void Logic::MovePieceDown(bool Force)
{
    if (Force == false)
    {
        if (PlayerData[Player].PlayerInput == CPU)
        {
            if (CPUPlayerEnabled == 1)
            {
                PlayerData[Player].CPUFrame++;
                if (PlayerData[Player].CPUFrame < 3)
                {
                    return;
                }
                else  PlayerData[Player].CPUFrame = 0;
            }
            else if (CPUPlayerEnabled == 2)
            {
                PlayerData[Player].CPUFrame++;
                if (PlayerData[Player].CPUFrame < 2)
                {
                    return;
                }
                else  PlayerData[Player].CPUFrame = 0;
            }
        }
    }

    DeletePieceFromPlayfieldMemory(Current);

	PlayerData[Player].PiecePlayfieldY++;

	if (PieceCollision() == CollisionWithPlayfield)
	{
        if (PlayersCanJoin == true)
        {
            if (PlayerData[0].PlayerStatus == GameOver && CPUPlayerEnabled > 0)
            {
                PlayerData[0].PlayerInput = CPU;
                PlayerData[0].PlayerStatus = NewPieceDropping;
            }

            if (PlayerData[1].PlayerStatus == GameOver && CPUPlayerEnabled > 0)
            {
                PlayerData[1].PlayerInput = CPU;
                PlayerData[1].PlayerStatus = NewPieceDropping;
            }

            if (PlayerData[2].PlayerStatus == GameOver && CPUPlayerEnabled > 0)
            {
                PlayerData[2].PlayerInput = CPU;
                PlayerData[2].PlayerStatus = NewPieceDropping;
            }

            if (PlayerData[3].PlayerStatus == GameOver && CPUPlayerEnabled > 0)
            {
                PlayerData[3].PlayerInput = CPU;
                PlayerData[3].PlayerStatus = NewPieceDropping;
            }

            if ( CPUPlayerEnabled > 0
                 && (PlayerData[0].PlayerInput == CPU || PlayerData[2].PlayerInput == CPU) )  audio->PlayDigitalSoundFX(13, 0);

            PlayersCanJoin = false;
        }

		PlayerData[Player].PiecePlayfieldY--;

		if (ThinkRussianTimer == 0 && PlayerData[Player].PlayerInput != CPU)  audio->PlayDigitalSoundFX(3, 0);

		PlayerData[Player].Score += PlayerData[Player].DropBonus;

		PlayerData[Player].DropBonus = 0;
		PlayerData[Player].PieceDropTimer = 0;

		if (PlayerData[Player].PlayerStatus == NewPieceDropping)
        {
            PlayerData[Player].PlayerStatus = GameOver;
            audio->PlayDigitalSoundFX(11, 0);
        }
		else  CheckForCompletedLines();
	}
	else
	{
    	GameDisplayChanged = true;
	}
}

//-------------------------------------------------------------------------------------------------
void Logic::MovePieceDownFast(void)
{
	while (PieceCollision() == CollisionNotTrue)
    {
        PlayerData[Player].PiecePlayfieldY++;
        PlayerData[Player].DropBonus++;
    }

	if (PieceCollision() == CollisionWithPlayfield)
	{
        if (PlayersCanJoin == true)
        {
            if (PlayerData[0].PlayerStatus == GameOver && CPUPlayerEnabled > 0)
            {
                PlayerData[0].PlayerInput = CPU;
                PlayerData[0].PlayerStatus = NewPieceDropping;
            }

            if (PlayerData[1].PlayerStatus == GameOver && CPUPlayerEnabled > 0)
            {
                PlayerData[1].PlayerInput = CPU;
                PlayerData[1].PlayerStatus = NewPieceDropping;
            }

            if (PlayerData[2].PlayerStatus == GameOver && CPUPlayerEnabled > 0)
            {
                PlayerData[2].PlayerInput = CPU;
                PlayerData[2].PlayerStatus = NewPieceDropping;
            }

            if (PlayerData[3].PlayerStatus == GameOver && CPUPlayerEnabled > 0)
            {
                PlayerData[3].PlayerInput = CPU;
                PlayerData[3].PlayerStatus = NewPieceDropping;
            }

            if ( CPUPlayerEnabled > 0
                 && (PlayerData[0].PlayerInput == CPU || PlayerData[2].PlayerInput == CPU) )  audio->PlayDigitalSoundFX(13, 0);

            PlayersCanJoin = false;
        }

		PlayerData[Player].PiecePlayfieldY--;

		if (ThinkRussianTimer == 0 && PlayerData[Player].PlayerInput != CPU)  audio->PlayDigitalSoundFX(3, 0);

		PlayerData[Player].Score += PlayerData[Player].DropBonus;

		PlayerData[Player].DropBonus = 0;
		PlayerData[Player].PieceDropTimer = 0;

		if (PlayerData[Player].PlayerStatus == NewPieceDropping)
        {
            PlayerData[Player].PlayerStatus = GameOver;
            audio->PlayDigitalSoundFX(11, 0);
        }
		else  CheckForCompletedLines();
	}
	else
	{
		GameDisplayChanged = true;
	}
}

//-------------------------------------------------------------------------------------------------
void Logic::MovePieceDownFastDropAndDrag(void)
{
    if (PieceCollisionDown() == true)
    {

    }
    else
    {
        while (PieceCollision() == CollisionNotTrue)
        {
            PlayerData[Player].PiecePlayfieldY++;
            PlayerData[Player].DropBonus++;
        }

        PlayerData[Player].PiecePlayfieldY--;

        PlayerData[Player].PieceDropTimer = 0;

        GameDisplayChanged = true;
    }
}

//-------------------------------------------------------------------------------------------------
bool Logic::RotatePieceCounterClockwise(void)
{
	if (PlayerData[Player].PieceRotation > 1)  PlayerData[Player].PieceRotation--;
	else  PlayerData[Player].PieceRotation = 4;

	if (PieceCollision() == CollisionNotTrue)
	{
		if (ThinkRussianTimer == 0 && PlayerData[Player].PlayerInput != CPU)  audio->PlayDigitalSoundFX(5, 0);

		GameDisplayChanged = true;

        return(true);
	}
	else
	{
		if (PlayerData[Player].PieceRotation < 4)  PlayerData[Player].PieceRotation++;
		else  PlayerData[Player].PieceRotation = 1;

        if (PlayerData[Player].RotateDirection == 0)  PlayerData[Player].RotateDirection = 1;
        else  PlayerData[Player].RotateDirection = 0;
    }

    return(false);
}

//-------------------------------------------------------------------------------------------------
bool Logic::RotatePieceClockwise(void)
{
	if (PlayerData[Player].PieceRotation < 4)  PlayerData[Player].PieceRotation++;
	else  PlayerData[Player].PieceRotation = 1;

	if (PieceCollision() == CollisionNotTrue)
	{
		if (ThinkRussianTimer == 0 && PlayerData[Player].PlayerInput != CPU)  audio->PlayDigitalSoundFX(5, 0);

		GameDisplayChanged = true;

        return(true);
	}
	else
	{
		if (PlayerData[Player].PieceRotation > 1)  PlayerData[Player].PieceRotation--;
		else  PlayerData[Player].PieceRotation = 4;

        if (PlayerData[Player].RotateDirection == 0)  PlayerData[Player].RotateDirection = 1;
        else  PlayerData[Player].RotateDirection = 0;
	}

    return(false);
}

//-------------------------------------------------------------------------------------------------
void Logic::MovePieceLeft(void)
{
    if (DelayAutoShift == 0)
    {
        if (PlayerData[Player].PieceMovementDelay > -15)  PlayerData[Player].PieceMovementDelay--;
        if ( (PlayerData[Player].PieceMovementDelay == -1)
           ||(PlayerData[Player].PieceMovementDelay == -5)
           ||(PlayerData[Player].PieceMovementDelay == -9)
           ||(PlayerData[Player].PieceMovementDelay < -14) )
        {
            PlayerData[Player].PiecePlayfieldX--;

            if (PieceCollision() == CollisionNotTrue)
            {
                GameDisplayChanged = true;

                if (ThinkRussianTimer == 0 && PlayerData[Player].PlayerInput != CPU)  audio->PlayDigitalSoundFX(2, 0);
            }
            else
            {
                PlayerData[Player].PiecePlayfieldX++;
            }
        }
    }
    else if (DelayAutoShift == 1)
    {
        if (PlayerData[Player].PieceMovementDelay > -16)  PlayerData[Player].PieceMovementDelay--;
        if ( (PlayerData[Player].PieceMovementDelay == -1)
           ||(PlayerData[Player].PieceMovementDelay < -15) )
        {
            PlayerData[Player].PiecePlayfieldX--;

            if (PieceCollision() == CollisionNotTrue)
            {
                GameDisplayChanged = true;

                if (ThinkRussianTimer == 0 && PlayerData[Player].PlayerInput != CPU)  audio->PlayDigitalSoundFX(2, 0);
            }
            else
            {
                PlayerData[Player].PiecePlayfieldX++;
            }
        }
    }
    else if (DelayAutoShift == 2)
    {
        if (PlayerData[Player].PieceMovementDelay > -6)  PlayerData[Player].PieceMovementDelay--;
        if ( (PlayerData[Player].PieceMovementDelay == -1)
           ||(PlayerData[Player].PieceMovementDelay < -5) )
        {
            PlayerData[Player].PiecePlayfieldX--;

            if (PieceCollision() == CollisionNotTrue)
            {
                GameDisplayChanged = true;

                if (ThinkRussianTimer == 0 && PlayerData[Player].PlayerInput != CPU)  audio->PlayDigitalSoundFX(2, 0);
            }
            else
            {
                PlayerData[Player].PiecePlayfieldX++;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Logic::MovePieceRight(void)
{
    if (DelayAutoShift == 0)
    {
        if (PlayerData[Player].PieceMovementDelay < 15)  PlayerData[Player].PieceMovementDelay++;
        if ( (PlayerData[Player].PieceMovementDelay == 1)
           ||(PlayerData[Player].PieceMovementDelay == 5)
           ||(PlayerData[Player].PieceMovementDelay == 9)
           ||(PlayerData[Player].PieceMovementDelay > 14) )
        {
            PlayerData[Player].PiecePlayfieldX++;

            if (PieceCollision() == CollisionNotTrue)
            {
                GameDisplayChanged = true;

                if (ThinkRussianTimer == 0 && PlayerData[Player].PlayerInput != CPU)  audio->PlayDigitalSoundFX(2, 0);
            }
            else
            {
                PlayerData[Player].PiecePlayfieldX--;
            }
        }
    }
    else if (DelayAutoShift == 1)
    {
        if (PlayerData[Player].PieceMovementDelay < 16)  PlayerData[Player].PieceMovementDelay++;
        if ( (PlayerData[Player].PieceMovementDelay == 1)
           ||(PlayerData[Player].PieceMovementDelay > 15) )
        {
            PlayerData[Player].PiecePlayfieldX++;

            if (PieceCollision() == CollisionNotTrue)
            {
                GameDisplayChanged = true;

                if (ThinkRussianTimer == 0 && PlayerData[Player].PlayerInput != CPU)  audio->PlayDigitalSoundFX(2, 0);
            }
            else
            {
                PlayerData[Player].PiecePlayfieldX--;
            }
        }
    }
    else if (DelayAutoShift == 2)
    {
        if (PlayerData[Player].PieceMovementDelay < 6)  PlayerData[Player].PieceMovementDelay++;
        if ( (PlayerData[Player].PieceMovementDelay == 1)
           ||(PlayerData[Player].PieceMovementDelay > 5) )
        {
            PlayerData[Player].PiecePlayfieldX++;

            if (PieceCollision() == CollisionNotTrue)
            {
                GameDisplayChanged = true;

                if (ThinkRussianTimer == 0 && PlayerData[Player].PlayerInput != CPU)  audio->PlayDigitalSoundFX(2, 0);
            }
            else
            {
                PlayerData[Player].PiecePlayfieldX--;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Logic::SetupForNewGame(void)
{
    PsychoBackgroundRotationOne = 0.0f;
    PsychoBackgroundRotationTwo = 0.0f;

    PlayersCanJoin = true;
    GameWasJustPlayed = true;

    if (SelectedBackground == 1)
    {
        audio->PlayDigitalSoundFX(9, 0);
        ThinkRussianTimer = 320;
    }
    else  ThinkRussianTimer = 0;

	InitializePieceData();
	ClearPlayfieldsWithCollisionDetection();

    PlayerData[0].PlayersPlayfieldScreenX = 80;
    PlayerData[0].PlayersPlayfieldScreenY = 230;

    PlayerData[1].PlayersPlayfieldScreenX = 240;
    PlayerData[1].PlayersPlayfieldScreenY = 230;

    PlayerData[2].PlayersPlayfieldScreenX = 400;
    PlayerData[2].PlayersPlayfieldScreenY = 230;

    PlayerData[3].PlayersPlayfieldScreenX = 560;
    PlayerData[3].PlayersPlayfieldScreenY = 230;

    PlayerData[0].PiecePlayfieldX = 5;
    PlayerData[0].PiecePlayfieldY = 0;

    PlayerData[1].PiecePlayfieldX = 5;
    PlayerData[1].PiecePlayfieldY = 0;

    PlayerData[2].PiecePlayfieldX = 5;
    PlayerData[2].PiecePlayfieldY = 0;

    PlayerData[3].PiecePlayfieldX = 5;
    PlayerData[3].PiecePlayfieldY = 0;

    PlayerData[0].PlayfieldStartX = 2;
    PlayerData[0].PlayfieldEndX = 12;

    PlayerData[1].PlayfieldStartX = 2;
    PlayerData[1].PlayfieldEndX = 12;

    PlayerData[2].PlayfieldStartX = 2;
    PlayerData[2].PlayfieldEndX = 12;

    PlayerData[3].PlayfieldStartX = 2;
    PlayerData[3].PlayfieldEndX = 12;

	for (int player = 0; player < NumberOfPlayers; player++)
	{
		Player = player;

		PlayerData[player].Piece = GetRandomPiece();
		PlayerData[player].PieceMovementDelay = 0;
		PlayerData[player].PieceRotation = 1;

		PlayerData[player].NextPiece = GetRandomPiece();

		PlayerData[player].PieceDropTimer = 0;
		PlayerData[player].TimeToDropPiece = 47;

		PlayerData[player].PieceRotated1 = false;
		PlayerData[player].PieceRotated2 = false;

		PlayerData[player].FlashCompletedLinesTimer = 0;
		PlayerData[player].ClearCompletedLinesTimer = 0;

		if (input->DEBUG == 1) PlayerData[player].Score = 3210;
        else  PlayerData[player].Score = 0;

		PlayerData[player].DropBonus = 0;
		PlayerData[player].Level = 0;

		if (input->DEBUG == 1) PlayerData[player].Lines = 9;
        else  PlayerData[player].Lines = 0;

		for (int y = 0; y < 12; y++)
			for (int x = 0; x < 10; x++)
				PlayerData[player].AttackLines[x][y] = 0;

        if (GameMode == TwentyLineChallengeMode)  PlayerData[player].TwentyLineCounter = 20;

        for (int x = 0; x < 12; x++)
        {
            for (int rotation = 1; rotation < 5; rotation++)
            {
                PlayerData[Player].MoveOneBlockCavernHoles[x][rotation] = 0;
                PlayerData[Player].MoveCompletedLines[x][rotation] = 0;
                PlayerData[Player].MovePieceHeight[x][rotation] = 0;
                PlayerData[Player].MovePlayfieldBoxEdges[x][rotation] = 0;
                PlayerData[Player].MoveTrappedHoles[x][rotation] = 0;
            }
        }

        PlayerData[Player].BestMoveX = -1;
        PlayerData[Player].BestRotation = -1;
        PlayerData[Player].BestMoveCalculated = false;
        PlayerData[Player].MovedToBestMove = false;

        PlayerData[Player].CPUFrame = 0;

        BlockAttackTransparency[Player] = 0;
	}

    PlayerData[0].PlayerStatus = GameOver;
    PlayerData[1].PlayerStatus = NewPieceDropping;
    PlayerData[2].PlayerStatus = GameOver;
    PlayerData[3].PlayerStatus = GameOver;

    if (screens->ScreenToDisplay != TestComputerSkillScreen)
    {
        if (PlayerData[1].PlayerInput == Keyboard)
        {
            if (input->JoystickDeviceOne != NULL)  PlayerData[0].PlayerInput = JoystickOne;
            else
            {
                PlayerData[0].PlayerInput = CPU;
                if (CPUPlayerEnabled != false)  PlayerData[0].PlayerStatus = NewPieceDropping;
                else  PlayerData[0].PlayerStatus = GameOver;
            }

            if (input->JoystickDeviceTwo != NULL)  PlayerData[2].PlayerInput = JoystickTwo;
            else
            {
                PlayerData[2].PlayerInput = CPU;
                if (CPUPlayerEnabled != false)  PlayerData[2].PlayerStatus = NewPieceDropping;
                else  PlayerData[2].PlayerStatus = GameOver;
            }

            if (input->JoystickDeviceThree != NULL)  PlayerData[3].PlayerInput = JoystickThree;
            else
            {
                PlayerData[3].PlayerInput = CPU;
                if (CPUPlayerEnabled != false)  PlayerData[3].PlayerStatus = NewPieceDropping;
                else  PlayerData[3].PlayerStatus = GameOver;
            }
        }
        else if (PlayerData[1].PlayerInput == JoystickOne)
        {
            PlayerData[0].PlayerInput = Keyboard;

            if (input->JoystickDeviceTwo != NULL)  PlayerData[2].PlayerInput = JoystickTwo;
            else
            {
                PlayerData[2].PlayerInput = CPU;
                if (CPUPlayerEnabled != false)  PlayerData[2].PlayerStatus = NewPieceDropping;
                else  PlayerData[2].PlayerStatus = GameOver;
            }

            if (input->JoystickDeviceThree != NULL)  PlayerData[3].PlayerInput = JoystickThree;
            else
            {
                PlayerData[3].PlayerInput = CPU;
                if (CPUPlayerEnabled != false)  PlayerData[3].PlayerStatus = NewPieceDropping;
                else  PlayerData[3].PlayerStatus = GameOver;
            }

        }
        else if (PlayerData[1].PlayerInput == JoystickTwo)
        {
            PlayerData[0].PlayerInput = Keyboard;

            if (input->JoystickDeviceOne != NULL)  PlayerData[2].PlayerInput = JoystickOne;
            else
            {
                PlayerData[2].PlayerInput = CPU;
                if (CPUPlayerEnabled != false)  PlayerData[2].PlayerStatus = NewPieceDropping;
                else  PlayerData[2].PlayerStatus = GameOver;
            }

            if (input->JoystickDeviceThree != NULL)  PlayerData[3].PlayerInput = JoystickThree;
            else
            {
                PlayerData[3].PlayerInput = CPU;
                if (CPUPlayerEnabled != false)  PlayerData[3].PlayerStatus = NewPieceDropping;
                else  PlayerData[3].PlayerStatus = GameOver;
            }
        }
        else if (PlayerData[1].PlayerInput == JoystickThree)
        {
            PlayerData[0].PlayerInput = Keyboard;

            if (input->JoystickDeviceOne != NULL)  PlayerData[2].PlayerInput = JoystickOne;
            else
            {
                PlayerData[2].PlayerInput = CPU;
                if (CPUPlayerEnabled != false)  PlayerData[2].PlayerStatus = NewPieceDropping;
                else  PlayerData[2].PlayerStatus = GameOver;
            }

            if (input->JoystickDeviceTwo != NULL)  PlayerData[3].PlayerInput = JoystickTwo;
            else
            {
                PlayerData[3].PlayerInput = CPU;
                if (CPUPlayerEnabled != false)  PlayerData[3].PlayerStatus = NewPieceDropping;
                else  PlayerData[3].PlayerStatus = GameOver;
            }
        }
    }
    else
    {
        for (int player = 0; player < 4; player++)
        {
            PlayerData[player].PlayerInput = CPU;
            PlayerData[player].PlayerStatus = NewPieceDropping;
        }
    }

    if (input->DEBUG == true && GameMode != CrisisMode)
    {
        PlayerData[0].Score = 34652096;
        PlayerData[0].Level = 2015;
        PlayerData[0].Lines = 20153;
        PlayerData[1].Score = 36663807;
        PlayerData[1].Level = 2108;
        PlayerData[1].Lines = 21084;
        PlayerData[2].Score = 32647097;
        PlayerData[2].Level = 1905;
        PlayerData[2].Lines = 19052;
        PlayerData[3].Score = 33539964;
        PlayerData[3].Level = 2094;
        PlayerData[3].Lines = 20947;
    }

	Player = 0;

	PAUSEgame = false;

	if (GameMode == TimeAttack30Mode)  TimeAttackTimer = 6000;
	else if (GameMode == TimeAttack60Mode)  TimeAttackTimer = 12000;
	else if (GameMode == TimeAttack120Mode)  TimeAttackTimer = 24000;
    else  TimeAttackTimer = 0;

    CrisisModeTimer = 0;
    Crisis7BGMPlayed = false;

    CrisisWon = false;
    CrisisModeOnePlayerLeftPlayfieldCleared = false;

    GameDisplayChanged = true;

    if (NewGameGarbageHeight > 0)
    {
        for (int player = 0; player < NumberOfPlayers; player++)
        {
            for (int y = 23; y > 23-NewGameGarbageHeight; y--)
            {
                int boxTotal = 0;
                for (int x = 2; x < 12; x++)
                {
                    Uint32 box = rand()%8;
                    if (box > 0)  boxTotal++;

                    if (boxTotal < 10)
                    {
                        if (box != 0)  PlayerData[player].Playfield[x][y] = (int)box+10;
                        else  PlayerData[player].Playfield[x][y] = 0;
                    }
                    else  PlayerData[player].Playfield[x][y] = 0;
                }
           }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Logic::FlashCompletedLines(void)
{
int numberOfCompletedLines = 0;

	if (PlayerData[Player].FlashCompletedLinesTimer < 21)  PlayerData[Player].FlashCompletedLinesTimer++;

	for (int y = 5; y < 24; y++)
	{
		int boxTotal = 0;

		for (int x = 2; x < 12; x++)
		{
			if ( (PlayerData[Player].Playfield[x][y] > 9)&&(PlayerData[Player].Playfield[x][y] < 30) )
				boxTotal++;
		}

		if (boxTotal == 10)
		{
            numberOfCompletedLines++;

			if (PlayerData[Player].FlashCompletedLinesTimer % 2 == 0)
			{
				for (int xTwo = 2; xTwo < 12; xTwo++)
					PlayerData[Player].Playfield[xTwo][y] = PlayerData[Player].Playfield[xTwo][y] + 10;
			}
			else
			{
				for (int xTwo = 2; xTwo < 12; xTwo++)
					PlayerData[Player].Playfield[xTwo][y] = PlayerData[Player].Playfield[xTwo][y] - 10;
			}
		}
	}

	if (PlayerData[Player].FlashCompletedLinesTimer == 21)
	{
		PlayerData[Player].PlayerStatus = ClearingCompletedLines;

        if (CrisisModeOnePlayerLeftPlayfieldCleared == false)
        {
            for (int y = 5; y < 24; y++)
            {
                int boxTotal = 0;

                for (int x = 2; x < 12; x++)
                {
                    if ( (PlayerData[Player].Playfield[x][y] > 9)&&(PlayerData[Player].Playfield[x][y] < 30) )
                        boxTotal++;
                }

                if (boxTotal == 10 && numberOfCompletedLines > 1)
                {
                    for (int attackY = 1; attackY < 12; attackY++)
                        for (int attackX = 0; attackX < 10; attackX++)
                            PlayerData[Player].AttackLines[attackX][attackY-1] = PlayerData[Player].AttackLines[attackX][attackY];

                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 1] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 2] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 3] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 4] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY ] = 0;

                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 5] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY+1 ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 6] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY+1 ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 7] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY+1 ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 8] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY+1 ] = 0;

                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 9] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY+2 ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [10] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY+2 ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [11] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY+2 ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [12] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY+2 ] = 0;

                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [13] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY+3 ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [14] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY+3 ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [15] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY+3 ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [16] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY+3 ] = 0;

                    int attackX = 0;
                    for (int xThree = 2; xThree < 12; xThree++)
                    {
                        PlayerData[Player].AttackLines[attackX][11] = PlayerData[Player].Playfield[xThree][y];
                        attackX++;
                    }
                }

                AddPieceToPlayfieldMemory(Current);
            }
        }
	}

	GameDisplayChanged = true;
}

//-------------------------------------------------------------------------------------------------
void Logic::ClearCompletedLines(void)
{
bool thereWasACompletedLine = false;

	for (int y = 5; y < 24; y++)
	{
		int boxTotal = 0;

		for (int x = 2; x < 12; x++)
		{
			if ( (PlayerData[Player].Playfield[x][y] > 9)&&(PlayerData[Player].Playfield[x][y] < 20) )
				boxTotal++;
		}

		if (boxTotal == 10)
		{
			thereWasACompletedLine = true;

			if (PlayerData[Player].ClearCompletedLinesTimer < 40)  PlayerData[Player].ClearCompletedLinesTimer++;

			if (PlayerData[Player].ClearCompletedLinesTimer % 10 == 0)
			{
				for (int yTwo = y; yTwo > 5; yTwo--)
		            for (int xTwo = 2; xTwo < 12; xTwo++)
						PlayerData[Player].Playfield[xTwo][yTwo] = PlayerData[Player].Playfield[xTwo][yTwo-1];

                for (int xTwo = 2; xTwo < 12; xTwo++)
					PlayerData[Player].Playfield[xTwo][5] = 0;

				PlayerData[Player].Lines++;

                if (input->DEBUG == 1 && GameMode == CrisisMode)
                {
                    if (PlayerData[Player].Level > 0)  PlayerData[Player].Lines = (PlayerData[Player].Level+1) * 10;
                    else PlayerData[Player].Lines = 10;
                }

                if (PlayerData[Player].TwentyLineCounter > 0)  PlayerData[Player].TwentyLineCounter--;

				if (PlayerData[Player].Lines % 10 == 0)
				{
                    if (GameMode == CrisisMode)
                    {
                        Uint8 playersAlive = 0;
                        if (PlayerData[0].PlayerStatus != GameOver)  playersAlive++;
                        if (PlayerData[1].PlayerStatus != GameOver)  playersAlive++;
                        if (PlayerData[2].PlayerStatus != GameOver)  playersAlive++;
                        if (PlayerData[3].PlayerStatus != GameOver)  playersAlive++;

                        if (PlayerData[Player].Level < 9 && playersAlive == 1)
                        {
                            PlayerData[Player].Level++;

                            if (PlayerData[Player].Level == 7 && Crisis7BGMPlayed == false)
                            {
                                audio->PlayMusic(24, -1);
                                audio->PlayDigitalSoundFX(12, 0);
                                audio->PlayDigitalSoundFX(12, 0);
                                audio->PlayDigitalSoundFX(12, 0);
                                audio->PlayDigitalSoundFX(12, 0);
                                audio->PlayDigitalSoundFX(12, 0);
                                audio->PlayDigitalSoundFX(12, 0);
                                audio->PlayDigitalSoundFX(12, 0);
                                audio->PlayDigitalSoundFX(12, 0);
                                audio->PlayDigitalSoundFX(12, 0);
                                Crisis7BGMPlayed = true;
                            }

                            PlayerData[Player].TimeToDropPiece-=5;
                            audio->PlayDigitalSoundFX(8, 0);
                        }
                        else if (PlayerData[Player].Level > 8)
                        {
                            PlayerData[0].PlayerStatus = GameOver;
                            PlayerData[1].PlayerStatus = GameOver;
                            PlayerData[2].PlayerStatus = GameOver;
                            PlayerData[3].PlayerStatus = GameOver;

                            PlayerData[Player].Level++;

                            CrisisWon = true;
                        }
                    }
                    else
                    {
                        if (PlayerData[Player].Level < 10000)
                        {
                            PlayerData[Player].Level++;
                            if (PlayerData[Player].TimeToDropPiece > 3)  PlayerData[Player].TimeToDropPiece-=2;
                            audio->PlayDigitalSoundFX(8, 0);
                        }
                    }
				}

				audio->PlayDigitalSoundFX(6, 0);
				GameDisplayChanged = true;
			}
		}
	}

	if (thereWasACompletedLine == false)
	{
		SetupNewPiece();
		PlayerData[Player].PlayerStatus = NewPieceDropping;
	}
}

//-------------------------------------------------------------------------------------------------
void Logic::AddAnAttackLineToEnemiesPlayfield(void)
{
int TEMP_Player = Player;

    for (Player = 0; Player < NumberOfPlayers; Player++)
    {
        if (Player != TEMP_Player && PlayerData[Player].PlayerStatus != GameOver)
        {
            if (PlayerData[Player].PlayerStatus != FlashingCompletedLines && PlayerData[Player].PlayerStatus != ClearingCompletedLines)
            {
                for (int x = 2; x < 12; x++)
                {
                    if (PlayerData[Player].Playfield[x][5] > 10 && PlayerData[Player].Playfield[x][5] < 20)
                    {
                        PlayerData[Player].PlayerStatus = GameOver;
                        return;
                    }
                }

                if (PieceCollisionDown() == true)  MovePieceDown(true);

                if (PlayerData[Player].PlayerStatus != FlashingCompletedLines
                    && PlayerData[Player].PlayerStatus != ClearingCompletedLines)
                {
                    if (ThinkRussianTimer == 0)  audio->PlayDigitalSoundFX(10, 0);

                    for (int y = 5; y < 23; y++)
                    {
                        for (int x = 2; x < 12; x++)
                        {
                            PlayerData[Player].Playfield[x][y] = PlayerData[Player].Playfield[x][y+1];
                        }
                    }

                    for (int x = 2; x < 12; x++)
                        PlayerData[Player].Playfield[x][23] = 0;

                    int attackX = 0;
                    for (int x = 2; x < 12; x++)
                    {
                        PlayerData[Player].Playfield[x][23] = PlayerData[TEMP_Player].AttackLines[attackX][11];
                        attackX++;
                    }
                }
            }
            else
            {
                audio->PlayDigitalSoundFX(14,0);
                BlockAttackTransparency[Player] = 255;
            }
        }
    }

    for (int y = 11; y > 0; y--)
        for (int x = 0; x < 10; x++)
            PlayerData[TEMP_Player].AttackLines[x][y] = PlayerData[TEMP_Player].AttackLines[x][y-1];

	GameDisplayChanged = true;
    Player = TEMP_Player;
}

//-------------------------------------------------------------------------------------------------
bool Logic::AddAnIncompleteLineToPlayfieldCrisisMode(void)
{
    if (PlayerData[Player].PlayerStatus == FlashingCompletedLines || PlayerData[Player].PlayerStatus == ClearingCompletedLines)
    {
        return(false);
    }

    if (ThinkRussianTimer == 0)  audio->PlayDigitalSoundFX(10, 0);

    for (int x = 2; x < 12; x++)
    {
        if (PlayerData[Player].Playfield[x][5] > 10 && PlayerData[Player].Playfield[x][5] < 20)
        {
            PlayerData[Player].PlayerStatus = GameOver;
            return(true);
        }
    }

    if (PieceCollisionDown() == true)  MovePieceDown(true);

    for (int y = 5; y < 23; y++)
	    for (int x = 2; x < 12; x++)
            PlayerData[Player].Playfield[x][y] = PlayerData[Player].Playfield[x][y+1];

    for (int x = 2; x < 12; x++)
        PlayerData[Player].Playfield[x][23] = 0;

    int boxTotal = 0;
    for (int x = 2; x < 12; x++)
    {
        Uint32 box = rand()%8;
        if (box > 0)  boxTotal++;

        if (boxTotal < 10)
        {
            if (box != 0)  PlayerData[Player].Playfield[x][23] = (int)box+10;
            else  PlayerData[Player].Playfield[x][23] = 0;
        }
        else  PlayerData[Player].Playfield[x][23] = 0;
    }

    GameDisplayChanged = true;
    return(true);
}

//-------------------------------------------------------------------------------------------------
void Logic::RunTetriGameEngine(void)
{
    if (  (input->CheckKeyCodeName("[Spacebar]") == true && input->UserDefinedKeyPause == -1)
       || ( (input->UserDefinedKeyPause != -1) && (input->KeyOnKeyboardPressedByUser == input->UserDefinedKeyPause) )  )
	{
		if (PAUSEgame == false)
        {
            PAUSEgame = true;
            Mix_PauseMusic();
        }
		else
        {
            PAUSEgame = false;
            Mix_ResumeMusic();
        }

        input->DelayAllUserInput = 20;

		audio->PlayDigitalSoundFX(0, 0);

        GameDisplayChanged = true;
	}

    if (ThinkRussianTimer > 0)  ThinkRussianTimer--;
    if (ThinkRussianTimer == 1)
    {
        audio->PlayMusic(1+SelectedMusicTrack, -1);
        Mix_ResumeMusic();
        ThinkRussianTimer = 0;
    }

	if (PAUSEgame == false)
	{
		for (Player = 0; Player < NumberOfPlayers; Player++)
		{
            if (PlayerData[Player].PlayerStatus != GameOver)
            {
                if (input->DEBUG != 1 && input->DEBUG != 4)  PlayerData[Player].PieceDropTimer++;

                if (input->JoystickDirectionVertical[PlayerData[Player].PlayerInput] == DOWN)
                {
                    PlayerData[Player].PieceDropTimer = 1+PlayerData[Player].TimeToDropPiece;
                }
                else  PlayerData[Player].DropBonus = 0;

                if (PlayerData[Player].PlayerStatus == NewPieceDropping)
                {
                    PlayerData[Player].UPActionTaken = true;
                    PlayerData[Player].DropBonus = 0;

                    if (PlayerData[Player].PiecePlayfieldY < PieceDropStartHeight[ PlayerData[Player].Piece ])
                        MovePieceDown(true);
                    else
                    {
                        if (DisplayNextPiece == true)  AddPieceToPlayfieldMemory(Next);
                        PlayerData[Player].PlayerStatus = PieceFalling;
                    }

                    GameDisplayChanged = true;
                }
                else if (PlayerData[Player].PlayerStatus == PieceFalling)
                {
                    if (GameMode == TwentyLineChallengeMode && PlayerData[Player].TwentyLineCounter == 0)
                    {
                        PlayerData[Player].PlayerStatus = GameOver;
                    }

                    if (PlayerData[Player].PieceDropTimer > PlayerData[Player].TimeToDropPiece)
                    {
                        MovePieceDown(false);
                    }

                    if (PlayerData[Player].PlayerInput == CPU)
                    {
                        ComputeComputerPlayerMove();
                    }
                    else
                    {
                        if (PressingUPAction == Rotate)
                        {
                            if (input->JoystickDirectionVertical[PlayerData[Player].PlayerInput] == UP)
                            {
                                if (PlayerData[Player].RotateDirection == 0)
                                {
                                    if (PlayerData[Player].UPActionTaken == false)  RotatePieceCounterClockwise();
                                    PlayerData[Player].UPActionTaken = true;
                                }
                                else if (PlayerData[Player].RotateDirection == 1)
                                {
                                    if (PlayerData[Player].UPActionTaken == false)  RotatePieceClockwise();
                                    PlayerData[Player].UPActionTaken = true;
                                }
                            }
                            else  PlayerData[Player].UPActionTaken = false;
                        }
                        else if (PressingUPAction == Fall)
                        {
                            if (input->JoystickDirectionVertical[PlayerData[Player].PlayerInput] == UP)
                            {
                                if (PlayerData[Player].UPActionTaken == false)
                                {
                                    MovePieceDownFast();
                                }

                                PlayerData[Player].UPActionTaken = true;
                            }
                            else  PlayerData[Player].UPActionTaken = false;
                        }
                        else if (PressingUPAction == DropAndDrag)
                        {
                            if (input->JoystickDirectionVertical[PlayerData[Player].PlayerInput] == UP)
                            {
                                if (PlayerData[Player].UPActionTaken == false)
                                {
                                    MovePieceDownFastDropAndDrag();
                                }

                                PlayerData[Player].UPActionTaken = true;
                            }
                            else  PlayerData[Player].UPActionTaken = false;
                        }

                        if (PlayerData[Player].PieceDropTimer > PlayerData[Player].TimeToDropPiece)
                        {
                            if (input->JoystickDirectionVertical[PlayerData[Player].PlayerInput] != DOWN)
                            {
                                if (ThinkRussianTimer == 0) audio->PlayDigitalSoundFX(2, 0);
                                PlayerData[Player].DropBonus = 0;
                            }
                            else  PlayerData[Player].DropBonus++;
                        }

                        if (input->JoystickButtonOne[PlayerData[Player].PlayerInput] == ON)
                        {
                            if (PlayerData[Player].PieceRotated1 == false)
                            {
                                RotatePieceCounterClockwise();
                                PlayerData[Player].PieceRotated1 = true;
                            }
                        }
                        else PlayerData[Player].PieceRotated1 = false;

                        if (input->JoystickButtonTwo[PlayerData[Player].PlayerInput] == ON)
                        {
                            if (PlayerData[Player].PieceRotated2 == false)
                            {
                                RotatePieceClockwise();
                                PlayerData[Player].PieceRotated2 = true;
                            }
                        }
                        else PlayerData[Player].PieceRotated2 = false;

                        if (input->JoystickDirectionHorizonal[PlayerData[Player].PlayerInput] == LEFT)  MovePieceLeft();
                        else if (input->JoystickDirectionHorizonal[PlayerData[Player].PlayerInput] == RIGHT)  MovePieceRight();
                        else  PlayerData[Player].PieceMovementDelay = 0;
                    }

                    if (GameMode == CrisisMode)
                    {
                        for (int x = 0; x < 10; x++)
                        {
                            if (PlayerData[Player].AttackLines[x][11] > 0)
                            {
                                x = 100;
                                AddAnAttackLineToEnemiesPlayfield();
                            }
                        }

                        if ( (Player == 0 && PlayerData[1].PlayerStatus == GameOver && PlayerData[2].PlayerStatus == GameOver && PlayerData[3].PlayerStatus == GameOver)
                           ||(Player == 1 && PlayerData[0].PlayerStatus == GameOver && PlayerData[2].PlayerStatus == GameOver && PlayerData[3].PlayerStatus == GameOver)
                           ||(Player == 2 && PlayerData[0].PlayerStatus == GameOver && PlayerData[1].PlayerStatus == GameOver && PlayerData[3].PlayerStatus == GameOver)
                           ||(Player == 3 && PlayerData[0].PlayerStatus == GameOver && PlayerData[1].PlayerStatus == GameOver && PlayerData[2].PlayerStatus == GameOver) )
                        {
                            if (CrisisModeOnePlayerLeftPlayfieldCleared == false
                                && PlayersCanJoin == false && PlayerData[Player].PlayerStatus == PieceFalling)
                            {
                                DeletePieceFromPlayfieldMemory(Current);
                                DeletePieceFromPlayfieldMemory(DropShadow);

                            	PlayerData[Player].PlayerStatus = ClearingPlayfield;
                            }

                            if (CrisisModeTimer < 300)  CrisisModeTimer++;
                            else
                            {
                                if (AddAnIncompleteLineToPlayfieldCrisisMode() == true)  CrisisModeTimer = 0;
                            }
                        }
                    }
                }
                else if (PlayerData[Player].PlayerStatus == FlashingCompletedLines)  FlashCompletedLines();
                else if (PlayerData[Player].PlayerStatus == ClearingCompletedLines)  ClearCompletedLines();
                else if (PlayerData[Player].PlayerStatus == ClearingPlayfield)
                {
                    if ( CrisisModeClearPlayfield() == false )
                    {
                        PlayerData[Player].PlayerStatus = PieceFalling;
                        CrisisModeOnePlayerLeftPlayfieldCleared = true;
                    }
                }

                if (PlayerData[Player].PieceDropTimer > PlayerData[Player].TimeToDropPiece)  PlayerData[Player].PieceDropTimer = 0;

                if (GameMode == TimeAttack30Mode || GameMode == TimeAttack60Mode || GameMode == TimeAttack120Mode)
                {
                    TimeAttackTimer--;

                    if (TimeAttackTimer == 0)
                    {
                        PlayerData[0].PlayerStatus = GameOver;
                        PlayerData[1].PlayerStatus = GameOver;
                        PlayerData[2].PlayerStatus = GameOver;
                        PlayerData[3].PlayerStatus = GameOver;
                    }
                }
            }

            if (GameMode == CrisisMode)
            {
                Uint8 playersAlive = 0;
                if (PlayerData[0].PlayerStatus != GameOver)  playersAlive++;
                if (PlayerData[1].PlayerStatus != GameOver)  playersAlive++;
                if (PlayerData[2].PlayerStatus != GameOver)  playersAlive++;
                if (PlayerData[3].PlayerStatus != GameOver)  playersAlive++;

                if (playersAlive > 1)  PlayerData[Player].Score = 0;
            }

            if (BlockAttackTransparency[Player] > 0.0f)  BlockAttackTransparency[Player]-=5;
		}
	}
}

//-------------------------------------------------------------------------------------------------
bool Logic::CrisisModeClearPlayfield(void)
{
bool returnValue = false;

	for (int y = 23; y > 5; y--)
	{
		for (int x = 2; x < 12; x++)
		{
            PlayerData[Player].Playfield[x][y] = PlayerData[Player].Playfield[x][y-1];
		}
    }

    for (int x = 2; x < 12; x++)
        PlayerData[Player].Playfield[x][5] = 0;

	for (int y = 5; y < 24; y++)
	{
		for (int x = 2; x < 12; x++)
		{
            if (PlayerData[Player].Playfield[x][y] != 0)  returnValue = true;
		}
    }

    if (returnValue == true)  GameDisplayChanged = true;

    return(returnValue);
}

//-------------------------------------------------------------------------------------------------
void Logic::ComputeComputerPlayerMove(void)
{
    if (PlayerData[Player].PlayerStatus != PieceFalling)  return;

    if (PlayerData[Player].BestMoveCalculated == false)
    {
        DeletePieceFromPlayfieldMemory(Current);

        for (int indexX = (PlayerData[Player].PlayfieldStartX-2); indexX < (PlayerData[Player].PlayfieldEndX-1); indexX+=1)
        {
            for (int indexRot = 1; indexRot < 5; indexRot+=1)
            {
                PlayerData[Player].MoveOneBlockCavernHoles[indexX][indexRot] = 0;
                PlayerData[Player].MoveCompletedLines[indexX][indexRot] = 0;
                PlayerData[Player].MovePieceHeight[indexX][indexRot] = 0;
                PlayerData[Player].MovePlayfieldBoxEdges[indexX][indexRot] = 999999999;
                PlayerData[Player].MoveTrappedHoles[indexX][indexRot] = 0;
            }
        }

        for (int pieceTestX = (PlayerData[Player].PlayfieldStartX-2); pieceTestX < (PlayerData[Player].PlayfieldEndX-1); pieceTestX+=1)
        {
            for (int rotationTest = 1; rotationTest <= MaxRotationArray[ PlayerData[Player].Piece ]; rotationTest+=1)
            {
                int TEMP_PieceRotation;
                TEMP_PieceRotation = PlayerData[Player].PieceRotation;
                int TEMP_PiecePlayfieldX;
                TEMP_PiecePlayfieldX = PlayerData[Player].PiecePlayfieldX;
                int TEMP_PiecePlayfieldY;
                TEMP_PiecePlayfieldY = PlayerData[Player].PiecePlayfieldY;

                PlayerData[Player].PiecePlayfieldX = pieceTestX;
                PlayerData[Player].PieceRotation = rotationTest;

                PlayerData[Player].MovePieceHeight[pieceTestX][rotationTest] = 0;
                if (PieceCollision() == CollisionNotTrue)
                {
                    int posY;
                    for (posY = PlayerData[Player].PiecePlayfieldY; posY < 23; posY+=1)
                    {
                        PlayerData[Player].PiecePlayfieldY  = posY;
                        if (PieceCollision() != CollisionNotTrue)
                        {
                            PlayerData[Player].PiecePlayfieldY = posY-1;
                            PlayerData[Player].MovePieceHeight[pieceTestX][rotationTest] = PlayerData[Player].PiecePlayfieldY;
                            posY = 100;
                        }
                    }

                    AddPieceToPlayfieldMemory(Current);

                    PlayerData[Player].MoveTrappedHoles[pieceTestX][rotationTest] = 0;
                    for (int posX = PlayerData[Player].PlayfieldStartX; posX < PlayerData[Player].PlayfieldEndX; posX+=1)
                    {
                        int numberOfEmpties;
                        numberOfEmpties = 0;
                        for (int posY = 23; posY > 4; posY-=1)
                        {
                            if (PlayerData[Player].Playfield[posX][posY] == 0)
                            {
                                numberOfEmpties+=1;
                            }
                            else if (PlayerData[Player].Playfield[posX][posY] > 10 && PlayerData[Player].Playfield[posX][posY] < 20)
                            {
                                PlayerData[Player].MoveTrappedHoles[pieceTestX][rotationTest]+=numberOfEmpties;
                                numberOfEmpties = 0;
                            }
                        }
                    }

                    PlayerData[Player].MoveCompletedLines[pieceTestX][rotationTest] = 0;
                    PlayerData[Player].MovePlayfieldBoxEdges[pieceTestX][rotationTest] = 0;
                    for (int posY = 5; posY < 25; posY+=1)
                    {
                        int boxTotal;
                        boxTotal = 0;
                        for ( int posX = (PlayerData[Player].PlayfieldStartX-1); posX < PlayerData[Player].PlayfieldEndX; posX+=1 )
                        {
                            if ( (PlayerData[Player].Playfield[posX][posY] > 10 && PlayerData[Player].Playfield[posX][posY] < 20)
                                || PlayerData[Player].Playfield[posX][posY] == 255 )
                            {
                                if (PlayerData[Player].Playfield[posX][posY] != 255)  boxTotal+=1;

                                if (PlayerData[Player].Playfield[posX][(posY-1)] == 0)
                                    PlayerData[Player].MovePlayfieldBoxEdges[pieceTestX][rotationTest]+=1;

                                if (PlayerData[Player].Playfield[posX][(posY+1)] == 0)
                                    PlayerData[Player].MovePlayfieldBoxEdges[pieceTestX][rotationTest]+=1;

                                if (PlayerData[Player].Playfield[(posX-1)][posY] == 0)
                                    PlayerData[Player].MovePlayfieldBoxEdges[pieceTestX][rotationTest]+=1;

                                if (PlayerData[Player].Playfield[(posX+1)][posY] == 0)
                                    PlayerData[Player].MovePlayfieldBoxEdges[pieceTestX][rotationTest]+=1;
                            }
                        }

                        if (boxTotal == 10)  PlayerData[Player].MoveCompletedLines[pieceTestX][rotationTest]+=1;
                    }

                    PlayerData[Player].MoveOneBlockCavernHoles[pieceTestX][rotationTest] = 0;
                    for (int posY = 5; posY < 24; posY+=1)
                    {
                        for (int posX = PlayerData[Player].PlayfieldStartX; posX < PlayerData[Player].PlayfieldEndX; posX+=1)
                        {
                            if (PlayerData[Player].Playfield[posX][posY] == 0
                            && PlayerData[Player].Playfield[(posX-1)][posY] != 0 && PlayerData[Player].Playfield[(posX+1)][posY] != 0)
                                PlayerData[Player].MoveOneBlockCavernHoles[pieceTestX][rotationTest]+=1;
                        }
                    }

                    DeletePieceFromPlayfieldMemory(Current);
                }
                else
                {
                    PlayerData[Player].MoveTrappedHoles[pieceTestX][rotationTest] = 99999;
                }

                PlayerData[Player].PieceRotation = TEMP_PieceRotation;
                PlayerData[Player].PiecePlayfieldX = TEMP_PiecePlayfieldX;
                PlayerData[Player].PiecePlayfieldY = TEMP_PiecePlayfieldY;
            }
        }

        PlayerData[Player].BestMoveX = -1;
        PlayerData[Player].BestRotation = -1;
        int bestValue;
        bestValue = 999999;
        for (int posX = (PlayerData[Player].PlayfieldStartX-1); posX < (PlayerData[Player].PlayfieldEndX-1); posX+=1)
        {
            for (int rot = 1; rot <= MaxRotationArray[ PlayerData[Player].Piece ]; rot+=1)
            {
                PlayerData[Player].MovePieceHeight[posX][rot]+=PlayerData[Player].MoveCompletedLines[posX][rot];

                float testValue;

                //--["Gift Of Sight" Tetris(R) A.I. Algorithm ~24,000+]---------------------------------------
                testValue = ( (3*PlayerData[Player].MoveTrappedHoles[posX][rot])
                            +(1*PlayerData[Player].MoveOneBlockCavernHoles[posX][rot])
                            +(1*PlayerData[Player].MovePlayfieldBoxEdges[posX][rot])
                            -(1*PlayerData[Player].MovePieceHeight[posX][rot]) );
                //---------------------------------------["Gift Of Sight" Tetris(R) A.I. Algorithm ~24,000+]--

                if (PlayerData[Player].MoveCompletedLines[posX][rot] > 1)
                    testValue = ( 0 - (PlayerData[Player].MoveCompletedLines[posX][rot]*100) );

                if (testValue <= bestValue)
                {
                    bestValue = testValue;
                    PlayerData[Player].BestMoveX = posX;
                    PlayerData[Player].BestRotation = rot;
                }
            }
        }

        PlayerData[Player].BestMoveCalculated = true;
    }

    if (PlayerData[Player].MovedToBestMove == false && PlayerData[Player].BestMoveX != -1 && PlayerData[Player].BestRotation != -1)
    {
        if (PlayerData[Player].PieceRotation < PlayerData[Player].BestRotation)  RotatePieceClockwise();
        else if (PlayerData[Player].PieceRotation > PlayerData[Player].BestRotation)  RotatePieceCounterClockwise();

        if (PlayerData[Player].BestMoveX < PlayerData[Player].PiecePlayfieldX)
            MovePieceLeft();
        else if (PlayerData[Player].BestMoveX > PlayerData[Player].PiecePlayfieldX)
            MovePieceRight();
        else if (PlayerData[Player].PieceRotation == PlayerData[Player].BestRotation)
        {
            if (CPUPlayerEnabled != 4)  MovePieceDown(true);
            PlayerData[Player].MovedToBestMove = true;
        }
    }
    else
    {
        if (CPUPlayerEnabled != 4)  MovePieceDown(false);
        else  MovePieceDownFast();
    }
}
