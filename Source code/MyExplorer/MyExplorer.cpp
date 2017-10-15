// MyExplorer.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "MyExplorer.h"
#include<windows.h>
#include <commctrl.h>
#include"CDrive.h"
#include"CTree_View.h"
#include<windowsX.h>
#include"CList_View.h"

RECT main;
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
CTree_View *g_TreeView;
CDrive *g_Drive;
CListView *g_ListView;
BOOL g_bStarted = FALSE;

void OnDestroy(HWND hwnd);
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnm);
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnm);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYEXPLORER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYEXPLORER));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYEXPLORER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYEXPLORER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, _T("My Explorer - Phạm Huỳnh Trung -1512617"), WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
      CW_USEDEFAULT, 0, 1280, 720, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_NOTIFY, OnNotify);
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
void OnDestroy(HWND hwnd)
{
	PostQuitMessage(0);
}
// Message handler for about box.
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
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	// Parse the menu selections:
	switch (id)
	{
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hwnd);
		break;
	}
}

LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnm)
{
	int nCurSelIndex;
	LPNMTOOLBAR lpnmToolBar = (LPNMTOOLBAR)pnm;

	if (g_bStarted) //Để tránh vòng lặp lẩn quẩn T_T
	{
		LPNMTREEVIEW lpnmTree = (LPNMTREEVIEW)pnm;

		switch (pnm->code)
		{
		case TVN_ITEMEXPANDING:
			g_TreeView->PreloadExpanding(lpnmTree->itemOld.hItem, lpnmTree->itemNew.hItem);
			break;
			//------------------------------------------------------------------------------
		case TVN_SELCHANGED:
			g_TreeView->Expand(g_TreeView->GetCurSel());
			g_ListView->DeleteAll(); //Xóa sạch List View để nạp cái mới
			g_ListView->LoadChild(g_TreeView->GetCurPath(), g_Drive);
			break;
			//------------------------------------------------------------------------------
		case NM_CLICK:
			if (pnm->hwndFrom == g_ListView->GetHandle())
			{
				nCurSelIndex = ListView_GetNextItem(g_ListView->GetHandle(), -1, LVNI_FOCUSED);
			}
		
			break;
		case NM_DBLCLK:
			if (pnm->hwndFrom == g_ListView->GetHandle())
				g_ListView->LoadCurSel();
			break;
		}//switch
	}//if

	return 0;
}
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	g_Drive = new CDrive;
	g_Drive->GetSystemDrives();

	g_TreeView = new CTree_View;
	g_TreeView->Create(hwnd, IDC_TREEVIEW, hInst, 400, 720);
	g_TreeView->LoadMyComputer(g_Drive);
	g_TreeView->GetFocus();

	g_ListView = new CListView;
	g_ListView->Create(hwnd, IDC_LISTVIEW, hInst,1280-400,720,400);
	g_ListView->LoadMyComputer(g_Drive);
	g_bStarted = TRUE;
	return TRUE;
}