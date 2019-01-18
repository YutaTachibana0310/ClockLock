//=============================================================================
//
// �G�l�~�[�Q�[�W���� [enemyGauge.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "enemyGauge.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define ENEMYGAUGE_SPEED (-40.0f)

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexEnemyGauge(void);									//���_�쐬�֐�
void SetTextureEnemyGauge(int cntPattern, ENEMYGAUGE *ptr);			//�e�N�X�`�����W�̌v�Z����
void SetTextureEnemyGaugeFrame(int cntPattern, ENEMYGAUGE *ptr);	//�e�N�X�`�����W�̌v�Z�����i�t���[���p�j
void SetVertexEnemyGauge(ENEMYGAUGE *ptr);							//���_�̌v�Z����
void SetVertexEnemyGaugeFrame(ENEMYGAUGE *ptr);						//���_�̌v�Z�����i�t���[���p�j
void SetDiffuseEnemyGauge(ENEMYGAUGE *ptr);							//���_�F�̌v�Z����
void ResetDiffuseEnemyGauge();										//���_�F�̃��Z�b�g����

/*****************************************************************************
�\���̒�`
*****************************************************************************/

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture = NULL;				// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9		textureFrame = NULL;		// �e�N�X�`���ւ̃|�C���^�i�t���[���p)
static LPDIRECT3DTEXTURE9		textureBG = NULL;			// �e�N�X�`���ւ̃|�C���^�i�w�i�p)

ENEMYGAUGE enemyGauge[ENEMYGAUGE_MAX];						//�v���C���[�o���b�g�z��

static VERTEX_2D		vertexWk[NUM_VERTEX];				//���_���i�[���[�N

/******************************************************************************
����������
******************************************************************************/
HRESULT InitEnemyGauge(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMYGAUGE* ptr = GetEnemyGaugeAdr(0);
	int i;

	for (i = 0; i < ENEMYGAUGE_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(ENEMYGAUGE_INITPOS_X, ENEMYGAUGE_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->active = false;
	}

	// ���_���̍쐬
	MakeVertexEnemyGauge();

	if(num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)ENEMYGAUGE_TEXTURE_NAME, pDevice);
		textureFrame = CreateTextureFromFile((LPSTR)ENEMYGAUGEFRAME_TEXTURE_NAME, pDevice);
		textureBG = CreateTextureFromFile((LPSTR)ENEMYGAUGEBG_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitEnemyGauge(void)
{
	if (texture != NULL)
	{	// �e�N�X�`���̊J��
		texture->Release();
		texture = NULL;
	}

	if (textureFrame != NULL)
	{
		textureFrame->Release();
		textureFrame = NULL;
	}
}

/******************************************************************************
�X�V����
******************************************************************************/
void UpdateEnemyGauge(void)
{
	//�Q�[�W�̍��W�X�V�A�X�P�[���X�V�͐e�i�G�l�~�[�j�̍X�V�������ōs��
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawEnemyGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMYGAUGE *ptr = GetEnemyGaugeAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�Q�[�W��`��
	for (i = 0; i < ENEMYGAUGE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//�Q�[�W�w�i��`��
			pDevice->SetTexture(0, textureBG);
			ResetDiffuseEnemyGauge();
			SetVertexEnemyGaugeFrame(ptr);
			SetTextureEnemyGaugeFrame(ptr->patternAnim, ptr);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

			//�Q�[�W��`��
			pDevice->SetTexture(0, texture);
			SetVertexEnemyGauge(ptr);
			SetTextureEnemyGauge(ptr->patternAnim, ptr);
			SetDiffuseEnemyGauge(ptr);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

			//�Q�[�W�t���[����`��
			pDevice->SetTexture(0, textureFrame);
			ResetDiffuseEnemyGauge();
			SetVertexEnemyGaugeFrame(ptr);
			SetTextureEnemyGaugeFrame(ptr->patternAnim, ptr);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexEnemyGauge(void)
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
�e�N�X�`�����W�̐ݒ�(�Q�[�W�p)
******************************************************************************/
void SetTextureEnemyGauge(int cntPattern, ENEMYGAUGE *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % ENEMYGAUGE_TEXTURE_DIVIDE_X;
	int y = cntPattern / ENEMYGAUGE_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / ENEMYGAUGE_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / ENEMYGAUGE_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ptr->scale, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ptr->scale, (float)(y)* sizeY + sizeY);
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�(�Q�[�W�t���[���p)
******************************************************************************/
void SetTextureEnemyGaugeFrame(int cntPattern, ENEMYGAUGE *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % ENEMYGAUGE_TEXTURE_DIVIDE_X;
	int y = cntPattern / ENEMYGAUGE_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / ENEMYGAUGE_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / ENEMYGAUGE_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

/******************************************************************************
���_���W�̐ݒ�(�Q�[�W�p)
******************************************************************************/
void SetVertexEnemyGauge(ENEMYGAUGE *ptr)
{
	float gaugePosX = ptr->pos.x - ENEMYGAUGE_TEXTURE_SIZE_X / 2;
	float gaugePosY = ptr->pos.y - ENEMYGAUGE_TEXTURE_SIZE_Y / 2;

	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(gaugePosX, gaugePosY, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(gaugePosX + ENEMYGAUGE_TEXTURE_SIZE_X * ptr->scale, gaugePosY, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(gaugePosX, gaugePosY + ENEMYGAUGE_TEXTURE_SIZE_Y, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(gaugePosX + ENEMYGAUGE_TEXTURE_SIZE_X * ptr->scale, gaugePosY + ENEMYGAUGE_TEXTURE_SIZE_Y, 0.0f);
}

/******************************************************************************
���_���W�̐ݒ�(�Q�[�W�t���[���p)
******************************************************************************/
void SetVertexEnemyGaugeFrame(ENEMYGAUGE *ptr)
{
	float gaugePosX = ptr->pos.x - ENEMYGAUGE_TEXTURE_SIZE_X / 2;
	float gaugePosY = ptr->pos.y - ENEMYGAUGE_TEXTURE_SIZE_Y / 2;

	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(gaugePosX, gaugePosY, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(gaugePosX + ENEMYGAUGE_TEXTURE_SIZE_X, gaugePosY, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(gaugePosX, gaugePosY + ENEMYGAUGE_TEXTURE_SIZE_Y, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(gaugePosX + ENEMYGAUGE_TEXTURE_SIZE_X, gaugePosY + ENEMYGAUGE_TEXTURE_SIZE_Y, 0.0f);
}

/******************************************************************************
���_�F�̐ݒ�
******************************************************************************/
void SetDiffuseEnemyGauge(ENEMYGAUGE *ptr)
{
	//�΂��物�F�ւ̑J��
	if (ptr->scale > 0.5f)
	{
		vertexWk[0].diffuse =
			vertexWk[1].diffuse =
			vertexWk[2].diffuse =
			vertexWk[3].diffuse = D3DCOLOR_RGBA((int)((1.0f - ptr->scale) * 510), 255, 0, 255);
	}
	//���F����Ԃւ̑J��
	else if (ptr->scale > 0.1f)
	{
		vertexWk[0].diffuse =
			vertexWk[1].diffuse =
			vertexWk[2].diffuse =
			vertexWk[3].diffuse = D3DCOLOR_RGBA(255, (int)((ptr->scale * 2.5f - 0.25f) * 255), 0, 255);
	}
	//�c�菭�Ȃ����͐Ԃ̂�
	else
	{
		vertexWk[0].diffuse =
			vertexWk[1].diffuse =
			vertexWk[2].diffuse =
			vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	}
}

/******************************************************************************
���_�F�̃��Z�b�g
******************************************************************************/
void ResetDiffuseEnemyGauge()
{
	vertexWk[0].diffuse =
		vertexWk[1].diffuse =
		vertexWk[2].diffuse =
		vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}

/******************************************************************************
//�֐���	�FENEMYGAUGE *GetEnemyGaugeAdr(int pno)
//����		�Fint pno�F�擾�������v���C���[�o���b�g�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�v���C���[�o���b�g�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�v���C���[�o���b�g�̃A�h���X�擾�֐��i���C���V���b�g�j
******************************************************************************/
ENEMYGAUGE *GetEnemyGaugeAdr(int pno)
{
	if (pno < ENEMYGAUGE_MAX)
	{
		return &enemyGauge[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�FENEMYGAUGE *GetNonActiveEnemyGauge(void)
//����		�Fvoid
//�߂�l	�F��A�N�e�B�u�̃G�l�~�[�Q�[�W�ւ̃|�C���^
//����		�F��A�N�e�B�u�̃G�l�~�[�Q�[�W���擾����֐�
******************************************************************************/
ENEMYGAUGE *GetNonActiveEnemyGauge(void)
{
	int i;
	ENEMYGAUGE *ptr = GetEnemyGaugeAdr(0);

	for (i = 0; i < ENEMYGAUGE_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			ptr->scale = 1.0f;
			ptr->active = true;
			return ptr;
		}
	}

	return NULL;
}