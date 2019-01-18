//=============================================================================
//
// �v���C���[�̔����w�b�_ [PlayerExplosion.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _PLAYEREXPLOSION_H_
#define _PLAYEREXPLOSION_H_


// �}�N����`
#define PLAYEREXPLOSION_TEXTURE_NAME	_T("data/TEXTURE/effect/playerExplosion.png")	// �v���C���[�̔����̃e�N�X�`��

#define PLAYEREXPLOSION_TEXTURE_SIZE_X					(80)					// �e�N�X�`���T�C�YX
#define PLAYEREXPLOSION_TEXTURE_SIZE_Y					(80)					// �e�N�X�`���T�C�YY

#define PLAYEREXPLOSION_TEXTURE_DIVIDE_X				(2)						// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define PLAYEREXPLOSION_TEXTURE_DIVIDE_Y				(2)						// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define PLAYEREXPLOSION_ANIM_NUM			(PLAYEREXPLOSION_TEXTURE_DIVIDE_X*PLAYEREXPLOSION_TEXTURE_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define PLAYEREXPLOSION_TIME_ANIMATION					(2)						// �A�j���[�V�����̐؂�ւ��J�E���g

#define PLAYEREXPLOSION_MAX								(4)					//�v���C���[�̔����̍ő吔

#define PLAYEREXPLOSION_INITPOS_X						(-100)					//�v���C���[�̔����̏���X���W
#define PLAYEREXPLOSION_INITPOS_Y						(-100)					//�v���C���[�̔����̏���Y���W

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

}PLAYEREXPLOSION;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayerExplosion(int num);							//�v���C���[�̔�������������
void UninitPlayerExplosion(void);								//�v���C���[�̔����I������
void UpdatePlayerExplosion(void);								//�v���C���[�̔����X�V����
void DrawPlayerExplosion(void);									//�v���C���[�̔����`�揈��
PLAYEREXPLOSION *GetPlayerExplosionAdr(int pno);				//�v���C���[�̔����A�h���X�擾�֐�
void SetPlayerExplosion(D3DXVECTOR3 pos);						//�v���C���[�����̃Z�b�g�֐�

#endif
