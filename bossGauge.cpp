//=============================================================================
//
// �{�X�Q�[�W���� [bossGauge.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "bossGauge.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define BOSSGAUGE_OPEN_VALUE				(0.01f)

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexBossGauge(void);				//���_�쐬�֐�
void SetTextureBossGauge(BOSSGAUGE *ptr);		// �e�N�X�`�����W�̌v�Z����
void SetTextureBossGaugeFrame(void);
void SetVertexBossGauge(BOSSGAUGE *ptr);		// ���_�̌v�Z����
void SetVertexBossGaugeFrame(BOSSGAUGE *ptr);		

/*****************************************************************************
�\���̒�`
*****************************************************************************/
enum BOSSGAUGE_TEXTURE
{
	BOSSGAUGE_FRAME,
	BOSSGAUGE_GAUGE
};

enum BOSSGAUGE_STATE
{
	BOSSGAUGE_OPEN,
	BOSSGAUGE_CLOSE,
	BOSSGAUGE_DEFAULT
};
/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N

BOSSGAUGE bossGauge;						//�{�X�Q�[�W�\����

/******************************************************************************
����������
******************************************************************************/
HRESULT InitBossGauge(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BOSSGAUGE* ptr = GetBossGaugeAdr();

	ptr->pos = D3DXVECTOR3(BOSSGAUGE_INITPOS_X, BOSSGAUGE_INITPOS_Y, 0.0f);
	ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	ptr->angle = atan2f(BOSSGAUGE_TEXTURE_SIZE_Y, BOSSGAUGE_TEXTURE_SIZE_X);
	ptr->radius = D3DXVec2Length(&D3DXVECTOR2(BOSSGAUGE_TEXTURE_SIZE_X, BOSSGAUGE_TEXTURE_SIZE_Y));

	ptr->state = BOSSGAUGE_DEFAULT;
	ptr->scale = 0.0f;

	ptr->active = false;

	// ���_���̍쐬
	MakeVertexBossGauge();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)BOSSGAUGE_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitBossGauge(void)
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
void UpdateBossGauge(void)
{
	BOSSGAUGE *ptr = &bossGauge;

	switch(ptr->state)
	{
	case BOSSGAUGE_OPEN:
		ptr->scale = Clampf(0.0f, 1.0f, ptr->scale + BOSSGAUGE_OPEN_VALUE);
		if (ptr->scale == 1.0f)
		{
			ptr->state = BOSSGAUGE_DEFAULT;
		}
		break;

	case BOSSGAUGE_CLOSE:
		ptr->scale = Clampf(0.0f, 1.0f, ptr->scale + BOSSGAUGE_OPEN_VALUE);
		if (ptr->scale == 0.0f)
		{
			ptr->active = false;
		}
		break;
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawBossGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BOSSGAUGE *ptr = GetBossGaugeAdr();

	if (!ptr->active)
	{
		return;
	}

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	//�{�X�Q�[�W��`��
	SetVertexBossGauge(ptr);
	SetTextureBossGauge(ptr);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//�t���[����`��
	SetVertexBossGaugeFrame(ptr);
	SetTextureBossGaugeFrame();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexBossGauge(void)
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
void SetTextureBossGauge(BOSSGAUGE *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = BOSSGAUGE_GAUGE % BOSSGAUGE_TEXTURE_DIVIDE_X;
	int y = BOSSGAUGE_GAUGE / BOSSGAUGE_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / BOSSGAUGE_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / BOSSGAUGE_TEXTURE_DIVIDE_Y;
	
	if (ptr->state == BOSSGAUGE_DEFAULT)
	{
		vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ptr->percent, (float)(y)* sizeY);
		vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ptr->percent, (float)(y)* sizeY + sizeY);
	}
	else
	{
		vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�(�t���[���p)
******************************************************************************/
void SetTextureBossGaugeFrame(void)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = BOSSGAUGE_FRAME % BOSSGAUGE_TEXTURE_DIVIDE_X;
	int y = BOSSGAUGE_FRAME / BOSSGAUGE_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / BOSSGAUGE_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / BOSSGAUGE_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexBossGauge(BOSSGAUGE *ptr)
{
	// ���_���W�̐ݒ�
	if (ptr->state == BOSSGAUGE_DEFAULT)
	{
		vertexWk[0].vtx.x = vertexWk[1].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius;
		vertexWk[0].vtx.y = vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius;
		vertexWk[2].vtx.x = vertexWk[3].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius;
		vertexWk[2].vtx.y = vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius;

		vertexWk[1].vtx.x += BOSSGAUGE_TEXTURE_SIZE_X * 2 * ptr->percent;
		vertexWk[3].vtx.x += BOSSGAUGE_TEXTURE_SIZE_X * 2 * ptr->percent;
	}
	else
	{
		vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
		vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius;
		vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
		vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius;
		vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
		vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius;
		vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
		vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius;
	}

}

/******************************************************************************
���_���W�̐ݒ�(�t���[���p)
******************************************************************************/
void SetVertexBossGaugeFrame(BOSSGAUGE *ptr)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius;
}


/******************************************************************************
//�֐���	�FBOSSGAUGE *GetBossGaugeAdr(int pno)
//����		�Fint pno�F�擾�������{�X�Q�[�W�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�{�X�Q�[�W�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�{�X�Q�[�W�̃A�h���X�擾�֐�
******************************************************************************/
BOSSGAUGE *GetBossGaugeAdr(void)
{
	return &bossGauge;
}

/******************************************************************************
//�֐���	�FBOSSGAUGE *SetBossGauge(void)
//����		�Fvoid
//�߂�l	�F�Z�b�g�����Q�[�W�̃A�h���X
//����		�F�{�X�Q�[�W��set�֐�
******************************************************************************/
BOSSGAUGE *SetBossGauge(void)
{
	BOSSGAUGE *ptr = &bossGauge;

	ptr->active = true;
	ptr->state = BOSSGAUGE_OPEN;
	ptr->scale = 0.0f;
	ptr->percent = 1.0f;

	return ptr;
}