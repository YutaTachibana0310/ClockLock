//=============================================================================
//
// �}�C���C�u�����w�b�_ [myLibrary.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _MYLIBRARY_H_
#define _MYLIBRARY_H_

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMyLibrary(int num);			//�}�C���C�u��������������
void UninitMyLibrary(void);				//�}�C���C�u�����I������

LPDIRECT3DTEXTURE9	CreateTextureFromFile(LPSTR szName, LPDIRECT3DDEVICE9 lpD3DDevice);	// �e�N�X�`���ǂݍ��݊֐�
float RandomRange(float x, float y);													// x�`y���͈͓̔��Ń����_���Ɏ�����Ԃ��֐�	
bool MoveToTarget(D3DXVECTOR3 *currentPos, D3DXVECTOR3 *targetPos, float speed);		//�ڕW���W�ւƈړ�����֐�
bool StartAccel(D3DXVECTOR3 *currentPos, D3DXVECTOR3 *direction, float speed, int frame, int currentFrame);	//���x�x�N�g����ڕW�X�s�[�h�։���������֐�
float SnipeTarget(D3DXVECTOR3 *pos, D3DXVECTOR3 *targetPos);							//�ݒ���W����ڕW���W�ւ̊p�x�����߂�֐�
float RotateToTarget(D3DXVECTOR3 *pos, D3DXVECTOR3 *targetPos, float currentRot, float angle); //�ݒ���W����ڕW���W�̕����։�]������֐�
void DrawYellowNum(VERTEX_2D *vertexWk, int num);										//���F��������`�悷��֐�
int WrapAround(int min, int max, int value);											//�Ώۂ̐��l�����b�v������֐�
int Clamp(int min, int max, int value);													//�Ώۂ̒l��͈͓��ɐ�������֐�
int Min(int val1, int val2);															//���������̒l��Ԃ��֐�
int Max(int val1, int val2);															//�傫�����̒l��Ԃ��֐�
float Clampf(float min, float max, float value);										//Clamp()��float��
float Minf(float val1, float val2);														//Min()��float��
float Maxf(float val1, float val2);														//Max()��float��
void ArrayShiftDown(int *buff, int startNum, int size);									//�z������փV�t�g����֐�
void VectorArrayShiftDown(D3DXVECTOR3 *array, int startNum, int size);					//�x�N�g���z������փV�t�g����֐�
void ArrayShiftUp(int *buff, int startNum, int size);									//�z�����փV�t�g����֐�
void VectorArrayShiftUp(D3DXVECTOR3 *array, int startNum, int size);					//�x�N�g���z�����փV�t�g����֐�
void ArrayCopy(int *dst, int *src, int size);											//�z��R�s�[�֐�
float EaseInCubic(float time, float start, float goal, float duration);
float EaseOutCubic(float time, float start, float goal, float duration);
float EaseInOutCubic(float time, float start, float goal, float duration);
#endif