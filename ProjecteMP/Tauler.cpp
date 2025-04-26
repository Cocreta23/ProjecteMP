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

    if (!fitxer.is_open()) {
        cout << "No s'ha trobat el fitxer " << nomFitxer << endl;
        return;
    }

    char tipus;
    string posicio;

    while (fitxer >> tipus >> posicio) {
        int col = posicio[0] - 'a'; 
        int fil = posicio[1] - '1'; 

        if (tipus == 'O') {
            m_tauler[fil][col].setTipus(TIPUS_NORMAL);
            m_tauler[fil][col].setColor(COLOR_BLANC);
        }
        else if (tipus == 'X') {
            m_tauler[fil][col].setTipus(TIPUS_NORMAL);
            m_tauler[fil][col].setColor(COLOR_NEGRE);
        }
        else if (tipus == 'D') {
            m_tauler[fil][col].setTipus(TIPUS_DAMA);
            m_tauler[fil][col].setColor(COLOR_BLANC);
        }
        else if (tipus == 'R') {
            m_tauler[fil][col].setTipus(TIPUS_DAMA);
            m_tauler[fil][col].setColor(COLOR_NEGRE);
        }
    }
}



void Tauler::getPosicionsPossiblesNorm(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[])
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
    else
        if (m_tauler[fila][col].getColor() != COLOR_BLANC) {

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




bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti) {
    int n = 0;
    Posicio posicionsPossibles[12];
    getPosicionsPossibles(origen, n, posicionsPossibles);

    for (int i = 0; i < n; i++) {
        if (desti == posicionsPossibles[i]) {
            m_tauler[desti.getFila()][desti.getCol()] = m_tauler[origen.getFila()][origen.getCol()];
            
            if (desti.getFila() == 7 && m_tauler[desti.getFila()][desti.getCol()].getTipus() == TIPUS_NORMAL)
            {
                m_tauler[desti.getFila()][desti.getCol()].setTipus(TIPUS_DAMA);
            }
            if (desti.getFila() > origen.getFila())
            {
                if (desti.getCol() > origen.getCol())
                {
                    for (int i = origen.getFila()+1; i < desti.getFila(); i++)
                    {
                        for (int j = origen.getCol() + 1; j < desti.getCol(); j++)
                        {
                            if (m_tauler[i][j].getTipus() != m_tauler[origen.getFila()][origen.getCol()].getTipus() && m_tauler[i][j].getTipus() != TIPUS_EMPTY)
                            {
                                m_tauler[i][j].setTipus(TIPUS_EMPTY);
                            }
                        }
                    }
                }
                else
                    for (int i = origen.getFila() + 1; i < desti.getFila(); i++)
                    {
                        for (int j = origen.getCol() - 1; j < desti.getCol(); j--)
                        {
                            if (m_tauler[i][j].getTipus() != m_tauler[origen.getFila()][origen.getCol()].getTipus() && m_tauler[i][j].getTipus() != TIPUS_EMPTY)
                            {
                                m_tauler[i][j].setTipus(TIPUS_EMPTY);
                            }
                        }
                    }

            }
            else
                if (desti.getCol() > origen.getCol())
                {
                    for (int i = origen.getFila() - 1; i < desti.getFila(); i++)
                    {
                        for (int j = origen.getCol() + 1; j < desti.getCol(); j++)
                        {
                            if (m_tauler[i][j].getTipus() != m_tauler[origen.getFila()][origen.getCol()].getTipus() && m_tauler[i][j].getTipus() != TIPUS_EMPTY)
                            {
                                m_tauler[i][j].setTipus(TIPUS_EMPTY);
                            }
                        }
                    }
                }
                else
                    for (int i = origen.getFila() - 1; i < desti.getFila(); i++)
                    {
                        for (int j = origen.getCol() - 1; j < desti.getCol(); j--)
                        {
                            if (m_tauler[i][j].getTipus() != m_tauler[origen.getFila()][origen.getCol()].getTipus() && m_tauler[i][j].getTipus() != TIPUS_EMPTY)
                            {
                                m_tauler[i][j].setTipus(TIPUS_EMPTY);
                            }
                        }
                    }
            m_tauler[origen.getFila()][origen.getCol()].setTipus(TIPUS_EMPTY);
            return true;

        }
    }

    return false;
}


string Tauler::toString() const {
    for (int i = 7; i >= 0; i--)
    {
        int fila = 7 - i;
        cout << i+1 << ": ";
        for (int i = 0; i < 8; i++)
        {
            if (m_tauler[fila][i].getTipus() == TIPUS_EMPTY) {
                cout << "- ";
            }
            else {
                if (m_tauler[fila][i].getTipus() == TIPUS_DAMA)
                {
                    if (m_tauler[fila][i].getColor() == COLOR_BLANC)
                    {
                        cout << "D ";
                    }
                    else {
                        cout << "R ";
                    }
                }
                else {
                    if (m_tauler[fila][i].getColor() == COLOR_BLANC)
                    {
                        cout << "O ";
                    }
                    else {
                        cout << "X ";
                    }
                }
            }
        }
        cout << endl;
    }
    cout << "   a b c d e f g h";
    return "hola";
}

void Tauler::getPosicionsPossiblesDames(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[])
{
    int fila = origen.getFila();
    int col = origen.getCol();
    if (m_tauler[fila][col].getColor() == COLOR_BLANC && fila < 8 && col < 8 && fila > 0 && col > 0)
    {
        if (m_tauler[fila + 1][col + 1].getColor() == COLOR_NEGRE && m_tauler[fila+1][col+1].getTipus() != TIPUS_EMPTY)
        {
            Posicio posicio = inttoString(fila + 2, col + 2);
            posicionsPossibles[nPosicions] = posicio;
            nPosicions++;
            getPosicionsPossiblesDames(posicio, nPosicions, posicionsPossibles);
        }
        else 
            if (m_tauler[fila + 1][col - 1].getColor() == COLOR_NEGRE && m_tauler[fila + 1][col + 1].getTipus() != TIPUS_EMPTY)
            {
                Posicio posicio = inttoString(fila + 2, col - 2);
                posicionsPossibles[nPosicions] = posicio;
                nPosicions++;
                getPosicionsPossiblesDames(posicio, nPosicions, posicionsPossibles);
            }

        else
            if (m_tauler[fila + 1][col + 1].getTipus() == TIPUS_EMPTY)
            {
                Posicio posicio = inttoString(fila + 1, col + 1);
                posicionsPossibles[nPosicions] = posicio;
                nPosicions++;
                getPosicionsPossiblesDames(posicio, nPosicions, posicionsPossibles);
            }
            else 
                if (m_tauler[fila + 1][col + 1].getTipus() == TIPUS_EMPTY)
                {
                    Posicio posicio = inttoString(fila + 1, col - 1);
                    posicionsPossibles[nPosicions] = posicio;
                    nPosicions++;
                    getPosicionsPossiblesDames(posicio, nPosicions, posicionsPossibles);
                }
            
                else 
                    if (m_tauler[fila - 1][col + 1].getColor() == COLOR_NEGRE && m_tauler[fila + 1][col + 1].getTipus() != TIPUS_EMPTY)
                    {
                        Posicio posicio = inttoString(fila - 2, col + 2);
                        posicionsPossibles[nPosicions] = posicio;
                        nPosicions++;
                        getPosicionsPossiblesDames(posicio, nPosicions, posicionsPossibles);
                    }
                    else
                        if (m_tauler[fila - 1][col - 1].getColor() == COLOR_NEGRE && m_tauler[fila + 1][col + 1].getTipus() != TIPUS_EMPTY)
                        {
                            Posicio posicio = inttoString(fila - 2, col - 2);
                            posicionsPossibles[nPosicions] = posicio;
                            nPosicions++;
                            getPosicionsPossiblesDames(posicio, nPosicions, posicionsPossibles);
                        }

                        else
                            if (m_tauler[fila - 1][col + 1].getTipus() == TIPUS_EMPTY)
                            {
                                Posicio posicio = inttoString(fila - 1, col + 1);
                                posicionsPossibles[nPosicions] = posicio;
                                nPosicions++;
                                getPosicionsPossiblesDames(posicio, nPosicions, posicionsPossibles);
                            }
                            else {
                                if (m_tauler[fila - 1][col + 1].getTipus() == TIPUS_EMPTY)
                                {
                                    Posicio posicio = inttoString(fila - 1, col - 1);
                                    posicionsPossibles[nPosicions] = posicio;
                                    nPosicions++;
                                    getPosicionsPossiblesDames(posicio, nPosicions, posicionsPossibles);
                                }
                            }

    }
    else
        if ((m_tauler[fila][col].getColor() == COLOR_NEGRE && fila < 8 && col < 8 && fila > 0 && col > 0)) {
            if (m_tauler[fila - 1][col + 1].getColor() == COLOR_BLANC && m_tauler[fila + 1][col + 1].getTipus() != TIPUS_EMPTY)
            {
                Posicio posicio = inttoString(fila - 2, col + 2);
                posicionsPossibles[nPosicions] = posicio;
                nPosicions++;
                getPosicionsPossiblesDames(posicio, nPosicions, posicionsPossibles);
            }
            else
                if (m_tauler[fila - 1][col - 1].getColor() == COLOR_BLANC && m_tauler[fila + 1][col + 1].getTipus() != TIPUS_EMPTY)
                {
                    Posicio posicio = inttoString(fila - 2, col - 2);
                    posicionsPossibles[nPosicions] = posicio;
                    nPosicions++;
                    getPosicionsPossiblesDames(posicio, nPosicions, posicionsPossibles);
                }

                else
                    if (m_tauler[fila - 1][col + 1].getTipus() == TIPUS_EMPTY)
                    {
                        Posicio posicio = inttoString(fila - 1, col + 1);
                        posicionsPossibles[nPosicions] = posicio;
                        nPosicions++;
                        getPosicionsPossiblesDames(posicio, nPosicions, posicionsPossibles);
                    }
                    else 
                        if (m_tauler[fila - 1][col + 1].getTipus() == TIPUS_EMPTY)
                        {
                            Posicio posicio = inttoString(fila - 1, col - 1);
                            posicionsPossibles[nPosicions] = posicio;
                            nPosicions++;
                            getPosicionsPossiblesDames(posicio, nPosicions, posicionsPossibles);
                        }
                    
                    if (m_tauler[fila + 1][col + 1].getColor() == COLOR_BLANC && m_tauler[fila + 1][col + 1].getTipus() != TIPUS_EMPTY)
                    {
                        Posicio posicio = inttoString(fila + 2, col + 2);
                        posicionsPossibles[nPosicions] = posicio;
                        nPosicions++;
                        getPosicionsPossiblesDames(posicio, nPosicions, posicionsPossibles);
                    }
                    else
                        if (m_tauler[fila + 1][col - 1].getColor() == COLOR_BLANC && m_tauler[fila + 1][col + 1].getTipus() != TIPUS_EMPTY)
                        {
                            Posicio posicio = inttoString(fila + 2, col - 2);
                            posicionsPossibles[nPosicions] = posicio;
                            nPosicions++;
                            getPosicionsPossiblesDames(posicio, nPosicions, posicionsPossibles);
                        }

                        else
                            if (m_tauler[fila + 1][col + 1].getTipus() == TIPUS_EMPTY)
                            {
                                Posicio posicio = inttoString(fila + 1, col + 1);
                                posicionsPossibles[nPosicions] = posicio;
                                nPosicions++;
                                getPosicionsPossiblesDames(posicio, nPosicions, posicionsPossibles);
                            }
                            else
                                if (m_tauler[fila + 1][col + 1].getTipus() == TIPUS_EMPTY)
                                {
                                    Posicio posicio = inttoString(fila + 1, col - 1);
                                    posicionsPossibles[nPosicions] = posicio;
                                    nPosicions++;
                                    getPosicionsPossiblesDames(posicio, nPosicions, posicionsPossibles);
                                }
        }

}


void Tauler::actualitzaMovimentsValids() {
    for (int i = 0; i < N_FILES; i++)
    {
        for (int ç = 0; ç < N_COLUMNES; ç++)
        {
            TipusFitxa tipus = m_tauler[i][ç].getTipus();
            Posicio pos = inttoString(i, ç);
            Posicio posicionsPossibles[N_FILES];
            int nPos = 0;
            if (tipus == TIPUS_NORMAL)
            {
                getPosicionsPossibles(pos, nPos, posicionsPossibles);
            }
            else
                if (tipus == TIPUS_DAMA)
                {
                    getPosicionsPossiblesDames(pos, nPos, posicionsPossibles);
                }
        }
    }
}


void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[])
{
    int fila = origen.getFila();
    int col = origen.getCol();
    if (m_tauler[fila][col].getTipus() == TIPUS_DAMA)
    {
        getPosicionsPossiblesDames(origen, nPosicions, posicionsPossibles);
    }
    else 
        if (m_tauler[fila][col].getTipus() == TIPUS_NORMAL)
        {
        getPosicionsPossiblesNorm(origen, nPosicions, posicionsPossibles);
    }
}

