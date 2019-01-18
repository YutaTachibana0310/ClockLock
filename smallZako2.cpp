//=============================================================================
//
// �X���[���U�R2���� [smallZako1.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "enemyManager.h"
#include "player.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define SMALLZAKO2_INITHP					(10)				//HP�����l
#define SMALLZAKO2_SCORE					(100)				//���Ď��̃X�R�A
#define SMALLZAKO2_INITSPEED				(10.0f)				//�X�s�[�h�����l

#define SMALLZAKO2_INIT_FRAME				(30)				//INIT��Ԃ��玟�֑J�ڂ��鎞��
#define SMALLZAKO2_INIT_RADIAN(f)			(RADIAN((f/2.0f)*180/SMALLZAKO2_INIT_FRAME))	//INIT��ԂŌ������邽�߂̌W��

#define SMALLZAKO2_WAIT_FRAME				(30)				//WAIT��Ԃ��玟�֑J�ڂ��鎞��

#define SMALLZAKO2_ATTACK_VALUE				(0.5f)				//�o���b�g�̑��x�̉��Z�l
#define SMALLZAKO2_ATTACK_NUMEASY			(1)					//��x�Ɍ��o���b�g�̐�
#define SMALLZAKO2_ATTACK_NUMNORMAL			(3)					//��x�Ɍ��o���b�g�̐�
#define SMALLZAKO2_ATTACK_NUMHARD			(6)					//��x�Ɍ��o���b�g�̐�
#define SMALLZAKO2_ATTACK_MAX				(3)					//�U���ő��
#define SMALLZAKO2_BULLET_SPEED				(4.5f)				//�o���b�g�̃X�s�[�h

#define SMALLZAKO2_INTERBAL_FRAME			(120)				//INTERBAL��Ԃ���J�ڂ��鎞��

#define SMALLZAKO2_HOMING_ANGLE				(RADIAN(2.0f))		//�ǔ����̍ŏ���]�p�x

#define SMALLZAKO2_SHADOW_OFFSETPOS			(D3DXVECTOR3(50.0f, 100.0f, 0.0f)) //�e�̃I�t�Z�b�g���W

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexSmallZako2(void);							//���_�쐬�֐�
void SetTextureSmallZako2(int cntPattern, SMALLZAKO2 *ptr);	// �e�N�X�`�����W�̌v�Z����
void SetVertexSmallZako2(SMALLZAKO2 *ptr);					// ���_�̌v�Z����
void SetShadowSmallZako2(SMALLZAKO2 *ptr);					//�e�p�̐ݒ菈��
void ActionSmallZako2(SMALLZAKO2 *ptr);						//�X���[���U�R2�̍s������
void AttackSmallZako2(SMALLZAKO2 *ptr);						//�X���[���U�R2�̍U������

/*****************************************************************************
�\���̒�`
*****************************************************************************/
//�e�N�X�`���ԍ��̗񋓑�
enum
{
	SHADOW,
	BODY
};

//�s����ԗ񋓑�
enum
{
	INIT,
	WAIT,
	ATTACK,
	INTERBAL,
	ESCAPE
};

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[SMALLZAKO2_TEXTURE_NUM];		// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];							//���_���i�[���[�N

static float angle;												//�|���S���̔��a
static float radius;											//�|���S���̒��_�p�x
static int	atkNum;												//�U����

//�U�����̃o���b�g�p�����[�^
static ENEMYATTACK bulletParam =
{
	GREEN_ROTATE,
	VECTOR3_ZERO,
	SMALLZAKO2_BULLET_SPEED,
	0.0f
};

SMALLZAKO2 smallZako1[SMALLZAKO2_MAX];							//�X���[���U�R2�z��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitSmallZako2(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SMALLZAKO2* ptr = GetSmallZako2Adr(0);
	int i;

	angle = atan2f(SMALLZAKO2_TEXTURE_SIZE_Y, SMALLZAKO2_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(SMALLZAKO2_TEXTURE_SIZE_X, SMALLZAKO2_TEXTURE_SIZE_Y));

	//��Փx�ɂ���čU���񐔂𕪊�
	int difficulty = GetDifficulty();
	atkNum = (difficulty == DIFFICULTY_EASY) ? SMALLZAKO2_ATTACK_NUMEASY : (difficulty == DIFFICULTY_NORMAL) ? SMALLZAKO2_ATTACK_NUMNORMAL : SMALLZAKO2_ATTACK_NUMHARD;

	for (i = 0; i < SMALLZAKO2_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(SMALLZAKO2_INITPOS_X, SMALLZAKO2_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->hp = SMALLZAKO2_INITHP;
		ptr->state = INIT;
		ptr->speed = SMALLZAKO2_INITSPEED;
		ptr->moveDir = VECTOR3_UP * -1;
		ptr->cntFrame = 0;

		ptr->active = false;
	}

	// ���_���̍쐬
	MakeVertexSmallZako2();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[SHADOW] = CreateTextureFromFile((LPSTR)SMALLZAKO2_SHADOWTEX_NAME, pDevice);
		texture[BODY] = CreateTextureFromFile((LPSTR)SMALLZAKO2_TEXTURE_NAME, pDevice);
	}
	//SetSmallZako2(D3DXVECTOR3(BG_LEFT + 200.0f, 0.0f, 0.0f));
	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitSmallZako2(void)
{
	for (int i = 0; i < SMALLZAKO2_TEXTURE_NUM; i++)
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
void UpdateSmallZako2(void)
{
	int i;
	SMALLZAKO2 *ptr = GetSmallZako2Adr(0);
	PLAYER *player = GetPlayerAdr(0);

	for (i = 0; i < SMALLZAKO2_MAX; i++, ptr++)
	{
		//��A�N�e�B�u�ł���Ώ������s�킸����
		if (!ptr->active)
		{
			continue;
		}

		//���ď���
		if (ptr->hp <= 0)
		{
			ptr->active = false;
			ExplodeSmallEnemy(ptr->pos);
			AddScore(SMALLZAKO2_SCORE);
			continue;
		}

		//�A�j���[�V����
		ptr->countAnim++;
		if (ptr->countAnim % SMALLZAKO2_TIME_ANIMATION == 0)
		{
			ptr->patternAnim = (ptr->patternAnim + 1) % SMALLZAKO2_ANIM_NUM;
		}

		//�v���C���[�̕��֌���
		ptr->rot.z += RotateToTarget(&ptr->pos, &player->pos, ptr->rot.z, SMALLZAKO2_HOMING_ANGLE);

		//�s������
		ActionSmallZako2(ptr);
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawSmallZako2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SMALLZAKO2 *ptr = GetSmallZako2Adr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e��`��
	pDevice->SetTexture(0, texture[SHADOW]);
	for (i = 0; i < SMALLZAKO2_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W�A�e�N�X�`�����W��ݒ�
			SetShadowSmallZako2(ptr);

			//�|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}

	//�{�̂�`��
	pDevice->SetTexture(0, texture[BODY]);
	ptr = GetSmallZako2Adr(0);
	for (i = 0; i < SMALLZAKO2_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexSmallZako2(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureSmallZako2(ptr->patternAnim, ptr);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//�����蔻��̕`��
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(SMALLZAKO2_BB_LEFT, SMALLZAKO2_BB_TOP, 0.0f), SMALLZAKO2_BB_WIDTH, SMALLZAKO2_BB_HIGHT);
#endif
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexSmallZako2(void)
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
void SetTextureSmallZako2(int cntPattern, SMALLZAKO2 *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % SMALLZAKO2_TEXTURE_DIVIDE_X;
	int y = cntPattern / SMALLZAKO2_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / SMALLZAKO2_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / SMALLZAKO2_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexSmallZako2(SMALLZAKO2 *ptr)
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
�e�p�̒��_���W�A�e�N�X�`�����W�̐ݒ�
******************************************************************************/
void SetShadowSmallZako2(SMALLZAKO2 *ptr)
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

	//�e�p�ɃI�t�Z�b�g
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertexWk[i].vtx += SMALLZAKO2_SHADOW_OFFSETPOS;
	}

	//�e�N�X�`�����W��ݒ�
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
//�֐���	�FSMALLZAKO2 *GetSmallZako2Adr(int pno)
//����		�Fint pno�F�擾�������X���[���U�R2�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�X���[���U�R2�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�X���[���U�R2�̃A�h���X�擾�֐�
******************************************************************************/
SMALLZAKO2 *GetSmallZako2Adr(int pno)
{
	if (pno < SMALLZAKO2_MAX)
	{
		return &smallZako1[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�Fvoid SetSmallZako2(D3DXVECTOR3 initPos)
//����		�FD3DXVECTOR3 initPos	�F�o�����W
//�߂�l	�Fvoid
//����		�F�X���[���U�R2�̃Z�b�g�֐�
******************************************************************************/
void SetSmallZako2(D3DXVECTOR3 initPos)
{
	int i;
	SMALLZAKO2 *ptr = GetSmallZako2Adr(0);

	for (i = 0; i < SMALLZAKO2_MAX; i++, ptr++)
	{
		//��A�N�e�B�u�̃X���[���U�R2������΃Z�b�g
		if (!ptr->active)
		{
			//�e�p�����[�^��ݒ�
			ptr->pos = initPos;
			ptr->rot.z = 0.0f;
			ptr->hp = SMALLZAKO2_INITHP;
			ptr->moveDir = VECTOR3_UP * -1;
			ptr->speed = SMALLZAKO2_INITSPEED;
			ptr->countAnim = 0;
			ptr->patternAnim = 0;
			ptr->cntFrame = 0;
			ptr->cntAttack = 0;
			ptr->state = INIT;

			//�Z�b�g�����^�[��
			ptr->active = true;
			return;
		}
	}
}

/******************************************************************************
//�֐���	�Fvoid ActionSmallZako2(SMALLZAKO2 *ptr)
//����		�FSMALLZAKO21 *ptr	�F�s������G�l�~�[�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�X���[���U�R2�̍s������
******************************************************************************/
void ActionSmallZako2(SMALLZAKO2 *ptr)
{
	PLAYER *player = GetPlayerAdr(0);

	ptr->cntFrame++;

	switch (ptr->state)
	{
	case INIT:
		//�ڕW�����֌������Č����ړ�
		ptr->speed = cosf(SMALLZAKO2_INIT_RADIAN(ptr->cntFrame)) * SMALLZAKO2_INITSPEED;
		ptr->pos += ptr->moveDir * ptr->speed;
		//��莞�Ԍo�߂��Ă����玟�̏�Ԃ֑J��
		if (ptr->cntFrame >= SMALLZAKO2_INIT_FRAME)
		{
			ptr->cntFrame = 0;
			ptr->state = WAIT;
		}
		break;

	case WAIT:
		//���̏�ň�莞�ԑҋ@
		if (ptr->cntFrame >= SMALLZAKO2_WAIT_FRAME)
		{
			ptr->cntFrame = 0;
			ptr->state = ATTACK;
		}
		break;

	case ATTACK:
		//�U������ԑJ��
		AttackSmallZako2(ptr);
		ptr->cntFrame = 0;
		ptr->state = INTERBAL;
		break;

	case INTERBAL:
		//��莞�ԑҋ@
		if (ptr->cntFrame >= SMALLZAKO2_INTERBAL_FRAME)
		{
			//�U���񐔂��ő�ł����ESCAPE��Ԃ֑J��
			if (ptr->cntAttack >= SMALLZAKO2_ATTACK_MAX)
			{
				ptr->cntFrame = 0;
				ptr->state = ESCAPE;
			}
			//�ĂэU����Ԃ֑J��
			else
			{
				ptr->cntFrame = 0;
				ptr->state = ATTACK;
			}
		}
		break;

	case ESCAPE:
		//��ʏ㕔�֓���
		StartAccel(&ptr->pos, &VECTOR3_UP, SMALLZAKO2_INITSPEED, SMALLZAKO2_INIT_FRAME, ptr->cntFrame);

		//��ʊO�ɏo�Ă���Δ�A�N�e�B�u��
		if (ptr->pos.y + SMALLZAKO2_TEXTURE_SIZE_Y < 0.0f)
		{
			ptr->active = false;
		}
		break;
	}
}

/******************************************************************************
//�֐���	�Fvoid AttackSmallZako2(SMALLZAKO2 *ptr)
//����		�FSMALLZAKO2 *ptr	�F�U������X���[���U�R2�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�X���[���U�R2�̍U������
******************************************************************************/
void AttackSmallZako2(SMALLZAKO2 *ptr)
{
	ptr->cntAttack++;

	//��ՓxEASY�Ȃ�U�����Ȃ�
	if (GetDifficulty() == DIFFICULTY_EASY)
	{
		return;
	}

	ENEMYATTACK param = bulletParam;

	//�e�p�����[�^��ݒ�
	param.angle = ptr->rot.z + RADIAN(90);
	param.pos = ptr->pos;

	//�U��
	for (int i = 0; i < atkNum; i++)
	{
		SetEnemyBullet(&param, &ptr->hp);
		param.speed += SMALLZAKO2_ATTACK_VALUE;
	}
}