//=============================================================================
//
// �X�e�[�W1�{�X���� [stage1Boss.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "enemyManager.h"
#include "player.h"
#include "gameClear.h"
#include "particle.h"
#include "stageManager.h"
#include "bgmPlayer.h"
#include "sePlayer.h"
#include "GUI.h"
#include "enemyParticle.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define STAGE1BOSS_SPEED (3.5f)

#define STAGE1BOSS_INITHP					(5500)		//����HP
#define STAGE1BOSS_SHADOW_OFFSET			(D3DXVECTOR3(50.0f, 100.0f, 0.0f))		//�e�̃I�t�Z�b�g���W
#define STAGE1BOSS_TARGETPOS				(D3DXVECTOR3(SCREEN_CENTER_X, 200.0f, 0.0f))	//�ړ��̖ڕW���W

#define STAGE1BOSS_COUNTMOVE_MIN			(0)			//�J�E���g���[�u�̍ŏ��l
#define STAGE1BOSS_COUNTMOVE_MAX			(720)		//�J�E���g���[�u�̍ő�l
#define STAGE1BOSS_COUNTMOVE_BORDER			(360)		//�ړ��������؂�ւ�邵�����l
#define STAGE1BOSS_MOVE_LENGTH_X			(0.7f)		//�ړ���X��
#define STAGE1BOSS_MOVE_LENGTH_Y			(0.1f)		//�ړ���Y��

#define STAGE1BOSS_WAY1_ATKMAX				(6)			//WAY1�U�����s����
#define STAGE1BOSS_WAY1_SHOTPOS				(D3DXVECTOR3(-100.0f, -100.0f, 0.0f))	//WAY�U���̔��ˈʒu
#define STAGE1BOSS_WAY1_ANGLE				(70.0f)		//WAY1�U���̔��ˊp�x
#define STAGE1BOSS_WAY1_SPEED				(5.0f)		//WAY1�U���̃o���b�g�X�s�[�h
#define STAGE1BOSS_WAY1_INTERBAL			(60)		//WAY1�U���̊Ԋu
#define STAGE1BOSS_WAY1_ORANGE_WAY			(5)			//WAY1�U���̃I�����W�̒e��WAY��
#define STAGE1BOSS_WAY1_ORANGE_RANGE		(80.0f)		//WAY1�U���̃I�����W�̒e�̊p�x
#define STAGE1BOSS_WAY1_GREEN_WAY			(4)			//WAY1�U���̗΂̋ʂ�WAY��
#define STAGE1BOSS_WAY1_GREEN_RANGE			(60.0f)		//WAY1�U���̗΂̋ʂ̊p�x
#define STAGE1BOSS_WAY1_LOOPMAXEASY			(5)			//WAY1�U����1��ɕ��e�̐��iEASY)
#define STAGE1BOSS_WAY1_LOOPMAXNORMAL		(10)		//WAY1�U����1��ɕ��e�̐��iNORMAL)
#define STAGE1BOSS_WAY1_LOOPMAXHARD			(20)		//WAY1�U����1��ɕ��e�̐��iHARD)
#define STAGE1BOSS_WAY1_SPEEDOFFSET			(0.2f)		//WAY1�U���Ō��e�̑��x����

#define STAGE1BOSS_WAY2_SHOTPOS				(D3DXVECTOR3(100.0f, -100.0f, 0.0f))	//WAY2�U���̔��ˈʒu
#define STAGE1BOSS_WAY2_INTERBAL			(120)		//WAY2�U���̔��ˊԊu

#define STAGE1BOSS_WIP1_SHOTPOS_L			(D3DXVECTOR3(-170.0f, 180.0f, 0.0f))	//WIP1�U���̔��ˈʒu(���j
#define STAGE1BOSS_WIP1_SHOTPOS_R			(D3DXVECTOR3(170.0f, 180.0f, 0.0f))		//WIP1�U���̔��ˈʒu�i�E�j
#define STAGE1BOSS_WIP1_RANGEEASY			(15)		//WIP1�U����WAY�̊p�x�iEASY)
#define STAGE1BOSS_WIP1_RANGENORMAL			(30)		//WIP1�U����WAY�̊p�x�iNORMAL)
#define STAGE1BOSS_WIP1_RANGEHARD			(45)		//WIP1�U����WAY�̊p�x�iHARD)
#define STAGE1BOSS_WIP1_ATKMAX				(72)		//WIP1�U����1���[�v�ő�U����
#define STAGE1BOSS_WIP1_INTERBAL			(1)			//WIP1�U���̊Ԋu
#define STAGE1BOSS_WIP1_SPEED				(2.5f)		//WIP1�U���̃o���b�g�X�s�[�h
#define STAGE1BOSS_WIP1_SPEEDOFFSET			(0.02f)		//WIP1�U���̃o���b�g�X�s�[�h����
#define STAGE1BOSS_WIP1_ROTANGLE			(5.0f)		//WIP1�U���̉�]�p�x
#define STAGE1BOSS_WIPMAX					(4*STAGE1BOSS_WIP1_INTERBAL*STAGE1BOSS_WIP1_ATKMAX)	//WIP1�U���̃��[�v�ő��
#define STAGE1BOSS_WIP_PERIOD				(12)		//WIP�U���̎���
#define STAGE1BOSS_WIP_BORDER				(STAGE1BOSS_WIP_PERIOD/2)	//WIP1�U���ŃI�����W�Ɨ΂��؂�ւ��^�C�~���O
#define STAGE1BOSS_WIP_EASYBORDER1			(3)			//���̃t���[�����ȉ��̎��AEASY�ł͍U�����Ȃ�
#define STAGe1BOSS_WIP_EASYBORDEE2			(9)			//���̃t���[�����ȏ�̎��AEASY�ł͍U�����Ȃ�

#define STAGE1BOSS_CIRCLE_SHOTPOS			(D3DXVECTOR3(0.0f, -50.0f, 0.0f))	//CIRCLE�U���̔��ˈʒu
#define STAGE1BOSS_CIRCLE_INTERBAL			(3)			//CIRCLE�U���̔��ˊԊu
#define STAGE1BOSS_CIRCLE_SPEED				(6.0f)		//CIRCLE�U���̃o���b�g�X�s�[�h
#define STAGE1BOSS_CIRCLE_ATKMAX			(239)		//CIRCLE�U���̍U���ő��
#define STAGE1BOSS_CIRCLE_ROTANGLE			(0.3f)		//CIRCLE�U���̉�]�p�x
#define STAGE1BOSS_CIRCLE_PERIODEASY		(20*STAGE1BOSS_CIRCLE_INTERBAL)	//CIRCLE�U���̎����iEASY�j
#define STAGE1BOSS_CIRCLE_PERIODNOMAL		(16*STAGE1BOSS_CIRCLE_INTERBAL)	//CIRCLE�U���̎����iNORMAL�j
#define STAGE1BOSS_CIRCLE_PERIODHARD		(10*STAGE1BOSS_CIRCLE_INTERBAL)	//CIRCLE�U���̎����iHARD)
//#define STAGE1BOSS_CIRCLE_BORDER			(STAGE1BOSS_CIRCLE_PERIOD/2)	//CIRCLE�U���̃I�����W�Ɨ΂��؂�ւ��^�C�~���O	
#define STAGE1BOSS_CIRCLE_WAY				(8)			//CIRCLE�U����WAY��
#define STAGE1BOSS_CIRCLE_WAYHARD			(12)		//CIRCLE�U����WAY���iHARD�j
#define STAGE1BOSS_CIRCLE_EASYBORDER1		(5*STAGE1BOSS_CIRCLE_INTERBAL)			//���̃t���[�����ȉ��̎��AEASY�ł�CIRCLE�U�����Ȃ�
#define STAGE1BOSS_CIRCLE_EASYBORDER2		(15*STAGE1BOSS_CIRCLE_INTERBAL)		//���̃t���[�����ȏ�̎��AEASY�ł�CIRCLE�U�����Ȃ�

#define STAGE1BOSS_SCORE					(10000)		//���Ď��̃X�R�A

#define STAGE1BOSS_DESTROYED_FRAMEMAX		(180)		//DESTROY��Ԃ���J�ڂ��鎞��
#define STAGE1BOSS_DESTROYED_EXPLRANGE		(150.0f)	//DESTROY��ԂŔ����𐶐�������W�͈̔�
#define STAGE1BOSS_DESTROYED_EXPLFRAME		(180)		//DESTROY��ԂŔ����𐶐��������鎞��
#define STAGE1BOSS_DESTROYED_FXPLINTERBAL	(10)		//DESTROY��ԂŔ����𐶐�����Ԋu

#define STAGE1BOSS_INTERBAL_FRAME			(120)		//INTERBAL��Ԃ���J�ڂ��鎞��
#define STAGE1BOSS_BGM_FADEOUTFRAME			(60)		//BGM���t�F�[�h�A�E�g����b��

#define STAGE1BOSS_QUAKE_LENGTH				(0.01f)		//DESTRYOED��ԂŐݒ肷��e�N�X�`�����W�̃I�t�Z�b�g
#define STAGE1BOSS_EXPLOSION_RANGE_X		(300.0f)	//���Ď��ɔ������Z�b�g����͈́iX)
#define STAGE1BOSS_EXPLOSION_RANGE_Y		(150.0f)	//���Ď��ɔ������Z�b�g����͈́iY)
#define STAGE1BOSS_EXPLOSION_NUM			(5)			//���Ď��ɃZ�b�g���锚���̐�

#define BOSSFIRE_EMITPARTICLE_NUM	(10)
#define BOSSFIRE_EMITOARTUCLE_DURATION	(5)

#define BOSSEXPLOSION_EMITPARTICLE_NUM	(50)
#define BOSSEXPLOSION_EMITPARTICLE_DURATION (45)

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexStage1Boss(void);				//���_�쐬�֐�
void SetTextureStage1Boss(STAGE1BOSS *ptr);		// �e�N�X�`�����W�̌v�Z����
void SetTextureStage1BossShadow(STAGE1BOSS *ptr);
void SetVertexStage1Boss(STAGE1BOSS *ptr);		// ���_�̌v�Z����
void SetVertexStage1BossShadow(STAGE1BOSS *ptr);

//�e�`�揈��
void DrawStage1BossShadow(LPDIRECT3DDEVICE9 pDevice);
void DrawStage1BossBody(LPDIRECT3DDEVICE9 pDevice);

//�e��Ԃ̍s������
void ActInitStage1Boss(STAGE1BOSS *ptr);
void ActInterbalStage1Boss(STAGE1BOSS *ptr);

void ActWay1Stage1Boss(STAGE1BOSS *ptr);
void AttackWay1(STAGE1BOSS *ptr, int side);

void ActWip1Stage1Boss(STAGE1BOSS *ptr);
void AttackWip1(STAGE1BOSS *ptr, int frame, int side);
void ActWip2Stage1Boss(STAGE1BOSS *ptr);

void ActCircleStage1Boss(STAGE1BOSS *ptr);
void AttackCircleStage1Boss(STAGE1BOSS *ptr, int frame);

void ActDestroyedStage1Boss(STAGE1BOSS *ptr);

void MoveStage1Boss(STAGE1BOSS *ptr);			//�ړ�����
void DestroyStage1Boss(STAGE1BOSS *ptr);		//���ď���
void ExplodeStage1Boss(STAGE1BOSS *ptr);		//��������

/*****************************************************************************
�\���̒�`
*****************************************************************************/
enum STAGE1BOSS_TEXTURE
{
	STAGE1BOSS_SHADOW,
	STAGE1BOSS_BODY,
	STAGE1BOSS_TEXMAX
};

enum
{
	ATK_RIGHT,
	ATK_LEFT
};

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[STAGE1BOSS_TEXMAX];	// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N

STAGE1BOSS stage1Boss;									//�X�e�[�W1�{�X�\����

//�U�����̒e�̃p�����[�^
const static ENEMYATTACK bulletWay1Orange = 
{
	ORANGE_CIRCLE,
	STAGE1BOSS_WAY1_SHOTPOS,
	STAGE1BOSS_WAY1_SPEED,
	RADIAN(STAGE1BOSS_WAY1_ANGLE)
};

const static ENEMYATTACK bulletWay1Green =
{
	GREEN_CIRCLE,
	STAGE1BOSS_WAY1_SHOTPOS,
	STAGE1BOSS_WAY1_SPEED,
	RADIAN(STAGE1BOSS_WAY1_ANGLE)
};

const static ENEMYATTACK bulletWip1Orange =
{
	ORANGE_ROTATE,
	STAGE1BOSS_WIP1_SHOTPOS_L,
	STAGE1BOSS_WIP1_SPEED,
	ENEMYBULLET_DAFAULT_ANGLE
};

const static ENEMYATTACK bulletWip1Green =
{
	GREEN_ROTATE,
	STAGE1BOSS_WIP1_SHOTPOS_R,
	STAGE1BOSS_WIP1_SPEED,
	ENEMYBULLET_DAFAULT_ANGLE
};

const static ENEMYATTACK bulletCircleOrange =
{
	ORANGE_CIRCLE,
	STAGE1BOSS_CIRCLE_SHOTPOS,
	STAGE1BOSS_CIRCLE_SPEED,
	ENEMYBULLET_DAFAULT_ANGLE
};

const static ENEMYATTACK bulletCircleGreen =
{
	GREEN_CIRCLE,
	STAGE1BOSS_CIRCLE_SHOTPOS,
	STAGE1BOSS_CIRCLE_SPEED,
	ENEMYBULLET_DAFAULT_ANGLE
};

/******************************************************************************
����������
******************************************************************************/
HRESULT InitStage1Boss(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	STAGE1BOSS* ptr = GetStage1BossAdr();

	ptr->pos = D3DXVECTOR3(STAGE1BOSS_INITPOS_X, STAGE1BOSS_INITPOS_Y, 0.0f);
	ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	ptr->angle = atan2f(STAGE1BOSS_TEXTURE_SIZE_Y, STAGE1BOSS_TEXTURE_SIZE_X);
	ptr->radius = D3DXVec2Length(&D3DXVECTOR2(STAGE1BOSS_TEXTURE_SIZE_X, STAGE1BOSS_TEXTURE_SIZE_Y));

	ptr->countAnim = 0;
	ptr->patternAnim = 0;

	ptr->active = false;


	// ���_���̍쐬
	MakeVertexStage1Boss();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[STAGE1BOSS_SHADOW] = CreateTextureFromFile((LPSTR)STAGE1BOSS_SHADOWTEX_NAME, pDevice);
		texture[STAGE1BOSS_BODY] = CreateTextureFromFile((LPSTR)STAGE1BOSS_TEXTURE_NAME, pDevice);
	}

	//SetStage1Boss(D3DXVECTOR3(SCREEN_CENTER_X, -100.0f, 0.0f));

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitStage1Boss(void)
{
	for (int i = 0; i < STAGE1BOSS_TEXMAX; i++)
	{
		if (texture[i] != NULL)
		{	// �e�N�X�`���̊J��
			texture[i]->Release();
			texture[i] = NULL;
		}
	}
}

/******************************************************************************
�X�V����
******************************************************************************/
void UpdateStage1Boss(void)
{
	STAGE1BOSS *ptr = GetStage1BossAdr();

	if (!ptr->active)
	{
		return;
	}

	//���ď���
	if (ptr->hp <= 0 && ptr->state != STAGE1BOSS_DESTROYED)
	{
		PlaySE(SE_BOSSDESTROY);
		ConvertBulletToScoreItem();
		ptr->gauge->active = false;
		ptr->state = STAGE1BOSS_DESTROYED;
		ptr->cntFrame = 0;
		SetEmitterEnemyParticle(ptr->pos, BOSSEXPLOSION_EMITPARTICLE_DURATION, BOSSEXPLOSION_EMITPARTICLE_NUM, EMITTER_CIRCLE);
		FadeoutBGM(STAGE1BOSS_BGM_FADEOUTFRAME);
		SetGUIShake();
		return;
	}

	//�A�j���[�V����
	ptr->countAnim++;
	if (ptr->countAnim % STAGE1BOSS_TIME_ANIMATION == 0)
	{
		ptr->patternAnim = WrapAround(0, STAGE1BOSS_ANIM_NUM, ptr->patternAnim + 1);
	}

	//�Q�[�W�X�V
	ptr->gauge->percent = ptr->hp / (float)STAGE1BOSS_INITHP;

	//�s����������
	switch (ptr->state)
	{
	case STAGE1BOSS_INIT:
		ActInitStage1Boss(ptr);
		break;

	case STAGE1BOSS_INTERBAL:
		ActInterbalStage1Boss(ptr);
		break;

	case STAGE1BOSS_WAY1:
		ActWay1Stage1Boss(ptr);
		break;

	case STAGE1BOSS_WIP1:
		ActWip1Stage1Boss(ptr);
		break;
	
	case STAGE1BOSS_WIP2:
		ActWip2Stage1Boss(ptr);
		break;

	case STAGE1BOSS_CIRCLE:
		ActCircleStage1Boss(ptr);
		break;

	case STAGE1BOSS_DESTROYED:
		ActDestroyedStage1Boss(ptr);
		break;
	}
}

/******************************************************************************
�s�������iINIT��ԁj
******************************************************************************/
void ActInitStage1Boss(STAGE1BOSS *ptr)
{
	if (MoveToTarget(&ptr->pos, &ptr->targetPos, STAGE1BOSS_SPEED))
	{
		//��ԑJ��
		ptr->cntFrame = 0;
		ptr->state = STAGE1BOSS_WAY1;
	}
}

/******************************************************************************
�s�������iINTERBAL��ԁj
******************************************************************************/
void ActInterbalStage1Boss(STAGE1BOSS *ptr)
{
	MoveStage1Boss(ptr);
	ptr->cntFrame++;

	if (ptr->cntFrame == STAGE1BOSS_INTERBAL_FRAME)
	{
		ptr->state = ptr->nextState;
		ptr->cntFrame = 0;
		ptr->atkNum = 0;
	}
}

/******************************************************************************
�s�������iWAY1��ԁj
******************************************************************************/
void ActWay1Stage1Boss(STAGE1BOSS *ptr)
{
	MoveStage1Boss(ptr);

	//�U���^�C�~���O�̂݃o���b�g����
	if (ptr->cntFrame % STAGE1BOSS_WAY2_INTERBAL == 0)
	{
		AttackWay1(ptr, ATK_LEFT);
	}
	else if (ptr->cntFrame % STAGE1BOSS_WAY1_INTERBAL == 0)
	{
		AttackWay1(ptr, ATK_RIGHT);
	}

	//�U���񐔂��ő�ł���Ώ�ԑJ��
	if (ptr->atkNum == STAGE1BOSS_WAY1_ATKMAX)
	{
		ptr->state = STAGE1BOSS_INTERBAL;
		ptr->nextState = STAGE1BOSS_WIP1;
		ptr->cntFrame = 0;
		return;
	}

	ptr->cntFrame++;

}

//WAY1�U��
void AttackWay1(STAGE1BOSS *ptr, int side)
{
	//�I�����W�̒e��WAY�e�𔭎�
	ENEMYATTACK param = bulletWay1Orange;
	PLAYER *player = GetPlayerAdr(0);
	int difficulty = GetDifficulty();
	int loopMax = (difficulty == DIFFICULTY_EASY) ? STAGE1BOSS_WAY1_LOOPMAXEASY : (difficulty == DIFFICULTY_NORMAL) ? STAGE1BOSS_WAY1_LOOPMAXNORMAL : STAGE1BOSS_WAY1_LOOPMAXHARD;

	//�I�����W����
	param.pos = ptr->pos;
	param.pos += (side == ATK_RIGHT) ? STAGE1BOSS_WAY2_SHOTPOS : STAGE1BOSS_WAY1_SHOTPOS;
	param.angle = SnipeTarget(&param.pos, &GetPlayerAdr(0)->pos);
	for (int i = 0; i < loopMax; i++)
	{
		FireWayBullet(&param, STAGE1BOSS_WAY1_ORANGE_WAY, STAGE1BOSS_WAY1_ORANGE_RANGE, &ptr->hp);
		param.speed += STAGE1BOSS_WAY1_SPEEDOFFSET;
	}

	//�΂Ŕ���
	param = bulletWay1Green;
	param.pos = ptr->pos;
	param.pos += (side == ATK_RIGHT) ? STAGE1BOSS_WAY2_SHOTPOS : STAGE1BOSS_WAY1_SHOTPOS;
	param.angle = SnipeTarget(&param.pos, &player->pos);
	for (int i = 0; i < loopMax; i++)
	{
		FireWayBullet(&param, STAGE1BOSS_WAY1_GREEN_WAY, STAGE1BOSS_WAY1_GREEN_RANGE, &ptr->hp);
		param.speed += STAGE1BOSS_WAY1_SPEEDOFFSET;
	}

	//�U���񐔂��J�E���g
	ptr->atkNum++;
}

/******************************************************************************
�s�������iWIP1��ԁj
******************************************************************************/
void ActWip1Stage1Boss(STAGE1BOSS *ptr)
{
	MoveStage1Boss(ptr);

	int frame = ptr->cntFrame % STAGE1BOSS_WIP_PERIOD;

	if (ptr->cntFrame == STAGE1BOSS_WIPMAX)
	{
		ptr->state = STAGE1BOSS_INTERBAL;
		ptr->cntFrame = 0;
		ptr->nextState = STAGE1BOSS_CIRCLE;
		return;
	}

	if (ptr->cntFrame % STAGE1BOSS_WIP1_INTERBAL == 0)
	{
		AttackWip1(ptr, frame, ATK_LEFT);
	}

	if (ptr->atkNum == STAGE1BOSS_WIP1_ATKMAX)
	{
		ptr->state = STAGE1BOSS_WIP2;
		ptr->atkNum = 0;
	}

	ptr->cntFrame++;
}

/******************************************************************************
�s�������iWIP2��ԁj
******************************************************************************/
void ActWip2Stage1Boss(STAGE1BOSS *ptr)
{
	MoveStage1Boss(ptr);

	int frame = ptr->cntFrame % STAGE1BOSS_WIP_PERIOD;

	if (ptr->cntFrame == STAGE1BOSS_WIPMAX)
	{
		ptr->state = STAGE1BOSS_INTERBAL;
		ptr->cntFrame = 0;
		ptr->atkNum = 0;
		ptr->nextState = STAGE1BOSS_CIRCLE;
		return;
	}

	if (ptr->cntFrame % STAGE1BOSS_WIP1_INTERBAL == 0)
	{
		AttackWip1(ptr, frame, ATK_RIGHT);
	}

	if (ptr->atkNum == STAGE1BOSS_WIP1_ATKMAX)
	{
		ptr->state = STAGE1BOSS_WIP1;
		ptr->atkNum = 0;
	}

	ptr->cntFrame++;
}

//WIP�U��
void AttackWip1(STAGE1BOSS *ptr,  int frame, int side)
{
	ENEMYATTACK param = (frame < STAGE1BOSS_WIP_BORDER) ? bulletWip1Orange : bulletWip1Green;
	int count = WrapAround(0, STAGE1BOSS_WIP1_ATKMAX * STAGE1BOSS_WIP1_INTERBAL, ptr->cntFrame);

	//EASY�ł͈ꕔ�t���[���ł͍U�����Ȃ�
	if (GetDifficulty() == DIFFICULTY_EASY)
	{
		if (frame < STAGE1BOSS_WIP_EASYBORDER1 || frame > STAGe1BOSS_WIP_EASYBORDEE2)
		{
			return;
		}
	}

	param.pos = ptr->pos;
	param.pos += (side == ATK_LEFT) ? STAGE1BOSS_WIP1_SHOTPOS_L : STAGE1BOSS_WIP1_SHOTPOS_R;

	param.angle += (side == ATK_LEFT) ? RADIAN(count * -STAGE1BOSS_WIP1_ROTANGLE) : RADIAN(count * STAGE1BOSS_WIP1_ROTANGLE);
	param.speed += count * STAGE1BOSS_WIP1_SPEEDOFFSET;

	SetEnemyBullet(&param, &ptr->hp);

	//HARD�ł͒ǉ��U��
	if (GetDifficulty() == DIFFICULTY_HARD)
	{
		param.angle += RADIAN(180);
		SetEnemyBullet(&param, &ptr->hp);
	}

	ptr->atkNum++;
}

/******************************************************************************
�s�������iCIECLE��ԁj
******************************************************************************/
void ActCircleStage1Boss(STAGE1BOSS *ptr)
{
	MoveStage1Boss(ptr);
	int difficulty = GetDifficulty();
	int period = (difficulty == DIFFICULTY_EASY) ? STAGE1BOSS_CIRCLE_PERIODEASY : (difficulty == DIFFICULTY_NORMAL) ? STAGE1BOSS_CIRCLE_PERIODNOMAL : STAGE1BOSS_CIRCLE_PERIODHARD;
	int frame = ptr->cntFrame % period;

	//��ԑJ��
	if (ptr->atkNum == STAGE1BOSS_CIRCLE_ATKMAX)
	{
		ptr->cntFrame = 0;
		ptr->atkNum = 0;
		ptr->state = STAGE1BOSS_INTERBAL;
		ptr->nextState = STAGE1BOSS_WAY1;
	}

	//�o���b�g����
	if (ptr->cntFrame % STAGE1BOSS_CIRCLE_INTERBAL == 0)
	{
		AttackCircleStage1Boss(ptr, frame);
	}

	ptr->cntFrame++;
}

//CIRCLE�U��
void AttackCircleStage1Boss(STAGE1BOSS *ptr, int frame)
{
	int difficulty = GetDifficulty();

	//EASY�ł͓���t���[���ł͍U�����s��Ȃ�
	if (difficulty == DIFFICULTY_EASY)
	{
		if (frame < STAGE1BOSS_CIRCLE_EASYBORDER1 || frame > STAGE1BOSS_CIRCLE_EASYBORDER2)
		{
			return;
		}
	}

	int period = (difficulty == DIFFICULTY_EASY) ? STAGE1BOSS_CIRCLE_PERIODEASY : (difficulty == DIFFICULTY_NORMAL) ? STAGE1BOSS_CIRCLE_PERIODNOMAL : STAGE1BOSS_CIRCLE_PERIODHARD;
	int border = period / 2;
	int way = (difficulty == DIFFICULTY_HARD) ? STAGE1BOSS_CIRCLE_WAYHARD : STAGE1BOSS_CIRCLE_WAY;

	ENEMYATTACK param = (frame < border) ? bulletCircleOrange : bulletCircleGreen;

	param.pos = ptr->pos + STAGE1BOSS_CIRCLE_SHOTPOS;

	param.angle += RADIAN(ptr->cntFrame * STAGE1BOSS_CIRCLE_ROTANGLE);

	FireCircleBullet(&param, way, &ptr->hp);

	ptr->atkNum++;
}

/******************************************************************************
�s�������iDESTROYED��ԁj
******************************************************************************/
void ActDestroyedStage1Boss(STAGE1BOSS *ptr)
{
	if (ptr->cntFrame == STAGE1BOSS_DESTROYED_FRAMEMAX)
	{
		DestroyStage1Boss(ptr);
	}

	if (ptr->cntFrame % STAGE1BOSS_DESTROYED_FXPLINTERBAL == 0)
	{
		ExplodeStage1Boss(ptr);
	}

	ptr->cntFrame++;

}


/******************************************************************************
�`�揈��
******************************************************************************/
void DrawStage1Boss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	STAGE1BOSS *ptr = GetStage1BossAdr();

	if (!ptr->active)
	{
		return;
	}

	DrawStage1BossShadow(pDevice);

	DrawStage1BossBody(pDevice);
#ifdef _DEBUG
	//�����蔻��̕`��
	//DrawBoxCollider(ptr->pos + D3DXVECTOR3(STAGE1BOSS_BB_LEFT, STAGE1BOSS_BB_TOP, 0.0f), STAGE1BOSS_BB_WIDTH, STAGE1BOSS_BB_HIGHT);
#endif
}

/******************************************************************************
�`�揈��(�e�p�j
******************************************************************************/
void DrawStage1BossShadow(LPDIRECT3DDEVICE9 pDevice)
{
	STAGE1BOSS *ptr = GetStage1BossAdr();

	pDevice->SetTexture(0, texture[STAGE1BOSS_SHADOW]);

	SetVertexStage1BossShadow(ptr);

	SetTextureStage1BossShadow(ptr);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
�`�揈���i�{�̗p�j
******************************************************************************/
void DrawStage1BossBody(LPDIRECT3DDEVICE9 pDevice)
{
	STAGE1BOSS *ptr = GetStage1BossAdr();

	pDevice->SetTexture(0, texture[STAGE1BOSS_BODY]);

	SetVertexStage1Boss(ptr);

	SetTextureStage1Boss(ptr);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexStage1Boss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// rhw�̐ݒ�
	vertexWk[0].rhw =
		vertexWk[1].rhw =
		vertexWk[2].rhw =
		vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	return S_OK;
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�
******************************************************************************/
void SetTextureStage1Boss(STAGE1BOSS *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = ptr->patternAnim % STAGE1BOSS_TEXTURE_DIVIDE_X;
	int y = ptr->form;
	float sizeX = 1.0f / STAGE1BOSS_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / STAGE1BOSS_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

	// DESTROYED��ԂȂ�h�炷
	if (ptr->state == STAGE1BOSS_DESTROYED)
	{
		float random = RandomRange(-STAGE1BOSS_QUAKE_LENGTH, STAGE1BOSS_QUAKE_LENGTH);
		vertexWk[0].tex.x += random;
		vertexWk[1].tex.x += random;
		vertexWk[2].tex.x += random;
		vertexWk[3].tex.x += random;

		random = RandomRange(-STAGE1BOSS_QUAKE_LENGTH, STAGE1BOSS_QUAKE_LENGTH);
		vertexWk[0].tex.y += random;
		vertexWk[1].tex.y += random;
		vertexWk[2].tex.y += random;
		vertexWk[3].tex.y += random;
	}
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�(�e�p�j
******************************************************************************/
void SetTextureStage1BossShadow(STAGE1BOSS *ptr)
{
	int y = ptr->form;
	float sizeY = 1.0f / STAGE1BOSS_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2(0.0f, y * sizeY);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, y * sizeY);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, y * sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, y * sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexStage1Boss(STAGE1BOSS *ptr)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius;
}

/******************************************************************************
���_���W�̐ݒ�i�e�p�j
******************************************************************************/
void SetVertexStage1BossShadow(STAGE1BOSS *ptr)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius;

	//�e�p�ɃI�t�Z�b�g
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertexWk[i].vtx += STAGE1BOSS_SHADOW_OFFSET;
	}
}

/******************************************************************************
//�֐���	�FSTAGE1BOSS *GetStage1BossAdr(int pno)
//����		�Fint pno�F�擾�������X�e�[�W1�{�X�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�X�e�[�W1�{�X�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�X�e�[�W1�{�X�̃A�h���X�擾�֐�
******************************************************************************/
STAGE1BOSS *GetStage1BossAdr(void)
{
	return &stage1Boss;
}

/******************************************************************************
�{�X�Z�b�g�֐�
******************************************************************************/
void SetStage1Boss(D3DXVECTOR3 pos)
{
	STAGE1BOSS *ptr = GetStage1BossAdr();

	if (ptr->active)
	{
		return;
	}

	ptr->pos = pos;
	ptr->targetPos = STAGE1BOSS_TARGETPOS;

	ptr->state = STAGE1BOSS_INIT;
	ptr->form = STAGE1BOSS_FIRST;

	ptr->countAnim = 0;
	ptr->patternAnim = 0;
	ptr->atkNum = 0;

	ptr->gauge = SetBossGauge();

	ptr->hp = STAGE1BOSS_INITHP;
	ptr->active = true;
}

/******************************************************************************
�{�X����ʓ��𓮂�����
******************************************************************************/
void MoveStage1Boss(STAGE1BOSS *ptr)
{
	ptr->cntMove = WrapAround(STAGE1BOSS_COUNTMOVE_MIN, STAGE1BOSS_COUNTMOVE_MAX, ptr->cntMove + 1);

	if (ptr->cntMove < STAGE1BOSS_COUNTMOVE_BORDER)
	{
		ptr->pos.x += STAGE1BOSS_MOVE_LENGTH_X * sinf(RADIAN(ptr->cntMove));
		ptr->pos.y += STAGE1BOSS_MOVE_LENGTH_Y * sinf(RADIAN(ptr->cntMove / 2.0f));
	}
	else
	{
		ptr->pos.x -= STAGE1BOSS_MOVE_LENGTH_X * sinf(RADIAN(ptr->cntMove));
		ptr->pos.y += STAGE1BOSS_MOVE_LENGTH_Y * sinf(RADIAN(ptr->cntMove / 2.0f));
	}
}

/******************************************************************************
//�֐���	�Fvoid DestroyStage1Boss(STAGE1BOSS *ptr)
//����		�FSTAGE1BOSS *ptr�F���Ă����{�X�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�X�e�[�W1�{�X���ď���
******************************************************************************/
void DestroyStage1Boss(STAGE1BOSS *ptr)
{
	D3DXVECTOR3 pos;

	for (int i = 0; i < STAGE1BOSS_EXPLOSION_NUM; i++)
	{
		pos = ptr->pos;
		pos.x += RandomRange(-STAGE1BOSS_EXPLOSION_RANGE_X, STAGE1BOSS_EXPLOSION_RANGE_X);
		pos.y += RandomRange(-STAGE1BOSS_EXPLOSION_RANGE_Y, STAGE1BOSS_EXPLOSION_RANGE_Y);
		ExplodeBigEnemy(pos);
		SetParticle(pos, BOSSFIRE);
	}

	ExplodeBigEnemy(ptr->pos);
	SetEmitterEnemyParticle(ptr->pos, BOSSEXPLOSION_EMITPARTICLE_DURATION, BOSSEXPLOSION_EMITPARTICLE_NUM, EMITTER_CIRCLE);
	AddScore(STAGE1BOSS_SCORE);
	ptr->active = false;
	ptr->state = STAGE1BOSS_STATEMAX;
	SetStageState(STAGE_CLEAR);
}

/******************************************************************************
//�֐���	�Fvoid ExplodeStage1Boss(STAGE1BOSS *ptr)
//����		�FSTAGE1BOSS *ptr	�F��������X�e�[�W1�{�X�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�X�e�[�W1�{�X��������
******************************************************************************/
void ExplodeStage1Boss(STAGE1BOSS *ptr)
{
	if (ptr->cntFrame > STAGE1BOSS_DESTROYED_EXPLFRAME)
	{
		return;
	}

	D3DXVECTOR3 pos = ptr->pos;

	pos.x += RandomRange(-STAGE1BOSS_DESTROYED_EXPLRANGE, STAGE1BOSS_DESTROYED_EXPLRANGE);
	pos.y += RandomRange(-STAGE1BOSS_DESTROYED_EXPLRANGE, STAGE1BOSS_DESTROYED_EXPLRANGE);

	SetParticle(pos, EXPLOSION);
	SetParticle(pos, BOSSFIRE);
	SetEmitterEnemyParticle(pos, BOSSFIRE_EMITOARTUCLE_DURATION, BOSSFIRE_EMITPARTICLE_NUM, EMITTER_CONE);
	PlaySE(SE_MIDDLEEXP);
}
