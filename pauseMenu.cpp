//=============================================================================
//
// �|�[�Y���j���[���� [pauseMenu.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "pauseMenu.h"
#include "input.h"
#include "transition.h"
/*****************************************************************************
�}�N����`
*****************************************************************************/
#define PAUSEMENU_OPEN_FRAME		(10)
#define PAUSEMENU_CLOSE_FRAME		(10)

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexPauseMenu(void);							//���_�쐬�֐�
void SetTexturePauseMenu(void);								// �e�N�X�`�����W�̌v�Z����
void SetTexturePauseCursor(int cntPattern);					//�e�N�X�`�����W�̌v�Z�����i�J�[�\���p�j
void SetVertexPauseMenu(PAUSEMENU *ptr);					// ���_�̌v�Z����
void UpdatePauseMenuOpen(PAUSEMENU *ptr);					//�X�V����
void UpdatePauseMenuSelect(PAUSEMENU *ptr);					//�X�V����
void UpdatePauseMenuClose(PAUSEMENU *ptr);					//�X�V����
void ProcessSelectedPauseMenu(PAUSEMENU *ptr);				//���j���[����
void DrawPauseMenuBack(PAUSEMENU *ptr);						//�`�揈���i�w�i�p�j
void DrawPauseMenuCursor(PAUSEMENU *ptr);					//�`�揈���i�J�[�\���p�j

/*****************************************************************************
�\���̒�`
*****************************************************************************/
//�|�[�Y���j���[�̑I�����j���[�񋓌^
enum PAUSE_MENU
{
	MENU_RESUME,
	MENU_RETRY,
	MENU_RETURN,
	MENU_MAX
};

//�|�[�Y���j���[�̃e�N�X�`���񋓑�
enum PAUSEMENU_TEXTURE
{
	BACK,
	CURSOR,
	TEXTURE_MAX
};

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[TEXTURE_MAX];				// �e�N�X�`���ւ̃|�C���^

static VERTEX_2D vertexWk[NUM_VERTEX];						//���_���i�[���[�N
static float angle, radius;									//�|���S���̔��a�A�p�x

PAUSEMENU pauseMenu[PAUSEMENU_MAX];							//�|�[�Y���j���[�z��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitPauseMenu(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PAUSEMENU* ptr = GetPauseMenuAdr(0);
	int i;

	for (i = 0; i < PAUSEMENU_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(PAUSEMENU_INITPOS_X, PAUSEMENU_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		angle = atan2f(PAUSEMENU_TEXTURE_SIZE_Y, PAUSEMENU_TEXTURE_SIZE_X);
		radius = D3DXVec2Length(&D3DXVECTOR2(PAUSEMENU_TEXTURE_SIZE_X, PAUSEMENU_TEXTURE_SIZE_Y));

		ptr->state = PAUSEMENU_NONE;
		ptr->scale = 0.0f;
		ptr->select = MENU_RESUME;

		ptr->active = false;
	}

	// ���_���̍쐬
	MakeVertexPauseMenu();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[BACK] = CreateTextureFromFile((LPSTR)PAUSEMENU_TEXTURE_NAME, pDevice);
		texture[CURSOR] = CreateTextureFromFile((LPSTR)PAUSEMENU_CURSORTEX_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitPauseMenu(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
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
void UpdatePauseMenu(void)
{
	PAUSEMENU *ptr = GetPauseMenuAdr(0);

	ptr->cntFrame++;
	switch (ptr->state)
	{
	case PAUSEMENU_OPEN:
		UpdatePauseMenuOpen(ptr);
		break;

	case PAUSEMENU_SELECT:
		UpdatePauseMenuSelect(ptr);
		break;

	case PAUSEMENU_CLOSE:
		UpdatePauseMenuClose(ptr);
		break;
	}

}

/******************************************************************************
�X�V�����iOPEN��ԁj
******************************************************************************/
void UpdatePauseMenuOpen(PAUSEMENU *ptr)
{
	if (ptr->cntFrame > PAUSEMENU_OPEN_FRAME)
	{
		ptr->state = PAUSEMENU_SELECT;
		return;
	}

	ptr->scale += 1.0f / PAUSEMENU_OPEN_FRAME;
}

/******************************************************************************
�X�V�����iSELECT��ԁj
******************************************************************************/
void UpdatePauseMenuSelect(PAUSEMENU *ptr)
{
	ptr->select = WrapAround(MENU_RESUME, MENU_MAX, ptr->select - GetMyAxisYTrigger(0));

	if (GetMyButton0Trigger(0))
	{
		ptr->state = PAUSEMENU_CLOSE;
		ptr->cntFrame = 0;
	}
}

/******************************************************************************
�X�V�����iCLOSE��ԁj
******************************************************************************/
void UpdatePauseMenuClose(PAUSEMENU *ptr)
{
	if (ptr->cntFrame > PAUSEMENU_CLOSE_FRAME)
	{
		ptr->state = PAUSEMENU_NONE;
		ptr->cntFrame = 0;

		ProcessSelectedPauseMenu(ptr);
		return;
	}

	ptr->scale -= 1.0f / PAUSEMENU_CLOSE_FRAME;
}

/******************************************************************************
�I�����ꂽ���j���[�̏���
******************************************************************************/
void ProcessSelectedPauseMenu(PAUSEMENU *ptr)
{
	switch (ptr->select)
	{
	case MENU_RESUME:
		Pause(false);
		ResumeGame();
		break;

	case MENU_RETRY:
		Pause(false);
		//StartStage(0);
		SetTransition(SCENE_GAME);
		break;

	case MENU_RETURN:
		Pause(false);
		SetTransition(SCENE_TITLE);
		break;
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawPauseMenu(void)
{
	PAUSEMENU *ptr = GetPauseMenuAdr(0);

	DrawPauseMenuBack(ptr);
	DrawPauseMenuCursor(ptr);
}

/******************************************************************************
�X�V�����i�w�i�p�j
******************************************************************************/
void DrawPauseMenuBack(PAUSEMENU *ptr)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, texture[BACK]);

	SetVertexPauseMenu(ptr);
	SetTexturePauseMenu();
	
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
�X�V�����i�J�[�\���p�j
******************************************************************************/
void DrawPauseMenuCursor(PAUSEMENU *ptr)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, texture[CURSOR]);

	SetTexturePauseCursor(ptr->select);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexPauseMenu(void)
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
void SetTexturePauseMenu(void)
{
	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�i�J�[�\���p�j
******************************************************************************/
void SetTexturePauseCursor(int cntPattern)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % PAUSEMENU_CURSORTEX_DIVIDE_X;
	float sizeX = 1.0f / PAUSEMENU_CURSORTEX_DIVIDE_X;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 1.0f);
}

/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexPauseMenu(PAUSEMENU *ptr)
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
//�֐���	�FPAUSEMENU *GetPauseMenuAdr(int pno)
//����		�Fint pno�F�擾�������|�[�Y���j���[�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�|�[�Y���j���[�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�|�[�Y���j���[�̃A�h���X�擾�֐�
******************************************************************************/
PAUSEMENU *GetPauseMenuAdr(int pno)
{
	if (pno < PAUSEMENU_MAX)
	{
		return &pauseMenu[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�Fvoid SetPauseMenustate(PAUSEMENU_STATE state)
//����		�FPAUSEMENU_STATE state		:�Z�b�g�������|�[�Y���j���[�̏��
//�߂�l	�Fvoid
//����		�F�|�[�Y���j���[�̏�ԃZ�b�g�֐�
******************************************************************************/
void SetPauseMenuState(PAUSEMENU_STATE state)
{
	PAUSEMENU *ptr = GetPauseMenuAdr(0);
	ptr->cntFrame = 0;
	ptr->state = state;
}