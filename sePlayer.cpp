//=============================================================================
//
// SEプレイヤー処理 [sePlayer.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "sound.h"
#include "sePlayer.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define SE_VOLUME_MAX		(100.0f)
#define SE_VOLUME_MIN		(0.0f)

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
// サウンドファイルのパス（hの通しナンバーと合わせること）
const TCHAR* c_seFilename[] = {
	//_T("data/BGM/sample000.wav"),
	_T("data/SOUND/smallExplosion.wav"),
	_T("data/SOUND/middleExplosion.wav"),
	_T("data/SOUND/bigExplosion.wav"),
	_T("data/SOUND/shutter.wav"),
	_T("data/SOUND/getItem.wav"),
	_T("data/SOUND/clock.wav"),
	_T("data/SOUND/warn.wav"),
	_T("data/SOUND/bomber.wav"),
	_T("data/SOUND/cursor37.wav"),
	_T("data/SOUND/power32.wav"),
	_T("data/SOUND/don03.wav"),
	_T("data/SOUND/gun25.wav")
};

const float volumeSE[] = {
	10.0f,			//volume SMALLEXP
	10.0f,			//volume MIDDLEEXP
	50.0f,			//volume BIGEXP
	15.0f,			//volume SHUTTER
	5.0f,			//volume GETITEM
	5.0f,			//volume CLOCK
	5.0f,			//volume WARN
	10.0f,			//volume BOMBER
	15.0f,			//volumw CURSOR
	5.0f,			//volume DECIDE
	5.0f,			//volume BOSSDESTROT
	0.3f,			//volume PLAYERSHOT
};

static SEPLAYER sePlayer[SE_KINDMAX];
/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitSEplayer(int num)
{
	SEPLAYER *ptr = &sePlayer[0];
	if (num == 0)
	{
		for (int i = 0; i < SE_KINDMAX; i++, ptr++)
		{
			ptr->clip = LoadSound(&c_seFilename[i][0]);
			ptr->volume = volumeSE[i];
			SetSoundVolume(ptr->clip, ptr->volume);
		}
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitSEplayer(void)
{
	for (int i = 0; i < SE_KINDMAX; i++)
	{
		sePlayer[i].clip->Release();
	}
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateSEplayer(void)
{

}

void PlaySE(int index)
{
	SEPLAYER *ptr = &sePlayer[index];
	if (ptr->clip == NULL)
	{
		return;
	}

	//SEをセット
	ptr->clip->SetCurrentPosition(0);
	PlaySound(ptr->clip, E_DS8_FLAG_NONE);

}

/********************************
周波数取得関数
*********************************/
LPDWORD GetSoundFrequency(int index)
{
	SEPLAYER *ptr = &sePlayer[index];
	LPDWORD freq = NULL;
	if (ptr->clip == NULL)
	{
		return NULL;
	}

	//周波数を取得
	ptr->clip->GetFrequency(freq);
	return freq;
}

/********************************
周波数セット関数
*********************************/
void SetSoundFrequency(DWORD freq, int index)
{
	SEPLAYER *ptr = &sePlayer[index];
	if (ptr->clip == NULL)
	{
		return;
	}

	//周波数をセット
	ptr->clip->SetFrequency(freq);
}