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
#define WINDOW_TITLE L"【致我们永不熄灭的游戏开发梦想】程序核心框架"

//-------------------[全局变量声明部分]----------------------------//
HDC g_hdc = NULL; //全局设备环境句柄
HPEN g_hPen[7] = { 0 };//定义画笔句柄的数组
HBRUSH g_hBrush[7] = { 0 };//定义笔刷句柄的数组
int g_iPenStyle[7] = { PS_SOLID, PS_DASH, PS_DOT, PS_DASHDOT, PS_DASHDOTDOT, PS_NULL, PS_INSIDEFRAME };
//定义画笔样式数组
int g_iBrushStyle[6] = { HS_VERTICAL, HS_HORIZONTAL, HS_CROSS, HS_DIAGCROSS, HS_FDIAGONAL, HS_BDIAGONAL };
//定义笔刷样式数组

//--------------------[全局函数声明部分]----------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lparam);//窗口环境过程函数
BOOL Game_Init(HWND hwnd); //进行资源初始化
VOID Game_Paint(HWND hwnd); //进行绘图
BOOL Game_CleanUp(HWND hwnd); //资源清理

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

	//[4]windows's remove、display and update
	MoveWindow(hwnd, 250, 80, WINDOWS_WIDTH, WINDOWS_HEIGHT, true);
	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	if (!Game_Init(hwnd))
	{
		MessageBox(hwnd, L"资源初始化失败", L"消息窗口", 0);
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
	PAINTSTRUCT paintstruct; //定义一个PAINTSTRUCT结构体来记录一些信息
	switch (message)
	{
	case WM_PAINT:
		g_hdc = BeginPaint(hwnd, &paintstruct);//指定窗口进行绘图工作的准备，并用将和绘图有关的信息填充到paintstruct结构体中
		Game_Paint(hwnd);
		EndPaint(hwnd, &paintstruct);//指定窗口的绘图工作结束
		ValidateRect(hwnd, NULL); //更新客户端显示
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

//------------------------[Game_Init()函数]------------------------
//描述：初始化函数，进行一些简单的初始化
//----------------------------------------------------------------
BOOL Game_Init(HWND hwnd)
{
	g_hdc = GetDC(hwnd); //获取设备环境句柄
	srand((unsigned)time(NULL));//初始化时间种子

	//随机画笔和笔刷的颜色值
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

//----------------------[Game_Paint()函数]-----------------------
//描述：绘制函数，在此函数中进行绘制
//------------------------------------------------------------
VOID Game_Paint(HWND hwnd)
{
	//定义Y坐标值
	int y = 0;

	for (int i = 0; i <= 6; i++)
	{
		y = (i + 1) * 70;
		SelectObject(g_hdc, g_hPen[i]);//将对应的画笔选好
		MoveToEx(g_hdc, 30, y, NULL);//”光标"移动到对应的(30,y)坐标注处
		LineTo(g_hdc, 100, y);
	}

	//定义两个x坐标值
	int x1 = 120;
	int x2 = 190;

	//用7种不同的画刷填充矩形
	for (int i = 0; i <= 60; i++)
	{
		SelectObject(g_hdc, g_hBrush[i]);
		Rectangle(g_hdc, x1, 70, x2, y);
		x1 += 90;
		x2 += 90;
	}

}

//------------------------[Game_CleanUp()函数]-------------------
//描述：资源清理函数
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