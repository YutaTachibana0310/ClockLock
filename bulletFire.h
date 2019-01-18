//=============================================================================
//
// �o���b�g�t�@�C�A�w�b�_ [bulletFire.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _BULLETFIRE_H_
#define _BULLETFIRE_H_

// �}�N����`
#define BULLETFIRE_TEXTURE_NAME		_T("data/TEXTURE/effect/bulletFireLumine.png")		// �o���b�g�t�@�C�A�̃e�N�X�`��

#define BULLETFIRE_TEXTURE_SIZE_X			(30 / 2)						// �e�N�X�`���T�C�Y�i�G�l�~�[�����p�j
#define BULLETFIRE_TEXTURE_SIZE_Y			(50 / 2)						// �e�N�X�`���T�C�Y�i�G�l�~�[�����p�j

#define BULLETFIRE_TEXTURE_DIVIDE_X			(2)								// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define BULLETFIRE_TEXTURE_DIVIDE_Y			(1)								// �A�j���p�^�[���̃e�N�X�`�����������iX)

#define BULLETFIRE_ANIM_NUM			(BULLETFIRE_TEXTURE_DIVIDE_X*BULLETFIRE_TEXTURE_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TIMEBULLETFIRE_ANIMATION			(2)								// �A�j���[�V�����̐؂�ւ��J�E���g

#define	BULLETFIRE_RADIUS_MIN				(50.0f)							// �|���S���̔��a�ŏ��l
#define	BULLETFIRE_RADIUS_MAX				(300.0f)						// �|���S���̔��a�ő�l
#define	BULLETFIRE_VALUE_ROTATION			(D3DX_PI * 0.01f)				// �|���S���̉�]��
#define	BULLETFIRE_VALUE_SCALE				(2.0f)							// �|���S���̃X�P�[���ω���                                                                                          

#define BULLETFIRE_MAX						(4*PLAYER_MAX)					//�o���b�g�t�@�C�A�̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	bool					active;							//�g�por���g�p�̔���

	D3DXVECTOR3				pos;							// �|���S���̍��W
	D3DXVECTOR3				rot;							// �|���S���̉�]��

	int						countAnim;						// �A�j���[�V�����J�E���g
	int						patternAnim;					// �A�j���[�V�����p�^�[���i���o�[

	float					radius;							//�|���S�����a
	float					angle;							//�|���S���p�x
}BULLETFIRE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBulletFire(int num);							//�o���b�g�t�@�C�A����������
void UninitBulletFire(void);								//�o���b�g�t�@�C�A�I������
void UpdateBulletFire(void);								//�o���b�g�t�@�C�A�X�V����
void DrawBulletFire(void);									//�o���b�g�t�@�C�A�`�揈��
BULLETFIRE *GetBulletFireAdr(int pno);						//�o���b�g�t�@�C�A�̃A�h���X�擾�֐�
void SetBulletFire(BULLETFIRE * ptr);						//�o���b�g�t�@�C�A�̃Z�b�g�֐�

#endif
