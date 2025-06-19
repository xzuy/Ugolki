#pragma once
#include "UPlayer.h"

class RPlayer : public UPlayer
{
public:
	RPlayer();
	void SetupRPlayer(CString name, CellType ct, Board* board);
	virtual ~RPlayer();
	virtual bool MakeMove(unsigned int ox, unsigned int oy, unsigned int nx, unsigned int ny);
	virtual bool IsHuman();
};