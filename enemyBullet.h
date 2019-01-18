//=============================================================================
//
// �G�l�~�[�o���b�g�w�b�_ [enemyBullet.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _ENEMYBULLET_H_
#define _ENEMYBULLET_H_


// �}�N����`
#define ENEMYBULLET_TEXTURE_MAIN	_T("data/TEXTURE/EnemyBullet/enemybullet.png")			// �G�l�~�[�o���b�g�̃e�N�X�`��
#define ENEMYBULLET_TEXTURE_LUMINE	_T("data/TEXTURE/EnemyBullet/enemybulletLumine3.png")	// �G�l�~�[�o���b�g�̃e�N�X�`��(��)

#define ENEMYBULLET_TEXTURE_SIZE_X					(60)					// �e�N�X�`���T�C�YX
#define ENEMYBULLET_TEXTURE_SIZE_Y					(60)					// �e�N�X�`���T�C�YY

#define ENEMYBULLET_TEXTURE_DIVIDE_X				(4)						// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define ENEMYBULLET_TEXTURE_DIVIDE_Y				(6)						// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_ENEMYBULLET_NUM						(4)						// �A�j���[�V�����p�^�[����
#define TIMEENEMYBULLET_ANIMATION					(4)						// �A�j���[�V�����̐؂�ւ��J�E���g


#define	ENEMYBULLET_RADIUS_MIN						(50.0f)					// �|���S���̔��a�ŏ��l
#define	ENEMYBULLET_RADIUS_MAX						(300.0f)				// �|���S���̔��a�ő�l
#define	ENEMYBULLET_VALUE_ROTATION					(D3DX_PI * 0.01f)		// �|���S���̉�]��
#define	ENEMYBULLET_VALUE_SCALE						(2.0f)					// �|���S���̃X�P�[���ω���                                                                                          

#define ENEMYBULLET_NUM_MAX							(1024)					//�G�l�~�[�o���b�g(1���)�̍ő吔
#define ENEMYBULLET_KIND_MAX						(6)						//�G�l�~�[�o���b�g�̎�ނ̍ő吔
#define ENEMYBULLET_MAX		(ENEMYBULLET_NUM_MAX*ENEMYBULLET_KIND_MAX)		//�G�l�~�[�o���b�g�S�̂̍ő吔

#define ENEMYBULLET_INITPOS_X						(-100)					//�G�l�~�[�o���b�g�̏���X���W
#define ENEMYBULLET_INITPOS_Y						(-100)					//�G�l�~�[�o���b�g�̏���Y���W

#define ENEMYBULLET_NUM_TEXTURE						(2)

//�G�l�~�[�o���b�g�̃o�E���f�B���O�{�b�N�X�̒�`
#define ENEMYBULLET_BB_TOP							(0.0f)
#define ENEMYBULLET_BB_LEFT							(0.0f)
#define ENEMYBULLET_BB_WIDTH						(8.0f)
#define ENEMYBULLET_BB_HIGHT						(8.0f)
#define ENEMYBULLET_BC_RADIUS						(8.0f)

#define ENEMYBULLET_DAFAULT_ANGLE					(1.5707961f)			//Y���ɑ΂��Đ����ɔ��˂��������̊p�x
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
//�o���b�g�^�C�v��\���񋓑�
enum BULLET_TYPE
{
	GREEN_ROTATE,
	GREEN_WHEEL,
	GREEN_CIRCLE,
	ORANGE_ROTATE,
	ORANGE_WHEEL,
	ORANGE_CIRCLE,
	BULLET_TYPE_MAX
};

//�G�l�~�[�o���b�g�̑��x�^�C�v��\���񋓑�
enum SPEEDTYPE
{
	DEFAULT_SPEED,										//�I�����W�A�΂̗������ʏ푬�x
	SLOW_ORANGE,										//�I�����W���x���A�΂�����
	SLOW_GREEN,											//�I�����W�������A�΂��x��
	SLOW_BOTH,											//�I�����W�Ɨ΂̗������x��
	FAST_BOTH,											//�I�����W�Ɨ΂̗���������
	SPEEDTYPE_MAX
};

//�G�l�~�[�o���b�g�\����
typedef struct 
{
	bool					active;						//�g�por���g�p�̔���

	D3DXVECTOR3				pos;						// �|���S���̍��W
	D3DXVECTOR3				rot;						// �|���S���̉�]��

	float					angle;						//���_�ւ̉�]�p
	float					radius;						//�|���S���̔��a

	int						countAnim;					// �A�j���[�V�����J�E���g
	int						patternAnim;				// �A�j���[�V�����p�^�[���i���o�[

	D3DXVECTOR3				velocity;					//�ړ����x
	BULLET_TYPE				type;						//�\������e�N�X�`���̃^�C�v

	int*					parentHp;					//�o���b�g�𔭎˂����G�l�~�[��HP
}ENEMYBULLET;

//�G�l�~�[�o���b�g���ˎ��Ɉ����œn���G�l�~�[�A�^�b�N�\����(type, pos, speed, angl)
typedef struct
{
	BULLET_TYPE type;									//���˂���G�l�~�[�o���b�g�̃^�C�v
	D3DXVECTOR3 pos;									//���ˈʒu
	float speed;										//���ˑ��x
	float angle;										//���ˊp�x�i0��Y���ɐ����ɔ�Ԃ��̂Ƃ���j	
}ENEMYATTACK;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemyBullet(int num);									//�G�l�~�[�o���b�g����������
void UninitEnemyBullet(void);										//�G�l�~�[�o���b�g�I������
void UpdateEnemyBullet(void);										//�G�l�~�[�o���b�g�X�V����
void DrawEnemyBullet(void);											//�G�l�~�[�o���b�g�`�揈��
ENEMYBULLET *GetEnemyBulletAdr(BULLET_TYPE type, int pno);			//�G�l�~�[�o���b�g�A�h���X�擾�֐�
ENEMYBULLET *SetEnemyBullet(ENEMYATTACK *param, int *parentHp);	//�G�l�~�[�o���b�g�Z�b�g�֐�
SPEEDTYPE GetSpeedType(void);										//���x�^�C�v���擾����֐�
void SetSpeedType(SPEEDTYPE type);									//���x�^�C�v���w�肷��֐�
void FireCircleBullet(ENEMYATTACK *param, int way, int *parentHp);				//�~��ɒe�𔭎˂���֐�
void FireWayBullet(ENEMYATTACK *param, int way, float range,int *parentHP);		//way��ɒe�𔭎˂���֐�
void DisableEnemyBullet(ENEMYBULLET *ptr);							//�G�l�~�[�o���b�g���A�N�e�B�u�ɃZ�b�g����֐�
void DisableAllEnemyBullet(void);									//�S�ẴG�l�~�[�o���b�g���A�N�e�B�u�ɃZ�b�g����֐�
void ConvertBulletToScoreItem(void);								//�S�ẴG�l�~�[�o���b�g���X�R�A�A�C�e���ɕϊ�����֐�

#ifdef _DEBUG
int GetEnemyBulletCnt(void);										//�Q�[�����ŃA�N�e�B�u�ȃG�l�~�[�o���b�g�̐����擾����֐�
#endif

#endif
