//=============================================================================
//
// �G�i�W�[GUI�w�b�_ [EnergyGUI.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _ENERGYGUI_H_
#define _ENERGYGUI_H_


// �}�N����`
#define ENERGYGUI_TEXTURE_NAME	_T("data/TEXTURE/GUI/energyGauge.png")		// �G�i�W�[GUI�̃e�N�X�`��
#define ENERGYGUI_FRAMETEX_NAME	_T("data/TEXTURE/GUI/energyGaugeFrame.png") //�G�i�W�[GUI�̃t���[���e�N�X�`��
#define ENERGYGUI_BACKTEX_NAME	_T("data/TEXTURE/GUI/EnergyGUI.png")		//�G�i�W�[GUI�̔w�i�e�N�X�`��

#define ENERGYGUI_TEXTURE_SIZE_X					(178)					// �e�N�X�`���T�C�YX
#define ENERGYGUI_TEXTURE_SIZE_Y					(133.0f)				// �e�N�X�`���T�C�YY

#define ENERGYGUI_BACKTEX_SIZE_X					(100)					//�G�i�W�[GUI�̔w�i�e�N�X�`���T�C�YX
#define ENERGYGUI_BACKTEX_SIZE_Y					(75)					//�G�i�W�[GUI�̔w�i�e�N�X�`���T�C�YY

#define ENERGYGUI_NUMTEX_SIZE_X						(12)					//�G�i�W�[GUI�̐����e�N�X�`���̃T�C�YX
#define ENERGYGUI_NUMTEX_SIZE_Y						(12)					//�G�i�W�[GUI�̐����e�N�X�`���̃T�C�YY

#define ENERGYGUI_INITPOS_X							(1160)					//�G�i�W�[GUI�̏���X���W
#define ENERGYGUI_INITPOS_Y							(100)					//�G�i�W�[GUI�̏���Y���W

#define ENERGYGUI_GAUGE_INITPOS_X					(-88)					//�G�i�W�[GUI�̃Q�[�W�������WX
#define ENERGYGUI_GAUGE_INITPOS_Y					(-65)					//�G�i�W�[GUI�̃Q�[�W�������WY

#define ENERGYGUI_NUM_INITPOS_X						(-5)						//�G�i�W�[GUI�̐����������WX
#define ENERGYGUI_NUM_INITPOS_Y						(47)					//�G�i�W�[GUI�̐����������WY
#define ENERGYGUI_NUM_OFFSET						(-15.0f)				//�G�i�W�[GUI�̐����I�t�Z�b�g�l
//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnergyGUI(int num);							//�G�i�W�[GUI����������
void UninitEnergyGUI(void);								//�G�i�W�[GUI�I������
void UpdateEnergyGUI(void);								//�G�i�W�[GUI�X�V����
void DrawEnergyGUI(void);								//�G�i�W�[GUI�`�揈��
void DrawEnergyGUIBack(void);							//�G�i�W�[GUI�`�揈���i�w�i�p�j
void DrawEnergyGUINum(void);							//�G�i�W�[GUI�`�揈���i�����p�j
#endif
