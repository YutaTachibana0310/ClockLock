//=============================================================================
//
// �R���C�_�[�w�b�_ [collider.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _COLLIDER_H_
#define _COLLIDER_H_

//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitCollider(void);							//�����蔻�菉��������
void UninitCollider(void);							//�����蔻��I������
void UpdateCollider(void);							//�����蔻��X�V����
void DrawCollider(void);							//�����蔻��`�揈��
bool HitBB(D3DXVECTOR4 bb1, D3DXVECTOR4 bb2);		//�o�E���f�B���O�{�b�N�X�̐ڐG����֐�
#endif