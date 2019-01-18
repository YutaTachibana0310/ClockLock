//=============================================================================
//
// �v���C���[�{���o�[���� [playerBomber.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "playerBomber.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define PLAYERBOMBER_ROTANGLE		(-1.0f)

#define PLAYERBOMBER_LIFEFRAME		(180)
#define PLAYERBOMBER_REDUCTIONFRAME	(150)

#define PLAYERBOMBER_SCALEFRAME		(30)
/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexPlayerBomber(void);				//���_�쐬�֐�
void SetTexturePlayerBomber(int cntPattern);	// �e�N�X�`�����W�̌v�Z����
void SetVertexPlayerBomber(PLAYERBOMBER *ptr);					// ���_�̌v�Z����

/*****************************************************************************
�\���̒�`
*****************************************************************************/

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N

PLAYERBOMBER playerBomber[PLAYERBOMBER_MAX];				//�v���C���[�{���o�[�z��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitPlayerBomber(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERBOMBER* ptr = GetPlayerBomberAdr(0);
	int i;

	for (i = 0; i < PLAYERBOMBER_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(PLAYERBOMBER_INITPOS_X, PLAYERBOMBER_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(PLAYERBOMBER_TEXTURE_SIZE_Y, PLAYERBOMBER_TEXTURE_SIZE_X);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(PLAYERBOMBER_TEXTURE_SIZE_X, PLAYERBOMBER_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->active = false;
	}

	// ���_���̍쐬
	MakeVertexPlayerBomber();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)PLAYERBOMBER_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitPlayerBomber(void)
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
void UpdatePlayerBomber(void)
{
	PLAYERBOMBER *ptr = &playerBomber[0];
	int cnt;

	for (int i = 0; i < PLAYERBOMBER_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		ptr->countAnim++;

		ptr->rot.z += PLAYERBOMBER_ROTANGLE;

		if (ptr->countAnim > PLAYERBOMBER_REDUCTIONFRAME)
		{
			cnt = ptr->countAnim - PLAYERBOMBER_REDUCTIONFRAME;
			ptr->scale = 1.0f - (float)cnt / PLAYERBOMBER_SCALEFRAME;
		}

		if (ptr->countAnim == PLAYERBOMBER_LIFEFRAME)
		{
			ptr->active = false;
		}
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawPlayerBomber(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERBOMBER *ptr = GetPlayerBomberAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	//�v���C���[�{���o�[�����Z�����ŕ`��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	for (i = 0; i < PLAYERBOMBER_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexPlayerBomber(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTexturePlayerBomber(ptr->patternAnim);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//�����蔻��̕`��
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(PLAYERBOMBER_BB_LEFT, PLAYERBOMBER_BB_TOP, 0.0f), PLAYERBOMBER_BB_WIDTH, PLAYERBOMBER_BB_HIGHT);
#endif
		}
	}
	//�`����@��ʏ�̕��@�ɍĐݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexPlayerBomber(void)
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
void SetTexturePlayerBomber(int cntPattern)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % PLAYERBOMBER_TEXTURE_DIVIDE_X;
	int y = cntPattern / PLAYERBOMBER_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / PLAYERBOMBER_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / PLAYERBOMBER_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexPlayerBomber(PLAYERBOMBER *ptr)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
}

/******************************************************************************
//�֐���	�FPLAYERBOMBER *GetPlayerBomberAdr(int pno)
//����		�Fint pno�F�擾�������v���C���[�{���o�[�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�v���C���[�{���o�[�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�v���C���[�{���o�[�̃A�h���X�擾�֐�
******************************************************************************/
PLAYERBOMBER *GetPlayerBomberAdr(int pno)
{
	if (pno < PLAYERBOMBER_MAX)
	{
		return &playerBomber[pno];
	}
	else
	{
		return NULL;
	}
}

bool SetPlayerBomber(D3DXVECTOR3 pos)
{
	PLAYERBOMBER *ptr = &playerBomber[0];

	for (int i = 0; i < PLAYERBOMBER_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;
		ptr->countAnim = 0;
		ptr->scale = 1.0f;
		ptr->active = true;
		return true;
	}

	return false;
}