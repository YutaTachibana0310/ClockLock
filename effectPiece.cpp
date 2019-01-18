//=============================================================================
//
// �G�t�F�N�g�s�[�X���� [effectPiece.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "effectPiece.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define EFFECTPIECE_POS_RANGE				(0.0f)			//�G�t�F�N�g�s�[�X�̍��W�ݒ�͈�
#define EFFECTPIECE_INITSPEED				(10.0f)			//�G�t�F�N�g�s�[�X�̏������x
#define EFFECTPIECE_SLOWFRAME				(16)			//��������t���[��
#define EFFECTPIECE_SCALE_MIN				(0.3f)			//�X�P�[���ŏ��l
#define EFFECTPIECE_SCALE_MAX				(2.5f)			//�X�P�[���ő�l
#define EFFECTPIECE_SPEED_MIN				(0.3f)			//�X�s�[�h�ŏ��l	
#define EFFECTPIECE_SPEED_MAX				(2.5f)			//�X�s�[�h�ő�l

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexEffectPiece(void);						//���_�쐬�֐�
void SetTextureEffectPiece(int cntPattern, EFFECT *ptr);	// �e�N�X�`�����W�̌v�Z����
void SetVertexEffectPiece(EFFECT *ptr);					// ���_�̌v�Z����

/*****************************************************************************
�\���̒�`
*****************************************************************************/

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N
static float radius, angle;

EFFECT effectPiece[EFFECTPIECE_MAX];					//�G�t�F�N�g�s�[�X�z��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitEffectPiece(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT* ptr = GetEffectPieceAdr(0);
	int i;

	angle = atan2f(EFFECTPIECE_TEXTURE_SIZE_Y, EFFECTPIECE_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(EFFECTPIECE_TEXTURE_SIZE_X, EFFECTPIECE_TEXTURE_SIZE_Y));

	for (i = 0; i < EFFECTPIECE_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(EFFECTPIECE_INITPOS_X, EFFECTPIECE_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->animTime = EFFECTPIECE_ANIM_TIME;

		ptr->active = false;
	}

	// ���_���̍쐬
	MakeVertexEffectPiece();

	if(num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)EFFECTPIECE_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitEffectPiece(void)
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
void UpdateEffectPiece(void)
{
	int i;
	EFFECT *ptr = GetEffectPieceAdr(0);

	for (i = 0; i < EFFECTPIECE_MAX; i++, ptr++)
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
			if (ptr->patternAnim >= EFFECTPIECE_ANIM_NUM)
			{
				ptr->active = false;
			}
		}

		//���W�̍X�V
		int animMaxHalf = EFFECTPIECE_ANIM_NUM * ptr->animTime;
		if (ptr->countAnim <= 14)
		{
			ptr->speed = cosf(RADIAN(ptr->countAnim * 90.0f / animMaxHalf)) * ptr->initSpeed;
		}
		ptr->pos += ptr->velocity * ptr->speed;

	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawEffectPiece(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT *ptr = GetEffectPieceAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	//�G�t�F�N�g�s�[�X��`��
	for (i = 0; i < EFFECTPIECE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexEffectPiece(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureEffectPiece(ptr->patternAnim, ptr);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexEffectPiece(void)
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
void SetTextureEffectPiece(int cntPattern, EFFECT *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % EFFECTPIECE_TEXTURE_DIVIDE_X;
	int y = cntPattern / EFFECTPIECE_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / EFFECTPIECE_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / EFFECTPIECE_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexEffectPiece(EFFECT *ptr)
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
//�֐���	�FEFFECTPIECE *GetEffectPieceAdr(int pno)
//����		�Fint pno�F�擾�������G�t�F�N�g�s�[�X�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�G�t�F�N�g�s�[�X�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�G�t�F�N�g�s�[�X�̃A�h���X�擾�֐�
******************************************************************************/
EFFECT *GetEffectPieceAdr(int pno)
{
	if (pno < EFFECTPIECE_MAX)
	{
		return &effectPiece[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�FEFFECT *SetEffectPiece(D3DXVECTOR3 pos)
//����1		�FD3DXVECTOR3 pos		�F�ݒ���W
//����2		�FD3DXVECTOR3 dir		�F�i�ޕ���
//�߂�l	�F�������F�Z�b�g�����G�t�F�N�g�ւ̃|�C���^	���s���FNULL
//����		�F�G�t�F�N�g�s�[�X�̃Z�b�g�֐�
******************************************************************************/
EFFECT *SetEffectPiece(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
{
	int i;
	EFFECT *ptr = GetEffectPieceAdr(0);

	for (i = 0; i < EFFECTPIECE_MAX; i++, ptr++)
	{
		//���g�p�̃G�t�F�N�g�s�[�X������΃Z�b�g
		if (!ptr->active)
		{
			//���W�̐ݒ�
			ptr->pos = pos;
			ptr->pos.x += RandomRange(-EFFECTPIECE_POS_RANGE, EFFECTPIECE_POS_RANGE);
			ptr->pos.y += RandomRange(-EFFECTPIECE_POS_RANGE, EFFECTPIECE_POS_RANGE);
			ptr->rot.z = RandomRange(0.0f, 360.0f);

			//�p�����[�^�ݒ�
			ptr->countAnim = 0;
			ptr->patternAnim = 0;
			ptr->animTime = EFFECTPIECE_ANIM_TIME;
			ptr->velocity = dir;
			ptr->speed = ptr->initSpeed = EFFECTPIECE_INITSPEED * RandomRange(EFFECTPIECE_SPEED_MIN, EFFECTPIECE_SPEED_MAX);
			ptr->scale = RandomRange(EFFECTPIECE_SCALE_MIN, EFFECTPIECE_SCALE_MAX);

			//�Z�b�g
			ptr->active = true;
			return ptr;
		}
	}

	return NULL;
}