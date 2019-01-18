//=============================================================================
//
// マイライブラリ処理 [myLibrary.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define LENGTH_STARTSLOW			(100*100)			//減速が始まる距離（距離を2乗で計算するので200*200)
#define SPEEDMAGNI_MIN				(0.15f)				//減速率の最低値
#define LENGTH_STOPMOVE				(10*10)				//移動を止める目標との距離

#define YELLOWNUMBER_TEXTURE_NAME _T("data/TEXTURE/GUI/scoreNum.png")	//黄色い数字テクスチャ
#define NUMTEXTURE_DEVIDE_X			(10)								//数字テクスチャの分割数

/*****************************************************************************
マクロ定義
*****************************************************************************/
enum TEX_INDEX
{
	YELLOWNUM,
	TEX_MAX
};

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[TEX_MAX];				//黄色い数字テクスチャへのポインタ

/*****************************************************************************
初期化処理
*****************************************************************************/
HRESULT InitMyLibrary(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (num == 0)
	{
		texture[YELLOWNUM] = CreateTextureFromFile((LPSTR)YELLOWNUMBER_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/*****************************************************************************
終了処理
*****************************************************************************/
void UninitMyLibrary(void)
{
	for (int i = 0; i < TEX_MAX; i++)
	{
		if (texture[i] != NULL)
		{
			texture[i]->Release();
			texture[i] = NULL;
		}
	}
}

#ifdef _DEBUG
/******************************************************************************
//関数名	：void DrawBoxCollider(D3DXVECTOR3 basePos, float width, float hight)
//引数1		：D3DXVECTOR3 basePos:バウンディングボックスの左上頂点
//引数2		：float width:バウンディングボックスの幅
//戻り値	：float hight:バウンディングボックスの高さ
//説明		：バウンディングボックスを可視化する関数
******************************************************************************/
void DrawBoxCollider(D3DXVECTOR3 basePos, float width, float hight)
{
	LPD3DXLINE line;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXVECTOR2 vec[NUM_BB_VERTEX] =
	{
		D3DXVECTOR2(basePos.x - width, basePos.y - hight),
		D3DXVECTOR2(basePos.x + width, basePos.y - hight),
		D3DXVECTOR2(basePos.x + width, basePos.y + hight),
		D3DXVECTOR2(basePos.x - width, basePos.y + hight),
		D3DXVECTOR2(basePos.x - width, basePos.y - hight)
	};

	D3DXCreateLine(pDevice, &line);

	line->SetAntialias(TRUE);
	line->SetWidth(2.0f);
	line->Begin();
	line->Draw(vec, NUM_BB_VERTEX, COLLIDER_COLOR);
	line->End();
	line->Release();
}
#endif

/******************************************************************************
//関数名	：float RandomRange(float x, float y)
//引数1		：float x	：
//引数2		：float y	：
//戻り値	：x～yの範囲内の実数
//説明		：xからyの範囲内でランダムに実数を返す関数
******************************************************************************/
float RandomRange(float x, float y)
{
	return rand() % 100 * (y - x) / 100.0f + x;
}

/******************************************************************************
//関数名	：void MoveToTarget(D3DXVECTOR3 *currentPos, D3DXVECTOR3 *targetPos, float speed)
//引数1		：D3DXVECTOR3 *currentPos	：現在の座標（実行後、更新される）
//引数2		：D3DXVECTOR3 *targetPos	：目標座標
//引数3		：float speed				：移動速度
//戻り値	：目標座標との距離が一定以下の場合：true	それ以外：false
//説明		：目標座標へ移動する関数（目標座標に近づくにつれ減速する）
******************************************************************************/
bool MoveToTarget(D3DXVECTOR3 *currentPos, D3DXVECTOR3 *targetPos, float speed)
{
	//現在座標から目標座標へのベクトルを生成
	D3DXVECTOR3 tmpVec = *targetPos - *currentPos;


	//現在座標と目標座標の距離から減速率を計算
	float length = D3DXVec3LengthSq(&tmpVec);

	if (length < LENGTH_STOPMOVE)
	{
		//距離が規定値以下なら移動を止める
		return true;
	}

	float magni = (length > LENGTH_STARTSLOW) ? 1.0f : length / LENGTH_STARTSLOW;
	if (magni < SPEEDMAGNI_MIN)
	{
		magni = SPEEDMAGNI_MIN;
	}

	//現在座標から目標座標へのベクトルを正規化
	D3DXVec3Normalize(&tmpVec, &tmpVec);

	//現在座標を更新
	*currentPos += tmpVec * speed * magni;

	return false;
}

/******************************************************************************
//関数名	：bool StartAccel(D3DXVECTOR3 *velocity, float speed)
//引数1		：D3DXVECTOR3 *currentPos	：現在座標
//引数2		：D3DXVECTOR3 *direction	：目標方向
//引数3		：float speed				：目標スピード
//引数4		：int frame					：加速にかけるフレーム
//引数5		：int currentFrame			：現在の経過フレーム
//戻り値	：速度が目標に達している：true	それ以外：false
//説明		：目標速度へ加速していく関数
******************************************************************************/
bool StartAccel(D3DXVECTOR3 *currentPos, D3DXVECTOR3 *direction, float speed, int frame, int currentFrame)
{
	D3DXVECTOR3 vec;
	D3DXVec3Normalize(&vec, direction);

	//経過フレームが加速に必要なフレームより大きい場合
	if (currentFrame > frame)
	{
		*currentPos += vec * speed;
		return true;
	}
	//小さい場合
	else
	{
		*currentPos += vec * (speed * currentFrame / frame);
		return false;
	}
}

/******************************************************************************
//関数名	：float SnipePlayer(D3DXVECTOR3 *pos, D3DXVECTOR3 *targetPos)
//引数1		：D3DXVECTOR3 *pos			：設定座標
//引数2		：D3DXVECTOR3 *targetPos	：目標座標
//戻り値	：設定座標から目標座標への角度（ラジアン）
//説明		：設定座標から目標座標への角度を求める関数
******************************************************************************/
float SnipeTarget(D3DXVECTOR3 *pos, D3DXVECTOR3 *targetPos)
{
	float angle = atan2f(targetPos->y - pos->y, targetPos->x - pos->x);
	return angle;
}

/******************************************************************************
//関数名	：float RotateToPlayer(D3DXVECTOR3 *pos, D3DXVECTOR3 *targetPos, float currentRot, float angle)
//引数1		：D3DXVECTOR3 *pos			：設定座標
//引数2		：D3DXVECTOR3 *targetPos	：目標座標
//引数3		：float currentRot			：現在の角度(ラジアン）
//引数4		：float angle				：回転角度(ラジアン)
//戻り値	：目標座標の方へ回転させる場合：回転角度*方向		させない場合：0.0f
//説明		：設定座標から目標座標の方向へ回転させる関数
******************************************************************************/
float RotateToTarget(D3DXVECTOR3 *pos, D3DXVECTOR3 *targetPos, float currentRot, float angle)
{

	float baseAngle = RADIAN(90.0f);

	D3DXVECTOR3 vec1 = D3DXVECTOR3(cosf(baseAngle + currentRot), sinf(baseAngle + currentRot), 0.0f);
	D3DXVECTOR3 vec2 = *targetPos - *pos;

	//各ベクトルを正規化
	D3DXVec3Normalize(&vec1, &vec1);
	D3DXVec3Normalize(&vec2, &vec2);

	//正規化したベクトル同士で内積を計算して回転の必要or不必要を計算
	float dot = D3DXVec3Dot(&vec1, &vec2);
	float tmp = cosf(angle);
	if (dot >= tmp)
	{
		return 0.0f;		//2つのベクトルの角度が回転角より小さければ0.0fをリターン
	}

	//回転させる方向の判定
	int sign = (vec1.y*vec2.x - vec1.x*vec2.y > 0) ? -1 : 1;

	//回転角度*方向をリターン
	return angle * sign;
}

/*******************************************************************
//関数名	：void DrawYellowNum(VERTEX_2D vertexWk[], int num)
//引数1		：VERTEX_2D vertexWk[]		：描画で使用する頂点情報
//引数2		：int num					：描画したい数字
//戻り値	：void
//説明		：黄色い数字を描画する関数
******************************************************************************/
void DrawYellowNum(VERTEX_2D *vertexWk, int num)
{
	float sizeX = 1.0f / NUMTEXTURE_DEVIDE_X;
	float x = sizeX * num;

	vertexWk[0].tex = D3DXVECTOR2(x, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(x + sizeX, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(x, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(x + sizeX, 1.0f);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture[YELLOWNUM]);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/*******************************************************************
//関数名	：WrapAround(int min, int max, int value)
//引数1		：int min		：最小値
//引数2		：int max		：最大値
//引数3		：int value		：対象の値
//戻り値	：void
//説明		：対象の値を最小から最大の範囲でラップさせる関数
******************************************************************************/
int WrapAround(int min, int max, int value)
{
	int n = (value - min) % (max - min);
	return (n >= 0) ? (n + min) : (n + max);
}

/*******************************************************************
//関数名	：Clamp(int min, int max, int value)
//引数1		：int min		：最小値
//引数2		：int max		：最大値
//引数3		：int value		：対象の値
//戻り値	：void
//説明		：対象の値を最小から最大の範囲に制限する関数
******************************************************************************/
int Clamp(int min, int max, int value)
{
	return Max(min, Min(max, value));
}

/*******************************************************************
//関数名	：Clampf(float min, float max, float value)
//引数1		：float min		：最小値
//引数2		：float max		：最大値
//引数3		：float value		：対象の値
//戻り値	：void
//説明		：対象の値を最小から最大の範囲に制限する関数
******************************************************************************/
float Clampf(float min, float max, float value)
{
	return Maxf(min, Minf(max, value));
}

/*******************************************************************
//関数名	：Min(int val1, int val2)
//引数1		：int val1		：対象値1
//引数2		：int val2		：対象値2
//戻り値	：小さい方の対象値
//説明		：2つの対象から小さい方の値を返す関数
******************************************************************************/
int Min(int val1, int val2)
{
	return (val1 < val2) ? val1 : val2;
}

/*******************************************************************
//関数名	：Max(int val1, int val2)
//引数1		：int val1		：対象値1
//引数2		：int val2		：対象値2
//戻り値	：大きい方の対象値
//説明		：2つの対象から大きい方の値を返す関数
******************************************************************************/
int Max(int val1, int val2)
{
	return (val1 > val2) ? val1 : val2;
}

/*******************************************************************
//関数名	：Minf(float val1, float val2)
//引数1		：float val1		：対象値1
//引数2		：float val2		：対象値2
//戻り値	：小さい方の対象値
//説明		：2つの対象から小さい方の値を返す関数
******************************************************************************/
float Minf(float val1, float val2)
{
	return (val1 < val2) ? val1 : val2;
}

/*******************************************************************
//関数名	：Maxf(float val1, float val2)
//引数1		：float val1		：対象値1
//引数2		：float val2		：対象値2
//戻り値	：大きい方の対象値
//説明		：2つの対象から大きい方の値を返す関数
******************************************************************************/
float Maxf(float val1, float val2)
{
	return (val1 > val2) ? val1 : val2;
}

/*******************************************************************
//関数名	：void ArrayShiftDown(int *array, int startNum, int size)
//引数1		：int *array		：シフトする配列の先頭へのポインタ
//引数2		：int startNum	：シフトを開始する位置
//引数3		：int size		：配列のサイズ
//戻り値	：void
//説明		：配列を開始位置から後ろにシフトする関数
******************************************************************************/
void ArrayShiftDown(int *array, int startNum, int size)
{
	if (startNum > size)
	{
		return;
	}

	for (int i = size - 1; i > startNum; i--)
	{
		array[i] = array[i - 1];
	}
}

/*******************************************************************
//関数名	：void VectorArrayShiftDown(int *array, int startNum, int size)
//引数1		：D3DXVECTOR3 *array	：シフトする配列の先頭へのポインタ
//引数2		：int startNum	：シフトを開始する位置
//引数3		：int size		：配列のサイズ
//戻り値	：void
//説明		：ベクトル配列を開始位置から後ろにシフトする関数
******************************************************************************/
void VectorArrayShiftDown(D3DXVECTOR3 *array, int startNum, int size)
{
	if (startNum > size)
	{
		return;
	}

	for (int i = size - 1; i > startNum; i--)
	{
		array[i] = array[i - 1];
	}
}

/*******************************************************************
//関数名	：void ArrayShiftUp(int *array, int startNum, int size)
//引数1		：int *array		：シフトする配列の先頭へのポインタ
//引数2		：int startNum		：シフトを開始する位置
//引数3		：int size			：配列のサイズ
//戻り値	：void
//説明		：配列を開始位置から上にシフトする関数
******************************************************************************/
void ArrayShiftUp(int *array, int startNum, int size)
{
	if (startNum > size)
	{
		return;
	}

	for (int i = 0; i < startNum; i++)
	{
		array[i] = array[i + 1];
	}
}

/*******************************************************************
//関数名	：void VectorArrayShiftUp(int *array, int startNum, int size)
//引数1		：int *array		：シフトする配列の先頭へのポインタ
//引数2		：int startNum		：シフトを開始する位置
//引数3		：int size			：配列のサイズ
//戻り値	：void
//説明		：ベクトル配列を開始位置から上にシフトする関数
******************************************************************************/
void VectorArrayShiftUp(D3DXVECTOR3 *array, int startNum, int size)
{
	if (startNum > size)
	{
		return;
	}

	for (int i = 0; i < startNum; i++)
	{
		array[i] = array[i + 1];
	}
}

/*******************************************************************
//関数名	：void ArrayShiftUp(int *array, int startNum, int size)
//引数1		：int *dst		：コピー対象
//引数2		：int *src		：コピー元
//引数3		：int size		：配列のサイズ
//戻り値	：void
//説明		：配列をコピーする関数
******************************************************************************/
void ArrayCopy(int *dst, int *src, int size)
{
	for (int i = 0; i < size; i++)
	{
		dst[i] = src[i];
	}
}

/*******************************************************************
//関数名	：float EaseInCubic
//引数1		：float time		：現在の進行度
//引数2		：float start		：初期値
//引数3		：float goal		：目標値
//引数4		：float duration	：進行度の採集地
//戻り値	：イージングの値
//説明		：イージング計算処理
********************************************************************/
float EaseInCubic(float time, float start, float goal, float duration)
{
	float diff = goal - start;
	float t = time / duration;

	return diff * t * t + start;
}

/*******************************************************************
//関数名	：float EaseOutCubic
//引数1		：float time		：現在の進行度
//引数2		：float start		：初期値
//引数3		：float goal		：目標値
//引数4		：float duration	：進行度の採集地
//戻り値	：イージングの値
//説明		：イージング計算処理
********************************************************************/
float EaseOutCubic(float time, float start, float goal, float duration)
{
	float diff = goal - start;
	float t = time / duration;

	return -diff * t * (t - 2.0f) + start;
}

/*******************************************************************
//関数名	：float EaseInOutCubic
//引数1		：float time		：現在の進行度
//引数2		：float start		：初期値
//引数3		：float goal		：目標値
//引数4		：float duration	：進行度の採集地
//戻り値	：イージングの値
//説明		：イージング計算処理
********************************************************************/
float EaseInOutCubic(float time, float start, float goal, float duration)
{
	float diff = goal - start;
	float t = 2.0f * time / duration;

	if (t < 1)
	{
		return diff / 2.0f * t * t + start;
	}
	else
	{
		t -= 1.0f;
		return -diff / 2.0f * (t * (t - 2) - 1) + start;
	}
}