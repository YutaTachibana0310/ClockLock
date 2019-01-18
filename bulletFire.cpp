//***************************************************************************==
//
// �o���b�g�t�@�C�A���� [bulletFire.cpp]
// Author : GP11A341 21 ���ԗY��
//
//***************************************************************************==
#include "main.h"
#include "bulletFire.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BULLETFIRE_INITPOS_X						(-100)
#define BULLETFIRE_INITPOS_Y						(-100)

#define EXPLOSIONMIDDLE_SIZE_LENGTH				(51)					//�����̃e�N�X�`���T�C�Y�̗�����
#define EXPLOSIONMIDDLE_INITSPEED				(3.0f)					//���^�����p�[�e�B�N���̏����X�s�[�h
#define EXPLOSIONMIDDLE_SPEED_LENGTH			(60)					//�����̃X�s�[�h�̗�����

#define ENEMYPIECE_SIZE_LENFTH					(15)					//�G�l�~�[�̔j�ЃT�C�Y�̗�����
#define ENEMYPIECE_SPEED_LENGTH					(50)					//�G�l�~�[�̔j�Ђ̃X�s�[�h�̗�����
#define ENEMYPIECE_INITSPEED					(50.0f)					//�G�l�~�[�̔j�Ђ̏����X�s�[�h

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBulletFire(void);									//���_�̍쐬
void SetTextureBulletFire(int cntPattern, BULLETFIRE *ptr);			//�e�N�X�`���̌v�Z����
void SetVertexBulletFire(BULLETFIRE *ptr);							//���_�̌v�Z����

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static VERTEX_2D				vertexWk[NUM_VERTEX];				// ���_����
static LPDIRECT3DTEXTURE9		textureBulletFire = NULL;			// �o���b�g�t�@�C�A�e�N�X�`���ւ̃|�C���^

static float					g_fRadius;							// �|���S���̔��a
static float					g_fBaseAngle;						// �|���S���̊p�x

BULLETFIRE						bulletFire[BULLETFIRE_MAX];			// �o���b�g�t�@�C�A�\���̂̔z��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitBulletFire(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLETFIRE *fire;
	int i;

	fire = GetBulletFireAdr(0);

	//�o���b�g�t�@�C�A�̏���������
	for (i = 0; i < BULLETFIRE_MAX; i++, fire++)
	{
		fire->pos = D3DXVECTOR3(BULLETFIRE_INITPOS_X, BULLETFIRE_INITPOS_Y, 0.0f);
		fire->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		fire->angle = atan2f(BULLETFIRE_TEXTURE_SIZE_Y, BULLETFIRE_TEXTURE_SIZE_X);
		fire->radius = D3DXVec2Length(&D3DXVECTOR2(BULLETFIRE_TEXTURE_SIZE_X, BULLETFIRE_TEXTURE_SIZE_Y));

		fire->countAnim = 0;
		fire->patternAnim = 0;

		fire->active = false;
	}
	// ���_���̍쐬
	MakeVertexBulletFire();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		textureBulletFire = CreateTextureFromFile((LPSTR)BULLETFIRE_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/***************************************************************************
�I������
//***************************************************************************/
void UninitBulletFire(void)
{
	if (textureBulletFire != NULL)
	{	// �e�N�X�`���̊J��
		textureBulletFire->Release();
		textureBulletFire = NULL;
	}
}

//***************************************************************************==
// �X�V����
//***************************************************************************==
void UpdateBulletFire(void)
{
	BULLETFIRE *effect = GetBulletFireAdr(0);
	int i;

	//�o���b�g�t�@�C�A�̍X�V����
	for (i = 0; i < BULLETFIRE_MAX; i++, effect++)
	{
		if (effect->active)
		{
			// �A�j���[�V����
			effect->countAnim++;

			// �A�j���[�V����Wait�`�F�b�N
			if (effect->countAnim % TIMEBULLETFIRE_ANIMATION == 0)
			{
				// �p�^�[���̐؂�ւ�
				effect->patternAnim++;
				//�p�^�[���̍Ō�܂Ő؂�ւ��Ă������A�N�e�B�u��
				if (effect->patternAnim >= BULLETFIRE_ANIM_NUM)
				{
					effect->active = false;
				}
			}
		}
	}
}

//***************************************************************************==
// �`�揈��
//***************************************************************************==
void DrawBulletFire(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLETFIRE *effect = GetBulletFireAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, textureBulletFire);

	//�o���b�g�t�@�C�A��`��
	for (i = 0; i < BULLETFIRE_MAX; i++, effect++)
	{
		if (effect->active)
		{
			//���_���W�̐ݒ�
			SetVertexBulletFire(effect);

			// �e�N�X�`�����W��ݒ�
			SetTextureBulletFire(effect->patternAnim, effect);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//***************************************************************************==
// ���_�̍쐬
//***************************************************************************==
HRESULT MakeVertexBulletFire(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLETFIRE *effect = GetBulletFireAdr(0);

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

//***************************************************************************==
// �e�N�X�`�����W�̐ݒ�
//***************************************************************************==
void SetTextureBulletFire(int cntPattern, BULLETFIRE *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % BULLETFIRE_TEXTURE_DIVIDE_X;
	int y = cntPattern / BULLETFIRE_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / BULLETFIRE_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / BULLETFIRE_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


//***************************************************************************==
// ���_���W�̐ݒ�
//***************************************************************************==
void SetVertexBulletFire(BULLETFIRE *ptr)
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
//�֐���	�FBULLETFIRE *GetBulletFireAdr(int pno)
//����		�Fint pno:�擾�������o���b�g�t�@�C�A�̃C���f�b�N�X
//�߂�l	�Fvoid
//����		�F�o���b�g�t�@�C�A�̃A�h���X���擾����֐�
******************************************************************************/
BULLETFIRE *GetBulletFireAdr(int pno)
{
	return &bulletFire[pno];
}

/******************************************************************************
//�֐���	�Fvoid SetBulletFire(BULLETFIRE *ptr)
//����		�FBULLETFIRE *ptr�F�Z�b�g�������o���b�g�t�@�C�A�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�Q�[�����Ƀo���b�g�t�@�C�A���Z�b�g����֐�
******************************************************************************/
void SetBulletFire(BULLETFIRE *ptr)
{
	if (!ptr->active)
	{
		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		SetVertexBulletFire(ptr);
		ptr->active = true;
	}
}
