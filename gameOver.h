//=============================================================================
//
// �Q�[���I�[�o�[�w�b�_ [GameOver.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _GAMEOVER_H_
#define _GAMEOVER_H_


// �}�N����`
#define GAMEOVER_TEXTURE_NAME	_T("data/TEXTURE/bg/gameover.png")	// �Q�[���I�[�o�[�̃e�N�X�`��

#define GAMEOVER_TEXTURE_SIZE_X					(390)					// �e�N�X�`���T�C�YX
#define GAMEOVER_TEXTURE_SIZE_Y					(480)					// �e�N�X�`���T�C�YY

#define GAMEOVER_INITPOS_X						(SCREEN_CENTER_X)					//�Q�[���I�[�o�[�̏���X���W
#define GAMEOVER_INITPOS_Y						(SCREEN_CENTER_Y)					//�Q�[���I�[�o�[�̏���Y���W

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	bool					active;						//�g�p���

	D3DXVECTOR3				pos;						// �|���S���̍��W

	float					radius;						// �|���S���̔��a
	float					angle;						// �|���S���̊p�x
	int						cntFrame;					//�J�E���g�t���[��
	int						state;						//���
}GAMEOVER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGameOver(int num);							//�Q�[���I�[�o�[����������
void UninitGameOver(void);								//�Q�[���I�[�o�[�I������
void UpdateGameOver(void);								//�Q�[���I�[�o�[�X�V����
void DrawGameOver(void);								//�Q�[���I�[�o�[�`�揈��
GAMEOVER *GetGameOverAdr(void);							//�Q�[���I�[�o�[�A�h���X�擾�֐�
void SetGameOver(void);									//�Q�[���I�[�o�[�Z�b�g�֐�

#endif
