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
#define MIDDLETANK_INITHP					(30)				//�̗͏����l
#define MIDDLETANK_SCORE					(500)				//���Ď��̃X�R�A
#define MIDDLETANK_BULLET_SPEED				(5.0f)				//�o���b�g�̑��x
#define MIDDLETANK_MOVE_SPEED				(3.5f)				//�ړ����x
#define MIDDLETANK_SCROLLSPEED				(-BG_STAGE1_SPEED3*SCREEN_HEIGHT) //�w�i�ɍ��킹���X�N���[�����x

#define MIDDLETANK_TURRET_ROTANGLE			(RADIAN(10.0f))		//�C���̉�]�p�x

#define MIDDLETANK_INITFRAME				(120)				//INIT��Ԃőҋ@����t���[����

#define MIDDLETANK_ATTACK_MAX				(4)					//�ő�U����
#define MIDDLETANK_ATTACK_INTERBAL			(3)					//�U���Ԋu
#define MIDDLETANK_ATTACK_FRAMEEASY			(MIDDLETANK_ATTACK_INTERBAL*2) //�U���ɂ����鎞��
#define MIDDLETANK_ATTACK_FRAMENORMAL		(MIDDLETANK_ATTACK_INTERBAL*4) //�U���ɂ����鎞��
#define MIDDLETANK_ATTACK_FRAMEHARD			(MIDDLETANK_ATTACK_INTERBAL*8) //�U���ɂ����鎞��

#define MIDDLETANK_WAIT_FRAME				(90)				//�ҋ@����

#define MIDDLETANK_SHOTPOS_X				(0.0f)				//�o���b�g���ˈʒu��X���W
#define MIDDLETANK_SHOTPOS_Y				(50.0f)				//�o���b�g���ˈʒu��Y���W

#define MIDDLETANK_ATTACK_WAYEASY				(2)					//WAY�e��WAY���iEASY)	
#define MIDDLETANK_ATTACK_WAY				(3)					//WAY�e��WAY��
#define MIDDLETANK_ATTACK_RANGE				(60)				//WAY�e�̊p�x

#define MIDDLETANK_SETTER_INTERBAL			(45)				//�Z�b�^�[���^���N���o��������Ԋu
#define MIDDLETANK_SETTER_SETMAX			(8)					//�Z�b�^�[���^���N���o��������ő吔
/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexMiddleTank(void);								//���_�쐬�֐�
void SetTextureMiddleTank(int cntPattern, MIDDLETANK *ptr);		// �e�N�X�`�����W�̌v�Z����
void SetVertexMiddleTank(MIDDLETANK *ptr);						// ���_�̌v�Z����
void SetTurretVertex(MIDDLETANK *ptr);							//���_�̌v�Z�����i�C���p)
void ActionMiddleTank(MIDDLETANK *ptr);							//�s������
void AttackMiddleTank(MIDDLETANK *ptr);							//�U������

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
static LPDIRECT3DTEXTURE9		texture[MIDDLETANK_TEXTURE_NUM];		// �e�N�X�`���ւ̃|�C���^

MIDDLETANK middleTank[MIDDLETANK_MAX];									//�~�h���^���N�z��

static VERTEX_2D		vertexWk[NUM_VERTEX];							//���_���i�[���[�N
static MIDDLETANK_SETTER setter[MIDDLETANK_SETTER_MAX];					//�~�h���^���N�Z�b�^�[

static int atkFrame;													//�U������

//�U���p�p�����[�^
static ENEMYATTACK bulletParam1 =
{
	GREEN_ROTATE,
	VECTOR3_ZERO,
	MIDDLETANK_BULLET_SPEED,
	0.0f
};

/******************************************************************************
����������
******************************************************************************/
HRESULT InitMiddleTank(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MIDDLETANK* ptr = GetMiddleTankAdr(0);
	int i;

	//��Փx�ɂ���čU�����Ԃ𕪊�
	int difficulty = GetDifficulty();
	atkFrame = (difficulty == DIFFICULTY_EASY) ? MIDDLETANK_ATTACK_FRAMEEASY : (difficulty == DIFFICULTY_NORMAL) ? MIDDLETANK_ATTACK_FRAMENORMAL : MIDDLETANK_ATTACK_FRAMEHARD;
	//�^���N�̏�����
	for (i = 0; i < MIDDLETANK_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(MIDDLETANK_INITPOS_X, MIDDLETANK_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(MIDDLETANK_TEXTURE_SIZE_Y, MIDDLETANK_TEXTURE_SIZE_X);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(MIDDLETANK_TEXTURE_SIZE_X, MIDDLETANK_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->turretRot = 0.0f;
		ptr->countAnim = 0;
		ptr->moveDir = VECTOR3_ZERO;
		ptr->state = INIT;

		ptr->hp = MIDDLETANK_INITHP;
		ptr->attackNum = 0;
		ptr->shotWay = (GetDifficulty() == DIFFICULTY_EASY) ? MIDDLETANK_ATTACK_WAYEASY : MIDDLETANK_ATTACK_WAY;

		ptr->active = false;
	}

	//�Z�b�^�[�̏�����
	for (i = 0; i < MIDDLETANK_SETTER_MAX; i++)
	{
		setter[i].active = false;
		setter[i].pos = VECTOR3_ZERO;
		setter[i].cntFrame = 0;
		setter[i].cntSet = 0;
	}

	// ���_���̍쐬
	MakeVertexMiddleTank();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[BODY] = CreateTextureFromFile((LPSTR)MIDDLETANK_TEXTURE_NAME, pDevice);
		texture[TURRET] = CreateTextureFromFile((LPSTR)MIDDLETANK_TURRETTEX_NAME, pDevice);
	}
	//SetMiddleTank(D3DXVECTOR3(BG_LEFT, 0.0f, 0.0f), D3DXVECTOR3(BG_RIGHT, 1.0f, 0.0f));
	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitMiddleTank(void)
{
	for (int i = 0; i < MIDDLETANK_TEXTURE_NUM; i++)
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
void UpdateMiddleTank(void)
{
	int i;
	MIDDLETANK *ptr = GetMiddleTankAdr(0);
	PLAYER *player = GetPlayerAdr(0);

	//�Z�b�^�[���X�V
	for (i = 0; i < MIDDLETANK_SETTER_MAX; i++)
	{
		if (setter[i].active)
		{
			setter[i].pos.y += MIDDLETANK_SCROLLSPEED;
			setter[i].targetPos.y += MIDDLETANK_SCROLLSPEED;
			if (setter[i].cntFrame % MIDDLETANK_SETTER_INTERBAL == 0)
			{
				SetMiddleTank(setter[i].pos, setter[i].targetPos);
				setter[i].cntSet++;

				if (setter[i].cntSet >= MIDDLETANK_SETTER_SETMAX)
				{
					setter[i].active = false;
				}
			}
			setter[i].cntFrame++;
		}
	}

	//�^���N���X�V
	for (i = 0; i < MIDDLETANK_MAX; i++, ptr++)
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
			AddScore(MIDDLETANK_SCORE);
			ExplodeMiddleEnemy(ptr->pos);
			SetGroundFire(ptr->pos, ptr->rot);
			continue;
		}

		//���W�̍X�V
		ptr->pos += ptr->moveDir * MIDDLETANK_MOVE_SPEED;
		ptr->pos.y += MIDDLETANK_SCROLLSPEED;

		//���W����ʊO�ł���Δ�A�N�e�B�u��
		if (ptr->state != INIT)
		{
			if (ptr->pos.x + MIDDLETANK_TEXTURE_SIZE_X < BG_LEFT || ptr->pos.x - MIDDLETANK_TEXTURE_SIZE_X > BG_RIGHT
				|| ptr->pos.y + MIDDLETANK_TEXTURE_SIZE_Y < 0.0f || ptr->pos.y - MIDDLETANK_TEXTURE_SIZE_Y > SCREEN_HEIGHT)
			{
				ptr->active = false;
			}
		}

		//�A�j���[�V����
		ptr->countAnim++;
		if (ptr->countAnim % MIDDLETANK_TIME_ANIMATION == 0)
		{
			ptr->patternAnim = (ptr->patternAnim + 1) % MIDDLETANK_ANIM_NUM;
		}

		//�C������]
		if (ptr->state != ATTACK)
		{
			ptr->turretRot += RotateToTarget(&ptr->pos, &player->pos, ptr->turretRot, MIDDLETANK_TURRET_ROTANGLE);
		}

		//�s������
		ActionMiddleTank(ptr);
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawMiddleTank(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MIDDLETANK *ptr = GetMiddleTankAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�~�h���^���N�̖{�̂�`��
	pDevice->SetTexture(0, texture[BODY]);
	for (i = 0; i < MIDDLETANK_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexMiddleTank(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureMiddleTank(ptr->patternAnim, ptr);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}

	//�~�h���^���N�̖C����`��
	ptr = GetMiddleTankAdr(0);
	pDevice->SetTexture(0, texture[TURRET]);
	for (i = 0; i < MIDDLETANK_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetTurretVertex(ptr);
			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//�����蔻��̕`��
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(MIDDLETANK_BB_LEFT, MIDDLETANK_BB_TOP, 0.0f), MIDDLETANK_BB_WIDTH, MIDDLETANK_BB_HIGHT);
#endif
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexMiddleTank(void)
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
void SetTextureMiddleTank(int cntPattern, MIDDLETANK *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % MIDDLETANK_TEXTURE_DIVIDE_X;
	int y = cntPattern / MIDDLETANK_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / MIDDLETANK_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / MIDDLETANK_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�(�{�̗p)
******************************************************************************/
void SetVertexMiddleTank(MIDDLETANK *ptr)
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
void SetTurretVertex(MIDDLETANK *ptr)
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
//�֐���	�FMIDDLETANK *GetMiddleTankAdr(int pno)
//����		�Fint pno�F�擾�������~�h���^���N�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�~�h���^���N�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�~�h���^���N�̃A�h���X�擾�֐��i���C���V���b�g�j
******************************************************************************/
MIDDLETANK *GetMiddleTankAdr(int pno)
{
	if (pno < MIDDLETANK_MAX)
	{
		return &middleTank[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�Fvoid SetMiddleTank(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos)
//����1		�FD3DXVECTOR3 initPos			�F�o�����W
//����2		�FD3DXVECTOR3 targetPos			�F�ڕW���W
//�߂�l	�Fvoid
//����		�F�~�h���^���N�̃Z�b�g�֐�
******************************************************************************/
void SetMiddleTank(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos)
{
	int i;
	MIDDLETANK *ptr = GetMiddleTankAdr(0);
	PLAYER *player = GetPlayerAdr(0);

	for (i = 0; i < MIDDLETANK_MAX; i++, ptr++)
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
			ptr->hp = MIDDLETANK_INITHP;
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
//�֐���	�Fvoid ActionMiddleTank(MIDDLETANK *ptr)
//����1		�FMIDDLETANK *ptr		�F�s�����s���~�h���^���N�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�~�h���^���N�̍s������
******************************************************************************/
void ActionMiddleTank(MIDDLETANK *ptr)
{
	ptr->countFrame++;

	switch (ptr->state)
	{
		//�o������̏��
	case INIT:
		//�o�����Ă����莞�Ԍo�߂�����ATTACK��Ԃ֑J��
		if (ptr->countFrame >= MIDDLETANK_INITFRAME)
		{
			ptr->countFrame = 0;
			ptr->state = ATTACK;
		}
		break;

		//�U�����
	case ATTACK:
		if (ptr->countFrame % MIDDLETANK_ATTACK_INTERBAL == 0)
		{
			//�U������
			AttackMiddleTank(ptr);
		}

		//�U���񐔂��ő�֓��B���Ă������ԑJ��
		if (ptr->attackNum >= MIDDLETANK_ATTACK_MAX)
		{
			ptr->countFrame = 0;
			ptr->state = ESCAPE;
		}
		//�U�����Ԃ��I����Ă���Αҋ@��Ԃ֑J��
		else if (ptr->countFrame >= atkFrame)
		{
			ptr->countFrame = 0;
			ptr->attackNum++;
			ptr->state = WAIT;
		}
		break;

		//�ҋ@���
	case WAIT:
		//��莞�Ԍo�߂��Ă�����U����Ԃ֑J��
		if (ptr->countFrame > MIDDLETANK_WAIT_FRAME)
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
//�֐���	�Fvoid AttackMiddleTank(MIDDLETANK *ptr)
//����1		�FMIDDLETANK *ptr		�F�U�����s���~�h���^���N�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�~�h���^���N�̍U������
******************************************************************************/
void AttackMiddleTank(MIDDLETANK *ptr)
{
	ENEMYATTACK param = bulletParam1;
	D3DXVECTOR3 shotPos = VECTOR3_ZERO;
	float shotAngle = ptr->turretRot + RADIAN(90);

	//���ˈʒu�̌v�Z
	shotPos.x = ptr->pos.x + cosf(shotAngle) * MIDDLETANK_SHOTPOS_Y;
	shotPos.y = ptr->pos.y + sinf(shotAngle) * MIDDLETANK_SHOTPOS_Y;

	//�p�����[�^��ݒ肵�o���b�g�𔭎�
	param.pos = shotPos;
	param.angle = shotAngle;
	FireWayBullet(&param, ptr->shotWay, MIDDLETANK_ATTACK_RANGE, &ptr->hp);
}

/******************************************************************************
//�֐���	�Fvoid CreateMiddleTankSetter(D3DXVECTOR3 initPos, D3DXVECTOR3 target)
//����1		�FD3DXVECTOR3 initPos			�F�o�����W
//����2		�FD3DXVECTOR3 targetPos			�F�ڕW���W
//�߂�l	�Fvoid
//����		�F�~�h���^���N�̃Z�b�^�[��ݒ肷��֐�
******************************************************************************/
void CreateMiddleTankSetter(D3DXVECTOR3 initPos, D3DXVECTOR3 target)
{
	for (int i = 0; i < MIDDLETANK_SETTER_MAX; i++)
	{
		if (!setter[i].active)
		{
			setter[i].active = true;
			setter[i].pos = initPos;
			setter[i].targetPos = target;
			setter[i].cntFrame = 0;
			setter[i].cntSet = 0;
			return;
		}
	}
}