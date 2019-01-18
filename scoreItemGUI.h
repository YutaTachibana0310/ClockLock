//=============================================================================
//
// �X�R�A�A�C�e��GUI�w�b�_ [ScoreItemGUI.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _SCOREITEMGUI_H_
#define _SCOREITEMGUI_H_


// �}�N����`
#define SCOREITEMGUI_TEXTURE_NAME	_T("data/TEXTURE/GUI/itemGUI.png") // �X�R�A�A�C�e��GUI�̃e�N�X�`��
#define SCOREITEMGUI_NUMTEX_NAME	_T("data/TEXTURE/GUI/scoreNum.png")  //�X�R�A�A�C�e��GUI�w�i�̃e�N�X�`��
#define SCOREITEMGUI_TEXTURE_NUM					(2)					//�X�R�A�A�C�e��GUI�̃e�N�X�`���̐�
		
#define SCOREITEMGUI_TEXTURE_SIZE_X					(100)				// �e�N�X�`���T�C�YX
#define SCOREITEMGUI_TEXTURE_SIZE_Y					(75)				// �e�N�X�`���T�C�YY

#define SCOREITEMGUI_NUMTEX_SIZE_X					(15)				//�����e�N�X�`���̃T�C�Y�iX)
#define SCOREITEMGUI_NUMTEX_SIZE_Y					(20)				//�����e�N�X�`���̃T�C�Y�iY)

#define SCOREITEMGUI_TEXTURE_DIVIDE_X				(10)				// �A�j���p�^�[���̃e�N�X�`�����������iX)

#define SCOREITEMGUI_MAX							(5)					//�X�R�A�A�C�e��GUI�̍ő吔

#define SCOREITEMGUI_INITPOS_X						(1160)				//�X�R�A�A�C�e��GUI�̏���X���W
#define SCOREITEMGUI_INITPOS_Y						(500)				//�X�R�A�A�C�e��GUI�̏���Y���W

#define SCOREITEMGUI_NUM_INITPOS_X					(70)				//�����e�N�X�`���̏������W�iX)
#define SCOREITEMGUI_NUM_INITPOS_Y					(25)				 //�����e�N�X�`���̏������W�iY)
#define SCOREITEMGUI_NUM_OFFSETPOS					(-20.0f)			//�����e�N�X�`���̃I�t�Z�b�g���W

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitScoreItemGUI(int num);							//�X�R�A�A�C�e��GUI����������
void UninitScoreItemGUI(void);								//�X�R�A�A�C�e��GUI�I������
void UpdateScoreItemGUI(void);								//�X�R�A�A�C�e��GUI�X�V����
void DrawScoreItemGUI(void);								//�X�R�A�A�C�e��GUI�`�揈���i�����p�j
void DrawScoreItemGUIBack(void);							//�X�R�A�A�C�e��GUI�`�揈���i�w�i�p�j
#endif
