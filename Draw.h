#ifndef DRAW_H
#define DRAW_H

#include <SDL2/SDL.h>

class Draw
{
public:
	Draw() {
		SetWindow();
		SetDDraw();
	}
	~Draw() {
		CleanUp();
	}
	SDL_Texture * GetSurface(const char* name);
	void Blit(SDL_Texture * surface, int x, int y, int dx, int dy);
	void BlitSquare(SDL_Texture * surface, int x, int y, int dx, int dy);
	void BlitOther(SDL_Texture * surface, int x, int y, int dx, int dy, int w, int h);
	void BlackSquare(int dx, int dy);
	void Flip();
private: //functions
	void SetWindow();
	void SetDDraw();
	void CleanUp();
	// HRESULT Restore();

private: //members
	// LPDIRECTDRAW lpDD;
	SDL_Surface * lpDDSPrimary;
	SDL_Surface * lpDDSBack;

	// HWND hwnd;

	SDL_Window * window;
	SDL_Renderer * renderer;

};

#endif // DRAW_H
