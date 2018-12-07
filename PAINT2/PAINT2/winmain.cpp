#include <windows.h>
#include <stdio.h>

const char* NazwaKlasy = TEXT("Klasa Okienka");
MSG Komunikat;

enum Figure {
	LINE, RECTANGLE, TRIANGLE, ELLIPSE
};

Figure figure[4] = { LINE,RECTANGLE,TRIANGLE,ELLIPSE };

COLORREF colors[16]
{
	RGB(255,255,255),
	RGB(0,0,0),
	RGB(255,0,0),
	RGB(0,255,0),
	RGB(0,0,255),
	RGB(255,255,0),
	RGB(0,255,255),
	RGB(255,0,255),
	RGB(155,0,155),
	RGB(100,100,100),
	RGB(30,55,85),
	RGB(255,20,180),
	RGB(100,100,60),
	RGB(100,100,60),
	RGB(10,10,10),
	RGB(200,150,0),
};
RECT figures_rect[4];
RECT colors_rect[16];
bool draw = false;

POINT first_point;
COLORREF selected_brush_color = -1;
COLORREF selected_pen_color = -1;

Figure selected_figure = ELLIPSE;

// OBS£UGA ZDARZEÑ
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HBRUSH brush;
	int saved;

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN:
	{

		RECT cr;
		POINT mouse_pos;
		mouse_pos.x = LOWORD(lParam);
		mouse_pos.y = HIWORD(lParam);

		GetClientRect(hwnd, &cr);

		if (mouse_pos.x > cr.right - 175 && mouse_pos.y < 600)
			// Przybornik
		{
			for (int i = 0; i < 16; i++)
			{
				if (PtInRect(&colors_rect[i], mouse_pos)) // Sprawdzanie kolizji z kolorowymi kwadratami
				{
					selected_brush_color = colors[i];
					break;
				}
			}
			for (int i = 0; i < 4; i++)
			{
				if (PtInRect(&figures_rect[i], mouse_pos)) // Sprawdzanie kolizji z kwadratami figur
				{
					selected_figure = figure[i];
					break;
				}
			}

			draw = false;
		}
		else
			// Pole rysowania
		{
			draw = true;
			first_point = mouse_pos;
		}
		break;
	}
	case WM_RBUTTONDOWN:
	{

		RECT cr;
		POINT mouse_pos;
		mouse_pos.x = LOWORD(lParam);
		mouse_pos.y = HIWORD(lParam);

		GetClientRect(hwnd, &cr);

		if (mouse_pos.x > cr.right - 175 && mouse_pos.y < 600)
			// INTERFEJS
		{
			for (int i = 0; i < 16; i++)
			{
				if (PtInRect(&colors_rect[i], mouse_pos))
				{
					selected_pen_color = colors[i];
					break;
				}
			}
		}
		break;
	}
	case WM_LBUTTONUP:
	{
		RECT cr;
		POINT mouse_pos;
		mouse_pos.x = LOWORD(lParam);
		mouse_pos.y = HIWORD(lParam);
		GetClientRect(hwnd, &cr);
		if (draw)
		{
			if (selected_brush_color == -1)
			{
				MessageBox(hwnd, TEXT("Wybierz kolor pedzla"), TEXT("B³êd"), NULL);
				break;
			}

			if (selected_pen_color == -1)
			{
				MessageBox(hwnd, TEXT("Wybierz kolor piora"), TEXT("B³êd"), NULL);
				break;
			}
			if (mouse_pos.x > cr.right - 175 && mouse_pos.y < 600) break;

			HDC hdc = GetDC(hwnd);

			switch (selected_figure)
			{
			case LINE:
				SelectObject(hdc, GetStockObject(DC_BRUSH));
				SelectObject(hdc, GetStockObject(DC_PEN));
				SetDCBrushColor(hdc, selected_brush_color);
				SetDCPenColor(hdc, selected_pen_color);
				MoveToEx(hdc, first_point.x, first_point.y, (LPPOINT)NULL);
				LineTo(hdc, mouse_pos.x, mouse_pos.y);
				break;
			case RECTANGLE:
				SelectObject(hdc, GetStockObject(DC_BRUSH));
				SelectObject(hdc, GetStockObject(DC_PEN));
				SetDCBrushColor(hdc, selected_brush_color);
				SetDCPenColor(hdc, selected_pen_color);
				Rectangle(hdc, first_point.x, first_point.y, mouse_pos.x, mouse_pos.y);
				break;
			case TRIANGLE:
			{
				SelectObject(hdc, GetStockObject(DC_BRUSH));
				SelectObject(hdc, GetStockObject(DC_PEN));
				SetDCBrushColor(hdc, selected_brush_color);
				SetDCPenColor(hdc, selected_pen_color);

				POINT point[3];

				point[0].x = mouse_pos.x;
				point[0].y = mouse_pos.y;
				point[1].x = first_point.x;
				point[1].y = mouse_pos.y;
				point[2].x = (first_point.x+mouse_pos.x)/2;
				point[2].y = first_point.y;

				Polygon(hdc, point, 3);

				break;
			}
			case ELLIPSE:
				SelectObject(hdc, GetStockObject(DC_BRUSH));
				SelectObject(hdc, GetStockObject(DC_PEN));
				SetDCBrushColor(hdc, selected_brush_color);
				SetDCPenColor(hdc, selected_pen_color);
				Ellipse(hdc, first_point.x, first_point.y, mouse_pos.x, mouse_pos.y);
				break;
			}

			ReleaseDC(hwnd, hdc);
		}
		break;
	}
	case WM_PAINT:
	{
		//InvalidateRect(hwnd, NULL, true);
		RedrawWindow(hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);

		HDC hdc;
		PAINTSTRUCT ps;
		RECT cr;
		HBRUSH int_brush;
		int column;

		GetClientRect(hwnd, &cr);

		hdc = BeginPaint(hwnd, &ps);
		saved = SaveDC(hdc);

		int_brush = CreateSolidBrush(0x00FEDCBA);

		SelectObject(hdc, int_brush);
		Rectangle(hdc, cr.right - 175, cr.top, cr.right, 600);

		column = 0;
		for (int i = 0; i < 16; i++)
		{
			SelectObject(hdc, GetStockObject(DC_BRUSH));
			SetDCBrushColor(hdc, colors[i]);

			colors_rect[i].left = cr.right - 150 + 55 * column;
			colors_rect[i].top = cr.top + 20 + 55 * (i % 8);
			colors_rect[i].right = cr.right - 100 + 55 * column;
			colors_rect[i].bottom = cr.top + 70 + 55 * (i % 8);

			Rectangle(hdc,colors_rect[i].left,colors_rect[i].top,colors_rect[i].right,colors_rect[i].bottom);

			if (i == 7)
				column = 1;
		}

		column = 0;
		for (int i = 0; i < 4; i++)
		{
			SelectObject(hdc, GetStockObject(WHITE_BRUSH));

			if (i == 2) column = 1;
			figures_rect[i].left = cr.right - 150 + 55 * column;
			figures_rect[i].top = cr.top + 480 + 55 * (i % 2);
			figures_rect[i].right = cr.right - 100 + 55 * column;
			figures_rect[i].bottom = cr.top + 530 + 55 * (i % 2);

			Rectangle(hdc,
				figures_rect[i].left,
				figures_rect[i].top,
				figures_rect[i].right,
				figures_rect[i].bottom);
		}


			MoveToEx(hdc, figures_rect[0].left+5,figures_rect[0].top+5, NULL);														// Linia w kwadracie
			LineTo(hdc, figures_rect[0].right - 5, figures_rect[0].bottom - 5);

			MoveToEx(hdc, figures_rect[1].left + 5, figures_rect[1].top + 5, NULL);													//Kwadrat w kwadracie
			Rectangle(hdc, figures_rect[1].left+ 5,figures_rect[1].top+5,figures_rect[1].right - 5, figures_rect[1].bottom - 5);

			MoveToEx(hdc, figures_rect[2].left + 5, figures_rect[2].bottom - 5, NULL);												// Trójkat w kwadracie
			LineTo(hdc, figures_rect[2].right - 5, figures_rect[2].bottom - 5);
			LineTo(hdc, (figures_rect[2].right+figures_rect[2].left)/2, figures_rect[2].top + 5);
			LineTo(hdc, figures_rect[2].left + 5, figures_rect[2].bottom - 5);

			MoveToEx(hdc, figures_rect[3].left + 5, figures_rect[3].top + 5, NULL);													// Kolo w kwadracie
			Ellipse(hdc, figures_rect[3].left + 5, figures_rect[3].top + 5, figures_rect[3].right - 5, figures_rect[3].bottom - 5);

		DeleteObject(int_brush);

		RestoreDC(hdc, saved);
		EndPaint(hwnd, &ps);
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{


	// WYPE£NIANIE STRUKTURY
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_VREDRAW | CS_HREDRAW;  // Dziêki temu rysuje siê jeszcze raz
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = NazwaKlasy;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


	// REJESTROWANIE KLASY OKNA
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("Nie przesz³o rejestracji"), TEXT("..."),
			MB_ICONEXCLAMATION | MB_OK);
		return 1;
	}


	// TWORZENIE OKNA
	HWND hwnd;

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, NazwaKlasy, TEXT("Oto okienko"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 400, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, TEXT("Ni ma okna"), TEXT("..."), MB_ICONEXCLAMATION);
		return 1;
	}


	ShowWindow(hwnd, nCmdShow); // Poka¿ okienko...
	UpdateWindow(hwnd);


	// Pêtla komunikatów
	while (GetMessage(&Komunikat, NULL, 0, 0))
	{
		TranslateMessage(&Komunikat);
		DispatchMessage(&Komunikat);
	}

	return Komunikat.wParam;
}