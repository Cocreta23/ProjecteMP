#pragma once
#include <iostream>
#include <string>
using namespace std;

class Posicio {
public:
    Posicio() : fila(-1), col(-1) {}
    Posicio(const string& posicio)
    {
        col = posicio[0] - 'a';
        fila = posicio[1] - '1';
    }
    bool operator==(const Posicio& posicio) const {
        return (fila == posicio.fila) && (col == posicio.col);
    }
    bool operator!=(const Posicio& posicio) const {
        return (fila != posicio.fila) || (col != posicio.col);
    }
    int getFila() const { return fila; }
    int getCol() const { return col; }
    friend ostream& operator<<(ostream& os, const Posicio& pos) {
        os << static_cast<char>(pos.col + 'a') << static_cast<char>(pos.fila + '1');
        return os;
    }
    friend istream& operator>>(istream& is, Posicio& pos) {
        string input;
        is >> input;

        if (input.length() == 2 &&
            input[0] >= 'a' && input[0] <= 'h' &&
            input[1] >= '1' && input[1] <= '8') {
            pos.col = input[0] - 'a';
            pos.fila = input[1] - '1';
        }
        else {
            pos.col = -1;
            pos.fila = -1;
            is.setstate(ios::failbit); 
        }

        return is;
    }

private:
    int fila;
    int col;
};
