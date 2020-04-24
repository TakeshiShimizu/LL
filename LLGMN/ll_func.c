#include "ll_func.h"
#include "parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


//�]���֐�
double Cost_Function(double *y, double *t, int size)
{
    double e = 0.0; //sum((y-t)^2)

    for(int k = 1; k <= size; k++){
        e -= t[k] * log(y[k]);
    }

    return e;
}


//���`��
void forward(LL_PARAM ll_param, double *data, double **w, double **layer_in, double **layer_out)
{
    int i, j, l;     //����ϐ�
    int h = ll_param.num_unit[0];
    int k = ll_param.output_layer_size;   //�N���X��
    int m = ll_param.component_num;    //�R���|�[�l���g��
    double exp_sum;   //���ԑw�ł̏o�͌v�Z���̕���


    /**************���͑w**************/
    layer_in[0] = data;
    layer_out[0] = layer_in[0];

    /**************���ԑw**************/
    //���ԑw�̓���
    for(i = 1; i <= k*m; i++){
      layer_in[1][i] = 0.0;

      for(j = 1; j <= h; j++){
        layer_in[1][i] += w[j][i] * layer_out[0][j];
      }
    }

    //���ԑw�̏o��
    //����̌v�Z
    exp_sum = 0.0;
    for(i = 1; i <= k*m; i++){
      exp_sum += exp(layer_in[1][i]);
    }

    for(i = 1; i<= k*m; i++){
      layer_out[1][i] = exp(layer_in[1][0]) / exp_sum;
    }

    /**************�o�͑w**************/
    //���ԑw�̑f�q�������[�v���C�o�͑w�̓��͂ɂ͒��ԑw�̏o�͂̃R���|�[�l���g���Ƃ̘a������
    l = 0;
    for(i = 1; i <= k*m; i++){
        //�R���|�[�l���g�̋�؂育�Ƃɏo�͑w�̓��͂�����������
        if(i % k == 0){
            l++;
            layer_in[2][l] = 0.0;
        }

        layer_in[2][l] += layer_out[1][i];
    }

    //�o��
    for(i = 1; i <= k; i++){
      layer_out[2][i] = layer_in[2][i];
    }
}


//�d�݂̍X�V
void update_w(LL_PARAM ll_param, double epsilon, double **w, double *t, double **layer_out)
{
    int i, j, l;     //����ϐ�
    int h = ll_param.num_unit[0];
    int k = ll_param.output_layer_size;   //�N���X��
    int m = ll_param.component_num;    //�R���|�[�l���g��
    double dJ_dw = 0.0;   //�]���֐��̔���

    for(i = 1; i <= h; i++){    //i : ���͂̎����̃C���f�b�N�X
        for(j = 1; j <= k; j++){    //j : �N���X�̃C���f�b�N�X
            for(l = 1; l <= m; l++){    //l : �R���|�[�l���g�̃C���f�b�N�X
                if(j*l < k*m){
                    //�����l�̌v�Z
                    dJ_dw = (layer_out[2][j] - t[j]) * layer_out[1][(j-1)*m + l] * layer_out[0][i] / layer_out[2][j];
                }

                //�X�V
                w[i][(j-1)*m + l] -= epsilon * dJ_dw;
            }
        }
    }
}


void batch_update_w(LL_PARAM ll_param, double epsilon, double **w, double **t, double ***layer_out, int batch_size)
{
    int i, j, l, n;     //����ϐ�
    int h = ll_param.num_unit[0];
    int k = ll_param.output_layer_size;   //�N���X��
    int m = ll_param.component_num;    //�R���|�[�l���g��
    double sum_dJ_dw;   //�]���֐��̔���

    for(n = 1; n <= batch_size; n++){
        for(i = 1; i <= h; i++){    //i : ���͂̎����̃C���f�b�N�X
            for(j = 1; j <= k; j++){    //j : �N���X�̃C���f�b�N�X
                for(l = 1; l <= m; l++){    //l : �R���|�[�l���g�̃C���f�b�N�X
                    sum_dJ_dw = 0.0;

                    if(j*l < k*m){
                        sum_dJ_dw += (layer_out[n][2][j] - t[n][j]) * layer_out[n][1][(j-1)*m + l] * layer_out[n][0][i] / layer_out[n][2][j];

                    }

                    //�X�V
                    w[i][(j-1)*m + l] -= epsilon * sum_dJ_dw;
                }
            }
        }
    }
}

//�\���̂̐ݒ�
LL_PARAM set_param(LL_PARAM ll_param)
{
    //num_unit�̃������m��
    if((ll_param.num_unit = (int*)malloc((LL_N) * sizeof(int))) == NULL){
        exit(-1);
    }

    //�e�w�̑f�q��
    ll_param.num_unit[0] = 1;
    for(int i = 1; i <= ll_param.input_layer_size; i++){
      ll_param.num_unit[0] += i;
    }
    ll_param.num_unit[1] = ll_param.output_layer_size * ll_param.component_num;     //K*Mk
    ll_param.num_unit[2] = ll_param.output_layer_size;                              //K

    return ll_param;
}


//���̓x�N�g���̔���`�ϊ�
void Non_linear_tranform(LL_PARAM ll_param, double **input_x, double **output_x)
{
    int i,j,k, n;  //����ϐ�
    int d = ll_param.input_layer_size;  //���̓x�N�g���̎���

    for (n = 0; n <= DATA_N; n++) {
        //��ꍀ��1
        output_x[n][1] = 1;

        /*
        //��񍀂͓��̓x�N�g��x
        for(i = 1; i <= d; i++){
            output_x[2][i] = input_x[i];
        }
        */

        //��O���ȍ~
        k = 0;  //���̓x�N�g��X�̃C���f�b�N�X
        for (i = 1; i <= d; i++) {
            j = 0;
            while (i + j <= d) {
                output_x[n][k + 2] = input_x[n][i] * input_x[n][i + j];
                //printf("output_x[%d][1] = input_x[%d] * input_x[%d] = %lf\n", k+3,i,j+i,output_x[k+3][1]);
                j++;
                k++;
            }
        }
    }
}