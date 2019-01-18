//=============================================================================
//
// �G�l�~�[�}�l�[�W���w�b�_ [EnemyManager.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _ENEMYMANAGER_H_
#define _ENEMYMANAGER_H_

#include "enemyGauge.h"
#include "bossGauge.h"
#include "bg.h"
#include "Score.h"
#include "enemyBullet.h"
#include "testEnemy.h"
#include "bigEnemy1.h"
#include "smallPropeller.h"
#include "middleTank.h"
#include "smallTank.h"
#include "smallZako1.h"
#include "itemCareer.h"
#include "smallZako2.h"
#include "stage1Boss.h"

// �}�N����`

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemyManager(int num);							//�G�l�~�[�}�l�[�W������������
void UninitEnemyManager(void);								//�G�l�~�[�}�l�[�W���I������
void UpdateEnemyManager(void);								//�G�l�~�[�}�l�[�W���X�V����
void DrawEnemyManager(void);								//�G�l�~�[�}�l�[�W���`�揈��
void ExplodeBigEnemy(D3DXVECTOR3 pos);						//��^�G�l�~�[�̔����Z�b�g�֐�
void ExplodeMiddleEnemy(D3DXVECTOR3 pos);					//���^�G�l�~�[�̔����Z�b�g�֐�
void ExplodeSmallEnemy(D3DXVECTOR3 pos);					//���^�G�l�~�[�̔����Z�b�g�֐�
void SetTextureEnemyShadow(VERTEX_2D *vertexWk);			//�G�l�~�[�̉e�e�N�X�`�����W�ݒ�֐�
#endif
