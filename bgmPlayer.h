//=============================================================================
//
// BGM�v���C���[�w�b�_ [BgmPlayer.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _BGMPLAYER_H_
#define _BGMPLAYER_H_

#include "sound.h"

//BGM�̍Đ����
enum BGMPLAYER_STATE
{
	BGM_DEFAULT,
	BGM_FADEIN,
	BGM_FADEOUT,
	BGM_STATEMAX
};

//BGM�ʂ��ԍ�
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
// �\���̒�`
//*****************************************************************************
typedef struct {
	int						cntFrame;				//�J�E���g�t���[��
	int						fadeFrame;				//�t�F�[�h�ɂ�����t���[��
	int						currentBGM;				//�Đ�����BGM�̒ʂ��ԍ�
	float					bgmVolume;				//

	BGMPLAYER_STATE			state;					//�Đ����
	LPDIRECTSOUNDBUFFER8	playingBGM;				//�Đ�����BGM
	
}BGMPLAYER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBgmPlayer(int num);							//BGM�v���C���[����������
void UninitBgmPlayer(void);								//BGM�v���C���[�I������
void UpdateBgmPlayer(void);								//BGM�v���C���[�X�V����
BGMPLAYER *GetBgmPlayerAdr(void);						//BGM�v���C���[�A�h���X�擾�֐�
void FadeinBGM(int index, int fadeFrame);				//�t�F�[�h�C���֐�
void FadeoutBGM(int fadeFrame);
void PlayBGM(int index);								//�ʏ�Đ��֐�
#endif
