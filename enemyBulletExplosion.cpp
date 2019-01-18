//=============================================================================
//
// �G�l�~�[�o���b�g�G�N�X�v���[�W�������� [enemyBulletExplosion.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "enemyBullet.h"
#include "enemyBulletExplosion.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define EBULLETEXPLOSION_SPEED (-40.0f)

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexEnemyBulletExplosion(void);				//���_�쐬�֐�
void SetTextureEnemyBulletExplosion(EBULLETEXPLOSION *ptr);	// �e�N�X�`�����W�̌v�Z����
void SetVertexEnemyBulletExplosion(EBULLETEXPLOSION *ptr);					// ���_�̌v�Z����

/*****************************************************************************
�\���̒�`
*****************************************************************************/
enum EBULLETEXPLOSION_TEXTYPE
{
	EXPLOSION_GREEN,
	EXPLOSION_ORANGE
};
/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N

EBULLETEXPLOSION enemyBulletExplosion[EBULLETEXPLOSION_MAX];				//�G�l�~�[�o���b�g�G�N�X�v���[�W�����z��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitEnemyBulletExplosion(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EBULLETEXPLOSION* ptr = GetEnemyBulletExplosionAdr(0);
	int i;

	for (i = 0; i < EBULLETEXPLOSION_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(EBULLETEXPLOSION_INITPOS_X, EBULLETEXPLOSION_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(EBULLETEXPLOSION_TEXTURE_SIZE_Y, EBULLETEXPLOSION_TEXTURE_SIZE_X);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(EBULLETEXPLOSION_TEXTURE_SIZE_X, EBULLETEXPLOSION_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->type = (i < EBULLETEXPLOSION_MAX / 2) ? EXPLOSION_GREEN : EXPLOSION_ORANGE;

		ptr->active = false;
	}

	// ���_���̍쐬
	MakeVertexEnemyBulletExplosion();

	if(num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)EBULLETEXPLOSION_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitEnemyBulletExplosion(void)
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
void UpdateEnemyBulletExplosion(void)
{
	EBULLETEXPLOSION *ptr = &enemyBulletExplosion[0];

	for (int i = 0; i < EBULLETEXPLOSION_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//�A�j���[�V����
		if (++ptr->countAnim % EBULLETEXPLOSION_TIME_ANIMATION == 0)
		{
			ptr->patternAnim++;
			if (ptr->patternAnim == EBULLETEXPLOSION_ANIM_NUM)
			{
				ptr->active = false;
			}
		}
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawEnemyBulletExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EBULLETEXPLOSION *ptr = GetEnemyBulletExplosionAdr(0);
	int i;

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	//�G�l�~�[�o���b�g�G�N�X�v���[�W���������Z�����ŕ`��
	for (i = 0; i < EBULLETEXPLOSION_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexEnemyBulletExplosion(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureEnemyBulletExplosion(ptr);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexEnemyBulletExplosion(void)
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
void SetTextureEnemyBulletExplosion(EBULLETEXPLOSION *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = ptr->patternAnim % EBULLETEXPLOSION_TEXTURE_DIVIDE_X;
	int y = ptr->type;
	float sizeX = 1.0f / EBULLETEXPLOSION_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / EBULLETEXPLOSION_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexEnemyBulletExplosion(EBULLETEXPLOSION *ptr)
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
//�֐���	�FEBULLETEXPLOSION *GetEnemyBulletExplosionAdr(int pno)
//����		�Fint pno�F�擾�������G�l�~�[�o���b�g�G�N�X�v���[�W�����̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�G�l�~�[�o���b�g�G�N�X�v���[�W�����̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�G�l�~�[�o���b�g�G�N�X�v���[�W�����̃A�h���X�擾�֐�
******************************************************************************/
EBULLETEXPLOSION *GetEnemyBulletExplosionAdr(int pno)
{
	if (pno < EBULLETEXPLOSION_MAX)
	{
		return &enemyBulletExplosion[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�Fvoid SetEnemyBulletExplosion(D3DXVECTOR3 pos, BULLET_TYPE type)
//����1		�FD3DXVECTOR3 pos	�F�Z�b�g���W
//����2		�FBULLET_TYPE type	�F��������o���b�g�̃^�C�v
//�߂�l	�F����Ȉ����F�G�l�~�[�o���b�g�G�N�X�v���[�W�����̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�G�l�~�[�o���b�g�G�N�X�v���[�W�����̃A�h���X�擾�֐�
******************************************************************************/
void SetEnemyBulletExplosion(D3DXVECTOR3 pos, BULLET_TYPE type)
{
	EBULLETEXPLOSION *ptr = &enemyBulletExplosion[0];

	for (int i = 0; i < EBULLETEXPLOSION_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;
		ptr->type = type / ORANGE_ROTATE;
		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->active = true;
		return;
	}
}
