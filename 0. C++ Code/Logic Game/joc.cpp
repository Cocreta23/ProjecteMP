#include <iostream>
#include <fstream>
#include "GraphicManager.h"
#include "joc.hpp"

Joc::Joc() {

    m_actual = nullptr;
    m_primer = nullptr;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            m_tauler.getTauler(i,j).setTipus(TIPUS_EMPTY);
    blanques = 0;
    negres = 0;
    clicRegistrat = false;
    filaFitxa = -1;
    columnaFitxa = -1;
}

void Joc::Inicialitza(ModeJoc mode, const string& nomFitxerTauler, const string& nomFitxerMoviments) {
    

    ifstream fitxer(nomFitxerTauler);
    if (!fitxer.is_open()) {
        cout << "No s'ha trobat el fitxer " << nomFitxerTauler << endl;
        return;
    }

    char tipus;
    string posicio;

    while (fitxer >> tipus >> posicio) {
        int col = posicio[0] - 'a';
        int fil = posicio[1] - '1';

        if (tipus == 'O') {
            m_tauler.getTauler(fil,col).setTipus(TIPUS_NORMAL);
            m_tauler.getTauler(fil, col).setColor(COLOR_BLANC);
        }
        else if (tipus == 'X') {
            m_tauler.getTauler(fil, col).setTipus(TIPUS_NORMAL);
            m_tauler.getTauler(fil, col).setColor(COLOR_NEGRE);
        }
        else if (tipus == 'D') {
            m_tauler.getTauler(fil, col).setTipus(TIPUS_DAMA);
            m_tauler.getTauler(fil, col).setColor(COLOR_BLANC);
        }
        else if (tipus == 'R') {
            m_tauler.getTauler(fil, col).setTipus(TIPUS_DAMA);
            m_tauler.getTauler(fil, col).setColor(COLOR_NEGRE);
        }
    }

    if (mode == MODE_JOC_REPLAY)
    {
        ifstream fitxer(nomFitxerMoviments);
        Posicio origen, desti;
        while (fitxer >> origen >> desti)
        {
            m_movimentsReplay.push_back(make_pair(origen, desti));
        }
        fitxer.close();
    }

}

bool Joc::actualitza(int mousePosX, int mousePosY, bool mouseStatus, int torn, bool& inici)
{
    const int midaCasella = 62;
    const int offsetX = 126;
    const int offsetY = 76;
    if(inici == true) {
        GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0);
        GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 325, 50, 1, "Menu");
        GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 275, 100, 1, "Mode Normal");
        GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 276, 140, 1, "Mode Replay");
        GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 265, 80, 1, "--------------------");
        GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 260, 120, 1, "|");
        GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 260, 147, 1, "|");
        GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 260, 93, 1, "|");
        GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 460, 120, 1, "|");
        GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 460, 147, 1, "|");
        GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 460, 93, 1, "|");
        GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 265, 120, 1, "--------------------");
        GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 265, 160, 1, "--------------------");
        if (mouseStatus && !clicAnterior) {
            if (mousePosX > 250 && mousePosX < 450 && mousePosY > 80 && mousePosY < 135) {
                inici = false;
                Inicialitza(MODE_JOC_NORMAL, "../1. Resources/data/Games/tauler_inicial.txt", "../1. Resources/data/Games/moviments.txt");
                Sound_Init();
                T_SOUND* so = Sound_LoadSound("data/sound.ogg");
                Sound_Play(so, SOUND_FORCE_RESTART);
                m_mode = "normal";
            }
            else if (mousePosX > 250 && mousePosX < 450 && mousePosY > 135 && mousePosY < 160) {
                inici = false;
                Inicialitza(MODE_JOC_REPLAY, "../1. Resources/data/Games/tauler_inicial.txt", "../1. Resources/data/Games/moviments.txt");
                Sound_Init();
                T_SOUND* so = Sound_LoadSound("data/sound.ogg");
                Sound_Play(so, SOUND_FORCE_RESTART);
                m_mode = "replay";


            }
        }

        clicAnterior = mouseStatus;

        return false;
    }

    else
        if (m_mode == "replay" && inici == false)
        {
            cout << "He entrat a replay";
            GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0);
            GraphicManager::getInstance()->drawSprite(GRAFIC_TAULER, 74, 8);
            if (m_indexMovimentActual < m_movimentsReplay.size())
            {
                auto moviment = m_movimentsReplay[m_indexMovimentActual];
                cout << "Reproduint moviment: " << moviment.first << " -> " << moviment.second << endl;
                m_tauler.mouFitxa(moviment.first, moviment.second);
                m_indexMovimentActual++;
                SDL_Delay(1000);
                for (int fila = 0; fila < 8; fila++) {
                    for (int col = 0; col < 8; col++) {
                        int posX = offsetX + col * midaCasella;
                        int posY = offsetY + fila * midaCasella;
                        if (m_tauler.getTauler(fila, col).getTipus() != TIPUS_EMPTY)
                        {

                            if (m_tauler.getTauler(fila, col).getColor() == COLOR_BLANC && m_tauler.getTauler(fila, col).getTipus() == TIPUS_NORMAL) {
                                GraphicManager::getInstance()->drawSprite(GRAFIC_FITXA_BLANCA, posX, posY);
                            }
                            else if (m_tauler.getTauler(fila, col).getColor() == COLOR_NEGRE && m_tauler.getTauler(fila, col).getTipus() == TIPUS_NORMAL) {
                                GraphicManager::getInstance()->drawSprite(GRAFIC_FITXA_NEGRA, posX, posY);
                            }
                            else if (m_tauler.getTauler(fila, col).getColor() == COLOR_BLANC && m_tauler.getTauler(fila, col).getTipus() == TIPUS_DAMA) {
                                GraphicManager::getInstance()->drawSprite(GRAFIC_DAMA_BLANCA, posX, posY);
                            }
                            else if (m_tauler.getTauler(fila, col).getColor() == COLOR_NEGRE && m_tauler.getTauler(fila, col).getTipus() == TIPUS_DAMA) {
                                GraphicManager::getInstance()->drawSprite(GRAFIC_DAMA_NEGRA, posX, posY);
                            }

                        }
                    }
                }
                return false;
            }
            else
            {
                return true;
            }
        }


    else
        if(m_mode == "normal" && inici == false){
        GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0);
        GraphicManager::getInstance()->drawSprite(GRAFIC_TAULER, 74, 8);

        if (mouseStatus && !clicAnterior &&
            mousePosX >= offsetX && mousePosX < offsetX + 8 * midaCasella &&
            mousePosY >= offsetY && mousePosY < offsetY + 8 * midaCasella)
        {
            int novaColumna = (mousePosX - offsetX) / midaCasella;
            int novaFila = (mousePosY - offsetY) / midaCasella;
            if (m_mostrarPosicions) {
                for (int i = 0; i < m_nPosicionsPossibles; i++) {
                    if (m_posicionsPossibles[i].getCol() == novaColumna &&
                        m_posicionsPossibles[i].getFila() == novaFila)
                    {
                        cout << "Moviment realitzat cap a " << (8-novaFila) << "," << novaColumna+1 << endl;
                        m_tauler.mouFitxa(m_posicioSeleccionada, m_posicionsPossibles[i]);
                        CuaMoviment* nou = new CuaMoviment(m_posicioSeleccionada, m_posicionsPossibles[i]);
                        nou->setNext(nullptr);
                        if (m_actual == nullptr) {
                            m_actual = nou;
                            m_primer = nou;
                        }
                        else {
                            m_actual->setNext(nou);
                            m_actual = nou;
                        }
                        m_mostrarPosicions = false;
                        m_nPosicionsPossibles = 0;
                        jugadaNova = true;
                        return false;
                    }
                }
            }

            if ((m_tauler.getTauler(novaFila, novaColumna).getColor() == COLOR_BLANC && torn == 0) ||
                (m_tauler.getTauler(novaFila, novaColumna).getColor() == COLOR_NEGRE && torn == 1))
            {
                if (m_tauler.getTauler(novaFila, novaColumna).getTipus() != TIPUS_EMPTY) {
                    m_posicioSeleccionada = m_tauler.inttoString(novaFila, novaColumna);
                    m_tauler.getPosicionsPossibles(m_posicioSeleccionada, m_nPosicionsPossibles, m_posicionsPossibles);
                    for (int i = 0; i < m_nPosicionsPossibles; ++i) {
                        m_valorsAleatoris[i] = rand() % 7;
                    }
                    m_mostrarPosicions = true;
                }

            }
        }

        clicAnterior = mouseStatus;

        if (m_mostrarPosicions) {
            for (int i = 0; i < m_nPosicionsPossibles; ++i) {
                int col = m_posicionsPossibles[i].getCol();
                int fila = m_posicionsPossibles[i].getFila();
                int posX = offsetX + col * midaCasella;
                int posY = offsetY + fila * midaCasella;
                IMAGE_NAME imatge = static_cast<IMAGE_NAME>(GRAFIC_POSICIO_VALIDA_1 + m_valorsAleatoris[i]);
                GraphicManager::getInstance()->drawSprite(imatge, posX, posY);
            }

        }

        blanques = 0;
        negres = 0;
        for (int fila = 0; fila < 8; fila++) {
            for (int col = 0; col < 8; col++) {
                int posX = offsetX + col * midaCasella;
                int posY = offsetY + fila * midaCasella;
                if (m_tauler.getTauler(fila, col).getTipus() != TIPUS_EMPTY)
                {

                    if (m_tauler.getTauler(fila, col).getColor() == COLOR_BLANC && m_tauler.getTauler(fila, col).getTipus() == TIPUS_NORMAL) {
                        GraphicManager::getInstance()->drawSprite(GRAFIC_FITXA_BLANCA, posX, posY);
                        blanques++;
                    }
                    else if (m_tauler.getTauler(fila, col).getColor() == COLOR_NEGRE && m_tauler.getTauler(fila, col).getTipus() == TIPUS_NORMAL) {
                        GraphicManager::getInstance()->drawSprite(GRAFIC_FITXA_NEGRA, posX, posY);
                        negres++;
                    }
                    else if (m_tauler.getTauler(fila, col).getColor() == COLOR_BLANC && m_tauler.getTauler(fila, col).getTipus() == TIPUS_DAMA) {
                        GraphicManager::getInstance()->drawSprite(GRAFIC_DAMA_BLANCA, posX, posY);
                        blanques++;
                    }
                    else if (m_tauler.getTauler(fila, col).getColor() == COLOR_NEGRE && m_tauler.getTauler(fila, col).getTipus() == TIPUS_DAMA) {
                        GraphicManager::getInstance()->drawSprite(GRAFIC_DAMA_NEGRA, posX, posY);
                        negres++;
                    }

                }
            }
        }
        int blanquesEli = 12 - blanques;
        int negresEli = 12 - negres;
        string negresElii = std::to_string(negresEli);
        string blanquesElii = to_string(blanquesEli);
        GraphicManager::getInstance()->drawFont(FONT_RED_30, 50, 700, 1, "Fitxes Blanques eliminades: ");
        GraphicManager::getInstance()->drawFont(FONT_RED_30, 50, 730, 1, "Fitxes Negres eliminades: ");
        GraphicManager::getInstance()->drawFont(FONT_RED_30, 435, 730, 1, negresElii);
        GraphicManager::getInstance()->drawFont(FONT_RED_30, 435, 700, 1, blanquesElii);




        if (negres == 0 || blanques == 0)
        {
            ofstream fitxer("../1. Resources/data/Games/moviments.txt");

            if (!fitxer.is_open()) {
                cerr << "Error obrint el fitxer per escriure: " << endl;
            }

            CuaMoviment* node = m_primer;
            while (node != nullptr) {


                fitxer << node->getOrigen() << " " << node->getDesti() << endl;
                node = node->getNext();
            }

            fitxer.close();
            return true;
        }
        return false;
    }
    
}