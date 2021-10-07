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

#include "input.h"

#include "visuals.h"
#include "screens.h"
#include "audio.h"
#include "logic.h"

extern Visuals* visuals;
extern Screens* screens;
extern Audio* audio;
extern Logic* logic;

//-------------------------------------------------------------------------------------------------
Input::Input(void)
{
    DEBUG = false;

    EXIT_Game = false;

    DelayAllUserInput = 0;

    KeyOnKeyboardPressedByUser = -1;

    MouseButtonPressed[0] = false;
    MouseButtonPressed[1] = false;

    JoystickDeviceOne = NULL;
    JoystickDeviceTwo = NULL;
    JoystickDeviceThree = NULL;

    if (SDL_NumJoysticks() == 0)
    {
        printf("No USB joysticks are plugged in.\n");
    }
    else
    {
        if (SDL_NumJoysticks()>0)
        {
            JoystickDeviceOne = SDL_JoystickOpen(0);
            NumberOfJoyButtons[0] = SDL_JoystickNumButtons(JoystickDeviceOne);
            NumberOfJoyAxises[0] = SDL_JoystickNumAxes(JoystickDeviceOne);
            printf("SDL2 Joystick 0 initialized.\n");
        }

        if (SDL_NumJoysticks()>1)
        {
            JoystickDeviceTwo = SDL_JoystickOpen(1);
            NumberOfJoyButtons[1] = SDL_JoystickNumButtons(JoystickDeviceTwo);
            NumberOfJoyAxises[1] = SDL_JoystickNumAxes(JoystickDeviceTwo);
            printf("SDL2 Joystick 1 initialized.\n");
        }

        if (SDL_NumJoysticks()>2)
        {
            JoystickDeviceThree = SDL_JoystickOpen(2);
            NumberOfJoyButtons[2] = SDL_JoystickNumButtons(JoystickDeviceThree);
            NumberOfJoyAxises[2] = SDL_JoystickNumAxes(JoystickDeviceThree);
            printf("SDL2 Joystick 2 initialized.\n");
        }
    }

    for (int joy = 0; joy < 3; joy++)
    {
        JoyUP[joy] = Axis1;
        JoyDOWN[joy] = Axis1;
        JoyLEFT[joy] = Axis0;
        JoyRIGHT[joy] = Axis0;
        JoyButton1[joy] = Button0;
        JoyButton2[joy] = Button1;
    }

    JoystickSetupProcess = JoySetupNotStarted;

    for (int index = 0; index < 5; index++)
    {
        JoystickDirectionHorizonal[index] = CENTER;
        JoystickDirectionVertical[index] = CENTER;
        JoystickButtonOne[index] = OFF;
        JoystickButtonTwo[index] = OFF;
        JoystickButton1Pressed[index] = false;
        JoystickButton2Pressed[index] = false;
    }

    KeyboardSetupProcess = KeyboardSetupNotStarted;
    UserDefinedKeyButtonOne = -1;
    UserDefinedKeyButtonTwo = -1;
    UserDefinedKeyUP = -1;
    UserDefinedKeyRIGHT = -1;
    UserDefinedKeyDOWN = -1;
    UserDefinedKeyLEFT = -1;
    UserDefinedKeyPause = -1;

    DelayAllUserInput = -1;
}

//-------------------------------------------------------------------------------------------------
Input::~Input(void)
{
    if (JoystickDeviceOne != NULL)  SDL_JoystickClose(JoystickDeviceOne);
    if (JoystickDeviceTwo != NULL)  SDL_JoystickClose(JoystickDeviceTwo);
    if (JoystickDeviceThree != NULL)  SDL_JoystickClose(JoystickDeviceThree);
}

//-------------------------------------------------------------------------------------------------
int Input::QueryJoysticksForAction(Uint8 joy)
{
int returnValue = -1;

	if (DelayAllUserInput > 0)  return(-1);

	if (joy == 0 && JoystickDeviceOne != NULL)//SDL_JoystickOpened(0))
	{
		SDL_JoystickUpdate();

        for (int index = 0; index < NumberOfJoyAxises[joy]; index++)
        {
            Sint16 joyAxis = 0;
            joyAxis = SDL_JoystickGetAxis(JoystickDeviceOne, index);
            if (joyAxis < -16383 || joyAxis > 16383)
            {
                returnValue = Axis0+index;
                return(returnValue);
            }
        }

        for (int index = 0; index < NumberOfJoyButtons[joy]; index++)
        {
            if ( SDL_JoystickGetButton(JoystickDeviceOne, index) )
            {
                returnValue = Button0+index;
                return(returnValue);
            }
        }
    }

	if (joy == 1 && JoystickDeviceTwo != NULL)//SDL_JoystickOpened(1))
	{
		SDL_JoystickUpdate();

        for (int index = 0; index < NumberOfJoyAxises[joy]; index++)
        {
            Sint16 joyAxis = 0;
            joyAxis = SDL_JoystickGetAxis(JoystickDeviceTwo, index);
            if (joyAxis < -16383 || joyAxis > 16383)
            {
                returnValue = Axis0+index;
                return(returnValue);
            }
        }

        for (int index = 0; index < NumberOfJoyButtons[joy]; index++)
        {
            if ( SDL_JoystickGetButton(JoystickDeviceTwo, index) )
            {
                returnValue = Button0+index;
                return(returnValue);
            }
        }
    }

	if (joy == 2 && JoystickDeviceThree != NULL)//SDL_JoystickOpened(2))
	{
		SDL_JoystickUpdate();

        for (int index = 0; index < NumberOfJoyAxises[joy]; index++)
        {
            Sint16 joyAxis = 0;
            joyAxis = SDL_JoystickGetAxis(JoystickDeviceThree, index);
            if (joyAxis < -16383 || joyAxis > 16383)
            {
                returnValue = Axis0+index;
                return(returnValue);
            }
        }

        for (int index = 0; index < NumberOfJoyButtons[joy]; index++)
        {
            if ( SDL_JoystickGetButton(JoystickDeviceThree, index) )
            {
                returnValue = Button0+index;
                return(returnValue);
            }
        }
    }

    return(returnValue);
}

//-------------------------------------------------------------------------------------------------
void Input::ConvertScancodeToNameAndChar(int Scancode)
{
    switch (Scancode)
    {
        case SDL_SCANCODE_1:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[!]"); strcpy(KeycodeChar, "!"); }
            else  { strcpy(KeycodeName, "[1]"); strcpy(KeycodeChar, "1"); }
            return;
            break;

        case SDL_SCANCODE_2:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[@]"); strcpy(KeycodeChar, "@"); }
            else  { strcpy(KeycodeName, "[2]"); strcpy(KeycodeChar, "2"); }
            return;
            break;

        case SDL_SCANCODE_3:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[#]"); strcpy(KeycodeChar, "#"); }
            else  { strcpy(KeycodeName, "[3]"); strcpy(KeycodeChar, "3"); }
            return;
            break;

        case SDL_SCANCODE_4:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[$]"); strcpy(KeycodeChar, "$"); }
            else  { strcpy(KeycodeName, "[4]"); strcpy(KeycodeChar, "4"); }
            return;
            break;

        case SDL_SCANCODE_5:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[%]"); strcpy(KeycodeChar, "%"); }
            else  { strcpy(KeycodeName, "[5]"); strcpy(KeycodeChar, "5"); }
            return;
            break;

        case SDL_SCANCODE_6:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[^]"); strcpy(KeycodeChar, "^"); }
            else  { strcpy(KeycodeName, "[6]"); strcpy(KeycodeChar, "6"); }
            return;
            break;

        case SDL_SCANCODE_7:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[&]"); strcpy(KeycodeChar, "&"); }
            else  { strcpy(KeycodeName, "[7]"); strcpy(KeycodeChar, "7"); }
            return;
            break;

        case SDL_SCANCODE_8:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[*]"); strcpy(KeycodeChar, "*"); }
            else  { strcpy(KeycodeName, "[8]"); strcpy(KeycodeChar, "8"); }
            return;
            break;

        case SDL_SCANCODE_9:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[(]"); strcpy(KeycodeChar, "("); }
            else  { strcpy(KeycodeName, "[9]"); strcpy(KeycodeChar, "9"); }
            return;
            break;

        case SDL_SCANCODE_0:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[)]"); strcpy(KeycodeChar, ")"); }
            else  { strcpy(KeycodeName, "[0]"); strcpy(KeycodeChar, "0"); }
            return;
            break;

        case SDL_SCANCODE_MINUS:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[_]"); strcpy(KeycodeChar, "_"); }
            else  { strcpy(KeycodeName, "[-]"); strcpy(KeycodeChar, "-"); }
            return;
            break;

        case SDL_SCANCODE_EQUALS:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[+]"); strcpy(KeycodeChar, "+"); }
            else  { strcpy(KeycodeName, "[=]"); strcpy(KeycodeChar, "="); }
            return;
            break;

        case SDL_SCANCODE_BACKSPACE:
            { strcpy(KeycodeName, "[Backspace]"); strcpy(KeycodeChar, ""); }
            return;
            break;

        case SDL_SCANCODE_LEFTBRACKET:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[{]"); strcpy(KeycodeChar, "{"); }
            else  { strcpy(KeycodeName, "[[]"); strcpy(KeycodeChar, "["); }
            return;
            break;

        case SDL_SCANCODE_RIGHTBRACKET:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[}]"); strcpy(KeycodeChar, "}"); }
            else  { strcpy(KeycodeName, "[]]"); strcpy(KeycodeChar, "]"); }
            return;
            break;

        case SDL_SCANCODE_BACKSLASH:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[|]"); strcpy(KeycodeChar, "|"); }
            else
            {
                char *backSlash = new char[3];
                sprintf(backSlash, "%c", 0x5C);

                strcpy(KeycodeName, "[");
                strcat(KeycodeName, backSlash);
                strcat(KeycodeName, "]");

                strcpy(KeycodeChar, backSlash);
            }
            return;
            break;

        case SDL_SCANCODE_SEMICOLON:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[:]"); strcpy(KeycodeChar, ":"); }
            else  { strcpy(KeycodeName, "[;]"); strcpy(KeycodeChar, ";"); }
            return;
            break;

        case SDL_SCANCODE_COMMA:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[<]"); strcpy(KeycodeChar, "<"); }
            else  { strcpy(KeycodeName, "[,]"); strcpy(KeycodeChar, ","); }
            return;
            break;

        case SDL_SCANCODE_PERIOD:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[>]"); strcpy(KeycodeChar, ">"); }
            else  { strcpy(KeycodeName, "[.]"); strcpy(KeycodeChar, "."); }
            return;
            break;

        case SDL_SCANCODE_SLASH:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[?]"); strcpy(KeycodeChar, "?"); }
            else  { strcpy(KeycodeName, "[/]"); strcpy(KeycodeChar, "/"); }
            return;
            break;


        case SDL_SCANCODE_UP:
            strcpy(KeycodeName, "[Arrow Up]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_RIGHT:
            strcpy(KeycodeName, "[Arrow Right]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_DOWN:
            strcpy(KeycodeName, "[Arrow Down]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_LEFT:
            strcpy(KeycodeName, "[Arrow Left]");
            strcpy(KeycodeChar, " ");
            return;
            break;


        case SDL_SCANCODE_TAB:
            strcpy(KeycodeName, "[Tab]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_INSERT:
            strcpy(KeycodeName, "[Insert]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_HOME:
            strcpy(KeycodeName, "[Home]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_PAGEUP:
            strcpy(KeycodeName, "[Page UP]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_DELETE:
            strcpy(KeycodeName, "[Delete]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_END:
            strcpy(KeycodeName, "[End]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_PAGEDOWN:
            strcpy(KeycodeName, "[Page Down]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_ESCAPE:
            strcpy(KeycodeName, "[Esc]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_PAUSE:
            strcpy(KeycodeName, "[Pause]");
            strcpy(KeycodeChar, " ");
            return;
            break;


        case SDL_SCANCODE_A:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[A]"); strcpy(KeycodeChar, "A"); }
            else  { strcpy(KeycodeName, "[a]"); strcpy(KeycodeChar, "a"); }
            return;
            break;

        case SDL_SCANCODE_B:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[B]"); strcpy(KeycodeChar, "B"); }
            else  { strcpy(KeycodeName, "[b]"); strcpy(KeycodeChar, "b"); }
            return;
            break;

        case SDL_SCANCODE_C:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[C]"); strcpy(KeycodeChar, "C"); }
            else  { strcpy(KeycodeName, "[c]"); strcpy(KeycodeChar, "c"); }
            return;
            break;

        case SDL_SCANCODE_D:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[D]"); strcpy(KeycodeChar, "D"); }
            else  { strcpy(KeycodeName, "[d]"); strcpy(KeycodeChar, "d"); }
            return;
            break;

        case SDL_SCANCODE_E:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[E]"); strcpy(KeycodeChar, "E"); }
            else  { strcpy(KeycodeName, "[e]"); strcpy(KeycodeChar, "e"); }
            return;
            break;

        case SDL_SCANCODE_F:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[F]"); strcpy(KeycodeChar, "F"); }
            else  { strcpy(KeycodeName, "[f]"); strcpy(KeycodeChar, "f"); }
            return;
            break;

        case SDL_SCANCODE_G:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[G]"); strcpy(KeycodeChar, "G"); }
            else  { strcpy(KeycodeName, "[g]"); strcpy(KeycodeChar, "g"); }
            return;
            break;

        case SDL_SCANCODE_H:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[H]"); strcpy(KeycodeChar, "H"); }
            else  { strcpy(KeycodeName, "[h]"); strcpy(KeycodeChar, "h"); }
            return;
            break;

        case SDL_SCANCODE_I:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[I]"); strcpy(KeycodeChar, "I"); }
            else  { strcpy(KeycodeName, "[i]"); strcpy(KeycodeChar, "i"); }
            return;
            break;

        case SDL_SCANCODE_J:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[J]"); strcpy(KeycodeChar, "J"); }
            else  { strcpy(KeycodeName, "[j]"); strcpy(KeycodeChar, "j"); }
            return;
            break;

        case SDL_SCANCODE_K:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[K]"); strcpy(KeycodeChar, "K"); }
            else  { strcpy(KeycodeName, "[k]"); strcpy(KeycodeChar, "k"); }
            return;
            break;

        case SDL_SCANCODE_L:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[L]"); strcpy(KeycodeChar, "L"); }
            else  { strcpy(KeycodeName, "[l]"); strcpy(KeycodeChar, "l"); }
            return;
            break;

        case SDL_SCANCODE_M:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[M]"); strcpy(KeycodeChar, "M"); }
            else  { strcpy(KeycodeName, "[m]"); strcpy(KeycodeChar, "m"); }
            return;
            break;

        case SDL_SCANCODE_N:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[N]"); strcpy(KeycodeChar, "N"); }
            else  { strcpy(KeycodeName, "[n]"); strcpy(KeycodeChar, "n"); }
            return;
            break;

        case SDL_SCANCODE_O:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[O]"); strcpy(KeycodeChar, "O"); }
            else  { strcpy(KeycodeName, "[o]"); strcpy(KeycodeChar, "o"); }
            return;
            break;

        case SDL_SCANCODE_P:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[P]"); strcpy(KeycodeChar, "P"); }
            else  { strcpy(KeycodeName, "[p]"); strcpy(KeycodeChar, "p"); }
            return;
            break;

        case SDL_SCANCODE_Q:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[Q]"); strcpy(KeycodeChar, "Q"); }
            else  { strcpy(KeycodeName, "[q]"); strcpy(KeycodeChar, "q"); }
            return;
            break;

        case SDL_SCANCODE_R:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[R]"); strcpy(KeycodeChar, "R"); }
            else  { strcpy(KeycodeName, "[r]"); strcpy(KeycodeChar, "r"); }
            return;
            break;

        case SDL_SCANCODE_S:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[S]"); strcpy(KeycodeChar, "S"); }
            else  { strcpy(KeycodeName, "[s]"); strcpy(KeycodeChar, "s"); }
            return;
            break;

        case SDL_SCANCODE_T:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[T]"); strcpy(KeycodeChar, "T"); }
            else  { strcpy(KeycodeName, "[t]"); strcpy(KeycodeChar, "t"); }
            return;
            break;

        case SDL_SCANCODE_U:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[U]"); strcpy(KeycodeChar, "U"); }
            else  { strcpy(KeycodeName, "[u]"); strcpy(KeycodeChar, "u"); }
            return;
            break;

        case SDL_SCANCODE_V:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[V]"); strcpy(KeycodeChar, "V"); }
            else  { strcpy(KeycodeName, "[v]"); strcpy(KeycodeChar, "v"); }
            return;
            break;

        case SDL_SCANCODE_W:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[W]"); strcpy(KeycodeChar, "W"); }
            else  { strcpy(KeycodeName, "[w]"); strcpy(KeycodeChar, "w"); }
            return;
            break;

        case SDL_SCANCODE_X:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[X]"); strcpy(KeycodeChar, "X"); }
            else  { strcpy(KeycodeName, "[x]"); strcpy(KeycodeChar, "x"); }
            return;
            break;

        case SDL_SCANCODE_Y:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[Y]"); strcpy(KeycodeChar, "Y"); }
            else  { strcpy(KeycodeName, "[y]"); strcpy(KeycodeChar, "y"); }
            return;
            break;

        case SDL_SCANCODE_Z:
            if (ShiftKeyPressed == true)  { strcpy(KeycodeName, "[Z]"); strcpy(KeycodeChar, "Z"); }
            else  { strcpy(KeycodeName, "[z]"); strcpy(KeycodeChar, "z"); }
            return;
            break;


        case SDL_SCANCODE_KP_0:
            strcpy(KeycodeName, "[Key Pad 0]");
            strcpy(KeycodeChar, "0");
            return;
            break;

        case SDL_SCANCODE_KP_1:
            strcpy(KeycodeName, "[Key Pad 1]");
            strcpy(KeycodeChar, "1");
            return;
            break;

        case SDL_SCANCODE_KP_2:
            strcpy(KeycodeName, "[Key Pad 2]");
            strcpy(KeycodeChar, "2");
            return;
            break;

        case SDL_SCANCODE_KP_3:
            strcpy(KeycodeName, "[Key Pad 3]");
            strcpy(KeycodeChar, "3");
            return;
            break;

        case SDL_SCANCODE_KP_4:
            strcpy(KeycodeName, "[Key Pad 4]");
            strcpy(KeycodeChar, "4");
            return;
            break;

        case SDL_SCANCODE_KP_5:
            strcpy(KeycodeName, "[Key Pad 5]");
            strcpy(KeycodeChar, "5");
            return;
            break;

        case SDL_SCANCODE_KP_6:
            strcpy(KeycodeName, "[Key Pad 6]");
            strcpy(KeycodeChar, "6");
            return;
            break;

        case SDL_SCANCODE_KP_7:
            strcpy(KeycodeName, "[Key Pad 7]");
            strcpy(KeycodeChar, "7");
            return;
            break;

        case SDL_SCANCODE_KP_8:
            strcpy(KeycodeName, "[Key Pad 8]");
            strcpy(KeycodeChar, "8");
            return;
            break;

        case SDL_SCANCODE_KP_9:
            strcpy(KeycodeName, "[Key Pad 9]");
            strcpy(KeycodeChar, "9");
            return;
            break;

        case SDL_SCANCODE_KP_DIVIDE:
            strcpy(KeycodeName, "[Key Pad /]");
            strcpy(KeycodeChar, "/");
            return;
            break;

        case SDL_SCANCODE_KP_MULTIPLY:
            strcpy(KeycodeName, "[Key Pad *]");
            strcpy(KeycodeChar, "*");
            return;
            break;

        case SDL_SCANCODE_KP_MINUS:
            strcpy(KeycodeName, "[Key Pad -]");
            strcpy(KeycodeChar, "-");
            return;
            break;

        case SDL_SCANCODE_KP_PLUS:
            strcpy(KeycodeName, "[Key Pad +]");
            strcpy(KeycodeChar, "+");
            return;
            break;

        case SDL_SCANCODE_KP_ENTER:
            strcpy(KeycodeName, "[Key Pad Enter]");
            strcpy(KeycodeChar, "");
            return;
            break;

        case SDL_SCANCODE_KP_PERIOD:
            strcpy(KeycodeName, "[Key Pad .]");
            strcpy(KeycodeChar, ".");
            return;
            break;


        case SDL_SCANCODE_SPACE:
            strcpy(KeycodeName, "[Spacebar]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_RETURN:
            strcpy(KeycodeName, "[Return]");
            strcpy(KeycodeChar, " ");
            return;
            break;


        case SDL_SCANCODE_F1:
            strcpy(KeycodeName, "[F1]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_F2:
            strcpy(KeycodeName, "[F2]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_F3:
            strcpy(KeycodeName, "[F3]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_F4:
            strcpy(KeycodeName, "[F4]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_F5:
            strcpy(KeycodeName, "[F5]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_F6:
            strcpy(KeycodeName, "[F6]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_F7:
            strcpy(KeycodeName, "[F7]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_F8:
            strcpy(KeycodeName, "[F8]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_F9:
            strcpy(KeycodeName, "[F9]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_F10:
            strcpy(KeycodeName, "[F10]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_F11:
            strcpy(KeycodeName, "[F11]");
            strcpy(KeycodeChar, " ");
            return;
            break;

        case SDL_SCANCODE_F12:
            strcpy(KeycodeName, "[F12]");
            strcpy(KeycodeChar, " ");
            return;
            break;


        default:
            strcpy(KeycodeName, "[Unknown]");
            strcpy(KeycodeChar, " ");
            break;
    }
}

//-------------------------------------------------------------------------------------------------
bool Input::CheckKeyCodeName(const char *compareKeyCodeName)
{
bool returnValue = false;

    if ( strcmp(KeycodeName, compareKeyCodeName) == 0 )  returnValue = true;

    return(returnValue);
}

//-------------------------------------------------------------------------------------------------
void Input::GetAllUserInput(void)
{
    if ( CheckKeyCodeName("[Esc]") == true )
    {
        if (screens->ScreenToDisplay == TitleScreen)  EXIT_Game = true;
        else  screens->ScreenTransitionStatus = FadeOut;
    }

    KeyOnKeyboardPressedByUser = -1;

    MouseButtonPressed[0] = false;
    MouseButtonPressed[1] = false;

    for (int index = 0; index < 5; index++)
    {
        JoystickDirectionHorizonal[index] = CENTER;
        JoystickDirectionVertical[index] = CENTER;
        JoystickButtonOne[index] = OFF;
        JoystickButtonTwo[index] = OFF;
    }

    strcpy(KeycodeName, "");
    strcpy(KeycodeChar, "");

    if (DelayAllUserInput > 0)
    {
        DelayAllUserInput--;
        return;
    }

    KeyModState = SDL_GetModState();

    if ( (KeyModState && KMOD_LSHIFT) || (KeyModState && KMOD_RSHIFT) )  ShiftKeyPressed = true;
    else  ShiftKeyPressed = false;

    while ( SDL_PollEvent(&Event) )
    {
        switch (Event.type)
        {
            case SDL_QUIT:
                EXIT_Game = true;
                return;
                break;

            case SDL_WINDOWEVENT:
                if (Event.window.event == SDL_WINDOWEVENT_RESIZED || Event.window.event == SDL_WINDOWEVENT_SHOWN)
                {
                    screens->ScreenIsDirty = true;
                    logic->GameDisplayChanged = true;
                }
                break;

            case SDL_KEYDOWN:
                KeyOnKeyboardPressedByUser = Event.key.keysym.scancode;

                if (KeyOnKeyboardPressedByUser == SDL_SCANCODE_LSHIFT || KeyOnKeyboardPressedByUser == SDL_SCANCODE_RSHIFT)
                    KeyOnKeyboardPressedByUser = -1;

                ConvertScancodeToNameAndChar(KeyOnKeyboardPressedByUser);
                break;

            default:
                break;
        }
    }

    bool usingCustomKeys = true;
    if (UserDefinedKeyButtonOne == -1
     || UserDefinedKeyButtonTwo == -1
     || UserDefinedKeyUP == -1
     || UserDefinedKeyRIGHT == -1
     || UserDefinedKeyDOWN == -1
     || UserDefinedKeyLEFT == -1
     || UserDefinedKeyPause == -1)  usingCustomKeys = false;

    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    if ( (screens->ScreenToDisplay != PlayingGameScreen)
     || (screens->ScreenToDisplay == PlayingGameScreen && usingCustomKeys == false) )
    {
        if      (keyboardState[    SDL_SCANCODE_Z] && screens->ScreenToDisplay != NameInputKeyboardScreen)  JoystickButtonOne[Keyboard] = ON;
        else if (keyboardState[    SDL_SCANCODE_X] && screens->ScreenToDisplay != NameInputKeyboardScreen)  JoystickButtonTwo[Keyboard] = ON;

        if      (keyboardState[   SDL_SCANCODE_UP])  JoystickDirectionVertical[Keyboard] = UP;
        else if (keyboardState[ SDL_SCANCODE_DOWN])  JoystickDirectionVertical[Keyboard] = DOWN;

        if      (keyboardState[ SDL_SCANCODE_LEFT])  JoystickDirectionHorizonal[Keyboard] = LEFT;
        else if (keyboardState[SDL_SCANCODE_RIGHT])  JoystickDirectionHorizonal[Keyboard] = RIGHT;
    }
    else if (screens->ScreenToDisplay == PlayingGameScreen && usingCustomKeys == true)
    {
        if      (keyboardState[UserDefinedKeyButtonOne] && screens->ScreenToDisplay != NameInputKeyboardScreen)  JoystickButtonOne[Keyboard] = ON;
        else if (keyboardState[UserDefinedKeyButtonTwo] && screens->ScreenToDisplay != NameInputKeyboardScreen)  JoystickButtonTwo[Keyboard] = ON;

        if      (keyboardState[       UserDefinedKeyUP])  JoystickDirectionVertical[Keyboard] = UP;
        else if (keyboardState[     UserDefinedKeyDOWN])  JoystickDirectionVertical[Keyboard] = DOWN;

        if      (keyboardState[     UserDefinedKeyLEFT])  JoystickDirectionHorizonal[Keyboard] = LEFT;
        else if (keyboardState[    UserDefinedKeyRIGHT])  JoystickDirectionHorizonal[Keyboard] = RIGHT;
    }

//------------------------------------------------------------------------
    SDL_PumpEvents();
    SDL_GetMouseState(&MouseX, &MouseY);

    float tempX, tempXtwo;
    float tempY, tempYtwo;;
    if (visuals->WindowWidthCurrent != 640)
    {
        tempX = visuals->WindowWidthCurrent;
        tempXtwo = tempX / 640;
        tempX = MouseX;
        MouseX = (int)(tempX / tempXtwo);
    }

    if (visuals->WindowHeightCurrent != 480)
    {
        tempY = visuals->WindowHeightCurrent;
        tempYtwo = tempY / 480;
        tempY = MouseY;
        MouseY = (int)(tempY / tempYtwo);
    }

    for (int index = 0; index < 2; index++)
    {
        if ( SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(index+1) )
        {
            if (MouseButtonWasClicked[index] == false)
            {
                MouseButtonPressed[index] = true;
                MouseButtonWasClicked[index] = true;
            }
        }
        else  MouseButtonWasClicked[index] = false;
    }

//------------------------------------------------------------------------
    if (JoystickDeviceOne != NULL)
	{
		SDL_JoystickUpdate();

		Sint16 joystickXmovement = 0;
        if (JoyLEFT[0] > Axis7)
        {
            if ( SDL_JoystickGetButton(JoystickDeviceOne, JoyLEFT[0]-8) )  JoystickDirectionHorizonal[JoystickOne] = LEFT;
            else  if ( SDL_JoystickGetButton(JoystickDeviceOne, JoyRIGHT[0]-8) )  JoystickDirectionHorizonal[JoystickOne] = RIGHT;
        }
        else joystickXmovement = SDL_JoystickGetAxis(JoystickDeviceOne, JoyLEFT[0]);

		Sint16 joystickYmovement = 0;
        if (JoyUP[0] > Axis7)
        {
            if ( SDL_JoystickGetButton(JoystickDeviceOne, JoyUP[0]-8) )  JoystickDirectionVertical[JoystickOne] = UP;
            else  if ( SDL_JoystickGetButton(JoystickDeviceOne, JoyDOWN[0]-8) )  JoystickDirectionVertical[JoystickOne] = DOWN;
        }
        else joystickYmovement = SDL_JoystickGetAxis(JoystickDeviceOne, JoyUP[0]);

		if (joystickYmovement < -16383)
		{
            JoystickDirectionVertical[JoystickOne] = UP;
		}
		else if (joystickYmovement > 16383)
		{
            JoystickDirectionVertical[JoystickOne] = DOWN;
		}

		if (joystickXmovement < -16383)
		{
            JoystickDirectionHorizonal[JoystickOne] = LEFT;
		}
		else if (joystickXmovement > 16383)
		{
            JoystickDirectionHorizonal[JoystickOne] = RIGHT;
		}

        if (JoyButton1[0] > Axis7)
        {
	    	if ( SDL_JoystickGetButton(JoystickDeviceOne, JoyButton1[0]-8) )  JoystickButtonOne[JoystickOne] = ON;
        }
        else
        {
            Sint16 padAsButton = SDL_JoystickGetAxis(JoystickDeviceOne, JoyButton1[0]);
            if (padAsButton < -16383)  JoystickButtonOne[JoystickOne] = ON;
        }

        if (JoyButton2[0] > Axis7)
        {
    		if ( SDL_JoystickGetButton(JoystickDeviceOne, JoyButton2[0]-8) )  JoystickButtonTwo[JoystickOne] = ON;
        }
        else
        {
            Sint16 padAsButton = SDL_JoystickGetAxis(JoystickDeviceOne, JoyButton2[0]);
            if (padAsButton < -16383)  JoystickButtonTwo[JoystickOne] = ON;
        }
    }

    if (JoystickDeviceTwo != NULL)
	{
		SDL_JoystickUpdate();

		Sint16 joystickXmovement = 0;
        if (JoyLEFT[1] > Axis7)
        {
            if ( SDL_JoystickGetButton(JoystickDeviceTwo, JoyLEFT[1]-8) )  JoystickDirectionHorizonal[JoystickTwo] = LEFT;
            else  if ( SDL_JoystickGetButton(JoystickDeviceTwo, JoyRIGHT[1]-8) )  JoystickDirectionHorizonal[JoystickTwo] = RIGHT;
        }
        else joystickXmovement = SDL_JoystickGetAxis(JoystickDeviceTwo, JoyLEFT[1]);

		Sint16 joystickYmovement = 0;
        if (JoyUP[1] > Axis7)
        {
            if ( SDL_JoystickGetButton(JoystickDeviceTwo, JoyUP[1]-8) )  JoystickDirectionVertical[JoystickTwo] = UP;
            else  if ( SDL_JoystickGetButton(JoystickDeviceTwo, JoyDOWN[1]-8) )  JoystickDirectionVertical[JoystickTwo] = DOWN;
        }
        else joystickYmovement = SDL_JoystickGetAxis(JoystickDeviceTwo, JoyUP[1]);

		if (joystickYmovement < -16383)
		{
            JoystickDirectionVertical[JoystickTwo] = UP;
		}
		else if (joystickYmovement > 16383)
		{
            JoystickDirectionVertical[JoystickTwo] = DOWN;
		}

		if (joystickXmovement < -16383)
		{
            JoystickDirectionHorizonal[JoystickTwo] = LEFT;
		}
		else if (joystickXmovement > 16383)
		{
            JoystickDirectionHorizonal[JoystickTwo] = RIGHT;
		}

        if (JoyButton1[1] > Axis7)
        {
	    	if ( SDL_JoystickGetButton(JoystickDeviceTwo, JoyButton1[1]-8) )  JoystickButtonOne[JoystickTwo] = ON;
        }
        else
        {
            Sint16 padAsButton = SDL_JoystickGetAxis(JoystickDeviceTwo, JoyButton1[1]);
            if (padAsButton < -16383)  JoystickButtonOne[JoystickTwo] = ON;
        }

        if (JoyButton2[1] > Axis7)
        {
    		if ( SDL_JoystickGetButton(JoystickDeviceTwo, JoyButton2[1]-8) )  JoystickButtonTwo[JoystickTwo] = ON;
        }
        else
        {
            Sint16 padAsButton = SDL_JoystickGetAxis(JoystickDeviceTwo, JoyButton2[1]);
            if (padAsButton < -16383)  JoystickButtonTwo[JoystickTwo] = ON;
        }
    }

    if (JoystickDeviceThree != NULL)
	{
		SDL_JoystickUpdate();

		Sint16 joystickXmovement = 0;
        if (JoyLEFT[2] > Axis7)
        {
            if ( SDL_JoystickGetButton(JoystickDeviceThree, JoyLEFT[2]-8) )  JoystickDirectionHorizonal[JoystickThree] = LEFT;
            else  if ( SDL_JoystickGetButton(JoystickDeviceThree, JoyRIGHT[2]-8) )  JoystickDirectionHorizonal[JoystickThree] = RIGHT;
        }
        else joystickXmovement = SDL_JoystickGetAxis(JoystickDeviceThree, JoyLEFT[2]);

		Sint16 joystickYmovement = 0;
        if (JoyUP[2] > Axis7)
        {
            if ( SDL_JoystickGetButton(JoystickDeviceThree, JoyUP[2]-8) )  JoystickDirectionVertical[JoystickThree] = UP;
            else  if ( SDL_JoystickGetButton(JoystickDeviceThree, JoyDOWN[2]-8) )  JoystickDirectionVertical[JoystickThree] = DOWN;
        }
        else joystickYmovement = SDL_JoystickGetAxis(JoystickDeviceThree, JoyUP[2]);

		if (joystickYmovement < -16383)
		{
            JoystickDirectionVertical[JoystickThree] = UP;
		}
		else if (joystickYmovement > 16383)
		{
            JoystickDirectionVertical[JoystickThree] = DOWN;
		}

		if (joystickXmovement < -16383)
		{
            JoystickDirectionHorizonal[JoystickThree] = LEFT;
		}
		else if (joystickXmovement > 16383)
		{
            JoystickDirectionHorizonal[JoystickThree] = RIGHT;
		}

        if (JoyButton1[2] > Axis7)
        {
	    	if ( SDL_JoystickGetButton(JoystickDeviceThree, JoyButton1[2]-8) )  JoystickButtonOne[JoystickThree] = ON;
        }
        else
        {
            Sint16 padAsButton = SDL_JoystickGetAxis(JoystickDeviceThree, JoyButton1[2]);
            if (padAsButton < -16383)  JoystickButtonOne[JoystickThree] = ON;
        }

        if (JoyButton2[2] > Axis7)
        {
    		if ( SDL_JoystickGetButton(JoystickDeviceThree, JoyButton2[2]-8) )  JoystickButtonTwo[JoystickThree] = ON;
        }
        else
        {
            Sint16 padAsButton = SDL_JoystickGetAxis(JoystickDeviceThree, JoyButton2[2]);
            if (padAsButton < -16383)  JoystickButtonTwo[JoystickThree] = ON;
        }
    }
//------------------------------------------------------------------------

    JoystickDirectionHorizonal[Any] = CENTER;
    JoystickDirectionVertical[Any] = CENTER;
    JoystickButtonOne[Any] = OFF;
    JoystickButtonTwo[Any] = OFF;

    for (int index = 0; index < 4; index++)
    {
        if (JoystickDirectionHorizonal[Any] == CENTER)  JoystickDirectionHorizonal[Any] = JoystickDirectionHorizonal[index];
        if (JoystickDirectionVertical[Any] == CENTER)  JoystickDirectionVertical[Any] = JoystickDirectionVertical[index];
        if (JoystickButtonOne[Any] == OFF)  JoystickButtonOne[Any] = JoystickButtonOne[index];
        if (JoystickButtonTwo[Any] == OFF)  JoystickButtonTwo[Any] = JoystickButtonTwo[index];
    }

//------------------------------------------------------------------------
}
