#include "pch.h"

CPlayer::CPlayer() {}

CPlayer::~CPlayer() {}

bool CPlayer::MakeMove(unsigned int ox, unsigned int oy, unsigned int nx, unsigned int ny)
{
	using namespace std;
	vector <MonteCarloEvaluator*> evaluators;
	for (unsigned int i = 0; i < 8; i++)
	{
		for (unsigned int j = 0; j < 8; j++)
		{
			for (unsigned int k = 0; k < 8; k++)
			{
				for (unsigned int l = 0; l < 8; l++)
				{
						if (this->board->CheckMove(j, i, l, k, this->ct) == 0)
						{
							evaluators.push_back(new MonteCarloEvaluator(this->board, 10, this->ct, j, i, l, k));
						}
				}
			}
		}
	}
	for (auto i = evaluators.begin(); i != evaluators.end(); i++)
	{
		(*i)->Evaluate();
	}
	int bigVic = -1;
	for (auto i = evaluators.begin(); i != evaluators.end(); i++)
	{
		if (this->ct == CellType_W)
		{
			if ((*i)->GetWVic() > bigVic)
				bigVic = (*i)->GetWVic();
		}
		else
		{
			if ((*i)->GetBVic() > bigVic)
				bigVic = (*i)->GetBVic();
		}
	}
	vector <MonteCarloEvaluator*> bigWinEvaluators;
	for (auto i = evaluators.begin(); i != evaluators.end(); i++)
	{
		int nVic = 0;
		if (this->ct == CellType_W) nVic = (*i)->GetWVic();
		else nVic = (*i)->GetBVic();
		if (nVic == bigVic) bigWinEvaluators.push_back(*i);
	}
	
	unsigned int index = rand() % bigWinEvaluators.size();
	if (this->board->CheckMove(bigWinEvaluators[index]->GetOX(), bigWinEvaluators[index]->GetOY(), bigWinEvaluators[index]->GetNX(), bigWinEvaluators[index]->GetNY(), this->ct) == 0)
	{
		this->board->Move(bigWinEvaluators[index]->GetOX(), bigWinEvaluators[index]->GetOY(), bigWinEvaluators[index]->GetNX(), bigWinEvaluators[index]->GetNY(), this->ct);
	}
	else
	{
		for (auto e = evaluators.begin(); e != evaluators.end(); e++)
		{
			delete (*e);
		}
		evaluators.clear();
		bigWinEvaluators.clear();
		return true;
	}

	for (auto e = evaluators.begin(); e != evaluators.end(); e++)
	{
		delete (*e);
	}
	evaluators.clear();
	bigWinEvaluators.clear();
	return true;
}

bool CPlayer::IsHuman()
{
	return false;
};