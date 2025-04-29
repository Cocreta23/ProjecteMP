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
	void getPosicionsPossiblesDames(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[], bool despresMenjar, bool daldreta, bool dalsquerra, bool baixdreta, bool baixsquerra);
	void getPosicionsPossibles(const Posicio& origen,int& nPosicions, Posicio posicionsPossibles[]);
	bool mouFitxa(const Posicio& origen, const Posicio& desti);
	string inttoString(int fila, int col) const;
	void NetejaTauler();
	string toString() const;
	void getPosicionsPossiblesNorm(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[], int fet, ColorFitxa color);
	bool daltdreta(Posicio pos);
	bool dalesquerra(Posicio pos);
	bool baixadreta(Posicio pos);
	bool baixesquerra(Posicio pos);

private:
	Fitxa m_tauler[N_FILES][N_COLUMNES];

};