/**
* @file WinApp.h
* @brief WindowsAPIに関するクラス
* @author イグチ_シュンペイ
*/

#include "WinApp.h"
#include <imgui_impl_win32.h>
#pragma comment(lib,"winmm.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

SIEngine::WinApp* SIEngine::WinApp::GetInstance()
{
	static WinApp instance;

	return &instance;
}

LRESULT SIEngine::WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// ImGui用ウィンドウプロシージャ呼び出し
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return true;

	//メッセージで分岐
	switch (msg)
	{
	case WM_DESTROY: //ウィンドウが破棄された
		PostQuitMessage(0); //ODに対してアプリの終了を伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//標準の処理を行う
}

void SIEngine::WinApp::Initialize()
{
	
	//ウィンドウクラスの設定
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;     //ウィンドウプロシージャを設定
	w.lpszClassName = L"SPACEASSAULT";		 //ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr);	 //ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW); //カーソル指定

	//ウィンドウクラスをOSに登録する
	RegisterClassEx(&w);
	//ウィンドウサイズ{ X座標 Y座標 横幅 立幅 }
	RECT wrc = { 0,0,window_width,window_height };
	//自動でサイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウオブジェクトの生成
	hwnd_ = CreateWindow(w.lpszClassName,//クラス名)
		L"SPACEASSAULT",         //タイトルバーの表示
		WS_OVERLAPPEDWINDOW,    //標準的なウィンドウスタイル
		CW_USEDEFAULT,          //表示X座標(OSに任せる)
		CW_USEDEFAULT,          //表示Y座標(OSに任せる)
		wrc.right - wrc.left,   //ウィンドウ横幅
		wrc.bottom - wrc.top,   //ウィンドウ立幅
		nullptr,				//親ウィンドウハンドル
		nullptr,				//メニューハンドル
		w.hInstance,			//呼び出しアプリケーションハンドル
		nullptr);				//オプション

	//ウィンドウを表示状態にする
	ShowWindow(hwnd_, SW_SHOW);

	//システムタイマーの分解能を上げる
	timeBeginPeriod(1);
}

void SIEngine::WinApp::Finalize()
{
	//ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);
}

bool SIEngine::WinApp::ProcessMessage()
{

	MSG msg{}; //メッセージ	

	//メッセージがある？
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);  //キー入力メッセージの処理
		DispatchMessage(&msg);   //プロシージャにメッセージを送る
	}

	//×ボタンで終了メッセージが来たらゲームループを抜ける
	if (msg.message == WM_QUIT)
	{
		return true;
	}

	return false;
}