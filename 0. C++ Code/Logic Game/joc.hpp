#ifndef JOC_H
#define JOC_H

#include <stdio.h>
#include "info_joc.hpp"
#include "tauler.h"
#include "CuaMoviment.h"
#include <vector>
#include <utility>


using namespace std;


class Joc
{

public:
    Joc();
    int getNegres() { return negres; }

    int getBlanques() { return blanques; }
    bool actualitza(int mousePosX, int mousePosY, bool mouseStatus, int torn, bool& inici);
    bool haFetJugadaNova() const { return jugadaNova; }
    void netejaJugadaNova() { jugadaNova = false; }
    void Inicialitza(ModeJoc mode, const string& nomFitxerTauler, const string& nomFitxerMoviments);

private:
    Tauler m_tauler;
    int filaFitxa = -1;
    int columnaFitxa = -1;
    bool clicRegistrat = false;
    bool jugadaNova = false;
    bool clicAnterior = false;
    Posicio m_posicioSeleccionada;
    Posicio m_posicionsPossibles[15];
    int m_nPosicionsPossibles;
    bool m_mostrarPosicions = false;
    CuaMoviment* m_actual;
    CuaMoviment* m_primer;
    vector<pair<Posicio, Posicio>> m_movimentsReplay;
    int m_indexMovimentActual = 0;
    string m_mode;
    int blanques;
    int negres;
    int m_valorsAleatoris[15];
};

#endif 