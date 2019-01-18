//=============================================================================
//
// �A�C�e���L�����A���� [itemCareer.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "enemyManager.h"
#include "energyItem.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define ITEMCAREER_SPEED			(7.0f)
#define ITEMCAREER_WAIT_FRAME		(240)
#define ITEMCAREER_ESCAPE_FRAME		(180)

#define ITEMCAREER_INITHP			(50)
#define ITEMCAREER_SCORE			(500)

#define ITEMCAREER_TARGETPOS_OFFSET	(D3DXVECTOR3(0.0f, 200.0f, 0.0f))
#define ITEMCAREER_SHADOW_OFFSETPOS	(D3DXVECTOR3(50.0f, 100.0f, 0.0f))
/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexItemCareer(void);				// ���_�쐬�֐�
void SetTextureItemCareer(int cntPattern);		// �e�N�X�`�����W�̌v�Z����
void SetVertexItemCareer(ITEMCAREER *ptr);		// ���_�̌v�Z����
void SetVertexItemCareerShadow(ITEMCAREER *ptr);

//�e�e�N�X�`���`�揈��
void DrawItemCareerShadow(LPDIRECT3DDEVICE9 pDevice);
void DrawItemCareerBody(LPDIRECT3DDEVICE9 pDevice);

//�X�V����
void UpdateItemCareerInit(ITEMCAREER *ptr);
void UpdateItemCareerWait(ITEMCAREER *ptr);
void UpdateItemCareerEscape(ITEMCAREER *ptr);

/*****************************************************************************
�\���̒�`
*****************************************************************************/
enum ITEMCAREER_TEX
{
	ITEMCAREER_SHADOW,
	ITEMCAREER_BODY,
	ITEMCAREER_TEXMAX
};

enum ITEMCAREER_STATE
{
	ITEMCAREER_INIT,
	ITEMCAREER_WAIT,
	ITEMCAREER_ESCAPE
};
/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[ITEMCAREER_TEXMAX];	// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N
static float angle, radius;								//�|���S�����a�A�p�x

ITEMCAREER itemCareer[ITEMCAREER_MAX];					//�A�C�e���L�����A�z��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitItemCareer(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ITEMCAREER* ptr = GetItemCareerAdr(0);
	int i;

	angle = atan2f(ITEMCAREER_TEXTURE_SIZE_Y, ITEMCAREER_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(ITEMCAREER_TEXTURE_SIZE_X, ITEMCAREER_TEXTURE_SIZE_Y));

	for (i = 0; i < ITEMCAREER_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(ITEMCAREER_INITPOS_X, ITEMCAREER_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->active = false;
	}

	// ���_���̍쐬
	MakeVertexItemCareer();

	if(num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[ITEMCAREER_SHADOW] = CreateTextureFromFile((LPSTR)ITEMCAREER_SHADOWTEX_NAME, pDevice);
		texture[ITEMCAREER_BODY] = CreateTextureFromFile((LPSTR)ITEMCAREER_TEXTURE_NAME, pDevice);
	}

	//SetItemCareer(D3DXVECTOR3(SCREEN_CENTER_X, 0.0f, 0.0f));

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitItemCareer(void)
{
	for (int i = 0; i < ITEMCAREER_TEXMAX; i++)
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
void UpdateItemCareer(void)
{
	ITEMCAREER *ptr = GetItemCareerAdr(0);

	for (int i = 0; i < ITEMCAREER_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		ptr->cntFrame++;

		//���ď���
		if (ptr->hp <= 0)
		{
			SetEnergyItem(ptr->pos);
			AddScore(ITEMCAREER_SCORE);
			ExplodeMiddleEnemy(ptr->pos);
			ptr->active = false;
			continue;
		}

		//�A�j���[�V����
		ptr->countAnim++;
		if (ptr->countAnim % ITEMCAREER_TIME_ANIMATION == 0)
		{
			ptr->patternAnim = WrapAround(0, ITEMCAREER_ANIM_NUM, ptr->patternAnim + 1);
		}

		//�s������
		switch (ptr->state)
		{
		case ITEMCAREER_INIT:
			UpdateItemCareerInit(ptr);
			break;

		case ITEMCAREER_WAIT:
			UpdateItemCareerWait(ptr);
			break;

		case ITEMCAREER_ESCAPE:
			UpdateItemCareerEscape(ptr);
			break;
		}
	}
}

/******************************************************************************
�X�V����(INIT���)
******************************************************************************/
void UpdateItemCareerInit(ITEMCAREER *ptr)
{
	if (MoveToTarget(&ptr->pos, &ptr->targetPos, ITEMCAREER_SPEED))
	{
		ptr->cntFrame = 0;
		ptr->state = ITEMCAREER_WAIT;
	}
}

/******************************************************************************
�X�V����(WAIT���)
******************************************************************************/
void UpdateItemCareerWait(ITEMCAREER *ptr)
{
	if (ptr->cntFrame >= ITEMCAREER_WAIT_FRAME)
	{
		ptr->cntFrame = 0;
		ptr->state = ITEMCAREER_ESCAPE;
	}
}

/******************************************************************************
�X�V����(ESCAPE���)
******************************************************************************/
void UpdateItemCareerEscape(ITEMCAREER *ptr)
{
	StartAccel(&ptr->pos, &-VECTOR3_UP, ITEMCAREER_SPEED, ITEMCAREER_ESCAPE_FRAME, ptr->cntFrame);

	if (ptr->pos.y >= SCREEN_HEIGHT + ITEMCAREER_TEXTURE_SIZE_Y)
	{
		ptr->active = false;
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawItemCareer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	DrawItemCareerShadow(pDevice);
	
	DrawItemCareerBody(pDevice);
}

/******************************************************************************
�`�揈��(�e)
******************************************************************************/
void DrawItemCareerShadow(LPDIRECT3DDEVICE9 pDevice)
{
	ITEMCAREER *ptr = GetItemCareerAdr(0);

	pDevice->SetTexture(0, texture[ITEMCAREER_SHADOW]);
	
	SetTextureEnemyShadow(vertexWk);

	for (int i = 0; i < ITEMCAREER_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		SetVertexItemCareerShadow(ptr);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
	}
}

/******************************************************************************
�`�揈��(�{��)
******************************************************************************/
void DrawItemCareerBody(LPDIRECT3DDEVICE9 pDevice)
{
	ITEMCAREER *ptr = GetItemCareerAdr(0);

	pDevice->SetTexture(0, texture[ITEMCAREER_BODY]);

	for (int i = 0; i < ITEMCAREER_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		SetVertexItemCareer(ptr);
		SetTextureItemCareer(ptr->patternAnim);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

#ifdef _DEBUG
		//�����蔻��̕`��
		DrawBoxCollider(ptr->pos + D3DXVECTOR3(ITEMCAREER_BB_LEFT, ITEMCAREER_BB_TOP, 0.0f), ITEMCAREER_BB_WIDTH, ITEMCAREER_BB_HIGHT);
#endif
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexItemCareer(void)
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
void SetTextureItemCareer(int cntPattern)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % ITEMCAREER_TEXTURE_DIVIDE_X;
	int y = cntPattern / ITEMCAREER_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / ITEMCAREER_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / ITEMCAREER_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexItemCareer(ITEMCAREER *ptr)
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
���_���W�̐ݒ�(�e)
******************************************************************************/
void SetVertexItemCareerShadow(ITEMCAREER *ptr)
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
		vertexWk[i].vtx += ITEMCAREER_SHADOW_OFFSETPOS;
	}
}

/******************************************************************************
//�֐���	�FITEMCAREER *GetItemCareerAdr(int pno)
//����		�Fint pno�F�擾�������A�C�e���L�����A�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�A�C�e���L�����A�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�A�C�e���L�����A�̃A�h���X�擾�֐�
******************************************************************************/
ITEMCAREER *GetItemCareerAdr(int pno)
{
	if (pno < ITEMCAREER_MAX)
	{
		return &itemCareer[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�Fvoid SetItemCareer(D3DXVECTOR3 pos)
//����		�FD3DXVECTOR3 pos		�F�o�����W
//�߂�l	�Fvoid
//����		�F�A�C�e���L�����A�̃Z�b�g�֐�
******************************************************************************/
void SetItemCareer(D3DXVECTOR3 pos)
{
	ITEMCAREER *ptr = GetItemCareerAdr(0);

	for (int i = 0; i < ITEMCAREER_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;
		ptr->targetPos = ptr->pos + ITEMCAREER_TARGETPOS_OFFSET;

		ptr->state = ITEMCAREER_INIT;
		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->cntFrame = 0;
		ptr->hp = ITEMCAREER_INITHP;

		ptr->active = true;
		return;
	}
}