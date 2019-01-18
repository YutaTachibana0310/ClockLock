//=============================================================================
//
// �v���C���[�������� [playerExplosion.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "playerExplosion.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexPlayerExplosion(void);				//���_�쐬�֐�
void SetTexturePlayerExplosion(int cntPattern, PLAYEREXPLOSION *ptr);	// �e�N�X�`�����W�̌v�Z����
void SetVertexPlayerExplosion(PLAYEREXPLOSION *ptr);					// ���_�̌v�Z����

/*****************************************************************************
�\���̒�`
*****************************************************************************/

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture = NULL;				// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D		vertexWk[NUM_VERTEX];					//���_���i�[���[�N

PLAYEREXPLOSION playerExplosion[PLAYEREXPLOSION_MAX];				//�v���C���[�����z��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitPlayerExplosion(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYEREXPLOSION* ptr = GetPlayerExplosionAdr(0);
	int i;

	for (i = 0; i < PLAYEREXPLOSION_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(PLAYEREXPLOSION_INITPOS_X, PLAYEREXPLOSION_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(PLAYEREXPLOSION_TEXTURE_SIZE_Y, PLAYEREXPLOSION_TEXTURE_SIZE_X);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(PLAYEREXPLOSION_TEXTURE_SIZE_X, PLAYEREXPLOSION_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->active = false;
	}

	// ���_���̍쐬
	MakeVertexPlayerExplosion();

	if(num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)PLAYEREXPLOSION_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitPlayerExplosion(void)
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
void UpdatePlayerExplosion(void)
{
	int i;
	PLAYEREXPLOSION *ptr = GetPlayerExplosionAdr(0);

	for (i = 0; i < PLAYEREXPLOSION_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//�A�j���[�V����
		ptr->countAnim++;
		if (ptr->countAnim % PLAYEREXPLOSION_TIME_ANIMATION == 0)
		{
			ptr->patternAnim = (ptr->patternAnim + 1) % PLAYEREXPLOSION_ANIM_NUM;
		}

		//�Ƃ肠������A�N�e�B�u����
		if (ptr->countAnim > 32)
		{
			ptr->active = false;
		}
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawPlayerExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYEREXPLOSION *ptr = GetPlayerExplosionAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	//�v���C���[������`��
	for (i = 0; i < PLAYEREXPLOSION_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexPlayerExplosion(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTexturePlayerExplosion(ptr->patternAnim, ptr);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexPlayerExplosion(void)
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
void SetTexturePlayerExplosion(int cntPattern, PLAYEREXPLOSION *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % PLAYEREXPLOSION_TEXTURE_DIVIDE_X;
	int y = cntPattern / PLAYEREXPLOSION_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / PLAYEREXPLOSION_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / PLAYEREXPLOSION_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexPlayerExplosion(PLAYEREXPLOSION *ptr)
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
//�֐���	�FPLAYEREXPLOSION *GetPlayerExplosionAdr(int pno)
//����		�Fint pno�F�擾�������v���C���[�����̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�v���C���[�����̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�v���C���[�����̃A�h���X�擾�֐�
******************************************************************************/
PLAYEREXPLOSION *GetPlayerExplosionAdr(int pno)
{
	if (pno < PLAYEREXPLOSION_MAX)
	{
		return &playerExplosion[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�Fvoid SetPlayerExplosion(D3DXVECTOR3 pos)
//����		�FD3DXVECTOR3 pos		�F�Z�b�g���������W
//�߂�l	�Fvoid
//����		�F�v���C���[�����̃Z�b�g�֐�
******************************************************************************/
void SetPlayerExplosion(D3DXVECTOR3 pos)
{
	int i;
	PLAYEREXPLOSION *ptr = GetPlayerExplosionAdr(0);

	for (i = 0; i < PLAYEREXPLOSION_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;
		ptr->patternAnim = 0;
		ptr->countAnim = 0;
		ptr->active = true;
		return;
	}
}