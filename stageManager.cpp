//=============================================================================
//
// ���� [stageManager.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include <stdio.h>
#include "stageManager.h"
#include "enemyManager.h"
#include "player.h"
#include "bg.h"
#include "bgmPlayer.h"
#include "gameClear.h"
#include "sePlayer.h"
#include "warning.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define STAGEMANAGER_CLEAR_FRAMEMAX			(120)			//�N���A�̑ҋ@����
#define STAGEMANAGER_PREBOSS_FRAMEMAX		(420)			//�{�X�O�̑ҋ@����

#define STAGEMANAGER_COMBOSCALE_MIN			(1.0f)			//�R���{�X�P�[���ŏ��l
#define STAGEMANAGER_COMBOSCALE_MAX			(1.5f)			//�R���{�X�P�[���ő�l

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
bool LoadStageData(void);						//�G�p�^�[���ǂݍ��݊֐�
void PopEnemy(STAGEMANAGER *ptr);				//�G�o������

//�e��Ԃ̃X�e�[�W����
void ProcessDefaultStage(STAGEMANAGER *ptr);
void ProcessPreBossStage(STAGEMANAGER *ptr);
void ProcessBossBattleStage(STAGEMANAGER *ptr);
void ProcessClearStage(STAGEMANAGER *ptr);
/*****************************************************************************
�\���̒�`
*****************************************************************************/
enum STAGE1_ENEMYTYPE
{
	ENEMY_SMALLZAKO1,
	ENEMY_SMALLTANK,
	ENEMY_MIDDLETANK,
	ENEMY_SMALLPROPELLER,
	ENEMY_BIGENEMY1,
	ENEMY_SMALLZAKO2,
	ENEMT_STAGE1BOSS
};

enum STAGEMANAGER_SCALESTATE
{
	COMBO_DEFAULT,
	COMBO_ENLARGEMENT,
	COMBO_REDUCTION,
};

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static STAGEMANAGER stageManager;				//�X�e�[�W�}�l�[�W���[�z��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitStageManager(int num)
{
	STAGEMANAGER *ptr = GetStageManagerAdr();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ptr->cntFrame = 0;
	ptr->lastPopFrame = 0;
	ptr->rank = 0;
	ptr->popNum = 0;

	//�T���v���[�X�e�[�g�p�����[�^��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);		// �e�N�X�`���g�厞�̕�Ԑݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);		// �e�N�X�`���k�����̕�Ԑݒ�

	ptr->state = STAGE_DEFAULT;

	ptr->combo = 0;
	ptr->comboScale = 1.0f;
	ptr->scaleState = COMBO_DEFAULT;

	/*
	if (num == 1)
	{
		PlayBGM(BGM_STAGE1);
	}
	*/

	//�G�p�^�[���̓ǂݍ���
	bool result;

	if (num == 0)
	{
		result = LoadStageData();
		if (!result)
		{
			//�Q�[���I��
			return S_FALSE;
		}
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitStageManager(void)
{
	STAGEMANAGER *ptr = GetStageManagerAdr();
}

/******************************************************************************
�X�V����
******************************************************************************/
void UpdateStageManager(void)
{
	STAGEMANAGER *ptr = GetStageManagerAdr();

	//�X�P�[���̐ݒ�
	if (ptr->scaleState == COMBO_ENLARGEMENT)
	{
		ptr->comboScale = Clampf(STAGEMANAGER_COMBOSCALE_MIN, STAGEMANAGER_COMBOSCALE_MAX, ptr->comboScale + 0.1f);
		if (ptr->comboScale == STAGEMANAGER_COMBOSCALE_MAX)
		{
			ptr->scaleState = COMBO_REDUCTION;
		}

	}
	else if (ptr->scaleState == COMBO_REDUCTION)
	{
		ptr->comboScale = Clampf(STAGEMANAGER_COMBOSCALE_MIN, STAGEMANAGER_COMBOSCALE_MAX, ptr->comboScale - 0.1f);
		if (ptr->comboScale == STAGEMANAGER_COMBOSCALE_MIN)
		{
			ptr->scaleState = COMBO_DEFAULT;
		}
	}

	switch (ptr->state)
	{
	case STAGE_DEFAULT:
		ProcessDefaultStage(ptr);
		break;

	case STAGE_PREBOSS:
		ProcessPreBossStage(ptr);
		break;

	case STAGE_BOSSBATTLE:
		ProcessBossBattleStage(ptr);
		break;

	case STAGE_CLEAR:
		ProcessClearStage(ptr);
		break;
	}

}

/******************************************************************************
DEFAULT����
******************************************************************************/
void ProcessDefaultStage(STAGEMANAGER *ptr)
{
	ptr->cntFrame++;
	int deltaFrame = ptr->cntFrame - ptr->lastPopFrame;

	//�G���o��������
	if (ptr->popNum < ptr->dataMax)
	{
		if (deltaFrame >= ptr->data[ptr->popNum].interbal)
		{
			PopEnemy(ptr);
			ptr->lastPopFrame = ptr->cntFrame;
		}
	}
}

/******************************************************************************
PREBOSS����
******************************************************************************/
void ProcessPreBossStage(STAGEMANAGER *ptr)
{
	ptr->cntFrame++;

	//�{�X�o���^�C�~���O�H
	if (ptr->cntFrame == STAGEMANAGER_PREBOSS_FRAMEMAX)
	{
		SetStageState(STAGE_BOSSBATTLE);
		PlayBGM(BGM_BOSS);
		SetStage1Boss(ptr->data[ptr->popNum].initPos);
	}
}

/******************************************************************************
BOSSBATTLE����
******************************************************************************/
void ProcessBossBattleStage(STAGEMANAGER *ptr)
{
	//�{�X�ҋ@
}

/******************************************************************************
CLEAR����
******************************************************************************/
void ProcessClearStage(STAGEMANAGER *ptr)
{
	ptr->cntFrame++;
	//������Ƃ��Ă���N���A����
	if (ptr->cntFrame == STAGEMANAGER_CLEAR_FRAMEMAX)
	{
		SetGameClear();
		ptr->state = STAGE_STATEMAX;
	}
	
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawStageManager(void)
{

}

/******************************************************************************
//�֐���	�FSTAGEMANAGER *GetStageManagerAdr(int pno)
//����		�Fint pno�F�擾�������X�e�[�W�}�l�[�W���[�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�X�e�[�W�}�l�[�W���[�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�X�e�[�W�}�l�[�W���[�̃A�h���X�擾�֐�
******************************************************************************/
STAGEMANAGER *GetStageManagerAdr()
{
	return &stageManager;
}

/******************************************************************************
//�֐���	�Fbool LoadStageData(void)
//����		�Fvoid
//�߂�l	�F�������Ftrue		���s���Ffalse
//����		�F�X�e�[�W�̓G�p�^�[���ǂݍ��݊֐�
******************************************************************************/
bool LoadStageData(void)
{
	FILE *fp = NULL;
	STAGEMANAGER *ptr = GetStageManagerAdr();
	int loadResult;

	//�t�@�C���̓ǂݍ���
	fp = fopen(STAGEMANAGER_DATA_PATH, "r");

	if (fp == NULL)
	{
		return false;
	}

	//�G�p�^�[���̐��������������m��
	int dataNum = 0;
	loadResult = fscanf(fp, "%d", &dataNum);

	if (loadResult == EOF)
	{
		return false;
	}

	ptr->dataMax = Clamp(0, STAGEDATA_MAX, dataNum);

	//�G�p�^�[����ǂݍ���
	int tmpInterbal, tmpType;
	float tmpInitX, tmpInitY, tmpTargetX, tmpTargetY;

	for (int i = 0; i < ptr->dataMax; i++)
	{
		loadResult = fscanf(fp, "%d,%d,%f,%f,%f,%f", &tmpInterbal, &tmpType, &tmpInitX, &tmpInitY, &tmpTargetX, &tmpTargetY);
		ptr->data[i].interbal = tmpInterbal;
		ptr->data[i].enemyType = tmpType;
		ptr->data[i].initPos = D3DXVECTOR3(tmpInitX, tmpInitY, 0.0f);
		ptr->data[i].targetPos = D3DXVECTOR3(tmpTargetX, tmpTargetY, 0.0f);
	}

	fclose(fp);
	return true;
}

/******************************************************************************
//�֐���	�Fvoid PopEnemy(STAGEMANAGER *ptr)
//����		�FSTAGEMANAGER *ptr
//�߂�l	�Fvoid
//����		�F�G�p�^�[����񂩂�G���o��������֐�
******************************************************************************/
void PopEnemy(STAGEMANAGER *ptr)
{
	//�e�p�����[�^��ǂݍ���
	int i = ptr->popNum;

	//�p�^�[���ɂ���ďo��������G�𕪊�
	switch (ptr->data[i].enemyType)
	{
	case ENEMY_BIGENEMY1:
		SetBigEnemy(ptr->data[i].initPos, ptr->data[i].targetPos);
		break;

	case ENEMY_MIDDLETANK:
		CreateMiddleTankSetter(ptr->data[i].initPos, ptr->data[i].targetPos);
		break;

	case ENEMY_SMALLPROPELLER:
		SetSmallPropeller(ptr->data[i].initPos, ptr->data[i].targetPos);
		break;

	case ENEMY_SMALLTANK:
		CreateSmallTankSetter(ptr->data[i].initPos, ptr->data[i].targetPos);
		break;

	case ENEMY_SMALLZAKO1:
		SetSmallZako1(ptr->data[i].initPos);
		break;

	case ENEMY_SMALLZAKO2:
		SetSmallZako2(ptr->data[i].initPos);
		break;

	case ENEMT_STAGE1BOSS:
		SetStageState(STAGE_PREBOSS);
		FadeoutBGM(120);
		PlaySE(SE_WARN);

		//WARNING�\��
		SetWarning();
		break;
	}

	ptr->popNum = Clamp(0, ptr->dataMax - 1, ptr->popNum + 1);
}

/******************************************************************************
//�֐���	�Fvoid SetStageState(int state)
//����		�Fint state
//�߂�l	�Fvoid
//����		�F�X�e�[�W�̏�ԃZ�b�g�֐�
******************************************************************************/
void SetStageState(int state)
{
	stageManager.state = state;
	stageManager.cntFrame = 0;
}

/******************************************************************************
//�֐���	�Fvoid AddComboCount(int value)
//����		�Fint value	�F���Z����l
//�߂�l	�Fvoid
//����		�F�R���{�����Z�֐�
******************************************************************************/
void AddComboCount(int value)
{
	stageManager.combo += value;

	if (stageManager.scaleState == COMBO_DEFAULT)
	{
		stageManager.scaleState = COMBO_ENLARGEMENT;
	}
}