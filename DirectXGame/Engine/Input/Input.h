/**
* @file Input.cpp
* @brief 入力による処理をまとめたクラス
* @author イグチ_シュンペイ
*/

#pragma once
#define DIRECTINPUT_VERSION    0x0800 //DirectInputのバージョン指定

#include<windows.h>
#include<wrl.h>
#include"WinApp.h"
#include<dinput.h>

namespace SIEngine
{
	class Input
	{
	public:
		static Input* GetInstance();

	private:
		//コンストラクタ
		Input() = default;
		//デストラクタ
		~Input() = default;
		//コピー封印
		Input(const Input&) = delete;
		//コピー封印
		Input& operator=(const Input&) = delete;

	public:
		//namespace省略
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	public: //メンバ関数
		//初期化
		void Initialize(SIEngine::WinApp* winApp);
		//更新
		void Update();

		//長押し
		bool PushKey(BYTE keyNumber);

		//押した瞬間
		bool TriggerKey(BYTE keyNumber);

		//離した瞬間
		bool ReleasedKey(BYTE keyNumber);
	private: //メンバ変数

		// DirectInputのインスタンス
		ComPtr<IDirectInput8> directInput = nullptr;

		//キーボードのデバイス
		ComPtr<IDirectInputDevice8> keyboard = nullptr;

		//全キーの状態
		BYTE key[256] = {};

		// 前回の全キーの状態
		BYTE keyPre[256] = {};

		//WindowsAPI
		SIEngine::WinApp* winApp = nullptr;
	};
}