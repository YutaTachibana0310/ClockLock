//=============================================================================
//
// �G�l�~�[�Q�[�W�w�b�_ [enemyGauge.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _ENEMYGAUGE_H_
#define _ENEMYGAUGE_H_


// �}�N����`
#define ENEMYGAUGE_TEXTURE_NAME	_T("data/TEXTURE/GUI/enemyGauge.png")			// �G�l�~�[�Q�[�W�̃e�N�X�`��
#define ENEMYGAUGEFRAME_TEXTURE_NAME	_T("data/TEXTURE/GUI/enemyGaugeFrame.png")	// �G�l�~�[�Q�[�W�t���[���̃e�N�X�`��
#define ENEMYGAUGEBG_TEXTURE_NAME	_T("data/TEXTURE/GUI/enemyGaugeBG.png")		// �G�l�~�[�Q�[�W�w�i�̃e�N�X�`��

#define ENEMYGAUGE_TEXTURE_SIZE_X					(100)					// �e�N�X�`���T�C�YX
#define ENEMYGAUGE_TEXTURE_SIZE_Y					(15)					// �e�N�X�`���T�C�YY

#define ENEMYGAUGE_TEXTURE_DIVIDE_X					(1)							// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define ENEMYGAUGE_TEXTURE_DIVIDE_Y					(1)							// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ENEMYGAUGE_ANIM_NUM			(ENEMYGAUGE_TEXTURE_DIVIDE_X*ENEMYGAUGE_TEXTURE_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ENEMYGAUGE_TIME_ANIMATION					(4)							// �A�j���[�V�����̐؂�ւ��J�E���g

#define ENEMYGAUGE_MAX								(56)						//�G�l�~�[�Q�[�W�̍ő吔

#define ENEMYGAUGE_INITPOS_X						(-100)						//�G�l�~�[�Q�[�W�̏���X���W
#define ENEMYGAUGE_INITPOS_Y						(-100)						//�G�l�~�[�Q�[�W�̏���Y���W

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	bool					active;							//�g�por���g�p�̔���

	D3DXVECTOR3				pos;							// �|���S���̍��W
	D3DXVECTOR3				rot;							// �|���S���̉�]��
	int						countAnim;						// �A�j���[�V�����J�E���g
	int						patternAnim;					// �A�j���[�V�����p�^�[���i���o�[
	float					scale;							//�Q�[�W�̃X�P�[��

}ENEMYGAUGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemyGauge(int num);
void UninitEnemyGauge(void);
void UpdateEnemyGauge(void);
void DrawEnemyGauge(void);
ENEMYGAUGE *GetEnemyGaugeAdr(int pno);
ENEMYGAUGE *GetNonActiveEnemyGauge(void);

#endif
