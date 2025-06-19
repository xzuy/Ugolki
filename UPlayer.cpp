#pragma once
#include "pch.h"
#include "UPlayer.h"

UPlayer::UPlayer() {}

void UPlayer::SetupPlayer(CString name, CellType ct, Board* board)
{
	this->name = name;
	this->ct = ct;
	this->board = new Board();
	this->board = board;
}

UPlayer::~UPlayer() {}

CString UPlayer::GetName()
{
	return this->name;
}