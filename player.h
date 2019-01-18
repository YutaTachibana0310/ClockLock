//=============================================================================
//
// �v���C���[�w�b�_�[ [Player.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "bulletFire.h"

// �}�N����`
#define PLAYER_TEXTURE	_T("data/TEXTURE/player/player.png")				// �v���C���[�̃e�N�X�`��
#define PLAYER_CORETEX_NAME	_T("data/TEXTURE/player/playercore.png")		// �v���C���[�R�A�̃e�N�X�`��
#define PLAYER_TEXTURE_SIZE_X		(90)									// �e�N�X�`���T�C�Y�iX)
#define PLAYER_TEXTURE_SIZE_Y		(120)									// �e�N�X�`���T�C�Y�iY)

#define TEXTURE_PATTERN_DIVIDE_X	(4)										// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(3)										// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION				(4)										// �A�j���[�V�����̐؂�ւ��J�E���g

#define PLAYER_MAX					(1)										//�v���C���[�̍ő吔

#define PLAYER_INITPOS_X			(SCREEN_CENTER_X)						//�v���C���[�̏������W
#define PLAYER_INITPOS_Y			(SCREEN_HEIGHT * 1.1f)					//�v���C���[�̏������W

#define PLAYER_NUM_BULLETFIRE		(4)										//�o���b�g���˂̃G�t�F�N�g�̐�

#define PLAYER_BB_LEFT				(0.0f)									//�v���C���[�̓����蔻��i������̃I�t�Z�b�g�l�j
#define PLAYER_BB_WIDTH				(5.0f)									//�v���C���[�̓����蔻��i���j
#define PLAYER_BB_TOP				(-10.0f)								//�v���C���[�̓����蔻��i�ォ��̃I�t�Z�b�g�l�j
#define PLAYER_BB_HIGHT				(5.0f)									//�v���C���[�̓����蔻��i�����j

#define PLAYER_ITEMBB_WIDTH			(20.0f)									//�v���C���[�̃A�C�e���p�����蔻��i���j
#define PLAYER_ITEMBB_HIGHT			(20.0f)									//�v���C���[�̃A�C�e���p�����蔻��i�����j

#define PLAYER_TIMEEFFECT_MAX		(720.0f)								//���ԑ����MAX

#define PLAYER_CORE_SIZE_X			(10)									//�v���C���[�R�A�̃e�N�X�`���T�C�Y
#define PLAYER_CORE_SIZE_Y			(10)									//�v���C���[�R�A�̃e�N�X�`���T�C�Y
#define PLAYER_CORE_OFFSET_Y		(-10.0f)								//�v���C���[�R�A�̃I�t�Z�b�g���W
#define PLAYER_CORE_DIVIDE_X		(6)										//�v���C���[�R�A�̃e�N�X�`��������X
#define PLAYER_CORE_DIVIDE_Y		(1)										//�v���C���[�R�A�̃e�N�X�`��������Y
#define PLAYER_CORE_PATTERNMAX		(6)										//�v���C���[�R�A�̃A�j���[�V����MAX

#define PLAYER_TRAILPOS_NUM			(50)									//�ۑ�����g���C���̐�
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	bool			active;								//�A�N�e�B�u���
	bool			flgInput;							//���͂̎�t���

	int				shotInterbal;						//�Ō�ɃV���b�g�������Ă���̌o�߃t���[��
	int				numFireShot;						//�T�u�V���b�g�̔��ˉ�
	int				countAnim;							// �A�j���[�V�����J�E���g
	int				patternAnim;						// �A�j���[�V�����p�^�[���i���o�[
	int				playerDir;							//�v���C���[��X���i�s�����i�e�N�X�`���ݒ�p�j
	float			timeEffect;							//���ԑ���G�t�F�N�g�̎c��t���[��
	int				zanki;								//�c�@

	float			angle;								//���_�ւ̊p�x
	float			radius;								//�|���S���̔��a

	float			angleCore;							//�R�A�̃|���S���p�x
	float			radiusCore;							//�R�A�̃|���S�����a

	D3DXVECTOR3		pos;								//���W
	D3DXVECTOR3		rot;								//��]��
	BULLETFIRE		*fire[PLAYER_NUM_BULLETFIRE];		//�o���b�g���˂̃G�t�F�N�g
	VERTEX_2D		vertexWk[NUM_VERTEX];				//���_���i�[���[�N
	VERTEX_2D		shadowWk[NUM_VERTEX];				//�e�p�̒��_���

	int				state;								//�v���C���[�̏��
	int				bomCnt;								//�{���c�e

	float			gaugeDeltaValue;					//1�t���[���̃Q�[�W������

	bool			lockTimeEffect;						//���ԑ���@�\�̃��b�N
	bool			lockBomber;							//�{���o�[�̃��b�N

	int				corePattern;						//�R�A�̃A�j���[�V�����p�^�[��
	int				cntFrame;							//�J�E���g�t���[��

	D3DXVECTOR3		trailPos[PLAYER_TRAILPOS_NUM];		//�g���C��

}PLAYER;

//�v���C���[�̏�ԗ񋓌^
enum PLAYER_STATE
{
	PLAYER_INITIALIZED,
	PLAYER_INVINCIBLE,
	PLAYER_DEFAULT,
	PLAYER_DESTROYED,
	PLAYER_GAMEOVER
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(int num);						//�v���C���[����������
void UninitPlayer(void);							//�v���C���[�I������
void UpdatePlayer(void);							//�v���C���[�X�V����
void DrawPlayer(void);								//�v���C���[�`�揈��
PLAYER *GetPlayerAdr(int pno);						//�v���C���[�A�h���X�擾�֐�
void DestroyPlayer(int i);							//�v���C���[���ď���
void RespornPlayer(int i);							//�v���C���[�ďo������

#endif
