//=============================================================================
//
// �G�l�~�[�o���b�g�G�N�X�v���[�W�����w�b�_ [EnemyBulletExplosion.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _EBULLETEXPLOSION_H_
#define _EBULLETEXPLOSION_H_


// �}�N����`
#define EBULLETEXPLOSION_TEXTURE_NAME	_T("data/TEXTURE/effect/eneyBulletExplosion.png")	// �G�l�~�[�o���b�g�G�N�X�v���[�W�����̃e�N�X�`��

#define EBULLETEXPLOSION_TEXTURE_SIZE_X					(50)					// �e�N�X�`���T�C�YX
#define EBULLETEXPLOSION_TEXTURE_SIZE_Y					(50)					// �e�N�X�`���T�C�YY

#define EBULLETEXPLOSION_TEXTURE_DIVIDE_X				(11)						// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define EBULLETEXPLOSION_TEXTURE_DIVIDE_Y				(2)						// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define EBULLETEXPLOSION_ANIM_NUM						(11)					// �A�j���[�V�����p�^�[����
#define EBULLETEXPLOSION_TIME_ANIMATION					(2)						// �A�j���[�V�����̐؂�ւ��J�E���g

#define EBULLETEXPLOSION_MAX							(ENEMYBULLET_MAX)					//�G�l�~�[�o���b�g�G�N�X�v���[�W�����̍ő吔

#define EBULLETEXPLOSION_INITPOS_X						(-100)					//�G�l�~�[�o���b�g�G�N�X�v���[�W�����̏���X���W
#define EBULLETEXPLOSION_INITPOS_Y						(-100)					//�G�l�~�[�o���b�g�G�N�X�v���[�W�����̏���Y���W

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
	int						type;
}EBULLETEXPLOSION;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemyBulletExplosion(int num);							//�G�l�~�[�o���b�g�G�N�X�v���[�W��������������
void UninitEnemyBulletExplosion(void);								//�G�l�~�[�o���b�g�G�N�X�v���[�W�����I������
void UpdateEnemyBulletExplosion(void);								//�G�l�~�[�o���b�g�G�N�X�v���[�W�����X�V����
void DrawEnemyBulletExplosion(void);								//�G�l�~�[�o���b�g�G�N�X�v���[�W�����`�揈��
EBULLETEXPLOSION *GetEnemyBulletExplosionAdr(int pno);				//�G�l�~�[�o���b�g�G�N�X�v���[�W�����A�h���X�擾�֐�
void SetEnemyBulletExplosion(D3DXVECTOR3 pos, BULLET_TYPE type);	//�G�l�~�[�o���b�g�G�N�X�v���[�W�����Z�b�g�֐�

#endif
