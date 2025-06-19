#include "RPlayer.h"
#include "pch.h"

bool RPlayer::MakeMove(unsigned int ox, unsigned int oy, unsigned int nx, unsigned int ny)
{
    unsigned int rox = (unsigned int)rand() % 8;
    unsigned int roy = (unsigned int)rand() % 8;
    unsigned int rnx = (unsigned int)rand() % 8;
    unsigned int rny = (unsigned int)rand() % 8;
    if (!(this->board->CheckMove(rox, roy, rnx, rny, this->ct)))
    {
        this->board->Move(rox, roy, rnx, rny, this->ct);
    }
    else
        return false;
    return true;
}

bool RPlayer::IsHuman()
{
    return false;
}

RPlayer::RPlayer() {}

void RPlayer::SetupRPlayer(CString name, CellType ct, Board* board)
{
    this->name = name;
    this->ct = ct;
    this->board = new Board();
    this->board = board;
}

RPlayer::~RPlayer() {}