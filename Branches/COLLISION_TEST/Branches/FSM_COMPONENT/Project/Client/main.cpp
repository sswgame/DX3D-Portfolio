// 39Engine.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "main.h"

#include <Engine/CCore.h>
#include <Engine/CDevice.h>

#include <Engine/CPrefab.h>
#include <Script/CSceneSaveLoad.h>

#include "CToolObjMgr.h"
#include "CImGuiMgr.h"
#include "ImGui/imgui_impl_win32.h"
#include "CTestScene.h"

// 전역 변수:
HINSTANCE hInst;
HWND      g_hWnd;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM             MyRegisterClass(HINSTANCE hInstance);
BOOL             InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE     hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR        lpCmdLine,
                      _In_ int           nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc( );

	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	// Engine Core 초기화
	if (FAILED(CCore::GetInst()->init(g_hWnd, POINT{ 1600, 900 })))
	{
		return 0;
	}

	// Prefab Save Load 콜백 등록
	CPrefab::m_pSaveFunc = (CPrefab::SaveFunc)&CSceneSaveLoad::SavePrefab;
	CPrefab::m_pLoadFunc = (CPrefab::LoadFunc)&CSceneSaveLoad::LoadPrefab;

	CTestScene::CreateTestScene();
	CCore::GetInst()->progress();

	// Editor Object
	CToolObjMgr::GetInst()->init();

	// ImGui 초기화
	CImGuiMgr::GetInst()->init(g_hWnd);

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY39ENGINE));
	MSG    msg;

	// 메시지 루프입니다:
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (WM_QUIT == msg.message)
			{
				break;
			}
		}

		else
		{
			// Engine Update
			CCore::GetInst()->progress();

			// ToolObject 
			CToolObjMgr::GetInst()->progress();

			// ImGui Update, render
			CImGuiMgr::GetInst()->progress();
			CImGuiMgr::GetInst()->render();

			// Present
			CDevice::GetInst()->Present();
		}
	}

	CImGuiMgr::GetInst()->clear();

	return (int)msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = WndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = hInstance;
	wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY39ENGINE));
	wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName  = nullptr; // MAKEINTRESOURCEW(IDC_MY39ENGINE);
	wcex.lpszClassName = L"WindowClass";
	wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	g_hWnd = CreateWindowW(L"WindowClass",
	                       L"39Engine",
	                       WS_OVERLAPPEDWINDOW,
	                       CW_USEDEFAULT,
	                       0,
	                       CW_USEDEFAULT,
	                       0,
	                       nullptr,
	                       nullptr,
	                       hInstance,
	                       nullptr);

	if (!g_hWnd)
	{
		return FALSE;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	return TRUE;
}

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_DPICHANGED:
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
		{
			//const int dpi = HIWORD(wParam);
			//printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
			const RECT* suggested_rect = (RECT*)lParam;
			::SetWindowPos(hWnd,
			               NULL,
			               suggested_rect->left,
			               suggested_rect->top,
			               suggested_rect->right - suggested_rect->left,
			               suggested_rect->bottom - suggested_rect->top,
			               SWP_NOZORDER | SWP_NOACTIVATE);
		}
		break;


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
