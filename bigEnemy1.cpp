//=============================================================================
//
// �r�b�O�G�l�~�[1���� [bigEnemy1.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "enemyManager.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define BIGENEMY1_SPEED				(8.0f)					//�r�b�O�G�l�~�[1�̈ړ��X�s�[�h		
#define BIGENEMY1_ESCAPESPEED		(3.0f)					//�r�b�O�G�l�~�[1�̓����X�s�[�h
#define BIGENEMY1_ATTACKINTERBAL	(90)					//�r�b�O�G�l�~�[1�̍U���Ԋu

#define BIGENEMY1_ATTACK_ANGLE		(7.0f)					//���˂���WAY�e��WAY�̌���
#define BIGENEMY1_ATTACK_WAYEASY	(4)						//���˂���WAY�e��WAY��(EASY)
#define BIGENEMY1_ATTACK_WAYNORMAL	(6)						//���˂���WAY�e��WAY��(NORMAL)
#define BIGENEMY1_ATTACK_WAYHARD	(10)					//���˂���WAY�e��WAY��(HARD)
#define BIGENEMY1_ROTATEANGLE		(20.0f)					//���ˈʒu����]������p�x
#define BIGENEMY1_BULLET_SPEED		(4.5f)					//�o���b�g�̃X�s�[�h
#define BIGENEMY1_ATTACK_OFFSETPOS	(-60.0f)				//�o���b�g���ˈʒu�̃I�t�Z�b�g���W�iY)

#define BIGENEMY1_WAITFRAME1		(30)					//�U���J�n�܂ł̑ҋ@�t���[��
#define BIGENEMY1_ATTACKFRAME		(BIGENEMY1_ATTACKINTERBAL*6)	//�U�����I���܂ł̃t���[����
#define BIGENEMY1_WAITFRAME2		(240)					//�U����̈ړ��J�n�܂ł̑ҋ@�t���[��

#define BIGENEMY1_INITHP			(900)					//HP�����l
#define BIGENEMY1_SCORE				(5000)					//���Ď��ɉ��Z�X�R�A

#define BIGENEMY1_GAUGE_OFFSETPOS	(D3DXVECTOR3(0.0f, -100.0f, 0.0f))	//HP�Q�[�W��\������I�t�Z�b�g���W
#define BIGENEMY1_SHADOW_OFFSETPOS	(D3DXVECTOR3(100.0f, 200.0f, 0.0f))	//�e��\������I�t�Z�b�g���W

#define BIGENEMY1_CIRCLESHOT_WAY	(18)					//�~�`�e��WAY��
#define BIGENEMY1_CIRCLESHOT_SPEED	(6.0f)					//�~�`�e�̃X�s�[�h
#define BIGENEMY1_CIRCLESHOT_DELTA	(0.5f)					//�~�`�e�̃X�s�[�h������
#define BIGENEMY1_CIRCLESHOT_LOOPMAX	(5)					//�~�`�e�̃��[�v��
/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexBigEnemy1(void);							//���_�쐬�֐�
void SetTextureBigEnemy1(int cntPattern, BIGENEMY1 *ptr);	// �e�N�X�`�����W�̌v�Z����
void SetVertexBigEnemy1(BIGENEMY1 *ptr);					// ���_�̌v�Z����
void SetShadowBigEnemy1(BIGENEMY1 *ptr);					//�e�̒��_�ݒ菈��
void BigEnemy1Action(BIGENEMY1 *ptr);						//�s������
void BigEnemy1Attack(BIGENEMY1 *ptr);						//�U������

/*****************************************************************************
�\���̒�`
*****************************************************************************/
enum
{
	BIGENEMY1_MOVE,
	BIGENEMY1_WAIT1,
	BIGENEMY1_ATTACK,
	BIGENEMY1_WAIT2,
	BIGENEMY1_ESCAPE
};

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture[BIGENEMY1_TEXTURE_NUM];				// �e�N�X�`���ւ̃|�C���^

BIGENEMY1 bigEnemy1[BIGENEMY1_MAX];							//�r�b�O�G�l�~�[1�z��
static int atkWay;											//�o���b�g��WAY��
static VERTEX_2D		vertexWk[NUM_VERTEX];				//���_���i�[���[�N

static ENEMYATTACK bulletParam1 =							//�U���p�p�����[�^1
{
	GREEN_ROTATE,
	VECTOR3_ZERO,
	BIGENEMY1_BULLET_SPEED,
	ENEMYBULLET_DAFAULT_ANGLE
};

static ENEMYATTACK bulletParam2 =							//�U���p�p�����[�^2
{
	ORANGE_ROTATE,
	VECTOR3_ZERO,
	BIGENEMY1_BULLET_SPEED,
	ENEMYBULLET_DAFAULT_ANGLE + RADIAN(BIGENEMY1_ROTATEANGLE / 2)
};

static ENEMYATTACK circleParam =
{
	ORANGE_WHEEL,
	VECTOR3_ZERO,
	BIGENEMY1_BULLET_SPEED,
	ENEMYBULLET_DAFAULT_ANGLE
};

/******************************************************************************
����������
******************************************************************************/
HRESULT InitBigEnemy1(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BIGENEMY1* ptr = GetBigEnemy1Adr(0);
	int i;

	//��Փx�ɂ����WAY���𕪊�
	int difficulty = GetDifficulty();
	atkWay = (difficulty == DIFFICULTY_EASY) ? BIGENEMY1_ATTACK_WAYEASY : (difficulty == DIFFICULTY_NORMAL) ? BIGENEMY1_ATTACK_WAYNORMAL : BIGENEMY1_ATTACK_WAYHARD;

	for (i = 0; i < BIGENEMY1_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(BIGENEMY1_INITPOS_X, BIGENEMY1_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(BIGENEMY1_TEXTURE_SIZE_Y, BIGENEMY1_TEXTURE_SIZE_X);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(BIGENEMY1_TEXTURE_SIZE_X, BIGENEMY1_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->targetPos = VECTOR3_ZERO;

		ptr->state = BIGENEMY1_MOVE;

		ptr->hp = BIGENEMY1_INITHP;

		ptr->gauge = NULL;

		ptr->active = false;
	}

	// ���_���̍쐬
	MakeVertexBigEnemy1();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[0] = CreateTextureFromFile((LPSTR)BIGENEMY1_SHADOWTEX_NAME, pDevice);
		texture[1] = CreateTextureFromFile((LPSTR)BIGENEMY1_TEXTURE_NAME, pDevice);
	}
	//SetBigEnemy(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(SCREEN_CENTER_X, 100.0f, 0.0));
	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitBigEnemy1(void)
{
	for (int i = 0; i < BIGENEMY1_TEXTURE_NUM; i++)
	{
		if (texture != NULL)
		{	// �e�N�X�`���̊J��
			texture[i]->Release();
			texture[i] = NULL;
		}
	}

}

/******************************************************************************
�X�V����
******************************************************************************/
void UpdateBigEnemy1(void)
{
	BIGENEMY1 *ptr = GetBigEnemy1Adr(0);
	ENEMYATTACK param1 = bulletParam1, param2 = bulletParam2;
	int i;

	for (i = 0; i < BIGENEMY1_MAX; i++, ptr++)
	{
		//��A�N�e�B�u�Ȃ珈�����s�킸���̃G�l�~�[��
		if (!ptr->active)
		{
			continue;
		}

		//���Ĕ���
		if (ptr->hp <= 0)
		{
			//���ď���
			ptr->active = false;
			ExplodeBigEnemy(ptr->pos);
			ConvertBulletToScoreItem();
			AddScore(BIGENEMY1_SCORE);

			//�Q�[�W�������
			ptr->gauge->active = false;
			ptr->gauge = NULL;

			continue;
		}

		//�A�j���[�V����
		ptr->countAnim++;
		if (ptr->countAnim % TIMEBIGENEMY1_ANIMATION == 0)
		{
			ptr->patternAnim = (ptr->patternAnim + 1) % ANIM_BIGENEMY1_NUM;
		}

		//�s������
		BigEnemy1Action(ptr);

		//�Q�[�W�X�V����
		ptr->gauge->pos = ptr->pos + BIGENEMY1_GAUGE_OFFSETPOS;
		ptr->gauge->scale = (float)ptr->hp / BIGENEMY1_INITHP;
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawBigEnemy1(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BIGENEMY1 *ptr = GetBigEnemy1Adr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture[0]);

	//�e��`��
	for (i = 0; i < BIGENEMY1_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_��ݒ�
			SetShadowBigEnemy1(ptr);
			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}

	ptr = GetBigEnemy1Adr(0);
	//�r�b�O�G�l�~�[1��`��
	pDevice->SetTexture(0, texture[1]);
	for (i = 0; i < BIGENEMY1_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexBigEnemy1(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureBigEnemy1(ptr->patternAnim, ptr);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//�����蔻��̕`��
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(BIGENEMY1_BB_LEFT, BIGENEMY1_BB_TOP, 0.0f), BIGENEMY1_BB_WIDTH, BIGENEMY1_BB_HIGHT);
#endif
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexBigEnemy1(void)
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
void SetTextureBigEnemy1(int cntPattern, BIGENEMY1 *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % BIGENEMY1_TEXTURE_DIVIDE_X;
	int y = cntPattern / BIGENEMY1_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / BIGENEMY1_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / BIGENEMY1_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexBigEnemy1(BIGENEMY1 *ptr)
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
�e�p�̒��_�ݒ�֐�
******************************************************************************/
void SetShadowBigEnemy1(BIGENEMY1 *ptr)
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

	//�e�p�ɒ��_���W�����炷
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertexWk[i].vtx += BIGENEMY1_SHADOW_OFFSETPOS;
	}

	//�e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}
/******************************************************************************
//�֐���	�FBIGENEMY1 *GetBigEnemy1Adr(int pno)
//����		�Fint pno�F�擾�������r�b�O�G�l�~�[1�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�r�b�O�G�l�~�[1�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�r�b�O�G�l�~�[1�̃A�h���X�擾�֐�
******************************************************************************/
BIGENEMY1 *GetBigEnemy1Adr(int pno)
{
	if (pno < BIGENEMY1_MAX)
	{
		return &bigEnemy1[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�Fvoid BigEnemy1Action(BIGENEMY1 *ptr)
//����		�FBIGENEMY1 *ptr		�F�s������r�b�O�G�l�~�[1�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�r�b�O�G�l�~�[1�̍s������
******************************************************************************/
void BigEnemy1Action(BIGENEMY1 *ptr)
{
	bool flgMove = false;

	switch (ptr->state)
	{
		//�ڕW���W�܂ňړ�
	case BIGENEMY1_MOVE:
		flgMove = MoveToTarget(&ptr->pos, &ptr->targetPos, BIGENEMY1_SPEED);
		//�ڕW���W�֓��B���Ă����玟�̏�Ԃ֑J��
		if (flgMove)
		{
			ptr->cntFrame = 0;
			ptr->state++;
		}
		break;

		//�U���J�n�܂ł̑ҋ@
	case BIGENEMY1_WAIT1:
		ptr->cntFrame++;
		if (ptr->cntFrame >= BIGENEMY1_WAITFRAME1)
		{
			ptr->cntFrame = 0;
			BigEnemy1Attack(ptr);
			ptr->state++;
		}
		break;

		//�U��
	case BIGENEMY1_ATTACK:
		ptr->cntFrame++;
		//���Ԋu�ōU��
		if (ptr->cntFrame % BIGENEMY1_ATTACKINTERBAL == 0)
		{
			BigEnemy1Attack(ptr);
		}

		//���t���[���o�߂����玟�̏�Ԃ֑J��
		if (ptr->cntFrame >= BIGENEMY1_ATTACKFRAME)
		{
			ptr->cntFrame = 0;
			ptr->state++;
		}
		break;

		//�U����̑ҋ@
	case BIGENEMY1_WAIT2:
		ptr->cntFrame++;
		if (ptr->cntFrame >= BIGENEMY1_WAITFRAME2)
		{
			ptr->cntFrame = 0;
			ptr->state++;
		}
		break;

		//��ʉ��֏��X�Ɉړ�
	case BIGENEMY1_ESCAPE:
		ptr->cntFrame++;
		StartAccel(&ptr->pos, &(VECTOR3_UP * -1), BIGENEMY1_ESCAPESPEED, 60, ptr->cntFrame);

		//��ʊO�֏o�����A�N�e�B�u��
		if (ptr->pos.y > SCREEN_HEIGHT + BIGENEMY1_TEXTURE_SIZE_Y)
		{
			ptr->active = false;
		}
		break;
	}
}

/******************************************************************************
//�֐���	�Fvoid BigEnemy1Attack(void)
//����		�FBIGENEMY1 *ptr	�F�U���������Ȃ�r�b�O�G�l�~�[1�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�r�b�O�G�l�~�[1�̍U��
******************************************************************************/
void BigEnemy1Attack(BIGENEMY1 *ptr)
{
	ENEMYATTACK param1 = bulletParam1, param2 = bulletParam2;
	float angle;

	//���˒n�_�����ݍ��W�ɐݒ�
	param1.pos = ptr->pos;
	param2.pos = ptr->pos;
	param1.pos.y += BIGENEMY1_ATTACK_OFFSETPOS;
	param2.pos.y += BIGENEMY1_ATTACK_OFFSETPOS;

	//���ˊp�x���������܂Ń��[�v
	for (angle = 0.0f; angle < 360.0f; angle += BIGENEMY1_ROTATEANGLE)
	{
		//�o���b�g�𔭎�
		FireWayBullet(&param1, atkWay, (float)atkWay - 1, &ptr->hp);
		FireWayBullet(&param2, atkWay, (float)atkWay - 1, &ptr->hp);

		//���ˊp�x����]
		param1.angle += RADIAN(BIGENEMY1_ROTATEANGLE);
		param2.angle += RADIAN(BIGENEMY1_ROTATEANGLE);
	}

	//��ՓxHARD�Ȃ�~�`�e�ǉ�
	if (GetDifficulty() == DIFFICULTY_HARD)
	{
		ENEMYATTACK param3 = circleParam;
		PLAYER *player = GetPlayerAdr(0);
		
		param3.pos = ptr->pos;
		param3.pos.y += BIGENEMY1_ATTACK_OFFSETPOS;
		param3.angle = SnipeTarget(&param3.pos, &player->pos);

		for (int i = 0; i < BIGENEMY1_CIRCLESHOT_LOOPMAX; i++)
		{
			FireCircleBullet(&param3, BIGENEMY1_CIRCLESHOT_WAY, &ptr->hp);
			param3.speed += BIGENEMY1_CIRCLESHOT_DELTA;
		}
	}
}

/******************************************************************************
//�֐���	�Fvoid SetBigEnemy(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos)
//����1		�FD3DXVECTOR3 initPos		�F�o�����W
//����2		�FD3DXVECTOR3 targetPos		�F�ڕW���W
//�߂�l	�Fvoid
//����		�F�r�b�O�G�l�~�[1���Q�[�����ɏo��������֐�
******************************************************************************/
void SetBigEnemy(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos)
{
	int i = 0;
	BIGENEMY1 *ptr = GetBigEnemy1Adr(0);

	for (i = 0; i < BIGENEMY1_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			//�e���W��ݒ�
			ptr->pos = initPos;
			ptr->targetPos = targetPos;

			//�e�p�����[�^��������
			ptr->gauge = GetNonActiveEnemyGauge();
			ptr->hp = BIGENEMY1_INITHP;
			ptr->state = BIGENEMY1_MOVE;
			ptr->patternAnim = 0;
			ptr->countAnim = 0;

			//�G�l�~�[���A�N�e�B�u�ɃZ�b�g�����^�[��
			ptr->state = BIGENEMY1_MOVE;
			ptr->active = true;
			return;
		}
	}
}