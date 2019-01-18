//=============================================================================
//
// BGMプレイヤー処理 [bgmPlayer.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "sound.h"
#include "bgmPlayer.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define BGM_VOLUME_MAX		(100.0f)
#define BGM_VOLUME_MIN		(0.0f)

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
void UpdateBgmFadein(BGMPLAYER *ptr);			//フェードイン更新処理
void UpdateBgmFadeout(BGMPLAYER *ptr);			//フェードアウト更新処理

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
// サウンドファイルのパス（hの通しナンバーと合わせること）
const TCHAR* c_bgmFilename[] = {
	//_T("data/BGM/sample000.wav"),
	_T("data/SOUND/Nocturnal.wav"),
	_T("data/SOUND/偽りの藍.wav"),
	_T("data/SOUND/Traffic_Jam.wav"),
	_T("data/SOUND/バーサルビート.wav"),
	_T("data/SOUND/To_the_Next_Generation.wav"),
	_T("data/SOUND/敵影.wav"),
};

static BGMPLAYER bgmPlayer;							//BGMプレイヤー
static LPDIRECTSOUNDBUFFER8	bgmBuff[BGM_KINDMAX];	//読み込んだBGM

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitBgmPlayer(int num)
{
	BGMPLAYER *ptr = &bgmPlayer;

	ptr->cntFrame = 0;
	ptr->fadeFrame = 0;
	ptr->state = BGM_DEFAULT;

	if (num == 0)
	{
		for (int i = 0; i < BGM_KINDMAX; i++)
		{
			bgmBuff[i] = LoadSound(&c_bgmFilename[i][0]);
		}
	}

	ptr->playingBGM = NULL;

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitBgmPlayer(void)
{
	for (int i = 0; i < BGM_KINDMAX; i++)
	{
		bgmBuff[i]->Release();
	}
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateBgmPlayer(void)
{
	BGMPLAYER *ptr = &bgmPlayer;

	switch (bgmPlayer.state)
	{
	case BGM_FADEIN:
		UpdateBgmFadein(ptr);
		break;

	case BGM_FADEOUT:
		UpdateBgmFadeout(ptr);
		break;
	}
}

/******************************************************************************
更新処理(フェードイン時)
******************************************************************************/
void UpdateBgmFadein(BGMPLAYER *ptr)
{
	ptr->cntFrame++;

	ptr->bgmVolume = BGM_VOLUME_MAX * ptr->cntFrame / ptr->fadeFrame;
	SetSoundVolume(ptr->playingBGM, ptr->bgmVolume);

	if (ptr->cntFrame == ptr->fadeFrame)
	{
		ptr->state = BGM_DEFAULT;
	}

}

/******************************************************************************
更新処理（フェードアウト時）
******************************************************************************/
void UpdateBgmFadeout(BGMPLAYER *ptr)
{
	ptr->cntFrame++;

	ptr->bgmVolume = BGM_VOLUME_MAX * (1.0f - ptr->cntFrame / (float)ptr->fadeFrame);
	SetSoundVolume(ptr->playingBGM, ptr->bgmVolume);

	if (ptr->cntFrame == ptr->fadeFrame)
	{
		StopSound(ptr->playingBGM);
		ptr->state = BGM_DEFAULT;
	}
}

/******************************************************************************
//関数名	：BGMPLAYER *GetBgmPlayerAdr(void)
//引数		：void
//戻り値	：正常な引数：BGMプレイヤーのアドレス
//説明		：BGMプレイヤーのアドレス取得関数
******************************************************************************/
BGMPLAYER *GetBgmPlayerAdr(void)
{
	return &bgmPlayer;
}

/******************************************************************************
//関数名	：void FadeinBGM(int index, int fadeFrame)
//引数1		：int index		：フェードインさせるBGMの通し番号
//引数2		：int fadeFrame	：フェードインにかけるフレーム
//戻り値	：void
//説明		：フェードイン開始関数
******************************************************************************/
void FadeinBGM(int index, int fadeFrame)
{
	BGMPLAYER *ptr = &bgmPlayer;

	//BGMが再生済みであれば停止
	if (ptr->playingBGM != NULL)
	{
		StopSound(ptr->playingBGM);
	}

	//カウンタをリセット
	ptr->cntFrame = 0;
	ptr->fadeFrame = fadeFrame;

	//BGMをセット
	ptr->playingBGM = bgmBuff[index];
	ptr->playingBGM->SetCurrentPosition(0);
	ptr->bgmVolume = BGM_VOLUME_MIN;
	SetSoundVolume(ptr->playingBGM, ptr->bgmVolume);
	PlaySound(ptr->playingBGM, E_DS8_FLAG_LOOP);
	ptr->state = BGM_FADEIN;

}

/******************************************************************************
//関数名	：void FadeoutBGM(int fadeFrame)
//引数1		：int fadeFrame	：フェードアウトにかけるフレーム
//戻り値	：void
//説明		：フェードアウト開始関数
******************************************************************************/
void FadeoutBGM(int fadeFrame)
{
	BGMPLAYER *ptr = &bgmPlayer;

	//BGMが再生中でなければリターン
	if (!IsPlaying(ptr->playingBGM))
	{
		return;
	}

	//カウンタをリセット
	ptr->cntFrame = 0;
	ptr->fadeFrame = fadeFrame;

	//フェードアウトをセット
	ptr->bgmVolume = BGM_VOLUME_MAX;
	SetSoundVolume(ptr->playingBGM, ptr->bgmVolume);
	ptr->state = BGM_FADEOUT;
}

/******************************************************************************
//関数名	：void PlayBGM(int index)
//引数1		：int index		：再生するBGMの通し番号
//戻り値	：void
//説明		：BGM再生関数
******************************************************************************/
void PlayBGM(int index)
{
	BGMPLAYER *ptr = &bgmPlayer;

	//BGMが再生済みであれば停止
	if (ptr->playingBGM != NULL)
	{
		StopSound(ptr->playingBGM);
	}

	//カウンタをリセット
	ptr->cntFrame = 0;

	//BGMをセット
	ptr->playingBGM = bgmBuff[index];
	ptr->playingBGM->SetCurrentPosition(0);
	ptr->bgmVolume = BGM_VOLUME_MAX;
	SetSoundVolume(ptr->playingBGM, ptr->bgmVolume);
	PlaySound(ptr->playingBGM, E_DS8_FLAG_LOOP);
	ptr->state = BGM_DEFAULT;

}