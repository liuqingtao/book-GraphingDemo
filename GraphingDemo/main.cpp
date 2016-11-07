//-----------------------------------------------
//objname:GameCore
//september 1st,2016
//describe:create game frame with program
//-----------------------------------------------

//----------------------[header file]-------------
#include <windows.h>
#include <time.h>

//---------------------[macro definition]----------
#define WINDOWS_WIDTH 800
#define WINDOWS_HEIGHT 600
#define WINDOW_TITLE L"������������Ϩ�����Ϸ�������롿������Ŀ��"

//-------------------[ȫ�ֱ�����������]----------------------------//
HDC g_hdc = NULL; //ȫ���豸�������
HPEN g_hPen[7] = { 0 };//���廭�ʾ��������
HBRUSH g_hBrush[7] = { 0 };//�����ˢ���������
int g_iPenStyle[7] = { PS_SOLID, PS_DASH, PS_DOT, PS_DASHDOT, PS_DASHDOTDOT, PS_NULL, PS_INSIDEFRAME };
//���廭����ʽ����
int g_iBrushStyle[6] = { HS_VERTICAL, HS_HORIZONTAL, HS_CROSS, HS_DIAGCROSS, HS_FDIAGONAL, HS_BDIAGONAL };
//�����ˢ��ʽ����

//--------------------[ȫ�ֺ�����������]----------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lparam);//���ڻ������̺���
BOOL Game_Init(HWND hwnd); //������Դ��ʼ��
VOID Game_Paint(HWND hwnd); //���л�ͼ
BOOL Game_CleanUp(HWND hwnd); //��Դ����

//-------------------[WinMain() functiono]-----------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//[1]design windows class
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"ForTheDreamofGameDevelop";

	//[2]register
	if (!RegisterClassEx(&wndClass))
		return -1;

	//[3]create windows
	HWND hwnd = CreateWindow(L"ForTheDreamofGameDevelop", WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOWS_WIDTH,
		WINDOWS_HEIGHT, NULL, NULL, hInstance, NULL);

	//[4]windows's remove��display and update
	MoveWindow(hwnd, 250, 80, WINDOWS_WIDTH, WINDOWS_HEIGHT, true);
	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	if (!Game_Init(hwnd))
	{
		MessageBox(hwnd, L"��Դ��ʼ��ʧ��", L"��Ϣ����", 0);
		return FALSE;
	}

	//[5]message loop
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	//[6]cancel windows class 
	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);
	return 0;
}

//---------------------[WndProc() function]----------------------
LRESULT  CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintstruct; //����һ��PAINTSTRUCT�ṹ������¼һЩ��Ϣ
	switch (message)
	{
	case WM_PAINT:
		g_hdc = BeginPaint(hwnd, &paintstruct);//ָ�����ڽ��л�ͼ������׼�������ý��ͻ�ͼ�йص���Ϣ��䵽paintstruct�ṹ����
		Game_Paint(hwnd);
		EndPaint(hwnd, &paintstruct);//ָ�����ڵĻ�ͼ��������
		ValidateRect(hwnd, NULL); //���¿ͻ�����ʾ
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		Game_CleanUp(hwnd);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

//------------------------[Game_Init()����]------------------------
//��������ʼ������������һЩ�򵥵ĳ�ʼ��
//----------------------------------------------------------------
BOOL Game_Init(HWND hwnd)
{
	g_hdc = GetDC(hwnd); //��ȡ�豸�������
	srand((unsigned)time(NULL));//��ʼ��ʱ������

	//������ʺͱ�ˢ����ɫֵ
	for (int i = 0; i <= 6; i++)
	{
		g_hPen[i] = CreatePen(g_iPenStyle[i], 1, RGB(rand() % 256, rand() % 256, rand() % 256));
		if (i == 6)
		{
			g_hBrush[i] = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
		}
		else
		{
			g_hBrush[i] = CreateHatchBrush(g_iBrushStyle[i],(rand() % 256, rand() % 256, rand() % 256));
		}
	}
	Game_Paint(hwnd); 
	ReleaseDC(hwnd, g_hdc);
	return TRUE;
}

//----------------------[Game_Paint()����]-----------------------
//���������ƺ������ڴ˺����н��л���
//------------------------------------------------------------
VOID Game_Paint(HWND hwnd)
{
	//����Y����ֵ
	int y = 0;

	for (int i = 0; i <= 6; i++)
	{
		y = (i + 1) * 70;
		SelectObject(g_hdc, g_hPen[i]);//����Ӧ�Ļ���ѡ��
		MoveToEx(g_hdc, 30, y, NULL);//�����"�ƶ�����Ӧ��(30,y)����ע��
		LineTo(g_hdc, 100, y);
	}

	//��������x����ֵ
	int x1 = 120;
	int x2 = 190;

	//��7�ֲ�ͬ�Ļ�ˢ������
	for (int i = 0; i <= 60; i++)
	{
		SelectObject(g_hdc, g_hBrush[i]);
		Rectangle(g_hdc, x1, 70, x2, y);
		x1 += 90;
		x2 += 90;
	}

}

//------------------------[Game_CleanUp()����]-------------------
//��������Դ������
//--------------------------------------------------------------
BOOL Game_CleanUp(HWND hwnd)
{
	for (int i = 0; i <= 6; i++)
	{
		DeleteObject(g_hPen[i]);
		DeleteObject(g_hBrush[i]);
	}
	return TRUE;
}