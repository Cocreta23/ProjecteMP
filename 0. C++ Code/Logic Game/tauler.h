#pragma once
#include <iostream>
#include <vector>
using namespace std;
#include "Posicio.h"
#include "Fitxa.h"
#include "Moviment.h"
const int N_FILES = 8;
const int N_COLUMNES = 8;


class Tauler {
public:
	void inicialitza(const string& nomFitxer);
	void getPosicionsPossiblesDames(const Posicio origen, int& nPosicions, Posicio posicionsPossibles[], int flagAdaltDreta, int flagAdaltEsquerra, int flagAbaixDreta, int flagAbaixEsquerra, int despresMenjar, ColorFitxa color, vector<Posicio>& cami_actual, bool& haCapturat);
	void getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]);
	bool mouFitxa(const Posicio& origen, const Posicio& desti);
	string inttoString(int fila, int col) const;
	void NetejaTauler();
	string toString() const;
	void getPosicionsPossiblesNorm(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[], int& fet, ColorFitxa Color, vector<Posicio> cami_actual);
	bool hiHaCapturaPossible(const Posicio& origen);
	void EliminaFitxaNormal2(Posicio origen, Posicio desti, Posicio posicionsPossibles[]);
	void EliminarFitxaNormal1(Posicio ori, Posicio dest);
	void eliminaFitxaDama(const Posicio origen, const Posicio desti, ColorFitxa color);
	Fitxa& getTauler(int fil, int col) { return m_tauler[fil][col]; }

private:
	Fitxa m_tauler[N_FILES][N_COLUMNES];
	vector<vector<Posicio>> vector_posicions;
};