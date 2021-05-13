//pix.h


class Draw;
class Board;
class Sound;

#define WOLD_MAX_X 800
#define WOLD_MAX_Y 600
#define COLUMNS 20
#define ROWS 15
#define IMAGE_WIDTH 40
#define bool short

#define BEAM_PERSISTANCE 4		//beam lasts for this many 0.025 second intervals

#define COLOR_DEPTH 16

extern HINSTANCE gb_hInstance;
extern bool gb_bActive;
extern int gb_nCmdShow;
extern Draw draw;
extern Board board;
extern Sound sound;

long FAR PASCAL WindowProc( HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam );

#define NAME "TANK"
#define TITLE "Laser Kombat"
