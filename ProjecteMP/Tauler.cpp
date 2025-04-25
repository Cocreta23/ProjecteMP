#include "Tauler.h"
#include <fstream>
void Tauler::NetejaTauler()
{
	for (int i = 0; i < N_FILES; i++)
	{
		for (int j = 0; j < N_COLUMNES; j++)
		{
			m_tauler[i][j].setTipus(TIPUS_EMPTY);
		}
	}
}
void Tauler::inicialitza(const string& nomFitxer)
{
	NetejaTauler();
	ifstream fitxer(nomFitxer);
	char tipus;
	string posicio;
	while (fitxer.is_open())
	{
		while (fitxer >> tipus >> posicio) {
			int col = posicio[0] - 1;
			int fil = posicio[1] - 'a';
			if (tipus == 'O')
			{
				m_tauler[fil][col].setTipus(TIPUS_NORMAL);
				m_tauler[fil][col].setColor(COLOR_BLANC);
			}
			else
				if (tipus == 'X')
				{
					m_tauler[fil][col].setTipus(TIPUS_NORMAL);
					m_tauler[fil][col].setColor(COLOR_NEGRE);
				}
				else
					if (tipus == 'D')
					{
						m_tauler[fil][col].setTipus(TIPUS_DAMA);
						m_tauler[fil][col].setColor(COLOR_BLANC);
					}
					else
						if (tipus == 'R')
						{
							m_tauler[fil][col].setTipus(TIPUS_DAMA);
							m_tauler[fil][col].setColor(COLOR_NEGRE);
						}

		}
	}
}


void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[])
{
    
    int fila = origen.getFila();
    int col = origen.getCol();

    if (m_tauler[fila][col].getColor() == COLOR_BLANC) {

        if (m_tauler[fila + 1][col + 1].getTipus() == TIPUS_EMPTY) {
            Posicio posicio = inttoString(fila + 1, col + 1);
            posicionsPossibles[nPosicions] = posicio;
            nPosicions++;
        }
        else if (m_tauler[fila + 2][col + 2].getTipus() == TIPUS_EMPTY &&
            m_tauler[fila + 1][col + 1].getColor() != COLOR_BLANC) {
            Posicio posicio = inttoString(fila + 2, col + 2);
            posicionsPossibles[nPosicions] = posicio;
            nPosicions++;
            getPosicionsPossibles(posicio, nPosicions, posicionsPossibles);
        }

        if (m_tauler[fila + 1][col - 1].getTipus() == TIPUS_EMPTY) {
            Posicio posicio = inttoString(fila + 1, col - 1);
            posicionsPossibles[nPosicions] = posicio;
            nPosicions++;
        }
        else if (m_tauler[fila + 2][col - 2].getTipus() == TIPUS_EMPTY &&
            m_tauler[fila + 1][col - 1].getColor() != COLOR_BLANC) {
            Posicio posicio = inttoString(fila + 2, col - 2);
            posicionsPossibles[nPosicions] = posicio;
            nPosicions++;
            getPosicionsPossibles(posicio, nPosicions, posicionsPossibles);
        }
    }
    else { 

        if (m_tauler[fila - 1][col + 1].getTipus() == TIPUS_EMPTY) {
            Posicio posicio = inttoString(fila - 1, col + 1);
            posicionsPossibles[nPosicions] = posicio;
            nPosicions++;
        }
        else if (m_tauler[fila - 2][col + 2].getTipus() == TIPUS_EMPTY &&
            m_tauler[fila - 1][col + 1].getColor() != COLOR_NEGRE) {
            Posicio posicio = inttoString(fila - 2, col + 2);
            posicionsPossibles[nPosicions] = posicio;
            nPosicions++;
        }

        if (m_tauler[fila - 1][col - 1].getTipus() == TIPUS_EMPTY) {
            Posicio posicio = inttoString(fila - 1, col - 1);
            posicionsPossibles[nPosicions] = posicio;
            nPosicions++;
        }
        else if (m_tauler[fila - 2][col - 2].getTipus() == TIPUS_EMPTY &&
            m_tauler[fila - 1][col - 1].getColor() != COLOR_NEGRE) {
            Posicio posicio = inttoString(fila - 2, col - 2);
            posicionsPossibles[nPosicions] = posicio;
            nPosicions++;
            getPosicionsPossibles(posicio, nPosicions, posicionsPossibles);
        }
    }
}


string Tauler::inttoString(int fila, int col) const
{
	char lletra_col = 'a' + col;
	return string(fila, lletra_col);
}




bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
{
    int n = 0;
    Posicio posicionsPossibles[12];
    getPosicionsPossibles(origen, n, posicionsPossibles);

    for (int i = 0; i < n; i++)
    {
        if (desti == posicionsPossibles[i]) 
        {
            return true;
        }
    }

    return false;
}

string Tauler::toString() const {
    for (int i = 7; i >= 0; i--)
    {
        int fila = 7 - i;
        cout << i << ": ";
        for (int i = 0; i < 8; i++)
        {
            if (m_tauler[fila][i].getTipus() == TIPUS_EMPTY) {
                cout << "_";
            }
            else {
                if (m_tauler[fila][i].getTipus() == TIPUS_DAMA)
                {
                    if (m_tauler[fila][i].getColor() == COLOR_BLANC)
                    {
                        cout << "D";
                    }
                    else{
                        cout << "R";
                    }
                }
                else {
                    if (m_tauler[fila][i].getColor() == COLOR_BLANC)
                    {
                        cout << "O";
                    }
                    else {
                        cout << "X";
                    }
                }
            }
        }
        cout << endl;
    }
    for (int i = 0; i < 8; i++)
    {
        cout << "   " << 'A' + i;
    }
}

