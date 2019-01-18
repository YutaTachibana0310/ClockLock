//=============================================================================
//
// �v���C���[�o���b�g�w�b�_ [PlayerBullet.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _PLAYERBULLET_H_
#define _PLAYERBULLET_H_


// �}�N����`
#define PLAYERBULLET_TEXTURE_MAIN	_T("data/TEXTURE/player/playerbullet.png")	// �v���C���[�o���b�g�̃e�N�X�`��
#define PLAYERBULLET_TEXTURE_SUB	_T("data/TEXTURE/player/playerbullet.png")	// �v���C���[�o���b�g�̃e�N�X�`��

#define PLAYERBULLET_TEXTURE_SIZE_X					(70 / 2)					// �e�N�X�`���T�C�YX
#define PLAYERBULLET_TEXTURE_SIZE_Y					(80 / 2)					// �e�N�X�`���T�C�YY

#define PLAYERBULLET_CENTER_X						(35)					//�v���C���[�o���b�g�̒��S�ʒuX
#define PLAYERBULLET_CENTER_Y						(40)					//�v���C���[�o���b�g�̒��S�ʒuY

#define PLAYERBULLET_TEXTURE_DIVIDE_X				(1)						// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define PLAYERBULLET_TEXTURE_DIVIDE_Y				(1)						// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PLAYERBULLET_NUM			(PLAYERBULLET_TEXTURE_DIVIDE_X*PLAYERBULLET_TEXTURE_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TIMEPLAYERBULLET_ANIMATION					(4)						// �A�j���[�V�����̐؂�ւ��J�E���g

#define PLAYERBULLET_MAIN_MAX						(56)					//�v���C���[�o���b�g�̍ő吔�i���C���V���b�g�j
#define PLAYERBULLET_SUB_MAX						(128)					//�v���C���[�o���b�g�̍ő吔�i�T�u�V���b�g�j

#define PLAYERBULLET_INITPOS_X						(-100)					//�v���C���[�o���b�g�̏���X���W
#define PLAYERBULLET_INITPOS_Y						(-100)					//�v���C���[�o���b�g�̏���Y���W

//�v���C���[�o���b�g�̃o�E���f�B���O�{�b�N�X�̒�`
#define PLAYERBULLET_BB_TOP							(0.0f)
#define PLAYERBULLET_BB_LEFT						(0.0f)
#define PLAYERBULLET_BB_WIDTH						(10.0f)
#define PLAYERBULLET_BB_HIGHT						(35.0f)

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

	D3DXVECTOR3				moveDir;						//�i�s����

}PLAYERBULLET;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayerBullet(int num);								//�v���C���[�o���b�g����������
void UninitPlayerBullet(void);									//�v���C���[�o���b�g�I������
void UpdatePlayerBullet(void);									//�v���C���[�o���b�g�X�V����
void DrawPlayerBullet(void);									//�v���C���[�o���b�g�`�揈��
PLAYERBULLET *GetPlayerBulletAdr(int pno);						//�v���C���[�o���b�g�A�h���X�擾�֐��i���C���V���b�g�j
PLAYERBULLET *GetPlayerSubBulletAdr(int pno);					//�v���C���[�o���b�g�A�h���X�擾�֐��i�T�u�V���b�g�j
void FirePlayerBullet(D3DXVECTOR3 pos);							//�v���C���[�o���b�g�Z�b�g�֐��i���C���V���b�g�j
void FirePlayerSubBullet(D3DXVECTOR3 pos, float rotAngle);		//�v���C���[�o���b�g�Z�b�g�֐��i�T�u�V���b�g�j


#endif
