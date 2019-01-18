//=============================================================================
//
// �v���C���[�V�[���h�w�b�_ [PlayerShield.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _PLAYERSHIELD_H_
#define _PLAYERSHIELD_H_


// �}�N����`
#define PLAYERSHIELD_TEXTURE_NAME	_T("data/TEXTURE/player/playerShield.png")	// �v���C���[�V�[���h�̃e�N�X�`��

#define PLAYERSHIELD_TEXTURE_SIZE_X					(80)					// �e�N�X�`���T�C�YX
#define PLAYERSHIELD_TEXTURE_SIZE_Y					(80)					// �e�N�X�`���T�C�YY

#define PLAYERSHIELD_INITPOS_X						(-100)						//�v���C���[�V�[���h�̏���X���W
#define PLAYERSHIELD_INITPOS_Y						(-100)						//�v���C���[�V�[���h�̏���Y���W

//�v���C���[�V�[���h�̃o�E���f�B���O�T�[�N���̒�`
#define PLAYERSHIELD_BC_RADIUS						(40.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	bool					active;							//�g�por���g�p�̔���

	D3DXVECTOR3				pos;							// �|���S���̍��W
	D3DXVECTOR3				rot;							// �|���S���̉�]��
	int						countAnim;						// �A�j���[�V�����J�E���g
	int						patternAnim;					// �A�j���[�V�����p�^�[���i���o�[

	float					radius;							// �|���S���̔��a
	float					angle;							// �|���S���̊p�x
	float					scale;							// �|���S���̃X�P�[��
	int						cntFrame;
	int						state;

}PLAYERSHIELD;

enum PLAYERSHIELD_COLOR
{
	SHIELD_ORANGE,
	SHIELD_GREEN,
	SHIELD_TYPEMAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayerShield(int num);							//�v���C���[�V�[���h����������
void UninitPlayerShield(void);								//�v���C���[�V�[���h�I������
void UpdatePlayerShield(void);								//�v���C���[�V�[���h�X�V����
void DrawPlayerShield(void);								//�v���C���[�V�[���h�`�揈��
PLAYERSHIELD *GetPlayerShieldAdr(void);						//�v���C���[�V�[���h�A�h���X�擾�֐�
void SetPlayerShield(void);									//�v���C���[�V�[���h��tp�@m�V�C
#endif
