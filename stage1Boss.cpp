//=============================================================================
//
// ステージ1ボス処理 [stage1Boss.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "enemyManager.h"
#include "player.h"
#include "gameClear.h"
#include "particle.h"
#include "stageManager.h"
#include "bgmPlayer.h"
#include "sePlayer.h"
#include "GUI.h"
#include "enemyParticle.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define STAGE1BOSS_SPEED (3.5f)

#define STAGE1BOSS_INITHP					(5500)		//初期HP
#define STAGE1BOSS_SHADOW_OFFSET			(D3DXVECTOR3(50.0f, 100.0f, 0.0f))		//影のオフセット座標
#define STAGE1BOSS_TARGETPOS				(D3DXVECTOR3(SCREEN_CENTER_X, 200.0f, 0.0f))	//移動の目標座標

#define STAGE1BOSS_COUNTMOVE_MIN			(0)			//カウントムーブの最小値
#define STAGE1BOSS_COUNTMOVE_MAX			(720)		//カウントムーブの最大値
#define STAGE1BOSS_COUNTMOVE_BORDER			(360)		//移動方向が切り替わるしきい値
#define STAGE1BOSS_MOVE_LENGTH_X			(0.7f)		//移動のX幅
#define STAGE1BOSS_MOVE_LENGTH_Y			(0.1f)		//移動のY幅

#define STAGE1BOSS_WAY1_ATKMAX				(6)			//WAY1攻撃を行う回数
#define STAGE1BOSS_WAY1_SHOTPOS				(D3DXVECTOR3(-100.0f, -100.0f, 0.0f))	//WAY攻撃の発射位置
#define STAGE1BOSS_WAY1_ANGLE				(70.0f)		//WAY1攻撃の発射角度
#define STAGE1BOSS_WAY1_SPEED				(5.0f)		//WAY1攻撃のバレットスピード
#define STAGE1BOSS_WAY1_INTERBAL			(60)		//WAY1攻撃の間隔
#define STAGE1BOSS_WAY1_ORANGE_WAY			(5)			//WAY1攻撃のオレンジの弾のWAY数
#define STAGE1BOSS_WAY1_ORANGE_RANGE		(80.0f)		//WAY1攻撃のオレンジの弾の角度
#define STAGE1BOSS_WAY1_GREEN_WAY			(4)			//WAY1攻撃の緑の玉のWAY数
#define STAGE1BOSS_WAY1_GREEN_RANGE			(60.0f)		//WAY1攻撃の緑の玉の角度
#define STAGE1BOSS_WAY1_LOOPMAXEASY			(5)			//WAY1攻撃で1回に放つ弾の数（EASY)
#define STAGE1BOSS_WAY1_LOOPMAXNORMAL		(10)		//WAY1攻撃で1回に放つ弾の数（NORMAL)
#define STAGE1BOSS_WAY1_LOOPMAXHARD			(20)		//WAY1攻撃で1回に放つ弾の数（HARD)
#define STAGE1BOSS_WAY1_SPEEDOFFSET			(0.2f)		//WAY1攻撃で撃つ弾の速度差分

#define STAGE1BOSS_WAY2_SHOTPOS				(D3DXVECTOR3(100.0f, -100.0f, 0.0f))	//WAY2攻撃の発射位置
#define STAGE1BOSS_WAY2_INTERBAL			(120)		//WAY2攻撃の発射間隔

#define STAGE1BOSS_WIP1_SHOTPOS_L			(D3DXVECTOR3(-170.0f, 180.0f, 0.0f))	//WIP1攻撃の発射位置(左）
#define STAGE1BOSS_WIP1_SHOTPOS_R			(D3DXVECTOR3(170.0f, 180.0f, 0.0f))		//WIP1攻撃の発射位置（右）
#define STAGE1BOSS_WIP1_RANGEEASY			(15)		//WIP1攻撃のWAYの角度（EASY)
#define STAGE1BOSS_WIP1_RANGENORMAL			(30)		//WIP1攻撃のWAYの角度（NORMAL)
#define STAGE1BOSS_WIP1_RANGEHARD			(45)		//WIP1攻撃のWAYの角度（HARD)
#define STAGE1BOSS_WIP1_ATKMAX				(72)		//WIP1攻撃の1ループ最大攻撃回数
#define STAGE1BOSS_WIP1_INTERBAL			(1)			//WIP1攻撃の間隔
#define STAGE1BOSS_WIP1_SPEED				(2.5f)		//WIP1攻撃のバレットスピード
#define STAGE1BOSS_WIP1_SPEEDOFFSET			(0.02f)		//WIP1攻撃のバレットスピード差分
#define STAGE1BOSS_WIP1_ROTANGLE			(5.0f)		//WIP1攻撃の回転角度
#define STAGE1BOSS_WIPMAX					(4*STAGE1BOSS_WIP1_INTERBAL*STAGE1BOSS_WIP1_ATKMAX)	//WIP1攻撃のループ最大回数
#define STAGE1BOSS_WIP_PERIOD				(12)		//WIP攻撃の周期
#define STAGE1BOSS_WIP_BORDER				(STAGE1BOSS_WIP_PERIOD/2)	//WIP1攻撃でオレンジと緑が切り替わるタイミング
#define STAGE1BOSS_WIP_EASYBORDER1			(3)			//このフレーム数以下の時、EASYでは攻撃しない
#define STAGe1BOSS_WIP_EASYBORDEE2			(9)			//このフレーム数以上の時、EASYでは攻撃しない

#define STAGE1BOSS_CIRCLE_SHOTPOS			(D3DXVECTOR3(0.0f, -50.0f, 0.0f))	//CIRCLE攻撃の発射位置
#define STAGE1BOSS_CIRCLE_INTERBAL			(3)			//CIRCLE攻撃の発射間隔
#define STAGE1BOSS_CIRCLE_SPEED				(6.0f)		//CIRCLE攻撃のバレットスピード
#define STAGE1BOSS_CIRCLE_ATKMAX			(239)		//CIRCLE攻撃の攻撃最大回数
#define STAGE1BOSS_CIRCLE_ROTANGLE			(0.3f)		//CIRCLE攻撃の回転角度
#define STAGE1BOSS_CIRCLE_PERIODEASY		(20*STAGE1BOSS_CIRCLE_INTERBAL)	//CIRCLE攻撃の周期（EASY）
#define STAGE1BOSS_CIRCLE_PERIODNOMAL		(16*STAGE1BOSS_CIRCLE_INTERBAL)	//CIRCLE攻撃の周期（NORMAL）
#define STAGE1BOSS_CIRCLE_PERIODHARD		(10*STAGE1BOSS_CIRCLE_INTERBAL)	//CIRCLE攻撃の周期（HARD)
//#define STAGE1BOSS_CIRCLE_BORDER			(STAGE1BOSS_CIRCLE_PERIOD/2)	//CIRCLE攻撃のオレンジと緑が切り替わるタイミング	
#define STAGE1BOSS_CIRCLE_WAY				(8)			//CIRCLE攻撃のWAY数
#define STAGE1BOSS_CIRCLE_WAYHARD			(12)		//CIRCLE攻撃のWAY数（HARD）
#define STAGE1BOSS_CIRCLE_EASYBORDER1		(5*STAGE1BOSS_CIRCLE_INTERBAL)			//このフレーム数以下の時、EASYではCIRCLE攻撃しない
#define STAGE1BOSS_CIRCLE_EASYBORDER2		(15*STAGE1BOSS_CIRCLE_INTERBAL)		//このフレーム数以上の時、EASYではCIRCLE攻撃しない

#define STAGE1BOSS_SCORE					(10000)		//撃墜時のスコア

#define STAGE1BOSS_DESTROYED_FRAMEMAX		(180)		//DESTROY状態から遷移する時間
#define STAGE1BOSS_DESTROYED_EXPLRANGE		(150.0f)	//DESTROY状態で爆発を生成する座標の範囲
#define STAGE1BOSS_DESTROYED_EXPLFRAME		(180)		//DESTROY状態で爆発を生成し続ける時間
#define STAGE1BOSS_DESTROYED_FXPLINTERBAL	(10)		//DESTROY状態で爆発を生成する間隔

#define STAGE1BOSS_INTERBAL_FRAME			(120)		//INTERBAL状態から遷移する時間
#define STAGE1BOSS_BGM_FADEOUTFRAME			(60)		//BGMがフェードアウトする秒数

#define STAGE1BOSS_QUAKE_LENGTH				(0.01f)		//DESTRYOED状態で設定するテクスチャ座標のオフセット
#define STAGE1BOSS_EXPLOSION_RANGE_X		(300.0f)	//撃墜時に爆発をセットする範囲（X)
#define STAGE1BOSS_EXPLOSION_RANGE_Y		(150.0f)	//撃墜時に爆発をセットする範囲（Y)
#define STAGE1BOSS_EXPLOSION_NUM			(5)			//撃墜時にセットする爆発の数

#define BOSSFIRE_EMITPARTICLE_NUM	(10)
#define BOSSFIRE_EMITOARTUCLE_DURATION	(5)

#define BOSSEXPLOSION_EMITPARTICLE_NUM	(50)
#define BOSSEXPLOSION_EMITPARTICLE_DURATION (45)

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexStage1Boss(void);				//頂点作成関数
void SetTextureStage1Boss(STAGE1BOSS *ptr);		// テクスチャ座標の計算処理
void SetTextureStage1BossShadow(STAGE1BOSS *ptr);
void SetVertexStage1Boss(STAGE1BOSS *ptr);		// 頂点の計算処理
void SetVertexStage1BossShadow(STAGE1BOSS *ptr);

//各描画処理
void DrawStage1BossShadow(LPDIRECT3DDEVICE9 pDevice);
void DrawStage1BossBody(LPDIRECT3DDEVICE9 pDevice);

//各状態の行動処理
void ActInitStage1Boss(STAGE1BOSS *ptr);
void ActInterbalStage1Boss(STAGE1BOSS *ptr);

void ActWay1Stage1Boss(STAGE1BOSS *ptr);
void AttackWay1(STAGE1BOSS *ptr, int side);

void ActWip1Stage1Boss(STAGE1BOSS *ptr);
void AttackWip1(STAGE1BOSS *ptr, int frame, int side);
void ActWip2Stage1Boss(STAGE1BOSS *ptr);

void ActCircleStage1Boss(STAGE1BOSS *ptr);
void AttackCircleStage1Boss(STAGE1BOSS *ptr, int frame);

void ActDestroyedStage1Boss(STAGE1BOSS *ptr);

void MoveStage1Boss(STAGE1BOSS *ptr);			//移動処理
void DestroyStage1Boss(STAGE1BOSS *ptr);		//撃墜処理
void ExplodeStage1Boss(STAGE1BOSS *ptr);		//爆発処理

/*****************************************************************************
構造体定義
*****************************************************************************/
enum STAGE1BOSS_TEXTURE
{
	STAGE1BOSS_SHADOW,
	STAGE1BOSS_BODY,
	STAGE1BOSS_TEXMAX
};

enum
{
	ATK_RIGHT,
	ATK_LEFT
};

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[STAGE1BOSS_TEXMAX];	// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク

STAGE1BOSS stage1Boss;									//ステージ1ボス構造体

//攻撃時の弾のパラメータ
const static ENEMYATTACK bulletWay1Orange = 
{
	ORANGE_CIRCLE,
	STAGE1BOSS_WAY1_SHOTPOS,
	STAGE1BOSS_WAY1_SPEED,
	RADIAN(STAGE1BOSS_WAY1_ANGLE)
};

const static ENEMYATTACK bulletWay1Green =
{
	GREEN_CIRCLE,
	STAGE1BOSS_WAY1_SHOTPOS,
	STAGE1BOSS_WAY1_SPEED,
	RADIAN(STAGE1BOSS_WAY1_ANGLE)
};

const static ENEMYATTACK bulletWip1Orange =
{
	ORANGE_ROTATE,
	STAGE1BOSS_WIP1_SHOTPOS_L,
	STAGE1BOSS_WIP1_SPEED,
	ENEMYBULLET_DAFAULT_ANGLE
};

const static ENEMYATTACK bulletWip1Green =
{
	GREEN_ROTATE,
	STAGE1BOSS_WIP1_SHOTPOS_R,
	STAGE1BOSS_WIP1_SPEED,
	ENEMYBULLET_DAFAULT_ANGLE
};

const static ENEMYATTACK bulletCircleOrange =
{
	ORANGE_CIRCLE,
	STAGE1BOSS_CIRCLE_SHOTPOS,
	STAGE1BOSS_CIRCLE_SPEED,
	ENEMYBULLET_DAFAULT_ANGLE
};

const static ENEMYATTACK bulletCircleGreen =
{
	GREEN_CIRCLE,
	STAGE1BOSS_CIRCLE_SHOTPOS,
	STAGE1BOSS_CIRCLE_SPEED,
	ENEMYBULLET_DAFAULT_ANGLE
};

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitStage1Boss(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	STAGE1BOSS* ptr = GetStage1BossAdr();

	ptr->pos = D3DXVECTOR3(STAGE1BOSS_INITPOS_X, STAGE1BOSS_INITPOS_Y, 0.0f);
	ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	ptr->angle = atan2f(STAGE1BOSS_TEXTURE_SIZE_Y, STAGE1BOSS_TEXTURE_SIZE_X);
	ptr->radius = D3DXVec2Length(&D3DXVECTOR2(STAGE1BOSS_TEXTURE_SIZE_X, STAGE1BOSS_TEXTURE_SIZE_Y));

	ptr->countAnim = 0;
	ptr->patternAnim = 0;

	ptr->active = false;


	// 頂点情報の作成
	MakeVertexStage1Boss();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture[STAGE1BOSS_SHADOW] = CreateTextureFromFile((LPSTR)STAGE1BOSS_SHADOWTEX_NAME, pDevice);
		texture[STAGE1BOSS_BODY] = CreateTextureFromFile((LPSTR)STAGE1BOSS_TEXTURE_NAME, pDevice);
	}

	//SetStage1Boss(D3DXVECTOR3(SCREEN_CENTER_X, -100.0f, 0.0f));

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitStage1Boss(void)
{
	for (int i = 0; i < STAGE1BOSS_TEXMAX; i++)
	{
		if (texture[i] != NULL)
		{	// テクスチャの開放
			texture[i]->Release();
			texture[i] = NULL;
		}
	}
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateStage1Boss(void)
{
	STAGE1BOSS *ptr = GetStage1BossAdr();

	if (!ptr->active)
	{
		return;
	}

	//撃墜処理
	if (ptr->hp <= 0 && ptr->state != STAGE1BOSS_DESTROYED)
	{
		PlaySE(SE_BOSSDESTROY);
		ConvertBulletToScoreItem();
		ptr->gauge->active = false;
		ptr->state = STAGE1BOSS_DESTROYED;
		ptr->cntFrame = 0;
		SetEmitterEnemyParticle(ptr->pos, BOSSEXPLOSION_EMITPARTICLE_DURATION, BOSSEXPLOSION_EMITPARTICLE_NUM, EMITTER_CIRCLE);
		FadeoutBGM(STAGE1BOSS_BGM_FADEOUTFRAME);
		SetGUIShake();
		return;
	}

	//アニメーション
	ptr->countAnim++;
	if (ptr->countAnim % STAGE1BOSS_TIME_ANIMATION == 0)
	{
		ptr->patternAnim = WrapAround(0, STAGE1BOSS_ANIM_NUM, ptr->patternAnim + 1);
	}

	//ゲージ更新
	ptr->gauge->percent = ptr->hp / (float)STAGE1BOSS_INITHP;

	//行動処理分岐
	switch (ptr->state)
	{
	case STAGE1BOSS_INIT:
		ActInitStage1Boss(ptr);
		break;

	case STAGE1BOSS_INTERBAL:
		ActInterbalStage1Boss(ptr);
		break;

	case STAGE1BOSS_WAY1:
		ActWay1Stage1Boss(ptr);
		break;

	case STAGE1BOSS_WIP1:
		ActWip1Stage1Boss(ptr);
		break;
	
	case STAGE1BOSS_WIP2:
		ActWip2Stage1Boss(ptr);
		break;

	case STAGE1BOSS_CIRCLE:
		ActCircleStage1Boss(ptr);
		break;

	case STAGE1BOSS_DESTROYED:
		ActDestroyedStage1Boss(ptr);
		break;
	}
}

/******************************************************************************
行動処理（INIT状態）
******************************************************************************/
void ActInitStage1Boss(STAGE1BOSS *ptr)
{
	if (MoveToTarget(&ptr->pos, &ptr->targetPos, STAGE1BOSS_SPEED))
	{
		//状態遷移
		ptr->cntFrame = 0;
		ptr->state = STAGE1BOSS_WAY1;
	}
}

/******************************************************************************
行動処理（INTERBAL状態）
******************************************************************************/
void ActInterbalStage1Boss(STAGE1BOSS *ptr)
{
	MoveStage1Boss(ptr);
	ptr->cntFrame++;

	if (ptr->cntFrame == STAGE1BOSS_INTERBAL_FRAME)
	{
		ptr->state = ptr->nextState;
		ptr->cntFrame = 0;
		ptr->atkNum = 0;
	}
}

/******************************************************************************
行動処理（WAY1状態）
******************************************************************************/
void ActWay1Stage1Boss(STAGE1BOSS *ptr)
{
	MoveStage1Boss(ptr);

	//攻撃タイミングのみバレット発射
	if (ptr->cntFrame % STAGE1BOSS_WAY2_INTERBAL == 0)
	{
		AttackWay1(ptr, ATK_LEFT);
	}
	else if (ptr->cntFrame % STAGE1BOSS_WAY1_INTERBAL == 0)
	{
		AttackWay1(ptr, ATK_RIGHT);
	}

	//攻撃回数が最大であれば状態遷移
	if (ptr->atkNum == STAGE1BOSS_WAY1_ATKMAX)
	{
		ptr->state = STAGE1BOSS_INTERBAL;
		ptr->nextState = STAGE1BOSS_WIP1;
		ptr->cntFrame = 0;
		return;
	}

	ptr->cntFrame++;

}

//WAY1攻撃
void AttackWay1(STAGE1BOSS *ptr, int side)
{
	//オレンジの弾でWAY弾を発射
	ENEMYATTACK param = bulletWay1Orange;
	PLAYER *player = GetPlayerAdr(0);
	int difficulty = GetDifficulty();
	int loopMax = (difficulty == DIFFICULTY_EASY) ? STAGE1BOSS_WAY1_LOOPMAXEASY : (difficulty == DIFFICULTY_NORMAL) ? STAGE1BOSS_WAY1_LOOPMAXNORMAL : STAGE1BOSS_WAY1_LOOPMAXHARD;

	//オレンジ発射
	param.pos = ptr->pos;
	param.pos += (side == ATK_RIGHT) ? STAGE1BOSS_WAY2_SHOTPOS : STAGE1BOSS_WAY1_SHOTPOS;
	param.angle = SnipeTarget(&param.pos, &GetPlayerAdr(0)->pos);
	for (int i = 0; i < loopMax; i++)
	{
		FireWayBullet(&param, STAGE1BOSS_WAY1_ORANGE_WAY, STAGE1BOSS_WAY1_ORANGE_RANGE, &ptr->hp);
		param.speed += STAGE1BOSS_WAY1_SPEEDOFFSET;
	}

	//緑で発射
	param = bulletWay1Green;
	param.pos = ptr->pos;
	param.pos += (side == ATK_RIGHT) ? STAGE1BOSS_WAY2_SHOTPOS : STAGE1BOSS_WAY1_SHOTPOS;
	param.angle = SnipeTarget(&param.pos, &player->pos);
	for (int i = 0; i < loopMax; i++)
	{
		FireWayBullet(&param, STAGE1BOSS_WAY1_GREEN_WAY, STAGE1BOSS_WAY1_GREEN_RANGE, &ptr->hp);
		param.speed += STAGE1BOSS_WAY1_SPEEDOFFSET;
	}

	//攻撃回数をカウント
	ptr->atkNum++;
}

/******************************************************************************
行動処理（WIP1状態）
******************************************************************************/
void ActWip1Stage1Boss(STAGE1BOSS *ptr)
{
	MoveStage1Boss(ptr);

	int frame = ptr->cntFrame % STAGE1BOSS_WIP_PERIOD;

	if (ptr->cntFrame == STAGE1BOSS_WIPMAX)
	{
		ptr->state = STAGE1BOSS_INTERBAL;
		ptr->cntFrame = 0;
		ptr->nextState = STAGE1BOSS_CIRCLE;
		return;
	}

	if (ptr->cntFrame % STAGE1BOSS_WIP1_INTERBAL == 0)
	{
		AttackWip1(ptr, frame, ATK_LEFT);
	}

	if (ptr->atkNum == STAGE1BOSS_WIP1_ATKMAX)
	{
		ptr->state = STAGE1BOSS_WIP2;
		ptr->atkNum = 0;
	}

	ptr->cntFrame++;
}

/******************************************************************************
行動処理（WIP2状態）
******************************************************************************/
void ActWip2Stage1Boss(STAGE1BOSS *ptr)
{
	MoveStage1Boss(ptr);

	int frame = ptr->cntFrame % STAGE1BOSS_WIP_PERIOD;

	if (ptr->cntFrame == STAGE1BOSS_WIPMAX)
	{
		ptr->state = STAGE1BOSS_INTERBAL;
		ptr->cntFrame = 0;
		ptr->atkNum = 0;
		ptr->nextState = STAGE1BOSS_CIRCLE;
		return;
	}

	if (ptr->cntFrame % STAGE1BOSS_WIP1_INTERBAL == 0)
	{
		AttackWip1(ptr, frame, ATK_RIGHT);
	}

	if (ptr->atkNum == STAGE1BOSS_WIP1_ATKMAX)
	{
		ptr->state = STAGE1BOSS_WIP1;
		ptr->atkNum = 0;
	}

	ptr->cntFrame++;
}

//WIP攻撃
void AttackWip1(STAGE1BOSS *ptr,  int frame, int side)
{
	ENEMYATTACK param = (frame < STAGE1BOSS_WIP_BORDER) ? bulletWip1Orange : bulletWip1Green;
	int count = WrapAround(0, STAGE1BOSS_WIP1_ATKMAX * STAGE1BOSS_WIP1_INTERBAL, ptr->cntFrame);

	//EASYでは一部フレームでは攻撃しない
	if (GetDifficulty() == DIFFICULTY_EASY)
	{
		if (frame < STAGE1BOSS_WIP_EASYBORDER1 || frame > STAGe1BOSS_WIP_EASYBORDEE2)
		{
			return;
		}
	}

	param.pos = ptr->pos;
	param.pos += (side == ATK_LEFT) ? STAGE1BOSS_WIP1_SHOTPOS_L : STAGE1BOSS_WIP1_SHOTPOS_R;

	param.angle += (side == ATK_LEFT) ? RADIAN(count * -STAGE1BOSS_WIP1_ROTANGLE) : RADIAN(count * STAGE1BOSS_WIP1_ROTANGLE);
	param.speed += count * STAGE1BOSS_WIP1_SPEEDOFFSET;

	SetEnemyBullet(&param, &ptr->hp);

	//HARDでは追加攻撃
	if (GetDifficulty() == DIFFICULTY_HARD)
	{
		param.angle += RADIAN(180);
		SetEnemyBullet(&param, &ptr->hp);
	}

	ptr->atkNum++;
}

/******************************************************************************
行動処理（CIECLE状態）
******************************************************************************/
void ActCircleStage1Boss(STAGE1BOSS *ptr)
{
	MoveStage1Boss(ptr);
	int difficulty = GetDifficulty();
	int period = (difficulty == DIFFICULTY_EASY) ? STAGE1BOSS_CIRCLE_PERIODEASY : (difficulty == DIFFICULTY_NORMAL) ? STAGE1BOSS_CIRCLE_PERIODNOMAL : STAGE1BOSS_CIRCLE_PERIODHARD;
	int frame = ptr->cntFrame % period;

	//状態遷移
	if (ptr->atkNum == STAGE1BOSS_CIRCLE_ATKMAX)
	{
		ptr->cntFrame = 0;
		ptr->atkNum = 0;
		ptr->state = STAGE1BOSS_INTERBAL;
		ptr->nextState = STAGE1BOSS_WAY1;
	}

	//バレット発射
	if (ptr->cntFrame % STAGE1BOSS_CIRCLE_INTERBAL == 0)
	{
		AttackCircleStage1Boss(ptr, frame);
	}

	ptr->cntFrame++;
}

//CIRCLE攻撃
void AttackCircleStage1Boss(STAGE1BOSS *ptr, int frame)
{
	int difficulty = GetDifficulty();

	//EASYでは特定フレームでは攻撃を行わない
	if (difficulty == DIFFICULTY_EASY)
	{
		if (frame < STAGE1BOSS_CIRCLE_EASYBORDER1 || frame > STAGE1BOSS_CIRCLE_EASYBORDER2)
		{
			return;
		}
	}

	int period = (difficulty == DIFFICULTY_EASY) ? STAGE1BOSS_CIRCLE_PERIODEASY : (difficulty == DIFFICULTY_NORMAL) ? STAGE1BOSS_CIRCLE_PERIODNOMAL : STAGE1BOSS_CIRCLE_PERIODHARD;
	int border = period / 2;
	int way = (difficulty == DIFFICULTY_HARD) ? STAGE1BOSS_CIRCLE_WAYHARD : STAGE1BOSS_CIRCLE_WAY;

	ENEMYATTACK param = (frame < border) ? bulletCircleOrange : bulletCircleGreen;

	param.pos = ptr->pos + STAGE1BOSS_CIRCLE_SHOTPOS;

	param.angle += RADIAN(ptr->cntFrame * STAGE1BOSS_CIRCLE_ROTANGLE);

	FireCircleBullet(&param, way, &ptr->hp);

	ptr->atkNum++;
}

/******************************************************************************
行動処理（DESTROYED状態）
******************************************************************************/
void ActDestroyedStage1Boss(STAGE1BOSS *ptr)
{
	if (ptr->cntFrame == STAGE1BOSS_DESTROYED_FRAMEMAX)
	{
		DestroyStage1Boss(ptr);
	}

	if (ptr->cntFrame % STAGE1BOSS_DESTROYED_FXPLINTERBAL == 0)
	{
		ExplodeStage1Boss(ptr);
	}

	ptr->cntFrame++;

}


/******************************************************************************
描画処理
******************************************************************************/
void DrawStage1Boss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	STAGE1BOSS *ptr = GetStage1BossAdr();

	if (!ptr->active)
	{
		return;
	}

	DrawStage1BossShadow(pDevice);

	DrawStage1BossBody(pDevice);
#ifdef _DEBUG
	//当たり判定の描画
	//DrawBoxCollider(ptr->pos + D3DXVECTOR3(STAGE1BOSS_BB_LEFT, STAGE1BOSS_BB_TOP, 0.0f), STAGE1BOSS_BB_WIDTH, STAGE1BOSS_BB_HIGHT);
#endif
}

/******************************************************************************
描画処理(影用）
******************************************************************************/
void DrawStage1BossShadow(LPDIRECT3DDEVICE9 pDevice)
{
	STAGE1BOSS *ptr = GetStage1BossAdr();

	pDevice->SetTexture(0, texture[STAGE1BOSS_SHADOW]);

	SetVertexStage1BossShadow(ptr);

	SetTextureStage1BossShadow(ptr);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
描画処理（本体用）
******************************************************************************/
void DrawStage1BossBody(LPDIRECT3DDEVICE9 pDevice)
{
	STAGE1BOSS *ptr = GetStage1BossAdr();

	pDevice->SetTexture(0, texture[STAGE1BOSS_BODY]);

	SetVertexStage1Boss(ptr);

	SetTextureStage1Boss(ptr);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexStage1Boss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// rhwの設定
	vertexWk[0].rhw =
		vertexWk[1].rhw =
		vertexWk[2].rhw =
		vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	return S_OK;
}

/******************************************************************************
テクスチャ座標の設定
******************************************************************************/
void SetTextureStage1Boss(STAGE1BOSS *ptr)
{
	// テクスチャ座標の設定
	int x = ptr->patternAnim % STAGE1BOSS_TEXTURE_DIVIDE_X;
	int y = ptr->form;
	float sizeX = 1.0f / STAGE1BOSS_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / STAGE1BOSS_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

	// DESTROYED状態なら揺らす
	if (ptr->state == STAGE1BOSS_DESTROYED)
	{
		float random = RandomRange(-STAGE1BOSS_QUAKE_LENGTH, STAGE1BOSS_QUAKE_LENGTH);
		vertexWk[0].tex.x += random;
		vertexWk[1].tex.x += random;
		vertexWk[2].tex.x += random;
		vertexWk[3].tex.x += random;

		random = RandomRange(-STAGE1BOSS_QUAKE_LENGTH, STAGE1BOSS_QUAKE_LENGTH);
		vertexWk[0].tex.y += random;
		vertexWk[1].tex.y += random;
		vertexWk[2].tex.y += random;
		vertexWk[3].tex.y += random;
	}
}

/******************************************************************************
テクスチャ座標の設定(影用）
******************************************************************************/
void SetTextureStage1BossShadow(STAGE1BOSS *ptr)
{
	int y = ptr->form;
	float sizeY = 1.0f / STAGE1BOSS_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2(0.0f, y * sizeY);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, y * sizeY);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, y * sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, y * sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexStage1Boss(STAGE1BOSS *ptr)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius;
}

/******************************************************************************
頂点座標の設定（影用）
******************************************************************************/
void SetVertexStage1BossShadow(STAGE1BOSS *ptr)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius;

	//影用にオフセット
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertexWk[i].vtx += STAGE1BOSS_SHADOW_OFFSET;
	}
}

/******************************************************************************
//関数名	：STAGE1BOSS *GetStage1BossAdr(int pno)
//引数		：int pno：取得したいステージ1ボスのインデックス
//戻り値	：正常な引数：ステージ1ボスのアドレス	異常な引数：NULL
//説明		：ステージ1ボスのアドレス取得関数
******************************************************************************/
STAGE1BOSS *GetStage1BossAdr(void)
{
	return &stage1Boss;
}

/******************************************************************************
ボスセット関数
******************************************************************************/
void SetStage1Boss(D3DXVECTOR3 pos)
{
	STAGE1BOSS *ptr = GetStage1BossAdr();

	if (ptr->active)
	{
		return;
	}

	ptr->pos = pos;
	ptr->targetPos = STAGE1BOSS_TARGETPOS;

	ptr->state = STAGE1BOSS_INIT;
	ptr->form = STAGE1BOSS_FIRST;

	ptr->countAnim = 0;
	ptr->patternAnim = 0;
	ptr->atkNum = 0;

	ptr->gauge = SetBossGauge();

	ptr->hp = STAGE1BOSS_INITHP;
	ptr->active = true;
}

/******************************************************************************
ボスが画面内を動く処理
******************************************************************************/
void MoveStage1Boss(STAGE1BOSS *ptr)
{
	ptr->cntMove = WrapAround(STAGE1BOSS_COUNTMOVE_MIN, STAGE1BOSS_COUNTMOVE_MAX, ptr->cntMove + 1);

	if (ptr->cntMove < STAGE1BOSS_COUNTMOVE_BORDER)
	{
		ptr->pos.x += STAGE1BOSS_MOVE_LENGTH_X * sinf(RADIAN(ptr->cntMove));
		ptr->pos.y += STAGE1BOSS_MOVE_LENGTH_Y * sinf(RADIAN(ptr->cntMove / 2.0f));
	}
	else
	{
		ptr->pos.x -= STAGE1BOSS_MOVE_LENGTH_X * sinf(RADIAN(ptr->cntMove));
		ptr->pos.y += STAGE1BOSS_MOVE_LENGTH_Y * sinf(RADIAN(ptr->cntMove / 2.0f));
	}
}

/******************************************************************************
//関数名	：void DestroyStage1Boss(STAGE1BOSS *ptr)
//引数		：STAGE1BOSS *ptr：撃墜されるボスへのポインタ
//戻り値	：void
//説明		：ステージ1ボス撃墜処理
******************************************************************************/
void DestroyStage1Boss(STAGE1BOSS *ptr)
{
	D3DXVECTOR3 pos;

	for (int i = 0; i < STAGE1BOSS_EXPLOSION_NUM; i++)
	{
		pos = ptr->pos;
		pos.x += RandomRange(-STAGE1BOSS_EXPLOSION_RANGE_X, STAGE1BOSS_EXPLOSION_RANGE_X);
		pos.y += RandomRange(-STAGE1BOSS_EXPLOSION_RANGE_Y, STAGE1BOSS_EXPLOSION_RANGE_Y);
		ExplodeBigEnemy(pos);
		SetParticle(pos, BOSSFIRE);
	}

	ExplodeBigEnemy(ptr->pos);
	SetEmitterEnemyParticle(ptr->pos, BOSSEXPLOSION_EMITPARTICLE_DURATION, BOSSEXPLOSION_EMITPARTICLE_NUM, EMITTER_CIRCLE);
	AddScore(STAGE1BOSS_SCORE);
	ptr->active = false;
	ptr->state = STAGE1BOSS_STATEMAX;
	SetStageState(STAGE_CLEAR);
}

/******************************************************************************
//関数名	：void ExplodeStage1Boss(STAGE1BOSS *ptr)
//引数		：STAGE1BOSS *ptr	：爆発するステージ1ボスへのポインタ
//戻り値	：void
//説明		：ステージ1ボス爆発処理
******************************************************************************/
void ExplodeStage1Boss(STAGE1BOSS *ptr)
{
	if (ptr->cntFrame > STAGE1BOSS_DESTROYED_EXPLFRAME)
	{
		return;
	}

	D3DXVECTOR3 pos = ptr->pos;

	pos.x += RandomRange(-STAGE1BOSS_DESTROYED_EXPLRANGE, STAGE1BOSS_DESTROYED_EXPLRANGE);
	pos.y += RandomRange(-STAGE1BOSS_DESTROYED_EXPLRANGE, STAGE1BOSS_DESTROYED_EXPLRANGE);

	SetParticle(pos, EXPLOSION);
	SetParticle(pos, BOSSFIRE);
	SetEmitterEnemyParticle(pos, BOSSFIRE_EMITOARTUCLE_DURATION, BOSSFIRE_EMITPARTICLE_NUM, EMITTER_CONE);
	PlaySE(SE_MIDDLEEXP);
}
