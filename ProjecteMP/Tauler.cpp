#include "Tauler.h"
#include <fstream>
#include <sstream>


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


void Tauler::getPosicionsPossiblesNorm(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[], int fet, ColorFitxa Color)
{
    int fila = origen.getFila();
    int col = origen.getCol();

    int direccio = (Color == COLOR_BLANC) ? 1 : -1;

    if (fila + direccio >= 0 && fila + direccio < 8 && col + 1 < 8) {
        if (m_tauler[fila + direccio][col + 1].getTipus() == TIPUS_EMPTY && fet == 0) {
            string posicio = inttoString(fila + direccio, col + 1);
            posicionsPossibles[nPosicions] = Posicio(posicio);
            nPosicions++;
        }
        else if (fila + 2 * direccio >= 0 && fila + 2 * direccio < 8 && col + 2 < 8
            && m_tauler[fila + 2 * direccio][col + 2].getTipus() == TIPUS_EMPTY
            && m_tauler[fila + direccio][col + 1].getColor() != Color) {
            string posicio = inttoString(fila + 2 * direccio, col + 2);
            posicionsPossibles[nPosicions] = Posicio(posicio);
            nPosicions++;
            fet++;
            getPosicionsPossiblesNorm(Posicio(posicio), nPosicions, posicionsPossibles, fet, Color);
            
        }
    }

    if (fila + direccio >= 0 && fila + direccio < 8 && col - 1 >= 0) {
        if (m_tauler[fila + direccio][col - 1].getTipus() == TIPUS_EMPTY && fet == 0) {
            string posicio = inttoString(fila + direccio, col - 1);
            posicionsPossibles[nPosicions] = Posicio(posicio);
            nPosicions++;
        }
        else if (fila + 2 * direccio >= 0 && fila + 2 * direccio < 8 && col - 2 >= 0
            && m_tauler[fila + 2 * direccio][col - 2].getTipus() == TIPUS_EMPTY
            && m_tauler[fila + direccio][col - 1].getColor() != Color) {
            string posicio = inttoString(fila + 2 * direccio, col - 2);
            posicionsPossibles[nPosicions] = Posicio(posicio);
            nPosicions++;
            fet++;
            getPosicionsPossiblesNorm(Posicio(posicio), nPosicions, posicionsPossibles, fet, Color);
        }
    }
}




void Tauler::getPosicionsPossiblesDames(const Posicio origen, int& nPosicions, Posicio posicionsPossibles[], bool despresMenjar, bool daldreta, bool dalsquerra, bool baixdreta, bool baixsquerra, int& haMenjat, ColorFitxa color) {
    int fila = origen.getFila();
    int col = origen.getCol();
    if (col >= 0 && col < 8 && fila >= 0 && fila < 8) {
        if (despresMenjar)
        {
            for (int i = 0; i < N_FILES; i++)
            {
                if (fila + 1 + i >= 0 && col + 1 + i >= 0) {
                    // Dalt dreta
                    if (
                        m_tauler[fila + i][col + i].getTipus() != TIPUS_EMPTY &&
                        m_tauler[fila + i][col + i].getColor() != m_tauler[fila][col].getColor() &&
                        m_tauler[fila + i + 1][col + i + 1].getTipus() == TIPUS_EMPTY)
                    {
                        daldreta = true;
                    }
                }
                if (fila + 1 + i < 8 && col - 1 - i >= 0) {

                    // Dalt esquerra
                    if (
                        m_tauler[fila + i][col - i].getTipus() != TIPUS_EMPTY &&
                        m_tauler[fila + i][col - i].getColor() != m_tauler[fila][col].getColor() &&
                        m_tauler[fila + i + 1][col - i - 1].getTipus() == TIPUS_EMPTY)
                    {
                        dalsquerra = true;

                    }
                }
                if (fila - 1 - i >= 0 && col + 1 + i < 8)
                    // Baix dreta
                    if (
                        m_tauler[fila - i][col + i].getTipus() != TIPUS_EMPTY &&
                        m_tauler[fila - i][col + i].getColor() != m_tauler[fila][col].getColor() &&
                        m_tauler[fila - i - 1][col + i + 1].getTipus() == TIPUS_EMPTY)
                    {
                        baixdreta = true;
                    }

                if (fila - 1 - i >= 0 && col - 1 - i >= 0) {
                    // Baix esquerra
                    if (
                        m_tauler[fila - i][col - i].getTipus() != TIPUS_EMPTY &&
                        m_tauler[fila - i][col - i].getColor() != m_tauler[fila][col].getColor() &&
                        m_tauler[fila - i - 1][col - i - 1].getTipus() == TIPUS_EMPTY)
                    {
                        baixsquerra = true;
                    }
                }
            }
        }

        //abaix a la dreta
        if (m_tauler[fila - 1][col + 1].getTipus() == TIPUS_EMPTY && fila - 1 >= 0 && col + 1 < 8 && baixdreta)
        {
            string posicio = inttoString(fila - 1, col + 1);
            posicionsPossibles[nPosicions] = Posicio(posicio);
            nPosicions++;
            getPosicionsPossiblesDames(Posicio(posicio), nPosicions, posicionsPossibles, 0, 0, 0, 1, 0, haMenjat, color);
        }

        //adalt a la dreta
        if (m_tauler[fila + 1][col + 1].getTipus() == TIPUS_EMPTY && fila + 1 < 8 && col + 1 < 8 && daldreta)
        {
            string posicio = inttoString(fila + 1, col + 1);
            posicionsPossibles[nPosicions] = Posicio(posicio);
            nPosicions++;
            getPosicionsPossiblesDames(Posicio(posicio), nPosicions, posicionsPossibles, 0, 1, 0, 0, 0, haMenjat, color);
        }



        //a baix esquerra
        if (m_tauler[fila - 1][col - 1].getTipus() == TIPUS_EMPTY && fila - 1 >= 0 && col - 1 >= 0 && baixsquerra)
        {
            string posicio = inttoString(fila - 1, col - 1);
            posicionsPossibles[nPosicions] = Posicio(posicio);
            nPosicions++;
            getPosicionsPossiblesDames(Posicio(posicio), nPosicions, posicionsPossibles, 0, 0, 0, 0, 1, haMenjat, color);
        }

        //adalt a l'esquerra
        if (m_tauler[fila + 1][col - 1].getTipus() == TIPUS_EMPTY && fila + 1 < 8 && col - 1 >= 0 && dalsquerra)
        {
            string posicio = inttoString(fila + 1, col - 1);
            posicionsPossibles[nPosicions] = Posicio(posicio);
            nPosicions++;
            getPosicionsPossiblesDames(Posicio(posicio), nPosicions, posicionsPossibles, 0, 0, 1, 0, 0, haMenjat, color);
        }


        if (col + 2 < 8 && fila + 2 < 8)
        {
            //menjar DalDreta
            if (m_tauler[fila + 1][col + 1].getTipus() != TIPUS_EMPTY &&
                m_tauler[fila + 1][col + 1].getColor() != color &&
                m_tauler[fila + 2][col + 2].getTipus() == TIPUS_EMPTY
                && daldreta)
            {
                string pos = inttoString(fila + 2, col + 2);
                Posicio posicio = Posicio(pos);
                posicionsPossibles[nPosicions] = posicio;
                nPosicions++;
                haMenjat++;
                getPosicionsPossiblesDames(posicio, nPosicions, posicionsPossibles, 1, 0, 0, 0, 0, haMenjat, color);
            }
        }
        //menjar baixdreta
        if (col + 2 < 8 && fila - 2 >= 0) {
            if (m_tauler[fila - 1][col + 1].getTipus() != TIPUS_EMPTY &&
                m_tauler[fila - 1][col + 1].getColor() != color &&
                m_tauler[fila - 2][col + 2].getTipus() == TIPUS_EMPTY &&
                baixdreta)
            {
                string pos = inttoString(fila - 2, col + 2);
                Posicio posicio = Posicio(pos);
                posicionsPossibles[nPosicions] = posicio;
                nPosicions++;
                haMenjat++;
                getPosicionsPossiblesDames(posicio, nPosicions, posicionsPossibles, 1, 0, 0, 1, 0, haMenjat, color);
            }
        }



        //menjar dalsquerra



        //menjar baixsquerra

    }
}

void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[])
{
    int fila = origen.getFila();
    int col = origen.getCol();
    TipusFitxa tipus = m_tauler[fila][col].getTipus();
    ColorFitxa color = m_tauler[fila][col].getColor();
    nPosicions = 0;
    int fet = 0;
    int haMenjat = 0;
    if (tipus == TIPUS_DAMA)
    {
        getPosicionsPossiblesDames(origen, nPosicions, posicionsPossibles, 0, 1, 1, 1, 1, haMenjat, color);
    }
    else
        if (tipus == TIPUS_NORMAL)
        {
            getPosicionsPossiblesNorm(origen, nPosicions, posicionsPossibles, fet, color);
        }
}

bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti) {
    
    int n = 0;
    Posicio posicionsPossibles[12];
    int haMenjat = 0;
    int filaOrig = origen.getFila();
    int colOrig = origen.getCol();
    int filaDest = desti.getFila();
    int colDest = desti.getCol();
    ColorFitxa color = m_tauler[filaOrig][colOrig].getColor();
    TipusFitxa tipus = m_tauler[filaOrig][colOrig].getTipus();
    
    if (tipus == TIPUS_DAMA)
    {
        getPosicionsPossiblesDames(origen, n, posicionsPossibles, 0, 1, 1, 1, 1, haMenjat, color);
    }
    else {
        getPosicionsPossiblesNorm(origen, n, posicionsPossibles, 0, color);
    }

    for (int i = 0; i < n; i++) {
        if (desti == posicionsPossibles[i]) {


            m_tauler[filaDest][colDest] = m_tauler[filaOrig][colOrig];
            m_tauler[filaOrig][colOrig].setTipus(TIPUS_EMPTY);

            if (tipus == TIPUS_NORMAL)
            {
                if (desti != origen && Posicio(inttoString(filaOrig + 1, colOrig + 1)) != desti &&
                    Posicio(inttoString(filaOrig + 1, colOrig - 1)) != desti &&
                    Posicio(inttoString(filaOrig - 1, colOrig + 1)) != desti &&
                    Posicio(inttoString(filaOrig - 1, colOrig - 1)) != desti)
                {
                    EliminaFitxes(origen, desti, posicionsPossibles);
                }
            }

                
            else {
                if (haMenjat > 0)
                {
                    EliminaFitxes(origen, desti, posicionsPossibles);
                }
            }

        if(haMenjat > 0)
            {
                if (bufar(desti))
                {
                    m_tauler[filaDest][colDest].setTipus(TIPUS_EMPTY);
                }
            }
        else {
            bufar2(color);
        }
            
        

            if ((filaDest == 7 && m_tauler[filaDest][colDest].getColor() == COLOR_BLANC) ||
                (filaDest == 0 && m_tauler[filaDest][colDest].getColor() == COLOR_NEGRE)) {
                m_tauler[filaDest][colDest].setTipus(TIPUS_DAMA);
            }

            actualitzaMovimentsValids();
            return true;

        }
    }
    return false;
}



void Tauler::actualitzaMovimentsValids()
{
    for (int i = 0; i < N_FILES; i++)
    {
        for (int j = 0; j < N_COLUMNES; j++)
        {
            if (m_tauler[i][j].getTipus() != TIPUS_EMPTY) {
                Posicio posicioActual(inttoString(i, j));
                int nPos = 0;
                Posicio posicionsPossibles[15];
                getPosicionsPossibles(posicioActual, nPos, posicionsPossibles);
            }
        }
    }
}


void Tauler::EliminaFitxes(Posicio origen, Posicio desti, Posicio PosicionsPossibles[])
{
    int col = desti.getCol();
    int fila = desti.getFila();
    Posicio posicioAct = Posicio(desti);
    if (m_tauler[fila][col].getTipus() == TIPUS_DAMA) {
        while (posicioAct != origen)
        {
            for (int i = 0; i < N_FILES; i++)
            {
                bool sortir_bucle = false;
                for (int j = 0; j < N_FILES; j++)
                {
                    if (fila + i + 1 < 8 && col + i + 1 < 8 && (Posicio(inttoString(fila + i + 1, col + i + 1)) == PosicionsPossibles[j] || Posicio(inttoString(fila + i + 1, col + i + 1)) == origen))
                    {
                        if(m_tauler[fila+i][col+i].getTipus() != TIPUS_EMPTY)
                        {
                            m_tauler[fila + i][col + i].setTipus(TIPUS_EMPTY);
                        }
                        posicioAct = inttoString(fila + i + 1, col + i + 1);
                        fila = posicioAct.getFila();
                        col = posicioAct.getCol();
                        sortir_bucle = true;
                        break;
                    }
                    if (fila + i + 1 < 8 && col - i - 1 >= 0 && (Posicio(inttoString(fila + i + 1, col - i - 1)) == PosicionsPossibles[j] || Posicio(inttoString(fila + i + 1, col - i - 1)) == origen))
                    {
                        if (m_tauler[fila + i][col - i].getTipus() != TIPUS_EMPTY)
                        {
                            m_tauler[fila + i][col - i].setTipus(TIPUS_EMPTY);
                        }
                        posicioAct = inttoString(fila + i + 1, col - i - 1);
                        fila = posicioAct.getFila();
                        col = posicioAct.getCol();
                        sortir_bucle = true;
                        break;

                    }
                    if (fila - i - 1 >= 0 && col + i + 1 < 8 && (Posicio(inttoString(fila - i - 1, col + i + 1)) == PosicionsPossibles[j] || Posicio(inttoString(fila - i - 1, col + i + 1)) == origen))
                    {
                        if (m_tauler[fila - i][col + i].getTipus() != TIPUS_EMPTY)
                        {
                            m_tauler[fila - i][col + i].setTipus(TIPUS_EMPTY);
                        }
                        posicioAct = inttoString(fila - i - 1, col + i + 1);
                        fila = posicioAct.getFila();
                        col = posicioAct.getCol();
                        sortir_bucle = true;
                        break;

                    }
                    if (fila - i - 1 >= 0 && col - i - 1 >= 0 && (Posicio(inttoString(fila - i - 1, col - i - 1)) == PosicionsPossibles[j] || Posicio(inttoString(fila - i - 1, col - i - 1)) == origen))
                    {
                        if (m_tauler[fila - i][col - i].getTipus() != TIPUS_EMPTY)
                        {
                            m_tauler[fila - i][col - i].setTipus(TIPUS_EMPTY);
                        }
                        posicioAct = inttoString(fila - i - 1, col - i - 1);
                        fila = posicioAct.getFila();
                        col = posicioAct.getCol();
                        sortir_bucle = true;
                        break;

                    }

                }
                if (sortir_bucle == true)
                {
                    break;
                }
            }
        }
    }
    else {
        while (posicioAct != origen) {
            for (int j = 0; j < N_FILES; j++)
            {
                if (fila + 2 < 8 && col + 2 < 8 && (Posicio(inttoString(fila + 2, col + 2)) == PosicionsPossibles[j] || Posicio(inttoString(fila + 2, col + 2)) == origen))
                {
                    m_tauler[fila + 1][col + 1].setTipus(TIPUS_EMPTY);
                    posicioAct = inttoString(fila + 2, col + 2);
                }
                if (fila + 2 < 8 && col - 2 >= 0 && (Posicio(inttoString(fila + 2, col - 2)) == PosicionsPossibles[j] || Posicio(inttoString(fila + 2, col - 2)) == origen))
                {
                    m_tauler[fila + 1][col - 1].setTipus(TIPUS_EMPTY);
                    posicioAct = inttoString(fila + 2, col - 2);
                }
                if (fila - 2 >= 0 && col + 2 < 8 && (Posicio(inttoString(fila - 2, col + 2)) == PosicionsPossibles[j] || Posicio(inttoString(fila - 2, col + 2)) == origen))
                {
                    m_tauler[fila - 1][col + 1].setTipus(TIPUS_EMPTY);
                    posicioAct = inttoString(fila - 2, col + 2);
                }
                if (fila - 2 >= 0 && col - 2 >= 0 && (Posicio(inttoString(fila - 2, col - 2)) == PosicionsPossibles[j] || Posicio(inttoString(fila - 2, col - 2)) == origen))
                {
                    m_tauler[fila - 1][col - 1].setTipus(TIPUS_EMPTY);
                    posicioAct = inttoString(fila - 2, col - 2);
                }
                fila = posicioAct.getFila();
                col = posicioAct.getCol();
            }
        }
    }

}




bool Tauler::bufar(Posicio pos)
{
    int col = pos.getCol();
    int fila = pos.getFila();
    TipusFitxa tipus = m_tauler[fila][col].getTipus();
    ColorFitxa color = m_tauler[fila][col].getColor();
    if (tipus == TIPUS_DAMA)
    {
        for (int i = 0; i < N_FILES; i++)
        {
            if (fila + i + 1 < 8 && col + i + 1 < 8 &&
                m_tauler[fila + i][col + i].getTipus() != TIPUS_EMPTY &&
                m_tauler[fila + i][col + i].getColor() != color &&
                m_tauler[fila + i + 1][col + i + 1].getTipus() == TIPUS_EMPTY) {
                return true;
            }
            else
                if (fila + i + 1 < 8 && col - i - 1 >= 0 &&
                    m_tauler[fila + i][col - i].getTipus() != TIPUS_EMPTY &&
                    m_tauler[fila + i][col - i].getColor() != color &&
                    m_tauler[fila + 1 + i][col - i - 1].getTipus() == TIPUS_EMPTY) {
                    return true;
                }
                else
                    if (fila - i - 1 >= 0 && col + i + 1 < 8 && 
                        m_tauler[fila - i][col + i].getTipus() != TIPUS_EMPTY && 
                        m_tauler[fila - i][col + i].getColor() != color &&
                        m_tauler[fila - 1 - i][col + i + 1].getTipus() == TIPUS_EMPTY) {
                        return true;
                    }
                    else
                        if (fila - i - 1 >= 0 && col - i - 1 >= 0 &&
                            m_tauler[fila - i][col - i].getTipus() != TIPUS_EMPTY &&
                            m_tauler[fila - i][col - i].getColor() != color &&
                            m_tauler[fila - 1 - i][col - i - 1].getTipus() == TIPUS_EMPTY) {
                            return true;
                        }
               
        }

    }
    else {
        if (color == COLOR_BLANC)
        {
            if (fila + 1 < 8 && col + 1 < 8 && m_tauler[fila + 1][col + 1].getTipus() != TIPUS_EMPTY && 
                m_tauler[fila + 1][col + 1].getColor() != color  &&
                m_tauler[fila+2][col+2].getTipus() == TIPUS_EMPTY) {
                return true;
            }
            else
                if (fila + 1 < 8 && col - 1 >= 0 && m_tauler[fila + 1][col - 1].getTipus() != TIPUS_EMPTY && 
                    m_tauler[fila + 1][col - 1].getColor() != color &&
                    m_tauler[fila + 2][col - 2].getTipus() == TIPUS_EMPTY) {
                    return true;
                }
        }
        else
        {
            if (fila - 1 >= 0 && col + 1 < 8 && m_tauler[fila - 1][col + 1].getTipus() != TIPUS_EMPTY && 
                m_tauler[fila - 1][col + 1].getColor() != color &&
                m_tauler[fila - 2][col + 2].getTipus() == TIPUS_EMPTY) {
                return true;
            }
            else
                if (fila - 1 >= 0 && col - 1 >= 0 && m_tauler[fila - 1][col - 1].getTipus() != TIPUS_EMPTY && 
                    m_tauler[fila - 1][col - 1].getColor() != color &&
                    m_tauler[fila - 2][col - 2].getTipus() == TIPUS_EMPTY) {
                    return true;
                }
        }



    }
    return false;
}

void Tauler::bufar2(ColorFitxa color)
{
    for (int i = 0; i < N_FILES; i++)
    {
        for (int j = 0; j < N_COLUMNES; j++)
        {
            if (m_tauler[i][j].getTipus() == TIPUS_NORMAL)
            {
                if (m_tauler[i][j].getColor() == COLOR_BLANC)
                {
                    if (i + 1 < 8 && j + 1 < 8 &&
                        m_tauler[i][j].getColor() == color &&
                        m_tauler[i + 1][j + 1].getTipus() != TIPUS_EMPTY &&
                        m_tauler[i + 2][j + 2].getTipus() == TIPUS_EMPTY &&
                        m_tauler[i + 1][j + 1].getColor() != color)
                    {
                        m_tauler[i][j].setTipus(TIPUS_EMPTY);
                    }

                    if (i + 1 < 8 && j - 1 >= 0 &&
                        m_tauler[i][j].getColor() == color &&
                        m_tauler[i + 1][j - 1].getTipus() != TIPUS_EMPTY &&
                        m_tauler[i + 2][j - 2].getTipus() == TIPUS_EMPTY &&
                        m_tauler[i + 1][j - 1].getColor() != color)
                    {
                        m_tauler[i][j].setTipus(TIPUS_EMPTY);
                    }
                }
                else {
                    if (i - 1 >= 0 && j + 1 < 8 &&
                        m_tauler[i][j].getColor() == color &&
                        m_tauler[i - 1][j + 1].getTipus() != TIPUS_EMPTY &&
                        m_tauler[i - 2][j + 2].getTipus() == TIPUS_EMPTY &&
                        m_tauler[i - 1][j + 1].getColor() != color)
                    {
                        m_tauler[i][j].setTipus(TIPUS_EMPTY);
                    }

                    if (i - 1 >= 0 && j - 1 >= 0 &&
                        m_tauler[i][j].getColor() == color &&
                        m_tauler[i - 1][j - 1].getTipus() != TIPUS_EMPTY &&
                        m_tauler[i - 2][j - 2].getTipus() == TIPUS_EMPTY &&
                        m_tauler[i - 1][j - 1].getColor() != color)
                    {
                        m_tauler[i][j].setTipus(TIPUS_EMPTY);
                    }
                }
            }

            else
            {
                if (m_tauler[i][j].getTipus() == TIPUS_DAMA)
                {
                    for (int k = 0; k < N_FILES; k++)
                    {
                        if (i + k + 1 < 8 && j + k + 1 < 8 &&
                            m_tauler[i][j].getColor() == color &&
                            m_tauler[i + k][j + k].getTipus() != TIPUS_EMPTY &&
                            m_tauler[i + k][j + k].getColor() != color &&
                            m_tauler[i + k + 1][j + k + 1].getTipus() == TIPUS_EMPTY)
                        {
                            m_tauler[i][j].setTipus(TIPUS_EMPTY);
                        }

                        if (i + k + 1 < 8 && j - k - 1 >= 0 &&
                            m_tauler[i][j].getColor() == color &&
                            m_tauler[i + k][j - k].getTipus() != TIPUS_EMPTY &&
                            m_tauler[i + k][j - k].getColor() != color &&
                            m_tauler[i + k + 1][j - k - 1].getTipus() == TIPUS_EMPTY)
                        {
                            m_tauler[i][j].setTipus(TIPUS_EMPTY);
                        }

                        if (i - k - 1 >= 0 && j + k + 1 < 8 &&
                            m_tauler[i][j].getColor() == color &&
                            m_tauler[i - k][j + k].getTipus() != TIPUS_EMPTY &&
                            m_tauler[i - k][j + k].getColor() != color &&
                            m_tauler[i - k - 1][j + k + 1].getTipus() == TIPUS_EMPTY)
                        {
                            m_tauler[i][j].setTipus(TIPUS_EMPTY);
                        }


                        if (i - k - 1 >= 0 && j - k - 1 >= 0 &&
                            m_tauler[i][j].getColor() == color &&
                            m_tauler[i - k][j - k].getTipus() != TIPUS_EMPTY &&
                            m_tauler[i - k][j - k].getColor() != color &&
                            m_tauler[i - k - 1][j - k - 1].getTipus() == TIPUS_EMPTY)
                        {
                            m_tauler[i][j].setTipus(TIPUS_EMPTY);
                        }
                    }
                }
            }
        }
    }
}