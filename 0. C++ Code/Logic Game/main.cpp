//
//  main.cpp
//
//  Copyright � 2018 Compiled Creations Limited. All rights reserved.
//
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined  (_WIN64)

#include <iostream>
//Definicio necesaria per poder incloure la llibreria i que trobi el main
#define SDL_MAIN_HANDLED
#include <windows.h>
//Llibreria grafica
#include "../Graphic Lib/libreria.h"
#include "../Graphic Lib/NFont/NFont.h"
#include <conio.h>      /* getch */ 

#elif __APPLE__
//Llibreria grafica
#include "../Graphic Lib/libreria.h"
#include "../Graphic Lib/NFont/NFont.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <SDL2/SDL.h>
#pragma clang diagnostic pop

#endif

#include "./joc.hpp"
#include "./info_joc.hpp"
#include "GraphicManager.h"


int main(int argc, const char* argv[])
{


    //Instruccions necesaries per poder incloure la llibreria i que trobi el main
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);

    //Inicialitza un objecte de la classe Screen que s'utilitza per gestionar la finestra grafica
    Screen pantalla(TAMANY_PANTALLA_X, TAMANY_PANTALLA_Y);

    //Mostrem la finestra grafica
    pantalla.show();
    bool final_joc = false;
    Joc joc;
    int torn = 0;
    bool inici = true;

    
    do
    {
        pantalla.processEvents();

        bool mouseStatus = Mouse_getBtnLeft();
        int mousePosX = Mouse_getX();
        int mousePosY = Mouse_getY();

        final_joc = joc.actualitza(mousePosX, mousePosY, mouseStatus, torn, inici);
        if (joc.haFetJugadaNova())
        {
            torn = (torn + 1) % 2;
            joc.netejaJugadaNova();
        }

        pantalla.update();

    } while (!Keyboard_GetKeyTrg(KEYBOARD_ESCAPE) && final_joc == false);


    while (!Keyboard_GetKeyTrg(KEYBOARD_ESCAPE)) {
        pantalla.show();
        GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0);
        if (joc.getNegres() > joc.getBlanques())
        {
            GraphicManager::getInstance()->drawFont(FONT_RED_30, 74, 8, 2, "Guanyen Negres");
            GraphicManager::getInstance()->drawSprite(GRAFIC_DOUBLE_TAKEDOWN, 150, 150);
            GraphicManager::getInstance()->drawSprite(GRAFIC_FINAL, 370, 370);
        }
        else {
            GraphicManager::getInstance()->drawFont(FONT_RED_30, 74, 8, 2, "Guanyen Blanques");
            GraphicManager::getInstance()->drawSprite(GRAFIC_FINAL, 370, 370);
            GraphicManager::getInstance()->drawSprite(GRAFIC_DOUBLE_TAKEDOWN, 150, 150);
        }
        pantalla.update();
    }
    SDL_Quit();


    return 0;
}

