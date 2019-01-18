//=============================================================================
//
// �����`�b�v�w�b�_ [CharChip.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _CHARCHIP_H_
#define _CHARCHIP_H_

// �}�N����`
#define CHARCHIP_TEXTURE_NAME	_T("data/TEXTURE/GUI/charchip.png")	// �����`�b�v�̃e�N�X�`��

#define CHARCHIP_TEXTURE_DIVIDE_X				(15)				// �e�N�X�`�����������iX)
#define CHARCHIP_TEXTURE_DIVIDE_Y				(2)					// �e�N�X�`�����������iY)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
enum DEFINE_CHARCHIP
{
	CHARCHIP_A,
	CHARCHIP_B,
	CHARCHIP_C,
	CHARCHIP_D,
	CHARCHIP_E,
	CHARCHIP_F,
	CHARCHIP_G,
	CHARCHIP_H,
	CHARCHIP_I,
	CHARCHIP_J,
	CHARCHIP_K,
	CHARCHIP_L,
	CHARCHIP_M,
	CHARCHIP_N,
	CHARCHIP_O,
	CHARCHIP_P,
	CHARCHIP_Q,
	CHARCHIP_R,
	CHARCHIP_S,
	CHARCHIP_T,
	CHARCHIP_U,
	CHARCHIP_V,
	CHARCHIP_W,
	CHARCHIP_X,
	CHARCHIP_Y,
	CHARCHIP_Z,
	CHARCHIP_ASTERISK,
	CHARCHIP_EXCLAMATION,
	CHARCHIP_QUESTION,
	CHARCHIP_END,
	CHARCHIP_MAX,
	CHARCHIP_NONE
};
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitCharChip(int num);							//�����`�b�v����������
void UninitCharChip(void);								//�����`�b�v�I������
void DrawCharChip(VERTEX_2D *vertexWk, int charchipID);		//�����`�b�v�`�揈��

#endif
