#include "iostream"
#include "windows.h"
#include <cmath>
#include <time.h>
#include <vector>
#include "cstdlib"
#include "Star.h"


using namespace std;

LPCSTR ClassName = TEXT("Program");
MSG msg;
HDC hdc;
int timerid;



//////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE HPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = ClassName;
	wc.style = 0;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&wc))
	{
		MessageBox(0, TEXT("Nie udalo sie zarejestrowac klasy"), TEXT("informacja"), MB_OK);
		return 1;
	}

	HWND h = CreateWindowEx(0, ClassName, TEXT("Okienko"), WS_OVERLAPPEDWINDOW, 0, 0, 600, 300, NULL, NULL, hInstance, NULL);
	if (!h)
	{
		MessageBox(NULL, TEXT("Blad stworzenia okna"), TEXT("error"), MB_ICONERROR | MB_OKCANCEL);
		UnregisterClass(ClassName, hInstance);
		return 1;
	}
	ShowWindow(h, SW_SHOW);
	UpdateWindow(h);
	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnregisterClass(ClassName, hInstance);
	return msg.wParam;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int saved_dc_id;
	PAINTSTRUCT ps;
	Star main_star(0, 0, 10, 5);
	POINT* external_vertices;
	external_vertices = main_star.getExternalVertices();
	vector<Star>minor_star;
	for (int i = 0; i < main_star.getLimbs(); i++)
	{
		int tmp_x, tmp_y;
		tmp_x = external_vertices[i].x;
		tmp_y = external_vertices[i].y;
		Star tmp(tmp_x, tmp_y, 1, 10);
		minor_star.push_back(tmp);
	}
	switch (uMsg)
	{
		RECT cr;
	case WM_CREATE:
		break;
	case WM_SIZE:
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:

		saved_dc_id = SaveDC(hdc);
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &cr);


		SetMapMode(hdc, MM_ISOTROPIC);
		SetWindowExtEx(hdc, 500, 500, NULL);
		SetViewportExtEx(hdc, cr.right, cr.bottom, NULL);
		SetViewportOrgEx(hdc, cr.right / 2, cr.bottom / 2, NULL);

		main_star.Draw(hdc, RGB(255, 0, 0), RGB(0, 255, 100));
		for (int i = 0; i < main_star.getLimbs(); i++)
		{
			minor_star[i].Draw(hdc, RGB(255, 0, 0), RGB(0, 255, 100));
		}


		RestoreDC(hdc, saved_dc_id);
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		InvalidateRect(hWnd, 0, TRUE);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
			case 0x51:
			{

				break;
			}
		default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		break;
	case WM_CLOSE:
		if (MessageBox(hWnd, TEXT("Czy chcesz zamknac?"), TEXT("uwaga"), MB_YESNO) == IDYES)
		{
			PostQuitMessage(0);
		}
		break;
	case WM_DESTROY:
		KillTimer(hWnd, timerid);
		delete external_vertices;
		break;


	default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}