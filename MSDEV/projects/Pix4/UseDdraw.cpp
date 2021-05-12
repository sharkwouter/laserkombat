/*------------------------------------------------------------------

Class CUseDdraw - Implementation
Shows some basic DirectDraw features 

--------------------------------------------------------------------*/
#include "UseDdraw.h"
#include "pix.h"
#include "ddutil.h"

LPDIRECTDRAW CUseDdraw::lpDD=NULL; 
LPDIRECTDRAWSURFACE CUseDdraw::lpDDSPrimary=NULL;
LPDIRECTDRAWSURFACE CUseDdraw::lpDDSBack=NULL;
LPDIRECTDRAWSURFACE CUseDdraw::lpDDSDonut=NULL;
LPDIRECTDRAWSURFACE CUseDdraw::lpDDSClouds=NULL;
LPDIRECTDRAWPALETTE CUseDdraw::lpDDPal=NULL;        // DirectDraw palette

int CUseDdraw::x=NULL;
int CUseDdraw::y=NULL;
Tile CUseDdraw::array[COLUMNS][ROWS][2]={ 0 };

// Constructor - Calls Start() to init. DDraw
CUseDdraw::CUseDdraw()
{
for (int i=0; i<70; i++) {
	array[i] = rand()%60;
}
ok = Start();
}
// Destructor - Calls Finish() to do the cleanup
CUseDdraw::~CUseDdraw()
{
Finish();
}

// Start() - Create the window that will be used to atach the
// DirectDraw object. Create the DD object and the surface.
BOOL CUseDdraw::Start()
{
WNDCLASS wc;
DDSURFACEDESC ddsd;
DDSCAPS ddscaps;
HRESULT ddrval;

// Create the Window class and the Window ...
wc.style = CS_HREDRAW | CS_VREDRAW;
wc.lpfnWndProc = WindowProc;
wc.cbClsExtra = 0;
wc.cbWndExtra = 0;
wc.hInstance = gb_hInstance;
wc.hIcon = LoadIcon( gb_hInstance, IDI_APPLICATION );
wc.hCursor = LoadCursor( NULL, IDC_ARROW );
wc.hbrBackground = GetStockObject(BLACK_BRUSH);
wc.lpszMenuName = NAME;
wc.lpszClassName = NAME;
if(!RegisterClass( &wc ))
return FALSE;

DWORD exStyle = 0;
DWORD style = WS_POPUP;

HWND hwnd = CreateWindowEx(exStyle,NAME,TITLE,
style,0,0,
GetSystemMetrics(SM_CXSCREEN),
GetSystemMetrics(SM_CYSCREEN),
NULL,NULL, gb_hInstance,NULL );

if( !hwnd ) return FALSE;

ShowWindow( hwnd, gb_nCmdShow );
UpdateWindow( hwnd );
SetFocus( hwnd );

// Create DD Object 
ddrval = DirectDrawCreate( NULL, &lpDD, NULL );
if( ddrval != DD_OK )
return FALSE;

ddrval = lpDD->SetCooperativeLevel( hwnd, /*DDSCL_ALLOWMODEX |*/ DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
if( ddrval != DD_OK )
return FALSE;

ddrval = lpDD->SetDisplayMode( WOLD_MAX_X,WOLD_MAX_Y, COLOR_DEPTH);

if(ddrval != DD_OK)
return FALSE;

// Create the primary surface with a back buffer 
ddsd.dwSize = sizeof( ddsd );
ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
DDSCAPS_FLIP |
DDSCAPS_COMPLEX;
ddsd.dwBackBufferCount = 1;
ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
if( ddrval != DD_OK )
return FALSE;

ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
ddrval = lpDDSPrimary->GetAttachedSurface(&ddscaps, &lpDDSBack);
if( ddrval != DD_OK )
return FALSE;

lpDDSDonut = DDLoadBitmap(lpDD, "DONUT", 0, 0);
if( lpDDSDonut == NULL )
{
	
	return FALSE;
}
ddrval = DDSetColorKey(lpDDSDonut, RGB(0,0,0));
if (ddrval != DD_OK)
return FALSE;


lpDDSClouds = DDLoadBitmap(lpDD, "CLOUDS", 0, 0);
if( lpDDSDonut == NULL )
{
	
	return FALSE;
}

/*lpDDPal = DDLoadPalette(lpDD, "DONUT");
if (!lpDDPal) return FALSE;
ddrval = lpDDSPrimary->SetPalette(lpDDPal);
if (ddrval != DD_OK)
	return FALSE;
ddrval = lpDDSDonut->SetPalette(lpDDPal);
if (ddrval != DD_OK)
	return FALSE;
ddrval = lpDDSBack->SetPalette(lpDDPal);
if (ddrval != DD_OK)
	return FALSE;

LPDIRECTDRAWPALETTE FAR * lplpDDPalette;

lpDDSDonut->GetPalette(lplpDDPalette);
ddrval = lpDDSPrimary->SetPalette(*lplpDDPalette);*/




return TRUE;

}

// Finish() - Cleanup function
void CUseDdraw::Finish()
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

// RestoreAll() - Restores the surface if it is lost ...
HRESULT CUseDdraw::RestoreAll()
{
HRESULT ddrval;
ddrval = lpDDSPrimary->Restore();
return ddrval;
}

// LockBackBuf() - Lock the back surface, returns a pointer 
// to the back buffer, UnLockBackBuf() must be called after !
unsigned char *CUseDdraw::LockBackBuf()
{
DDSURFACEDESC ddsd;
ddsd.dwSize = sizeof(ddsd);
HRESULT ret;
//while (ret== DDERR_WASSTILLDRAWING)
ret=lpDDSBack->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);
return ret==DD_OK ? (unsigned char *)ddsd.lpSurface : NULL;
}

// UnLockBackBuf() - Unlocks the surface previous locked with 
// LockBackBuf()
void CUseDdraw::UnLockBackBuf()
{
lpDDSBack->Unlock(NULL);
}

// Update() - Flip the surfaces and update the screen
void CUseDdraw::Update()
{

static int number=0;
static int old_x=0;
static int old_y=0;
//const static RECT  r={0, 0, 64, 64};
//RECT r2={x,y,64,64};
number++;
number%=60;

RECT  r={(number%10*64), (int(number/10)*64),(number%10*64+64),(int(number/10)*64+64)};
//RECT r2={(x*64), (y*64),(x*64+64),(y*64+64)};

//RECT r2={0,0,WOLD_MAX_X,WOLD_MAX_Y};
//lpDDSBack->BltFast(0,0, lpDDSClouds, &r2, DDBLTFAST_WAIT);


//lpDDSBack->BltFast(x,y, lpDDSClouds, &r2, DDBLTFAST_WAIT);
    RECT rc = {0,0, WOLD_MAX_X,WOLD_MAX_Y};

    DDBLTFX fx;
    memset(&fx, 0, sizeof(fx));
    fx.dwSize = sizeof(DDBLTFX);
    fx.dwFillColor = RGB(255,255,255);
    lpDDSBack->Blt(&rc, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fx);
 

lpDDSBack->BltFast(x,y,lpDDSDonut,&r, /*DDBLTFAST_SRCCOLORKEY|*/DDBLTFAST_WAIT);
old_y=y; old_x=x;


while( 1 )
{
	HRESULT ddrval = lpDDSPrimary->Flip( NULL, DDFLIP_WAIT );
	if( ddrval == DD_OK ) break;

	if( ddrval == DDERR_SURFACELOST ) {
		ddrval = RestoreAll();
		if( ddrval != DD_OK )
		break;
	}

	break;

}

}
