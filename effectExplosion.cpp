//=============================================================================
//
// �G�t�F�N�g�G�N�X�v���[�W�������� [effectExplosion.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "effectExplosion.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define EFFECTEXPLOSION_POS_RANGE				(80.0f)			//�G�t�F�N�g�G�N�X�v���[�W�����̍��W�ݒ�͈�
#define EFFECTEXPLOSION_INITSPEED				(0.5f)			//�����X�s�[�h
#define EFFECTEXPLOSION_SLOWFRAME				(20)			//��������t���[��
#define EFFECTEXPLOSION_SPEEDMAGNI_MIN			(0.8f)			//���x�W���̍ŏ��l
#define EFFECTEXPLOSION_SPEEDMAGNI_MAX			(1.5f)			//���x�W���̍ő�l
#define EFFECTEXPLOSION_SCALE_MIN				(0.8f)			//�X�P�[���ŏ��l
#define EFFECTEXPLOSION_SCALE_MAX				(1.5f)			//�X�P�[���ő�l
#define EFFECTEXPLOSION_INITPATTERN	(EFFECTEXPLOSION_ANIM_TIME + rand() % 2);	//�A�j���p�^�[���̏����l
#define EFFECTEXPLOSION_BASEROTATION			(RADIAN(45.0f))	//��]�p�x�̏������Z�l

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexEffectExplosion(void);						//���_�쐬�֐�
void SetTextureEffectExplosion(int cntPattern, EFFECT *ptr);	// �e�N�X�`�����W�̌v�Z����
void SetVertexEffectExplosion(EFFECT *ptr);					// ���_�̌v�Z����

/*****************************************************************************
�\���̒�`
*****************************************************************************/

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N
static float radius, angle;

EFFECT effectExplosion[EFFECTEXPLOSION_MAX];					//�G�t�F�N�g�G�N�X�v���[�W�����z��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitEffectExplosion(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT* ptr = GetEffectExplosionAdr(0);
	int i;

	angle = atan2f(EFFECTEXPLOSION_TEXTURE_SIZE_Y, EFFECTEXPLOSION_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(EFFECTEXPLOSION_TEXTURE_SIZE_X, EFFECTEXPLOSION_TEXTURE_SIZE_Y));

	for (i = 0; i < EFFECTEXPLOSION_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(EFFECTEXPLOSION_INITPOS_X, EFFECTEXPLOSION_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->animTime = EFFECTEXPLOSION_ANIM_TIME;

		ptr->active = false;
	}

	// ���_���̍쐬
	MakeVertexEffectExplosion();

	if(num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)EFFECTEXPLOSION_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitEffectExplosion(void)
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
void UpdateEffectExplosion(void)
{
	int i;
	EFFECT *ptr = GetEffectExplosionAdr(0);

	for (i = 0; i < EFFECTEXPLOSION_MAX; i++, ptr++)
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
			if (ptr->patternAnim >= EFFECTEXPLOSION_ANIM_NUM)
			{
				ptr->active = false;
			}
		}

		//���W�̍X�V
		ptr->pos += ptr->velocity * ptr->speed;
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawEffectExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT *ptr = GetEffectExplosionAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	//�G�t�F�N�g�G�N�X�v���[�W������`��
	for (i = 0; i < EFFECTEXPLOSION_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexEffectExplosion(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureEffectExplosion(ptr->patternAnim, ptr);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexEffectExplosion(void)
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
void SetTextureEffectExplosion(int cntPattern, EFFECT *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % EFFECTEXPLOSION_TEXTURE_DIVIDE_X;
	int y = cntPattern / EFFECTEXPLOSION_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / EFFECTEXPLOSION_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / EFFECTEXPLOSION_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexEffectExplosion(EFFECT *ptr)
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
//�֐���	�FEFFECTEXPLOSION *GetEffectExplosionAdr(int pno)
//����		�Fint pno�F�擾�������G�t�F�N�g�G�N�X�v���[�W�����̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�G�t�F�N�g�G�N�X�v���[�W�����̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�G�t�F�N�g�G�N�X�v���[�W�����̃A�h���X�擾�֐�
******************************************************************************/
EFFECT *GetEffectExplosionAdr(int pno)
{
	if (pno < EFFECTEXPLOSION_MAX)
	{
		return &effectExplosion[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�FEFFECT *SetEffectExplosion(D3DXVECTOR3 pos)
//����1		�FD3DXVECTOR3 pos		�F�ݒ���W
//����2		�FD3DXVECTOR3 dir		�F�i�ޕ���
//�߂�l	�F�������F�Z�b�g�����G�t�F�N�g�ւ̃|�C���^	���s���FNULL
//����		�F�G�t�F�N�g�G�N�X�v���[�W�����̃Z�b�g�֐�
******************************************************************************/
EFFECT *SetEffectExplosion(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
{
	int i;
	EFFECT *ptr = GetEffectExplosionAdr(0);
	float tmp = RADIAN(RandomRange(0.0f, 360.0f));

	for (i = 0; i < EFFECTEXPLOSION_MAX; i++, ptr++)
	{
		//���g�p�̃G�t�F�N�g�G�N�X�v���[�W����������΃Z�b�g
		if (!ptr->active)
		{
			//���W�̐ݒ�
			ptr->pos = pos;
			//ptr->pos += dir * RandomRange(0, EFFECTEXPLOSION_POS_RANGE);
			//ptr->rot.z = acosf(dir.x) + RADIAN(45);
			ptr->pos.x += cosf(tmp) * RandomRange(0, EFFECTEXPLOSION_POS_RANGE);
			ptr->pos.y += sinf(tmp) * RandomRange(0, EFFECTEXPLOSION_POS_RANGE);
			ptr->rot.z = tmp + EFFECTEXPLOSION_BASEROTATION;

			//�p�����[�^�ݒ�
			ptr->countAnim = 0;
			ptr->patternAnim = 0;
			ptr->animTime = EFFECTEXPLOSION_INITPATTERN;
			//ptr->velocity = dir;
			ptr->velocity = D3DXVECTOR3(cosf(tmp), sinf(tmp), 0.0f);
			ptr->speed = ptr->initSpeed = EFFECTEXPLOSION_INITSPEED * RandomRange(EFFECTEXPLOSION_SPEEDMAGNI_MIN, EFFECTEXPLOSION_SPEEDMAGNI_MAX);
			ptr->scale = RandomRange(EFFECTEXPLOSION_SCALE_MIN, EFFECTEXPLOSION_SCALE_MAX);

			//�Z�b�g
			ptr->active = true;
			return ptr;
		}
	}

	return NULL;
}