/*------------------------------------------------------------------

Class CUseDdraw - declaration
Shows some basic DirectDraw features 

--------------------------------------------------------------------*/
#ifndef USEDDRAW_H
#define USEDDRAW_H

#include <ddrawex.h>
class CUseDdraw
{

static LPDIRECTDRAW lpDD;
static LPDIRECTDRAWSURFACE lpDDSPrimary;
static LPDIRECTDRAWSURFACE lpDDSBack;
static LPDIRECTDRAWSURFACE lpDDSDonut;
static LPDIRECTDRAWSURFACE lpDDSClouds;
static LPDIRECTDRAWPALETTE lpDDPal;        // DirectDraw palette



static Tile array[COLUMNS][ROWS][2];


bool Start();
void Finish();

HRESULT RestoreAll();

bool ok;

public:
static int x;
static int y;

CUseDdraw();
~CUseDdraw();

void Update();

bool Ok()
{
return ok;
}

unsigned char *LockBackBuf();
void UnLockBackBuf();
};

#endif