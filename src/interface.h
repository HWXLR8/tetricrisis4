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
#ifndef INTERFACE
#define INTERFACE

class Interface
{
public:

	Interface(void);
	virtual ~Interface(void);

    #define NumberOfButtons     7
    struct ButtonGUI
    {
        Uint16 SpriteIndex;
        Sint8 ScreenIndex;
        float Scale;
        int ScreenY;
        Sint16 AnimationTimer;
    } ButtonGUIs[NumberOfButtons];

    Uint8 ButtonSelectedByKeyboard;
    Uint8 ButtonSelectedByPlayer;

    #define NumberOfArrowSets     10
    struct ArrowSetGUI
    {
        float LeftArrowScreenIndex;
        float LeftArrowScale;
        float RightArrowScale;
        int ScreenY;
        Sint16 AnimationTimer;
    } ArrowSetGUIs[NumberOfArrowSets];

    Uint8 ArrowSetSelectedByKeyboard;
    float ArrowSetArrowSelectedByPlayer;

    void CreateButton(Uint16 SpriteIndex, Uint8 ScreenIndex, int ScreenY);
    void DisplayAllButtonsOntoScreenBuffer(void);
    void ProcessAllButtons(void);
    void DestroyAllButtons(void);

    void CreateArrowSet(float ScreenIndex, int ScreenY);
    void DisplayAllArrowSetsOntoScreenBuffer(void);
    void ProcessAllArrowSets(void);
    void DestroyAllArrowSets(void);
};

#endif

