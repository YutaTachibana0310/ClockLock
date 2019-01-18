//=============================================================================
//
// �����`�b�v���� [charChip.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "charChip.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
void SetTextureCharChip(int cntPattern, VERTEX_2D *vertexWk);	// �e�N�X�`�����W�̌v�Z����

/*****************************************************************************
�\���̒�`
*****************************************************************************/

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// �e�N�X�`���ւ̃|�C���^

/******************************************************************************
����������
******************************************************************************/
HRESULT InitCharChip(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)CHARCHIP_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitCharChip(void)
{
	if (texture != NULL)
	{	// �e�N�X�`���̊J��
		texture->Release();
		texture = NULL;
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawCharChip(VERTEX_2D *vertexWk, int charchipID)
{
	if (charchipID < CHARCHIP_A || charchipID >= CHARCHIP_MAX)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	// �e�N�X�`�����W��ݒ�
	SetTextureCharChip(charchipID, vertexWk);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�
******************************************************************************/
void SetTextureCharChip(int cntPattern, VERTEX_2D *vertexWk)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % CHARCHIP_TEXTURE_DIVIDE_X;
	int y = cntPattern / CHARCHIP_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / CHARCHIP_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / CHARCHIP_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}