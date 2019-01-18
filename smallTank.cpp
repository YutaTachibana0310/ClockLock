//=============================================================================
//
// ���� [middleTank.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "bg.h"
#include "enemyManager.h"
#include "groundFire.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define SMALLTANK_INITHP					(30)				//�̗͏����l
#define SMALLTANK_SCORE						(500)				//���Ď��̃X�R�A
#define SMALLTANK_BULLET_SPEED				(5.5f)				//�o���b�g�̑��x
#define SMALLTANK_MOVE_SPEED				(2.5f)				//�ړ����x
#define SMALLTANK_SCROLLSPEED				(-BG_STAGE1_SPEED3*SCREEN_HEIGHT) //�w�i�ɍ��킹���X�N���[�����x

#define SMALLTANK_TURRET_ROTANGLE			(RADIAN(10.0f))		//�C���̉�]�p�x

#define SMALLTANK_INITFRAMEEASY					(240)			//INIT��Ԃőҋ@����t���[����
#define SMALLTANK_INITFRAMENORMAL				(120)			//INIT��Ԃőҋ@����t���[����
#define SMALLTANK_INITFRAMEHARD					(30)			//INIT��Ԃőҋ@����t���[����

#define SMALLTANK_ATTACK_MAX				(4)					//�ő�U����
#define SMALLTANK_ATTACK_INTERBAL			(5)					//�U���Ԋu
#define SMALLTANK_ATTACK_FRAME				(SMALLTANK_ATTACK_INTERBAL*3) //�U���ɂ����鎞��

#define SMALLTANK_WAIT_FRAME				(90)				//�ҋ@����

#define SMALLTANK_SHOTPOS_X					(0.0f)				//�o���b�g���ˈʒu��X���W
#define SMALLTANK_SHOTPOS_Y					(50.0f)				//�o���b�g���ˈʒu��Y���W

#define SMALLTANK_ATTACK_WAY				(3)					//WAY�e��WAY��
#define SMALLTANK_ATTACK_RANGE				(60)				//WAY�e�̊p�x

#define SMALLTANK_SETTER_INTERBAL			(30)				//�Z�b�^�[���^���N���o��������Ԋu
#define SMALLTANK_SETTER_SETMAX				(8)				//�Z�b�^�[���^���N���o��������ő吔

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexSmallTank(void);								//���_�쐬�֐�
void SetTextureSmallTank(int cntPattern, SMALLTANK *ptr);		// �e�N�X�`�����W�̌v�Z����
void SetVertexSmallTank(SMALLTANK *ptr);						// ���_�̌v�Z����
void SetTurretVertex(SMALLTANK *ptr);							//���_�̌v�Z�����i�C���p)
void ActionSmallTank(SMALLTANK *ptr);							//�s������
void AttackSmallTank(SMALLTANK *ptr);							//�U������

/*****************************************************************************
�\���̒�`
*****************************************************************************/
//�e�N�X�`���ԍ��̗񋓑�
enum
{
	BODY,
	TURRET
};

//�s����Ԃ̗񋓑�
enum
{
	INIT,
	ATTACK,
	WAIT,
	ESCAPE
};

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture[SMALLTANK_TEXTURE_NUM];			// �e�N�X�`���ւ̃|�C���^

SMALLTANK middleTank[SMALLTANK_MAX];									//�~�h���^���N�z��

static VERTEX_2D		vertexWk[NUM_VERTEX];							//���_���i�[���[�N
static SMALLTANK_SETTER setter[SMALLTANK_SETTER_MAX];					//�Z�b�^�[�z��
static int initWaitFrame;												//INIT��Ԃ���J�ڂ��鎞��

//�U���p�p�����[�^
static ENEMYATTACK bulletParam1 =
{
	ORANGE_CIRCLE,
	VECTOR3_ZERO,
	SMALLTANK_BULLET_SPEED,
	0.0f
};

/******************************************************************************
����������
******************************************************************************/
HRESULT InitSmallTank(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SMALLTANK* ptr = GetSmallTankAdr(0);
	int i;

	//INIT�ł̑ҋ@���Ԃ��Փx�ŕ���
	int difficulty = GetDifficulty();
	initWaitFrame = (difficulty == DIFFICULTY_EASY) ? SMALLTANK_INITFRAMEEASY : (difficulty == DIFFICULTY_NORMAL) ? SMALLTANK_INITFRAMENORMAL : SMALLTANK_INITFRAMEHARD;

	//�^���N�̏�����
	for (i = 0; i < SMALLTANK_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(SMALLTANK_INITPOS_X, SMALLTANK_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(SMALLTANK_TEXTURE_SIZE_Y, SMALLTANK_TEXTURE_SIZE_X);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(SMALLTANK_TEXTURE_SIZE_X, SMALLTANK_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->turretRot = 0.0f;
		ptr->countAnim = 0;
		ptr->moveDir = VECTOR3_ZERO;
		ptr->state = INIT;

		ptr->hp = SMALLTANK_INITHP;
		ptr->attackNum = 0;

		ptr->active = false;
	}

	//�Z�b�^�[�̏�����
	for (i = 0; i < SMALLTANK_SETTER_MAX; i++)
	{
		setter[i].active = false;
		setter[i].cntFrame = 0;
		setter[i].cntSet = 0;
		setter[i].pos = VECTOR3_ZERO;
		setter[i].targetPos = VECTOR3_ZERO;
	}

	// ���_���̍쐬
	MakeVertexSmallTank();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[BODY] = CreateTextureFromFile((LPSTR)SMALLTANK_TEXTURE_NAME, pDevice);
		texture[TURRET] = CreateTextureFromFile((LPSTR)SMALLTANK_TURRETTEX_NAME, pDevice);
	}
	//SetSmallTank(D3DXVECTOR3(BG_LEFT, 0.0f, 0.0f), D3DXVECTOR3(BG_RIGHT, 1.0f, 0.0f));
	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitSmallTank(void)
{
	for (int i = 0; i < SMALLTANK_TEXTURE_NUM; i++)
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
void UpdateSmallTank(void)
{
	int i;
	SMALLTANK *ptr = GetSmallTankAdr(0);
	PLAYER *player = GetPlayerAdr(0);

	//�Z�b�^�[�̍X�V
	for (i = 0; i < SMALLTANK_SETTER_MAX; i++)
	{
		if (setter[i].active)
		{
			setter[i].pos.y += SMALLTANK_SCROLLSPEED;
			setter[i].targetPos.y += SMALLTANK_SCROLLSPEED;
			if (setter[i].cntFrame % SMALLTANK_SETTER_INTERBAL == 0)
			{
				SetSmallTank(setter[i].pos, setter[i].targetPos);
				setter[i].cntSet++;

				if (setter[i].cntSet >= SMALLTANK_SETTER_SETMAX)
				{
					setter[i].active = false;
				}
			}

			setter[i].cntFrame++;
		}
	}

	//�^���N�̍X�V
	for (i = 0; i < SMALLTANK_MAX; i++, ptr++)
	{
		//��A�N�e�B�u�Ȃ玟�̃G�l�~�[��
		if (!ptr->active)
		{
			continue;
		}

		//���Ĕ���
		if (ptr->hp <= 0)
		{
			//���ď���
			ptr->active = false;
			AddScore(SMALLTANK_SCORE);
			ExplodeSmallEnemy(ptr->pos);
			SetGroundFire(ptr->pos, ptr->rot);
			continue;
		}

		//���W�̍X�V
		ptr->pos += ptr->moveDir * SMALLTANK_MOVE_SPEED;
		ptr->pos.y += SMALLTANK_SCROLLSPEED;

		//���W����ʊO�ł���Δ�A�N�e�B�u��
		if (ptr->state != INIT)
		{
			if (ptr->pos.x + SMALLTANK_TEXTURE_SIZE_X < BG_LEFT || ptr->pos.x - SMALLTANK_TEXTURE_SIZE_X > BG_RIGHT
				|| ptr->pos.y + SMALLTANK_TEXTURE_SIZE_Y < 0.0f || ptr->pos.y - SMALLTANK_TEXTURE_SIZE_Y > SCREEN_HEIGHT)
			{
				ptr->active = false;
			}
		}

		//�A�j���[�V����
		ptr->countAnim++;
		if (ptr->countAnim % SMALLTANK_TIME_ANIMATION == 0)
		{
			ptr->patternAnim = (ptr->patternAnim + 1) % SMALLTANK_ANIM_NUM;
		}

		//�C������]
		if (ptr->state != ATTACK)
		{
			ptr->turretRot += RotateToTarget(&ptr->pos, &player->pos, ptr->turretRot, SMALLTANK_TURRET_ROTANGLE);
		}

		//�s������
		ActionSmallTank(ptr);
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawSmallTank(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SMALLTANK *ptr = GetSmallTankAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�~�h���^���N�̖{�̂�`��
	pDevice->SetTexture(0, texture[BODY]);
	for (i = 0; i < SMALLTANK_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexSmallTank(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureSmallTank(ptr->patternAnim, ptr);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}

	//�~�h���^���N�̖C����`��
	ptr = GetSmallTankAdr(0);
	pDevice->SetTexture(0, texture[TURRET]);
	for (i = 0; i < SMALLTANK_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetTurretVertex(ptr);
			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//�����蔻��̕`��
			DrawBoxCollider(ptr->pos + D3DXVECTOR3(SMALLTANK_BB_LEFT, SMALLTANK_BB_TOP, 0.0f), SMALLTANK_BB_WIDTH, SMALLTANK_BB_HIGHT);
#endif
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexSmallTank(void)
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
void SetTextureSmallTank(int cntPattern, SMALLTANK *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % SMALLTANK_TEXTURE_DIVIDE_X;
	int y = cntPattern / SMALLTANK_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / SMALLTANK_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / SMALLTANK_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�(�{�̗p)
******************************************************************************/
void SetVertexSmallTank(SMALLTANK *ptr)
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
���_���W�̐ݒ�(�C���p)
******************************************************************************/
void SetTurretVertex(SMALLTANK *ptr)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->turretRot) * ptr->radius;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->turretRot) * ptr->radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->turretRot) * ptr->radius;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->turretRot) * ptr->radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->turretRot) * ptr->radius;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->turretRot) * ptr->radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->turretRot) * ptr->radius;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->turretRot) * ptr->radius;
}

/******************************************************************************
//�֐���	�FSMALLTANK *GetSmallTankAdr(int pno)
//����		�Fint pno�F�擾�������~�h���^���N�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�~�h���^���N�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�~�h���^���N�̃A�h���X�擾�֐��i���C���V���b�g�j
******************************************************************************/
SMALLTANK *GetSmallTankAdr(int pno)
{
	if (pno < SMALLTANK_MAX)
	{
		return &middleTank[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�Fvoid SetSmallTank(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos)
//����1		�FD3DXVECTOR3 initPos			�F�o�����W
//����2		�FD3DXVECTOR3 targetPos			�F�ڕW���W
//�߂�l	�Fvoid
//����		�F�~�h���^���N�̃Z�b�g�֐�
******************************************************************************/
void SetSmallTank(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos)
{
	int i;
	SMALLTANK *ptr = GetSmallTankAdr(0);
	PLAYER *player = GetPlayerAdr(0);

	for (i = 0; i < SMALLTANK_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			//�e���W�̐ݒ�
			ptr->pos = initPos;
			D3DXVECTOR3 tmp = targetPos - initPos;
			D3DXVec3Normalize(&ptr->moveDir, &tmp);

			//�|���S���̉�]�ʂ�ݒ�
			tmp = ptr->pos + ptr->moveDir;
			ptr->rot.z = SnipeTarget(&ptr->pos, &tmp) - RADIAN(90);;

			//�e�p�����[�^��������
			ptr->turretRot = SnipeTarget(&ptr->pos, &player->pos) - RADIAN(90);
			ptr->countAnim = 0;
			ptr->state = INIT;
			ptr->hp = SMALLTANK_INITHP;
			ptr->countAnim = 0;
			ptr->patternAnim = 0;
			ptr->attackNum = 0;

			//�~�h���^���N���o���������^�[��
			ptr->active = true;
			return;
		}
	}
}

/******************************************************************************
//�֐���	�Fvoid ActionSmallTank(SMALLTANK *ptr)
//����1		�FSMALLTANK *ptr		�F�s�����s���~�h���^���N�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�~�h���^���N�̍s������
******************************************************************************/
void ActionSmallTank(SMALLTANK *ptr)
{
	ptr->countFrame++;

	switch (ptr->state)
	{
		//�o������̏��
	case INIT:
		//�o�����Ă����莞�Ԍo�߂�����ATTACK��Ԃ֑J��
		if (ptr->countFrame >= initWaitFrame)
		{
			ptr->countFrame = 0;
			ptr->state = ATTACK;
		}
		break;

		//�U�����
	case ATTACK:
		if (ptr->countFrame % SMALLTANK_ATTACK_INTERBAL == 0)
		{
			//�U������
			AttackSmallTank(ptr);
		}

		//�U���񐔂��ő�֓��B���Ă������ԑJ��
		if (ptr->attackNum >= SMALLTANK_ATTACK_MAX)
		{
			ptr->countFrame = 0;
			ptr->state = ESCAPE;
		}
		//�U�����Ԃ��I����Ă���Αҋ@��Ԃ֑J��
		else if (ptr->countFrame >= SMALLTANK_ATTACK_FRAME)
		{
			ptr->countFrame = 0;
			ptr->attackNum++;
			ptr->state = WAIT;
		}
		break;

		//�ҋ@���
	case WAIT:
		//��莞�Ԍo�߂��Ă�����U����Ԃ֑J��
		if (ptr->countFrame > SMALLTANK_WAIT_FRAME)
		{
			ptr->countFrame = 0;
			ptr->state = ATTACK;
		}
		break;

	case ESCAPE:
		//���̏�Ԃ̎��͈ړ��ȊO�������Ȃ�
		break;
	}
}

/******************************************************************************
//�֐���	�Fvoid AttackSmallTank(SMALLTANK *ptr)
//����1		�FSMALLTANK *ptr		�F�U�����s���~�h���^���N�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�~�h���^���N�̍U������
******************************************************************************/
void AttackSmallTank(SMALLTANK *ptr)
{
	ENEMYATTACK param = bulletParam1;
	D3DXVECTOR3 shotPos = VECTOR3_ZERO;
	float shotAngle = ptr->turretRot + RADIAN(90);

	//���ˈʒu�̌v�Z
	shotPos.x = ptr->pos.x + cosf(shotAngle) * SMALLTANK_SHOTPOS_Y;
	shotPos.y = ptr->pos.y + sinf(shotAngle) * SMALLTANK_SHOTPOS_Y;

	//�p�����[�^��ݒ肵�o���b�g�𔭎�
	param.pos = shotPos;
	param.angle = shotAngle;
	SetEnemyBullet(&param, &ptr->hp);
}

/******************************************************************************
//�֐���	�Fvoid CreateSmallTankSetter(D3DXVECTOR3 initPos, D3DXVECTOR3 target)
//����1		�FD3DXVECTOR3 initPos			�F�o�����W
//����2		�FD3DXVECTOR3 targetPos			�F�ڕW���W
//�߂�l	�Fvoid
//����		�F�~�h���^���N�̃Z�b�^�[�ݒ�֐�
******************************************************************************/
void CreateSmallTankSetter(D3DXVECTOR3 initPos, D3DXVECTOR3 target)
{
	for (int i = 0; i < SMALLTANK_SETTER_MAX; i++)
	{
		if (!setter[i].active)
		{
			setter[i].active = true;
			setter[i].cntFrame = 0;
			setter[i].cntSet = 0;
			setter[i].pos = initPos;
			setter[i].targetPos = target;
			return;
		}
	}
}