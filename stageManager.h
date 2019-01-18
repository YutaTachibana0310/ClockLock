//=============================================================================
//
// ステージマネージャーヘッダ [StageManager.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _STAGEMANAGER_H_
#define _STAGEMANAGER_H_

// マクロ定義
#define STAGEMANAGER_DATA_PATH ("data/STAGE/StageData.dat")				//ステージ情報が保存されたファイルへのパス
#define STAGEDATA_MAX			(256)						//ステージデータ最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
//ステージに出現する敵パターンの構造体
typedef struct {
	int				interbal;					//出現間隔
	int				enemyType;					//出現する敵のタイプ

	D3DXVECTOR3		initPos;					//出現座標
	D3DXVECTOR3		targetPos;					//目標座標
}STAGEDATA;

//ステージマネージャー構造体
typedef struct {
	int				cntFrame;					//ステージの経過フレーム数
	int				rank;						//ランク（難易度）
	int				lastPopFrame;				//最後に敵をポップしたフレーム

	STAGEDATA		data[STAGEDATA_MAX];		//敵パターン構造体へのポインタ
	int				dataMax;					//読み込んだ敵パターンの数
	int				popNum;						//ポップした敵パターンの数
	int				state;						//ステージ状態
	int				combo;						//コンボ数

	int				scaleState;
	float			comboScale;

}STAGEMANAGER;

enum STAGE_STATE
{
	STAGE_DEFAULT,
	STAGE_PREBOSS,
	STAGE_BOSSBATTLE,
	STAGE_CLEAR,
	STAGE_STATEMAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitStageManager(int num);							//ステージマネージャー初期化処理
void UninitStageManager(void);								//ステージマネージャー終了処理
void UpdateStageManager(void);								//ステージマネージャー更新処理
void DrawStageManager(void);								//ステージマネージャー描画処理
STAGEMANAGER *GetStageManagerAdr(void);						//ステージマネージャーアドレス取得関数
void SetStageState(int state);								//ステートセット関数
void AddComboCount(int value);								//コンボ数セット関数
#endif
