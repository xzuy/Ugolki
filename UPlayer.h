#pragma once
#include "pch.h"
#include "CellType.h"

class UPlayer
{
public:
	UPlayer();
	virtual ~UPlayer();
	CString name;
	void SetupPlayer(CString name, CellType ct, Board* board);
	virtual bool MakeMove(unsigned int ox, unsigned int oy, unsigned int nx, unsigned int ny) = 0;
	virtual bool IsHuman() = 0;
	CString GetName();
protected:
	Board* board;
	CellType ct;
};