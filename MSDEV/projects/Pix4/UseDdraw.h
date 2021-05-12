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


BOOL Start();
void Finish();

HRESULT RestoreAll();

BOOL ok;

public:
static int x;
static int y;

CUseDdraw();
~CUseDdraw();

void Update();

BOOL Ok()
{
return ok;
}

unsigned char *LockBackBuf();
void UnLockBackBuf();
};

#endif