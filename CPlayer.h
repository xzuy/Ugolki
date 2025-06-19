#pragma once
#include "pch.h"

class CPlayer : public UPlayer
{
public:
	CPlayer();
	virtual ~CPlayer();
	virtual bool MakeMove(unsigned int ox, unsigned int oy, unsigned int nx, unsigned int ny);
	bool IsHuman();
};
