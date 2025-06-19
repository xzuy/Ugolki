#pragma once
#include "Board.h"
#include "CellType.h"
#include "pch.h"

class MonteCarloEvaluator
{
private:
	Board* board;
	int nGames, nVicW, nVicB, nDraws;
	unsigned int ox, oy, nx, ny;
	CellType startCellType;
public:
	MonteCarloEvaluator(Board* board,
						int Iter,
						CellType startCellType,
						unsigned int ox, unsigned int oy,
						unsigned int nx, unsigned int ny);
	~MonteCarloEvaluator();
	void EvaluateBoard();
	void Evaluate();
	int GetWVic();
	int GetBVic();
	int GetDraws();
	int GetOX();
	int GetOY();
	int GetNX();
	int GetNY();
};