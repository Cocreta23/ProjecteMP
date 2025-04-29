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




void Tauler::getPosicionsPossiblesDames(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[], bool despresMenjar, bool daldreta, bool dalsquerra, bool baixdreta, bool baixsquerra) {
    int fila = origen.getFila();
    int col = origen.getCol();
    ColorFitxa colorFitxa = m_tauler[fila][col].getColor();
    if (despresMenjar)
    {
        for (int i = 0; i < N_FILES; i++)
        {
            // Dalt dreta
            if (fila + i + 1 < 8 && col + i + 1 < 8 &&
                m_tauler[fila + i][col + i].getTipus() != TIPUS_EMPTY &&
                m_tauler[fila + i][col + i].getColor() != m_tauler[fila][col].getColor() &&
                m_tauler[fila + i + 1][col + i + 1].getTipus() == TIPUS_EMPTY)
            {
                daldreta = true;
            }

            // Dalt esquerra
            if (fila + i + 1 < 8 && col - i - 1 >= 0 &&
                m_tauler[fila + i][col - i].getTipus() != TIPUS_EMPTY &&
                m_tauler[fila + i][col - i].getColor() != m_tauler[fila][col].getColor() &&
                m_tauler[fila + i + 1][col - i - 1].getTipus() == TIPUS_EMPTY)
            {
                dalsquerra = true;
                
            }

            // Baix dreta
            if (fila - i - 1 > 0 && col + i + 1 < 8 &&
                m_tauler[fila - i][col + i].getTipus() != TIPUS_EMPTY &&
                m_tauler[fila - i][col + i].getColor() != m_tauler[fila][col].getColor() &&
                m_tauler[fila - i - 1][col + i + 1].getTipus() == TIPUS_EMPTY)
            {
                baixdreta = true;
            }


            // Baix esquerra
            if (fila - i - 1 >= 0 && col - i - 1 >= 0 &&
                m_tauler[fila - i][col - i].getTipus() != TIPUS_EMPTY &&
                m_tauler[fila - i][col - i].getColor() != m_tauler[fila][col].getColor() &&
                m_tauler[fila - i - 1][col - i - 1].getTipus() == TIPUS_EMPTY)
            {
                baixsquerra = true;
            }
        }
    }

    
    //adalt a la dreta
    if (m_tauler[fila + 1][col + 1].getTipus() == TIPUS_EMPTY && fila < 8 && col < 8 && daldreta)
    {
        string posicio = inttoString(fila + 1, col + 1);
        posicionsPossibles[nPosicions] = Posicio(posicio);
        nPosicions++;
        getPosicionsPossiblesDames(Posicio(posicio), nPosicions, posicionsPossibles, 0, 1,0,0,0);
    }
    
        //abaix a la dreta
    if (m_tauler[fila - 1][col + 1].getTipus() == TIPUS_EMPTY && fila > 0 && col < 8 && baixdreta)
    {
        string posicio = inttoString(fila - 1, col + 1);
        posicionsPossibles[nPosicions] = Posicio(posicio);
        nPosicions++;
        getPosicionsPossiblesDames(Posicio(posicio), nPosicions, posicionsPossibles, 0, 0, 0, 1, 0);
    }
        
            //a baix esquerra
    if (m_tauler[fila - 1][col - 1].getTipus() == TIPUS_EMPTY && fila > 0 && col > 0 && baixsquerra)
    {
        string posicio = inttoString(fila - 1, col - 1);
        posicionsPossibles[nPosicions] = Posicio(posicio);
        nPosicions++;
        getPosicionsPossiblesDames(Posicio(posicio), nPosicions, posicionsPossibles, 0, 0, 0, 0, 1);
    }
            
    //adalt a l'esquerra
    if (m_tauler[fila + 1][col - 1].getTipus() == TIPUS_EMPTY && fila < 8 && col > 0 && dalsquerra)
    {
        string posicio = inttoString(fila + 1, col - 1);
        posicionsPossibles[nPosicions] = Posicio(posicio);
        nPosicions++;
        getPosicionsPossiblesDames(Posicio(posicio), nPosicions, posicionsPossibles, 0,0,1,0,0);
    }
                
    if (m_tauler[fila + 1][col + 1].getTipus() != TIPUS_EMPTY && m_tauler[fila + 2][col + 2].getTipus() == TIPUS_EMPTY && m_tauler[fila + 1][col + 1].getColor() != colorFitxa
    && fila + 2 < 8 && col + 2 < 8 && daldreta)
    {
    string posicio = inttoString(fila + 2, col + 2);
    posicionsPossibles[nPosicions] = Posicio(posicio);
    nPosicions++;
    getPosicionsPossiblesDames(Posicio(posicio), nPosicions, posicionsPossibles, 1, 0, 0, 0, 0);
    }
    
    if (m_tauler[fila + 1][col - 1].getTipus() != TIPUS_EMPTY && m_tauler[fila + 2][col - 2].getTipus() == TIPUS_EMPTY && m_tauler[fila + 1][col - 1].getColor() != colorFitxa
        && fila + 2 < 8 && col - 2 > 0 && dalsquerra)
    {
        string posicio = inttoString(fila + 2, col - 2);
        posicionsPossibles[nPosicions] = Posicio(posicio);
        nPosicions++;
        getPosicionsPossiblesDames(Posicio(posicio), nPosicions, posicionsPossibles, 1, 0, 0, 0, 0);
    }
        
    
            
    if (m_tauler[fila - 1][col - 1].getTipus() != TIPUS_EMPTY && m_tauler[fila - 2][col - 2].getTipus() == TIPUS_EMPTY && m_tauler[fila - 1][col - 1].getColor() != colorFitxa
        && fila - 2 > 0 && col - 2 > 8 && baixsquerra)
    {
        string posicio = inttoString(fila - 2, col - 2);
        posicionsPossibles[nPosicions] = Posicio(posicio);
        nPosicions++;
        getPosicionsPossiblesDames(Posicio(posicio), nPosicions, posicionsPossibles, 1, 0, 0, 0, 0);
    }

    

}

void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[])
{
    int fila = origen.getFila();
    int col = origen.getCol();
    ColorFitxa color = m_tauler[fila][col].getColor();
    nPosicions = 0;
    int fet = 0;
    if (m_tauler[fila][col].getTipus() == TIPUS_DAMA)
    {
        getPosicionsPossiblesDames(origen, nPosicions, posicionsPossibles, 0, 1, 1, 1, 1);
    }
    else
        if (m_tauler[fila][col].getTipus() == TIPUS_NORMAL)
        {
            getPosicionsPossiblesNorm(origen, nPosicions, posicionsPossibles, fet, color);
        }
}

bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti) {
    

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
    int nPosicions;
    int col = desti.getCol();
    int fila = desti.getFila();
    Posicio posicioAct = Posicio(desti);
    if (m_tauler[fila][col].getTipus() == TIPUS_DAMA) {
        while (posicioAct != origen)
        {
            for (int i = 0; i < N_FILES; i++)
            {
                for (int j = 0; j < N_FILES; j++)
                {
                    if (fila + i + 1 < 8 && col + i + 1 < 8 && Posicio(inttoString(fila + i + 1, col + i + 1)) == PosicionsPossibles[j])
                    {
                        m_tauler[fila + i][col + i].setTipus(TIPUS_EMPTY);
                        posicioAct = inttoString(fila + i + 1, col + i + 1);
                    }
                    if (fila + i + 1 < 8 && col - i - 1 >= 0 && Posicio(inttoString(fila + i + 1, col - i - 1)) == PosicionsPossibles[j])
                    {
                        m_tauler[fila + i][col - i].setTipus(TIPUS_EMPTY);
                        posicioAct = inttoString(fila + i + 1, col - i - 1);

                    }
                    if (fila - i - 1 >= 0 && col + i + 1 < 8 && Posicio(inttoString(fila - i - 1, col + i + 1)) == PosicionsPossibles[j])
                    {
                        m_tauler[fila - i][col + i].setTipus(TIPUS_EMPTY);
                        posicioAct = inttoString(fila - i - 1, col + i + 1);

                    }
                    if (fila - i - 1 >= 0 && col - i - 1 >= 0 && Posicio(inttoString(fila - i - 1, col - i - 1)) == PosicionsPossibles[j])
                    {
                        m_tauler[fila - i][col - i].setTipus(TIPUS_EMPTY);
                        posicioAct = inttoString(fila - i - 1, col - i - 1);

                    }
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
            if (fila + i + 1 < 8 && col + i + 1 < 8 && m_tauler[fila + i][col + i].getTipus() != TIPUS_EMPTY && m_tauler[fila + i + 1][col + i + 1].getColor() != color) {
                return true;
            }
            else 
                if (fila + i + 1 < 8 && col - i - 1 >= 0 && m_tauler[fila + i][col - i].getTipus() != TIPUS_EMPTY && m_tauler[fila + i + 1][col - i - 1].getColor() != color) {
                    return true;
                }
                else
                    if (fila - i - 1 >= 0 && col + i + 1 < 8 && m_tauler[fila - i][col + i].getTipus() != TIPUS_EMPTY && m_tauler[fila - i - 1][col + i + 1].getColor() != color) {
                        return true;
                    }
                    else
                        if (fila - i - 1 >= 8 && col - i - 1 >= 0 && m_tauler[fila + i][col + i].getTipus() != TIPUS_EMPTY && m_tauler[fila - i - 1][col - i - 1].getColor() != color) {
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