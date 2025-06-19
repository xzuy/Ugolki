#include "pch.h"

HPlayer::HPlayer()
{
}

HPlayer::~HPlayer()
{
}

bool HPlayer::MakeMove(unsigned int ox, unsigned int oy, unsigned int nx, unsigned int ny)
{
	if (this->board->CheckMove(ox, oy, nx, ny, this->ct) == 0)
	{
		this->board->Move(ox, oy, nx, ny, this->ct);
	}
	else
		return false;
	return true;
}

bool HPlayer::IsHuman()
{
	return true;
}