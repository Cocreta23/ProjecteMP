#pragma once
#include <vector>
#include <iostream>
using namespace std;
#include "Tauler.h"

class Moviment {
public:
	Moviment() {
		moviments = vector<vector<vector<Posicio>>>(N_FILES, vector<vector<Posicio>>(N_COLUMNES));
		for (int i = 0; i < N_FILES; i++)
		{
			for (int j = 0; j < N_COLUMNES; j++)
			{
				moviments[i][j] = {  };
			}
		}
	}

private:
	vector<vector<vector<Posicio>>> moviments;
};