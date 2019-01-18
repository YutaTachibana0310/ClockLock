//=============================================================================
//
// �G�t�F�N�g�t�@�C�A���� [effectFire.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "effectFire.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define EFFECTFIRE_INITPOS_RANGE_MAX			(50.0f)			//�G�t�F�N�g�t�@�C�A�̍��W�ݒ�͈�
#define EFFECTFIRE_INITPOS_RANGE_MIN			(0.0f)			//�G�t�F�N�g�t�@�C�A�̍��W�ݒ�͈�
#define EFFECTFIRE_INITSPEED					(3.0f)			//�G�t�F�N�g�t�@�C�A�̏������x
#define EFFECTFIRE_ROTATION_MIN					(0.0f)			//��]�ʍŏ��l
#define EFFECTFIRE_ROTATION_MAX					(360.0f)		//��]�ʍő�l
#define EFFECTFIRE_SPEEDMAGNI_MIN				(0.8f)			//�X�s�[�h�{���ŏ��l
#define EFFECTFIRE_SPEEDMAGNI_MAX				(1.2f)			//�X�s�[�h�{���ő�l
#define EFFECTFIRE_SCALE_MAX					(0.5f)			//�X�P�[���ŏ��l
#define EFFECTFIRE_SCALE_MIN					(1.0f)			//�X�P�[���ő�l

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexEffectFire(void);						//���_�쐬�֐�
void SetTextureEffectFire(int cntPattern, EFFECT *ptr);	// �e�N�X�`�����W�̌v�Z����
void SetVertexEffectFire(EFFECT *ptr);					// ���_�̌v�Z����

/*****************************************************************************
�\���̒�`
*****************************************************************************/

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N
static float radius, angle;								//�|���S���̔��a�ƒ��_�p�x

EFFECT effectFire[EFFECTFIRE_MAX];						//�G�t�F�N�g�t�@�C�A�z��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitEffectFire(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT* ptr = GetEffectFireAdr(0);
	int i;

	angle = atan2f(EFFECTFIRE_TEXTURE_SIZE_Y, EFFECTFIRE_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(EFFECTFIRE_TEXTURE_SIZE_X, EFFECTFIRE_TEXTURE_SIZE_Y));

	for (i = 0; i < EFFECTFIRE_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(EFFECTFIRE_INITPOS_X, EFFECTFIRE_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->animTime = EFFECTFIRE_ANIM_TIME;

		ptr->active = false;
	}

	// ���_���̍쐬
	MakeVertexEffectFire();

	if(num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)EFFECTFIRE_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitEffectFire(void)
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
void UpdateEffectFire(void)
{
	int i;
	EFFECT *ptr = GetEffectFireAdr(0);

	for (i = 0; i < EFFECTFIRE_MAX; i++, ptr++)
	{
		//��A�N�e�B�u�ł���Ώ������Ȃ�
		if (!ptr->active)
		{
			continue;
		}

		//�A�j���[�V����
		ptr->countAnim++;
		if (ptr->countAnim % ptr->animTime == 0)
		{
			ptr->patternAnim++;

			//�p�^�[���̍Ō�܂ŃA�j���[�V���������Ă������A�N�e�B�u�ɃZ�b�g
			if (ptr->patternAnim >= EFFECTFIRE_ANIM_NUM)
			{
				ptr->active = false;
			}
		}

		//���W�̍X�V
		if (ptr->countAnim <= 12)
		{
			ptr->speed = cosf(RADIAN(7.5f * ptr->countAnim)) * ptr->initSpeed;
		}
		ptr->pos += ptr->velocity  * ptr->speed;

	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawEffectFire(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT *ptr = GetEffectFireAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	//�G�t�F�N�g�t�@�C�A��`��
	for (i = 0; i < EFFECTFIRE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexEffectFire(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureEffectFire(ptr->patternAnim, ptr);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexEffectFire(void)
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
void SetTextureEffectFire(int cntPattern, EFFECT *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % EFFECTFIRE_TEXTURE_DIVIDE_X;
	int y = cntPattern / EFFECTFIRE_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / EFFECTFIRE_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / EFFECTFIRE_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexEffectFire(EFFECT *ptr)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = ptr->pos.x - cosf(angle + ptr->rot.z) * radius * ptr->scale;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(angle + ptr->rot.z) * radius * ptr->scale;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(angle - ptr->rot.z) * radius * ptr->scale;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(angle - ptr->rot.z) * radius * ptr->scale;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(angle - ptr->rot.z) * radius * ptr->scale;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(angle - ptr->rot.z) * radius * ptr->scale;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(angle + ptr->rot.z) * radius * ptr->scale;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(angle + ptr->rot.z) * radius * ptr->scale;
}

/******************************************************************************
//�֐���	�FEFFECTFIRE *GetEffectFireAdr(int pno)
//����		�Fint pno�F�擾�������G�t�F�N�g�t�@�C�A�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�G�t�F�N�g�t�@�C�A�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�G�t�F�N�g�t�@�C�A�̃A�h���X�擾�֐�
******************************************************************************/
EFFECT *GetEffectFireAdr(int pno)
{
	if (pno < EFFECTFIRE_MAX)
	{
		return &effectFire[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�FEFFECT *SetEffectFire(D3DXVECTOR3 pos)
//����1		�FD3DXVECTOR3 pos		�F�ݒ���W
//����2		�FD3DXVECTOR3 dir		�F�i�ޕ���
//�߂�l	�F�������F�Z�b�g�����G�t�F�N�g�ւ̃|�C���^	���s���FNULL
//����		�F�G�t�F�N�g�t�@�C�A�̃Z�b�g�֐�
******************************************************************************/
EFFECT *SetEffectFire(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
{
	int i;
	EFFECT *ptr = GetEffectFireAdr(0);

	for (i = 0; i < EFFECTFIRE_MAX; i++, ptr++)
	{
		//���g�p�̃G�t�F�N�g�t�@�C�A������΃Z�b�g
		if (!ptr->active)
		{
			//���W�̐ݒ�
			ptr->pos = pos;
			ptr->pos += dir * RandomRange(EFFECTFIRE_INITPOS_RANGE_MIN, EFFECTFIRE_INITPOS_RANGE_MAX);

			//�p�����[�^�ݒ�
			ptr->countAnim = 0;
			ptr->patternAnim = 0;
			ptr->velocity = dir;
			ptr->rot.z = RandomRange(EFFECTFIRE_ROTATION_MIN, EFFECTFIRE_ROTATION_MAX);
			ptr->scale = RandomRange(0.5f, 1.0f);
			ptr->speed = ptr->initSpeed = EFFECTFIRE_INITSPEED * RandomRange(EFFECTFIRE_SPEEDMAGNI_MIN, EFFECTFIRE_SPEEDMAGNI_MAX);

			//�Z�b�g
			ptr->active = true;
			return ptr;
		}
	}

	return NULL;
}

/******************************************************************************
//�֐���	�FEFFECT *SetBossFire(D3DXVECTOR3 pos, D3DXVECTOR3 dir, float speed)
//����1		�FD3DXVECTOR3 pos		�F�ݒ���W
//����2		�FD3DXVECTOR3 dir		�F�i�ޕ���
//����3		�Ffloat speed			�F�X�s�[�h
//�߂�l	�F�������F�Z�b�g�����G�t�F�N�g�ւ̃|�C���^	���s���FNULL
//����		�F�{�X�t�@�C�A�̃Z�b�g�֐�
******************************************************************************/
EFFECT *SetBossFire(D3DXVECTOR3 pos, D3DXVECTOR3 dir, float speed)
{
	int i;
	EFFECT *ptr = GetEffectFireAdr(0);

	//���g�p�̃G�t�F�N�g�t�@�C�A������΃Z�b�g
	for (i = 0; i < EFFECTFIRE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		//���W�̐ݒ�
		ptr->pos = pos;
		
		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->velocity = dir;
		ptr->rot.z = RandomRange(EFFECTFIRE_ROTATION_MIN, EFFECTFIRE_ROTATION_MAX);
		ptr->scale = RandomRange(EFFECTFIRE_SCALE_MIN, EFFECTFIRE_SCALE_MAX);
		ptr->speed = ptr->initSpeed = speed;

		//�Z�b�g
		ptr->active = true;
		return ptr;
	}

	return NULL;
}