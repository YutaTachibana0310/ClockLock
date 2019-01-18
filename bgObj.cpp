//=============================================================================
//
// ���� [bgObj.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "bg.h"
#include "bgObj.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexBgObj(void);						//���_�쐬�֐�
void SetTextureBgObj(int cntPattern, BGOBJ *ptr);	// �e�N�X�`�����W�̌v�Z����
void SetVertexBgObj(BGOBJ *ptr);					// ���_�̌v�Z����

/*****************************************************************************
�\���̒�`
*****************************************************************************/

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N

BGOBJ bgObj[BGOBJ_MAX];									//BG�I�u�W�F�z��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitBgObj(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BGOBJ* ptr = GetBgObjAdr(0);
	int i;

	for (i = 0; i < BGOBJ_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(BGOBJ_INITPOS_X, BGOBJ_INITPOS_Y - BGOBJ_TEXTURE_SIZE_Y * (i + 1), 0.0f);

		ptr->patternAnim = i;
	}

	// ���_���̍쐬
	MakeVertexBgObj();

	if(num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)BGOBJ_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitBgObj(void)
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
void UpdateBgObj(void)
{
	int i;
	BGOBJ *ptr = GetBgObjAdr(0);

	for (i = 0; i < BGOBJ_MAX; i++, ptr++)
	{
		ptr->pos.y += BGOBJ_SPEED;
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawBgObj(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BGOBJ *ptr = GetBgObjAdr(0);
	int i;
	bool isRendered = false;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	//BG�I�u�W�F��`��
	for (i = 0; i < BGOBJ_MAX; i++, ptr++)
	{
		//��ʂɉf��Ȃ��I�u�W�F�ł���Ε`����s��Ȃ�
		isRendered = ptr->pos.y < SCREEN_HEIGHT || ptr->pos.y > -SCREEN_HEIGHT;
		if (isRendered)
		{
			//���_���W��ݒ�
			SetVertexBgObj(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureBgObj(ptr->patternAnim, ptr);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexBgObj(void)
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
void SetTextureBgObj(int cntPattern, BGOBJ *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % BGOBJ_TEXTURE_DIVIDE_X;
	int y = cntPattern / BGOBJ_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / BGOBJ_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / BGOBJ_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexBgObj(BGOBJ *ptr)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(ptr->pos.x, ptr->pos.y, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(ptr->pos.x + BGOBJ_TEXTURE_SIZE_X, ptr->pos.y, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(ptr->pos.x, ptr->pos.y + BGOBJ_TEXTURE_SIZE_Y, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(ptr->pos.x + BGOBJ_TEXTURE_SIZE_X, ptr->pos.y + BGOBJ_TEXTURE_SIZE_Y, 0.0f);
}

/******************************************************************************
//�֐���	�FBGOBJ *GetBgObjAdr(int pno)
//����		�Fint pno�F�擾������BG�I�u�W�F�̃C���f�b�N�X
//�߂�l	�F����Ȉ����FBG�I�u�W�F�̃A�h���X	�ُ�Ȉ����FNULL
//����		�FBG�I�u�W�F�̃A�h���X�擾�֐�
******************************************************************************/
BGOBJ *GetBgObjAdr(int pno)
{
	if (pno < BGOBJ_MAX)
	{
		return &bgObj[pno];
	}
	else
	{
		return NULL;
	}
}