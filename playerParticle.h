//=============================================================================
//
// �G�l�~�[�p�[�e�B�N���w�b�_ [PlayerParticle.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _PLAYERPARTICLE_H_
#define _PLAYERPARTICLE_H_


// �}�N����`
#define PLAYERPARTICLE_TEXTURE_NAME	_T("data/TEXTURE/effect/playerParticle.png")	// �G�l�~�[�p�[�e�B�N���̃e�N�X�`��

#define PLAYERPARTICLE_TEXTURE_SIZE_X					(8)					// �e�N�X�`���T�C�YX
#define PLAYERPARTICLE_TEXTURE_SIZE_Y					(8)					// �e�N�X�`���T�C�YY

#define PLAYERPARTICLE_MAX							(1024)					//�G�l�~�[�p�[�e�B�N���̍ő吔
#define PLAYERPARTICLE_EMITTER_MAX					(128)

#define PLAYERPARTICLE_INITPOS_X						(-100)					//�G�l�~�[�p�[�e�B�N���̏���X���W
#define PLAYERPARTICLE_INITPOS_Y						(-100)					//�G�l�~�[�p�[�e�B�N���̏���Y���W

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	bool					active;						//�g�por���g�p�̔���

	D3DXVECTOR3				pos;						// �|���S���̍��W

	float					radius;						// �|���S���̔��a
	float					angle;						// �|���S���̊p�x

	int						frameCnt;					//�t���[���J�E���g
	float					speed, initSpeed;
	D3DXVECTOR3				dir;
	float					scale, initScale;
	int						lifeFrame;
}PLAYERPARTICLE;

typedef struct {
	bool					active;

	D3DXVECTOR3				pos;

	int						frameCnt;
	int						duration;
	int						shape;
	int						emitNum;
	float					maxScale;
}EMITTER_PLAYERPARTICLE;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayerParticle(int num);							//�G�l�~�[�p�[�e�B�N������������
void UninitPlayerParticle(void);								//�G�l�~�[�p�[�e�B�N���I������
void UpdatePlayerParticle(void);								//�G�l�~�[�p�[�e�B�N���X�V����
void DrawPlayerParticle(void);								//�G�l�~�[�p�[�e�B�N���`�揈��
PLAYERPARTICLE *GetPlayerParticleAdr(int pno);						//�G�l�~�[�p�[�e�B�N���A�h���X�擾�֐�
void SetPlayerParticle(D3DXVECTOR3 pos, int shape, float maxScale);
void SetEmitterPlayerParticle(D3DXVECTOR3 pos, int duration, int emitNum, int shape, float maxScale);

#endif
