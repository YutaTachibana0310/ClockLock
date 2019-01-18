//=============================================================================
//
// �v���C���[�{���o�[�w�b�_ [PlayerBomber.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _PLAYERBOMBER_H_
#define _PLAYERBOMBER_H_


// �}�N����`
#define PLAYERBOMBER_TEXTURE_NAME	_T("data/TEXTURE/player/playerBomber.png")	// �v���C���[�{���o�[�̃e�N�X�`��

#define PLAYERBOMBER_TEXTURE_SIZE_X					(300)					// �e�N�X�`���T�C�YX
#define PLAYERBOMBER_TEXTURE_SIZE_Y					(300)					// �e�N�X�`���T�C�YY

#define PLAYERBOMBER_TEXTURE_DIVIDE_X				(1)						// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define PLAYERBOMBER_TEXTURE_DIVIDE_Y				(1)						// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define PLAYERBOMBER_ANIM_NUM			(PLAYERBOMBER_TEXTURE_DIVIDE_X*PLAYERBOMBER_TEXTURE_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define PLAYERBOMBER_TIME_ANIMATION					(4)						// �A�j���[�V�����̐؂�ւ��J�E���g

#define PLAYERBOMBER_MAX							(1)					//�v���C���[�{���o�[�̍ő吔

#define PLAYERBOMBER_INITPOS_X						(-100)					//�v���C���[�{���o�[�̏���X���W
#define PLAYERBOMBER_INITPOS_Y						(-100)					//�v���C���[�{���o�[�̏���Y���W

//�v���C���[�{���o�[�̃o�E���f�B���O�{�b�N�X�̒�`
#define PLAYERBOMBER_BB_TOP							(0.0f)
#define PLAYERBOMBER_BB_LEFT						(0.0f)
#define PLAYERBOMBER_BB_WIDTH						(250.0f)
#define PLAYERBOMBER_BB_HIGHT						(250.0f)

//�v���C���[�{���o�[�̃o�E���f�B���O�T�[�N����`
#define PLAYERBOMBER_BC_RADIUS						(250)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	bool					active;							//�g�por���g�p�̔���

	D3DXVECTOR3				pos;						// �|���S���̍��W
	D3DXVECTOR3				rot;						// �|���S���̉�]��
	int						countAnim;						// �A�j���[�V�����J�E���g
	int						patternAnim;					// �A�j���[�V�����p�^�[���i���o�[

	float					radius;							// �|���S���̔��a
	float					angle;						// �|���S���̊p�x
	float					scale;					//�|���S���̃X�P�[��

}PLAYERBOMBER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayerBomber(int num);							//�v���C���[�{���o�[����������
void UninitPlayerBomber(void);								//�v���C���[�{���o�[�I������
void UpdatePlayerBomber(void);								//�v���C���[�{���o�[�X�V����
void DrawPlayerBomber(void);								//�v���C���[�{���o�[�`�揈��
PLAYERBOMBER *GetPlayerBomberAdr(int pno);						//�v���C���[�{���o�[�A�h���X�擾�֐�
bool SetPlayerBomber(D3DXVECTOR3 pos);						//�v���C���[�{���o�[�Z�b�g�֐�
#endif
