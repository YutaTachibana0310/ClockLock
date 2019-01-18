//=============================================================================
//
// �G�l�~�[�}�l�[�W������ [enemyManager.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "particle.h"
#include "scoreItem.h"
#include "enemyManager.h"
#include "sePlayer.h"
#include "GUI.h"
#include "stageManager.h"
#include "enemyParticle.h"

//#include "testEnemy.h"
//#include "bigEnemy1.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define BIGEXPLOSION_RANGE			(100.0f)				//��^�G�l�~�[�̔�������������͈�
#define BIGEXPLOSION_NUM			(5)						//��^�G�l�~�[�̔����Ŕ������锚���̐�
#define BIGEXPLOSION_ITEMNUM		(20)					//��^�G�l�~�[�̔����Ŕ�������A�C�e���̐�

#define MIDDLEEXPLOSION_ITEMNUM		(5)						//���^�G�l�~�[�̔����Ŕ�������A�C�e���̐�
#define MIDDLEEXPLOSION_RANGE		(70.0f)					//���^�G�l�~�[�̔����Ŕ�������A�C�e���͈̔�

#define SMALLEXPLOSION_ITEMNUM		(1)						//���^�G�l�~�[�̔����Ŕ�������A�C�e���̐�

#define ENEMYEXPLOSION_EMITPARTICLE_NUM		(3)
#define MIDDLEEXPLOSION_EMITPARTICLE_NUM	(10)
#define BIGEXPLOSION_EMITPARTICLE_NUM		(500)

#define ENEMYPARTICLE_EMITTER_DURATION	(5)
/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/


/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/


/******************************************************************************
����������
*****************************************************************************/
HRESULT InitEnemyManager(int num)
{
	InitEnemyGauge(num);
	InitBossGauge(num);
	InitTestEnemy(num);
	InitBigEnemy1(num);
	InitSmallPropeller(num);
	InitMiddleTank(num);
	InitSmallTank(num);
	InitSmallZako1(num);
	InitSmallZako2(num);
	InitItemCareer(num);
	InitStage1Boss(num);
	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitEnemyManager(void)
{
	UninitTestEnemy();
	UninitBigEnemy1();
	UninitEnemyGauge();
	UninitSmallPropeller();
	UninitMiddleTank();
	UninitSmallTank();
	UninitSmallZako1();
	UninitSmallZako2();
	UninitItemCareer();
	UninitStage1Boss();
	UninitBossGauge();
}

/******************************************************************************
�X�V����
******************************************************************************/
void UpdateEnemyManager(void)
{
	//�G�l�~�[�̍X�V
	UpdateTestEnemy();
	UpdateBigEnemy1();
	UpdateSmallPropeller();
	UpdateMiddleTank();
	UpdateSmallTank();
	UpdateSmallZako1();
	UpdateSmallZako2();
	UpdateItemCareer();
	UpdateStage1Boss();

	//�G�l�~�[�Q�[�W�̍X�V
	UpdateEnemyGauge();
	UpdateBossGauge();
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawEnemyManager(void)
{
	//�n��G�l�~�[�̕`��
	DrawSmallTank();
	DrawMiddleTank();

	//��^�G�l�~�[�̕`��
	DrawStage1Boss();
	DrawBigEnemy1();

	//���^�G�l�~�[�̕`��
	DrawTestEnemy();
	DrawItemCareer();

	//���^�G�l�~�[�̕`��
	DrawSmallPropeller();
	DrawSmallZako1();
	DrawSmallZako2();
	
}

/******************************************************************************
//�֐���	�Fvoid ExplodeBigEnemy(D3DXVECTOR3 pos)
//����1		�FD3DXVECTOR3 pos	�F�������Z�b�g������W
//�߂�l	�Fvoid
//����		�F��^�G�l�~�[�̔������Z�b�g����֐�
******************************************************************************/
void ExplodeBigEnemy(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 tmp;
	for (int i = 0; i < BIGEXPLOSION_NUM; i++)
	{
		tmp = pos;
		tmp.x += RandomRange(-BIGEXPLOSION_RANGE, BIGEXPLOSION_RANGE);
		tmp.y += RandomRange(-BIGEXPLOSION_RANGE, BIGEXPLOSION_RANGE);
		SetParticle(tmp, EXPLOSION);
	}

	SetScoreItem(pos, BIGEXPLOSION_RANGE, BIGEXPLOSION_ITEMNUM);
	SetParticle(pos, PIECE);
	SetEmitterEnemyParticle(pos, ENEMYPARTICLE_EMITTER_DURATION, BIGEXPLOSION_EMITPARTICLE_NUM ,EMITTER_CIRCLE);
	SetGUIShake();
	AddComboCount(1);
	PlaySE(SE_BIGEXP);
}

/******************************************************************************
//�֐���	�Fvoid ExplodeMiddleEnemy(D3DXVECTOR3 pos)
//����1		�FD3DXVECTOR3 pos	�F�������Z�b�g������W
//�߂�l	�Fvoid
//����		�F���^�G�l�~�[�̔������Z�b�g����֐�
******************************************************************************/
void ExplodeMiddleEnemy(D3DXVECTOR3 pos)
{
	SetParticle(pos, EXPLOSION);
	SetScoreItem(pos, MIDDLEEXPLOSION_RANGE, MIDDLEEXPLOSION_ITEMNUM);
	SetParticle(pos, PIECE);
	AddComboCount(1);
	SetEmitterEnemyParticle(pos, ENEMYPARTICLE_EMITTER_DURATION, MIDDLEEXPLOSION_EMITPARTICLE_NUM, EMITTER_CONE);
	PlaySE(SE_MIDDLEEXP);
}

/******************************************************************************
//�֐���	�Fvoid ExplodeSmallEnemy(D3DXVECTOR3 pos)
//����1		�FD3DXVECTOR3 pos	�F�������Z�b�g������W
//�߂�l	�Fvoid
//����		�F���^�G�l�~�[�̔������Z�b�g����֐�
******************************************************************************/
void ExplodeSmallEnemy(D3DXVECTOR3 pos)
{
	SetParticle(pos, FIRE);
	SetParticle(pos, PIECE);
	SetScoreItem(pos, 0.0f, SMALLEXPLOSION_ITEMNUM);
	AddComboCount(1);
	SetEmitterEnemyParticle(pos, ENEMYPARTICLE_EMITTER_DURATION, ENEMYEXPLOSION_EMITPARTICLE_NUM, EMITTER_CONE);
	PlaySE(SE_SMALLEXP);
}

/******************************************************************************
//�֐���	�Fvoid SetTextureEnemyShadow(VERTEX_2D *vertexWk)
//����1		�FVERTEX_2D *vertexWk	�F���삷�钸�_���
//�߂�l	�Fvoid
//����		�F�G�l�~�[�̉e�p�Ƀe�N�X�`�����W��ݒ肷��֐�
******************************************************************************/
void SetTextureEnemyShadow(VERTEX_2D *vertexWk)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}