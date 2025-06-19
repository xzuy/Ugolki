#pragma once
#include "pch.h"

class HPlayer : public UPlayer
{
public:
	HPlayer();
	virtual ~HPlayer();
	virtual bool MakeMove(unsigned int ox, unsigned int oy, unsigned int nx, unsigned int ny);
	bool IsHuman();
};

