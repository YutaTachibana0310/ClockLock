//=============================================================================
//
// BGM�v���C���[���� [bgmPlayer.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "sound.h"
#include "bgmPlayer.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define BGM_VOLUME_MAX		(100.0f)
#define BGM_VOLUME_MIN		(0.0f)

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
void UpdateBgmFadein(BGMPLAYER *ptr);			//�t�F�[�h�C���X�V����
void UpdateBgmFadeout(BGMPLAYER *ptr);			//�t�F�[�h�A�E�g�X�V����

/*****************************************************************************
�\���̒�`
*****************************************************************************/

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
// �T�E���h�t�@�C���̃p�X�ih�̒ʂ��i���o�[�ƍ��킹�邱�Ɓj
const TCHAR* c_bgmFilename[] = {
	//_T("data/BGM/sample000.wav"),
	_T("data/SOUND/Nocturnal.wav"),
	_T("data/SOUND/�U��̗�.wav"),
	_T("data/SOUND/Traffic_Jam.wav"),
	_T("data/SOUND/�o�[�T���r�[�g.wav"),
	_T("data/SOUND/To_the_Next_Generation.wav"),
	_T("data/SOUND/�G�e.wav"),
};

static BGMPLAYER bgmPlayer;							//BGM�v���C���[
static LPDIRECTSOUNDBUFFER8	bgmBuff[BGM_KINDMAX];	//�ǂݍ���BGM

/******************************************************************************
����������
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
�I������
******************************************************************************/
void UninitBgmPlayer(void)
{
	for (int i = 0; i < BGM_KINDMAX; i++)
	{
		bgmBuff[i]->Release();
	}
}

/******************************************************************************
�X�V����
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
�X�V����(�t�F�[�h�C����)
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
�X�V�����i�t�F�[�h�A�E�g���j
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
//�֐���	�FBGMPLAYER *GetBgmPlayerAdr(void)
//����		�Fvoid
//�߂�l	�F����Ȉ����FBGM�v���C���[�̃A�h���X
//����		�FBGM�v���C���[�̃A�h���X�擾�֐�
******************************************************************************/
BGMPLAYER *GetBgmPlayerAdr(void)
{
	return &bgmPlayer;
}

/******************************************************************************
//�֐���	�Fvoid FadeinBGM(int index, int fadeFrame)
//����1		�Fint index		�F�t�F�[�h�C��������BGM�̒ʂ��ԍ�
//����2		�Fint fadeFrame	�F�t�F�[�h�C���ɂ�����t���[��
//�߂�l	�Fvoid
//����		�F�t�F�[�h�C���J�n�֐�
******************************************************************************/
void FadeinBGM(int index, int fadeFrame)
{
	BGMPLAYER *ptr = &bgmPlayer;

	//BGM���Đ��ς݂ł���Β�~
	if (ptr->playingBGM != NULL)
	{
		StopSound(ptr->playingBGM);
	}

	//�J�E���^�����Z�b�g
	ptr->cntFrame = 0;
	ptr->fadeFrame = fadeFrame;

	//BGM���Z�b�g
	ptr->playingBGM = bgmBuff[index];
	ptr->playingBGM->SetCurrentPosition(0);
	ptr->bgmVolume = BGM_VOLUME_MIN;
	SetSoundVolume(ptr->playingBGM, ptr->bgmVolume);
	PlaySound(ptr->playingBGM, E_DS8_FLAG_LOOP);
	ptr->state = BGM_FADEIN;

}

/******************************************************************************
//�֐���	�Fvoid FadeoutBGM(int fadeFrame)
//����1		�Fint fadeFrame	�F�t�F�[�h�A�E�g�ɂ�����t���[��
//�߂�l	�Fvoid
//����		�F�t�F�[�h�A�E�g�J�n�֐�
******************************************************************************/
void FadeoutBGM(int fadeFrame)
{
	BGMPLAYER *ptr = &bgmPlayer;

	//BGM���Đ����łȂ���΃��^�[��
	if (!IsPlaying(ptr->playingBGM))
	{
		return;
	}

	//�J�E���^�����Z�b�g
	ptr->cntFrame = 0;
	ptr->fadeFrame = fadeFrame;

	//�t�F�[�h�A�E�g���Z�b�g
	ptr->bgmVolume = BGM_VOLUME_MAX;
	SetSoundVolume(ptr->playingBGM, ptr->bgmVolume);
	ptr->state = BGM_FADEOUT;
}

/******************************************************************************
//�֐���	�Fvoid PlayBGM(int index)
//����1		�Fint index		�F�Đ�����BGM�̒ʂ��ԍ�
//�߂�l	�Fvoid
//����		�FBGM�Đ��֐�
******************************************************************************/
void PlayBGM(int index)
{
	BGMPLAYER *ptr = &bgmPlayer;

	//BGM���Đ��ς݂ł���Β�~
	if (ptr->playingBGM != NULL)
	{
		StopSound(ptr->playingBGM);
	}

	//�J�E���^�����Z�b�g
	ptr->cntFrame = 0;

	//BGM���Z�b�g
	ptr->playingBGM = bgmBuff[index];
	ptr->playingBGM->SetCurrentPosition(0);
	ptr->bgmVolume = BGM_VOLUME_MAX;
	SetSoundVolume(ptr->playingBGM, ptr->bgmVolume);
	PlaySound(ptr->playingBGM, E_DS8_FLAG_LOOP);
	ptr->state = BGM_DEFAULT;

}