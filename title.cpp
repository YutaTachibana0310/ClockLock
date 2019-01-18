//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "title.h"
#include "input.h"
#include "transition.h"
#include "charChip.h"
#include "bgmPlayer.h"
#include "sePlayer.h"
#include "titleGear.h"
#include "bgmPlayer.h"
#include "player.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define TITLE_MINUTEHAND_ANGLE			(30.0f)			//�Z�j����x�ɉ�]����p�x
#define TITLE_MINUTEHAND_ROTFRAME		(60)			//�Z�j�����t���[�������ɉ�]���邩
#define TITLE_HOURHAND_ANGLE			(30.0f/TITLE_MINUTEHAND_ROTFRAME/12) //���j��1�t���[���ɉ�]����p�x

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexTitle(void);				//���_�쐬�֐�
void SetTextureTitle(void);					// �e�N�X�`�����W�̌v�Z����
void SetTextureTitleDifficulty(void);		// �e�N�X�`�����W�̌v�Z�����i��Փx�p�j
void SetTextureTitleGUI(void);				// �e�N�X�`�����W�̌v�Z�����iGUI�p�j

void SetVertexTitle(void);					// ���_�̌v�Z�����i�w�i�p�j
void SetVertexTitleLogo(void);				// ���_�̌v�Z�����i���S�p�j
void SetVertexTitleMinuteHand(void);		// ���_�̌v�Z�����i���j�p�j
void SetVertexTitleHourHand(void);			// ���_�̌v�Z�����i�Z�j�p�j
void SetVertexTitleDifficulty(void);		// ���_�̌v�Z�����i��Փx�p�j
void SetVertexTitleGearCenter(void);		// ���_�̌v�Z�����i���ԗp�j
void SetVertexTitleGUI(void);				// ���_�̌v�Z�����iGUI�p�j
void SetVertexTitleCopyRight(void);			// ���_�̌v�Z�����i�R�s�[���C�g�p�j

//�e�e�N�X�`���`�揈��
void DrawTitleBG(void);
void DrawTitleMinuteHand(void);
void DrawTitleHourHand(void);
void DrawTitleLogo(void);
void DrawTitleDifficulty(void);
void DrawTitleGearCenter(void);
void DrawTitleGUI(void);
void DrawTitleCopyRight(void);

/*****************************************************************************
�\���̒�`
*****************************************************************************/
enum TITLE_TEXTURE
{
	TITLE_BACK,
	TITLE_DIAL,
	TITLE_MINUTEHAND,
	TITLE_HOURHAND,
	TITLE_LOGO,
	TITLE_DIFFICULTY,
	TITLE_GEAR,
	TITLE_GUI,
	TITLE_COPYRIGHT,
	TITLETEX_MAX
};

enum TITLE_STATE
{
	TITLE_INIT,
	TITLE_SELECT,
	TITLE_TRANSITION,
	TITLE_STATEMAX
};

enum TITLE_DIFFICULTY
{
	TITLE_EASY,
	TITLE_NORMAL,
	TITLE_HARD,
	TITLE_DEMO,
	TITLE_DIFFICULTYMAX
};
/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[TITLETEX_MAX];	// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];				//���_���i�[���[�N

static TITLE title;									//�^�C�g��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitTitle(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLE* ptr = GetTitleAdr();

	ptr->pos = D3DXVECTOR3(TITLE_INITPOS_X, TITLE_INITPOS_Y, 0.0f);
	ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	ptr->angle = atan2f(TITLE_TEXTURE_SIZE_Y, TITLE_TEXTURE_SIZE_X);
	ptr->radius = D3DXVec2Length(&D3DXVECTOR2(TITLE_TEXTURE_SIZE_X, TITLE_TEXTURE_SIZE_Y));

	ptr->angleLogo = atan2f(TITLE_LOGO_SIZE_Y, TITLE_LOGO_SIZE_X);
	ptr->radiusLogo = D3DXVec2Length(&D3DXVECTOR2(TITLE_LOGO_SIZE_X, TITLE_LOGO_SIZE_Y));

	ptr->angleDifficulty = atan2f(TITLE_DIFFICULTY_SIZE_Y, TITLE_DIFFICULTY_SIZE_X);
	ptr->radiusDifficulty = D3DXVec2Length(&D3DXVECTOR2(TITLE_DIFFICULTY_SIZE_X, TITLE_DIFFICULTY_SIZE_Y));


	ptr->angleGearBig = atan2f(TITLE_GEARCENTER_SIZE_Y, TITLE_GEARCENTER_SIZE_X);
	ptr->radiusGearBig = D3DXVec2Length(&D3DXVECTOR2(TITLE_GEARCENTER_SIZE_X, TITLE_GEARCENTER_SIZE_Y));

	ptr->angleGUI = atan2f(TITLE_GUITEX_SIZE_Y, TITLE_GUITEX_SIZE_X);
	ptr->radiusGUI = D3DXVec2Length(&D3DXVECTOR2(TITLE_GUITEX_SIZE_X, TITLE_GUITEX_SIZE_Y));

	ptr->state = TITLE_SELECT;


	// ���_���̍쐬
	MakeVertexTitle();
	SetTextureTitle();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[TITLE_BACK] = CreateTextureFromFile((LPSTR)TITLE_BGTEX_NAME, pDevice);
		texture[TITLE_DIAL] = CreateTextureFromFile((LPSTR)TITLE_ANIMTEX0_NAME, pDevice);
		texture[TITLE_MINUTEHAND] = CreateTextureFromFile((LPSTR)TITLE_ANIMTEX1_NAME, pDevice);
		texture[TITLE_HOURHAND] = CreateTextureFromFile((LPSTR)TITLE_ANIMTEX2_NAME, pDevice);
		texture[TITLE_LOGO] = CreateTextureFromFile((LPSTR)TITLE_TEXTURE_NAME, pDevice);
		texture[TITLE_DIFFICULTY] = CreateTextureFromFile((LPSTR)TITLE_DIFFICULTY_NAME, pDevice);
		texture[TITLE_GEAR] = CreateTextureFromFile((LPSTR)TITLE_GEARTEX_NAME, pDevice);
		texture[TITLE_GUI] = CreateTextureFromFile((LPSTR)TITLE_GUITEX_NAME, pDevice);
		texture[TITLE_COPYRIGHT] = CreateTextureFromFile((LPSTR)TITLE_COPYRIGHT_NAME, pDevice);

		//��Փx������
		ptr->difficulty = DIFFICULTY_EASY;
	}

	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���g�厞�̕�Ԑݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���k�����̕�Ԑݒ�

	InitTitleGear(num);

	//FadeinBGM(BGM_TITLE, 6000);
	PlayBGM(BGM_TITLE);
	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitTitle(void)
{
	for (int i = 0; i < TITLETEX_MAX; i++)
	{
		if (texture[i] != NULL)
		{	// �e�N�X�`���̊J��
			texture[i]->Release();
			texture[i] = NULL;
		}
	}

	UninitTitleGear();
}

/******************************************************************************
�X�V����
******************************************************************************/
void UpdateTitle(void)
{
	TITLE *ptr = &title;

	UpdateTitleGear();

	//�^�C�g���A�j���[�V����
	ptr->cntFrame++;
	if (ptr->cntFrame % TITLE_MINUTEHAND_ROTFRAME == 0)
	{
		PlaySE(SE_CLOCK);
	}

	//�@�ے��͈ȍ~�̏������s��Ȃ�
	if (ptr->state == TITLE_TRANSITION)
	{
		return;
	}

	bool flgInput = GetMyButton0Trigger(0) | GetMyButton1Trigger(0) | GetMyButton2Trigger(0);

	//�ŏ��̃{�^�����m
	if (flgInput && ptr->state == TITLE_INIT)
	{
		ptr->state = TITLE_SELECT;
		PlaySE(SE_DECIDE);
		return;
	}

	//��Փx�I��
	else if (ptr->state == TITLE_SELECT)
	{
		//���L�[�œ�Փx�I��
		int x = GetMyAxisXRepeat(0);
		ptr->difficulty = WrapAround(TITLE_EASY, TITLE_DIFFICULTYMAX, ptr->difficulty + x);
		if (x != 0)
		{
			//�I������SE�Đ�
			PlaySE(SE_CURSOR);
		}

		//����{�^���������ꂽ��`���[�g���A���J�n
		if (flgInput && ptr->state == TITLE_SELECT)
		{
			ptr->state = TITLE_TRANSITION;
			PlaySE(SE_DECIDE);
			//StartStage(ptr->difficulty);
			SetDifficulty(ptr->difficulty);
			SetTransition(SCENE_TUTORIAL);
			InitGame(0);
			FadeoutBGM(10);

		}
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawTitle(void)
{
	SetTextureTitle();

	DrawTitleBG();
	DrawTitleGear();
	DrawTitleGearCenter();
	DrawTitleMinuteHand();
	DrawTitleHourHand();
	DrawTitleLogo();
	DrawTitleCopyRight();

	DrawTitleDifficulty();
	DrawTitleGUI();
}

/******************************************************************************
�`�揈���i�w�i�p�j
******************************************************************************/
void DrawTitleBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture[TITLE_BACK]);
	SetVertexTitle();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
�`�揈���i���j�p�j
******************************************************************************/
void DrawTitleHourHand(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture[TITLE_HOURHAND]);

	SetVertexTitleHourHand();

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
�`�揈���i�Z�j�p�j
******************************************************************************/
void DrawTitleMinuteHand(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture[TITLE_MINUTEHAND]);

	SetVertexTitleMinuteHand();

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
�`�揈���i���S�p�j
******************************************************************************/
void DrawTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture[TITLE_LOGO]);

	SetVertexTitleLogo();

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
�`�揈���i��Փx�p�j
******************************************************************************/
void DrawTitleDifficulty(void)
{
	if (title.state == TITLE_INIT)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture[TITLE_DIFFICULTY]);

	SetVertexTitleDifficulty();
	SetTextureTitleDifficulty();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
�`�揈���i���ԗp�j
******************************************************************************/
void DrawTitleGearCenter(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture[TITLE_GEAR]);

	SetVertexTitleGearCenter();

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
�`�揈���iGUI�p�j
******************************************************************************/
void DrawTitleGUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture[TITLE_GUI]);

	SetVertexTitleGUI();
	SetTextureTitleGUI();

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
�`�揈���i�R�s�[���C�g�p�j
******************************************************************************/
void DrawTitleCopyRight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture[TITLE_COPYRIGHT]);

	SetVertexTitleCopyRight();

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexTitle(void)
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
void SetTextureTitle(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�i��Փx�p�j
******************************************************************************/
void SetTextureTitleDifficulty(void)
{
	TITLE *ptr = &title;

	float sizeY = 1.0f / TITLE_DIFFICULTY_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2(0.0, ptr->difficulty * sizeY);
	vertexWk[1].tex = D3DXVECTOR2(1.0, ptr->difficulty * sizeY);
	vertexWk[2].tex = D3DXVECTOR2(0.0, ptr->difficulty * sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2(1.0, ptr->difficulty * sizeY + sizeY);
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�iGUI�p�j
******************************************************************************/
void SetTextureTitleGUI(void)
{
	TITLE *ptr = &title;

	int y = Clamp(0, TITLE_SELECT, ptr->state);
	float sizeY = 1.0f / TITLE_GUITEX_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2(0.0, y * sizeY);
	vertexWk[1].tex = D3DXVECTOR2(1.0, y * sizeY);
	vertexWk[2].tex = D3DXVECTOR2(0.0, y * sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2(1.0, y * sizeY + sizeY);
}

/******************************************************************************
���_���W�̐ݒ�i�w�i�p�j
******************************************************************************/
void SetVertexTitle(void)
{
	vertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
}

/******************************************************************************
���_���W�̐ݒ�i���S�p�j
******************************************************************************/
void SetVertexTitleLogo(void)
{
	static D3DXVECTOR3 pos = D3DXVECTOR3(TITLE_LOGO_INITPOS_X, TITLE_LOGO_INITPOS_Y, 0.0f);
	TITLE *ptr = &title;

	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleLogo) * ptr->radiusLogo;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleLogo) * ptr->radiusLogo;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleLogo) * ptr->radiusLogo;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleLogo) * ptr->radiusLogo;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleLogo) * ptr->radiusLogo;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleLogo) * ptr->radiusLogo;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleLogo) * ptr->radiusLogo;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleLogo) * ptr->radiusLogo;
}

/******************************************************************************
���_���W�̐ݒ�(�Z�j�p�j
******************************************************************************/
void SetVertexTitleMinuteHand(void)
{
	static D3DXVECTOR3 pos = D3DXVECTOR3(TITLE_LOGO_INITPOS_X, TITLE_LOGO_INITPOS_Y, 0.0f);
	TITLE *ptr = &title;
	float angle = (ptr->cntFrame / TITLE_MINUTEHAND_ROTFRAME) * TITLE_MINUTEHAND_ANGLE;
	float rot = RADIAN(angle);

	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleLogo + rot) * ptr->radiusLogo;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleLogo + rot) * ptr->radiusLogo;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleLogo - rot) * ptr->radiusLogo;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleLogo - rot) * ptr->radiusLogo;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleLogo - rot) * ptr->radiusLogo;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleLogo - rot) * ptr->radiusLogo;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleLogo + rot) * ptr->radiusLogo;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleLogo + rot) * ptr->radiusLogo;
}

/******************************************************************************
���_���W�̐ݒ�i���j�p�j
******************************************************************************/
void SetVertexTitleHourHand(void)
{
	static D3DXVECTOR3 pos = D3DXVECTOR3(TITLE_LOGO_INITPOS_X, TITLE_LOGO_INITPOS_Y, 0.0f);
	TITLE *ptr = &title;
	float rot = RADIAN(TITLE_HOURHAND_ANGLE * ptr->cntFrame);

	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleLogo + rot) * ptr->radiusLogo;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleLogo + rot) * ptr->radiusLogo;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleLogo - rot) * ptr->radiusLogo;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleLogo - rot) * ptr->radiusLogo;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleLogo - rot) * ptr->radiusLogo;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleLogo - rot) * ptr->radiusLogo;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleLogo + rot) * ptr->radiusLogo;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleLogo + rot) * ptr->radiusLogo;
}

/******************************************************************************
���_���W�̐ݒ�i��Փx�p�j
******************************************************************************/
void SetVertexTitleDifficulty(void)
{
	static D3DXVECTOR3 pos = D3DXVECTOR3(TITLE_DIFFICULTY_INITPOS_X, TITLE_DIFFICULTY_INITPOS_Y, 0.0f);
	TITLE *ptr = &title;

	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleDifficulty) * ptr->radiusDifficulty;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleDifficulty) * ptr->radiusDifficulty;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleDifficulty) * ptr->radiusDifficulty;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleDifficulty) * ptr->radiusDifficulty;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleDifficulty) * ptr->radiusDifficulty;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleDifficulty) * ptr->radiusDifficulty;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleDifficulty) * ptr->radiusDifficulty;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleDifficulty) * ptr->radiusDifficulty;
}

/******************************************************************************
���_���W�̐ݒ�i���ԗp�j
******************************************************************************/
void SetVertexTitleGearCenter(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(TITLE_LOGO_INITPOS_X, TITLE_LOGO_INITPOS_Y, 0.0f);
	TITLE *ptr = &title;

	//���_���W�̐ݒ�
	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleGearBig) * ptr->radiusGearBig;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleGearBig) * ptr->radiusGearBig;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleGearBig) * ptr->radiusGearBig;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleGearBig) * ptr->radiusGearBig;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleGearBig) * ptr->radiusGearBig;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleGearBig) * ptr->radiusGearBig;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleGearBig) * ptr->radiusGearBig;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleGearBig) * ptr->radiusGearBig;
}

/******************************************************************************
���_���W�̐ݒ�iGUI�p�j
******************************************************************************/
void SetVertexTitleGUI(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(TITLE_DIFFICULTY_INITPOS_X, TITLE_DIFFICULTY_INITPOS_Y, 0.0f);
	TITLE *ptr = &title;

	//���_���W�̐ݒ�
	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleGUI) * ptr->radiusGUI;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleGUI) * ptr->radiusGUI;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleGUI) * ptr->radiusGUI;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleGUI) * ptr->radiusGUI;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleGUI) * ptr->radiusGUI;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleGUI) * ptr->radiusGUI;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleGUI) * ptr->radiusGUI;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleGUI) * ptr->radiusGUI;
}

/******************************************************************************
���_���W�̐ݒ�i�R�s�[���C�g�p�j
******************************************************************************/
void SetVertexTitleCopyRight(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(TITLE_COPYRIGHT_INITPOS_X, TITLE_COPYRIGHT_INITPOS_Y, 0.0f);
	TITLE *ptr = &title;

	//���_���W�̐ݒ�
	vertexWk[0].vtx = pos;
	vertexWk[1].vtx = pos + D3DXVECTOR3(TITLE_COPYRIGHT_SIZE_X, 0.0f, 0.0f);
	vertexWk[2].vtx = pos + D3DXVECTOR3(0.0f, TITLE_COPYRIGHT_SIZE_Y, 0.0f);
	vertexWk[3].vtx = pos + D3DXVECTOR3(TITLE_COPYRIGHT_SIZE_X, TITLE_COPYRIGHT_SIZE_Y, 0.0f);
}

/******************************************************************************
//�֐���	�FTITLE *GetTitleAdr(int pno)
//����		�Fint pno�F�擾�������^�C�g���̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�^�C�g���̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�^�C�g���̃A�h���X�擾�֐�
******************************************************************************/
TITLE *GetTitleAdr(void)
{
	return &title;
}