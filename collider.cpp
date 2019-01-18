//=============================================================================
//
// �R���C�_�[���� [collider.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "collider.h"
#include "player.h"
#include "playerBullet.h"
#include "enemyBullet.h"
#include "PBulletExplosion.h"
#include "scoreItem.h"
#include "Score.h"
#include "enemyManager.h"
#include "sePlayer.h"
#include "energyItem.h"
#include "playerBomber.h"
#include "tutorialEnemy.h"
#include "playerShield.h"
#include "EnemyBulletExplosion.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define PLAYER_ENERGY_ADDVALUEEASY			(2.0f)		//�A�C�e���擾���ɑ�����G�i�W�[�̗ʁiEASY)
#define PLAYER_ENERGY_ADDVALUENORMAL		(1.5f)		//�A�C�e���擾���ɑ�����G�i�W�[�̗ʁiNORMAL�j
#define PLAYER_ENERGY_ADDVALUEHARD			(0.5f)		//�A�C�e���擾���ɑ�����G�i�W�[�̗ʁiHARD)

#define BOMBER_DAMAGE						(3)			//�{���o�[�̃_���[�W

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
void CollisionEnemyBullet(void);					//�G�l�~�[�o���b�g�̓����蔻��֐�
void CollisionTestEnemy(void);						//�e�X�g�G�l�~�[�̓����蔻��֐�
void CollisionScoreItem(void);						//�X�R�A�A�C�e���̓����蔻��
void CollisionBigEnemy1(void);						//�r�b�O�G�l�~�[1�̓����蔻��
void CollisionSmallPropeller();						//�X���[���v���y���ƐڐG����
void CollisionMiddleTank(void);						//�~�h���^���N�̓����蔻��
void CollisionSmallTank(void);						//�X���[���^���N�̓����蔻��
void CollisionSmallZako1(void);						//�X���[���U�R1�̓����蔻��
void CollisionSmallZako2(void);						//�X���[���U�R2�̓����蔻��
void CollisionItemCareer(void);						//�A�C�e���L�����A�̓����蔻��
void CollisionEnergyItem(void);						//�G�i�W�[�A�C�e���̓����蔻��
void CollisionStage1Boss(void);						//�X�e�[�W1�{�X�̓����蔻��
void CollisionTutorialEnemy(void);					//�`���[�g���A���G�l�~�[�̓����蔻��
void CollisionBomber(void);							//�{���o�[�̓����蔻��
void CollisionTutorialEnemy(void);					//�`���[�g���A���G�l�~�[�̓����蔻��
void CollisionPlayerShield(void);					//�v���C���[�V�[���h�̓����蔻��

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/

/******************************************************************************
����������
******************************************************************************/
HRESULT InitCollider(void)
{
	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitCollider(void)
{

}

/******************************************************************************
�X�V����
******************************************************************************/
void UpdateCollider(void)
{
	//�X�R�A�A�C�e���̓����蔻��
	CollisionScoreItem();
	CollisionEnergyItem();

	//�G�l�~�[�̓����蔻��
	CollisionTestEnemy();
	CollisionBigEnemy1();
	CollisionSmallPropeller();
	CollisionMiddleTank();
	CollisionSmallTank();
	CollisionSmallZako1();
	CollisionSmallZako2();
	CollisionItemCareer();
	CollisionStage1Boss();
	CollisionTutorialEnemy();
	//CollisionPlayerShield();
	CollisionBomber();
	CollisionPlayerShield();

	//�G�l�~�[�o���b�g�̓����蔻��
	CollisionEnemyBullet();
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawCollider(void)
{

}

/******************************************************************************
//�֐���	�Fbool HitBB(D3DXVECTOR4 bb1, D3DXVECTOR4 bb2)
//����1		�FD3DXVECTOR4 bb1	�F�o�E���f�B���O�{�b�N�X1�ix:x���W y:y���W z:�{�b�N�X��(����) w:�{�b�N�X����(����)�j
//����2		�FD3DXVECTOR4 bb2	�F�o�E���f�B���O�{�b�N�X1�ix:x���W y:y���W z:�{�b�N�X��(����) w:�{�b�N�X����(����)�j
//�߂�l	�F2��BB���ڐG���Ă���ꍇ�Ftrue	�ڐG���Ă��Ȃ��ꍇ�Ffalse
//����		�F�o�E���f�B���O�{�b�N�X�̐ڐG�𔻒肷��֐�
******************************************************************************/
bool HitBB(D3DXVECTOR4 bb1, D3DXVECTOR4 bb2)
{
	if (bb1.x + bb1.z > bb2.x - bb2.z && bb1.x - bb1.z < bb2.x + bb2.z)
	{
		if (bb1.y + bb1.w > bb2.y - bb2.w && bb1.y - bb1.w < bb2.y + bb2.w)
		{
			return true;
		}
	}
	return false;
}

/******************************************************************************
//�֐���	�Fbool HitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float radius1, float radius2)
//����1		�FD3DXVRCTOR3 pos1		�F�Ώ�1�̍��W
//����2		�FD3DXVRCTOR3 pos1		�F�Ώ�2�̍��W
//����3		�Ffloat radius1			�F�Ώ�1�̔��a
//����4		�Ffloat radius2			�F�Ώ�2�̔��a
//�߂�l	�F2��BC���ڐG���Ă���ꍇ�Ftrue	�ڐG���Ă��Ȃ��ꍇ�Ffalse
//����		�F�o�E���f�B���O�T�[�N���̐ڐG�𔻒肷��֐�
******************************************************************************/
bool HitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float radius1, float radius2)
{
	float lengthSq = D3DXVec3LengthSq(&(pos1 - pos2));

	return (pow(radius1 + radius2, 2) > lengthSq) ? true : false;

}

/******************************************************************************
//�֐���	�Fvoid CollisionBullet(void)
//����1		�Fvoid
//�߂�l	�Fvoid
//����		�F�l�~�[�o���b�g�̐ڐG����
******************************************************************************/
void CollisionEnemyBullet(void) {

	int i, j;
	PLAYER *player = GetPlayerAdr(0);								//�v���C���[�ւ̃|�C���^
	ENEMYBULLET *eBullet = GetEnemyBulletAdr(GREEN_ROTATE, 0);		//�G�l�~�[�o���b�g�̃|�C���^
	D3DXVECTOR4 bb1, bb2;											//�v���C���[�A�G�l�~�[�o���b�g�̃o�E���f�B���O�{�b�N�X

	//�S�Ẵv���C���[�ƃG�l�~�[�o���b�g�ŐڐG����
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->state != PLAYER_DEFAULT)
		{
			continue;
		}

		eBullet = GetEnemyBulletAdr(GREEN_ROTATE, 0);
		for (j = 0; j < ENEMYBULLET_MAX; j++, eBullet++)
		{
			if (!eBullet->active)
			{
				continue;
			}


			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
			bb2 = D3DXVECTOR4(eBullet->pos.x + ENEMYBULLET_BB_LEFT, eBullet->pos.y + ENEMYBULLET_BB_TOP, ENEMYBULLET_BB_WIDTH, ENEMYBULLET_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				//�v���C���[�ƃG�l�~�[�o���b�g���ڐG���Ă����猂��
				DestroyPlayer(i);
			}
		}
	}
}

/******************************************************************************
//�֐���	�Fvoid CollisionTestEnemy(void)
//����1		�Fvoid
//�߂�l	�Fvoid
//����		�F�e�X�g�G�l�~�[�̐ڐG����
******************************************************************************/
void CollisionTestEnemy(void)
{
	int i, j;
	PLAYERBULLET *pBullet = GetPlayerBulletAdr(0);
	TESTENEMY *enemy = GetTestEnemyAdr(0);
	D3DXVECTOR4 bb1, bb2;

	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetTestEnemyAdr(0);
		for (j = 0; j < TESTENEMY_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + TESTENEMY_BB_LEFT, enemy->pos.y + TESTENEMY_BB_TOP, TESTENEMY_BB_WIDTH, TESTENEMY_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	pBullet = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetTestEnemyAdr(0);
		for (j = 0; j < TESTENEMY_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + TESTENEMY_BB_LEFT, enemy->pos.y + TESTENEMY_BB_TOP, TESTENEMY_BB_WIDTH, TESTENEMY_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}
}

/******************************************************************************
//�֐���	�Fvoid CollisionTestEnemy(void)
//����1		�Fvoid
//�߂�l	�Fvoid
//����		�F�X�R�A�A�C�e���̐ڐG����
******************************************************************************/
void CollisionScoreItem(void)
{
	PLAYER *player = GetPlayerAdr(0);
	SCOREITEM *item = GetScoreItemAdr(0);
	D3DXVECTOR4 bb1, bb2;
	int i, j, difficulty = GetDifficulty();
	float addValue;

	addValue = (difficulty == DIFFICULTY_EASY) ? PLAYER_ENERGY_ADDVALUEEASY : (difficulty == DIFFICULTY_NORMAL) ? PLAYER_ENERGY_ADDVALUENORMAL : PLAYER_ENERGY_ADDVALUEHARD;
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (!player->active)
		{
			continue;
		}

		for (j = 0; j < SCOREITEM_MAX; j++, item++)
		{
			if (!item->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_ITEMBB_WIDTH, PLAYER_ITEMBB_HIGHT);
			bb2 = D3DXVECTOR4(item->pos.x + SCOREITEM_BB_LEFT, item->pos.y + SCOREITEM_BB_TOP, SCOREITEM_BB_WIDTH, SCOREITEM_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				//�X�R�A�����Z���A�C�e�����A�N�e�B�u�ɃZ�b�g
				AddScore(SCOREITEM_ADDVALUE);
				SetScoreItemCount(1);
				item->active = false;
				player->timeEffect = Clampf(0.0f, PLAYER_TIMEEFFECT_MAX, player->timeEffect + addValue);
				PlaySE(SE_GETITEM);
#ifdef _DEBUG
				SetActiveItemCount(-1);
#endif
			}
		}
	}
}

/******************************************************************************
//�֐���	�Fvoid CollisionEnergyEnemy(void)
//����1		�Fvoid
//�߂�l	�Fvoid
//����		�F�G�i�W�[�A�C�e���̐ڐG����
******************************************************************************/
void CollisionEnergyItem(void)
{
	PLAYER *player = GetPlayerAdr(0);
	ENERGYITEM *item = GetEnergyItemAdr(0);
	D3DXVECTOR4 bb1, bb2;
	int i, j;

	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (!player->active)
		{
			continue;
		}

		for (j = 0; j < ENERGYITEM_MAX; j++, item++)
		{
			if (!item->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_ITEMBB_WIDTH, PLAYER_ITEMBB_HIGHT);
			bb2 = D3DXVECTOR4(item->pos.x + ENERGYITEM_BB_LEFT, item->pos.y + ENERGYITEM_BB_TOP, ENERGYITEM_BB_WIDTH, ENERGYITEM_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				//�G�i�W�[�����Z���A�C�e�����A�N�e�B�u�ɃZ�b�g
				player->timeEffect = Clampf(0, PLAYER_TIMEEFFECT_MAX, player->timeEffect + ENERGYITEM_ADDVALUE);
				item->active = false;
				PlaySE(SE_GETITEM);
			}
		}
	}
}
/******************************************************************************
//�֐���	�Fvoid CollisionBigEnemy1(void)
//����1		�Fvoid
//�߂�l	�Fvoid
//����		�F�r�b�O�G�l�~�[1�̐ڐG����
******************************************************************************/
void CollisionBigEnemy1(void)
{
	int i, j;
	PLAYERBULLET *pBullet = GetPlayerBulletAdr(0);
	BIGENEMY1 *enemy = GetBigEnemy1Adr(0);
	D3DXVECTOR4 bb1, bb2;


	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetBigEnemy1Adr(0);
		for (j = 0; j < BIGENEMY1_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + BIGENEMY1_BB_LEFT, enemy->pos.y + BIGENEMY1_BB_TOP, BIGENEMY1_BB_WIDTH, BIGENEMY1_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	pBullet = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetBigEnemy1Adr(0);
		for (j = 0; j < BIGENEMY1_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + BIGENEMY1_BB_LEFT, enemy->pos.y + BIGENEMY1_BB_TOP, BIGENEMY1_BB_WIDTH, BIGENEMY1_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}


	//�{���Ƃ̓����蔻��
	PLAYERBOMBER *bom = GetPlayerBomberAdr(0);
	for (i = 0; i < PLAYERBOMBER_MAX; i++, bom++)
	{
		if (!bom->active)
		{
			continue;
		}

		enemy = GetBigEnemy1Adr(0);
		for (j = 0; j < BIGENEMY1_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(bom->pos.x + PLAYERBOMBER_BB_LEFT, bom->pos.y + PLAYERBOMBER_BB_TOP, PLAYERBOMBER_BB_WIDTH * bom->scale, PLAYERBOMBER_BB_HIGHT * bom->scale);
			bb2 = D3DXVECTOR4(enemy->pos.x + BIGENEMY1_BB_LEFT, enemy->pos.y + BIGENEMY1_BB_TOP, BIGENEMY1_BB_WIDTH, BIGENEMY1_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= BOMBER_DAMAGE;
			}
		}
	}

	//�v���C���[�Ƃ̓����蔻��
	PLAYER *player = GetPlayerAdr(0);
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->state != PLAYER_DEFAULT)
		{
			continue;
		}

		enemy = GetBigEnemy1Adr(0);
		for (j = 0; j < BIGENEMY1_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + BIGENEMY1_BB_LEFT, enemy->pos.y + BIGENEMY1_BB_TOP, BIGENEMY1_BB_WIDTH, BIGENEMY1_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				DestroyPlayer(i);
			}
		}
	}
}

/******************************************************************************
//�֐���	�Fvoid CollisionSmallPropeller(void)
//����1		�Fvoid
//�߂�l	�Fvoid
//����		�F�X���[���v���y���̐ڐG����
******************************************************************************/
void CollisionSmallPropeller(void)
{
	int i, j;
	PLAYERBULLET *pBullet = GetPlayerBulletAdr(0);
	SMALLPROPELLER *enemy = GetSmallPropellerAdr(0);
	D3DXVECTOR4 bb1, bb2;

	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetSmallPropellerAdr(0);
		for (j = 0; j < SMALLPROPELLER_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLPROPELLER_BB_LEFT, enemy->pos.y + SMALLPROPELLER_BB_TOP, SMALLPROPELLER_BB_WIDTH, SMALLPROPELLER_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	pBullet = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetSmallPropellerAdr(0);
		for (j = 0; j < SMALLPROPELLER_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLPROPELLER_BB_LEFT, enemy->pos.y + SMALLPROPELLER_BB_TOP, SMALLPROPELLER_BB_WIDTH, SMALLPROPELLER_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	//�{���Ƃ̓����蔻��
	PLAYERBOMBER *bom = GetPlayerBomberAdr(0);
	for (i = 0; i < PLAYERBOMBER_MAX; i++, bom++)
	{
		if (!bom->active)
		{
			continue;
		}
		enemy = GetSmallPropellerAdr(0);
		for (j = 0; j < SMALLPROPELLER_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(bom->pos.x + PLAYERBOMBER_BB_LEFT, bom->pos.y + PLAYERBOMBER_BB_TOP, PLAYERBOMBER_BB_WIDTH * bom->scale, PLAYERBOMBER_BB_HIGHT * bom->scale);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLPROPELLER_BB_LEFT, enemy->pos.y + SMALLPROPELLER_BB_TOP, SMALLPROPELLER_BB_WIDTH, SMALLPROPELLER_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= BOMBER_DAMAGE;
			}
		}
	}

	//�v���C���[�Ƃ̓����蔻��
	PLAYER *player = GetPlayerAdr(0);
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->state != PLAYER_DEFAULT)
		{
			continue;
		}

		enemy = GetSmallPropellerAdr(0);
		for (j = 0; j < BIGENEMY1_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLPROPELLER_BB_LEFT, enemy->pos.y + SMALLPROPELLER_BB_TOP, SMALLPROPELLER_BB_WIDTH, SMALLPROPELLER_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				DestroyPlayer(i);
			}
		}
	}
}

/******************************************************************************
//�֐���	�Fvoid CollisionMiddleTank(void)
//����1		�Fvoid
//�߂�l	�Fvoid
//����		�F�~�h���^���N�̐ڐG����
******************************************************************************/
void CollisionMiddleTank(void)
{
	int i, j;
	PLAYERBULLET *pBullet = GetPlayerBulletAdr(0);
	MIDDLETANK *enemy = GetMiddleTankAdr(0);
	D3DXVECTOR4 bb1, bb2;

	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetMiddleTankAdr(0);
		for (j = 0; j < MIDDLETANK_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + MIDDLETANK_BB_LEFT, enemy->pos.y + MIDDLETANK_BB_TOP, MIDDLETANK_BB_WIDTH, MIDDLETANK_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	pBullet = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetMiddleTankAdr(0);
		for (j = 0; j < MIDDLETANK_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + MIDDLETANK_BB_LEFT, enemy->pos.y + MIDDLETANK_BB_TOP, MIDDLETANK_BB_WIDTH, MIDDLETANK_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	//�{���Ƃ̓����蔻��
	PLAYERBOMBER *bom = GetPlayerBomberAdr(0);
	for (i = 0; i < PLAYERBOMBER_MAX; i++, bom++)
	{
		if (!bom->active)
		{
			continue;
		}
		enemy = GetMiddleTankAdr(0);
		for (j = 0; j < MIDDLETANK_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(bom->pos.x + PLAYERBOMBER_BB_LEFT, bom->pos.y + PLAYERBOMBER_BB_TOP, PLAYERBOMBER_BB_WIDTH * bom->scale, PLAYERBOMBER_BB_HIGHT * bom->scale);
			bb2 = D3DXVECTOR4(enemy->pos.x + MIDDLETANK_BB_LEFT, enemy->pos.y + MIDDLETANK_BB_TOP, MIDDLETANK_BB_WIDTH, MIDDLETANK_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= BOMBER_DAMAGE;
			}
		}
	}

	/* ��ԂȂ̂Ńv���C���[�Ƃ̓����蔻��̓I�~�b�g
	//�v���C���[�Ƃ̓����蔻��
	PLAYER *player = GetPlayerAdr(0);
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->state != PLAYER_DEFAULT)
		{
			continue;
		}

		enemy = GetMiddleTankAdr(0);
		for (j = 0; j < MIDDLETANK_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + MIDDLETANK_BB_LEFT, enemy->pos.y + MIDDLETANK_BB_TOP, MIDDLETANK_BB_WIDTH, MIDDLETANK_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				DestroyPlayer(i);
			}
		}
	}
	*/
}

/******************************************************************************
//�֐���	�Fvoid CollisionSmallTank(void)
//����1		�Fvoid
//�߂�l	�Fvoid
//����		�F�X���[���^���N�̐ڐG����
******************************************************************************/
void CollisionSmallTank(void)
{
	int i, j;
	PLAYERBULLET *pBullet = GetPlayerBulletAdr(0);
	SMALLTANK *enemy = GetSmallTankAdr(0);
	D3DXVECTOR4 bb1, bb2;

	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetSmallTankAdr(0);
		for (j = 0; j < SMALLTANK_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLTANK_BB_LEFT, enemy->pos.y + SMALLTANK_BB_TOP, SMALLTANK_BB_WIDTH, SMALLTANK_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	pBullet = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetSmallTankAdr(0);
		for (j = 0; j < SMALLTANK_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLTANK_BB_LEFT, enemy->pos.y + SMALLTANK_BB_TOP, SMALLTANK_BB_WIDTH, SMALLTANK_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	//�{���Ƃ̓����蔻��
	PLAYERBOMBER *bom = GetPlayerBomberAdr(0);
	for (i = 0; i < PLAYERBOMBER_MAX; i++, bom++)
	{
		if (!bom->active)
		{
			continue;
		}
		enemy = GetSmallTankAdr(0);
		for (j = 0; j < SMALLTANK_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(bom->pos.x + PLAYERBOMBER_BB_LEFT, bom->pos.y + PLAYERBOMBER_BB_TOP, PLAYERBOMBER_BB_WIDTH * bom->scale, PLAYERBOMBER_BB_HIGHT * bom->scale);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLTANK_BB_LEFT, enemy->pos.y + SMALLTANK_BB_TOP, SMALLTANK_BB_WIDTH, SMALLTANK_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= BOMBER_DAMAGE;
			}
		}
	}

	/* ��Ԃ����v���C���[�Ƃ̓����蔻��̓I�~�b�g
	//�v���C���[�Ƃ̓����蔻��
	PLAYER *player = GetPlayerAdr(0);
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (!player->active)
		{
			continue;
		}

		enemy = GetSmallTankAdr(0);
		for (j = 0; j < SMALLTANK_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLTANK_BB_LEFT, enemy->pos.y + SMALLTANK_BB_TOP, SMALLTANK_BB_WIDTH, SMALLTANK_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				DestroyPlayer(i);
			}
		}
	}
	*/
}

/******************************************************************************
//�֐���	�Fvoid CollisionSmallTank(void)
//����1		�Fvoid
//�߂�l	�Fvoid
//����		�F�X���[���U�R1�̐ڐG����
******************************************************************************/
void CollisionSmallZako1(void)
{
	int i, j;
	PLAYERBULLET *pBullet = GetPlayerBulletAdr(0);
	SMALLZAKO1 *enemy = GetSmallZako1Adr(0);
	D3DXVECTOR4 bb1, bb2;

	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetSmallZako1Adr(0);
		for (j = 0; j < SMALLZAKO1_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLZAKO1_BB_LEFT, enemy->pos.y + SMALLZAKO1_BB_TOP, SMALLZAKO1_BB_WIDTH, SMALLZAKO1_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	pBullet = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetSmallZako1Adr(0);
		for (j = 0; j < SMALLZAKO1_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLZAKO1_BB_LEFT, enemy->pos.y + SMALLZAKO1_BB_TOP, SMALLZAKO1_BB_WIDTH, SMALLZAKO1_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	//�{���Ƃ̓����蔻��
	PLAYERBOMBER *bom = GetPlayerBomberAdr(0);
	for (i = 0; i < PLAYERBOMBER_MAX; i++, bom++)
	{
		if (!bom->active)
		{
			continue;
		}
		enemy = GetSmallZako1Adr(0);
		for (j = 0; j < SMALLZAKO1_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(bom->pos.x + PLAYERBOMBER_BB_LEFT, bom->pos.y + PLAYERBOMBER_BB_TOP, PLAYERBOMBER_BB_WIDTH * bom->scale, PLAYERBOMBER_BB_HIGHT * bom->scale);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLZAKO1_BB_LEFT, enemy->pos.y + SMALLZAKO1_BB_TOP, SMALLZAKO1_BB_WIDTH, SMALLZAKO1_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= BOMBER_DAMAGE;
			}
		}
	}

	//�v���C���[�Ƃ̓����蔻��
	PLAYER *player = GetPlayerAdr(0);
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->state != PLAYER_DEFAULT)
		{
			continue;
		}

		enemy = GetSmallZako1Adr(0);
		for (j = 0; j < SMALLZAKO1_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLZAKO1_BB_LEFT, enemy->pos.y + SMALLZAKO1_BB_TOP, SMALLZAKO1_BB_WIDTH, SMALLZAKO1_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				DestroyPlayer(i);
			}
		}
	}
}

/******************************************************************************
//�֐���	�Fvoid CollisionSmallTank(void)
//����1		�Fvoid
//�߂�l	�Fvoid
//����		�F�X���[���U�R2�̐ڐG����
******************************************************************************/
void CollisionSmallZako2(void)
{
	int i, j;
	PLAYERBULLET *pBullet = GetPlayerBulletAdr(0);
	SMALLZAKO2 *enemy = GetSmallZako2Adr(0);
	D3DXVECTOR4 bb1, bb2;

	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetSmallZako2Adr(0);
		for (j = 0; j < SMALLZAKO2_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLZAKO2_BB_LEFT, enemy->pos.y + SMALLZAKO2_BB_TOP, SMALLZAKO2_BB_WIDTH, SMALLZAKO2_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	pBullet = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetSmallZako2Adr(0);
		for (j = 0; j < SMALLZAKO2_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLZAKO2_BB_LEFT, enemy->pos.y + SMALLZAKO2_BB_TOP, SMALLZAKO2_BB_WIDTH, SMALLZAKO2_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	//�{���Ƃ̓����蔻��
	PLAYERBOMBER *bom = GetPlayerBomberAdr(0);
	for (i = 0; i < PLAYERBOMBER_MAX; i++, bom++)
	{
		if (!bom->active)
		{
			continue;
		}
		enemy = GetSmallZako2Adr(0);
		for (j = 0; j < SMALLZAKO2_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(bom->pos.x + PLAYERBOMBER_BB_LEFT, bom->pos.y + PLAYERBOMBER_BB_TOP, PLAYERBOMBER_BB_WIDTH * bom->scale, PLAYERBOMBER_BB_HIGHT * bom->scale);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLZAKO2_BB_LEFT, enemy->pos.y + SMALLZAKO2_BB_TOP, SMALLZAKO2_BB_WIDTH, SMALLZAKO2_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= BOMBER_DAMAGE;
			}
		}
	}

	//�v���C���[�Ƃ̓����蔻��
	PLAYER *player = GetPlayerAdr(0);
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->state != PLAYER_DEFAULT)
		{
			continue;
		}

		enemy = GetSmallZako2Adr(0);
		for (j = 0; j < SMALLZAKO2_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + SMALLZAKO2_BB_LEFT, enemy->pos.y + SMALLZAKO2_BB_TOP, SMALLZAKO2_BB_WIDTH, SMALLZAKO2_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				DestroyPlayer(i);
			}
		}
	}
}

/******************************************************************************
//�֐���	�Fvoid CollisionItemCareer(void)
//����1		�Fvoid
//�߂�l	�Fvoid
//����		�F�A�C�e���L�����A�̐ڐG����
******************************************************************************/
void CollisionItemCareer(void)
{
	int i, j;
	PLAYERBULLET *pBullet = GetPlayerBulletAdr(0);
	ITEMCAREER *enemy = GetItemCareerAdr(0);
	D3DXVECTOR4 bb1, bb2;

	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetItemCareerAdr(0);
		for (j = 0; j < ITEMCAREER_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + ITEMCAREER_BB_LEFT, enemy->pos.y + ITEMCAREER_BB_TOP, ITEMCAREER_BB_WIDTH, ITEMCAREER_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	pBullet = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetItemCareerAdr(0);
		for (j = 0; j < ITEMCAREER_MAX; j++, enemy++)

		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + ITEMCAREER_BB_LEFT, enemy->pos.y + ITEMCAREER_BB_TOP, ITEMCAREER_BB_WIDTH, ITEMCAREER_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	//�{���Ƃ̓����蔻��
	PLAYERBOMBER *bom = GetPlayerBomberAdr(0);
	for (i = 0; i < PLAYERBOMBER_MAX; i++, bom++)
	{
		if (!bom->active)
		{
			continue;
		}
		enemy = GetItemCareerAdr(0);
		for (j = 0; j < ITEMCAREER_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(bom->pos.x + PLAYERBOMBER_BB_LEFT, bom->pos.y + PLAYERBOMBER_BB_TOP, PLAYERBOMBER_BB_WIDTH * bom->scale, PLAYERBOMBER_BB_HIGHT * bom->scale);
			bb2 = D3DXVECTOR4(enemy->pos.x + ITEMCAREER_BB_LEFT, enemy->pos.y + ITEMCAREER_BB_TOP, ITEMCAREER_BB_WIDTH, ITEMCAREER_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= BOMBER_DAMAGE;
			}
		}
	}

	//�v���C���[�Ƃ̓����蔻��
	PLAYER *player = GetPlayerAdr(0);
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->state != PLAYER_DEFAULT)
		{
			continue;
		}

		enemy = GetItemCareerAdr(0);
		for (j = 0; j < ITEMCAREER_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}
			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + ITEMCAREER_BB_LEFT, enemy->pos.y + ITEMCAREER_BB_TOP, ITEMCAREER_BB_WIDTH, ITEMCAREER_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				DestroyPlayer(i);
			}
		}
	}

}

/******************************************************************************
//�֐���	�Fvoid CollisionStage1BOss(void)
//����1		�Fvoid
//�߂�l	�Fvoid
//����		�F�X�e�[�W1�{�X�̐ڐG����
******************************************************************************/
void CollisionStage1Boss(void)
{
	int i;
	PLAYERBULLET *pBullet = GetPlayerBulletAdr(0);
	STAGE1BOSS *enemy = GetStage1BossAdr();
	D3DXVECTOR4 bb1, bb2;

	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetStage1BossAdr();

		if (!enemy->active)
		{
			continue;
		}

		bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
		bb2 = D3DXVECTOR4(enemy->pos.x + STAGE1BOSS_BB_LEFT, enemy->pos.y + STAGE1BOSS_BB_TOP, STAGE1BOSS_BB_WIDTH, STAGE1BOSS_BB_HIGHT);
		if (HitBB(bb1, bb2))
		{
			enemy->hp -= 1;
			SetPBulletExplosion(pBullet->pos);
			pBullet->active = false;
		}
	}

	pBullet = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetStage1BossAdr();
		if (!enemy->active)
		{
			break;;
		}

		bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
		bb2 = D3DXVECTOR4(enemy->pos.x + STAGE1BOSS_BB_LEFT, enemy->pos.y + STAGE1BOSS_BB_TOP, STAGE1BOSS_BB_WIDTH, STAGE1BOSS_BB_HIGHT);
		if (HitBB(bb1, bb2))
		{
			enemy->hp -= 1;
			SetPBulletExplosion(pBullet->pos);
			pBullet->active = false;
		}
	}

	//�{���Ƃ̓����蔻��
	PLAYERBOMBER *bom = GetPlayerBomberAdr(0);
	for (i = 0; i < PLAYERBOMBER_MAX; i++, bom++)
	{
		if (!bom->active)
		{
			continue;
		}
		enemy = GetStage1BossAdr();
		if (!enemy->active)
		{
			continue;
		}

		bb1 = D3DXVECTOR4(bom->pos.x + PLAYERBOMBER_BB_LEFT, bom->pos.y + PLAYERBOMBER_BB_TOP, PLAYERBOMBER_BB_WIDTH * bom->scale, PLAYERBOMBER_BB_HIGHT * bom->scale);
		bb2 = D3DXVECTOR4(enemy->pos.x + STAGE1BOSS_BB_LEFT, enemy->pos.y + STAGE1BOSS_BB_TOP, STAGE1BOSS_BB_WIDTH, STAGE1BOSS_BB_HIGHT);
		if (HitBB(bb1, bb2))
		{
			enemy->hp -= BOMBER_DAMAGE;
		}

	}

	//�v���C���[�Ƃ̓����蔻��
	PLAYER *player = GetPlayerAdr(0);
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->state != PLAYER_DEFAULT)
		{
			continue;
		}

		enemy = GetStage1BossAdr();
		if (!enemy->active)
		{
			break;
		}

		bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
		bb2 = D3DXVECTOR4(enemy->pos.x + STAGE1BOSS_BB_LEFT, enemy->pos.y + STAGE1BOSS_BB_TOP, STAGE1BOSS_BB_WIDTH, STAGE1BOSS_BB_HIGHT);
		if (HitBB(bb1, bb2))
		{
			DestroyPlayer(i);
		}

	}
}

/******************************************************************************
//�֐���	�Fvoid CollisionTutorialEnemy(void)
//����1		�Fvoid
//�߂�l	�Fvoid
//����		�F�X�e�[�W1�{�X�̐ڐG����
******************************************************************************/
void CollisionTutorialEnemy(void)
{
	int i, j;
	PLAYERBULLET *pBullet = GetPlayerBulletAdr(0);
	TUTORIALENEMY *enemy = GetTutorialEnemyAdr(0);
	D3DXVECTOR4 bb1, bb2;


	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetTutorialEnemyAdr(0);
		for (j = 0; j < TUTORIALENEMY_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + TUTORIALENEMY_BB_LEFT, enemy->pos.y + TUTORIALENEMY_BB_TOP, TUTORIALENEMY_BB_WIDTH, TUTORIALENEMY_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}

	pBullet = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, pBullet++)
	{
		if (!pBullet->active)
		{
			continue;
		}

		enemy = GetTutorialEnemyAdr(0);
		for (j = 0; j < TUTORIALENEMY_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(pBullet->pos.x + PLAYERBULLET_BB_LEFT, pBullet->pos.y + PLAYERBULLET_BB_TOP, PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + TUTORIALENEMY_BB_LEFT, enemy->pos.y + TUTORIALENEMY_BB_TOP, TUTORIALENEMY_BB_WIDTH, TUTORIALENEMY_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= 1;
				SetPBulletExplosion(pBullet->pos);
				pBullet->active = false;
			}
		}
	}


	//�{���Ƃ̓����蔻��
	PLAYERBOMBER *bom = GetPlayerBomberAdr(0);
	for (i = 0; i < PLAYERBOMBER_MAX; i++, bom++)
	{
		if (!bom->active)
		{
			continue;
		}

		enemy = GetTutorialEnemyAdr(0);
		for (j = 0; j < TUTORIALENEMY_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(bom->pos.x + PLAYERBOMBER_BB_LEFT, bom->pos.y + PLAYERBOMBER_BB_TOP, PLAYERBOMBER_BB_WIDTH * bom->scale, PLAYERBOMBER_BB_HIGHT * bom->scale);
			bb2 = D3DXVECTOR4(enemy->pos.x + TUTORIALENEMY_BB_LEFT, enemy->pos.y + TUTORIALENEMY_BB_TOP, TUTORIALENEMY_BB_WIDTH, TUTORIALENEMY_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				enemy->hp -= BOMBER_DAMAGE;
			}
		}
	}

	//�v���C���[�Ƃ̓����蔻��
	PLAYER *player = GetPlayerAdr(0);
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->state != PLAYER_DEFAULT)
		{
			continue;
		}

		enemy = GetTutorialEnemyAdr(0);
		for (j = 0; j < TUTORIALENEMY_MAX; j++, enemy++)
		{
			if (!enemy->active)
			{
				continue;
			}

			bb1 = D3DXVECTOR4(player->pos.x + PLAYER_BB_LEFT, player->pos.y + PLAYER_BB_TOP, PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
			bb2 = D3DXVECTOR4(enemy->pos.x + TUTORIALENEMY_BB_LEFT, enemy->pos.y + TUTORIALENEMY_BB_TOP, TUTORIALENEMY_BB_WIDTH, TUTORIALENEMY_BB_HIGHT);
			if (HitBB(bb1, bb2))
			{
				DestroyPlayer(i);
			}
		}
	}
}

/******************************************************************************
//�֐���	�Fvoid CollisionPlayerShield(void)
//����1		�Fvoid
//�߂�l	�Fvoid
//����		�F�v���C���[�V�[���h�̐ڐG����
******************************************************************************/
/*
void CollisionPlayerShield(void)
{
	PLAYERSHIELD *shield = GetPlayerShieldAdr();
	ENEMYBULLET *bullet = GetEnemyBulletAdr(GREEN_ROTATE, 0);

	if (!shield->active)
	{
		return;
	}

	for (int i = 0; i < ENEMYBULLET_MAX / 2; i++, bullet++)
	{
		if (!bullet->active)
		{
			continue;
		}

		if (HitBC(shield->pos, bullet->pos, PLAYERSHIELD_BC_RADIUS, ENEMYBULLET_BC_RADIUS))
		{
			bullet->active = false;
		}
	}
}
*/

/******************************************************************************
//�֐���	�Fvoid CollisionBomber(void)
//����1		�Fvoid
//�߂�l	�Fvoid
//����		�F�v���C���[�{���o�[�̐ڐG����
******************************************************************************/
void CollisionBomber(void)
{
	PLAYERBOMBER *bomber = GetPlayerBomberAdr(0);
	ENEMYBULLET *bullet = GetEnemyBulletAdr(GREEN_ROTATE, 0);

	for (int i = 0; i < PLAYERBOMBER_MAX; i++, bomber++)
	{
		if (!bomber->active)
		{
			continue;
		}

		bullet = GetEnemyBulletAdr(GREEN_ROTATE, 0);
		for (int j = 0; j < ENEMYBULLET_MAX; j++, bullet++)
		{
			if (HitBC(bomber->pos, bullet->pos, PLAYERBOMBER_BC_RADIUS * bomber->scale, ENEMYBULLET_BC_RADIUS))
			{
				bullet->active = false;
			}
		}
	}

}

/******************************************************************************
//�֐���	�Fvoid CollisionPlayerShield(void)
//����1		�Fvoid
//�߂�l	�Fvoid
//����		�F�v���C���[�V�[���h�̐ڐG����
******************************************************************************/
void CollisionPlayerShield(void)
{
	PLAYERSHIELD *shield = GetPlayerShieldAdr();
	ENEMYBULLET *bullet = GetEnemyBulletAdr(GREEN_ROTATE, 0);

	if (!shield->active)
	{
		return;
	}

	for (int i = 0; i < ENEMYBULLET_MAX; i++, bullet++)
	{
		if (!bullet->active)
		{
			continue;
		}

		if (HitBC(shield->pos, bullet->pos, PLAYERSHIELD_BC_RADIUS, ENEMYBULLET_BC_RADIUS))
		{
			bullet->active = false;
			SetEnemyBulletExplosion(bullet->pos, bullet->type);
		}
	}
}