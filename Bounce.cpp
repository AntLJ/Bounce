// Bounce.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Bounce.h"

#define MAX_LOADSTRING 100
#define ID_TIMER 1

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BOUNCE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BOUNCE));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BOUNCE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BOUNCE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hBitmap;
	static int cxClient, cyClient, xCenter, yCenter, cxTotal, cyTotal, cxRadius, cyRadius, cxMove, cyMove, xPixel, yPixel;
	HBRUSH hBrush;
	HDC hdc, hdcMem;
	int iScale;


	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		xPixel = GetDeviceCaps(hdc, ASPECTX);
		yPixel = GetDeviceCaps(hdc, ASPECTY);
		ReleaseDC(hwnd, hdc);

		SetTimer(hwnd, ID_TIMER, 50, NULL);
		return 0;

	case WM_SIZE:
		xCenter = (cxClient = LOWORD(lParam)) / 2;
		yCenter = (cyClient = HIWORD(lParam)) / 2;

		iScale = min(cxClient * xPixel, cyClient * yPixel) / 16;

		cxRadius = iScale / xPixel;
		cyRadius = iScale / yPixel;

		cxMove = max(1, cxRadius / 2);
		cyMove = max(1, cyRadius / 2);

		cxTotal = 2 * (cxRadius + cxMove);
		cyTotal = 2 * (cyRadius + cyMove);

		if (hBitmap)
			DeleteObject(hBitmap);
		hdc = GetDC(hwnd);
		hdcMem = CreateCompatibleDC(hdc);
		hBitmap = CreateCompatibleBitmap(hdc, cxTotal, cyTotal);
		ReleaseDC(hwnd, hdc);

		SelectObject(hdcMem, hBitmap);
		Rectangle(hdcMem, -1, -1, cxTotal + 1, cyTotal + 1);

		hBrush = CreateHatchBrush(HS_DIAGCROSS, 0L);
		SelectObject(hdcMem, hBrush);
		SetBkColor(hdcMem, RGB(255, 0, 255));
		Ellipse(hdcMem, cxMove, cyMove, cxTotal - cxMove, cyTotal - cyMove);
		DeleteDC(hdcMem);
		DeleteObject(hBrush);
		return 0;

	case WM_TIMER:
		if (!hBitmap)
			break;

		hdc = GetDC(hwnd);
		hdcMem = CreateCompatibleDC(hdc);
		SelectObject(hdcMem, hBitmap);

		BitBlt(hdc, xCenter - cxTotal / 2, yCenter - cyTotal / 2, cxTotal, cyTotal, hdcMem, 0, 0, SRCCOPY);

		ReleaseDC(hwnd, hdc);
		DeleteDC(hdcMem);

		xCenter += cxMove;
		yCenter += cyMove;

		if ((xCenter + cxRadius >= cxClient) || (xCenter - cxRadius <= 0))
			cxMove = -cxMove;

		if ((yCenter + cyRadius >= cyClient) || (yCenter - cyRadius <= 0))
			cyMove = -cyMove;

		return 0;

	case WM_DESTROY:
		if (hBitmap)
			DeleteObject(hBitmap);

		KillTimer(hwnd, ID_TIMER);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
