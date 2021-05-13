// Draw.cpp

#include <ddraw.h>
#include "ddutil.h"
#include "pix.h"
#include "sound.h"

#include "Exception.h"
#include "Draw.h"
#include "Tile.h"

#include "Square.h"
#include "Board.h"

void Draw::SetWindow()
{
	WNDCLASS wc;

	// Create the Window class and the Window ...
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = gb_hInstance;
	wc.hIcon = LoadIcon( gb_hInstance, "MAINICON" );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NAME;
	wc.lpszClassName = NAME;
	if(!RegisterClass( &wc ))
		throw Exception("Failed to register window class.");

	DWORD exStyle = 0;
	DWORD style = WS_POPUP;

	hwnd = CreateWindowEx(exStyle,NAME,TITLE,
	style,0,0,
	GetSystemMetrics(SM_CXSCREEN),
	GetSystemMetrics(SM_CYSCREEN),
	NULL,NULL, gb_hInstance,NULL );

	if( !hwnd ) throw Exception("Window creation failed.");

	ShowWindow( hwnd, gb_nCmdShow );
	UpdateWindow( hwnd );
	SetFocus( hwnd );

}

void Draw::SetDDraw()
{
	DDSURFACEDESC ddsd;
	DDSCAPS ddscaps;
	HRESULT ddrval;

	// Create DD Object 
	ddrval = DirectDrawCreate( NULL, &lpDD, NULL );
	if( ddrval != DD_OK )
	throw Exception("DirectDrawCreate failed.");

	ddrval = lpDD->SetCooperativeLevel( hwnd, /*DDSCL_ALLOWMODEX |*/ DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
	if( ddrval != DD_OK )
	throw Exception("SetCooperativeLevel failed.");

	ddrval = lpDD->SetDisplayMode( WOLD_MAX_X,WOLD_MAX_Y, COLOR_DEPTH);

	if(ddrval != DD_OK)
	throw Exception("SetDisplayMode failed.");

	// Create the primary surface with a back buffer 
	ddsd.dwSize = sizeof( ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
	DDSCAPS_FLIP |
	DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount = 1;
	ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
	if( ddrval != DD_OK )
	throw Exception("Could not create primary surface.");

	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	ddrval = lpDDSPrimary->GetAttachedSurface(&ddscaps, &lpDDSBack);
	if( ddrval != DD_OK )
	throw Exception("Could not attach back surface.");
	/// fill in black
	RECT rc = {0,0, WOLD_MAX_X,WOLD_MAX_Y};

    DDBLTFX fx;
    memset(&fx, 0, sizeof(fx));
    fx.dwSize = sizeof(DDBLTFX);
    fx.dwFillColor = RGB(0,0,0);
    lpDDSBack->Blt(&rc, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fx);

}

void Draw::CleanUp()
{
	if( lpDD != NULL )
	{
		if( lpDDSPrimary != NULL )
		{
			lpDDSPrimary->Release();
			lpDDSPrimary = NULL;
		}

		lpDD->Release();
		lpDD = NULL;
	}
}

HRESULT Draw::Restore()
{
	HRESULT ddrval;
	ddrval = lpDDSPrimary->Restore();
	GroundTile::RestoreSurface();
	RedBlock::RestoreSurface();
	Water::RestoreSurface();
	Static::RestoreSurface();
	Mirror::RestoreSurface();
	Tee::RestoreSurface();
	Nuke::RestoreSurface();
	Rusty::RestoreSurface();
	Message::RestoreSurface();
	Tank::RestoreSurface();
	BarsVert::RestoreSurface();

	Tile::RestoreBeamSurface();
	Board::RestoreHelpSurface();
	return ddrval;
}

LPDIRECTDRAWSURFACE Draw::GetSurface(const char* name)
{
	HRESULT ddrval;

	LPDIRECTDRAWSURFACE surface = DDLoadBitmap(lpDD, name, 0, 0);
	if( surface == NULL )
		throw Exception("Error loading bitmap.");
	ddrval = DDSetColorKey(surface, RGB(0,0,0));
	if (ddrval != DD_OK)
		throw Exception("Error setting color key for surface.");
	return surface;

}
void Draw::BlitSquare(LPDIRECTDRAWSURFACE surface, int x, int y, int dx, int dy)
{
	x*=IMAGE_WIDTH;
	y*=IMAGE_WIDTH;
	dx*=IMAGE_WIDTH;
	dy*=IMAGE_WIDTH;
	RECT r={x,y,x+IMAGE_WIDTH, y+IMAGE_WIDTH};
	lpDDSBack->BltFast(dx,dy,surface,&r, DDBLTFAST_SRCCOLORKEY|DDBLTFAST_WAIT);

	//RECT rd={dx, dy, dx+IMAGE_WIDTH, dy+IMAGE_WIDTH};
	//lpDDSBack->Blt(&rd, surface, &r, DDBLT_WAIT|DDBLT_ALPHASRC, NULL);
}

void Draw::Blit(LPDIRECTDRAWSURFACE surface, int x, int y, int dx, int dy)
{
	RECT r={x,y,x+IMAGE_WIDTH, y+IMAGE_WIDTH};
	lpDDSBack->BltFast(dx,dy,surface,&r, DDBLTFAST_SRCCOLORKEY|DDBLTFAST_WAIT);
	//Exception::Output("Blit called");

}

void Draw::BlitOther(LPDIRECTDRAWSURFACE surface, int x, int y, int dx, int dy, int w, int h)
{
	RECT r={x,y,x+w, y+h};
	lpDDSBack->BltFast(dx,dy,surface,&r, DDBLTFAST_SRCCOLORKEY|DDBLTFAST_WAIT);
	//Exception::Output("Blit called");

}

void Draw::BlackSquare(int dx, int dy)
{
	dx*=IMAGE_WIDTH;
	dy*=IMAGE_WIDTH;

	RECT rc = {dx, dy, dx+IMAGE_WIDTH, dy+IMAGE_WIDTH};

    DDBLTFX fx;
    memset(&fx, 0, sizeof(fx));
    fx.dwSize = sizeof(DDBLTFX);
    fx.dwFillColor = RGB(0,0,0);
    lpDDSBack->Blt(&rc, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fx);


}


void Draw::Flip()
{
	while( 1 )
	{
		HRESULT ddrval = lpDDSPrimary->Flip( NULL, DDFLIP_WAIT );
		if( ddrval == DD_OK ) break;

		if( ddrval == DDERR_SURFACELOST ) {
			ddrval = Restore();
			if( ddrval != DD_OK )
			break;
		}

		break;

	}
}

