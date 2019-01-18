//=============================================================================
//
// エネミーバレットヘッダ [enemyBullet.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _ENEMYBULLET_H_
#define _ENEMYBULLET_H_


// マクロ定義
#define ENEMYBULLET_TEXTURE_MAIN	_T("data/TEXTURE/EnemyBullet/enemybullet.png")			// エネミーバレットのテクスチャ
#define ENEMYBULLET_TEXTURE_LUMINE	_T("data/TEXTURE/EnemyBullet/enemybulletLumine3.png")	// エネミーバレットのテクスチャ(光)

#define ENEMYBULLET_TEXTURE_SIZE_X					(60)					// テクスチャサイズX
#define ENEMYBULLET_TEXTURE_SIZE_Y					(60)					// テクスチャサイズY

#define ENEMYBULLET_TEXTURE_DIVIDE_X				(4)						// アニメパターンのテクスチャ内分割数（X)
#define ENEMYBULLET_TEXTURE_DIVIDE_Y				(6)						// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_ENEMYBULLET_NUM						(4)						// アニメーションパターン数
#define TIMEENEMYBULLET_ANIMATION					(4)						// アニメーションの切り替わるカウント


#define	ENEMYBULLET_RADIUS_MIN						(50.0f)					// ポリゴンの半径最小値
#define	ENEMYBULLET_RADIUS_MAX						(300.0f)				// ポリゴンの半径最大値
#define	ENEMYBULLET_VALUE_ROTATION					(D3DX_PI * 0.01f)		// ポリゴンの回転量
#define	ENEMYBULLET_VALUE_SCALE						(2.0f)					// ポリゴンのスケール変化量                                                                                          

#define ENEMYBULLET_NUM_MAX							(1024)					//エネミーバレット(1種類)の最大数
#define ENEMYBULLET_KIND_MAX						(6)						//エネミーバレットの種類の最大数
#define ENEMYBULLET_MAX		(ENEMYBULLET_NUM_MAX*ENEMYBULLET_KIND_MAX)		//エネミーバレット全体の最大数

#define ENEMYBULLET_INITPOS_X						(-100)					//エネミーバレットの初期X座標
#define ENEMYBULLET_INITPOS_Y						(-100)					//エネミーバレットの初期Y座標

#define ENEMYBULLET_NUM_TEXTURE						(2)

//エネミーバレットのバウンディングボックスの定義
#define ENEMYBULLET_BB_TOP							(0.0f)
#define ENEMYBULLET_BB_LEFT							(0.0f)
#define ENEMYBULLET_BB_WIDTH						(8.0f)
#define ENEMYBULLET_BB_HIGHT						(8.0f)
#define ENEMYBULLET_BC_RADIUS						(8.0f)

#define ENEMYBULLET_DAFAULT_ANGLE					(1.5707961f)			//Y軸に対して水平に発射したい時の角度
//*****************************************************************************
// 構造体定義
//*****************************************************************************
//バレットタイプを表す列挙体
enum BULLET_TYPE
{
	GREEN_ROTATE,
	GREEN_WHEEL,
	GREEN_CIRCLE,
	ORANGE_ROTATE,
	ORANGE_WHEEL,
	ORANGE_CIRCLE,
	BULLET_TYPE_MAX
};

//エネミーバレットの速度タイプを表す列挙体
enum SPEEDTYPE
{
	DEFAULT_SPEED,										//オレンジ、緑の両方が通常速度
	SLOW_ORANGE,										//オレンジが遅く、緑が速い
	SLOW_GREEN,											//オレンジが速く、緑が遅い
	SLOW_BOTH,											//オレンジと緑の両方が遅い
	FAST_BOTH,											//オレンジと緑の両方が速い
	SPEEDTYPE_MAX
};

//エネミーバレット構造体
typedef struct 
{
	bool					active;						//使用or未使用の判定

	D3DXVECTOR3				pos;						// ポリゴンの座標
	D3DXVECTOR3				rot;						// ポリゴンの回転量

	float					angle;						//頂点への回転角
	float					radius;						//ポリゴンの半径

	int						countAnim;					// アニメーションカウント
	int						patternAnim;				// アニメーションパターンナンバー

	D3DXVECTOR3				velocity;					//移動速度
	BULLET_TYPE				type;						//表示するテクスチャのタイプ

	int*					parentHp;					//バレットを発射したエネミーのHP
}ENEMYBULLET;

//エネミーバレット発射時に引数で渡すエネミーアタック構造体(type, pos, speed, angl)
typedef struct
{
	BULLET_TYPE type;									//発射するエネミーバレットのタイプ
	D3DXVECTOR3 pos;									//発射位置
	float speed;										//発射速度
	float angle;										//発射角度（0でY軸に水平に飛ぶものとする）	
}ENEMYATTACK;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemyBullet(int num);									//エネミーバレット初期化処理
void UninitEnemyBullet(void);										//エネミーバレット終了処理
void UpdateEnemyBullet(void);										//エネミーバレット更新処理
void DrawEnemyBullet(void);											//エネミーバレット描画処理
ENEMYBULLET *GetEnemyBulletAdr(BULLET_TYPE type, int pno);			//エネミーバレットアドレス取得関数
ENEMYBULLET *SetEnemyBullet(ENEMYATTACK *param, int *parentHp);	//エネミーバレットセット関数
SPEEDTYPE GetSpeedType(void);										//速度タイプを取得する関数
void SetSpeedType(SPEEDTYPE type);									//速度タイプを指定する関数
void FireCircleBullet(ENEMYATTACK *param, int way, int *parentHp);				//円状に弾を発射する関数
void FireWayBullet(ENEMYATTACK *param, int way, float range,int *parentHP);		//way状に弾を発射する関数
void DisableEnemyBullet(ENEMYBULLET *ptr);							//エネミーバレットを非アクティブにセットする関数
void DisableAllEnemyBullet(void);									//全てのエネミーバレットを非アクティブにセットする関数
void ConvertBulletToScoreItem(void);								//全てのエネミーバレットをスコアアイテムに変換する関数

#ifdef _DEBUG
int GetEnemyBulletCnt(void);										//ゲーム中でアクティブなエネミーバレットの数を取得する関数
#endif

#endif
