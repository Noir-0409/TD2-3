#include <KamataEngine.h>
#include "GameScene.h"

#include "Title.h"
#include "Rule.h"
#include "ClearScene.h"
#include "OverScene.h"

#include <2d\ImGuiManager.h>
#include <3d\AxisIndicator.h>
#include <3d\PrimitiveDrawer.h>
#include <audio\Audio.h>
#include <base\DirectXCommon.h>
#include <base\TextureManager.h>
#include <base\WinApp.h>

using namespace KamataEngine;

Title* titleScene = nullptr;
Rule* ruleScene = nullptr;
GameScene* gameScene = nullptr;
ClearScene* clearScene = nullptr;
OverScene* overScene = nullptr;

// シーン（型）
enum class Scene {
	kUnknown = 0,

	kTitle,
	kRule, // アクション
	kGame,
	kClear,
	kGameOver
};
// 現在シーン（型）
Scene scene = Scene::kTitle;

void ChangeScene();
void UpdateScene();
void DrawScene();

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;


	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"2344_惑星探索");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();


#pragma endregion

	// 最初のシーンの初期化
	scene = Scene::kTitle;
	// タイトルシーンの初期化
	titleScene = new Title();
	titleScene->Initialize();

#ifdef _DEBUG
	//// 最初のシーンの初期化
	//scene = Scene::kGame;
	//// タイトルシーンの初期化
	//delete  titleScene;
	//titleScene = nullptr;
	//gameScene = new GameScene();
	//gameScene->Initialize();
#endif // _DEBUG



	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();
		// シーン切り替え
		ChangeScene();

		// 現在のシーン
		UpdateScene();

		// 軸表示の更新
		axisIndicator->Update();
		// ImGui受付終了
		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();
		// 現在シーンの描画
		DrawScene();
		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	delete ruleScene;
	delete gameScene;
	delete clearScene;
	delete overScene;
	delete titleScene;

	// 3Dモデル解放
	Model::StaticFinalize();
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}

//void ChangeScene() {
//	switch (scene) {
//	case Scene::kTitle:
//		if (titleScene->IsFinished()) {
//			// シーン変更
//			scene = Scene::kRule;
//			// 旧シーンの解放
//			delete titleScene;
//			titleScene = nullptr;
//			// 新しいシーンの生成と初期化
//			ruleScene = new Rule;
//			ruleScene->Initialize();
//		}
//		break;
//	case Scene::kRule:
//		if (ruleScene->IsFinished()) {
//			// シーン変更
//			scene = Scene::kGame;
//			// 旧シーンの解放
//			delete ruleScene;
//			ruleScene = nullptr;
//			// 新しいシーンの生成と初期化
//			gameScene = new GameScene;
//			gameScene->Initialize();
//		}
//		break;
//	case Scene::kGame:
//		if (gameScene->IsCleared()) {
//			// シーン変更
//			scene = Scene::kClear;
//			// 旧シーンの解放
//			delete gameScene;
//			gameScene = nullptr;
//			// 新しいシーンの生成と初期化
//			clearScene = new ClearScene();
//			clearScene->Initialize();
//
//		}else if (gameScene->IsFinished()) {
//
//			//シーン変更
//			scene = Scene::kGameOver;
//			//旧シーンの解放
//			delete gameScene;
//			gameScene = nullptr;
//			//新しいシーンの生成と初期化
//			overScene = new OverScene();
//			overScene->Initialize();
//
//		}
//		break;
//	case Scene::kClear:
//		if (clearScene->IsFinished()) {
//			//シーン変更
//			scene = Scene::kGame;
//			//旧シーンの解放
//			delete clearScene;
//			clearScene = nullptr;
//			//新しいシーンの生成と初期化
//			/*gameScene = new GameScene();
//			gameScene->Initialize();*/
//
//			titleScene = new Title();
//			titleScene->Initialize();
//
//		}
//		break;
//	case Scene::kGameOver:
//		if (overScene->IsFinished()) {
//			//シーン変更
//			scene = Scene::kGame;
//			//旧シーンの解放
//			delete overScene;
//			overScene = nullptr;
//			//新しいシーンの生成と初期化
//			/*gameScene = new GameScene();
//			gameScene->Initialize();*/
//
//			titleScene = new Title();
//			titleScene->Initialize();
//
//		}
//
//	}
//
//
//}

void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsFinished()) {
			// シーン変更
			scene = Scene::kRule;
			// 旧シーンの解放
			delete titleScene;
			titleScene = nullptr;
			// 新しいシーンの生成と初期化
			ruleScene = new Rule;
			ruleScene->Initialize();
		}
		break;
	case Scene::kRule:
		if (ruleScene->IsFinished()) {
			// シーン変更
			scene = Scene::kGame;
			// 旧シーンの解放
			delete ruleScene;
			ruleScene = nullptr;
			// 新しいシーンの生成と初期化
			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
		if (gameScene->IsCleared()) {
			// シーン変更
			scene = Scene::kClear;
			// 旧シーンの解放
			delete gameScene;
			gameScene = nullptr;
			// 新しいシーンの生成と初期化
			clearScene = new ClearScene();
			clearScene->Initialize();

		} else if (gameScene->IsFinished()) {
			// シーン変更
			scene = Scene::kGameOver;
			// 旧シーンの解放
			delete gameScene;
			gameScene = nullptr;
			// 新しいシーンの生成と初期化
			overScene = new OverScene();
			overScene->Initialize();

		}
		break;
	case Scene::kClear:
		if (clearScene->IsFinished()) {
			// シーン変更
			scene = Scene::kTitle;  // タイトルシーンに遷移
			// 旧シーンの解放
			delete clearScene;
			clearScene = nullptr;
			// 新しいシーンの生成と初期化
			titleScene = new Title();  // 新しいタイトルシーンを生成
			titleScene->Initialize();
		}
		break;
	case Scene::kGameOver:
		if (overScene->IsFinished()) {
			// シーン変更
			scene = Scene::kTitle;  // タイトルシーンに遷移
			// 旧シーンの解放
			delete overScene;
			overScene = nullptr;
			// 新しいシーンの生成と初期化
			titleScene = new Title();  // 新しいタイトルシーンを生成
			titleScene->Initialize();
		}
		break;
	}
}


void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kRule:
		ruleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	case Scene::kClear:
		clearScene->Update();
		break;
	case Scene::kGameOver:
		overScene->Update();
		break;
	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kRule:
		ruleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kClear:
		clearScene->Draw();
		break;
	case Scene::kGameOver:
		overScene->Draw();
		break;
	}
}