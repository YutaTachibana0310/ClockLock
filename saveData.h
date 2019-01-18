//=============================================================================
//
// �Z�[�u�f�[�^�w�b�_ [SaveData.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _SAVEDATA_H_
#define _SAVEDATA_H_

#include "main.h"

// �}�N����`
#define SAVEDATA_FILE_PATH ("data/save.dat")						//�Z�[�u�f�[�^�̃t�@�C���ւ̃p�X

#define SAVEDATA_HISCORE_MAX				(10)					//�ۑ������n�C�X�R�A�̍ő吔

#define SAVEDATA_PLAYERNAME_DEFAULT1			(CHARCHIP_S)					//�f�t�H���g�v���C���[��
#define SAVEDATA_PLAYERNAME_DEFAULT2			(CHARCHIP_F)					//�f�t�H���g�v���C���[��
#define SAVEDATA_PLAYERNAME_DEFAULT3			(CHARCHIP_ASTERISK)				//�f�t�H���g�v���C���[��
#define SAVEDATA_PLAYERNAME_DEFAULT4			(CHARCHIP_ASTERISK)					//�f�t�H���g�v���C���[��
#define SAVEDATA_PLAYERNAME_DEFAULT5			(CHARCHIP_ASTERISK)					//�f�t�H���g�v���C���[��

#define SAVEDATA_HISCORE_DEFAULT			(10000)					//�f�t�H���g�n�C�X�R�A

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	int highScore[SAVEDATA_HISCORE_MAX];
	int name[SAVEDATA_HISCORE_MAX][PLAYERNAME_MAX];

}SAVEDATA;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSaveData(int num);							//�Z�[�u�f�[�^����������
void UninitSaveData(void);								//�Z�[�u�f�[�^�I������
SAVEDATA *GetSaveDataAdr(void);						//�Z�[�u�f�[�^�A�h���X�擾�֐�

#endif
