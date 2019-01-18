//=============================================================================
//
// エネミーバレットエクスプロージョンヘッダ [EnemyBulletExplosion.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _EBULLETEXPLOSION_H_
#define _EBULLETEXPLOSION_H_


// マクロ定義
#define EBULLETEXPLOSION_TEXTURE_NAME	_T("data/TEXTURE/effect/eneyBulletExplosion.png")	// エネミーバレットエクスプロージョンのテクスチャ

#define EBULLETEXPLOSION_TEXTURE_SIZE_X					(50)					// テクスチャサイズX
#define EBULLETEXPLOSION_TEXTURE_SIZE_Y					(50)					// テクスチャサイズY

#define EBULLETEXPLOSION_TEXTURE_DIVIDE_X				(11)						// アニメパターンのテクスチャ内分割数（X)
#define EBULLETEXPLOSION_TEXTURE_DIVIDE_Y				(2)						// アニメパターンのテクスチャ内分割数（Y)
#define EBULLETEXPLOSION_ANIM_NUM						(11)					// アニメーションパターン数
#define EBULLETEXPLOSION_TIME_ANIMATION					(2)						// アニメーションの切り替わるカウント

#define EBULLETEXPLOSION_MAX							(ENEMYBULLET_MAX)					//エネミーバレットエクスプロージョンの最大数

#define EBULLETEXPLOSION_INITPOS_X						(-100)					//エネミーバレットエクスプロージョンの初期X座標
#define EBULLETEXPLOSION_INITPOS_Y						(-100)					//エネミーバレットエクスプロージョンの初期Y座標

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	bool					active;							//使用or未使用の判定

	D3DXVECTOR3				pos;						// ポリゴンの座標
	D3DXVECTOR3				rot;						// ポリゴンの回転量
	int						countAnim;						// アニメーションカウント
	int						patternAnim;					// アニメーションパターンナンバー

	float					radius;							// ポリゴンの半径
	float					angle;						// ポリゴンの角度
	int						type;
}EBULLETEXPLOSION;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemyBulletExplosion(int num);							//エネミーバレットエクスプロージョン初期化処理
void UninitEnemyBulletExplosion(void);								//エネミーバレットエクスプロージョン終了処理
void UpdateEnemyBulletExplosion(void);								//エネミーバレットエクスプロージョン更新処理
void DrawEnemyBulletExplosion(void);								//エネミーバレットエクスプロージョン描画処理
EBULLETEXPLOSION *GetEnemyBulletExplosionAdr(int pno);				//エネミーバレットエクスプロージョンアドレス取得関数
void SetEnemyBulletExplosion(D3DXVECTOR3 pos, BULLET_TYPE type);	//エネミーバレットエクスプロージョンセット関数

#endif
