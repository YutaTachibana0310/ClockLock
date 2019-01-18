//=============================================================================
//
// BGMプレイヤーヘッダ [BgmPlayer.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _BGMPLAYER_H_
#define _BGMPLAYER_H_

#include "sound.h"

//BGMの再生状態
enum BGMPLAYER_STATE
{
	BGM_DEFAULT,
	BGM_FADEIN,
	BGM_FADEOUT,
	BGM_STATEMAX
};

//BGM通し番号
enum
{
	BGM_TITLE,
	BGM_STAGE1,
	BGM_NAMEENTRY,
	BGM_BOSS,
	BGM_CLEAR,
	BGM_TUTORIAL,
	BGM_KINDMAX
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	int						cntFrame;				//カウントフレーム
	int						fadeFrame;				//フェードにかけるフレーム
	int						currentBGM;				//再生中のBGMの通し番号
	float					bgmVolume;				//

	BGMPLAYER_STATE			state;					//再生状態
	LPDIRECTSOUNDBUFFER8	playingBGM;				//再生中のBGM
	
}BGMPLAYER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBgmPlayer(int num);							//BGMプレイヤー初期化処理
void UninitBgmPlayer(void);								//BGMプレイヤー終了処理
void UpdateBgmPlayer(void);								//BGMプレイヤー更新処理
BGMPLAYER *GetBgmPlayerAdr(void);						//BGMプレイヤーアドレス取得関数
void FadeinBGM(int index, int fadeFrame);				//フェードイン関数
void FadeoutBGM(int fadeFrame);
void PlayBGM(int index);								//通常再生関数
#endif
