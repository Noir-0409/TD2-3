#pragma once

// シーン（型）
enum class Scene {
	kUnknown = 0,

	kTitle,
	kRule, // アクション
	kGame,
	kClear,
	kGameOver
};