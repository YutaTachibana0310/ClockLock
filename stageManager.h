//=============================================================================
//
// �X�e�[�W�}�l�[�W���[�w�b�_ [StageManager.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _STAGEMANAGER_H_
#define _STAGEMANAGER_H_

// �}�N����`
#define STAGEMANAGER_DATA_PATH ("data/STAGE/StageData.dat")				//�X�e�[�W��񂪕ۑ����ꂽ�t�@�C���ւ̃p�X
#define STAGEDATA_MAX			(256)						//�X�e�[�W�f�[�^�ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
//�X�e�[�W�ɏo������G�p�^�[���̍\����
typedef struct {
	int				interbal;					//�o���Ԋu
	int				enemyType;					//�o������G�̃^�C�v

	D3DXVECTOR3		initPos;					//�o�����W
	D3DXVECTOR3		targetPos;					//�ڕW���W
}STAGEDATA;

//�X�e�[�W�}�l�[�W���[�\����
typedef struct {
	int				cntFrame;					//�X�e�[�W�̌o�߃t���[����
	int				rank;						//�����N�i��Փx�j
	int				lastPopFrame;				//�Ō�ɓG���|�b�v�����t���[��

	STAGEDATA		data[STAGEDATA_MAX];		//�G�p�^�[���\���̂ւ̃|�C���^
	int				dataMax;					//�ǂݍ��񂾓G�p�^�[���̐�
	int				popNum;						//�|�b�v�����G�p�^�[���̐�
	int				state;						//�X�e�[�W���
	int				combo;						//�R���{��

	int				scaleState;
	float			comboScale;

}STAGEMANAGER;

enum STAGE_STATE
{
	STAGE_DEFAULT,
	STAGE_PREBOSS,
	STAGE_BOSSBATTLE,
	STAGE_CLEAR,
	STAGE_STATEMAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitStageManager(int num);							//�X�e�[�W�}�l�[�W���[����������
void UninitStageManager(void);								//�X�e�[�W�}�l�[�W���[�I������
void UpdateStageManager(void);								//�X�e�[�W�}�l�[�W���[�X�V����
void DrawStageManager(void);								//�X�e�[�W�}�l�[�W���[�`�揈��
STAGEMANAGER *GetStageManagerAdr(void);						//�X�e�[�W�}�l�[�W���[�A�h���X�擾�֐�
void SetStageState(int state);								//�X�e�[�g�Z�b�g�֐�
void AddComboCount(int value);								//�R���{���Z�b�g�֐�
#endif
