//=============================================================================
//
//SEプレイヤーヘッダ [sePlayer.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _SEPLAYER_H_
#define _SEPLAYER_H_

#include "sound.h"

// マクロ定義
enum
{
	SE_SMALLEXP,
	SE_MIDDLEEXP,
	SE_BIGEXP,
	SE_SHUTTER,
	SE_GETITEM,
	SE_CLOCK,
	SE_WARN,
	SE_BOMBER,
	SE_CURSOR,
	SE_DECIDE,
	SE_BOSSDESTROY,
	SE_PLAYERSHOT,
	SE_KINDMAX
};
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	LPDIRECTSOUNDBUFFER8	clip;			//音源
	float					volume;			//ボリューム

}SEPLAYER;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSEplayer(int num);							//SEプレイヤー初期化処理
void UninitSEplayer(void);								//SEプレイヤー終了処理
void UpdateSEplayer(void);								//SEプレイヤー更新処理
void PlaySE(int index);									//効果音再生
LPDWORD GetSoundFrequency(int index);
void SetSoundFrequency(DWORD freq, int index);
#endif
