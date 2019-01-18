//=============================================================================
//
// �p�[�e�B�N���w�b�_�[ [particle.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "effect.h"

// �}�N����`
#define PARTICLE_MAX					(56)				//�p�[�e�B�N���̍ő吔

/*****************************************************************************
//�\���̒�`
*****************************************************************************/
enum PARTICLETYPE
{
	PIECE,
	FIRE,
	EXPLOSION,
	BOSSFIRE,
	KIND_MAX
};

typedef struct 
{	
	bool				active;				//�Q�[�����ŃA�N�e�B�u���ǂ���

	D3DXVECTOR3			pos;				//���W

	int					cntFrame;			//�Z�b�g����Ă���̌o�߃t���[��

	EFFECT				*particle;			//�p�[�e�B�N�����\������G�t�F�N�g�ւ̃|�C���^
}PARTICLE;

/*****************************************************************************
// �v���g�^�C�v�錾
*****************************************************************************/
HRESULT InitParticle(int num);					//�p�[�e�B�N������������
void UninitParticle(void);						//�p�[�e�B�N���I������
void UpdateParticle(void);						//�p�[�e�B�N���X�V����
void DrawParticle(void);						//�p�[�e�B�N���`�揈��
PARTICLE *GetParticleAdr(int type, int pno);	//�p�[�e�B�N���A�h���X�擾�֐�
void SetParticle(D3DXVECTOR3 pos, int type);	//�����p�[�e�B�N���̃Z�b�g�֐�

#endif
