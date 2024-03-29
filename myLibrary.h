//=============================================================================
//
// マイライブラリヘッダ [myLibrary.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _MYLIBRARY_H_
#define _MYLIBRARY_H_

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMyLibrary(int num);			//マイライブラリ初期化処理
void UninitMyLibrary(void);				//マイライブラリ終了処理

LPDIRECT3DTEXTURE9	CreateTextureFromFile(LPSTR szName, LPDIRECT3DDEVICE9 lpD3DDevice);	// テクスチャ読み込み関数
float RandomRange(float x, float y);													// x〜y内の範囲内でランダムに実数を返す関数	
bool MoveToTarget(D3DXVECTOR3 *currentPos, D3DXVECTOR3 *targetPos, float speed);		//目標座標へと移動する関数
bool StartAccel(D3DXVECTOR3 *currentPos, D3DXVECTOR3 *direction, float speed, int frame, int currentFrame);	//速度ベクトルを目標スピードへ加速させる関数
float SnipeTarget(D3DXVECTOR3 *pos, D3DXVECTOR3 *targetPos);							//設定座標から目標座標への角度を求める関数
float RotateToTarget(D3DXVECTOR3 *pos, D3DXVECTOR3 *targetPos, float currentRot, float angle); //設定座標から目標座標の方向へ回転させる関数
void DrawYellowNum(VERTEX_2D *vertexWk, int num);										//黄色い数字を描画する関数
int WrapAround(int min, int max, int value);											//対象の数値をラップさせる関数
int Clamp(int min, int max, int value);													//対象の値を範囲内に制限する関数
int Min(int val1, int val2);															//小さい方の値を返す関数
int Max(int val1, int val2);															//大きい方の値を返す関数
float Clampf(float min, float max, float value);										//Clamp()のfloat版
float Minf(float val1, float val2);														//Min()のfloat版
float Maxf(float val1, float val2);														//Max()のfloat版
void ArrayShiftDown(int *buff, int startNum, int size);									//配列を下へシフトする関数
void VectorArrayShiftDown(D3DXVECTOR3 *array, int startNum, int size);					//ベクトル配列を下へシフトする関数
void ArrayShiftUp(int *buff, int startNum, int size);									//配列を上へシフトする関数
void VectorArrayShiftUp(D3DXVECTOR3 *array, int startNum, int size);					//ベクトル配列を上へシフトする関数
void ArrayCopy(int *dst, int *src, int size);											//配列コピー関数
float EaseInCubic(float time, float start, float goal, float duration);
float EaseOutCubic(float time, float start, float goal, float duration);
float EaseInOutCubic(float time, float start, float goal, float duration);
#endif