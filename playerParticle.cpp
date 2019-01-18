//=============================================================================
//
// �G�l�~�[�p�[�e�B�N������ [playerParticle.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "playerParticle.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define PLAYERPARTICLE_LIFEFRAME_MAX			(30)
#define PLAYERPARTICLE_LIFEFRAME_MIN			(10)

#define PLAYERPARTICLE_SPEED_MAX				(30.0f)
#define PLAYERPARTICLE_SPEED_MIN				(10.0f)

#define PLAYERPARTICLE_SCALE_MIN				(0.5f)

#define PLAYERPARTICLE_ANGLE_MIN				(-10.0f)
#define PLAYERPARTICLE_ANGLE_MAX				(190.0f)

#define PLAYEREXPLOSION_EMITTER_DURATIONMAX		(10)


/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexPlayerParticle(void);				//���_�쐬�֐�
void SetTexturePlayerParticle(void);		// �e�N�X�`�����W�̌v�Z����
void SetVertexPlayerParticle(PLAYERPARTICLE *ptr);	// ���_�̌v�Z����

/*****************************************************************************
�\���̒�`
*****************************************************************************/

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N

PLAYERPARTICLE playerParticle[PLAYERPARTICLE_MAX];				//�G�l�~�[�p�[�e�B�N���z��
EMITTER_PLAYERPARTICLE	emitterPlayerParticle[PLAYERPARTICLE_EMITTER_MAX];

/******************************************************************************
����������
******************************************************************************/
HRESULT InitPlayerParticle(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERPARTICLE* ptr = GetPlayerParticleAdr(0);
	EMITTER_PLAYERPARTICLE *emitter = &emitterPlayerParticle[0];
	int i;

	for (i = 0; i < PLAYERPARTICLE_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(PLAYERPARTICLE_INITPOS_X, PLAYERPARTICLE_INITPOS_Y, 0.0f);

		ptr->angle = atan2f(PLAYERPARTICLE_TEXTURE_SIZE_Y, PLAYERPARTICLE_TEXTURE_SIZE_X);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(PLAYERPARTICLE_TEXTURE_SIZE_X, PLAYERPARTICLE_TEXTURE_SIZE_Y));

		ptr->frameCnt = 0;
		ptr->speed = 0;
		ptr->dir = VECTOR3_ZERO;
		ptr->scale = 1.0f;

		ptr->active = false;
	}

	for (i = 0; i < PLAYERPARTICLE_EMITTER_MAX; i++, emitter++)
	{
		emitter->pos = VECTOR3_ZERO;
		emitter->active = false;
		emitter->frameCnt = 0;
		emitter->duration = 0;
	}

	// ���_���̍쐬
	MakeVertexPlayerParticle();

	if(num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)PLAYERPARTICLE_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitPlayerParticle(void)
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
void UpdatePlayerParticle(void)
{
	PLAYERPARTICLE *ptr = &playerParticle[0];
	EMITTER_PLAYERPARTICLE *emitter = &emitterPlayerParticle[0];

	//�G�~�b�^�̍X�V
	for (int i = 0; i < PLAYERPARTICLE_EMITTER_MAX; i++, emitter++)
	{
		if (!emitter->active)
		{
			continue;
		}

		if (++emitter->frameCnt == emitter->duration)
		{
			emitter->active = false;
			continue;
		}

		for (int j = 0; j < emitter->emitNum; j++)
		{
			SetPlayerParticle(emitter->pos, emitter->shape, emitter->maxScale);
		}
	}

	//�p�[�e�B�N���̍X�V
 	for (int i = 0; i < PLAYERPARTICLE_MAX; i++, ptr++)
	{
		//��A�N�e�B�u�ł��ꂠ�R���e�B�j���[
		if (!ptr->active)
		{
			continue;
		}

		//�����ł���Δ�A�N�e�B�u�ɂ��R���e�B�j���[
		if (++ptr->frameCnt == ptr->lifeFrame)
		{
			ptr->active = false;
			continue;
		}

		//�X�s�[�h�ƃX�P�[�����C�[�W���O����
		ptr->speed = EaseInCubic((float)ptr->frameCnt, ptr->initSpeed, 0.0f, (float)ptr->lifeFrame);
		ptr->scale = EaseOutCubic((float)ptr->frameCnt, ptr->initScale, 0.0f, (float)ptr->lifeFrame);

		//���W�X�V
		ptr->pos += ptr->dir * ptr->speed;
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawPlayerParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERPARTICLE *ptr = GetPlayerParticleAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	//�G�l�~�[�p�[�e�B�N�������Z�����ŕ`��
	for (i = 0; i < PLAYERPARTICLE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexPlayerParticle(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTexturePlayerParticle();

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexPlayerParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// rhw�̐ݒ�
	vertexWk[0].rhw =
		vertexWk[1].rhw =
		vertexWk[2].rhw =
		vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 250);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 250);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 250);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 250);

	return S_OK;
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�
******************************************************************************/
void SetTexturePlayerParticle(void)
{
	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexPlayerParticle(PLAYERPARTICLE *ptr)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle) * ptr->radius * ptr->scale;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle) * ptr->radius * ptr->scale;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle) * ptr->radius * ptr->scale;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle) * ptr->radius * ptr->scale;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle) * ptr->radius * ptr->scale;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle) * ptr->radius * ptr->scale;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle) * ptr->radius * ptr->scale;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle) * ptr->radius * ptr->scale;
}

/******************************************************************************
//�֐���	�FPLAYERPARTICLE *GetPlayerParticleAdr(int pno)
//����		�Fint pno�F�擾�������G�l�~�[�p�[�e�B�N���̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�G�l�~�[�p�[�e�B�N���̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�G�l�~�[�p�[�e�B�N���̃A�h���X�擾�֐�
******************************************************************************/
PLAYERPARTICLE *GetPlayerParticleAdr(int pno)
{
	if (pno < PLAYERPARTICLE_MAX)
	{
		return &playerParticle[pno];
	}
	else
	{
		return NULL;
	}
}

/*
�G�l�~�[�p�[�e�B�N���Z�b�g�֐�
*/
void SetPlayerParticle(D3DXVECTOR3 pos, int shape, float maxScale)
{
	PLAYERPARTICLE *ptr = &playerParticle[0];

	for (int i = 0; i < PLAYERPARTICLE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;

		float angle;
		if (shape == EMITTER_CONE)
		{
			angle = RandomRange(PLAYERPARTICLE_ANGLE_MIN, PLAYERPARTICLE_ANGLE_MAX);
		}
		else if (shape == EMITTER_CIRCLE)
		{
			angle = RandomRange(0.0f, 360.0f);
		}

		ptr->dir = D3DXVECTOR3(cosf(RADIAN(angle)), sinf(RADIAN(angle)), 0.0f);

		ptr->initScale = ptr->scale = RandomRange(PLAYERPARTICLE_SCALE_MIN, maxScale);
		ptr->initSpeed = ptr->speed = RandomRange(PLAYERPARTICLE_SPEED_MIN, PLAYERPARTICLE_SPEED_MAX);

		ptr->lifeFrame = PLAYERPARTICLE_LIFEFRAME_MIN + rand() % PLAYERPARTICLE_LIFEFRAME_MAX;
		ptr->frameCnt = 0;

		ptr->active = true;
		return;
	}
}

/******************************************************************************
//�֐���	�Fvoid SetPlayerParticle(D3DXVECTOR3 pos, int shape, float maxScale)
//����1		�FD3DXVECTOR3 pos	�F�������W
//����2		�Fint shape			�F�����`��
//����3		�Ffloat maxScale	�F�ő�X�P�[��
//�߂�l	�Fvoid
//����		�F�G�l�~�[�p�[�e�B�N���̃G�~�b�^�Z�b�g�֐�
******************************************************************************/
void SetEmitterPlayerParticle(D3DXVECTOR3 pos, int duration, int emitNum, int shape, float maxScale)
{
	EMITTER_PLAYERPARTICLE *emitter = &emitterPlayerParticle[0];

	for (int i = 0; i < PLAYERPARTICLE_EMITTER_MAX; i++, emitter++)
	{
		if (emitter->active)
		{
			continue;
		}

		emitter->pos = pos;
		emitter->frameCnt = 0;
		emitter->duration = duration;
		emitter->shape = shape;
		emitter->emitNum = emitNum;
		emitter->maxScale = maxScale;

		emitter->active = true;
		return;
	}
}
