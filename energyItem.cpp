//=============================================================================
//
// �G�i�W�[�A�C�e������ [energyItem.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "energyItem.h"
#include "bg.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define ENERGYITEM_SPEED			(2.5f)

#define ENERGYITEM_SHADOW_OFFSET	(D3DXVECTOR3(50.0f, 100.0f, 0.0f))
/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexEnergyItem(void);					//���_�쐬�֐�
void SetTextureEnergyItem(int cntPattern);			// �e�N�X�`�����W�̌v�Z����
void SetVertexEnergyItem(ENERGYITEM *ptr);			// ���_�̌v�Z����
void SetShadowEnergyItem(ENERGYITEM *ptr);			//�e�̒��_�ݒ�

/*****************************************************************************
�\���̒�`
*****************************************************************************/
enum ENERGYITEM_TEX
{
	ENERGYITEM_SHADOW,
	ENERGYITEM_BODY,
	ENERGYITEM_TEXMAX
};
/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[ENERGYITEM_TEXMAX];	// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N
static float angle, radius;								//�|���S���̔��a�A�p�x

ENERGYITEM energyItem[ENERGYITEM_MAX];					//�G�i�W�[�A�C�e���z��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitEnergyItem(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENERGYITEM* ptr = GetEnergyItemAdr(0);
	int i;


	angle = atan2f(ENERGYITEM_TEXTURE_SIZE_Y, ENERGYITEM_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(ENERGYITEM_TEXTURE_SIZE_X, ENERGYITEM_TEXTURE_SIZE_Y));

	for (i = 0; i < ENERGYITEM_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(ENERGYITEM_INITPOS_X, ENERGYITEM_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->state = ENERGYITEM_DEFAULT;

		ptr->active = false;
	}

	// ���_���̍쐬
	MakeVertexEnergyItem();

	if(num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[ENERGYITEM_SHADOW] = CreateTextureFromFile((LPSTR)ENERGYITEM_SHADOWTEX_NAME, pDevice);
		texture[ENERGYITEM_BODY] = CreateTextureFromFile((LPSTR)ENERGYITEM_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitEnergyItem(void)
{
	for (int i = 0; i < ENERGYITEM_TEXMAX; i++)
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
void UpdateEnergyItem(void)
{
	ENERGYITEM *ptr = GetEnergyItemAdr(0);

	for (int i = 0; i < ENERGYITEM_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//���W���X�V
		ptr->pos += ptr->moveDir * ENERGYITEM_SPEED;

		//��ʊO����
		if (ptr->pos.x > BG_RIGHT + ENERGYITEM_TEXTURE_SIZE_X || ptr->pos.x < BG_LEFT + ENERGYITEM_TEXTURE_SIZE_X)
		{
			ptr->active = false;
		}
		else if (ptr->pos.y > SCREEN_HEIGHT + ENERGYITEM_TEXTURE_SIZE_Y || ptr->pos.y < -ENERGYITEM_TEXTURE_SIZE_Y)
		{
			ptr->active = false;
		}

		//�A�j���[�V����
		ptr->countAnim++;
		if (ptr->countAnim % ENERGYITEM_TIME_ANIMATION == 0)
		{
			ptr->patternAnim = WrapAround(0, ENERGYITEM_ANIM_NUM, ptr->patternAnim + 1);
		}
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawEnergyItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENERGYITEM *ptr = GetEnergyItemAdr(0);
	int i;

	// �e��`��
	pDevice->SetTexture(0, texture[ENERGYITEM_SHADOW]);
	for (int i = 0; i < ENERGYITEM_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			SetShadowEnergyItem(ptr);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}

	//�G�i�W�[�A�C�e����`��
	pDevice->SetTexture(0, texture[ENERGYITEM_BODY]);
	ptr = GetEnergyItemAdr(0);
	for (i = 0; i < ENERGYITEM_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexEnergyItem(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureEnergyItem(ptr->patternAnim);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//�����蔻��̕`��
			DrawBoxCollider(ptr->pos + D3DXVECTOR3(ENERGYITEM_BB_LEFT, ENERGYITEM_BB_TOP, 0.0f), ENERGYITEM_BB_WIDTH, ENERGYITEM_BB_HIGHT);
#endif
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexEnergyItem(void)
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
void SetTextureEnergyItem(int cntPattern)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % ENERGYITEM_TEXTURE_DIVIDE_X;
	int y = cntPattern / ENERGYITEM_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / ENERGYITEM_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / ENERGYITEM_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexEnergyItem(ENERGYITEM *ptr)
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
�e�̒��_�̐ݒ�
******************************************************************************/
void SetShadowEnergyItem(ENERGYITEM *ptr)
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

	//�I�t�Z�b�g
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertexWk[i].vtx += ENERGYITEM_SHADOW_OFFSET;
	}

	//�e�N�X�`���ݒ�
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
//�֐���	�FENERGYITEM *GetEnergyItemAdr(int pno)
//����		�Fint pno�F�擾�������G�i�W�[�A�C�e���̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�G�i�W�[�A�C�e���̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�G�i�W�[�A�C�e���̃A�h���X�擾�֐�
******************************************************************************/
ENERGYITEM *GetEnergyItemAdr(int pno)
{
	if (pno < ENERGYITEM_MAX)
	{
		return &energyItem[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�Fvoid SetEnergyItem(D3DXVECTOR3 pos)
//����		�FD3DXVECTOR3 pos		�F�o�����W
//�߂�l	�Fvoid
//����		�F�G�i�W�[�A�C�e���̃Z�b�g�֐�
******************************************************************************/
void SetEnergyItem(D3DXVECTOR3 pos)
{
	ENERGYITEM *ptr = GetEnergyItemAdr(0);

	for (int i = 0; i < ENERGYITEM_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;
		ptr->moveDir = -VECTOR3_UP;
		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->active = true;
		return;
	}
}