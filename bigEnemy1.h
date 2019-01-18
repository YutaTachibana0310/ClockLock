//=============================================================================
//
// �r�b�O�G�l�~�[1�w�b�_ [BigEnemy1.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _BIGENEMY1_H_
#define _BIGENEMY1_H_


// �}�N����`
#define BIGENEMY1_TEXTURE_NAME	_T("data/TEXTURE/Enemy/bigWing1.png")	// �r�b�O�G�l�~�[1�̃e�N�X�`��
#define BIGENEMY1_SHADOWTEX_NAME	_T("data/TEXTURE/Shadow/bigWing1.png")	// �r�b�O�G�l�~�[1�̃e�N�X�`��(�e�p)

#define BIGENEMY1_TEXTURE_SIZE_X					(560 / 2)					// �e�N�X�`���T�C�YX
#define BIGENEMY1_TEXTURE_SIZE_Y					(210 / 2)					// �e�N�X�`���T�C�YY

#define BIGENEMY1_TEXTURE_NUM						(2)							//�e�N�X�`���̐�

#define BIGENEMY1_TEXTURE_DIVIDE_X				(1)								// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define BIGENEMY1_TEXTURE_DIVIDE_Y				(2)								// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_BIGENEMY1_NUM			(BIGENEMY1_TEXTURE_DIVIDE_X*BIGENEMY1_TEXTURE_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TIMEBIGENEMY1_ANIMATION					(2)								// �A�j���[�V�����̐؂�ւ��J�E���g

#define BIGENEMY1_MAX							(5)								//�r�b�O�G�l�~�[1�̍ő吔

#define BIGENEMY1_INITPOS_X						(-100)							//�r�b�O�G�l�~�[1�̏���X���W
#define BIGENEMY1_INITPOS_Y						(-100)							//�r�b�O�G�l�~�[1�̏���Y���W

//�r�b�O�G�l�~�[1�̃o�E���f�B���O�{�b�N�X�̒�`
#define BIGENEMY1_BB_TOP						(0.0f)
#define BIGENEMY1_BB_LEFT						(0.0f)
#define BIGENEMY1_BB_WIDTH						(250.0f)
#define BIGENEMY1_BB_HIGHT						(50.0f)

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

	D3DXVECTOR3				targetPos;						//�ړ��̖ڕW���W

	int						state;							//�s�����
	int						cntFrame;						//�J�E���g�t���[��

	int						hp;								//HP

	ENEMYGAUGE				*gauge;							//HP�Q�[�W�ւ̃|�C���^

}BIGENEMY1;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBigEnemy1(int num);										//�r�b�O�G�l�~�[1����������
void UninitBigEnemy1(void);											//�r�b�O�G�l�~�[1�I������
void UpdateBigEnemy1(void);											//�r�b�O�G�l�~�[1�X�V����
void DrawBigEnemy1(void);											//�r�b�O�G�l�~�[1�`�揈��
BIGENEMY1 *GetBigEnemy1Adr(int pno);								//�r�b�O�G�l�~�[1�A�h���X�擾�֐�
void SetBigEnemy(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos);		//�r�b�O�G�l�~�[���Q�[�����ɏo��������֐�

#endif
