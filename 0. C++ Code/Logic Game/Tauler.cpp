#include "tauler.h"
#include <fstream>
#include <sstream>
#include "GraphicManager.h"
#include <chrono>
#include <thread>



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

string Tauler::inttoString(int fila, int col) const {
    if (fila < 0 || fila >= N_FILES || col < 0 || col >= N_COLUMNES)
        return "XX";

    char lletra_col = 'a' + col;
    return string(1, lletra_col) + to_string(fila + 1);
}



string Tauler::toString() const {
    stringstream ss;
    for (int fila = N_FILES - 1; fila >= 0; fila--) {
        ss << (fila + 1) << ": ";
        for (int col = 0; col < N_COLUMNES; col++) {
            const Fitxa& fitxa = m_tauler[fila][col];
            if (fitxa.getTipus() == TIPUS_EMPTY) {
                ss << "_ ";
            }
            else {
                if (fitxa.getTipus() == TIPUS_DAMA) {
                    ss << (fitxa.getColor() == COLOR_BLANC ? "D " : "R ");
                }
                else {
                    ss << (fitxa.getColor() == COLOR_BLANC ? "O " : "X ");
                }
            }
        }
        ss << endl;
    }
    ss << "   a b c d e f g h";
    return ss.str();
}


void Tauler::getPosicionsPossiblesNorm(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[], int& fet, ColorFitxa Color, vector<Posicio> cami_actual)
{
    int fila = origen.getFila();
    int col = origen.getCol();
    int direccio = (Color == COLOR_BLANC) ? 1 : -1;

    if (fila + direccio >= 0 && fila + direccio < 8 && col + 1 < 8) {
        if (m_tauler[fila + direccio][col + 1].getTipus() == TIPUS_EMPTY && fet == 0) {
            Posicio posicio = inttoString(fila + direccio, col + 1);
            posicionsPossibles[nPosicions] = posicio;
            nPosicions++;
            cami_actual.push_back(posicio);
        }
        else {
            if (fila + 2 * direccio >= 0 && fila + 2 * direccio < 8 && col + 2 < 8
                && m_tauler[fila + 2 * direccio][col + 2].getTipus() == TIPUS_EMPTY
                && m_tauler[fila + 1 * direccio][col + 1].getTipus() != TIPUS_EMPTY
                && m_tauler[fila + direccio][col + 1].getColor() != Color) {
                Posicio posicio = inttoString(fila + 2 * direccio, col + 2);
                posicionsPossibles[nPosicions] = posicio;
                nPosicions++;
                fet++;
                cami_actual.push_back(posicio);
                getPosicionsPossiblesNorm(posicio, nPosicions, posicionsPossibles, fet, Color, cami_actual);
            }
        }
    }

    if (fila + direccio >= 0 && fila + direccio < 8 && col - 1 >= 0) {
        if (m_tauler[fila + direccio][col - 1].getTipus() == TIPUS_EMPTY && fet == 0) {
            Posicio posicio = inttoString(fila + direccio, col - 1);
            posicionsPossibles[nPosicions] = posicio;
            nPosicions++;
            cami_actual.push_back(posicio);
        }
        else {
            if (fila + 2 * direccio >= 0 && fila + 2 * direccio < 8 && col - 2 >= 0
                && m_tauler[fila + 2 * direccio][col - 2].getTipus() == TIPUS_EMPTY
                && m_tauler[fila + 1 * direccio][col - 1].getTipus() != TIPUS_EMPTY
                && m_tauler[fila + direccio][col - 1].getColor() != Color) {
                Posicio posicio = inttoString(fila + 2 * direccio, col - 2);
                posicionsPossibles[nPosicions] = posicio;
                nPosicions++;
                fet++;
                cami_actual.push_back(posicio);
                getPosicionsPossiblesNorm(posicio, nPosicions, posicionsPossibles, fet, Color, cami_actual);
            }
        }
    }
    fet--;
    vector_posicions.push_back(cami_actual);
    cami_actual.clear();
}


void Tauler::getPosicionsPossiblesDames(const Posicio origen, int& nPosicions, Posicio posicionsPossibles[], int flagAdaltDreta, int flagAdaltEsquerra, int flagAbaixDreta, int flagAbaixEsquerra, int despresMenjar, ColorFitxa color, vector<Posicio>& cami_actual, bool& haCapturat)
{
    int fila = origen.getFila();
    int col = origen.getCol();

    // Activar Flags Després De Menjar
    if (despresMenjar == 1)
    {
        for (int i = 1; i < N_FILES; i++)
        {
            // Adalt Dreta
            if (fila + i + 1 < 8 && col + i + 1 < 8 &&
                m_tauler[fila + i][col + i].getTipus() != TIPUS_EMPTY &&
                m_tauler[fila + i][col + i].getColor() != color &&
                m_tauler[fila + i + 1][col + i + 1].getTipus() == TIPUS_EMPTY)
            {
                flagAdaltDreta = 1;
            }
            // Adalt Esquerra
            if (fila + i + 1 < 8 && col - i - 1 >= 0 &&
                m_tauler[fila + i][col - i].getTipus() != TIPUS_EMPTY &&
                m_tauler[fila + i][col - i].getColor() != color &&
                m_tauler[fila + i + 1][col - i - 1].getTipus() == TIPUS_EMPTY)
            {
                flagAdaltEsquerra = 1;
            }
            // Abaix Dreta
            if (fila - i - 1 >= 0 && col + i + 1 < 8 &&
                m_tauler[fila - i][col + i].getTipus() != TIPUS_EMPTY &&
                m_tauler[fila - i][col + i].getColor() != color &&
                m_tauler[fila - i - 1][col + i + 1].getTipus() == TIPUS_EMPTY)
            {
                flagAbaixDreta = 1;
            }
            // Abaix Esquerra
            if (fila - i - 1 >= 0 && col - i - 1 >= 0 &&
                m_tauler[fila - i][col - i].getTipus() != TIPUS_EMPTY &&
                m_tauler[fila - i][col - i].getColor() != color &&
                m_tauler[fila - i - 1][col - i - 1].getTipus() == TIPUS_EMPTY)
            {
                flagAbaixEsquerra = 1;
            }
        }
    }

    // Avançar Simple
    {
        if (fila + 1 < 8 && col + 1 < 8 &&
            m_tauler[fila + 1][col + 1].getTipus() == TIPUS_EMPTY && flagAdaltDreta == 1)
        {
            Posicio pos = inttoString(fila + 1, col + 1);
            bool existeix = false;
            for (int i = 0; i < nPosicions; ++i)
                if (posicionsPossibles[i] == pos)
                    existeix = true;
            if (!existeix)
            {
                posicionsPossibles[nPosicions++] = pos;
                cami_actual.push_back(pos);
                getPosicionsPossiblesDames(pos, nPosicions, posicionsPossibles, 1, 0, 0, 0, 0, color, cami_actual, haCapturat);
            }
        }

        if (fila + 1 < 8 && col - 1 >= 0 &&
            m_tauler[fila + 1][col - 1].getTipus() == TIPUS_EMPTY && flagAdaltEsquerra == 1)
        {
            Posicio pos = inttoString(fila + 1, col - 1);
            bool existeix = false;
            for (int i = 0; i < nPosicions; ++i)
                if (posicionsPossibles[i] == pos)
                    existeix = true;
            if (!existeix)
            {
                posicionsPossibles[nPosicions++] = pos;
                cami_actual.push_back(pos);
                getPosicionsPossiblesDames(pos, nPosicions, posicionsPossibles, 0, 1, 0, 0, 0, color, cami_actual, haCapturat);
            }
        }

        if (fila - 1 >= 0 && col + 1 < 8 &&
            m_tauler[fila - 1][col + 1].getTipus() == TIPUS_EMPTY && flagAbaixDreta == 1)
        {
            Posicio pos = inttoString(fila - 1, col + 1);
            bool existeix = false;
            for (int i = 0; i < nPosicions; ++i)
                if (posicionsPossibles[i] == pos)
                    existeix = true;
            if (!existeix)
            {
                posicionsPossibles[nPosicions++] = pos;
                cami_actual.push_back(pos);
                getPosicionsPossiblesDames(pos, nPosicions, posicionsPossibles, 0, 0, 1, 0, 0, color, cami_actual, haCapturat);
            }
        }

        if (fila - 1 >= 0 && col - 1 >= 0 &&
            m_tauler[fila - 1][col - 1].getTipus() == TIPUS_EMPTY && flagAbaixEsquerra == 1)
        {
            Posicio pos = inttoString(fila - 1, col - 1);
            bool existeix = false;
            for (int i = 0; i < nPosicions; ++i)
                if (posicionsPossibles[i] == pos)
                    existeix = true;
            if (!existeix)
            {
                posicionsPossibles[nPosicions++] = pos;
                cami_actual.push_back(pos);
                getPosicionsPossiblesDames(pos, nPosicions, posicionsPossibles, 0, 0, 0, 1, 0, color, cami_actual, haCapturat);
            }
        }
    }

    // Captura Simple
    {
        // Adalt Dreta
        if (fila + 2 < 8 && col + 2 < 8 &&
            m_tauler[fila + 1][col + 1].getTipus() != TIPUS_EMPTY &&
            m_tauler[fila + 1][col + 1].getColor() != color &&
            m_tauler[fila + 2][col + 2].getTipus() == TIPUS_EMPTY
            && flagAdaltDreta == 1)
        {
            Posicio pos = inttoString(fila + 2, col + 2);
            bool existeix = false;
            for (int i = 0; i < nPosicions; ++i)
                if (posicionsPossibles[i] == pos)
                    existeix = true;
            if (!existeix)
            {
                posicionsPossibles[nPosicions++] = pos;
                cami_actual.push_back(pos);
                haCapturat = true;
                getPosicionsPossiblesDames(pos, nPosicions, posicionsPossibles, 0, 0, 0, 0, 1, color, cami_actual, haCapturat);
            }
        }

        // Adalt Esquerra
        if (fila + 2 < 8 && col - 2 >= 0 &&
            m_tauler[fila + 1][col - 1].getTipus() != TIPUS_EMPTY &&
            m_tauler[fila + 1][col - 1].getColor() != color &&
            m_tauler[fila + 2][col - 2].getTipus() == TIPUS_EMPTY &&
            flagAdaltEsquerra == 1)
        {
            Posicio pos = inttoString(fila + 2, col - 2);
            bool existeix = false;
            for (int i = 0; i < nPosicions; ++i)
                if (posicionsPossibles[i] == pos)
                    existeix = true;
            if (!existeix)
            {
                posicionsPossibles[nPosicions++] = pos;
                cami_actual.push_back(pos);
                haCapturat = true;
                getPosicionsPossiblesDames(pos, nPosicions, posicionsPossibles, 0, 0, 0, 0, 1, color, cami_actual, haCapturat);
            }
        }

        // Abaix Dreta
        if (fila - 2 >= 0 && col + 2 < 8 &&
            m_tauler[fila - 1][col + 1].getTipus() != TIPUS_EMPTY &&
            m_tauler[fila - 1][col + 1].getColor() != color &&
            m_tauler[fila - 2][col + 2].getTipus() == TIPUS_EMPTY &&
            flagAbaixDreta == 1)
        {
            Posicio pos = inttoString(fila - 2, col + 2);
            bool existeix = false;
            for (int i = 0; i < nPosicions; ++i)
                if (posicionsPossibles[i] == pos)
                    existeix = true;
            if (!existeix)
            {
                posicionsPossibles[nPosicions++] = pos;
                cami_actual.push_back(pos);
                haCapturat = true;
                getPosicionsPossiblesDames(pos, nPosicions, posicionsPossibles, 0, 0, 0, 0, 1, color, cami_actual, haCapturat);
            }
        }

        // Abaix Esquerra
        if (fila - 2 >= 0 && col - 2 >= 0 &&
            m_tauler[fila - 1][col - 1].getTipus() != TIPUS_EMPTY &&
            m_tauler[fila - 1][col - 1].getColor() != color &&
            m_tauler[fila - 2][col - 2].getTipus() == TIPUS_EMPTY && flagAbaixEsquerra == 1)
        {
            Posicio pos = inttoString(fila - 2, col - 2);
            bool existeix = false;
            for (int i = 0; i < nPosicions; ++i)
                if (posicionsPossibles[i] == pos)
                    existeix = true;
            if (!existeix)
            {
                posicionsPossibles[nPosicions++] = pos;
                cami_actual.push_back(pos);
                haCapturat = true;
                getPosicionsPossiblesDames(pos, nPosicions, posicionsPossibles, 0, 0, 0, 0, 1, color, cami_actual, haCapturat);
            }
        }
    }
    vector_posicions.push_back(cami_actual);
    cami_actual.clear();
}



void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[])
{
    int fila = origen.getFila();
    int col = origen.getCol();
    TipusFitxa tipus = m_tauler[fila][col].getTipus();
    ColorFitxa color = m_tauler[fila][col].getColor();
    bool haCapturat = false;
    nPosicions = 0;
    int fet = 0;
    int haMenjat = 0;
    vector<Posicio> cami_actual;
    if (tipus == TIPUS_DAMA)
    {
        getPosicionsPossiblesDames(origen, nPosicions, posicionsPossibles, 1, 1, 1, 1, 0, color, cami_actual, haCapturat);
    }
    else
        if (tipus == TIPUS_NORMAL)
        {
            getPosicionsPossiblesNorm(origen, nPosicions, posicionsPossibles, fet, color, cami_actual);
        }
}


bool Tauler::hiHaCapturaPossible(const Posicio& origen)
{
    int n = 0;
    Posicio posicionsPossibles[20];
    int haMenjat = 0;
    int fet = 0;
    bool haCapturat = false;
    ColorFitxa color = m_tauler[origen.getFila()][origen.getCol()].getColor();
    TipusFitxa tipus = m_tauler[origen.getFila()][origen.getCol()].getTipus();
    vector<Posicio> cami_actual;


    if (tipus == TIPUS_DAMA) {
        getPosicionsPossiblesDames(origen, n, posicionsPossibles, 1, 1, 1, 1, 0, color, cami_actual, haCapturat);
        if (haCapturat) return true;
    }
    else {
        if (color == COLOR_BLANC) {
            if (origen.getFila() + 2 < 8 && origen.getCol() + 2 < 8)
            {
                if (m_tauler[origen.getFila() + 2][origen.getCol() + 2].getTipus() == TIPUS_EMPTY &&
                    m_tauler[origen.getFila() + 1][origen.getCol() + 1].getTipus() != TIPUS_EMPTY &&
                    m_tauler[origen.getFila() + 1][origen.getCol() + 1].getColor() != color)
                {
                    return true;
                }
            }

            if (origen.getFila() + 2 < 8 && origen.getCol() - 2 >= 0)
            {
                if (m_tauler[origen.getFila() + 2][origen.getCol() - 2].getTipus() == TIPUS_EMPTY &&
                    m_tauler[origen.getFila() + 1][origen.getCol() - 1].getTipus() != TIPUS_EMPTY &&
                    m_tauler[origen.getFila() + 1][origen.getCol() - 1].getColor() != color)
                {
                    return true;
                }
            }
        }
        if (color == COLOR_NEGRE) {
            if (origen.getFila() - 2 >= 0 && origen.getCol() + 2 < 8)
            {
                if (m_tauler[origen.getFila() - 2][origen.getCol() + 2].getTipus() == TIPUS_EMPTY &&
                    m_tauler[origen.getFila() - 1][origen.getCol() + 1].getTipus() != TIPUS_EMPTY &&
                    m_tauler[origen.getFila() - 1][origen.getCol() + 1].getColor() != color)
                {
                    return true;
                }
            }

            if (origen.getFila() - 2 >= 0 && origen.getCol() - 2 >= 0)
            {
                if (m_tauler[origen.getFila() - 2][origen.getCol() - 2].getTipus() == TIPUS_EMPTY &&
                    m_tauler[origen.getFila() - 1][origen.getCol() - 1].getTipus() != TIPUS_EMPTY &&
                    m_tauler[origen.getFila() - 1][origen.getCol() - 1].getColor() != color)
                {
                    return true;
                }
            }
        }
    }
    return false;

}


bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti) {
    int n = 0;
    Posicio posicionsPossibles[20];
    int haMenjat = 0;
    int filaOrig = origen.getFila();
    int colOrig = origen.getCol();
    int filaDest = desti.getFila();
    int colDest = desti.getCol();
    bool haEliminat = false;
    bool bufar = true;
    ColorFitxa color = m_tauler[filaOrig][colOrig].getColor();
    TipusFitxa tipus = m_tauler[filaOrig][colOrig].getTipus();
    int fet = 0;
    vector<Posicio> cami_actual;
    bool hiHaviaCaptura = hiHaCapturaPossible(origen); 
    bool haCapturat = false;

    if (tipus == TIPUS_DAMA) {
        getPosicionsPossiblesDames(origen, n, posicionsPossibles, 1, 1, 1, 1, 0, color, cami_actual, haCapturat);
    }
    else
        getPosicionsPossiblesNorm(origen, n, posicionsPossibles, fet, color, cami_actual);

    for (int i = 0; i < n; i++) {
        if (desti == posicionsPossibles[i]) {
            if (tipus == TIPUS_NORMAL) {

                bool esCapturaSimple = (abs(filaDest - filaOrig) == 2 && abs(colDest - colOrig) == 2);
                
                if (esCapturaSimple) {
                    EliminarFitxaNormal1(origen, desti);
                    bufar = false;
                    haEliminat = true;
                }

                if (!esCapturaSimple &&
                    desti != inttoString(filaOrig + 1, colOrig + 1) &&
                    desti != inttoString(filaOrig - 1, colOrig - 1) &&
                    desti != inttoString(filaOrig + 1, colOrig - 1) &&
                    desti != inttoString(filaOrig - 1, colOrig + 1)) {
                    EliminaFitxaNormal2(origen, desti, posicionsPossibles);
                    bufar = false;
                    haEliminat = true;
                }

                if (bufar) {
                    if (hiHaviaCaptura) {
                        m_tauler[filaOrig][colOrig].setTipus(TIPUS_EMPTY);
                        return false;
                    }
                    else{
                        for (int i = 0; i < 8; i++)
                        {
                            for (int j = 0; j < 8; j++)
                            {
                                    Posicio pos = inttoString(i, j);
                                    bool hihaCapturaNorm = hiHaCapturaPossible(pos);
                                    if (hihaCapturaNorm && m_tauler[i][j].getColor() == color)
                                    {
                                        m_tauler[i][j].setTipus(TIPUS_EMPTY);
                                    }
                            }         
                        }   
                    }
                }
                m_tauler[filaDest][colDest] = m_tauler[filaOrig][colOrig];
                m_tauler[filaOrig][colOrig].setTipus(TIPUS_EMPTY);
                if (hiHaCapturaPossible(desti) && haEliminat)
                {
                    m_tauler[desti.getFila()][desti.getCol()].setTipus(TIPUS_EMPTY);
                }



                if ((filaDest == 7 && color == COLOR_BLANC) || (filaDest == 0 && color == COLOR_NEGRE)) {
                    m_tauler[filaDest][colDest].setTipus(TIPUS_DAMA);
                }

                return true;
            }
            else {
                bool bufar = true;

                m_tauler[filaDest][colDest] = m_tauler[filaOrig][colOrig];
                m_tauler[filaOrig][colOrig].setTipus(TIPUS_EMPTY);

                if (haCapturat) {
                    eliminaFitxaDama(origen, desti, color);
                    bufar = false;
                    haEliminat = true;
                    if (hiHaCapturaPossible(desti) && haEliminat)
                    {
                        m_tauler[desti.getFila()][desti.getCol()].setTipus(TIPUS_EMPTY);
                    }
                }


                if (bufar) {
                        if (!hiHaCapturaPossible(desti))
                        {
                            for (int i = 0; i < 8; i++)
                            {
                                for (int j = 0; j < 8; j++)
                                {                                 
                                    Posicio pos = inttoString(i, j);
                                    bool hihaCaptura = hiHaCapturaPossible(pos);
                                    if (hihaCaptura && m_tauler[i][j].getColor() == color)
                                    {
                                        m_tauler[i][j].setTipus(TIPUS_EMPTY);
                                    } 
                                }

                            }
                        }
                    }
                return true;
                }
                
            }
        }
        return false;
}

void Tauler::EliminarFitxaNormal1(const Posicio origen, Posicio desti)
{
    int filaMenjada = (origen.getFila() + desti.getFila()) / 2;
    int colMenjada = (origen.getCol() + desti.getCol()) / 2;

    if (abs(desti.getFila() - origen.getFila()) == 2 && abs(desti.getCol() - origen.getCol()) == 2)
    {
        ColorFitxa colorOrigen = m_tauler[origen.getFila()][origen.getCol()].getColor();
        ColorFitxa colorMig = m_tauler[filaMenjada][colMenjada].getColor();

        if (m_tauler[filaMenjada][colMenjada].getTipus() != TIPUS_EMPTY &&
            colorOrigen != colorMig)
        {
            m_tauler[filaMenjada][colMenjada].setTipus(TIPUS_EMPTY);
        }
    }
}



void Tauler::EliminaFitxaNormal2(Posicio origen, Posicio desti, Posicio PosicionsPossibles[])
{
    ColorFitxa color = m_tauler[origen.getFila()][origen.getCol()].getColor();
    for (size_t i = 0; i < vector_posicions.size(); ++i) {
        //Es selecciona l'ultim element, no s'ha de bufar
        if (!vector_posicions[i].empty() && desti == vector_posicions[i].back()) {
            vector_posicions[i].insert(vector_posicions[i].begin(), origen);
            int j = vector_posicions[i].size() - 1;
            while (j > 0) {
                //Hi ha Captura
                if (abs(vector_posicions[i][j].getFila() - vector_posicions[i][j - 1].getFila()) == 2 &&
                    abs(vector_posicions[i][j].getCol() - vector_posicions[i][j - 1].getCol()) == 2)
                {
                    if (color == COLOR_NEGRE) {
                        if (vector_posicions[i][j].getFila() > vector_posicions[i][j - 1].getFila() &&
                            vector_posicions[i][j].getCol() > vector_posicions[i][j - 1].getCol())
                        {
                            m_tauler[vector_posicions[i][j].getFila() - 1][vector_posicions[i][j].getCol() - 1].setTipus(TIPUS_EMPTY);
                        }
                        else
                            if (vector_posicions[i][j].getFila() > vector_posicions[i][j - 1].getFila() &&
                                vector_posicions[i][j].getCol() < vector_posicions[i][j - 1].getCol()) {
                                m_tauler[vector_posicions[i][j].getFila() - 1][vector_posicions[i][j].getCol() + 1].setTipus(TIPUS_EMPTY);
                            }
                    }
                    else {
                        if (vector_posicions[i][j].getFila() < vector_posicions[i][j - 1].getFila() &&
                            vector_posicions[i][j].getCol() > vector_posicions[i][j - 1].getCol()) {
                            m_tauler[vector_posicions[i][j - 1].getFila() - 1][vector_posicions[i][j].getCol() - 1].setTipus(TIPUS_EMPTY);
                        }
                        else
                            if (vector_posicions[i][j].getFila() < vector_posicions[i][j - 1].getFila() &&
                                vector_posicions[i][j].getCol() < vector_posicions[i][j - 1].getCol()) {
                                m_tauler[vector_posicions[i][j - 1].getFila() - 1][vector_posicions[i][j - 1].getCol() - 1].setTipus(TIPUS_EMPTY);
                            }
                    }
                    j--;
                }
                else {
                    j--;
                }
            }
        }
    }
    bool heEntrat = false;
    for (int i = 0; i < vector_posicions.size(); i++)
    {
        for (int j = 0; j < vector_posicions[i].size(); j++)
        {
            if (vector_posicions[i][j] == desti)
            {
                while (j > 0) {
                    //Hi ha Captura
                    if (abs(vector_posicions[i][j].getFila() - vector_posicions[i][j - 1].getFila()) == 2 &&
                        abs(vector_posicions[i][j].getCol() - vector_posicions[i][j - 1].getCol()) == 2)
                    {
                        if (vector_posicions[i][j].getFila() > vector_posicions[i][j - 1].getFila() &&
                            vector_posicions[i][j].getCol() > vector_posicions[i][j - 1].getCol())
                        {
                            m_tauler[vector_posicions[i][j].getFila() - 1][vector_posicions[i][j].getCol() - 1].setTipus(TIPUS_EMPTY);
                        }
                        else
                            if (vector_posicions[i][j].getFila() > vector_posicions[i][j - 1].getFila() &&
                                vector_posicions[i][j].getCol() < vector_posicions[i][j - 1].getCol()) {
                                m_tauler[vector_posicions[i][j].getFila() - 1][vector_posicions[i][j - 1].getCol() - 1].setTipus(TIPUS_EMPTY);
                            }
                            else
                                if (vector_posicions[i][j].getFila() < vector_posicions[i][j - 1].getFila() &&
                                    vector_posicions[i][j].getCol() > vector_posicions[i][j - 1].getCol()) {
                                    m_tauler[vector_posicions[i][j - 1].getFila() - 1][vector_posicions[i][j].getCol() - 1].setTipus(TIPUS_EMPTY);
                                }
                                else
                                    if (vector_posicions[i][j].getFila() < vector_posicions[i][j - 1].getFila() &&
                                        vector_posicions[i][j].getCol() < vector_posicions[i][j - 1].getCol()) {
                                        m_tauler[vector_posicions[i][j - 1].getFila() - 1][vector_posicions[i][j - 1].getCol() - 1].setTipus(TIPUS_EMPTY);
                                    }
                        j--;
                    }
                    else {
                        j--;
                    }
                    heEntrat = true;
                }

            }
            if (heEntrat)
            {
                break;
            }
        }
        if (heEntrat)
        {
            break;
        }
    }
}


void Tauler::eliminaFitxaDama(const Posicio origen, const Posicio desti, ColorFitxa color)
{
    cout << "he entrat" << endl;
    for (size_t i = 0; i < vector_posicions.size(); ++i) {
        //Es selecciona l'ultim element, no s'ha de bufar
        if (!vector_posicions[i].empty() && desti == vector_posicions[i].back()) {
            cout << "He entrat al if" << endl;
            vector_posicions[i].insert(vector_posicions[i].begin(), origen);
            int j = vector_posicions[i].size() - 1;
            while (j > 0) {
                //Hi ha Captura
                cout << "He entrat al while" << endl;
                if (abs(vector_posicions[i][j].getFila() - vector_posicions[i][j - 1].getFila()) == 2 &&
                    abs(vector_posicions[i][j].getCol() - vector_posicions[i][j - 1].getCol()) == 2)
                {
                    if (vector_posicions[i][j].getFila() > vector_posicions[i][j - 1].getFila() &&
                        vector_posicions[i][j].getCol() > vector_posicions[i][j - 1].getCol())
                    {
                        m_tauler[vector_posicions[i][j].getFila() - 1][vector_posicions[i][j].getCol() - 1].setTipus(TIPUS_EMPTY);
                    }
                    else
                        if (vector_posicions[i][j].getFila() > vector_posicions[i][j - 1].getFila() &&
                            vector_posicions[i][j].getCol() < vector_posicions[i][j - 1].getCol()) {
                            m_tauler[vector_posicions[i][j].getFila() - 1][vector_posicions[i][j - 1].getCol() - 1].setTipus(TIPUS_EMPTY);
                        }
                        else
                            if (vector_posicions[i][j].getFila() < vector_posicions[i][j - 1].getFila() &&
                                vector_posicions[i][j].getCol() > vector_posicions[i][j - 1].getCol()) {
                                m_tauler[vector_posicions[i][j - 1].getFila() - 1][vector_posicions[i][j].getCol() - 1].setTipus(TIPUS_EMPTY);
                            }
                            else
                                if (vector_posicions[i][j].getFila() < vector_posicions[i][j - 1].getFila() &&
                                    vector_posicions[i][j].getCol() < vector_posicions[i][j - 1].getCol()) {
                                    m_tauler[vector_posicions[i][j - 1].getFila() - 1][vector_posicions[i][j - 1].getCol() - 1].setTipus(TIPUS_EMPTY);
                                }
                    j--;
                }
                else {
                    j--;
                }
            }
        }
    }
    bool heEntrat = false;
    for (int i = 0; i < vector_posicions.size(); i++)
    {
        for (int j = 0; j < vector_posicions[i].size(); j++)
        {
            if (vector_posicions[i][j] == desti)
            {
                while (j > 0) {
                    //Hi ha Captura
                    if (abs(vector_posicions[i][j].getFila() - vector_posicions[i][j - 1].getFila()) == 2 &&
                        abs(vector_posicions[i][j].getCol() - vector_posicions[i][j - 1].getCol()) == 2)
                    {
                        if (vector_posicions[i][j].getFila() > vector_posicions[i][j - 1].getFila() &&
                            vector_posicions[i][j].getCol() > vector_posicions[i][j - 1].getCol())
                        {
                            m_tauler[vector_posicions[i][j].getFila() - 1][vector_posicions[i][j].getCol() - 1].setTipus(TIPUS_EMPTY);
                        }
                        else
                            if (vector_posicions[i][j].getFila() > vector_posicions[i][j - 1].getFila() &&
                                vector_posicions[i][j].getCol() < vector_posicions[i][j - 1].getCol()) {
                                m_tauler[vector_posicions[i][j].getFila() - 1][vector_posicions[i][j - 1].getCol() - 1].setTipus(TIPUS_EMPTY);
                            }
                            else
                                if (vector_posicions[i][j].getFila() < vector_posicions[i][j - 1].getFila() &&
                                    vector_posicions[i][j].getCol() > vector_posicions[i][j - 1].getCol()) {
                                    m_tauler[vector_posicions[i][j - 1].getFila() - 1][vector_posicions[i][j].getCol() - 1].setTipus(TIPUS_EMPTY);
                                }
                                else
                                    if (vector_posicions[i][j].getFila() < vector_posicions[i][j - 1].getFila() &&
                                        vector_posicions[i][j].getCol() < vector_posicions[i][j - 1].getCol()) {
                                        m_tauler[vector_posicions[i][j - 1].getFila() - 1][vector_posicions[i][j - 1].getCol() - 1].setTipus(TIPUS_EMPTY);
                                    }
                        j--;
                    }
                    else {
                        j--;
                    }
                    heEntrat = true;
                }

            }
            if (heEntrat)
            {
                break;
            }
        }
        if (heEntrat)
        {
            break;
        }
    }
}
