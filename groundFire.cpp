//=============================================================================
//
// �O�����h�t�@�C�A���� [groundFire.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "groundFire.h"
#include "bg.h"
#include "bgObj.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexGroundFire(void);					//���_�쐬�֐�
void SetTextureGroundFire(int cntPattern);			// �e�N�X�`�����W�̌v�Z����
void SetVertexGroundFire(GROUNDFIRE *ptr);			// ���_�̌v�Z����

/*****************************************************************************
�\���̒�`
*****************************************************************************/

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;			// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];				// ���_���i�[���[�N
static float radius, angle;							// �|���S���̔��a�A�p�x

GROUNDFIRE groundFire[GROUNDFIRE_MAX];				// �O�����h�t�@�C�A�z��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitGroundFire(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GROUNDFIRE* ptr = GetGroundFireAdr(0);
	int i;

	radius = D3DXVec2Length(&D3DXVECTOR2(GROUNDFIRE_TEXTURE_SIZE_X, GROUNDFIRE_TEXTURE_SIZE_Y));
	angle = atan2f(GROUNDFIRE_TEXTURE_SIZE_Y, GROUNDFIRE_TEXTURE_SIZE_X);

	for (i = 0; i < GROUNDFIRE_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(GROUNDFIRE_INITPOS_X, GROUNDFIRE_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->active = false;
	}

	// ���_���̍쐬
	MakeVertexGroundFire();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)GROUNDFIRE_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitGroundFire(void)
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
void UpdateGroundFire(void)
{
	GROUNDFIRE *ptr = GetGroundFireAdr(0);

	for (int i = 0; i < GROUNDFIRE_MAX; i++, ptr++)
	{
		//��A�N�e�B�u�ł����continue
		if (!ptr->active)
		{
			continue;
		}

		//�A�j���[�V����
		ptr->countAnim++;
		if (ptr->countAnim % GROUNDFIRE_TIME_ANIMATION == 0)
		{
			ptr->patternAnim = WrapAround(0, GROUNDFIRE_ANIM_NUM, ptr->patternAnim + 1);
		}

		//���W�̍X�V
		ptr->pos.y += BGOBJ_SPEED;

		//��ʊO�ɏo�Ă������A�N�e�B�u��
		if (ptr->pos.y > SCREEN_HEIGHT + GROUNDFIRE_TEXTURE_SIZE_Y)
		{
			ptr->active = false;
		}
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawGroundFire(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GROUNDFIRE *ptr = GetGroundFireAdr(0);
	int i;

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	//�O�����h�t�@�C�A��`��
	for (i = 0; i < GROUNDFIRE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexGroundFire(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureGroundFire(ptr->patternAnim);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexGroundFire(void)
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
void SetTextureGroundFire(int cntPattern)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % GROUNDFIRE_TEXTURE_DIVIDE_X;
	int y = cntPattern / GROUNDFIRE_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / GROUNDFIRE_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / GROUNDFIRE_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexGroundFire(GROUNDFIRE *ptr)
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
//�֐���	�FGROUNDFIRE *GetGroundFireAdr(int pno)
//����		�Fint pno�F�擾�������O�����h�t�@�C�A�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�O�����h�t�@�C�A�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�O�����h�t�@�C�A�̃A�h���X�擾�֐�
******************************************************************************/
GROUNDFIRE *GetGroundFireAdr(int pno)
{
	if (pno < GROUNDFIRE_MAX)
	{
		return &groundFire[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�Fvoid SetGroundFire(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
//����1		�FD3DXVECTOR3 pos	�F�Z�b�g���W
//����2		�FD3DXVECTOR3 rot	�F��]��
//�߂�l	�Fvoid
//����		�F�O�����h�t�@�C�A�̃Z�b�g�֐�
******************************************************************************/
void SetGroundFire(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	GROUNDFIRE *ptr = &groundFire[0];

	for (int i = 0; i < GROUNDFIRE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;
		ptr->rot = rot;
		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->active = true;
		return;
	}
}