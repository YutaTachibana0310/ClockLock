//=============================================================================
//
// �v���C���[�o���b�g���� [playerShield.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "player.h"
#include "playerShield.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define PLAYERSHIELD_SPEED				(-40.0f)

#define PLAYERSHIELD_SCALE_MAX			(1.0f)
#define PLAYERSHIELD_SCALE_MIN			(0.0f)
#define PLAYERSHIELD_SCALE_ADDVALUE		(0.1f)

#define PLAYERSHIELD_COLOR_GREEN		(D3DCOLOR_RGBA(204, 255, 0, 128))
#define PLAYERSHIELD_COLOR_ORANGE		(D3DCOLOR_RGBA(255, 102, 0, 128))

#define PLAYERSHIELD_POS_OFFSET			(D3DXVECTOR3(0.0f, -10.0f, 0.0f))

#define PLAYERSHIELD_OPEN_FRAME			(180)
#define PLAYERSHIELD_CLOSE_FRAME		(120)

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexPlayerShield(void);					//���_�쐬�֐�
void SetTexturePlayerShield(int cntPattern);			// �e�N�X�`�����W�̌v�Z����
void SetVertexPlayerShield(PLAYERSHIELD *ptr);			// ���_�̌v�Z����
void SetDiffusePlayerShield(PLAYERSHIELD *ptr);			//�f�B�t���[�Y�ݒ菈��
void SetPlayerShieldColor(PLAYERSHIELD *ptr);			//��Ԑݒ�
void SetPlayerShieldAlpha(int val);

/*****************************************************************************
�\���̒�`
*****************************************************************************/

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N

PLAYERSHIELD playerShield;								//�v���C���[�o���b�g�z��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitPlayerShield(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERSHIELD* ptr = GetPlayerShieldAdr();

	ptr->pos = D3DXVECTOR3(PLAYERSHIELD_INITPOS_X, PLAYERSHIELD_INITPOS_Y, 0.0f);
	ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	ptr->angle = atan2f(PLAYERSHIELD_TEXTURE_SIZE_Y, PLAYERSHIELD_TEXTURE_SIZE_X);
	ptr->radius = D3DXVec2Length(&D3DXVECTOR2(PLAYERSHIELD_TEXTURE_SIZE_X, PLAYERSHIELD_TEXTURE_SIZE_Y));
	ptr->scale = 1.0f;

	ptr->countAnim = 0;
	ptr->patternAnim = 0;
	ptr->cntFrame = 0;

	//ptr->active = false;

	// ���_���̍쐬
	MakeVertexPlayerShield();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)PLAYERSHIELD_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitPlayerShield(void)
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
void UpdatePlayerShield(void)
{
	PLAYERSHIELD *ptr = &playerShield;
	PLAYER *player = GetPlayerAdr(0);

	if (ptr->active)
	{
		//if (player->state == PLAYER_INVINCIBLE)
		//{
			ptr->cntFrame++;
		//}

		if (ptr->cntFrame > PLAYERSHIELD_OPEN_FRAME + PLAYERSHIELD_CLOSE_FRAME)
		{
			ptr->active = false;
			return;
		}

		if (ptr->cntFrame > PLAYERSHIELD_OPEN_FRAME)
		{
			float percent = (PLAYERSHIELD_OPEN_FRAME + PLAYERSHIELD_CLOSE_FRAME - ptr->cntFrame) / (float)PLAYERSHIELD_CLOSE_FRAME;
			int alpha = (int)(percent * 255);
			SetPlayerShieldAlpha(alpha);
		}

		ptr->pos = player->pos + PLAYERSHIELD_POS_OFFSET;

		ptr->active = (ptr->scale != PLAYERSHIELD_SCALE_MIN) ? true : false;


	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawPlayerShield(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERSHIELD *ptr = GetPlayerShieldAdr();

	if (!ptr->active)
	{
		return;
	}

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	//���_���W��ݒ�
	SetVertexPlayerShield(ptr);

	// �e�N�X�`�����W��ݒ�
	SetTexturePlayerShield(ptr->patternAnim);

	//�f�B�t���[�Y�ݒ�(�F�ύX�̓I�~�b�g)
	//SetDiffusePlayerShield(ptr);

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexPlayerShield(void)
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
void SetTexturePlayerShield(int cntPattern)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexPlayerShield(PLAYERSHIELD *ptr)
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

void SetDiffusePlayerShield(PLAYERSHIELD *ptr)
{
	/*D3DCOLOR color = (ptr->color == SHIELD_GREEN) ? PLAYERSHIELD_COLOR_GREEN : PLAYERSHIELD_COLOR_ORANGE;

	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertexWk[i].diffuse = color;
	}*/
}

/******************************************************************************
//�֐���	�FPLAYERSHIELD *GetPlayerShieldAdr(int pno)
//����		�Fint pno�F�擾�������v���C���[�o���b�g�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�v���C���[�o���b�g�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�v���C���[�o���b�g�̃A�h���X�擾�֐�
******************************************************************************/
PLAYERSHIELD *GetPlayerShieldAdr(void)
{
	return &playerShield;
}

void SetPlayerShield(void)
{
	PLAYERSHIELD *ptr = &playerShield;

	if (!ptr->active)
	{
		SetPlayerShieldAlpha(255);
		ptr->cntFrame = 0;
		ptr->active = true;
	}
}

void SetPlayerShieldColor(PLAYERSHIELD *ptr)
{
	if (GetMyButton1Down(0))
	{
		ptr->active = true;
		//ptr->color = SHIELD_ORANGE;
		return;
	}

	if (GetMyButton2Down(0))
	{
		ptr->active = true;
		//ptr->color = SHIELD_GREEN;
	}

	ptr->active = false;
}

void SetPlayerShieldAlpha(int val)
{
	vertexWk[0].diffuse =
		vertexWk[1].diffuse =
		vertexWk[2].diffuse =
		vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, val);
}