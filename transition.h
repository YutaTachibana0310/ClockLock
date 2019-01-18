//=============================================================================
//
// �g�����W�V�����w�b�_ [Transition.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _TRANSITION_H_
#define _TRANSITION_H_


// �}�N����`
#define TRANSITION_TEXTURE_NAME	_T("data/TEXTURE/GUI/shutter.png")			//�e�N�X�`���ւ̃p�X

#define TRANSITION_TEXTURE_SIZE_X					(640)					// �e�N�X�`���T�C�YX
#define TRANSITION_TEXTURE_SIZE_Y					(240)					// �e�N�X�`���T�C�YY

#define TRANSITION_TEXTURE_DIVIDE_X					(1)						// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TRANSITION_TEXTURE_DIVIDE_Y					(2)						// �A�j���p�^�[���̃e�N�X�`�����������iY)

#define TRANSITION_INITPOS_X						(SCREEN_CENTER_X)		//�g�����W�V�����̏���X���W
#define TRANSITION_TOP_INITPOS_Y					(-TRANSITION_TEXTURE_SIZE_Y)				//�㕔�g�����W�V�����̏���Y���W
#define TRANSITION_BOTTOM_INITPOS_Y					(SCREEN_HEIGHT+TRANSITION_TEXTURE_SIZE_Y)	//�����g�����W�V�����̏���Y���W


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	int						cntFrame;					//�J�E���g�t���[��
	int						state;						//�J�ڏ��
	int						nextScene;					//���ɑJ�ڂ���V�[��
}TRANSITION;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTransition(int num);							//�g�����W�V��������������
void UninitTransition(void);								//�g�����W�V�����I������
void UpdateTransition(void);								//�g�����W�V�����X�V����
void DrawTransition(void);									//�g�����W�V�����`�揈��
TRANSITION *GetTransitionAdr(void);							//�g�����W�V�����A�h���X�擾�֐�
void SetTransition(int nextScene);							//�J�ڃZ�b�g�֐�

#endif
