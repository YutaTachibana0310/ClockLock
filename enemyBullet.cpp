//=============================================================================
//
// �G�l�~�[�o���b�g���� [enemyBullet.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "enemyBullet.h"
#include "bg.h"
#include "myLibrary.h"
#include "scoreItem.h"
#include "EnemyBulletExplosion.h"

#ifdef _DEBUG
#include "player.h"
#endif

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define ENEMYBULLET_MAGNI		((ptr->type < ORANGE_ROTATE) ? magniGreen : magniOrange)	//�ΐF�ƃI�����W�F�ő��x�{�����g��������}�N��
#define ENEMYBULLET_SLOW		(0.1f)							//�x���Ƃ��̑��x�{��
#define ENEMYBULLET_FAST		(1.0f)							//�����Ƃ��̑��x�{��
#define ENEMYBULLET_SLOWBOTH	(0.0f)							//�����x���Ƃ��̑��x�{��

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexEnemyBullet(void);							// ���_�쐬�֐�
void SetTextureEnemyBullet(int cntPattern, ENEMYBULLET *ptr);	// �e�N�X�`�����W�̌v�Z����
void SetVertexEnemyBullet(ENEMYBULLET *ptr);					// ���_�̌v�Z����
void DecideSpeedMagni(float *magniO, float *magniG);			// ���x�{���̌���֐�


/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture[ENEMYBULLET_NUM_TEXTURE];			// �G�l�~�[�o���b�g�̃e�N�X�`��

ENEMYBULLET				bullet[ENEMYBULLET_KIND_MAX][ENEMYBULLET_NUM_MAX];	//�G�l�~�[�o���b�g�z��

static VERTEX_2D		vertexWk[NUM_VERTEX];								//���_���i�[���[�N
SPEEDTYPE				speedType;											//�G�l�~�[�o���b�g�̑��x�̃��[�h

static int interbal = 0;

#ifdef _DEBUG
static int cntBullet;														//�A�N�e�B�u�ȃG�l�~�[�o���b�g�̐�
#endif

/******************************************************************************
����������
******************************************************************************/
HRESULT InitEnemyBullet(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMYBULLET* ptr = GetEnemyBulletAdr(GREEN_ROTATE, 0);
	int i;
	float x = ENEMYBULLET_TEXTURE_SIZE_X / 2.0f;
	float y = ENEMYBULLET_TEXTURE_SIZE_Y / 2.0f;

#ifdef _DEBUG
	cntBullet = 0;
#endif // _DEBUG

	for (i = 0; i < ENEMYBULLET_KIND_MAX*ENEMYBULLET_NUM_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(ENEMYBULLET_INITPOS_X, ENEMYBULLET_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ptr->angle = atan2f(y, x);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(x, y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->type = (BULLET_TYPE)(i / ENEMYBULLET_NUM_MAX);
		ptr->velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->active = false;
	}
	// ���_���̍쐬
	MakeVertexEnemyBullet();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[0] = CreateTextureFromFile((LPSTR)ENEMYBULLET_TEXTURE_MAIN, pDevice);
		texture[1] = CreateTextureFromFile((LPSTR)ENEMYBULLET_TEXTURE_LUMINE, pDevice);
	}
	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitEnemyBullet(void)
{
	if (texture[0] != NULL)
	{	// �e�N�X�`���̊J��
		texture[0]->Release();
		texture[0] = NULL;
	}

	if (texture[1] != NULL)
	{
		texture[1]->Release();
		texture[1] = NULL;
	}
}

/******************************************************************************
�X�V����
******************************************************************************/
void UpdateEnemyBullet(void)
{
	ENEMYBULLET *ptr = GetEnemyBulletAdr(GREEN_ROTATE, 0);
	int i;
	float magniOrange, magniGreen;							//���ꂼ��̒e�̑��x�{��^

	DecideSpeedMagni(&magniOrange, &magniGreen);			//�e�̑��x�{��������

#ifdef _DEBUG
	
	PLAYER *player = GetPlayerAdr(0);
	if (speedType != SLOW_BOTH)
	{
		interbal++;
	}
	if (interbal % 30 == 0)
	{
		ENEMYATTACK param1 = { GREEN_ROTATE, D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), 5.0f, RADIAN(10) };
		ENEMYATTACK param2 = { ORANGE_ROTATE, D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), 5.0f, RADIAN(32.5f) };
		//param1.angle = SnipeTarget(&D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), &(player->pos));
		//param2.angle = SnipeTarget(&D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), &(player->pos));
		//SetEnemyBullet(&param1);
		//FireWayBullet(&param1, 3, 120.0f);
		//FireCircleBullet(&param1, 8);
		//FireCircleBullet(&param2, 8);
	}
	
#endif

	for (i = 0; i < ENEMYBULLET_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//�e������ł��邩�ǂ���
			if (ptr->parentHp != NULL && ptr->type < ORANGE_ROTATE)
			{
				if (*(ptr->parentHp) <= 0)
				{
					if (speedType == SLOW_GREEN)
					{
						DisableEnemyBullet(ptr);
						SetScoreItem(ptr->pos, 10.0f, 1);
					}
					else
					{
						ptr->parentHp = NULL;
					}
				}
			}

			//���W�̍X�V
			ptr->pos += ptr->velocity * ENEMYBULLET_MAGNI;
			if (ptr->pos.y < -ENEMYBULLET_TEXTURE_SIZE_Y || ptr->pos.y > SCREEN_HEIGHT)
			{
#ifdef _DEBUG
				if (ptr->active)
				{
					cntBullet--;
				}
#endif
				ptr->active = false;
				ptr->parentHp = NULL;
			}
			else if (ptr->pos.x < BG_POS_X - ENEMYBULLET_TEXTURE_SIZE_X / 2 || ptr->pos.x > BG_POS_X + BG_TEXTURESIZE_X + ENEMYBULLET_BB_WIDTH / 2)
			{
#ifdef _DEBUG
				if (ptr->active)
				{
					cntBullet--;
				}
#endif
				ptr->active = false;
				ptr->parentHp = NULL;
			}

			// �A�j���[�V����
			ptr->countAnim++;

			// �A�j���[�V����Wait�`�F�b�N
			if (ptr->countAnim % TIMEENEMYBULLET_ANIMATION == 0)
			{
				// �p�^�[���̐؂�ւ�
				ptr->patternAnim = (ptr->patternAnim + 1) % ANIM_ENEMYBULLET_NUM;
			}
		}
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawEnemyBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMYBULLET *ptr = GetEnemyBulletAdr(GREEN_ROTATE, 0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�G�l�~�[�o���b�g�̌���`��
	pDevice->SetTexture(0, texture[1]);
	for (i = 0; i < ENEMYBULLET_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexEnemyBullet(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureEnemyBullet(ptr->patternAnim, ptr);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}

	//�G�l�~�[�o���b�g�{�̂�`��
	ptr = GetEnemyBulletAdr(GREEN_ROTATE, 0);
	pDevice->SetTexture(0, texture[0]);
	for (i = GREEN_ROTATE; i < ENEMYBULLET_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexEnemyBullet(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureEnemyBullet(ptr->patternAnim, ptr);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//�����蔻��̕`��
			//DrawBoxCollider(ptr->pos, ENEMYBULLET_BB_WIDTH, ENEMYBULLET_BB_HIGHT);
#endif
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexEnemyBullet()
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
void SetTextureEnemyBullet(int cntPattern, ENEMYBULLET *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % ENEMYBULLET_TEXTURE_DIVIDE_X;
	int y = ptr->type;
	float sizeX = 1.0f / ENEMYBULLET_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / ENEMYBULLET_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexEnemyBullet(ENEMYBULLET *ptr)
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
//�֐���	�FENEMYBULLET *GetEnemyBulletAdr(BULLET_TYPE type, int pno) {
//����1		�FBULLET_TYPE type	�F�擾�������o���b�g�̃^�C�v
//����2		�Fint pno			�F�擾�������G�l�~�[�o���b�g�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�G�l�~�[�o���b�g�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�G�l�~�[�o���b�g�̃A�h���X�擾�֐�
******************************************************************************/
ENEMYBULLET *GetEnemyBulletAdr(BULLET_TYPE type, int pno) {
	if (pno < ENEMYBULLET_NUM_MAX && type < ENEMYBULLET_KIND_MAX)
	{
		return &bullet[type][pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�FENEMYBULLET *SetEnemyBullet(BULLET_PARAM *param)
//����1		�FBULLET_PARAM *param	�F���˂������o���b�g�ւ̍U���p�����[�^
//�߂�l	�F�Z�b�g�������F�G�l�~�[�o���b�g�̃A�h���X	�Z�b�g�ُ펞�FNULL
//����		�F�G�l�~�[�o���b�g���Q�[�����ɃZ�b�g����֐�
******************************************************************************/
ENEMYBULLET *SetEnemyBullet(ENEMYATTACK *param, int *parentHp)
{
	int i;
	ENEMYBULLET *ptr = GetEnemyBulletAdr(param->type, 0);
	float angle = param->angle;

	for (i = 0; i < ENEMYBULLET_NUM_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			ptr->pos = param->pos;
			ptr->velocity = param->speed * D3DXVECTOR3(cosf(angle), sinf(angle), 0.0f);
			ptr->rot.z = angle + ENEMYBULLET_DAFAULT_ANGLE;
			ptr->active = true;
			ptr->countAnim = 0;
			ptr->patternAnim = 0;
			ptr->parentHp = parentHp;
#ifdef _DEBUG
			cntBullet++;
#endif
			return ptr;
		}
	}
	return NULL;
}

/******************************************************************************
//�֐���	�Fvoid SetSpeedType(SPEEDTYPE type)
//����1		�FSPEEDTYPE type	�F�Z�b�g�������X�s�[�h�^�C�v
//�߂�l	�Fvoid
//����		�F�X�s�[�h�^�C�v���Z�b�g����֐�
******************************************************************************/
void SetSpeedType(SPEEDTYPE type)
{
	speedType = type;
}

/******************************************************************************
//�֐���	�Fvoid GetSpeedType(void)
//����1		�Fvoid
//�߂�l	�Fvoid
//����		�F�X�s�[�h�^�C�v���擾����֐�
******************************************************************************/
SPEEDTYPE GetSpeedType(void)
{
	return speedType;
}

/******************************************************************************
//�֐���	�Fvoid DecideSpeedMode(int *magniO, int *magniG)
//����1		�Fint *magniO	�F�I�����W�̒e�̑��x�{�����i�[����|�C���^
//����2		�Fint *magniG	�F�΂̒e�̑��x�{�����i�[����|�C���^
//�߂�l	�Fvoid
//����		�F�G�l�~�[�o���b�g�̑��x�{�������肷��֐�
******************************************************************************/
void DecideSpeedMagni(float *magniO, float *magniG)
{
	//�I�����W���x���A�΂�����
	if (speedType == SLOW_ORANGE)
	{
		*magniO = ENEMYBULLET_SLOW;
		*magniG = ENEMYBULLET_FAST;
	}
	//�I�����W�������A�΂��x��
	else if (speedType == SLOW_GREEN)
	{
		*magniO = ENEMYBULLET_FAST;
		*magniG = ENEMYBULLET_SLOW;
	}
	//�����x��
	else if (speedType == SLOW_BOTH)
	{
		*magniO = ENEMYBULLET_SLOWBOTH;
		*magniG = ENEMYBULLET_SLOWBOTH;
	}
	//��������
	else if (speedType == FAST_BOTH)
	{
		*magniO = ENEMYBULLET_FAST;
		*magniG = ENEMYBULLET_FAST;
	}
	//�f�t�H���g
	else
	{
		*magniO = 1.0f;
		*magniG = 1.0f;
	}
}

/******************************************************************************
//�֐���	�Fvoid FireCircleBullet(BULLET_PARAM *param, int way)
//����1		�FBULLET_PARAM *param	�F�U���̃p�����[�^���ۑ����ꂽ�\���̂ւ̃|�C���^
//����2		�Fint way				�Fway��
//�߂�l	�Fvoid
//����		�F�~�`�ɉ����Ēe��way�����֐�
******************************************************************************/
void FireCircleBullet(ENEMYATTACK *param, int way, int *parentHp)
{
	int i;
	float tmpAngl = param->angle;

	for (i = 0; i < 360 / way; i++)
	{
		SetEnemyBullet(param, parentHp);
		param->angle += RADIAN(360.0f / way);
	}
	param->angle = tmpAngl;
}

/******************************************************************************
//�֐���	�Fvoid FireWayBullet(BULLET_PARAM *param, int way, float range)
//����1		�FBULLET_PARAM *param	�F�U���̃p�����[�^���ۑ����ꂽ�\���̂ւ̃|�C���^
//����2		�Fint way				�Fway��
//����3		�Ffloat range			�FWAY�e���L����p�x
//�߂�l	�Fvoid
//����		�F���ɒe��way�����֐�
******************************************************************************/
void FireWayBullet(ENEMYATTACK *param, int way, float range, int *parentHp)
{
	int i;
	float tmpAngl = param->angle;

	param->angle -= RADIAN(range / 2.0f);

	for (i = 0; i < way; i++)
	{
		SetEnemyBullet(param, parentHp);
		param->angle += RADIAN(range / (way - 1));
	}
	param->angle = tmpAngl;
}

/******************************************************************************
//�֐���	�Fvoid DisableEnemyBullet(ENEMYBULLET *ptr)
//����1		�FENEMYBULLET *ptr	�F��A�N�e�B�u�ɂ������G�l�~�[�o���b�g�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�w�肵���G�l�~�[�o���b�g���A�N�e�B�u�ɂ���֐�
******************************************************************************/
void DisableEnemyBullet(ENEMYBULLET *ptr)
{
#ifdef _DEBUG
	if (ptr->active)
	{
		cntBullet--;
	}
#endif

	ptr->active = false;
	ptr->parentHp = NULL;
	//�G�t�F�N�g���Z�b�g
	SetEnemyBulletExplosion(ptr->pos, ptr->type);

}

/******************************************************************************
//�֐���	�Fvoid DisableAllEnemyBullet(ENEMYBULLET *ptr)
//����1		�Fvoid
//�߂�l	�Fvoid
//����		�F�S�ẴG�l�~�[�o���b�g���A�N�e�B�u�ɂ���֐�
******************************************************************************/
void DisableAllEnemyBullet(void)
{
	int i;
	ENEMYBULLET *ptr = GetEnemyBulletAdr((BULLET_TYPE)0, 0);
	for (i = 0; i < ENEMYBULLET_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			DisableEnemyBullet(ptr);
		}
	}
}

/******************************************************************************
//�֐���	�Fvoid ConvertBulletToScoreItem(void)
//����1		�Fvoid
//�߂�l	�Fvoid
//����		�F�S�ẴG�l�~�[�o���b�g���X�R�A�A�C�e���ɕϊ�����֐�
******************************************************************************/
void ConvertBulletToScoreItem(void)
{
	int i;
	ENEMYBULLET *ptr = GetEnemyBulletAdr((BULLET_TYPE)0, 0);
	for (i = 0; i < ENEMYBULLET_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			DisableEnemyBullet(ptr);
			SetScoreItem(ptr->pos, 10.0f, 1);
		}
	}
}

#ifdef _DEBUG
/******************************************************************************
//�֐���	�Fint GetBulletCnt
//����1		�Fint		�F�Q�[�����ŃA�N�e�B�u�ȃG�l�~�[�o���b�g�̐�
//�߂�l	�Fvoid
//����		�F�A�N�e�B�u�ȃG�l�~�[�o���b�g�̐����擾����֐�
******************************************************************************/
int GetEnemyBulletCnt(void)
{
	return cntBullet;
}
#endif