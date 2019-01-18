//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "particle.h"
#include "effectExplosion.h"
#include "effectPiece.h"
#include "effectFire.h"
#include "input.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define PARTICLE_FIRE_EMITTFRAME			(12)			//�t�@�C�A�p�[�e�B�N���̕��o����
#define PARTICLE_PIECE_EMITTFRAME			(6)				//�s�[�X�p�[�e�B�N���̕��o����
#define PARTICLE_EXPLOSION_EMITTFRAME		(12)			//�����p�[�e�B�N���̕��o����
#define PARTICLE_BOSSFIRE_EMITTFRAME		(6)				//�{�X�����p�[�e�B�N���̕��o����

#define PARTICLE_FIRE_EMITTNUM				(3)				//�t�@�C�A�p�[�e�B�N���̕��o��
#define PARTICLE_PIECE_EMITTNUM				(6)				//�s�[�X�p�[�e�B�N���̕��o��
#define PARTICLE_EXPLOSION_EMITTNUM			(2)				//�����p�[�e�B�N���̕��o��
#define PARTICLE_BOSSFIRE_EMITTNUM			(12)			//�{�X�����p�[�e�B�N���̕��o��

#define PARTICLE_BOSSFIRE_INITSPEED			(4.0f)			//�{�X�t�@�C�A�̏����X�s�[�h
#define PARTICLE_BOSSFIRE_SPEEDOFFSET		(3.0f)			//�{�X�t�@�C�A�̃X�s�[�h����

#define PARTICLE_PIECE_ANGLEMIN				(30.0f)			//�s�[�X�p�[�e�B�N���̊p�x�ŏ��l
#define PARTICLE_PIECE_ANGLEMAX				(150.0f)		//�s�[�X�p�[�e�B�N���̊p�x�ő�l

#define PARTICLE_FIRE_ANGLEMIN				(0.0f)			//�t�@�C�A�p�[�e�B�N���̊p�x�ŏ��l
#define PARTICLE_FIRE_ANGLEMAX				(360.0f)		//�t�@�C�A�p�[�e�B�N���̊p�x�ő�l

#define PARTICLE_EXPLOSION_ANGLEMIN			(0.0f)			//�G�N�X�v���[�W�����p�[�e�B�N���̊p�x�ŏ��l
#define PARTICLE_EXPLOSION_ANGLEMAX			(360.0f)		//�G�N�X�v���[�W�����p�[�e�B�N���̊p�x�ő�l

//#define PARTICLE_FIRE_PARTICLENUM			(10)			//�t�@�C�A�p�[�e�B�N���̃p�[�e�B�N����
//#define PARTICEL_PIECE_PARTICLENUM			(40)			//�s�[�X�p�[�e�B�N���̃p�[�e�B�N����
//#define PARTICLE_EXPLOSION_PARTICLENUM		(30)			//�����p�[�e�B�N���̃p�[�e�B�N����

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/

/*****************************************************************************
�\���̒�`
****************************************************************************/

/*****************************************************************************
�O���[�o���ϐ�
****************************************************************************/
static PARTICLE particle[KIND_MAX][PARTICLE_MAX];			//�p�[�e�B�N���\���̔z��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitParticle(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PARTICLE *ptr = GetParticleAdr(0, 0);
	int i;

	for (i = 0; i < KIND_MAX*PARTICLE_MAX; i++, ptr++)
	{
		ptr->pos = VECTOR3_ZERO;
		ptr->cntFrame = 0;
		ptr->active = false;
	}
	
	InitEffect(num);

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitParticle(void)
{
	UninitEffect();
}

/******************************************************************************
�X�V����
******************************************************************************/
void UpdateParticle(void)
{
	PARTICLE *ptr = GetParticleAdr(0, 0);
	int i, j, k;
	D3DXVECTOR3 dir = VECTOR3_ZERO;
	float tmp;

	/*//�m�F�p�f�o�b�O�@�\
	if (GetKeyboardTrigger(DIK_SPACE))
	{
		//SetParticle(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), EXPLOSION);
		//SetParticle(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), PIECE);
		//SetParticle(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), FIRE);
		SetParticle(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), BOSSFIRE);
	}
	*/

	for (i = 0; i < KIND_MAX; i++)
	{
		for (j = 0; j < PARTICLE_MAX; j++, ptr++)
		{
			//��A�N�e�B�u�̃p�[�e�B�N���Ȃ珈�����s��Ȃ�
			if (!ptr->active)
			{
				continue;
			}

			//�p�[�e�B�N���̎�ނɂ���ď����𕪊�
			switch (i)
			{
			case FIRE:
				for (k = 0; k < PARTICLE_FIRE_EMITTNUM; k++)
				{
					tmp = RandomRange(PARTICLE_FIRE_ANGLEMIN, PARTICLE_FIRE_ANGLEMAX);
					dir.x = cosf(RADIAN(tmp));
					dir.y = sinf(RADIAN(tmp));
					SetEffectFire(ptr->pos, dir);

					ptr->cntFrame++;
					if (ptr->cntFrame >= PARTICLE_FIRE_EMITTFRAME)
					{
						ptr->active = false;
					}
				}
				break;

			case PIECE:
				for (k = 0; k < PARTICLE_PIECE_EMITTNUM; k++)
				{
					tmp = RandomRange(PARTICLE_PIECE_ANGLEMIN, PARTICLE_PIECE_ANGLEMAX);
					dir.x = cosf(RADIAN(tmp));
					dir.y = sinf(RADIAN(tmp));
					SetEffectPiece(ptr->pos, dir);
				}

				ptr->cntFrame++;
				if (ptr->cntFrame >= PARTICLE_PIECE_EMITTFRAME)
				{
					ptr->active = false;
				}
				break;

			case EXPLOSION:
				for (k = 0; k < PARTICLE_EXPLOSION_EMITTNUM; k++)
				{
					tmp = RandomRange(PARTICLE_EXPLOSION_ANGLEMIN, PARTICLE_EXPLOSION_ANGLEMAX);
					dir.x = cosf(RADIAN(tmp));
					dir.y = sinf(RADIAN(tmp));
					SetEffectExplosion(ptr->pos, dir);
				}

				ptr->cntFrame++;
				if (ptr->cntFrame >= PARTICLE_EXPLOSION_EMITTFRAME)
				{
					ptr->active = false;
				}
				break;

			case BOSSFIRE:
				tmp = RandomRange(PARTICLE_FIRE_ANGLEMIN, PARTICLE_FIRE_ANGLEMAX);
				dir.x = cosf(RADIAN(tmp));
				dir.y = sinf(RADIAN(tmp));
				for (k = 0; k < PARTICLE_BOSSFIRE_EMITTNUM; k++)
				{
					SetBossFire(ptr->pos, dir, PARTICLE_BOSSFIRE_INITSPEED + k * PARTICLE_BOSSFIRE_SPEEDOFFSET);
				}

				ptr->cntFrame++;
				if (ptr->cntFrame >= PARTICLE_BOSSFIRE_EMITTFRAME)
				{
					ptr->active = false;
				}
				break;
			}
		}
	}

	UpdateEffect();
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	DrawEffect();
}

/******************************************************************************
//�֐���	�FPARTICLE *GetParticleAdr(int type, int pno)
//����1		�Fint type	�F�擾�������p�[�e�B�N���̎��
//����2		�Fint pno	�F�擾�������p�[�e�B�N���̃C���f�b�N�X
//�߂�l	�Fvoid
//����		�F���^�����̃A�h���X�擾�֐�
******************************************************************************/
PARTICLE *GetParticleAdr(int type, int pno)
{
	return &particle[type][pno];
}

/******************************************************************************
//�֐���	�Fvoid FireParticle(D3DXVECTOR3 pos)
//����1		�FD3DXVECTOR3 pos	�F�Z�b�g���������W
//����2		�Fint type			�F�Z�b�g�������p�[�e�B�N���̃^�C�v
//�߂�l	�Fvoid
//����		�F�p�[�e�B�N�����Q�[�����ɃZ�b�g����֐�
******************************************************************************/
void SetParticle(D3DXVECTOR3 pos, int type)
{
	PARTICLE *ptr = GetParticleAdr(type, 0);
	int i;

	for (i = 0; i < PARTICLE_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			ptr->pos = pos;
			ptr->cntFrame = 0;
			ptr->active = true;
			return;
		}
	}
}