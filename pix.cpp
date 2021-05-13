/*------------------------------------------------------------------
TEST

Sample class to demonstrate DirectDraw ...

By: Jose Carlos Ramos - 1996
---------------------------------------------------------------------*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <time.h>
#include <mmsystem.h>
#include <ddraw.h>
#include <ddraw.h>
#include "pix.h"

#include "sound.h"
#include "Exception.h"
#include "draw.h"
#include "Tile.h"

#include "square.h"
#include "board.h"



HINSTANCE gb_hInstance;
bool gb_bActive;
int gb_nCmdShow;
Draw draw;
Board board;
Sound sound;


/*-------------------------------------------------------------------

WindowProc - 
Window procedure used by the Window created in CUseDdraw::Start()

---------------------------------------------------------------------*/

long FAR PASCAL WindowProc( HWND hWnd, UINT message, 
WPARAM wParam, LPARAM lParam )
{
switch( message )
{
case WM_ACTIVATEAPP:
gb_bActive = wParam;
break;

case WM_SETCURSOR:
SetCursor(NULL);
return true;

case WM_CREATE:
break;

case WM_KEYDOWN:
int any_ret;
if ((any_ret=board.AnyKey( wParam))==666)
	PostMessage(hWnd, WM_CLOSE, 0,0);
else if (any_ret) {
	switch( wParam )
	{
	case VK_RIGHT:
		board.Right(); break;
	case VK_LEFT:
		board.Left(); break;
	case VK_UP:
		board.Up(); break;
	case VK_DOWN:
		board.Down(); break;
	case VK_SPACE:
		board.Fire(); break;
	case VK_RETURN:
		board.Restart(); break;
	case VK_NEXT:
		board.NextLevel(); break;
	case VK_PRIOR:
		board.Previous(); break;
	case VK_ESCAPE:
	case VK_F12:
	PostMessage(hWnd, WM_CLOSE, 0, 0);
}
break;

default:
break;
}
break;

case WM_DESTROY:
PostQuitMessage( 0 );
break;

}
return DefWindowProc(hWnd, message, wParam, lParam);
}

/*-------------------------------------------------------------------

WinMain 

---------------------------------------------------------------------*/

int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE ,LPSTR , int nCmdShow)
{
	MSG msg;
	gb_hInstance = hInstance;
	gb_nCmdShow = nCmdShow;


	//CUseDdraw *pUseDD = new CUseDdraw();
	//if(!pUseDD) return 255;

	//if(!pUseDD->Ok()) return 255;

	//srand( (unsigned)time( NULL ) );

	while( 1 ) {
		if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) {
			if( !GetMessage( &msg, NULL, 0, 0 ) ) {
				return msg.wParam;
			}
			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}

		else if( gb_bActive ) {
			// Get the pointer and do the drawing ...
			//unsigned char *pBuff = pUseDD->LockBackBuf();
			//if(!pBuff) 
			//return 0;

			//for(int i=0;i<WOLD_MAX_X*WOLD_MAX_Y;i++) {
			//	*pBuff = (unsigned char)rand();
			//	pBuff++; 
			//}
			//pUseDD->UnLockBackBuf();

			// Free the pointer and update the screen ...
			//Sleep(50);
			board.Animate();
		}
		else WaitMessage();

	}

	return 0;
}




