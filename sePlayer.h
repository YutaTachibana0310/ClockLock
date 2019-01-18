//=============================================================================
//
//SE�v���C���[�w�b�_ [sePlayer.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _SEPLAYER_H_
#define _SEPLAYER_H_

#include "sound.h"

// �}�N����`
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
// �\���̒�`
//*****************************************************************************
typedef struct
{
	LPDIRECTSOUNDBUFFER8	clip;			//����
	float					volume;			//�{�����[��

}SEPLAYER;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSEplayer(int num);							//SE�v���C���[����������
void UninitSEplayer(void);								//SE�v���C���[�I������
void UpdateSEplayer(void);								//SE�v���C���[�X�V����
void PlaySE(int index);									//���ʉ��Đ�
LPDWORD GetSoundFrequency(int index);
void SetSoundFrequency(DWORD freq, int index);
#endif
