#pragma once
#include "../Adapter/Adapter.h"
#include "IEnemy.h"
#include "Childs/Minor/Childs/Saiji/Saiji.h"
#include "Childs/Minor/Childs/OniHayha/OniHayha.h"
#include "Childs/Boss/Childs/Ogre/Ogre.h"
#include <list>
#include <memory>
#include "SpawnData.h"
#include "../UI/DamageEffectEmitter.h"
#include "ParameterEditor.h"

class Player;
class SEPlayer;
class World;

/// <summary>
/// 全ての敵を管理するクラス
/// </summary>
class EnemyManager
{
public:

	~EnemyManager();

	//初期化
	void Initialize();
	//終了処理
	void Finalize();
	//更新
	void Update();
	//プレイヤーをセット
	void SetPlayer(Player* player) { player_ = player; }
	//SEPlayerセット
	void SetSEPlayer(SEPlayer* sePlayer) { sePlayer_ = sePlayer; }
	//Worldセット
	void SetWorld(World* world) { world_ = world; }
	//カメラセット
	void SetCamera(LWP::Object::Camera* camera) { camera_ = camera; }
	//ダメージエフェクトエミッターのセット
	void SetDamageEffectEmitter(DamageEffectEmitter* emitter) { damageEffectEmitter_ = emitter; }
	//ダメージエフェクトエミッター取得
	DamageEffectEmitter& GetDamageEffectEmitter() { return *damageEffectEmitter_; }
	//敵追加、配置
	void CreateEnemy(const Vector3& position, EnemyType type);
	//リストのクリア
	void ClearList();
	//デバッグウィンドウ
	void DebugGUI();
	//リスト取得
	std::list<IEnemy*>* GetEnemyListPtr() { return &enemies_; }
	//リストを距離順にソート(昇順。後半になるにつれて距離が長くなる)
	void SortAscendingDistanceList();
	/// <summary>
	/// 敵の攻撃パラメータを取得する関数
	/// </summary>
	/// <param name="name">コライダー名</param>
	/// <returns></returns>
	const EnemyAttackParameter& GetEnemyAttackParameter(const std::string& name) const;
	//全ての敵が倒されたフラグ
	bool GetIsDefeatedAllEnemy() const { return isDefeatedAllEnemy_; }
	//ウェーブ開始させる
	void StartWave();
	//誰か一人でも攻撃しているかどうか
	bool IsAnyAttack();
	//同一攻撃タイプの中で誰かが攻撃しているか
	bool IsAnyAttackWithinType(AttackType type);
	//攻撃態勢人数取得
	uint16_t GetAttackPhaseCount();
	//ウェーブ開始フラグ取得
	bool GetIsStartWave() const { return isStartWave_; }
	//スポーンデータ表示非表示切り替え
	void SetIsShowSpawnDataModel(bool flag){ spawnData_.SetIsShowModel(flag); }
	//パリィエフェクトの発生タイミング取得
	float GetParryEffectOccurTime() const { return parameterEditor_.GetParryEffectOccurTime(); }

	//次の近距離敵攻撃ID
	uint16_t shortNextAttackID = 0;
	//近距離敵攻撃に割り当てるID
	uint16_t shortAssignAttackID = 0;
	//次の遠距離敵攻撃ID
	uint16_t longNextAttackID = 0;
	//遠距離敵攻撃に割り当てるID
	uint16_t longAssignAttackID = 0;

private:

	//ウェーブデータから敵をスポーンさせる
	void SpawnFromWaveData(WaveData& waveData);
	//スポーンするかどうかをチェック
	void CheckIsSpawn();
	//全ウェーブ終了時の処理
	void EndGame();
	//最新のパラメータを適用させる
	void ApplyLatestParameter();

private:
	//プレイヤーのポインタ
	Player* player_;
	//SEPlayerのポインタ
	SEPlayer* sePlayer_;
	//Worldポインタ
	World* world_;
	//ダメージエフェクトエミッターのポインタ
	DamageEffectEmitter* damageEffectEmitter_;
	//カメラのポインタ
	LWP::Object::Camera* camera_ = nullptr;

	//全ての敵を管理するリスト
	std::list<IEnemy*> enemies_;
	//出現管理データ
	SpawnData spawnData_;
	//パラメータ調整クラス
	EnemyConfig::ParameterEditor parameterEditor_;

	//json
	SaijiState::ParameterConfig saijiParameter_;
	OniHayhaState::ParameterConfig oniHayhaParameter_;
	OgreState::ParameterConfig ogreParameter_;

#pragma region デバッグ用

	//スポーン座標指定
	Vector3 spawnPoint_;

#pragma endregion


	//敵が互いに取る距離
	float enemyDist_ = 3.0f;
	//攻撃する敵同士が取る距離
	float attackEnemyDist_ = 1.5f;
	//プレイヤーと取る距離
	float playerDist_ = 0.8f;
	//敵の出現上限
	const uint16_t kMaxEnemyCount_ = 20;
	//スポーンデータの敵を表示するフラグ
	bool isShowSpawnDataModel_ = true;
	//ウェーブを開始するかどうか
	bool isStartWave_ = false;
	//全ての敵が倒されたフラグ
	bool isDefeatedAllEnemy_ = false;

};

