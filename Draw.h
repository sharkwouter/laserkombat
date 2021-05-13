// Draw.h

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
	LPDIRECTDRAWSURFACE GetSurface(const char* name);
	void Blit(LPDIRECTDRAWSURFACE surface, int x, int y, int dx, int dy);
	void BlitSquare(LPDIRECTDRAWSURFACE surface, int x, int y, int dx, int dy);
	void BlitOther(LPDIRECTDRAWSURFACE surface, int x, int y, int dx, int dy, int w, int h);
	void BlackSquare(int dx, int dy);
	void Flip();
private: //functions
	void SetWindow();
	void SetDDraw();
	void CleanUp();
	HRESULT Restore();

private: //members
	LPDIRECTDRAW lpDD;
	LPDIRECTDRAWSURFACE lpDDSPrimary;
	LPDIRECTDRAWSURFACE lpDDSBack;

	HWND hwnd;



};