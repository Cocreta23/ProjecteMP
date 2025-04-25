#pragma once

class Posicio {
public:
	Posicio() : fila(-1), col(-1), m_col("") {}
	Posicio(const string& posicio)
	{
		fila = posicio[0] - 1;
		col = posicio[1] - 'a';
		m_col = posicio[1];
	}
	bool operator==(const Posicio& posicio) const {
		return (fila == posicio.fila) && (col == posicio.col);
	}
	int getFila() const { return fila; }
	int getCol() const { return col; }
	string getM_col() const { return m_col; }
private:
	int fila;
	int col;
	string m_col;
};