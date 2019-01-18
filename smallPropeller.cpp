//=============================================================================
//
// �X���[���v���y������ [smallPropeller.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "enemyManager.h"
#include "player.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define SMALLPROPELLER_INITHP					(70)				//�����̗�
#define SMALLPROPELLER_BULLET_SPEED				(4.5f)				//�o���b�g�̑��x
#define SMALLPROPELLER_ROTANGLE					(RADIAN(5.0f))		//�v���C���[�̕��֌����ۂ̊p�x
#define SMALLPROPELLER_MOVESPEED				(7.5f)				//�ړ��X�s�[�h

#define SMALLPROPELLER_ATTACKMAX				(6)					//�U���̍ő��
#define SMALLPROPELLER_ATTACKINTERBAL			(5)					//�U���̃C���^�[�o��
#define SMALLPROPELLER_ATTACKFRAMEEASY			(SMALLPROPELLER_ATTACKINTERBAL*2) //�U������
#define SMALLPROPELLER_ATTACKFRAMENORMAL		(SMALLPROPELLER_ATTACKINTERBAL*5) //�U������
#define SMALLPROPELLER_ATTACKFRAMEHARD			(SMALLPROPELLER_ATTACKINTERBAL*8) //�U������

#define SMALLPROPELLER_WAIT						(60)				//�ҋ@����
#define SMALLPROPELLER_WAITHARD					(30)				//�ҋ@���ԁiHARD�j	

#define SMALLPROPELLER_SHADOW_OFFSETPOS			(D3DXVECTOR3(100.0f, 200.0f, 0.0f))	//�e�̃I�t�Z�b�g���W

#define SMALLPROPELLER_SCORE					(500)				//���Ď��̃X�R�A

#define SMALLPROPELLER_ACCELFRAME				(120)				//������Ƃ��ɉ������鎞��

#define SMALLPROPELLER_ATTACKPOS_X				(25.0f)				//�o���b�g���ˈʒu��X���W
#define SMALLPROPELLER_ATTACKPOS_Y				(50.0f)				//�o���b�g���ˈʒu��Y���W

#define SMALLPROPELLER_SHOT_ANGLE				(80)				//WAY�V���b�g�p�x
#define SMALLPROPELLER_SHOT_WAY					(2)					//WAY�V���b�gWAY��
/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexSmallPropeller(void);								//���_�쐬�֐�
void SetTextureSmallPropeller(int cntPattern, SMALLPROPELLER *ptr);	// �e�N�X�`�����W�̌v�Z����
void SetVertexSmallPropeller(SMALLPROPELLER *ptr);					// ���_�̌v�Z����
void SetShadowSmallPropeller(SMALLPROPELLER *ptr);					//�e�p�̒��_�ݒ菈��
void SmallPropellerAction(SMALLPROPELLER *ptr);						//�s������
void SmallPropellerAttack(SMALLPROPELLER *ptr);						//�U������

/*****************************************************************************
�\���̒�`
*****************************************************************************/
//�G�l�~�[�̍s����Ԃ������񋓑�
enum
{
	MOVE,
	WAIT,
	ATTACK,
	ESCAPE
};

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[SMALLPROPELLER_TEXTURE_NUM];	// �e�N�X�`���ւ̃|�C���^

static SMALLPROPELLER sPropeller[SMALLPROPELLER_MAX];			//�X���[���v���y���z��

static VERTEX_2D vertexWk[NUM_VERTEX];							//���_���i�[���[�N
static int atkFrame;											//�U������

static ENEMYATTACK bulletParam2 =								//�U���p�̃o���b�g�p�����[�^
{
	GREEN_CIRCLE,
	VECTOR3_ZERO,
	SMALLPROPELLER_BULLET_SPEED,
	ENEMYBULLET_DAFAULT_ANGLE
};

/******************************************************************************
����������
******************************************************************************/
HRESULT InitSmallPropeller(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SMALLPROPELLER* ptr = GetSmallPropellerAdr(0);
	int i;

	//��Փx�ɂ���čU�����Ԃ𕪊�
	int difficulty = GetDifficulty();
	atkFrame = (difficulty == DIFFICULTY_EASY) ? SMALLPROPELLER_ATTACKFRAMEEASY : (difficulty == DIFFICULTY_NORMAL) ? SMALLPROPELLER_ATTACKFRAMENORMAL : SMALLPROPELLER_ATTACKFRAMEHARD;

	for (i = 0; i < SMALLPROPELLER_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(SMALLPROPELLER_INITPOS_X, SMALLPROPELLER_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(SMALLPROPELLER_TEXTURE_SIZE_Y, SMALLPROPELLER_TEXTURE_SIZE_X);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(SMALLPROPELLER_TEXTURE_SIZE_X, SMALLPROPELLER_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->shotPosAngle = atan2f(SMALLPROPELLER_ATTACKPOS_Y, SMALLPROPELLER_ATTACKPOS_X);
		ptr->shotPosRadius = D3DXVec2Length(&D3DXVECTOR2(SMALLPROPELLER_ATTACKPOS_X, SMALLPROPELLER_ATTACKPOS_Y));

		ptr->active = false;
	}

	// ���_���̍쐬
	MakeVertexSmallPropeller();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[0] = CreateTextureFromFile((LPSTR)SMALLPROPELLER_SHADOWTEX_NAME, pDevice);
		texture[1] = CreateTextureFromFile((LPSTR)SMALLPROPELLER_TEXTURE_NAME, pDevice);
	}
	//SetSmallPropeller(D3DXVECTOR3(SCREEN_CENTER_X, 0.0f, 0.0f), D3DXVECTOR3(SCREEN_CENTER_X, 200.0f, 0.0f));
	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitSmallPropeller(void)
{
	for (int i = 0; i < SMALLPROPELLER_TEXTURE_NUM; i++)
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
void UpdateSmallPropeller(void)
{
	int i;
	SMALLPROPELLER *ptr = GetSmallPropellerAdr(0);

	for (i = 0; i < SMALLPROPELLER_MAX; i++, ptr++)
	{
		//��A�N�e�B�u�ł���Ύ��̃G�l�~�[��
		if (!ptr->active)
		{
			continue;
		}

		//���Ĕ���
		if (ptr->hp <= 0)
		{
			//���ď���
			ptr->active = false;
			AddScore(SMALLPROPELLER_SCORE);
			ExplodeMiddleEnemy(ptr->pos);
			continue;
		}

		//�A�j���[�V����
		ptr->countAnim++;
		if (ptr->countAnim % SMALLPROPELLER_TIME_ANIMATION == 0)
		{
			ptr->patternAnim = (ptr->patternAnim + 1) % SMALLPROPELLER_ANIM_NUM;
		}

		//�s������
		SmallPropellerAction(ptr);
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawSmallPropeller(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SMALLPROPELLER *ptr = GetSmallPropellerAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e��`��
	pDevice->SetTexture(0, texture[0]);
	for (i = 0; i < SMALLPROPELLER_MAX; i++, ptr++)
	{
		//�A�N�e�B�u�ł���Ήe��`��
		if (ptr->active)
		{
			SetShadowSmallPropeller(ptr);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}

	//�X���[���v���y����`��
	pDevice->SetTexture(0, texture[1]);
	ptr = GetSmallPropellerAdr(0);
	for (i = 0; i < SMALLPROPELLER_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexSmallPropeller(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureSmallPropeller(ptr->patternAnim, ptr);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//�����蔻��̕`��
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(SMALLPROPELLER_BB_LEFT, SMALLPROPELLER_BB_TOP, 0.0f), SMALLPROPELLER_BB_WIDTH, SMALLPROPELLER_BB_HIGHT);
#endif
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexSmallPropeller(void)
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
void SetTextureSmallPropeller(int cntPattern, SMALLPROPELLER *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % SMALLPROPELLER_TEXTURE_DIVIDE_X;
	int y = cntPattern / SMALLPROPELLER_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / SMALLPROPELLER_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / SMALLPROPELLER_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexSmallPropeller(SMALLPROPELLER *ptr)
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
�e�p�̒��_�ݒ�
******************************************************************************/
void SetShadowSmallPropeller(SMALLPROPELLER *ptr)
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

	//�e�p�ɒ��_�����炷
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertexWk[i].vtx += SMALLPROPELLER_SHADOW_OFFSETPOS;
	}

	//�e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
//�֐���	�FSMALLPROPELLER *GetSmallPropellerAdr(int pno)
//����		�Fint pno�F�擾�������X���[���v���y���̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�X���[���v���y���̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�X���[���v���y���̃A�h���X�擾�֐��i���C���V���b�g�j
******************************************************************************/
SMALLPROPELLER *GetSmallPropellerAdr(int pno)
{
	if (pno < SMALLPROPELLER_MAX)
	{
		return &sPropeller[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�Fvoid SetSmallPropeller(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos)
//����1		�FD3DXVECTOR3 initPos		�F�o�����W
//����2		�FD3DXVECTOR3 targetPos		�F�ڕW���W
//�߂�l	�Fvoid
//����		�F�X���[���v���y�����o��������֐�
******************************************************************************/
void SetSmallPropeller(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos)
{
	int i = 0;
	SMALLPROPELLER *ptr = GetSmallPropellerAdr(0);

	for (i = 0; i < SMALLPROPELLER_MAX; i++, ptr++)
	{
		//��A�N�e�B�u�̃G�l�~�[�ł���Ώo��������
		if (!ptr->active)
		{
			//�e���W�̐ݒ�
			ptr->pos = initPos;
			ptr->targetPos = targetPos;

			//�e��p�����[�^�̏�����
			ptr->hp = SMALLPROPELLER_INITHP;
			ptr->rot = VECTOR3_ZERO;
			ptr->patternAnim = 0;
			ptr->countAnim = 0;

			//�A�N�e�B�u�ɂ��ă��^�[��
			ptr->state = MOVE;
			ptr->active = true;
			return;
		}
	}
}

/******************************************************************************
//�֐���	�Fvoid SmallPropellerAction(SMALLPROPELLER *ptr)
//����1		�FSMALLPROPELLER *ptr	�F�s������X���[���v���y���ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�X���[���v���y���̍s������
******************************************************************************/
void SmallPropellerAction(SMALLPROPELLER *ptr)
{
	bool flgMove = false;
	PLAYER *player = GetPlayerAdr(0);
	int wait = (GetDifficulty() == DIFFICULTY_HARD) ? SMALLPROPELLER_WAITHARD : SMALLPROPELLER_WAIT;
	ptr->countFrame++;

	//�v���C���[�̕��֌���
	ptr->rot.z += RotateToTarget(&ptr->pos, &player->pos, ptr->rot.z, SMALLPROPELLER_ROTANGLE);

	switch (ptr->state)
	{
		//�ڕW���W�֌������Ĉړ�������
	case MOVE:		
		//�ړ�
		flgMove = MoveToTarget(&ptr->pos, &ptr->targetPos, SMALLPROPELLER_MOVESPEED);
		if (flgMove)
		{
			//�ڕW���W�֓��B���Ă����玟�̏�Ԃ֑J��
			ptr->countFrame = 0;
			ptr->attackNum = 0;
			ptr->state = WAIT;
		}
		break;

		//�U����̑ҋ@���
	case WAIT:
		//�ҋ@���Ԃ��o�߂��Ă�����U����Ԃ֑J��
		if (ptr->countFrame >= wait)
		{
			ptr->countFrame = 0;
			ptr->state = ATTACK;
		}
		break;

		//�U��������
	case ATTACK:
		if (ptr->countFrame % SMALLPROPELLER_ATTACKINTERBAL == 0)
		{
			//�U������
			SmallPropellerAttack(ptr);
		}

		//�U���񐔂��ő�ɓ��B���Ă����瓦�����Ԃ֑J��
		if (ptr->attackNum >= SMALLPROPELLER_ATTACKMAX)
		{
			ptr->countFrame = 0;
			ptr->state = ESCAPE;
		}
		//�U�����Ԃ��o�߂��Ă�����ҋ@��Ԃ֑J��
		else if (ptr->countFrame >= atkFrame)
		{
			ptr->attackNum++;					//�U���񐔂��J�E���g
			ptr->countFrame = 0;
			ptr->state = WAIT;
		}
		break;


		//�U�����I�����ē�������
	case ESCAPE:
		//�ړ�
		StartAccel(&ptr->pos, &VECTOR3_UP, SMALLPROPELLER_MOVESPEED, SMALLPROPELLER_ACCELFRAME, ptr->countFrame);

		//��ʊO�֏o�Ă������A�N�e�B�u��
		if (ptr->pos.y <= -SMALLPROPELLER_TEXTURE_SIZE_Y)
		{
			ptr->active = false;
		}
		break;
	}
}

/******************************************************************************
//�֐���	�Fvoid SmallPropellerAttack(SMALLPROPELLER *ptr)
//����1		�FSMALLPROPELLER *ptr	�F�U������X���[���v���y���ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�X���[���v���y���̍U������
******************************************************************************/
void SmallPropellerAttack(SMALLPROPELLER *ptr)
{
	ENEMYATTACK param = bulletParam2;
	PLAYER *player = GetPlayerAdr(0);
	D3DXVECTOR3 shotPos = VECTOR3_ZERO;
	int difficulty = GetDifficulty();

	//���̔��ˈʒu���甭��
	shotPos.x = ptr->pos.x - cosf(ptr->shotPosAngle - ptr->rot.z) * ptr->shotPosRadius;
	shotPos.y = ptr->pos.y + sinf(ptr->shotPosAngle - ptr->rot.z) * ptr->shotPosRadius;
	param.pos = shotPos;
	param.angle = SnipeTarget(&param.pos, &player->pos);
	SetEnemyBullet(&param, &ptr->hp);

	//��ՓxHARD�Ȃ�WAY�e�ǉ�
	if (difficulty == DIFFICULTY_HARD)
	{
		FireWayBullet(&param, SMALLPROPELLER_SHOT_WAY, SMALLPROPELLER_SHOT_ANGLE, &ptr->hp);
	}

	//�E�̔��ˈʒu���甭��
	shotPos.x = ptr->pos.x + cosf(ptr->shotPosAngle + ptr->rot.z) * ptr->shotPosRadius;
	shotPos.y = ptr->pos.y + sinf(ptr->shotPosAngle + ptr->rot.z) * ptr->shotPosRadius;
	param.pos = shotPos;
	param.angle = SnipeTarget(&param.pos, &player->pos);
	SetEnemyBullet(&param, &ptr->hp);

	//��ՓxHARD�Ȃ�WAY�e�ǉ�
	if (difficulty == DIFFICULTY_HARD)
	{
		FireWayBullet(&param, SMALLPROPELLER_SHOT_WAY, SMALLPROPELLER_SHOT_ANGLE, &ptr->hp);
	}
}