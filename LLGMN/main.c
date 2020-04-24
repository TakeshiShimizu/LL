#include <stdio.h>
#include "ll_func.h"
#include "parameters.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#pragma warning(disable : 4996)

int main(void){
    int i, j, k;        //����ϐ�
    int key;  //�Θb�p�I����
    LL_PARAM ll_param;  //�\����
    FILE *fp;
    double Loss_batch;
    double Loss_seq;        //�����֐�
    double epsilon;     //�w�K��
    int batch_count;    //�ꊇ�w�K��
    int seq_count;      //�����w�K��


    /**************************�w���E�f�q���̐ݒ�*****************************/
    /*
    printf("�R���|�[�l���g������͂��Ă��������F\n");
    scanf("%d", &ll_param.component_num);

    printf("���͑w�̑w������͂��Ă��������F\n");
    scanf("%d", &ll_param.input_layer_size);

    printf("�o�͑w�̑f�q������͂��Ă��������F\n");
    scanf("%d", &ll_param.output_layer_size);
    */

    ll_param.component_num = 2;
    ll_param.input_layer_size = 2;
    ll_param.output_layer_size = 4;

    /**************************�e��p�����[�^�̐ݒ�*****************************/
    ll_param = set_param(ll_param);

    double **train_data = NULL;     //���̓f�[�^
    double **w = NULL;              //�d��
    double ***layer_in = NULL;       //�e�w�̓���
    double ***layer_out = NULL;      //�e�w�̏o��
    double **t = NULL;              //�����f�[�^
    double **unlearn_data;          //���w�K�f�[�^
    double **output_x = NULL;       //����`�ϊ���̓��̓x�N�g��

    //train_data
    if((train_data = (double**)malloc((DATA_N + 1) * sizeof(double*))) == NULL) {
        exit(-1);
    }

    for(i = 0; i <= DATA_N; i++){
        if((train_data[i] = (double*)malloc((ll_param.input_layer_size + 1) * sizeof(double))) == NULL){
            exit(-1);
        }
    }

    //w
    if((w = (double**)malloc((ll_param.num_unit[0] + 1) * sizeof(double*))) == NULL) {
        exit(-1);
    }

    //�����̏�����
    srand((unsigned int)time(NULL));

    for(i = 0; i <= ll_param.num_unit[0]; i++) {
        if((w[i] = (double*)malloc((ll_param.num_unit[1] + 1) * sizeof(double))) == NULL) {
            exit(-1);
        }

        for(j = 0; j <= ll_param.num_unit[1]; j++) {
            if (j == ll_param.num_unit[1]) {
                w[i][j] = 0.0;
            }
            else {
                w[i][j] = ((double)rand() / RAND_MAX) * 2 - 1;  //������w��������
            }
        }
    }

    //�w�K�O�̃p�����[�^���o�͂���t�@�C�����J��
    fp = fopen("w.csv", "w");
    if( fp == NULL ){
        printf( "�t�@�C�����J���܂���\n");
        return -1;
    }

    printf("�d��\n");

    for(i = 0; i <= ll_param.num_unit[0]; i++) {
        printf("w[%d] ", i);
        for(j = 0; j <= ll_param.num_unit[1]; j++) {
            printf(" %9lf", w[i][j]);
            fprintf(fp, "%d,%d,%lf\n", i, j, w[i][j]);
        }printf("\n");
    }
    printf("\n");

    fclose(fp);

    //layer_out
    if((layer_out = (double***)malloc((DATA_N + 1) * sizeof(double**))) == NULL) {
        exit(-1);
    }

    for(i = 0; i <= DATA_N; i++) {
        if((layer_out[i] = (double**)malloc((LL_N) * sizeof(double*))) == NULL) {
            exit(-1);
        }

        for(j = 0; j < LL_N; j++) {
            if((layer_out[i][j] = (double*)malloc((ll_param.num_unit[j] + 1) * sizeof(double))) == NULL) {
                exit(-1);
            }

            for(k = 0; k <= ll_param.num_unit[j]; k++){
                layer_out[i][j][k] = 0.0;
            }
        }
    }

    //layer_in
    if((layer_in = (double***)malloc((DATA_N + 1) * sizeof(double**))) == NULL) {
        exit(-1);
    }

    for(i = 0; i <= DATA_N; i++) {
        if((layer_in[i] = (double**)malloc((LL_N) * sizeof(double*))) == NULL) {
            exit(-1);
        }

        for(j = 0; j < LL_N; j++) {
            if((layer_in[i][j] = (double*)malloc((ll_param.num_unit[j] + 1) * sizeof(double))) == NULL) {
                exit(-1);
            }

            for(k = 0; k <= ll_param.num_unit[j]; k++){
                layer_in[i][j][k] = 0.0;
            }
        }
    }

    //t
    if((t = (double**)malloc((DATA_N + 1) * sizeof(double*))) == NULL) {
        exit(-1);
    }

    for(i = 0; i <= DATA_N; i++){
        if((t[i] = (double*)malloc((ll_param.output_layer_size + 1) * sizeof(double))) == NULL){
            exit(-1);
        }
    }

    for(i = 0; i <= DATA_N; i++){
        for(j = 0; j <= ll_param.output_layer_size; j++){
            t[i][j] = 0.0;
        }
    }

    //unlearn_data
    if((unlearn_data = (double**)malloc((DATA_N + 1) * sizeof(double*))) == NULL) {
        exit(-1);
    }

    for(i = 0; i <= DATA_N; i++){
        if((unlearn_data[i] = (double*)malloc((ll_param.input_layer_size + 1) * sizeof(double))) == NULL){
            exit(-1);
        }
    }

    //output_x
    if((output_x = (double**)malloc((DATA_N + 1) * sizeof(double*))) == NULL) {
        exit(-1);
    }

    for(i = 0; i <= DATA_N; i++){
        if((output_x[i] = (double*)malloc((ll_param.num_unit[0] + 1) * sizeof(double))) == NULL){
            exit(-1);
        }
    }


    /**************************���t�f�[�^�E���w�K�f�[�^�̓ǂݍ���*****************************/

    //���t�f�[�^
    //�t�@�C���I�[�v��
    if ((fp = fopen("lea_sig.csv", "r")) == NULL) {
        printf("�t�@�C�����J���܂���ł����D\n");
        return -1;
    }
    //�f�[�^�ǂݍ���
    //printf("���t�f�[�^\n");
    i = 0;
    while ((fscanf(fp, "%lf,%lf", &train_data[i][1], &train_data[i][2])) != EOF) {
      //printf("%.1lf %.1lf\n", train_data[i][1], train_data[i][2]);
      i++;
    }
    //printf("\n");
    fclose(fp);

    //�����f�[�^
    if ((fp = fopen("lea_T_sig.csv", "r")) == NULL) {
        printf("�t�@�C�����J���܂���ł����D\n");
        return -1;
    }
    //�f�[�^�ǂݍ���
    //printf("�����f�[�^\n");
    i = 0;
    while ((fscanf(fp, "%lf,%lf,%lf,%lf", &t[i][1], &t[i][2], &t[i][3], &t[i][4])) != EOF) {
      //printf("%.1lf %.1lf %.1lf %.1lf\n", t[i][1], t[i][2], t[i][3], t[i][4]);
      i++;
    }
    //printf("\n");
    fclose(fp);

    /*
    Non_linear_tranform(ll_param, train_data, output_x);

    for (int l = 0; l < DATA_N; l++) {
        printf("%lf %lf %lf %lf %lf %lf\n", output_x[l][1], output_x[l][2], output_x[l][3], output_x[l][4], output_x[l][5], output_x[l][6]);
    }
    printf("\n");
    */

    /**************************�V�X�e��*****************************/


    printf("**************************************************\n");
    printf("LLGMN\n");
    printf("**************************************************\n");


    while(1){

      printf("[a] �ꊇ�X�V�w�K�@\n");
      printf("[b] �����X�V�w�K�@\n");
      printf("[c] �w�K�ς݃j���[�����̃e�X�g\n");
      printf("[d] �p�����[�^�̃��Z�b�g\n");
      printf("[ESC] �v���O�����I��\n");

      printf("**************************************************\n");
      printf("�L�[����͂��ċ@�\��I�����Ă��������F\n");
      key = getch();
      printf("**************************************************\n");

      switch (key) {
        case 'a':
        /**************************�ꊇ�w�K*************************************/
        printf("�w�K������͂��Ă��������F\n");
        scanf("%lf", &epsilon);
        printf("**************************************************\n");

        //���t�f�[�^�̔���`�ϊ�
        Non_linear_tranform(ll_param, train_data, output_x);

        printf("�ꊇ�w�K�̏������n�߂܂��D\n");
        //�������i�[����t�@�C�����J��
        fp = fopen("loss_batch.csv", "w");
        if( fp == NULL ){
            printf( "�t�@�C�����J���܂���\n");
            return -1;
        }
        batch_count = 0;    //�J�E���g�̏�����
        do{
            //���t�f�[�^�ɂ��Ĉ�����`���E�t�`�����s���C�d�݂̍X�V�̓G�|�b�N���Ƃɍs��
            Loss_batch = 0.0;
            for(i = 0; i < DATA_N; i++){
                //���`��
                forward(ll_param, output_x[i], w, layer_in[i], layer_out[i]);
                Loss_batch += Cost_Function(layer_out[i][2], t[i], ll_param.output_layer_size) / DATA_N;

                /*
                if (i % 10 == 0) {
                    printf("i = %d : %lf\n", i, Loss_batch);
                }
                */
            }

            //�d�݂̍X�V
            batch_update_w(ll_param, epsilon, w, t, layer_out, DATA_N);

            //�J�E���g
            batch_count++;

            if(batch_count % 1 == 0){
                printf("batch_count = %d : %lf\n", batch_count, Loss_batch);
            }
            fprintf(fp, "%d,%lf\n", batch_count, Loss_batch);
        }while(fabs(Loss_batch) > LOSS_MIN && batch_count < N);

        fclose(fp);

        //�w�K��̃p�����[�^���o�͂���t�@�C�����J��
        fp = fopen("w_batch.csv", "w");
        if( fp == NULL ){
            printf( "�t�@�C�����J���܂���\n");
            return -1;
        }

        for(i = 0; i <= ll_param.num_unit[0]; i++) {
            for(j = 0; j <= ll_param.num_unit[1]; j++) {
                fprintf(fp, "%d,%d,%lf\n", i, j, w[i][j]);
            }
        }

        fclose(fp);

          if(fabs(Loss_batch) < LOSS_MIN){
            printf("�����̑傫�������l�������܂����B\n");
            printf("�����̑傫�� : %lf\n", Loss_batch);
            printf("�J��Ԃ��񐔂�%d\n", batch_count);
        }else if(batch_count >= N){
            printf("�J��Ԃ��񐔂���萔�𒴂��܂����B\n");
            printf("�����̑傫�� : %lf\n", Loss_batch);
            printf("�J��Ԃ��񐔂�%d\n", batch_count);
          }else{
            printf("�����̑傫�� : %lf\n", Loss_batch);
            printf("�J��Ԃ��񐔂�%d\n", batch_count);
          }
          printf("**************************************************\n");
          break;

        case 'b':
        /**************************�����w�K*************************************/
        printf("�w�K������͂��Ă��������F\n");
        scanf("%lf", &epsilon);
        printf("**************************************************\n");

        //���t�f�[�^�̔���`�ϊ�
        Non_linear_tranform(ll_param, train_data, output_x);

        printf("�����w�K�̏������n�߂܂��D\n");
        //�����֐����o�͂���t�@�C�����J��
        fp = fopen("loss_seq.csv", "w");
        if( fp == NULL ){
            printf( "�t�@�C�����J���܂���\n");
            return -1;
        }
        seq_count = 0;  //�J�E���g�̏�����
        do{
            //���t�f�[�^�ɂ��Ĉ�����`���E�t�`���E�d�݂̍X�V���s��
            for(i = 0; i < DATA_N; i++){
                //���`��
                //���`��
                forward(ll_param, output_x[i], w, layer_in[i], layer_out[i]);
                Loss_seq = Cost_Function(layer_out[i][2], t[i], ll_param.output_layer_size);

                if (i % 1 == 0) {
                    printf("i = %d : %lf\n", i, Loss_seq);
                }

                //�d�݂̍X�V
                update_w(ll_param, epsilon, w, t[i], layer_out[i]);
            }
            //�J�E���g
            seq_count++;

            if(seq_count % 1 == 0){
                printf("seq_count = %d : %lf\n", seq_count, Loss_seq);
            }

            fprintf(fp, "%d,%lf\n", seq_count, Loss_seq);
        }while((fabs(Loss_seq) > LOSS_MIN) && (seq_count < N));

        fclose(fp);

        //�w�K��̃p�����[�^���o�͂���t�@�C�����J��
        fp = fopen("w_seq.csv", "w");
        if( fp == NULL ){
            printf( "�t�@�C�����J���܂���\n");
            return -1;
        }

        for(i = 0; i <= ll_param.num_unit[0]; i++) {
            for(j = 0; j <= ll_param.num_unit[1]; j++) {
                fprintf(fp, "%d,%d,%lf\n", i, j, w[i][j]);
            }
        }

        fclose(fp);

          if(fabs(Loss_seq) < LOSS_MIN){
            printf("�����̑傫�������l�������܂����B\n");
            printf("�����̑傫�� : %lf\n", Loss_seq);
            printf("�J��Ԃ��񐔂�%d\n", seq_count);
        }else if(seq_count > N){
            printf("�J��Ԃ��񐔂���萔�𒴂��܂����B\n");
            printf("�����̑傫�� : %lf\n", Loss_seq);
            printf("�J��Ԃ��񐔂�%d\n", seq_count);
          }else{
            printf("�����̑傫�� : %lf\n", Loss_seq);
            printf("�J��Ԃ��񐔂�%d\n", seq_count);
          }
          printf("**************************************************\n");
          break;

        case 'c':
        //���w�K�f�[�^
        //�t�@�C���I�[�v��
    	if ((fp = fopen("dis_sig.csv", "r")) == NULL) {
            printf("�t�@�C�����J���܂���ł����D\n");
            return -1;
    	}
    	//�f�[�^�ǂݍ���
        printf("���w�K�f�[�^\n");
        i = 0;
    	while ((fscanf(fp, "%lf,%lf", &unlearn_data[i][1], &unlearn_data[i][2])) != EOF) {
    		printf("%.1lf %.1lf\n", unlearn_data[i][1], unlearn_data[i][2]);
    		i++;
    	}
        printf("\n");
        fclose(fp);

        Non_linear_tranform(ll_param, unlearn_data, output_x);

        for(i = 0; i < DATA_N; i++){
            forward(ll_param, output_x[i], w, layer_in[i], layer_out[i]);
        }

        printf("���w�K�f�[�^�̏o��\n");
        for(i = 0; i < DATA_N; i++){
            printf("%d :\n", i);
            for(j = 1; j <= ll_param.output_layer_size; j++){
                printf(" %d: %lf", j, layer_out[i][2][j]);
            }
            printf("\n");
        }

         printf("**************************************************\n");
         break;

        case 'd':
            printf("�p�����[�^�i�d�݁C�o�C�A�X�j�����Z�b�g���܂�\n\n");

            srand((unsigned int)time(NULL));

            for(i = 0; i <= ll_param.num_unit[0]; i++) {
                for(j = 0; j <= ll_param.num_unit[1]; j++) {
                    w[i][j] = ((double)rand()/RAND_MAX) * 2 - 1;  //������w��������
                }
            }

            //�w�K�O�̃p�����[�^���o�͂���t�@�C�����J��
            fp = fopen("w.csv", "w");
            if( fp == NULL ){
                printf( "�t�@�C�����J���܂���\n");
                return -1;
            }

            for(i = 0; i <= ll_param.num_unit[0]; i++) {
                for(j = 0; j <= ll_param.num_unit[1]; j++) {
                    printf("w[%d][%d] = %lf\n", i, j, w[i][j]);
                    fprintf(fp, "%d,%d,%lf\n", i, j, w[i][j]);
                }
            }
            printf("\n");

            fclose(fp);

            printf("�p�����[�^�̃��Z�b�g���������܂����D\n");

            printf("**************************************************\n");
            break;

        case 0x1b:
            printf("�v���O�������I�����܂�.\n");
            return -1;
      }
    }

    //�������J��
    for(i = 0; i <= ll_param.input_layer_size; i++){
        free(train_data[i]);
    }
    free(train_data);
    for(i = 0; i <= ll_param.input_layer_size; i++){
        free(unlearn_data[i]);
    }
    free(unlearn_data);
    for(i = 0; i <= ll_param.component_num; i++){
        free(w[i]);
    }
    free(w);
    for(i = 0; i <= LL_N; i++){
        for(j = 0; j <= ll_param.output_layer_size; j++){
            free(layer_out[i][j]);
        }

        free(layer_out[i]);
    }
    free(layer_out);
    for(i = 0; i <= LL_N; i++){
        for(j = 0; j <= ll_param.output_layer_size; j++){
            free(layer_in[i][j]);
        }

        free(layer_in[i]);
    }
    free(layer_in);
    for(i = 0; i <= ll_param.output_layer_size; i++){
        free(t[i]);
    }
    free(t);
    for(i = 0; i <= ll_param.input_layer_size; i++){
        free(output_x[i]);
    }
    free(output_x);
    free(ll_param.num_unit);



    return 0;
}
