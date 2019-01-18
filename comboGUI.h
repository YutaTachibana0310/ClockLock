//=============================================================================
//
// �R���{GUI�w�b�_ [ComboGUI.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _COMBOGUI_H_
#define _COMBOGUI_H_


// �}�N����`
#define COMBOGUI_TEXTURE_NAME	_T("data/TEXTURE/GUI/comboGUI.png")	// �R���{GUI�̃e�N�X�`��

#define COMBOGUI_TEXTURE_SIZE_X					(100)				// �e�N�X�`���T�C�YX
#define COMBOGUI_TEXTURE_SIZE_Y					(75)				// �e�N�X�`���T�C�YY

#define COMBOGUI_INITPOS_X						(1160)				//�R���{GUI�̏���X���W
#define COMBOGUI_INITPOS_Y						(300)				//�R���{GUI�̏���Y���W

#define COMBOGUI_NUM_SIZE_X						(15)				//�����e�N�X�`���̃T�C�YX
#define COMBOGUI_NUM_SIZE_Y						(35)				//�����e�N�X�`���̃T�C�YY

#define COMBOGUI_NUM_INITPOS_X					(-20)				//�����e�N�X�`���̏������WX
#define COMBOGUI_NUM_INITPOS_Y					(25)				//�����e�N�X�`���̏������WY
#define COMBOGUI_NUM_OFFSET						(-20.0f)			//�����e�N�X�`���̃I�t�Z�b�g�l

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitComboGUI(int num);							//�R���{GUI����������
void UninitComboGUI(void);								//�R���{GUI�I������
void UpdateComboGUI(void);								//�R���{GUI�X�V����
void DrawComboGUI(void);								//�R���{GUI�`�揈���i�����j
void DrawComboGUIBack(void);							//�R���{GUI�`�揈���i�w�i�j
#endif
