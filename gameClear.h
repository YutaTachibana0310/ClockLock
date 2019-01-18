//=============================================================================
//
// �Q�[���N���A�w�b�_ [GameClear.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _GAMECLEAR_H_
#define _GAMECLEAR_H_


// �}�N����`
#define GAMECLEAR_TEXTURE_NAME	_T("data/TEXTURE/bg/gameClear.png")	// �Q�[���N���A�̃e�N�X�`��
#define GAMECLEAR_CHARTEX_NAME	_T("data/TEXTURE/bg/clearText.png")	//�e�L�X�g�̃e�N�X�`��

#define GAMECLEAR_TEXTURE_SIZE_X					(390)					// �e�N�X�`���T�C�YX
#define GAMECLEAR_TEXTURE_SIZE_Y					(480)					// �e�N�X�`���T�C�YY

#define GAMECLEAR_CHARTEX_SIZE_X					(380)					//�e�L�X�g�e�N�X�`���̃T�C�YX
#define GAMECLEAR_CHARTEX_SIZE_Y					(60)					//�e�L�X�g�e�N�X�`���̃T�C�YY
#define GAMECLEAR_CHATEX_DIVIDE_Y					(2)						//�e�N�X�`��������

#define GAMECLEAR_INITPOS_X							(SCREEN_CENTER_X)		//�Q�[���N���A�̏���X���W
#define GAMECLEAR_INITPOS_Y							(SCREEN_CENTER_Y)		//�Q�[���N���A�̏���Y���W

#define GAMECLEAR_STAGETEX_INITPOS_X				(1280.0f)
#define GAMECLEAR_STAGETEX_INITPOS_Y				(100.0f)

#define GAMECLEAR_CLEARTEX_INITPOS_X				(0.0f)
#define GAMECLEAR_CLEARTEX_INITPOS_Y				(220.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	bool					active;						//�g�p���

	D3DXVECTOR3				pos;						// �|���S���̍��W

	float					radius;						// �|���S���̔��a
	float					angle;						// �|���S���̊p�x

	float					radiusChar;
	float					angleChar;

	int						cntFrame;					//�J�E���g�t���[��
	int						state;						//���
}GAMECLEAR;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGameClear(int num);							//�Q�[���N���A����������
void UninitGameClear(void);								//�Q�[���N���A�I������
void UpdateGameClear(void);								//�Q�[���N���A�X�V����
void DrawGameClear(void);								//�Q�[���N���A�`�揈��
GAMECLEAR *GetGameClearAdr(void);						//�Q�[���N���A�A�h���X�擾�֐�
void SetGameClear(void);								//�Q�[���N���A�Z�b�g�֐�

#endif
