#include "Moviment.h"

void Moviment::AfegirMoviment(Posicio pos, int fila, int col)
{
	moviments[fila][col].push_back(pos);
}