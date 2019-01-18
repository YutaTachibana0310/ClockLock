//=============================================================================
//
// �X�R�A�w�b�_ [score.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _HISCORE_H_
#define _HISCORE_H_


// �}�N����`
#define HISCORE_TEXTURE_NAME	_T("data/TEXTURE/GUI/hiScoreGUI.png")				// �X�R�A�̃e�N�X�`��

#define HISCOREBACK_TEXTURE_SIZE_X				(100)							//�X�R�A�w�i�e�N�X�`���T�C�YX
#define HISCOREBACK_TEXTURE_SIZE_Y				(375.0f)						//�X�R�A�w�i�e�N�X�`���T�C�YY

#define HISCORENUM_TEXTURE_SIZE_X				(12)							//�����e�N�X�`���T�C�YX
#define HISCORENUM_TEXTURE_SIZE_Y				(16)							//�����e�N�X�`���T�C�YY

#define HISCORE_INITPOS_X						(120)							//�X�R�A��X���W
#define HISCORE_INITPOS_Y						(400)							//�X�R�A��Y���W

#define HISCORENUM_INITPOS_X					(70)							//�X�R�A������X���W
#define HISCORENUM_INITPOS_Y					(-285)							//�X�R�A������Y���W
#define HISCORENUM_OFFSETPOS_X					(-15.0f)						//�X�R�A�����̃I�t�Z�b�g���WX
#define HISCORENUM_OFFSETPOS_Y					(63.0f)							//�X�R�A�����̃I�t�Z�b�g���WY

#define HISCORE_DRAW_MAX						(10)								//�\������n�C�X�R�A�̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitHiScoreGUI(int num);								//�X�R�A����������
void UninitHiScoreGUI(void);									//�X�R�A�I������
void UpdateHiScoreGUI(void);									//�X�R�A�X�V����
void DrawHiscoreNum(void);									//�X�R�A�`�揈���i�����p�j
void DrawHiScoreGUIBack(void);									//�X�R�A�`�揈���i�w�i�p�j

#endif
