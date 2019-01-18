//=============================================================================
//
// �`���[�g���A������ [tutorial.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "bg.h"
#include "tutorial.h"
#include "tutorialPlayer.h"
#include "bulletFire.h"
#include "playerBullet.h"
#include "tutorialEnemy.h"
#include "player.h"
#include "input.h"
#include "transition.h"
#include "bgmPlayer.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define TUTORIAL_BG_SCROLLSPEED		(-0.003f)		//�w�i�̃X�N���[���X�s�[�h
#define TUTORIAL_GUIDE_FADEFRAME	(60)			//�K�C�h���t�F�[�h���鎞��
#define TUTORIAL_GUIDE_VIEWFRAME	(360)			//�K�C�h��\�����鎞��
#define TUTORIAL_SKIP_COUNT			(30)			//���̃t���[�����A�X�y�[�X�������ꂽ��X�L�b�v
#define TUTORIAL_END_WAIT			(120)			//�`���[�g���A���I����̑ҋ@�t���[��

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
void SetVertexTutorialGuide(void);			//���_���W�ݒ菈���i�K�C�h�p�j
void SetVertexTutorialChar(void);			//���_���W�ݒ菈���iTUTORIAL�p�j
void SetVertexTutorialBG(void);				//���_���W�ݒ菈���i�w�i�p�j
void SetVertexTutorialSkip(void);			//���_���W�ݒ菈���i�X�L�b�v�p�j

void SetTextureTutorialGuide(void);			//�e�N�X�`�����W�ݒ菈���i�K�C�h�p�j
void SetTextureTutorialChar(void);			//�e�N�X�`�����W�ݒ菈���iTUTORIAL�p�j
void SetTextureTutorialBG(void);			//�e�N�X�`�����W�ݒ菈���i�w�i�p�j

HRESULT MakeVertexTutorial(void);			//���_�쐬����

void SetDiffuseTutorialGuide(void);			//�f�B�t���[�Y�ݒ菈��

/*****************************************************************************
�\���̒�`
*****************************************************************************/
//�`���[�g���A���e�N�X�`���ʂ��ԍ�
enum TUTORIAL_TEXTURE
{
	TUTORIAL_BG,
	TUTORIAL_GUIDE,
	TUTORIAL_CHAR,
	TUTORIAL_SKIP,
	TUTORIAL_SKIPPAD,
	TUTORIAL_GUIDEPAD,
	TUTORIAL_TEXMAX
};

//�`���[�g���A���̃K�C�h�ʂ��ԍ�
enum TUTORIAL_GUIDEINDEX
{
	TUTORIAL_ATTACKGUIDE,
	TUTORIAL_LOCKGUIDE1,
	TUTORIAL_LOCKGUIDE2,
	TUTORIAL_BOMBERGUIDE,
	TUTORIAL_ITEMGUIDE,
	TUTORIAL_GUIDEMAX
};

//�`���[�g���A���̃K�C�h�\�����
enum TUTORIAL_GUIDESTATE
{
	TUTORIAL_GUIDE_FADEIN,
	TUTORIAL_GUIDE_NOTFADE,
	TUTORIAL_GUIDE_FADEOUT,
	TUTORIAL_GUIDE_STATEMAX
};
/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static TUTORIAL tutorial;								//�`���[�g���A���\����
static LPDIRECT3DTEXTURE9 texture[TUTORIAL_TEXMAX];		//�e�N�X�`��
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���
static VERTEX_2D GUIvertexWk[NUM_VERTEX];				//�K�C�h�p�̒��_���

/******************************************************************************
����������
******************************************************************************/
HRESULT InitTutorial(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TUTORIAL *ptr = &tutorial;

	ptr->angleGuide = atan2f(TUTORIAL_TEXTURE_SIZE_Y, TUTORIAL_TEXTURE_SIZE_X);
	ptr->radiusGuide = D3DXVec2Length(&D3DXVECTOR2(TUTORIAL_TEXTURE_SIZE_X, TUTORIAL_TEXTURE_SIZE_Y));

	ptr->angleChar = atan2f(TUTORIAL_CHAR_SIZE_Y, TUTORIAL_CHAR_SIZE_X);
	ptr->radiusChar = D3DXVec2Length(&D3DXVECTOR2(TUTORIAL_CHAR_SIZE_X, TUTORIAL_CHAR_SIZE_Y));

	ptr->bgOffset = 0.0f;
	ptr->guideState = TUTORIAL_GUIDE_FADEIN;
	ptr->guideAlpha = 0.0f;

	ptr->buttonCount = 0;
	ptr->cntFrame = 0;
	ptr->tutorialIndex = 0;

	//�e�N�X�`���u�����f�B���O��ݒ�
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	MakeVertexTutorial();

	if (num == 0)
	{
		texture[TUTORIAL_BG] = CreateTextureFromFile((LPSTR)TUTORIAL_BGTEX_NAME, pDevice);
		texture[TUTORIAL_GUIDE] = CreateTextureFromFile((LPSTR)TUTORIAL_TEXTURE_NAME, pDevice);
		texture[TUTORIAL_CHAR] = CreateTextureFromFile((LPSTR)TUTORIAL_CHARTEX_NAME, pDevice);
		texture[TUTORIAL_SKIP] = CreateTextureFromFile((LPSTR)TUTORIAL_SKIPTEX_NAME, pDevice);
		texture[TUTORIAL_GUIDEPAD] = CreateTextureFromFile((LPSTR)TUTORIAL_PADTEX_NAME, pDevice);
		texture[TUTORIAL_SKIPPAD] = CreateTextureFromFile((LPSTR)TUTORIAL_SKIPPADTEX_NAME, pDevice);
	}

	InitTutorialEnemy(num);

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitTutorial(void)
{
	//�e�N�X�`���̊J��
	for (int i = 0; i < TUTORIAL_TEXMAX; i++)
	{
		if (texture[i] != NULL)
		{
			texture[i]->Release();
			texture[i] = NULL;
		}
	}
}

/******************************************************************************
�X�V����
******************************************************************************/
void UpdateTutorial(void)
{
	TUTORIAL *ptr = &tutorial;
	PLAYER *player = GetPlayerAdr(0);

	//�`���[�g���A���G�l�~�[�X�V����
	UpdateTutorialEnemy();

	ptr->cntFrame++;

	//�w�i�X�N���[��
	ptr->bgOffset += TUTORIAL_BG_SCROLLSPEED;
	if (ptr->bgOffset > 1.0f)
	{
		ptr->bgOffset -= 1.0f;
	}

	//�X�y�[�X�L�[����莞�ԉ����ꑱ���Ă�����`���[�g���A���X�L�b�v
	if (GetKeyboardPress(DIK_SPACE) || IsButtonPressed(0, BUTTON_M))
	{
		ptr->buttonCount++;
		if (ptr->buttonCount == TUTORIAL_SKIP_COUNT)
		{
			DisableTutorialEnemy();
			SetTransition(SCENE_GAME);
		}
	}
	else
	{
		//�L�[��������Ă��Ȃ�������J�E���g���Z�b�g
		ptr->buttonCount = 0;
	}

	//�`���[�g���A�����Ō�܂ŏI�������Ԍo�߂�����Q�[���X�^�[�g
	if (ptr->tutorialIndex == TUTORIAL_GUIDEMAX && ptr->cntFrame == TUTORIAL_END_WAIT)
	{
		DisableTutorialEnemy();
		SetTransition(SCENE_GAME);
	}

	//�`���[�g���A�����I�����Ă�����ȉ��̏����͍s��Ȃ�
	if (ptr->tutorialIndex == TUTORIAL_GUIDEMAX)
	{
		return;
	}

	switch (ptr->guideState)
	{
	case TUTORIAL_GUIDE_FADEIN:
		ptr->guideAlpha += 1.0f;
		//�w�莞�Ԃ��o�߂��Ă�����t�F�[�h�C���I��
		if (ptr->cntFrame == TUTORIAL_GUIDE_FADEFRAME)
		{
			ptr->guideState = TUTORIAL_GUIDE_NOTFADE;
			ptr->cntFrame = 0;
		}
		break;

	case TUTORIAL_GUIDE_NOTFADE:
		//�w�莞�Ԃ��o�߂��Ă�����t�F�[�h�A�E�g�J�n
		if (ptr->cntFrame == TUTORIAL_GUIDE_VIEWFRAME)
		{
			ptr->guideState = TUTORIAL_GUIDE_FADEOUT;
			ptr->cntFrame = 0;
		}
		break;

	case TUTORIAL_GUIDE_FADEOUT:
		//�w�莞�Ԃ��o�߂��Ă�����K�C�h�؂�ւ�
		ptr->guideAlpha -= 1.0f;
		if (ptr->cntFrame == TUTORIAL_GUIDE_FADEFRAME)
		{
			ptr->guideState = TUTORIAL_GUIDE_FADEIN;
			ptr->cntFrame = 0;

			ptr->tutorialIndex++;

			//�`���[�g���A���i�s�x�ɂ���ăv���C���[�̋@�\���b�N����
			if (ptr->tutorialIndex == TUTORIAL_LOCKGUIDE1)
			{
				player->lockTimeEffect = false;
			}
			else if (ptr->tutorialIndex == TUTORIAL_BOMBERGUIDE)
			{
				player->lockBomber = false;
			}
		}
		break;
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TUTORIAL *ptr = &tutorial;

	DrawTutorialEnemy();

	//TUTORIAL�����`��
	pDevice->SetTexture(0, texture[TUTORIAL_CHAR]);
	SetVertexTutorialChar();
	SetTextureTutorialChar();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//�X�L�b�v�`��
	pDevice->SetTexture(0, texture[(GetPadCount == 0) ? TUTORIAL_SKIP : TUTORIAL_SKIPPAD]);
	SetVertexTutorialSkip();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//�K�C�h�`��(tutorialIndex��MAX�����ł����)
	if (ptr->tutorialIndex < TUTORIAL_GUIDEMAX)
	{
		LPDIRECT3DTEXTURE9 tex = (GetPadCount() > 0) ? texture[TUTORIAL_GUIDEPAD] : texture[TUTORIAL_GUIDE];
		pDevice->SetTexture(0, tex);

		SetVertexTutorialGuide();
		SetTextureTutorialGuide();
		SetDiffuseTutorialGuide();
		
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, GUIvertexWk, sizeof(VERTEX_2D));
	}
}

/******************************************************************************
�`�揈��(�w�i)
******************************************************************************/
void DrawTutorialBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->SetTexture(0, texture[TUTORIAL_BG]);
	SetVertexTutorialBG();
	SetTextureTutorialBG();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�i�K�C�h�p�j
******************************************************************************/
void SetTextureTutorialGuide(void)
{
	TUTORIAL *ptr = &tutorial;

	int y = ptr->tutorialIndex;
	float sizeY = 1.0f / TUTORIAL_DIVIDE_Y;

	GUIvertexWk[0].tex = D3DXVECTOR2(0.0f, y * sizeY);
	GUIvertexWk[1].tex = D3DXVECTOR2(1.0f, y * sizeY);
	GUIvertexWk[2].tex = D3DXVECTOR2(0.0f, y * sizeY + sizeY);
	GUIvertexWk[3].tex = D3DXVECTOR2(1.0f, y * sizeY + sizeY);
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�iTUTORIAL�p�j
******************************************************************************/
void SetTextureTutorialChar(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�i�w�i�p�j
******************************************************************************/
void SetTextureTutorialBG(void)
{
	TUTORIAL *ptr = &tutorial;

	vertexWk[0].tex = D3DXVECTOR2(0.0f, ptr->bgOffset);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, ptr->bgOffset);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, ptr->bgOffset + 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, ptr->bgOffset + 1.0f);
}

/******************************************************************************
���_���W�̐ݒ�i�K�C�h�p�j
******************************************************************************/
void SetVertexTutorialGuide(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(TUTORIAL_INITPOS_X, TUTORIAL_INITPOS_Y, 0.0f);
	TUTORIAL *ptr = &tutorial;

	GUIvertexWk[0].vtx.x = pos.x - cosf(ptr->angleGuide) * ptr->radiusGuide;
	GUIvertexWk[0].vtx.y = pos.y - sinf(ptr->angleGuide) * ptr->radiusGuide;
	GUIvertexWk[1].vtx.x = pos.x + cosf(ptr->angleGuide) * ptr->radiusGuide;
	GUIvertexWk[1].vtx.y = pos.y - sinf(ptr->angleGuide) * ptr->radiusGuide;
	GUIvertexWk[2].vtx.x = pos.x - cosf(ptr->angleGuide) * ptr->radiusGuide;
	GUIvertexWk[2].vtx.y = pos.y + sinf(ptr->angleGuide) * ptr->radiusGuide;
	GUIvertexWk[3].vtx.x = pos.x + cosf(ptr->angleGuide) * ptr->radiusGuide;
	GUIvertexWk[3].vtx.y = pos.y + sinf(ptr->angleGuide) * ptr->radiusGuide;
}

/******************************************************************************
���_���W�̐ݒ�iTUTORIAL�p�j
******************************************************************************/
void SetVertexTutorialChar(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(TUTORIAL_CHAR_INITPOS_X, TUTORIAL_CHAR_INITPOS_Y, 0.0f);
	TUTORIAL *ptr = &tutorial;

	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleChar) * ptr->radiusChar;
}

/******************************************************************************
���_���W�̐ݒ�iTUTORIAL�p�j
******************************************************************************/
void SetVertexTutorialBG(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(TUTORIAL_CHAR_INITPOS_X, TUTORIAL_CHAR_INITPOS_Y, 0.0f);

	vertexWk[0].vtx = D3DXVECTOR3(BG_POS_X, 0.0f, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(BG_POS_X + BG_TEXTURESIZE_X, 0.0f, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(BG_POS_X, BG_TEXTURESIZE_Y, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(BG_POS_X + BG_TEXTURESIZE_X, BG_TEXTURESIZE_Y, 0.0f);
}

/******************************************************************************
���_���쐬
******************************************************************************/
HRESULT MakeVertexTutorial(void)
{
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

	// rhw�̐ݒ�
	GUIvertexWk[0].rhw =
		GUIvertexWk[1].rhw =
		GUIvertexWk[2].rhw =
		GUIvertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	GUIvertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	GUIvertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	GUIvertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	GUIvertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

	return S_OK;
}

/******************************************************************************
�f�B�t���[�Y�ݒ菈��
******************************************************************************/
void SetDiffuseTutorialGuide(void)
{
	TUTORIAL *ptr = &tutorial;
	int alpha = Clamp(0, 255, (int)(255 * ptr->guideAlpha / TUTORIAL_GUIDE_FADEFRAME));

	GUIvertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	GUIvertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	GUIvertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
	GUIvertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
}

/******************************************************************************
���_���W�ݒ菈���i�X�L�b�v�����p�j
******************************************************************************/
void SetVertexTutorialSkip(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(TUTORIAL_SKIPTEX_INITPOS_X, TUTORIAL_SKIPTEX_INITPOS_Y, 0.0f);

	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + TUTORIAL_SKIPTEX_SIZE_X, pos.y, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y + TUTORIAL_SKIPTEX_SIZE_Y, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + TUTORIAL_SKIPTEX_SIZE_X, pos.y + TUTORIAL_SKIPTEX_SIZE_Y, 0.0f);
}