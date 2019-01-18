//=============================================================================
//
// �G�l�~�[�p�[�e�B�N���w�b�_ [EnemyParticle.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _ENEMYPARTICLE_H_
#define _ENEMYPARTICLE_H_


// �}�N����`
#define ENEMYPARTICLE_TEXTURE_NAME	_T("data/TEXTURE/effect/enemyParticle.png")	// �G�l�~�[�p�[�e�B�N���̃e�N�X�`��

#define ENEMYPARTICLE_TEXTURE_SIZE_X					(30)					// �e�N�X�`���T�C�YX
#define ENEMYPARTICLE_TEXTURE_SIZE_Y					(30)					// �e�N�X�`���T�C�YY

#define ENEMYPARTICLE_MAX							(1024)					//�G�l�~�[�p�[�e�B�N���̍ő吔
#define ENEMYPARTICLE_EMITTER_MAX					(128)

#define ENEMYPARTICLE_INITPOS_X						(-100)					//�G�l�~�[�p�[�e�B�N���̏���X���W
#define ENEMYPARTICLE_INITPOS_Y						(-100)					//�G�l�~�[�p�[�e�B�N���̏���Y���W

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
}ENEMYPARTICLE;

typedef struct {
	bool					active;

	D3DXVECTOR3				pos;

	int						frameCnt;
	int						duration;
	int						shape;
	int					emitNum;
}EMITTER_ENEMYPARTICLE;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemyParticle(int num);							//�G�l�~�[�p�[�e�B�N������������
void UninitEnemyParticle(void);								//�G�l�~�[�p�[�e�B�N���I������
void UpdateEnemyParticle(void);								//�G�l�~�[�p�[�e�B�N���X�V����
void DrawEnemyParticle(void);								//�G�l�~�[�p�[�e�B�N���`�揈��
ENEMYPARTICLE *GetEnemyParticleAdr(int pno);						//�G�l�~�[�p�[�e�B�N���A�h���X�擾�֐�
void SetEnemyParticle(D3DXVECTOR3 pos, int shape);
void SetEmitterEnemyParticle(D3DXVECTOR3 pos, int duration, int emitNum, int shape);
#endif
