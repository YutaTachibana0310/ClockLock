//=============================================================================
//
// �X���[���U�R1���� [smallZako1.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "enemyManager.h"
#include "player.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define SMALLZAKO1_INITHP					(10)				//HP�����l
#define SMALLZAKO1_SCORE					(100)				//���Ď��̃X�R�A
#define SMALLZAKO1_INITSPEED				(9.5f)				//�X�s�[�h�����l

#define SMALLZAKO1_BULLET_SPEED				(4.5f)				//�o���b�g�̃X�s�[�h

#define SMALLZAKO1_INIT_FRAME				(15)				//INIT��Ԃ��玟�֑J�ڂ��鎞��

#define SMALLZAKO1_HOMING_LENGTH			(150*150)			//�ǔ����~�߂�v���C���[�Ƃ̋���
#define SMALLZAKO1_HOMING_ANGLE				(RADIAN(2.0f))		//�ǔ����̍ŏ���]�p�x

#define SMALLZAKO1_SHADOW_OFFSETPOS			(D3DXVECTOR3(50.0f, 100.0f, 0.0f)) //�e�̃I�t�Z�b�g���W

#define SMALLZAKO1_SHOT_WAY					(12)					//�����Ԃ��eWAY��

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexSmallZako1(void);							//���_�쐬�֐�
void SetTextureSmallZako1(int cntPattern, SMALLZAKO1 *ptr);	// �e�N�X�`�����W�̌v�Z����
void SetVertexSmallZako1(SMALLZAKO1 *ptr);					// ���_�̌v�Z����
void SetShadowSmallZako1(SMALLZAKO1 *ptr);					//�e�p�̐ݒ菈��
void ActionSmallZako1(SMALLZAKO1 *ptr);						//�X���[���U�R1�̍s������

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
	HOMING,
	NOT_HOMING
};

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[SMALLZAKO1_TEXTURE_NUM];		// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];							//���_���i�[���[�N

static float angle;												//�|���S���̔��a
static float radius;											//�|���S���̒��_�p�x

SMALLZAKO1 smallZako1[SMALLZAKO1_MAX];							//�X���[���U�R1�z��

static ENEMYATTACK bulletParam = {
	GREEN_CIRCLE,
	VECTOR3_ZERO,
	SMALLZAKO1_BULLET_SPEED,
	0.0f
};

/******************************************************************************
����������
******************************************************************************/
HRESULT InitSmallZako1(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SMALLZAKO1* ptr = GetSmallZako1Adr(0);
	int i;

	angle = atan2f(SMALLZAKO1_TEXTURE_SIZE_Y, SMALLZAKO1_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(SMALLZAKO1_TEXTURE_SIZE_X, SMALLZAKO1_TEXTURE_SIZE_Y));

	for (i = 0; i < SMALLZAKO1_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(SMALLZAKO1_INITPOS_X, SMALLZAKO1_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->hp = SMALLZAKO1_INITHP;
		ptr->state = INIT;
		ptr->speed = SMALLZAKO1_INITSPEED;
		ptr->moveDir = VECTOR3_UP * -1;
		ptr->cntFrame = 0;

		ptr->active = false;
	}

	// ���_���̍쐬
	MakeVertexSmallZako1();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[SHADOW] = CreateTextureFromFile((LPSTR)SMALLZAKO1_SHADOWTEX_NAME, pDevice);
		texture[BODY] = CreateTextureFromFile((LPSTR)SMALLZAKO1_TEXTURE_NAME, pDevice);
	}
	//SetSmallZako1(D3DXVECTOR3(BG_LEFT + 200.0f, 0.0f, 0.0f));
	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitSmallZako1(void)
{
	for (int i = 0; i < SMALLZAKO1_TEXTURE_NUM; i++)
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
void UpdateSmallZako1(void)
{
	int i;
	SMALLZAKO1 *ptr = GetSmallZako1Adr(0);
	PLAYER *player = GetPlayerAdr(0);
	ENEMYATTACK param = bulletParam;

	for (i = 0; i < SMALLZAKO1_MAX; i++, ptr++)
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
			AddScore(SMALLZAKO1_SCORE);

			//�����Ԃ��e�𔭐��i��Փx�FHARD�ȏ�j
			D3DXVECTOR3 tmp = player->pos - ptr->pos;
			float distSq = D3DXVec3Length(&tmp);
			if (GetDifficulty() >= DIFFICULTY_HARD && distSq > 100 * 100)
			{
				param.angle = SnipeTarget(&ptr->pos, &player->pos);
				param.pos = ptr->pos;
				FireCircleBullet(&param, SMALLZAKO1_SHOT_WAY, &ptr->hp);
				//SetEnemyBullet(&param, &ptr->hp);
			}
			continue;
		}

		//�A�j���[�V����
		ptr->countAnim++;
		if (ptr->countAnim % SMALLZAKO1_TIME_ANIMATION == 0)
		{
			ptr->patternAnim = (ptr->patternAnim + 1) % SMALLZAKO1_ANIM_NUM;
		}

		//�s������
		ActionSmallZako1(ptr);
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawSmallZako1(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SMALLZAKO1 *ptr = GetSmallZako1Adr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e��`��
	pDevice->SetTexture(0, texture[SHADOW]);
	for (i = 0; i < SMALLZAKO1_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W�A�e�N�X�`�����W��ݒ�
			SetShadowSmallZako1(ptr);

			//�|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}

	//�{�̂�`��
	pDevice->SetTexture(0, texture[BODY]);
	ptr = GetSmallZako1Adr(0);
	for (i = 0; i < SMALLZAKO1_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexSmallZako1(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureSmallZako1(ptr->patternAnim, ptr);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//�����蔻��̕`��
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(SMALLZAKO1_BB_LEFT, SMALLZAKO1_BB_TOP, 0.0f), SMALLZAKO1_BB_WIDTH, SMALLZAKO1_BB_HIGHT);
#endif
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexSmallZako1(void)
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
void SetTextureSmallZako1(int cntPattern, SMALLZAKO1 *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % SMALLZAKO1_TEXTURE_DIVIDE_X;
	int y = cntPattern / SMALLZAKO1_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / SMALLZAKO1_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / SMALLZAKO1_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexSmallZako1(SMALLZAKO1 *ptr)
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
void SetShadowSmallZako1(SMALLZAKO1 *ptr)
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
		vertexWk[i].vtx += SMALLZAKO1_SHADOW_OFFSETPOS;
	}

	//�e�N�X�`�����W��ݒ�
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
//�֐���	�FSMALLZAKO1 *GetSmallZako1Adr(int pno)
//����		�Fint pno�F�擾�������X���[���U�R1�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�X���[���U�R1�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�X���[���U�R1�̃A�h���X�擾�֐�
******************************************************************************/
SMALLZAKO1 *GetSmallZako1Adr(int pno)
{
	if (pno < SMALLZAKO1_MAX)
	{
		return &smallZako1[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�Fvoid SetSmallZako1(D3DXVECTOR3 initPos)
//����		�FD3DXVECTOR3 initPos	�F�o�����W
//�߂�l	�Fvoid
//����		�F�X���[���U�R1�̃Z�b�g�֐�
******************************************************************************/
void SetSmallZako1(D3DXVECTOR3 initPos)
{
	int i;
	SMALLZAKO1 *ptr = GetSmallZako1Adr(0);

	for (i = 0; i < SMALLZAKO1_MAX; i++, ptr++)
	{
		//��A�N�e�B�u�̃X���[���U�R1������΃Z�b�g
		if (!ptr->active)
		{
			//�e�p�����[�^��ݒ�
			ptr->pos = initPos;
			ptr->rot.z = 0.0f;
			ptr->hp = SMALLZAKO1_INITHP;
			ptr->moveDir = VECTOR3_UP * -1;
			ptr->speed = SMALLZAKO1_INITSPEED;
			ptr->countAnim = 0;
			ptr->patternAnim = 0;
			ptr->cntFrame = 0;
			ptr->state = INIT;

			//�Z�b�g�����^�[��
			ptr->active = true;
			return;
		}
	}
}

/******************************************************************************
//�֐���	�Fvoid ActionSmallZako1(SMALLZAKO1 *ptr)
//����		�FSMALLZAKO11 *ptr	�F�s������G�l�~�[�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�X���[���U�R1�̍s������
******************************************************************************/
void ActionSmallZako1(SMALLZAKO1 *ptr)
{
	PLAYER *player = GetPlayerAdr(0);
	float lengthSq = 99999.9f;

	ptr->cntFrame++;

	switch (ptr->state)
	{
	case INIT:
		//�i�s�����Ɍ������Ē��i
		ptr->pos += ptr->moveDir * ptr->speed;

		//��莞�Ԍo�߂��Ă����玟�̏�Ԃ֑J��
		if (ptr->cntFrame > SMALLZAKO1_INIT_FRAME)
		{
			ptr->cntFrame = 0;
			ptr->state = HOMING;
		}
		break;

	case HOMING:
		//�v���C���[����A�N�e�B�u�ł���Ύ��̏�Ԃ֑J��
		if (!player->active)
		{
			ptr->state = NOT_HOMING;
			ptr->cntFrame = 0;
		}

		//�v���C���[�Ƃ̋��������ȉ��Ȃ玟�̏�Ԃ֑J��
		lengthSq = D3DXVec3LengthSq(&(player->pos - ptr->pos));
		if (lengthSq <= SMALLZAKO1_HOMING_LENGTH)
		{
			ptr->state = NOT_HOMING;
			ptr->cntFrame = 0;
		}
		//�����łȂ��Ȃ�v���C���[��ǔ�
		else
		{
			ptr->rot.z += RotateToTarget(&ptr->pos, &player->pos, ptr->rot.z, SMALLZAKO1_HOMING_ANGLE);
			ptr->moveDir = D3DXVECTOR3(cosf(ptr->rot.z + RADIAN(90)), sinf(ptr->rot.z + RADIAN(90)), 0.0f);
		}

	case NOT_HOMING:
		//�i�s�����Ɍ������Ē��i
		ptr->pos += ptr->moveDir * ptr->speed;

		//�X�V��̍��W����ʊO�ł���Δ�A�N�e�B�u��
		if (ptr->pos.x - SMALLZAKO1_TEXTURE_SIZE_X > BG_RIGHT || ptr->pos.x + SMALLZAKO1_TEXTURE_SIZE_X < BG_LEFT
			|| ptr->pos.y - SMALLZAKO1_TEXTURE_SIZE_Y > SCREEN_HEIGHT || ptr->pos.y + SMALLZAKO1_TEXTURE_SIZE_Y < 0.0f)
		{
			ptr->active = false;
		}

		break;
	}
}