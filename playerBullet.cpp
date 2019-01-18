//=============================================================================
//
// �v���C���[�o���b�g���� [playerBullet.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "playerBullet.h"
#include "bg.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define PLAYERBULLET_SPEED (-40.0f)

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexPlayerBullet(PLAYERBULLET *ptr);				//���_�쐬�֐�
void SetTexturePlayerBullet(int cntPattern, PLAYERBULLET *ptr);	// �e�N�X�`�����W�̌v�Z����
void SetVertexPlayerBullet(PLAYERBULLET *ptr);					// ���_�̌v�Z����

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
LPDIRECT3DTEXTURE9		textureMainBullet = NULL;				// �e�N�X�`���ւ̃|�C���^�i���C���p�j
LPDIRECT3DTEXTURE9		textureSubBullet = NULL;				// �e�N�X�`���ւ̃|�C���^�i�T�u�p�j

PLAYERBULLET playerBullet[PLAYERBULLET_MAIN_MAX];				//�v���C���[�o���b�g�z��(���C���V���b�g)
PLAYERBULLET playerSubBullet[PLAYERBULLET_SUB_MAX];				//�v���C���[�o���b�g�z��i�T�u�V���b�g�j

static VERTEX_2D		vertexWk[NUM_VERTEX];					//���_���i�[���[�N

#ifdef _DEBUG
static int cntBullet;
#endif

/******************************************************************************
����������
******************************************************************************/
HRESULT InitPlayerBullet(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERBULLET* ptr = GetPlayerBulletAdr(0);
	int i;

	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(PLAYERBULLET_INITPOS_X, PLAYERBULLET_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(PLAYERBULLET_TEXTURE_SIZE_Y, PLAYERBULLET_TEXTURE_SIZE_Y);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(PLAYERBULLET_TEXTURE_SIZE_X, PLAYERBULLET_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->moveDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		ptr->active = false;

		// ���_���̍쐬
		MakeVertexPlayerBullet(ptr);
	}

	ptr = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_SUB_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(PLAYERBULLET_INITPOS_X, PLAYERBULLET_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(PLAYERBULLET_TEXTURE_SIZE_Y, PLAYERBULLET_TEXTURE_SIZE_Y);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(PLAYERBULLET_TEXTURE_SIZE_X, PLAYERBULLET_TEXTURE_SIZE_Y));
	
		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->moveDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		ptr->active = false;

		// ���_���̍쐬
		MakeVertexPlayerBullet(ptr);
	}

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		textureMainBullet = CreateTextureFromFile((LPSTR)PLAYERBULLET_TEXTURE_MAIN, pDevice);
		textureSubBullet = CreateTextureFromFile((LPSTR)PLAYERBULLET_TEXTURE_SUB, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitPlayerBullet(void)
{
	if (textureMainBullet != NULL)
	{	// �e�N�X�`���̊J��
		textureMainBullet->Release();
		textureMainBullet = NULL;
	}

	if (textureSubBullet != NULL)
	{
		textureSubBullet->Release();
		textureMainBullet = NULL;
	}
}

/******************************************************************************
�X�V����
******************************************************************************/
void UpdatePlayerBullet(void)
{
	PLAYERBULLET *ptr = GetPlayerBulletAdr(0);
	int i;

	//���C���o���b�g�̍X�V
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���W�̍X�V
			ptr->pos += PLAYERBULLET_SPEED * ptr->moveDir;
			if (ptr->pos.y < -PLAYERBULLET_BB_WIDTH|| ptr->pos.y > SCREEN_HEIGHT + PLAYERBULLET_BB_HIGHT)
			{
				ptr->active = false;
			}
			else if (ptr->pos.x + PLAYERBULLET_BB_WIDTH < BG_LEFT || ptr->pos.x - PLAYERBULLET_BB_WIDTH > BG_RIGHT)
			{
				ptr->active = false;
			}

			// �A�j���[�V����
			ptr->countAnim++;

			// �A�j���[�V����Wait�`�F�b�N
			if (ptr->countAnim % TIMEPLAYERBULLET_ANIMATION == 0)
			{
				// �p�^�[���̐؂�ւ�
				ptr->patternAnim = (ptr->patternAnim + 1) % ANIM_PLAYERBULLET_NUM;
			}
		}
	}

	//�T�u�o���b�g�̍X�V
	ptr = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_SUB_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���W�̍X�V
			ptr->pos += PLAYERBULLET_SPEED * ptr->moveDir;
			if (ptr->pos.y < -PLAYERBULLET_BB_HIGHT || ptr->pos.y > SCREEN_HEIGHT + PLAYERBULLET_BB_HIGHT)
			{
				ptr->active = false;
			}
			else if (ptr->pos.x + PLAYERBULLET_BB_WIDTH < BG_LEFT || ptr->pos.x - PLAYERBULLET_BB_WIDTH > BG_RIGHT)
			{
				ptr->active = false;
			}

			// �A�j���[�V����
			ptr->countAnim++;

			// �A�j���[�V����Wait�`�F�b�N
			if (ptr->countAnim % TIMEPLAYERBULLET_ANIMATION == 0)
			{
				// �p�^�[���̐؂�ւ�
				ptr->patternAnim = (ptr->patternAnim + 1) % ANIM_PLAYERBULLET_NUM;
			}
		}
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawPlayerBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERBULLET *ptr = GetPlayerBulletAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, textureMainBullet);

	//�v���C���[�o���b�g�����Z�����ŕ`��
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexPlayerBullet(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTexturePlayerBullet(ptr->patternAnim, ptr);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//�����蔻��̕`��
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(PLAYERBULLET_BB_LEFT, PLAYERBULLET_BB_TOP, 0.0f), PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
#endif
		}
	}

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, textureSubBullet);

	ptr = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_SUB_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexPlayerBullet(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTexturePlayerBullet(ptr->patternAnim, ptr);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//�����蔻��̕`��
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(PLAYERBULLET_BB_LEFT, PLAYERBULLET_BB_TOP, 0.0f), PLAYERBULLET_BB_WIDTH, PLAYERBULLET_BB_HIGHT);
#endif
		}
	}

	//�`����@��ʏ�̕��@�ɍĐݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexPlayerBullet(PLAYERBULLET *ptr)
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
void SetTexturePlayerBullet(int cntPattern, PLAYERBULLET *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % PLAYERBULLET_TEXTURE_DIVIDE_X;
	int y = cntPattern / PLAYERBULLET_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / PLAYERBULLET_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / PLAYERBULLET_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexPlayerBullet(PLAYERBULLET *ptr)
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
//�֐���	�FPLAYERBULLET *GetPlayerBulletAdr(int pno)
//����		�Fint pno�F�擾�������v���C���[�o���b�g�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�v���C���[�o���b�g�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�v���C���[�o���b�g�̃A�h���X�擾�֐��i���C���V���b�g�j
******************************************************************************/
PLAYERBULLET *GetPlayerBulletAdr(int pno)
{
	if (pno < PLAYERBULLET_MAIN_MAX)
	{
		return &playerBullet[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�FPLAYERBULLET *GetPlayerSubBulletAdr(int pno)
//����		�Fint pno�F�擾�������v���C���[�o���b�g�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�v���C���[�o���b�g�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�v���C���[�o���b�g�̃A�h���X�擾�֐��i�T�u�V���b�g�j
******************************************************************************/
PLAYERBULLET *GetPlayerSubBulletAdr(int pno)
{
	if (pno < PLAYERBULLET_SUB_MAX)
	{
		return &playerSubBullet[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�Fvoid FirePlayerBullet(D3DXVECTOR3 pos)
//����		�FD3DXVECTOR3 pos�F�v���C���[�o���b�g�̔��ˈʒu
//�߂�l	�Fvoid
//����		�F�v���C���[�o���b�g�𔭎˂���֐��i���C���V���b�g�p�j
******************************************************************************/
void FirePlayerBullet(D3DXVECTOR3 pos)
{
	int i;
	PLAYERBULLET *ptr = GetPlayerBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_MAIN_MAX; i++, ptr++)
	{
		//��A�N�e�B�u�̃v���C���[�o���b�g������Δ���
		if (!ptr->active)
		{
			ptr->pos = pos;
			ptr->active = true;
			return;
		}
	}
}
/******************************************************************************
//�֐���	�Fvoid FirePlayerSubBullet(D3DXVECTOR3 pos)
//����1		�FD3DXVECTOR3 pos�F�v���C���[�o���b�g�̔��ˈʒu
//����2		�Ffloat rotAngle�F�o���b�g����]������p�x
//�߂�l	�Fvoid
//����		�F�v���C���[�o���b�g�𔭎˂���֐��i�T�u�V���b�g�p�j
******************************************************************************/
void FirePlayerSubBullet(D3DXVECTOR3 pos, float rotAngle)
{
	int i;
	PLAYERBULLET *ptr = GetPlayerSubBulletAdr(0);
	for (i = 0; i < PLAYERBULLET_SUB_MAX; i++, ptr++)
	{
		//��A�N�e�B�u�̃v���C���[�o���b�g������Δ���
		if (!ptr->active)
		{
			ptr->pos = pos;
			ptr->moveDir = D3DXVECTOR3(cosf(RADIAN(rotAngle)), sinf(RADIAN(rotAngle)), 0.0f);
			ptr->rot.z = RADIAN(90 + rotAngle);
			ptr->active = true;
			return;
		}
	}
}
