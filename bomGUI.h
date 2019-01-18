//=============================================================================
//
// �{��GUI�w�b�_ [BomGUI.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _BOMGUI_H_
#define _BOMGUI_H_


// �}�N����`
#define BOMGUI_TEXTURE_NAME	_T("data/TEXTURE/GUI/bomIcon.png")			//�{��GUI�̃e�N�X�`���i�A�C�R���j
#define BOMGUI_BACKTEX_NAME	_T("data/TEXTURE/GUI/bomGUI.png")			//�{��GUI�̃e�N�X�`���i�w�i�p�j

#define BOMGUI_TEXTURE_NUM						(2)						//�e�N�X�`���̐�

#define BOMGUI_TEXTURE_SIZE_X					(25)					//�e�N�X�`���T�C�YX�i�A�C�R���p�j
#define BOMGUI_TEXTURE_SIZE_Y					(25)					//�e�N�X�`���T�C�YY�i�A�C�R���p�j

#define BOMGUI_BACKTEX_SIZE_X					(100)					// �e�N�X�`���T�C�YX(�w�i�p)
#define BOMGUI_BACKTEX_SIZE_Y					(75)					// �e�N�X�`���T�C�YY(�w�i�p)

#define BOMGUI_INITPOS_X						(0)					//�{��GUI�̏���X���W�i�A�C�R���p�j
#define BOMGUI_INITPOS_Y						(20)					//�{��GUI�̏���Y���W�i�A�C�R���p�j

#define BOMGUI_BACK_INITPOS_X					(1000)					//�{��GUI�̏���X���W�i�w�i�p�j
#define BOMGUI_BACK_INITPOS_Y					(60)					//�{��GUI�̏���Y���W�i�w�i�p�j

#define BOMGUI_OFFSETPOS						(-30.0f)					//�{���A�C�R���̃I�t�Z�b�g�l

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBomGUI(int num);							//�{��GUI����������
void UninitBomGUI(void);								//�{��GUI�I������
void UpdateBomGUI(void);								//�{��GUI�X�V����
void DrawBomGUI(void);								//�{��GUI�`�揈��(�A�C�R���p)
void DrawBomGUIBack(void);							//�{��GUI�`�揈���i�w�i�p�j

#endif
