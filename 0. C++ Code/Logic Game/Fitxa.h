#pragma once

typedef enum
{
	TIPUS_NORMAL,
	TIPUS_DAMA,
	TIPUS_EMPTY
} TipusFitxa;


typedef enum
{
	COLOR_NEGRE,
	COLOR_BLANC,
} ColorFitxa;


class Fitxa {
private:
	TipusFitxa tipus;
	ColorFitxa color;
public:
	TipusFitxa getTipus() const { return tipus; }
	ColorFitxa getColor() const { return color; }
	void setTipus(TipusFitxa t) { tipus = t; }
	void setColor(ColorFitxa c) { color = c; }
};