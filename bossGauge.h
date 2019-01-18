//=============================================================================
//
// �{�X�Q�[�W�w�b�_ [BossGauge.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _BOSSGAUGE_H_
#define _BOSSGAUGE_H_


// �}�N����`
#define BOSSGAUGE_TEXTURE_NAME	_T("data/TEXTURE/GUI/bossGauge.png")	// �{�X�Q�[�W�̃e�N�X�`��

#define BOSSGAUGE_TEXTURE_SIZE_X					(390)					// �e�N�X�`���T�C�YX
#define BOSSGAUGE_TEXTURE_SIZE_Y					(30)					// �e�N�X�`���T�C�YY

#define BOSSGAUGE_TEXTURE_DIVIDE_X					(1)						// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define BOSSGAUGE_TEXTURE_DIVIDE_Y					(2)						// �A�j���p�^�[���̃e�N�X�`�����������iY)

#define BOSSGAUGE_INITPOS_X							(SCREEN_CENTER_X)		//�{�X�Q�[�W�̏���X���W
#define BOSSGAUGE_INITPOS_Y							(25)					//�{�X�Q�[�W�̏���Y���W

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	bool					active;						//�g�por���g�p�̔���

	D3DXVECTOR3				pos;						// �|���S���̍��W
	D3DXVECTOR3				rot;						// �|���S���̉�]��

	float					radius;						// �|���S���̔��a
	float					angle;						// �|���S���̊p�x

	int						state;						//���
	float					scale;						//�X�P�[��
	float					percent;					//�c�芄��

}BOSSGAUGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBossGauge(int num);							//�{�X�Q�[�W����������
void UninitBossGauge(void);								//�{�X�Q�[�W�I������
void UpdateBossGauge(void);								//�{�X�Q�[�W�X�V����
void DrawBossGauge(void);								//�{�X�Q�[�W�`�揈��
BOSSGAUGE *GetBossGaugeAdr(void);						//�{�X�Q�[�W�A�h���X�擾�֐�
BOSSGAUGE *SetBossGauge(void);							//�{�X�Q�[�W�Z�b�g�֐�

#endif
