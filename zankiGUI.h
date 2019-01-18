//=============================================================================
//
// �c�@GUI�w�b�_ [ZankiGUI.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _ZANKIGUI_H_
#define _ZANKIGUI_H_


// �}�N����`
#define ZANKIGUI_TEXTURE_NAME	_T("data/TEXTURE/GUI/zanki.png")		//�c�@GUI�̃e�N�X�`���i�A�C�R���j
#define ZANKIGUI_BACKTEX_NAME	_T("data/TEXTURE/GUI/zankiBack.png")	//�c�@GUI�̃e�N�X�`���i�w�i�p�j

#define ZANKIGUI_TEXTURE_NUM					(2)						//�e�N�X�`���̐�

#define ZANKIGUI_TEXTURE_SIZE_X					(30)					//�e�N�X�`���T�C�YX�i�A�C�R���p�j
#define ZANKIGUI_TEXTURE_SIZE_Y					(30)					//�e�N�X�`���T�C�YY�i�A�C�R���p�j

#define ZANKIGUI_BACKTEX_SIZE_X					(100)					// �e�N�X�`���T�C�YX(�w�i�p)
#define ZANKIGUI_BACKTEX_SIZE_Y					(75)					// �e�N�X�`���T�C�YY(�w�i�p)

#define ZANKIGUI_INITPOS_X						(-30)					//�c�@GUI�̏���X���W�i�A�C�R���p�j
#define ZANKIGUI_INITPOS_Y						(0)					//�c�@GUI�̏���Y���W�i�A�C�R���p�j

#define ZANKIGUI_BACK_INITPOS_X					(960)					//�c�@GUI�̏���X���W�i�w�i�p�j
#define ZANKIGUI_BACK_INITPOS_Y					(30)					//�c�@GUI�̏���Y���W�i�w�i�p�j

#define ZANKIGUI_CHAR_SIZE_X					(10)					//�����e�N�X�`���̃T�C�YX
#define ZANKIGUI_CHAR_SIZE_Y					(10)					//�����e�N�X�`���̃T�C�YY
#define ZANKIGUI_CHAR_INITPOS_X					(10.0f)					//�����e�N�X�`���̏������WX
#define ZANKIGUI_CHAR_INITPOS_Y					(0.0f)					//�����e�N�X�`���̏������WY

#define ZANKIGUI_OFFSETPOS						(30.0f)					//�c�@�A�C�R���̃I�t�Z�b�g�l

#define ZANKIGUI_NUM_INITPOS_X					(35.0f)					//�c�@���̏������WX
#define ZANKIGUI_NUM_INITPOS_Y					(0.0f)					//�c�@���̏������WY
#define ZANKIGUI_NUM_OFFSETPOS					(20.0f)					//�c�@���̃I�t�Z�b�g
#define ZANKIGUI_NUM_SIZE_X						(15)
#define ZANKIGUI_NUM_SIZE_Y						(25)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitZankiGUI(int num);							//�c�@GUI����������
void UninitZankiGUI(void);								//�c�@GUI�I������
void UpdateZankiGUI(void);								//�c�@GUI�X�V����
void DrawZankiGUI(void);								//�c�@GUI�`�揈��(�A�C�R���p)
void DrawZankiGUIBack(void);							//�c�@GUI�`�揈���i�w�i�p�j

#endif
