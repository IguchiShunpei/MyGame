#pragma once
#include <windows.h>
#include <wrl.h>
#include <dinput.h>
#include "WinApp.h"
#include <dwrite.h>
#include <wchar.h>
#include <wrl/client.h>
#include <random>
#include <xinput.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment (lib, "xinput.lib")

#define MaxCountrollers 4  
#define MaxVibration 65535

// 入力
class Input final
{
public:
	static Input* GetInstance();

public:
	struct CountrolerState
	{
		XINPUT_STATE state; // コントローラーの状態の取得
		XINPUT_VIBRATION vibration;  // バイブレーション
		//bool Connected;
	};
	CountrolerState GamePad;

public:
	// namespace
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // メンバ関数
	// 初期化
	void Initialize(WinApp* winApp);

	// 更新
	void Update();

	/// キーの押下をチェック
	bool PushKey(BYTE keyNumber);

	/// キーのトリガーをチェック
	bool TriggerKey(BYTE keyNumber);

private:
	Input() = default;
	~Input() = default;
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

private: // メンバ変数

	// キーボードのデバイス
	ComPtr<IDirectInputDevice8> keyboard;
	// DirectInputのインスタンス
	ComPtr<IDirectInput8> directInput;
	// 全キーの状態
	BYTE key[256] = {};
	// 前回の全キーの状態
	BYTE keyPre[256] = {};
	// WindowsAPI
	WinApp* winApp = nullptr;
};