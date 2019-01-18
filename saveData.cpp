//=============================================================================
//
// ���� [saveData.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "charChip.h"
#include "saveData.h"
#include "Score.h"
#include <stdio.h>
#include <string.h>

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define SAVEDATA_SPEED (-40.0f)

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
bool LoadSaveData(void);			//�Ǎ��֐�
bool CreateSaveData(void);			//�쐬�֐�
bool SetSaveData(void);				//�ۑ��֐�

/*****************************************************************************
�\���̒�`
*****************************************************************************/

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
SAVEDATA saveData;				//�v���C���[�o���b�g�z��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitSaveData(int num)
{
	bool result = LoadSaveData();

	if (!result)
	{
		result = CreateSaveData();

		if (!result)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitSaveData(void)
{
	SetSaveData();
}

/******************************************************************************
�Z�[�u�f�[�^�擾�֐�
******************************************************************************/
SAVEDATA *GetSaveDataAdr(void)
{
	return &saveData;
}

/******************************************************************************
�Z�[�u�f�[�^�ǂݍ��݊֐�
******************************************************************************/
bool LoadSaveData(void)
{
	FILE *fp = fopen(SAVEDATA_FILE_PATH, "rb");

	if (fp == NULL)
	{
		return false;
	}

	SAVEDATA *ptr = GetSaveDataAdr();
	fread(ptr, sizeof(SAVEDATA), 1, fp);

	fclose(fp);
	return true;
}

/******************************************************************************
�Z�[�u�f�[�^�쐬�֐�
******************************************************************************/
bool CreateSaveData(void)
{
	//�Z�[�u�f�[�^��������
	SAVEDATA *ptr = GetSaveDataAdr();
	for (int i = 0; i < SAVEDATA_HISCORE_MAX; i++)
	{
		ptr->name[i][0] = SAVEDATA_PLAYERNAME_DEFAULT1;
		ptr->name[i][1] = SAVEDATA_PLAYERNAME_DEFAULT2;
		ptr->name[i][2] = SAVEDATA_PLAYERNAME_DEFAULT3;
		ptr->name[i][3] = SAVEDATA_PLAYERNAME_DEFAULT4;
		ptr->name[i][4] = SAVEDATA_PLAYERNAME_DEFAULT5;

		ptr->highScore[i] = (SAVEDATA_HISCORE_MAX - i) * SAVEDATA_HISCORE_DEFAULT;
	}

	//�t�@�C���֕ۑ�
	FILE *fp = fopen(SAVEDATA_FILE_PATH, "wb");

	if (fp == NULL)
	{
		return false;
	}

	fwrite(ptr, sizeof(SAVEDATA), 1, fp);

	fclose(fp);
	return true;
}

/******************************************************************************
�Z�[�u�f�[�^�ۑ��֐�
******************************************************************************/
bool SetSaveData(void)
{
	FILE *fp = fopen(SAVEDATA_FILE_PATH, "wb");

	if (fp == NULL)
	{
		return false;
	}

	SAVEDATA *ptr = GetSaveDataAdr();
	SCOREDATA *scoreData = GetScoreDataAdr();

	for (int i = 0; i < SAVEDATA_HISCORE_MAX; i++)
	{
		ptr->highScore[i] = scoreData->HiScore[i];
		//strcpy(&ptr->hiScore[i].name[0], &scoreData->playerName[i][0]);
		//memcpy(&ptr->name[i][0], &scoreData->playerName[i][0], sizeof(ptr->name));
		ArrayCopy(&ptr->name[i][0], &scoreData->playerName[i][0], PLAYERNAME_MAX);
	}

	fwrite(ptr, sizeof(SAVEDATA), 1, fp);
	fclose(fp);
	return true;
}