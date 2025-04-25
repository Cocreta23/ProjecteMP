#pragma once
#include <iostream>
using namespace std;
#include "Posicio.h"
#include "Fitxa.h"
const int N_FILES = 8;
const int N_COLUMNES = 8;


class Tauler {
public:
	void inicialitza(const string& nomFitxer);
	void actualitzaMovimentsValids();
	void getPosicionsPossibles(const Posicio& origen,int& nPosicions, Posicio posicionsPossibles[]);
	bool mouFitxa(const Posicio& origen, const Posicio& desti);
	string inttoString(int fila, int col) const;
	void NetejaTauler();
	string toString() const;
private:
	Fitxa m_tauler[N_FILES][N_COLUMNES];
};