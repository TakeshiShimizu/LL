#include <stdlib.h>
#ifndef __INC_PARAMETERS_H
#define __INC_PARAMETERS_H

#define N 10000        //���s�񐔂̍ő�l
#define LOSS_MIN 0.01  //������臒l
#define LL_N 3          //LLGMN�̑w��
#define DATA_N 800      //�f�[�^��

//�\����
typedef struct {
    int input_layer_size;   //���͑w�̃T�C�Y
    int component_num;      //���ԑw�̃T�C�Y
    int output_layer_size;  //�o�͑w�̃T�C�Y
    int *num_unit;              //�e�w�̑f�q��
} LL_PARAM;

//�ϐ�
double **train_data;        //���̓f�[�^
double **w;                //�d��
double ***layer_in;          //�e�w�̓���
double ***layer_out;         //�e�w�̏o��
double **t;                  //�����f�[�^
double **unlearn_data;      //���w�K�f�[�^
double **output_x;          //����`�ϊ���̓��̓x�N�g��

#endif
