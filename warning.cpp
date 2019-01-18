//=============================================================================
//
// ���[�j���O���� [warning.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "warning.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define WARNING_TEXT_MOVELENGTH			(5.0f)	//�e�L�X�g�e�N�X�`���������͈�
#define WARNING_ROLL_SCROLLVALUE		(0.005f)	//�e�N�X�`���̃X�N���[����

#define WARNING_OPEN_FRAME				(60)	//OPEN��Ԃ���J�ڂ��鎞��
#define WARNING_OFFSETPOS_MAX			(80.0f)	//offsetPos�ő�l
#define WARNING_OFFSETPOS_ADDVALUE		(2.0f)	//offsetPos���Z�l

#define WARNING_DEFAULT_FRAME			(300)	//DEFAULT��Ԃ���J�ڂ��鎞��

#define WARNING_BG_DIFFUSE_MIN			(0)
#define WARNING_BG_DIFFUSE_MAX			(60)

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexWarning(void);				//���_�쐬�֐�
void SetTextureWarningText(int num);			//�e�N�X�`�����W�̌v�Z����
void SetTextureWarningRoll(int side);			//�e�N�X�`�����W�̌v�Z�����i�сj
void SetTextureWarningBG(void);					//�e�N�X�`�����W�̌v�Z�����i�w�i�j
void SetVertexWarningText(void);				//���_�̌v�Z����
void SetVertexWarningExclamation(void);			//���_�̌v�Z����
void SetVertexWarningRoll(int side);			//���_�̌v�Z�����i�сj
void SetVertexWarningBG(void);					//���_�̌v�Z�����i�w�i�j

/*****************************************************************************
�\���̒�`
*****************************************************************************/
enum WARNING_ROLLSIDE
{
	WARNING_TOP,
	WARNING_BOTTOM
};

enum WARNING_TEXTURE
{
	WARNING_EXCLAMATION,
	WARNING_WARNING,
};

enum WARNING_TEXINDEX
{
	WARNING_ROLL,
	WARNING_TEXT,
	WARNING_BG,
	WARNING_TEXMAX
};

enum WARNING_STATE
{
	WARNING_OPEN,
	WARNING_DEFAULT,
	WARNING_STATEMAX
};

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[WARNING_TEXMAX];				// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N

WARNING warning;										//���[�j���O�\����

/******************************************************************************
����������
******************************************************************************/
HRESULT InitWarning(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	WARNING* ptr = &warning;
	
	ptr->angle = atan2f(WARNING_TEXTURE_SIZE_Y, WARNING_TEXTURE_SIZE_X);
	ptr->radius = D3DXVec2Length(&D3DXVECTOR2(WARNING_TEXTURE_SIZE_X, WARNING_TEXTURE_SIZE_Y));

	ptr->angleRoll = atan2f(WARNING_ROLL_SIZE_Y, WARNING_ROLL_SIZE_X);
	ptr->radiusRoll = D3DXVec2Length(&D3DXVECTOR2(WARNING_ROLL_SIZE_X, WARNING_ROLL_SIZE_Y));

	ptr->angleBG = atan2f(WARNING_BG_SIZE_Y, WARNING_BG_SIZE_X);
	ptr->radiusBG = D3DXVec2Length(&D3DXVECTOR2(WARNING_BG_SIZE_X, WARNING_BG_SIZE_Y));

	ptr->countFrame = 0;
	ptr->offsetPos = 0.0f;
	ptr->offsetUV = 0.0f;
	ptr->countDiffuse = 0;
	ptr->flgDiffuse = true;
	ptr->state = WARNING_OPEN;
	ptr->active = false;


	// ���_���̍쐬
	MakeVertexWarning();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[WARNING_TEXT] = CreateTextureFromFile((LPSTR)WARNING_TEXTURE_NAME, pDevice);
		texture[WARNING_ROLL] = CreateTextureFromFile((LPSTR)WARNING_ROLLTEX_NAME, pDevice);
		texture[WARNING_BG] = CreateTextureFromFile((LPSTR)WARNING_BG_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitWarning(void)
{
	for (int i = 0; i < WARNING_TEXMAX; i++)
	{
		if (texture[i] != NULL)
		{	// �e�N�X�`���̊J��
			texture[i]->Release();
			texture[i] = NULL;
		}
	}
}

/******************************************************************************
�X�V����
******************************************************************************/
void UpdateWarning(void)
{
	WARNING *ptr = &warning;

	//��A�N�e�B�u�ł���Ώ������s��Ȃ�
	if (!ptr->active)
	{
		return;
	}

	ptr->countFrame++;

	//�w�i�̃A���t�@�v�Z
	ptr->countDiffuse += (ptr->flgDiffuse) ? 1 : -1;
	if (ptr->countDiffuse == WARNING_BG_DIFFUSE_MAX || ptr->countDiffuse == WARNING_BG_DIFFUSE_MIN)
	{
		ptr->flgDiffuse = !ptr->flgDiffuse;
	}

	//OPEN��Ԃ̏���
	if (ptr->state == WARNING_OPEN)
	{
		ptr->offsetPos = Clampf(0.0f, WARNING_OFFSETPOS_MAX, ptr->offsetPos +WARNING_OFFSETPOS_ADDVALUE);
		ptr->offsetUV += WARNING_ROLL_SCROLLVALUE;

		if (ptr->countFrame == WARNING_OPEN_FRAME)
		{
			ptr->state = WARNING_DEFAULT;
			ptr->countFrame = 0;
		}
	}

	//DEFAULT��Ԃ̏���
	else if (ptr->state == WARNING_DEFAULT)
	{
		ptr->offsetUV += WARNING_ROLL_SCROLLVALUE;

		if (ptr->countFrame == WARNING_DEFAULT_FRAME)
		{
			ptr->active = false;
		}
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawWarning(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	WARNING *ptr = &warning;

	if (!ptr->active)
	{
		return;
	}

	//�w�i�̕`��
	pDevice->SetTexture(0, texture[WARNING_BG]);
	SetTextureWarningBG();
	SetVertexWarningBG();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//�т�`��
	pDevice->SetTexture(0, texture[WARNING_ROLL]);

	SetVertexWarningRoll(WARNING_TOP);
	SetTextureWarningRoll(WARNING_TOP);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	SetVertexWarningRoll(WARNING_BOTTOM);
	SetTextureWarningRoll(WARNING_BOTTOM);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//�e�L�X�g��`��(DEFAULT��Ԃ̂�)
	if (ptr->state == WARNING_DEFAULT)
	{
		pDevice->SetTexture(0, texture[WARNING_TEXT]);

		SetVertexWarningExclamation();
		SetTextureWarningText(WARNING_EXCLAMATION);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

		SetVertexWarningText();
		SetTextureWarningText(WARNING_TEXT);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexWarning(void)
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
void SetTextureWarningText(int num)
{
	float sizeY = 1.0f / WARNING_TEXTURE_DIVIDE_Y;

	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2(0.0f, num * sizeY);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, num * sizeY);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, num * sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, num * sizeY + sizeY);
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�i�сj
******************************************************************************/
void SetTextureWarningRoll(int side)
{
	WARNING *ptr = &warning;

	vertexWk[0].tex = D3DXVECTOR2(ptr->offsetUV, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(ptr->offsetUV + 1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(ptr->offsetUV, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(ptr->offsetUV + 1.0f, 1.0f);
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�i�w�i�j
******************************************************************************/
void SetTextureWarningBG()
{
	WARNING *ptr = &warning;

	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexWarningText(void)
{
	WARNING *ptr = &warning;
	D3DXVECTOR3 pos = D3DXVECTOR3(WARNING_INITPOS_X, WARNING_INITPOS_Y, 0.0f);
	pos.x += RandomRange(-WARNING_TEXT_MOVELENGTH, WARNING_TEXT_MOVELENGTH);
	pos.y += RandomRange(-WARNING_TEXT_MOVELENGTH, WARNING_TEXT_MOVELENGTH);

	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = pos.x - cosf(ptr->angle) * ptr->radius;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angle) * ptr->radius;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angle) * ptr->radius;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angle) * ptr->radius;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angle) * ptr->radius;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angle) * ptr->radius;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angle) * ptr->radius;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angle) * ptr->radius;
}

/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexWarningExclamation(void)
{
	WARNING *ptr = &warning;
	D3DXVECTOR3 pos = D3DXVECTOR3(WARNING_INITPOS_X, WARNING_INITPOS_Y, 0.0f);

	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = pos.x - cosf(ptr->angle) * ptr->radius;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angle) * ptr->radius;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angle) * ptr->radius;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angle) * ptr->radius;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angle) * ptr->radius;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angle) * ptr->radius;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angle) * ptr->radius;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angle) * ptr->radius;
}

/******************************************************************************
���_���W�̐ݒ�(��)
******************************************************************************/
void SetVertexWarningRoll(int side)
{
	WARNING *ptr = &warning;
	D3DXVECTOR3 pos = D3DXVECTOR3(WARNING_INITPOS_X, WARNING_INITPOS_Y, 0.0f);
	pos.y += (side == WARNING_TOP) ? ptr->offsetPos : -1 * ptr->offsetPos;
	int sign = (side == WARNING_TOP) ? 1 : -1;

	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = pos.x - sign * cosf(ptr->angleRoll) * ptr->radiusRoll;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleRoll) * ptr->radiusRoll;
	vertexWk[1].vtx.x = pos.x + sign * cosf(ptr->angleRoll) * ptr->radiusRoll;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleRoll) * ptr->radiusRoll;
	vertexWk[2].vtx.x = pos.x - sign * cosf(ptr->angleRoll) * ptr->radiusRoll;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleRoll) * ptr->radiusRoll;
	vertexWk[3].vtx.x = pos.x + sign * cosf(ptr->angleRoll) * ptr->radiusRoll;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleRoll) * ptr->radiusRoll;

}

/******************************************************************************
���_���W�̐ݒ�(�w�i)
******************************************************************************/
void SetVertexWarningBG(void)
{
	WARNING *ptr = &warning;
	D3DXVECTOR3 pos = D3DXVECTOR3(WARNING_BG_INITPOS_X, WARNING_BG_INITPOS_Y, 0.0f);

	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleBG) * ptr->radiusBG;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleBG) * ptr->radiusBG;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleBG) * ptr->radiusBG;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleBG) * ptr->radiusBG;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleBG) * ptr->radiusBG;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleBG) * ptr->radiusBG;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleBG) * ptr->radiusBG;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleBG) * ptr->radiusBG;

	int alpha = (int)(255 * (float)ptr->countDiffuse / WARNING_BG_DIFFUSE_MAX);
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
}

/******************************************************************************
//�֐���	�FWARNING *GetWarningAdr(int pno)
//����		�Fvoid
//�߂�l	�F����Ȉ����F���[�j���O�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F���[�j���O�̃A�h���X�擾�֐�
******************************************************************************/
WARNING *GetWarningAdr(void)
{
	return &warning;
}

/******************************************************************************
//�֐���	�Fvoid SetWarning(void)
//����		�Fvoid
//�߂�l	�Fvoid
//����		�G���[�j���O�̃Z�b�g�֐�
******************************************************************************/
void SetWarning(void)
{
	WARNING *ptr = &warning;

	ptr->countFrame = 0;

	ptr->offsetPos = 0.0f;
	ptr->offsetUV = 0.0f;
	ptr->state = WARNING_OPEN;

	ptr->active = true;
}