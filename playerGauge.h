//=============================================================================
//
// �v���C���[�Q�[�W�w�b�_ [PlayerGauge.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _PLAYERGAUGE_H_
#define _PLAYERGAUGE_H_


// �}�N����`
#define PLAYERGAUGE_TEXTURE_NAME	_T("data/TEXTURE/player/circuit3.png")	// �v���C���[�Q�[�W�̃e�N�X�`��
#define PLAYERGAUGE_BACKTEXTURE_NAME _T("data/TEXTURE/player/circuit2.png")	// �v���C���[�Q�[�W�i�o�b�N�j�̃e�N�X�`��

#define PLAYERGAUGE_TEXTURE_SIZE_X					(150 / 2)					// �e�N�X�`���T�C�YX
#define PLAYERGAUGE_TEXTURE_SIZE_Y					(150 / 2)					// �e�N�X�`���T�C�YY

#define PLAYERGAUGE_MAX								(1)							//�v���C���[�Q�[�W�̍ő吔

#define PLAYERGAUGE_INITPOS_X						(-100)						//�v���C���[�Q�[�W�̏���X���W
#define PLAYERGAUGE_INITPOS_Y						(-100)						//�v���C���[�Q�[�W�̏���Y���W

#define PLAYERGAUGE_VERTEX_NUM						(144 + 2)					//�v���C���[�Q�[�W�̒��_��
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
enum GAUGESTATE
{
	GAUGESTATE_OPEN,
	GAUGESTATE_NORMAL,
	GAUGESTATE_CLOSE
};

typedef struct
{
	bool					active;						//�g�por���g�p�̔���

	D3DXVECTOR3				pos;						// �|���S���̍��W
	D3DXVECTOR3				rot;						// �|���S���̉�]��

	float					radius;						// �|���S���̔��a
	float					angle;						// �|���S���̊p�x

	float					percent;					//�Q�[�W�̉~���̒��������߂郁���o
	float					scale;						//�Q�[�W�̑傫��

	GAUGESTATE				state;						//�Q�[�W�̏��
}PLAYERGAUGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayerGauge(int num);						//�v���C���[�Q�[�W����������
void UninitPlayerGauge(void);							//�v���C���[�Q�[�W�I������
void UpdatePlayerGauge(void);							//�v���C���[�Q�[�W�X�V����
void DrawPlayerGauge(void);								//�v���C���[�Q�[�W�`�揈��
PLAYERGAUGE *GetPlayerGaugeAdr(int pno);				//�v���C���[�Q�[�W�̃A�h���X�擾�֐�
void SetGaugeState(GAUGESTATE state, int no);			//�v���C���[�Q�[�W�̏�ԃZ�b�g�֐�

#endif