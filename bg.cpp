//=============================================================================
//
// �w�i���� [bg.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "bg.h"
#include "bgObj.h"
#include "enemyBullet.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/


/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexBG(void);						// ���_���쐬
void SetTextureBG(BG *ptrBG);					// �e�N�X�`���A�j���[�V���� 
void SetVertexBG(void);							// ���_�̌v�Z����
BG *GetBGAdr(int pno);							// �w�i�A�h���X�擾�֐�

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9	timeEffect;							//���ԑ��쒆�̃G�t�F�N�g�e�N�X�`��
static VERTEX_2D			vertexWk[NUM_VERTEX];				// ���_���i�[���[�N
static VERTEX_2D			effectVertexWk[NUM_VERTEX];			//���ԑ���G�t�F�N�g�p���_���
static BG					bg[BG_MAX];							// BG�\����
static int					cntTimeEffect = 0;					//���ԑ��삪�J�n����Ă���̌o�߃t���[��

/******************************************************************************
/ ����������
*****************************************************************************/
HRESULT InitBG(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BG *ptrBG = GetBGAdr(0);

	for (int i = 0; i < BG_MAX; i++, ptrBG++)
	{
		ptrBG->offsetX = 0.0f;
		ptrBG->offsetY = 0.0f;
		// ���_���̍쐬
		MakeVertexBG();
	}

	ptrBG = GetBGAdr(0);

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		ptrBG->tex = CreateTextureFromFile((LPSTR)BG_TEXTURENAME_STAGE1_0, pDevice);
		ptrBG->offsetSpeedY = BG_STAGE1_SPEED1;
		ptrBG++;

		ptrBG->tex = CreateTextureFromFile((LPSTR)BG_TEXTURENAME_STAGE1_1, pDevice);
		ptrBG->offsetSpeedY = BG_STAGE1_SPEED2;
		ptrBG++;

		ptrBG->tex = CreateTextureFromFile((LPSTR)BG_TEXTURENAME_STAGE1_2, pDevice);
		ptrBG->offsetSpeedY = BG_STAGE1_SPEED3;

		timeEffect = CreateTextureFromFile((LPSTR)BG_TEXTURENAME_EFFECT1, pDevice);
	}

	InitBgObj(num);

	return S_OK;
}

/******************************************************************************
// �I������
******************************************************************************/
void UninitBG(void)
{
	int i;
	BG *ptrBG = GetBGAdr(0);

	for (i = 0; i < BG_MAX; i++, ptrBG++)
	{
		if (ptrBG->tex != NULL)
		{	// �e�N�X�`���̊J��
			ptrBG->tex->Release();
			ptrBG->tex = NULL;
		}
	}

	UninitBgObj();
}

/******************************************************************************
// �X�V����
******************************************************************************/
void UpdateBG(void)
{
	BG *ptrBG = GetBGAdr(0);
	int i;

	for (i = 0; i < BG_MAX; i++, ptrBG++)
	{
		ptrBG->offsetY += ptrBG->offsetSpeedY;

		if (ptrBG->offsetX > 1.0f)
		{
			ptrBG->offsetX -= 1.0f;
		}
		if (ptrBG->offsetY > 1.0f)
		{
			ptrBG->offsetY -= 1.0f;
		}
	}

	UpdateBgObj();
}

/********************************************************************************
// �`�揈��
*******************************************************************************/
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BG *ptrBG = GetBGAdr(0);
	SPEEDTYPE type = GetSpeedType();
	int i;

	//pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (i = 0; i < BG_MAX; i++, ptrBG++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, ptrBG->tex);

		//�e�N�X�`�����W�̐ݒ�
		SetTextureBG(ptrBG);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &vertexWk, sizeof(VERTEX_2D));
	}

	DrawBgObj();
	DrawTimeEffect();
}

/********************************************************************************
// �`�揈��(�^�C���G�t�F�N�g)
*******************************************************************************/
void DrawTimeEffect(void)
{
	SPEEDTYPE type = GetSpeedType();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	SetTimeEffect(type);
	//���ԑ��씭�����ł���΃G�t�F�N�g�p�̔w�i��`��
	if (type != DEFAULT_SPEED)
	{
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		pDevice->SetTexture(0, timeEffect);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, effectVertexWk, sizeof(VERTEX_2D));
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

/******************************************************************************
// ���_�̍쐬
*******************************************************************************/
HRESULT MakeVertexBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// rhw�̐ݒ�
	vertexWk[0].rhw =
		vertexWk[1].rhw =
		vertexWk[2].rhw =
		vertexWk[3].rhw = 1.0f;

	effectVertexWk[0].rhw =
		effectVertexWk[1].rhw =
		effectVertexWk[2].rhw =
		effectVertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	//���_���W�̐ݒ�
	SetVertexBG();


	return S_OK;
}

/******************************************************************************
// �e�N�X�`�����W�̐ݒ�
******************************************************************************/
void SetTextureBG(BG *ptrBG)
{
	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2(ptrBG->offsetX, ptrBG->offsetY);
	vertexWk[1].tex = D3DXVECTOR2(ptrBG->offsetX + 1.0f, ptrBG->offsetY);
	vertexWk[2].tex = D3DXVECTOR2(ptrBG->offsetX, ptrBG->offsetY + 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(ptrBG->offsetX + 1.0f, ptrBG->offsetY + 1.0f);

}


/*******************************************************************************
// ���_���W�̐ݒ�
*******************************************************************************/
void SetVertexBG(void)
{
	/*�w�i�͉�]�����Ȃ��̂ō���̒��_����ɂ��Ē��_���W��ݒ肷��*/
	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(BG_POS_X, 0.0f, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(BG_POS_X + BG_TEXTURESIZE_X, 0.0f, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(BG_POS_X, BG_TEXTURESIZE_Y, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(BG_POS_X + BG_TEXTURESIZE_X, BG_TEXTURESIZE_Y, 0.0f);

	effectVertexWk[0].vtx = D3DXVECTOR3(BG_POS_X, 0.0f, 0.0f);
	effectVertexWk[1].vtx = D3DXVECTOR3(BG_POS_X + BG_TEXTURESIZE_X, 0.0f, 0.0f);
	effectVertexWk[2].vtx = D3DXVECTOR3(BG_POS_X, BG_TEXTURESIZE_Y, 0.0f);
	effectVertexWk[3].vtx = D3DXVECTOR3(BG_POS_X + BG_TEXTURESIZE_X, BG_TEXTURESIZE_Y, 0.0f);
}

/****************************************************************
�֐���	�FBG *GetBGAdr(int pno)
����	�F�w�i�A�h���X�擾�֐�
����	�Fint pno�F�A�h���X���擾������BG�̃C���f�b�N�X
�߂�l	�Fbg[i]�̃A�h���X
*****************************************************************/
BG *GetBGAdr(int pno)
{
	return &bg[pno];
}

/****************************************************************
�֐���	�Fvoid SetTimeEffect(void)
����	�F���ԑ���G�t�F�N�g�w�i�̃Z�b�g�֐�
����	�F
�߂�l	�Fvoid
*****************************************************************/
void SetTimeEffect(SPEEDTYPE type)
{
	//���_�̔��ˌ��̐ݒ�
	cntTimeEffect += 10;
	float tmp = (cntTimeEffect + 1 > 100) ? 1.0f : cntTimeEffect / 100.0f;
	if (type == SLOW_ORANGE)
	{
		effectVertexWk[0].diffuse =
			effectVertexWk[1].diffuse =
			effectVertexWk[2].diffuse =
			effectVertexWk[3].diffuse = D3DCOLOR_RGBA((int)(255 - tmp * 78), 100, 0, 200);
	}
	else if (type == SLOW_GREEN)
	{
		effectVertexWk[0].diffuse =
			effectVertexWk[1].diffuse =
			effectVertexWk[2].diffuse =
			effectVertexWk[3].diffuse = D3DCOLOR_RGBA(0, (int)(255 - tmp * 78), 0, 200);
	}
	else if (type == SLOW_BOTH)
	{
		effectVertexWk[0].diffuse =
			effectVertexWk[1].diffuse =
			effectVertexWk[2].diffuse =
			effectVertexWk[3].diffuse = D3DCOLOR_RGBA(0, 0, (int)(255 - tmp * 78), 200);
	}
	else
	{
		cntTimeEffect = 0;
		effectVertexWk[0].diffuse =
			effectVertexWk[1].diffuse =
			effectVertexWk[2].diffuse =
			effectVertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//�e�N�X�`�����W��ݒ�
	effectVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	effectVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	effectVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	effectVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}
