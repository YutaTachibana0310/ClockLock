//=============================================================================
//
// �}�C���C�u�������� [myLibrary.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define LENGTH_STARTSLOW			(100*100)			//�������n�܂鋗���i������2��Ōv�Z����̂�200*200)
#define SPEEDMAGNI_MIN				(0.15f)				//�������̍Œ�l
#define LENGTH_STOPMOVE				(10*10)				//�ړ����~�߂�ڕW�Ƃ̋���

#define YELLOWNUMBER_TEXTURE_NAME _T("data/TEXTURE/GUI/scoreNum.png")	//���F�������e�N�X�`��
#define NUMTEXTURE_DEVIDE_X			(10)								//�����e�N�X�`���̕�����

/*****************************************************************************
�}�N����`
*****************************************************************************/
enum TEX_INDEX
{
	YELLOWNUM,
	TEX_MAX
};

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[TEX_MAX];				//���F�������e�N�X�`���ւ̃|�C���^

/*****************************************************************************
����������
*****************************************************************************/
HRESULT InitMyLibrary(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (num == 0)
	{
		texture[YELLOWNUM] = CreateTextureFromFile((LPSTR)YELLOWNUMBER_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/*****************************************************************************
�I������
*****************************************************************************/
void UninitMyLibrary(void)
{
	for (int i = 0; i < TEX_MAX; i++)
	{
		if (texture[i] != NULL)
		{
			texture[i]->Release();
			texture[i] = NULL;
		}
	}
}

#ifdef _DEBUG
/******************************************************************************
//�֐���	�Fvoid DrawBoxCollider(D3DXVECTOR3 basePos, float width, float hight)
//����1		�FD3DXVECTOR3 basePos:�o�E���f�B���O�{�b�N�X�̍��㒸�_
//����2		�Ffloat width:�o�E���f�B���O�{�b�N�X�̕�
//�߂�l	�Ffloat hight:�o�E���f�B���O�{�b�N�X�̍���
//����		�F�o�E���f�B���O�{�b�N�X����������֐�
******************************************************************************/
void DrawBoxCollider(D3DXVECTOR3 basePos, float width, float hight)
{
	LPD3DXLINE line;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXVECTOR2 vec[NUM_BB_VERTEX] =
	{
		D3DXVECTOR2(basePos.x - width, basePos.y - hight),
		D3DXVECTOR2(basePos.x + width, basePos.y - hight),
		D3DXVECTOR2(basePos.x + width, basePos.y + hight),
		D3DXVECTOR2(basePos.x - width, basePos.y + hight),
		D3DXVECTOR2(basePos.x - width, basePos.y - hight)
	};

	D3DXCreateLine(pDevice, &line);

	line->SetAntialias(TRUE);
	line->SetWidth(2.0f);
	line->Begin();
	line->Draw(vec, NUM_BB_VERTEX, COLLIDER_COLOR);
	line->End();
	line->Release();
}
#endif

/******************************************************************************
//�֐���	�Ffloat RandomRange(float x, float y)
//����1		�Ffloat x	�F
//����2		�Ffloat y	�F
//�߂�l	�Fx�`y�͈͓̔��̎���
//����		�Fx����y�͈͓̔��Ń����_���Ɏ�����Ԃ��֐�
******************************************************************************/
float RandomRange(float x, float y)
{
	return rand() % 100 * (y - x) / 100.0f + x;
}

/******************************************************************************
//�֐���	�Fvoid MoveToTarget(D3DXVECTOR3 *currentPos, D3DXVECTOR3 *targetPos, float speed)
//����1		�FD3DXVECTOR3 *currentPos	�F���݂̍��W�i���s��A�X�V�����j
//����2		�FD3DXVECTOR3 *targetPos	�F�ڕW���W
//����3		�Ffloat speed				�F�ړ����x
//�߂�l	�F�ڕW���W�Ƃ̋��������ȉ��̏ꍇ�Ftrue	����ȊO�Ffalse
//����		�F�ڕW���W�ֈړ�����֐��i�ڕW���W�ɋ߂Â��ɂꌸ������j
******************************************************************************/
bool MoveToTarget(D3DXVECTOR3 *currentPos, D3DXVECTOR3 *targetPos, float speed)
{
	//���ݍ��W����ڕW���W�ւ̃x�N�g���𐶐�
	D3DXVECTOR3 tmpVec = *targetPos - *currentPos;


	//���ݍ��W�ƖڕW���W�̋������猸�������v�Z
	float length = D3DXVec3LengthSq(&tmpVec);

	if (length < LENGTH_STOPMOVE)
	{
		//�������K��l�ȉ��Ȃ�ړ����~�߂�
		return true;
	}

	float magni = (length > LENGTH_STARTSLOW) ? 1.0f : length / LENGTH_STARTSLOW;
	if (magni < SPEEDMAGNI_MIN)
	{
		magni = SPEEDMAGNI_MIN;
	}

	//���ݍ��W����ڕW���W�ւ̃x�N�g���𐳋K��
	D3DXVec3Normalize(&tmpVec, &tmpVec);

	//���ݍ��W���X�V
	*currentPos += tmpVec * speed * magni;

	return false;
}

/******************************************************************************
//�֐���	�Fbool StartAccel(D3DXVECTOR3 *velocity, float speed)
//����1		�FD3DXVECTOR3 *currentPos	�F���ݍ��W
//����2		�FD3DXVECTOR3 *direction	�F�ڕW����
//����3		�Ffloat speed				�F�ڕW�X�s�[�h
//����4		�Fint frame					�F�����ɂ�����t���[��
//����5		�Fint currentFrame			�F���݂̌o�߃t���[��
//�߂�l	�F���x���ڕW�ɒB���Ă���Ftrue	����ȊO�Ffalse
//����		�F�ڕW���x�։������Ă����֐�
******************************************************************************/
bool StartAccel(D3DXVECTOR3 *currentPos, D3DXVECTOR3 *direction, float speed, int frame, int currentFrame)
{
	D3DXVECTOR3 vec;
	D3DXVec3Normalize(&vec, direction);

	//�o�߃t���[���������ɕK�v�ȃt���[�����傫���ꍇ
	if (currentFrame > frame)
	{
		*currentPos += vec * speed;
		return true;
	}
	//�������ꍇ
	else
	{
		*currentPos += vec * (speed * currentFrame / frame);
		return false;
	}
}

/******************************************************************************
//�֐���	�Ffloat SnipePlayer(D3DXVECTOR3 *pos, D3DXVECTOR3 *targetPos)
//����1		�FD3DXVECTOR3 *pos			�F�ݒ���W
//����2		�FD3DXVECTOR3 *targetPos	�F�ڕW���W
//�߂�l	�F�ݒ���W����ڕW���W�ւ̊p�x�i���W�A���j
//����		�F�ݒ���W����ڕW���W�ւ̊p�x�����߂�֐�
******************************************************************************/
float SnipeTarget(D3DXVECTOR3 *pos, D3DXVECTOR3 *targetPos)
{
	float angle = atan2f(targetPos->y - pos->y, targetPos->x - pos->x);
	return angle;
}

/******************************************************************************
//�֐���	�Ffloat RotateToPlayer(D3DXVECTOR3 *pos, D3DXVECTOR3 *targetPos, float currentRot, float angle)
//����1		�FD3DXVECTOR3 *pos			�F�ݒ���W
//����2		�FD3DXVECTOR3 *targetPos	�F�ڕW���W
//����3		�Ffloat currentRot			�F���݂̊p�x(���W�A���j
//����4		�Ffloat angle				�F��]�p�x(���W�A��)
//�߂�l	�F�ڕW���W�̕��։�]������ꍇ�F��]�p�x*����		�����Ȃ��ꍇ�F0.0f
//����		�F�ݒ���W����ڕW���W�̕����։�]������֐�
******************************************************************************/
float RotateToTarget(D3DXVECTOR3 *pos, D3DXVECTOR3 *targetPos, float currentRot, float angle)
{

	float baseAngle = RADIAN(90.0f);

	D3DXVECTOR3 vec1 = D3DXVECTOR3(cosf(baseAngle + currentRot), sinf(baseAngle + currentRot), 0.0f);
	D3DXVECTOR3 vec2 = *targetPos - *pos;

	//�e�x�N�g���𐳋K��
	D3DXVec3Normalize(&vec1, &vec1);
	D3DXVec3Normalize(&vec2, &vec2);

	//���K�������x�N�g�����m�œ��ς��v�Z���ĉ�]�̕K�vor�s�K�v���v�Z
	float dot = D3DXVec3Dot(&vec1, &vec2);
	float tmp = cosf(angle);
	if (dot >= tmp)
	{
		return 0.0f;		//2�̃x�N�g���̊p�x����]�p��菬�������0.0f�����^�[��
	}

	//��]����������̔���
	int sign = (vec1.y*vec2.x - vec1.x*vec2.y > 0) ? -1 : 1;

	//��]�p�x*���������^�[��
	return angle * sign;
}

/*******************************************************************
//�֐���	�Fvoid DrawYellowNum(VERTEX_2D vertexWk[], int num)
//����1		�FVERTEX_2D vertexWk[]		�F�`��Ŏg�p���钸�_���
//����2		�Fint num					�F�`�悵��������
//�߂�l	�Fvoid
//����		�F���F��������`�悷��֐�
******************************************************************************/
void DrawYellowNum(VERTEX_2D *vertexWk, int num)
{
	float sizeX = 1.0f / NUMTEXTURE_DEVIDE_X;
	float x = sizeX * num;

	vertexWk[0].tex = D3DXVECTOR2(x, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(x + sizeX, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(x, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(x + sizeX, 1.0f);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture[YELLOWNUM]);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/*******************************************************************
//�֐���	�FWrapAround(int min, int max, int value)
//����1		�Fint min		�F�ŏ��l
//����2		�Fint max		�F�ő�l
//����3		�Fint value		�F�Ώۂ̒l
//�߂�l	�Fvoid
//����		�F�Ώۂ̒l���ŏ�����ő�͈̔͂Ń��b�v������֐�
******************************************************************************/
int WrapAround(int min, int max, int value)
{
	int n = (value - min) % (max - min);
	return (n >= 0) ? (n + min) : (n + max);
}

/*******************************************************************
//�֐���	�FClamp(int min, int max, int value)
//����1		�Fint min		�F�ŏ��l
//����2		�Fint max		�F�ő�l
//����3		�Fint value		�F�Ώۂ̒l
//�߂�l	�Fvoid
//����		�F�Ώۂ̒l���ŏ�����ő�͈̔͂ɐ�������֐�
******************************************************************************/
int Clamp(int min, int max, int value)
{
	return Max(min, Min(max, value));
}

/*******************************************************************
//�֐���	�FClampf(float min, float max, float value)
//����1		�Ffloat min		�F�ŏ��l
//����2		�Ffloat max		�F�ő�l
//����3		�Ffloat value		�F�Ώۂ̒l
//�߂�l	�Fvoid
//����		�F�Ώۂ̒l���ŏ�����ő�͈̔͂ɐ�������֐�
******************************************************************************/
float Clampf(float min, float max, float value)
{
	return Maxf(min, Minf(max, value));
}

/*******************************************************************
//�֐���	�FMin(int val1, int val2)
//����1		�Fint val1		�F�Ώےl1
//����2		�Fint val2		�F�Ώےl2
//�߂�l	�F���������̑Ώےl
//����		�F2�̑Ώۂ��珬�������̒l��Ԃ��֐�
******************************************************************************/
int Min(int val1, int val2)
{
	return (val1 < val2) ? val1 : val2;
}

/*******************************************************************
//�֐���	�FMax(int val1, int val2)
//����1		�Fint val1		�F�Ώےl1
//����2		�Fint val2		�F�Ώےl2
//�߂�l	�F�傫�����̑Ώےl
//����		�F2�̑Ώۂ���傫�����̒l��Ԃ��֐�
******************************************************************************/
int Max(int val1, int val2)
{
	return (val1 > val2) ? val1 : val2;
}

/*******************************************************************
//�֐���	�FMinf(float val1, float val2)
//����1		�Ffloat val1		�F�Ώےl1
//����2		�Ffloat val2		�F�Ώےl2
//�߂�l	�F���������̑Ώےl
//����		�F2�̑Ώۂ��珬�������̒l��Ԃ��֐�
******************************************************************************/
float Minf(float val1, float val2)
{
	return (val1 < val2) ? val1 : val2;
}

/*******************************************************************
//�֐���	�FMaxf(float val1, float val2)
//����1		�Ffloat val1		�F�Ώےl1
//����2		�Ffloat val2		�F�Ώےl2
//�߂�l	�F�傫�����̑Ώےl
//����		�F2�̑Ώۂ���傫�����̒l��Ԃ��֐�
******************************************************************************/
float Maxf(float val1, float val2)
{
	return (val1 > val2) ? val1 : val2;
}

/*******************************************************************
//�֐���	�Fvoid ArrayShiftDown(int *array, int startNum, int size)
//����1		�Fint *array		�F�V�t�g����z��̐擪�ւ̃|�C���^
//����2		�Fint startNum	�F�V�t�g���J�n����ʒu
//����3		�Fint size		�F�z��̃T�C�Y
//�߂�l	�Fvoid
//����		�F�z����J�n�ʒu������ɃV�t�g����֐�
******************************************************************************/
void ArrayShiftDown(int *array, int startNum, int size)
{
	if (startNum > size)
	{
		return;
	}

	for (int i = size - 1; i > startNum; i--)
	{
		array[i] = array[i - 1];
	}
}

/*******************************************************************
//�֐���	�Fvoid VectorArrayShiftDown(int *array, int startNum, int size)
//����1		�FD3DXVECTOR3 *array	�F�V�t�g����z��̐擪�ւ̃|�C���^
//����2		�Fint startNum	�F�V�t�g���J�n����ʒu
//����3		�Fint size		�F�z��̃T�C�Y
//�߂�l	�Fvoid
//����		�F�x�N�g���z����J�n�ʒu������ɃV�t�g����֐�
******************************************************************************/
void VectorArrayShiftDown(D3DXVECTOR3 *array, int startNum, int size)
{
	if (startNum > size)
	{
		return;
	}

	for (int i = size - 1; i > startNum; i--)
	{
		array[i] = array[i - 1];
	}
}

/*******************************************************************
//�֐���	�Fvoid ArrayShiftUp(int *array, int startNum, int size)
//����1		�Fint *array		�F�V�t�g����z��̐擪�ւ̃|�C���^
//����2		�Fint startNum		�F�V�t�g���J�n����ʒu
//����3		�Fint size			�F�z��̃T�C�Y
//�߂�l	�Fvoid
//����		�F�z����J�n�ʒu�����ɃV�t�g����֐�
******************************************************************************/
void ArrayShiftUp(int *array, int startNum, int size)
{
	if (startNum > size)
	{
		return;
	}

	for (int i = 0; i < startNum; i++)
	{
		array[i] = array[i + 1];
	}
}

/*******************************************************************
//�֐���	�Fvoid VectorArrayShiftUp(int *array, int startNum, int size)
//����1		�Fint *array		�F�V�t�g����z��̐擪�ւ̃|�C���^
//����2		�Fint startNum		�F�V�t�g���J�n����ʒu
//����3		�Fint size			�F�z��̃T�C�Y
//�߂�l	�Fvoid
//����		�F�x�N�g���z����J�n�ʒu�����ɃV�t�g����֐�
******************************************************************************/
void VectorArrayShiftUp(D3DXVECTOR3 *array, int startNum, int size)
{
	if (startNum > size)
	{
		return;
	}

	for (int i = 0; i < startNum; i++)
	{
		array[i] = array[i + 1];
	}
}

/*******************************************************************
//�֐���	�Fvoid ArrayShiftUp(int *array, int startNum, int size)
//����1		�Fint *dst		�F�R�s�[�Ώ�
//����2		�Fint *src		�F�R�s�[��
//����3		�Fint size		�F�z��̃T�C�Y
//�߂�l	�Fvoid
//����		�F�z����R�s�[����֐�
******************************************************************************/
void ArrayCopy(int *dst, int *src, int size)
{
	for (int i = 0; i < size; i++)
	{
		dst[i] = src[i];
	}
}

/*******************************************************************
//�֐���	�Ffloat EaseInCubic
//����1		�Ffloat time		�F���݂̐i�s�x
//����2		�Ffloat start		�F�����l
//����3		�Ffloat goal		�F�ڕW�l
//����4		�Ffloat duration	�F�i�s�x�̍̏W�n
//�߂�l	�F�C�[�W���O�̒l
//����		�F�C�[�W���O�v�Z����
********************************************************************/
float EaseInCubic(float time, float start, float goal, float duration)
{
	float diff = goal - start;
	float t = time / duration;

	return diff * t * t + start;
}

/*******************************************************************
//�֐���	�Ffloat EaseOutCubic
//����1		�Ffloat time		�F���݂̐i�s�x
//����2		�Ffloat start		�F�����l
//����3		�Ffloat goal		�F�ڕW�l
//����4		�Ffloat duration	�F�i�s�x�̍̏W�n
//�߂�l	�F�C�[�W���O�̒l
//����		�F�C�[�W���O�v�Z����
********************************************************************/
float EaseOutCubic(float time, float start, float goal, float duration)
{
	float diff = goal - start;
	float t = time / duration;

	return -diff * t * (t - 2.0f) + start;
}

/*******************************************************************
//�֐���	�Ffloat EaseInOutCubic
//����1		�Ffloat time		�F���݂̐i�s�x
//����2		�Ffloat start		�F�����l
//����3		�Ffloat goal		�F�ڕW�l
//����4		�Ffloat duration	�F�i�s�x�̍̏W�n
//�߂�l	�F�C�[�W���O�̒l
//����		�F�C�[�W���O�v�Z����
********************************************************************/
float EaseInOutCubic(float time, float start, float goal, float duration)
{
	float diff = goal - start;
	float t = 2.0f * time / duration;

	if (t < 1)
	{
		return diff / 2.0f * t * t + start;
	}
	else
	{
		t -= 1.0f;
		return -diff / 2.0f * (t * (t - 2) - 1) + start;
	}
}