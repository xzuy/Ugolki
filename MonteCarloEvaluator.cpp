#include "MonteCarloEvaluator.h"
#include "RPlayer.h"
#include "pch.h"

MonteCarloEvaluator::MonteCarloEvaluator(Board* board,
	int Iter,
	CellType startCellType,
	unsigned int ox, unsigned int oy,
	unsigned int nx, unsigned int ny)
{
	this->board = board;
	this->nGames = Iter;
	this->nVicW = 0;
	this->nVicB = 0;
	this->nDraws = 0;
	this->ox = ox;
	this->oy = oy;
	this->nx = nx;
	this->ny = ny;
	this->startCellType = startCellType;
}

MonteCarloEvaluator::~MonteCarloEvaluator()
{
}

void MonteCarloEvaluator::EvaluateBoard()
{
	Board* board = new Board(this->board);
	
	RPlayer* Player1 = new RPlayer();
	Player1->SetupRPlayer(L"W", CellType_W, board);

	RPlayer* Player2 = new RPlayer();
	Player1->SetupRPlayer(L"B", CellType_B, board);

	RPlayer* currentPlayer = Player1;
	bool bRandomFinish = false;
	if (board->CheckEndCondition())
	{
		if (board->IsWVictory()) nVicW++;
		if (board->IsBVictory()) nVicB++;
		else nDraws++;
		bRandomFinish = true;
		return;
	}
	while (!bRandomFinish)
	{
		while (!currentPlayer->MakeMove(0,0,0,0));
		if (board->CheckEndCondition())
		{
			if (board->IsWVictory()) nVicW++;
			if (board->IsBVictory()) nVicB++;
			else nDraws++;
			bRandomFinish = true;
			return;
		}
	}
	if (currentPlayer == Player1)
	{
		currentPlayer = Player2;
	}
	else currentPlayer = Player1;
}

void MonteCarloEvaluator::Evaluate()
{
	for (int i = 0; i < nGames; i++) this->EvaluateBoard();
}

int MonteCarloEvaluator::GetWVic()
{
	return nVicW;
}

int MonteCarloEvaluator::GetBVic()
{
	return nVicB;
}

int MonteCarloEvaluator::GetDraws()
{
	return nDraws;
}

int MonteCarloEvaluator::GetOX()
{
	return ox;
}

int MonteCarloEvaluator::GetOY()
{
	return oy;
}

int MonteCarloEvaluator::GetNX()
{
	return nx;
}

int MonteCarloEvaluator::GetNY()
{
	return ny;
}