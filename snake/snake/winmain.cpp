#include "iostream"
#include "windows.h"
#include <cmath>
#include <time.h>
#include <vector>
#include "cstdlib"


using namespace std;

LPCSTR ClassName = TEXT("Program");
MSG msg;
HDC hdc;
SYSTEMTIME systime;
int timerid;

vector <RECT> snake;
enum directions
{
	up, down, leftt, rightt
};
directions current_direction=up;
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
	switch (uMsg)
	{
		RECT cr;
	case WM_CREATE:
		srand(time(NULL));
		timerid=SetTimer(hWnd,0, 1000, NULL);
		
		RECT snake_head;
		snake_head.left = -5;
		snake_head.top = 5;
		snake_head.right = 5;
		snake_head.bottom = 5;

		snake.push_back(snake_head);

		break;
	case WM_SIZE:
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:

		saved_dc_id = SaveDC(hdc);
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &cr);


		SetMapMode(hdc, MM_ISOTROPIC);
		SetWindowExtEx(hdc, 600, 600, NULL);
		SetViewportExtEx(hdc, cr.right, cr.bottom, NULL);
		SetViewportOrgEx(hdc, cr.right / 2, cr.bottom / 2, NULL);

		HPEN mypen;
		HBRUSH mybrush;
		mypen=CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
		SelectObject(hdc, mypen);
		mybrush=CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(hdc, mybrush);


		//for(int i = 0; i < snake.size(); i++)
		//{
			Rectangle(hdc, snake[0].left,snake[0].top,snake[0].right,snake[0].bottom);
		//}

		RestoreDC(hdc, saved_dc_id);
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		InvalidateRect(hWnd, 0, TRUE);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case '0x57':
			current_direction=up;
				break;
		case '0x54':
			current_direction=down;
				break;
		case '0x41':
			current_direction=leftt;
				break;
		case '0x44':
			current_direction=rightt;
				break;
		default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	case WM_CLOSE:
		if (MessageBox(hWnd, TEXT("Czy chcesz zamknac?"), TEXT("uwaga"), MB_YESNO) == IDYES)
		{
			PostQuitMessage(0);
		}
		break;
	case WM_DESTROY:
		KillTimer(hWnd, timerid);
		break;


	default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}