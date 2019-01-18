//=============================================================================
//
// �`���[�g���A���G�l�~�[���� [tutorialEnemy.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "tutorialEnemy.h"
#include "enemyManager.h"
#include "enemyBullet.h"
#include "particle.h"
#include "sePlayer.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define TUTORIALENEMY_TARGETPOS_Y			(400)		//�ڕW���WY
#define TUTORIALENEMY_MOVESPEED				(8.0f)		//�ړ��X�s�[�h

#define TUTORIALENEMY_INITPOS1_X			(450.0f)	//�`���[�g���A���G�l�~�[�̏���X���W
#define TUTORIALENEMY_INITPOS2_X			(830.0f)
#define TUTORIALENEMY_INITPOS_Y				(-100.0f)	//�`���[�g���A���G�l�~�[�̏���Y���W

#define TUTORIALENEMY_BULLETSPEED			(5.0f)		//�o���b�g�̃X�s�[�h

#define TUTORIALENEMY_SHOTPOS_R				(D3DXVECTOR3(30.0f, 50.0f, 0.0f))	//�o���b�g���ˈʒu�E
#define TUTORIALENEMY_SHOTPOS_L				(D3DXVECTOR3(-30.0f, 50.0f, 0.0f))	//�o���b�g���ˈʒu�E
#define TUTORIALENEMY_SHOTINTERBAL			(10)		//�o���b�g���ˊԊu
#define TUTORIALENEMY_INITHP				(200)		//�`���[�g���A���G�l�~�[����HP

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexTutorialEnemy(void);				//���_�쐬�֐�
void SetTextureTutorialEnemy(int cntPattern);		// �e�N�X�`�����W�̌v�Z����
void SetVertexTutorialEnemy(TUTORIALENEMY *ptr);	// ���_�̌v�Z����
void AttackTutorialEnemy(TUTORIALENEMY *ptr);		// �U������
void ResetTutorialEnemy(TUTORIALENEMY *ptr);		// �`���[�g���A���G�l�~�[���Z�b�g����

/*****************************************************************************
�\���̒�`
*****************************************************************************/
enum TUTORIALENEMY_STATE
{
	TUTORIALENEMY_MOVE,
	TUTORIALENEMY_ATTACK
};
/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N
static float angle, radius;								//�|���S���̔��a�A�p�x

TUTORIALENEMY tutorialEnemy[TUTORIALENEMY_MAX];			//�`���[�g���A���G�l�~�[�z��

//�o���b�g�p�����[�^�i�I�����W�j
static ENEMYATTACK bullet1 = {
	ORANGE_CIRCLE,
	VECTOR3_ZERO,
	TUTORIALENEMY_BULLETSPEED,
	ENEMYBULLET_DAFAULT_ANGLE
};

//�o���b�g�p�����[�^�i�O���[���j
static ENEMYATTACK bullet2 = {
	GREEN_CIRCLE,
	VECTOR3_ZERO,
	TUTORIALENEMY_BULLETSPEED,
	ENEMYBULLET_DAFAULT_ANGLE
};

/******************************************************************************
����������
******************************************************************************/
HRESULT InitTutorialEnemy(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TUTORIALENEMY* ptr = GetTutorialEnemyAdr(0);
	int i;


	angle = atan2f(TUTORIALENEMY_TEXTURE_SIZE_Y, TUTORIALENEMY_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(TUTORIALENEMY_TEXTURE_SIZE_X, TUTORIALENEMY_TEXTURE_SIZE_Y));

	for (i = 0; i < TUTORIALENEMY_MAX; i++, ptr++)
	{
		ptr->pos.x = (i == 0) ? TUTORIALENEMY_INITPOS1_X : TUTORIALENEMY_INITPOS2_X;
		ptr->pos.y = TUTORIALENEMY_INITPOS_Y;

		ptr->targetPos = D3DXVECTOR3(ptr->pos.x, TUTORIALENEMY_TARGETPOS_Y, 0.0f);

		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->state = TUTORIALENEMY_MOVE;
		ptr->id = i;
		ptr->hp = TUTORIALENEMY_INITHP;

		ptr->active = true;
	}

	// ���_���̍쐬
	MakeVertexTutorialEnemy();

	if(num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)TUTORIALENEMY_TEXTURE_NAME, pDevice);
	}
	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitTutorialEnemy(void)
{
	if (texture != NULL)
	{	// �e�N�X�`���̊J��
		texture->Release();
		texture = NULL;
	}
}

/******************************************************************************
�X�V����
******************************************************************************/
void UpdateTutorialEnemy(void)
{
	TUTORIALENEMY *ptr = &tutorialEnemy[0];

	for (int i = 0; i < TUTORIALENEMY_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//���ď���
		if (ptr->hp <= 0)
		{
			//SetParticle(ptr->pos, EXPLOSION);
			//SetParticle(ptr->pos, PIECE);
			ExplodeMiddleEnemy(ptr->pos);
			PlaySE(SE_MIDDLEEXP);
			ResetTutorialEnemy(ptr);
		}

		if (ptr->state == TUTORIALENEMY_MOVE)
		{
			bool isMoveCompleted = MoveToTarget(&ptr->pos, &ptr->targetPos, TUTORIALENEMY_MOVESPEED);
			if (isMoveCompleted)
			{
				ptr->state = TUTORIALENEMY_ATTACK;
			}
		}

		else if (ptr->state == TUTORIALENEMY_ATTACK)
		{
			ptr->countAnim++;
			if (ptr->countAnim % TUTORIALENEMY_SHOTINTERBAL == 0)
			{
				AttackTutorialEnemy(ptr);
			}
		}
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawTutorialEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TUTORIALENEMY *ptr = GetTutorialEnemyAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	//�`���[�g���A���G�l�~�[�����Z�����ŕ`��
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	for (i = 0; i < TUTORIALENEMY_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexTutorialEnemy(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureTutorialEnemy(ptr->patternAnim);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//�����蔻��̕`��
			DrawBoxCollider(ptr->pos + D3DXVECTOR3(TUTORIALENEMY_BB_LEFT, TUTORIALENEMY_BB_TOP, 0.0f), TUTORIALENEMY_BB_WIDTH, TUTORIALENEMY_BB_HIGHT);
#endif
		}
	}
	//�`����@��ʏ�̕��@�ɍĐݒ�
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexTutorialEnemy(void)
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
void SetTextureTutorialEnemy(int cntPattern)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TUTORIALENEMY_TEXTURE_DIVIDE_X;
	int y = cntPattern / TUTORIALENEMY_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / TUTORIALENEMY_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / TUTORIALENEMY_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexTutorialEnemy(TUTORIALENEMY *ptr)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = ptr->pos.x - cosf(angle + ptr->rot.z) * radius;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(angle + ptr->rot.z) * radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(angle - ptr->rot.z) * radius;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(angle - ptr->rot.z) * radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(angle - ptr->rot.z) * radius;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(angle - ptr->rot.z) * radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(angle + ptr->rot.z) * radius;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(angle + ptr->rot.z) * radius;
}

/******************************************************************************
//�֐���	�FTUTORIALENEMY *GetTutorialEnemyAdr(int pno)
//����		�Fint pno�F�擾�������`���[�g���A���G�l�~�[�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�`���[�g���A���G�l�~�[�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�`���[�g���A���G�l�~�[�̃A�h���X�擾�֐�
******************************************************************************/
TUTORIALENEMY *GetTutorialEnemyAdr(int pno)
{
	if (pno < TUTORIALENEMY_MAX)
	{
		return &tutorialEnemy[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�FTUTORIALENEMY *SetTutorialEnemy(void)
//����		�F�`���[�g���A���G�l�~�[���Z�b�g����֐�
//�߂�l	�Fvoid
//����		�F�`���[�g���A���G�l�~�[�Z�b�g�֐�
******************************************************************************/
void SetTutorialEnemy(void)
{
	TUTORIALENEMY *ptr = &tutorialEnemy[0];

	for (int i = 0; i < TUTORIALENEMY_MAX; i++, ptr++)
	{
		ResetTutorialEnemy(ptr);
	}
}

/******************************************************************************
//�֐���	�Fvoid AttackTutorialEnemy(TUTORIALENEMY *ptr)
//����		�FTUTORIALENEMY *ptr	�F�U������`���[�g���A���G�l�~�[�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�`���[�g���A���G�l�~�[�U������
******************************************************************************/
void AttackTutorialEnemy(TUTORIALENEMY *ptr)
{
	ENEMYATTACK param = (ptr->id % 2 == 0) ? bullet1 : bullet2;

	//�E�̔��ˈʒu����U��
	param.pos = ptr->pos + TUTORIALENEMY_SHOTPOS_R;
	SetEnemyBullet(&param, &ptr->hp);

	//���̔��ˈʒu����U��
	param.pos = ptr->pos + TUTORIALENEMY_SHOTPOS_L;
	SetEnemyBullet(&param, &ptr->hp);
}

/******************************************************************************
//�֐���	�Fvoid ResetTutorialEnemy(TUTORIALENEMY *ptr)
//����		�FTUTORIALENEMY *ptr	�F�U������`���[�g���A���G�l�~�[�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�`���[�g���A���G�l�~�[���Z�b�g����
******************************************************************************/
void ResetTutorialEnemy(TUTORIALENEMY *ptr)
{
	ptr->pos.x = (ptr->id % 2 == 0) ? TUTORIALENEMY_INITPOS1_X : TUTORIALENEMY_INITPOS2_X;
	ptr->pos.y = TUTORIALENEMY_INITPOS_Y;

	ptr->countAnim = 0;
	ptr->patternAnim = 0;

	ptr->state = TUTORIALENEMY_MOVE;
	ptr->hp = TUTORIALENEMY_INITHP;
	ptr->state = TUTORIALENEMY_MOVE;

	ptr->active = true;
}

/******************************************************************************
//�֐���	�Fvoid DisableTutorialEnemy(void)
//����		�Fvoid
//�߂�l	�Fvoid
//����		�F���ׂẴ`���[�g���A���G�l�~�[���A�N�e�B�u�ɂ���֐�
******************************************************************************/
void DisableTutorialEnemy(void)
{
	TUTORIALENEMY *ptr = &tutorialEnemy[0];

	for (int i = 0; i < TUTORIALENEMY_MAX; i++, ptr++)
	{
		ptr->active = false;
	}
}