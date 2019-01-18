//=============================================================================
//
// �v���C���[�o���b�g�̔������� [tmplate.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "pBulletExplosion.h"
#include "playerParticle.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define PBULLETEXPLOSION_SPEED					(-40.0f)
#define PLAYERPARTICLE_EMITT_NUM				(3)
#define PLAYERPARTICLE_EMITTER_DURATION			(5)
#define PLAYERPARTICLE_SCALE_MAX				(1.0f)

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexPBulletExplosion(void);								//���_�쐬�֐�
void SetTexturePBulletExplosion(int cntPattern, PBULLETEXPLOSION *ptr);	// �e�N�X�`�����W�̌v�Z����
void SetVertexPBulletExplosion(PBULLETEXPLOSION *ptr);					// ���_�̌v�Z����

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture = NULL;							// �e�N�X�`���ւ̃|�C���^�i���C���p�j

PBULLETEXPLOSION pBulletExplosion[PBULLETEXPLOSION_MAX];				//�v���C���[�o���b�g�z��(���C���V���b�g)

static VERTEX_2D		vertexWk[NUM_VERTEX];							//���_���i�[���[�N

/******************************************************************************
����������
******************************************************************************/
HRESULT InitPBulletExplosion(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PBULLETEXPLOSION* ptr = GetPBulletExplosionAdr(0);
	int i;

	for (i = 0; i < PBULLETEXPLOSION_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(PBULLETEXPLOSION_INITPOS_X, PBULLETEXPLOSION_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(PBULLETEXPLOSION_TEXTURE_SIZE_Y, PBULLETEXPLOSION_TEXTURE_SIZE_Y);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(PBULLETEXPLOSION_TEXTURE_SIZE_X, PBULLETEXPLOSION_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->active = false;
	}

	// ���_���̍쐬
	MakeVertexPBulletExplosion();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)PBULLETEXPLOSION_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitPBulletExplosion(void)
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
void UpdatePBulletExplosion(void)
{
	int i;
	PBULLETEXPLOSION *ptr = GetPBulletExplosionAdr(0);

	for (i = 0; i < PBULLETEXPLOSION_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//�A�j���[�V�����̍X�V
			if (ptr->countAnim % TIMEPBULLETEXPLOSION_ANIMATION == 0)
			{
				ptr->patternAnim++;
				//�Ō�܂ŃA�j���[�V�������Ă������A�N�e�B�u��
				if (ptr->patternAnim >= ANIM_PBULLETEXPLOSION_NUM)
				{
					ptr->active = false;
				}
			}
		}
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawPBulletExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PBULLETEXPLOSION *ptr = GetPBulletExplosionAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	//�v���C���[�o���b�g�����Z�����ŕ`��
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	for (i = 0; i < PBULLETEXPLOSION_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexPBulletExplosion(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTexturePBulletExplosion(ptr->patternAnim, ptr);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//�����蔻��̕`��
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(PBULLETEXPLOSION_BB_LEFT, PBULLETEXPLOSION_BB_TOP, 0.0f), PBULLETEXPLOSION_BB_WIDTH, PBULLETEXPLOSION_BB_HIGHT);
#endif
		}
	}
	//�`����@��ʏ�̕��@�ɍĐݒ�
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexPBulletExplosion(void)
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
void SetTexturePBulletExplosion(int cntPattern, PBULLETEXPLOSION *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % PBULLETEXPLOSION_TEXTURE_DIVIDE_X;
	int y = cntPattern / PBULLETEXPLOSION_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / PBULLETEXPLOSION_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / PBULLETEXPLOSION_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexPBulletExplosion(PBULLETEXPLOSION *ptr)
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
//�֐���	�FPBULLETEXPLOSION *GetPBulletExplosionAdr(int pno)
//����		�Fint pno�F�擾�������v���C���[�o���b�g�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�v���C���[�o���b�g�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�v���C���[�o���b�g�̃A�h���X�擾�֐��i���C���V���b�g�j
******************************************************************************/
PBULLETEXPLOSION *GetPBulletExplosionAdr(int pno)
{
	if (pno < PBULLETEXPLOSION_MAX)
	{
		return &pBulletExplosion[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�Fvoid SetPBulletExplosion(D3DXVECTOR3 pos)
//����		�FD3DXVECTOR3 pos	�F�������Z�b�g���������W
//�߂�l	�Fvoid
//����		�F�v���C���[�o���b�g�̔����̃Z�b�g�֐�
******************************************************************************/
void SetPBulletExplosion(D3DXVECTOR3 pos)
{
	int i;
	PBULLETEXPLOSION *ptr = GetPBulletExplosionAdr(0);

	for (i = 0; i < PBULLETEXPLOSION_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			ptr->pos = pos + D3DXVECTOR3(RandomRange(-5.0f, 5.0f), RandomRange(-5.0f, 5.0f), 0.0f);
			ptr->rot.z = RandomRange(0.0f, 360.0f);
			ptr->patternAnim = 0;
			ptr->active = true;
			SetEmitterPlayerParticle(pos, PLAYERPARTICLE_EMITTER_DURATION, PLAYERPARTICLE_EMITT_NUM, EMITTER_CONE, PLAYERPARTICLE_SCALE_MAX);
			return;
		}
	}
}