//=============================================================================
//
// �`���[�g���A���w�b�_ [Tutorial.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

// �}�N����`
#define TUTORIAL_TEXTURE_NAME	_T("data/TEXTURE/GUI/tutorial.png")		//�`���[�g���A���̃e�N�X�`��
#define TUTORIAL_CHARTEX_NAME	_T("data/TEXTURE/GUI/tutorialChar.png")	//TUTORIAL�����e�N�X�`��
#define TUTORIAL_BGTEX_NAME		_T("data/TEXTURE/bg/tutorialBG.png")	//�`���[�g���A���w�i�̃e�N�X�`��
#define TUTORIAL_SKIPTEX_NAME		_T("data/TEXTURE/GUI/skip.png")		//�X�L�b�v�����̃e�N�X�`��
#define TUTORIAL_SKIPPADTEX_NAME	_T("data/TEXTURE/GUI/skipPad.png")	//�p�b�h�p�̃X�L�b�v����
#define TUTORIAL_PADTEX_NAME	_T("data/TEXTURE/GUI/tutorialPad.png")	//�p�b�h�p�`���[�g���A���̃e�N�X�`��

#define TUTORIAL_DIVIDE_X			(1)					//�`���[�g���A���e�N�X�`���̕�����X
#define TUTORIAL_DIVIDE_Y			(5)					//�`���[�g���A���̃e�N�X�`��������Y

#define TUTORIAL_TEXTURE_SIZE_X		(390)				//�`���[�g���A���e�N�X�`���T�C�YX
#define TUTORIAL_TEXTURE_SIZE_Y		(130)				//�`���[�g���A���e�N�X�`���T�C�YY

#define TUTORIAL_INITPOS_X			(SCREEN_CENTER_X)	//�`���[�g���A���̃e�N�X�`���������WX
#define TUTORIAL_INITPOS_Y			(200)				//�`���[�g���A���̃e�N�X�`���������WY

#define TUTORIAL_CHAR_INITPOS_X		(SCREEN_CENTER_X)	//TUTORIAL�����̏������W
#define TUTORIAL_CHAR_INITPOS_Y		(50)				//TUTORIAL�����̏������W
#define TUTORIAL_CHAR_SIZE_X		(120)				//TUTORIAL�����̃e�N�X�`���T�C�Y
#define TUTORIAL_CHAR_SIZE_Y		(40)				//TUTORIAL�����̃e�N�X�`��

#define TUTORIAL_SKIPTEX_INITPOS_X	(630)				//�X�L�b�v�����̏������WX
#define TUTORIAL_SKIPTEX_INITPOS_Y	(910)				//�X�L�b�v�����̏������WY
#define TUTORIAL_SKIPTEX_SIZE_X		(400)				//�X�L�b�v�����̃e�N�X�`���T�C�YX
#define TUTORIAL_SKIPTEX_SIZE_Y		(50)				//�X�L�b�v�����̃e�N�X�`���T�C�YY

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	int				cntFrame;
	int				tutorialIndex;

	float			angleGuide;
	float			radiusGuide;

	float			angleChar;
	float			radiusChar;

	float			bgOffset;
	int				guideState;
	float			guideAlpha;

	int				buttonCount;
}TUTORIAL;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTutorial(int num);							//�`���[�g���A������������
void UninitTutorial(void);								//�`���[�g���A���I������
void UpdateTutorial(void);								//�`���[�g���A���X�V����
void DrawTutorial(void);								//�`���[�g���A���`�揈��
void DrawTutorialBG(void);								//�`���[�g���A���w�i�`�揈��
#endif
