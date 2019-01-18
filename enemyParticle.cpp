//=============================================================================
//
// �G�l�~�[�p�[�e�B�N������ [enemyParticle.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "enemyParticle.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define ENEMYPARTICLE_LIFEFRAME_MAX			(60)
#define ENEMYPARTICLE_LIFEFRAME_MIN			(30)

#define ENEMYPARTICLE_SPEED_MAX				(15.0f)
#define ENEMYPARTICLE_SPEED_MIN				(1.0f)

#define ENEMYPARTICLE_SCALE_MAX				(1.2f)
#define ENEMYPARTICLE_SCALE_MIN				(0.5f)

#define ENEMYPARTICLE_ANGLE_MIN				(60.0f)
#define ENEMYPARTICLE_ANGLE_MAX				(120.0f)


/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexEnemyParticle(void);				//���_�쐬�֐�
void SetTextureEnemyParticle(void);		// �e�N�X�`�����W�̌v�Z����
void SetVertexEnemyParticle(ENEMYPARTICLE *ptr);	// ���_�̌v�Z����

/*****************************************************************************
�\���̒�`
*****************************************************************************/

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N

ENEMYPARTICLE enemyParticle[ENEMYPARTICLE_MAX];				//�G�l�~�[�p�[�e�B�N���z��
EMITTER_ENEMYPARTICLE	emitterEnemyParticle[ENEMYPARTICLE_EMITTER_MAX];

/******************************************************************************
����������
******************************************************************************/
HRESULT InitEnemyParticle(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMYPARTICLE* ptr = GetEnemyParticleAdr(0);
	EMITTER_ENEMYPARTICLE *emitter = &emitterEnemyParticle[0];
	int i;

	for (i = 0; i < ENEMYPARTICLE_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(ENEMYPARTICLE_INITPOS_X, ENEMYPARTICLE_INITPOS_Y, 0.0f);

		ptr->angle = atan2f(ENEMYPARTICLE_TEXTURE_SIZE_Y, ENEMYPARTICLE_TEXTURE_SIZE_X);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(ENEMYPARTICLE_TEXTURE_SIZE_X, ENEMYPARTICLE_TEXTURE_SIZE_Y));

		ptr->frameCnt = 0;
		ptr->speed = 0;
		ptr->dir = VECTOR3_ZERO;
		ptr->scale = 1.0f;

		ptr->active = false;
	}

	for (i = 0; i < ENEMYPARTICLE_EMITTER_MAX; i++, emitter++)
	{
		emitter->pos = VECTOR3_ZERO;
		emitter->active = false;
		emitter->frameCnt = 0;
		emitter->duration = 0;
	}

	// ���_���̍쐬
	MakeVertexEnemyParticle();

	if(num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)ENEMYPARTICLE_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitEnemyParticle(void)
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
void UpdateEnemyParticle(void)
{
	ENEMYPARTICLE *ptr = &enemyParticle[0];
	EMITTER_ENEMYPARTICLE *emitter = &emitterEnemyParticle[0];

	//�G�~�b�^�̍X�V
	for (int i = 0; i < ENEMYPARTICLE_EMITTER_MAX; i++, emitter++)
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
			SetEnemyParticle(emitter->pos, emitter->shape);

		}
	}

	//�p�[�e�B�N���̍X�V
 	for (int i = 0; i < ENEMYPARTICLE_MAX; i++, ptr++)
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
void DrawEnemyParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMYPARTICLE *ptr = GetEnemyParticleAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	//�G�l�~�[�p�[�e�B�N�������Z�����ŕ`��
	for (i = 0; i < ENEMYPARTICLE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexEnemyParticle(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureEnemyParticle();

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexEnemyParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// rhw�̐ݒ�
	vertexWk[0].rhw =
		vertexWk[1].rhw =
		vertexWk[2].rhw =
		vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);

	return S_OK;
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�
******************************************************************************/
void SetTextureEnemyParticle(void)
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
void SetVertexEnemyParticle(ENEMYPARTICLE *ptr)
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
//�֐���	�FENEMYPARTICLE *GetEnemyParticleAdr(int pno)
//����		�Fint pno�F�擾�������G�l�~�[�p�[�e�B�N���̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�G�l�~�[�p�[�e�B�N���̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�G�l�~�[�p�[�e�B�N���̃A�h���X�擾�֐�
******************************************************************************/
ENEMYPARTICLE *GetEnemyParticleAdr(int pno)
{
	if (pno < ENEMYPARTICLE_MAX)
	{
		return &enemyParticle[pno];
	}
	else
	{
		return NULL;
	}
}

void SetEnemyParticle(D3DXVECTOR3 pos, int shape)
{
	ENEMYPARTICLE *ptr = &enemyParticle[0];

	for (int i = 0; i < ENEMYPARTICLE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;

		float angle;
		if (shape == EMITTER_CONE)
		{
			angle = RandomRange(ENEMYPARTICLE_ANGLE_MIN, ENEMYPARTICLE_ANGLE_MAX);
		}
		else if (shape == EMITTER_CIRCLE)
		{
			angle = RandomRange(0.0f, 360.0f);
		}

		ptr->dir = D3DXVECTOR3(cosf(RADIAN(angle)), sinf(RADIAN(angle)), 0.0f);

		ptr->initScale = ptr->scale = RandomRange(ENEMYPARTICLE_SCALE_MIN, ENEMYPARTICLE_SCALE_MAX);
		ptr->initSpeed = ptr->speed = RandomRange(ENEMYPARTICLE_SPEED_MIN, ENEMYPARTICLE_SPEED_MAX);

		ptr->lifeFrame = ENEMYPARTICLE_LIFEFRAME_MIN + rand() % ENEMYPARTICLE_LIFEFRAME_MAX;
		ptr->frameCnt = 0;

		ptr->active = true;
		return;
	}
}

void SetEmitterEnemyParticle(D3DXVECTOR3 pos, int duration, int emitNum, int shape)
{
	EMITTER_ENEMYPARTICLE *emitter = &emitterEnemyParticle[0];

	for (int i = 0; i < ENEMYPARTICLE_EMITTER_MAX; i++, emitter++)
	{
		if (emitter->active)
		{
			continue;
		}

		emitter->pos = pos;
		emitter->frameCnt = 0;
		emitter->shape = shape;
		emitter->duration = duration;
		emitter->emitNum = emitNum;

		emitter->active = true;
		return;
	}
}