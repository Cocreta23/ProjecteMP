#pragma once
#include "Posicio.h"

class CuaMoviment {
private:
    Posicio m_origen;
    Posicio m_desti;
    CuaMoviment* seguent;
public:
    CuaMoviment(const Posicio& origen, const Posicio desti) : m_origen(origen), m_desti(desti), seguent(nullptr) {}

    void setValor(const Posicio& origen, const Posicio& desti) {
        m_origen = origen;
        m_desti = desti;
    }

    CuaMoviment* getNext() const {
        return seguent;
    }

    Posicio getOrigen() { return m_origen; }
    Posicio getDesti() { return m_desti; }


    void setNext(CuaMoviment* node) {
        seguent = node;
    }
};
