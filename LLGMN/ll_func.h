#ifndef __INC_LL_FUNC_H
#define __INC_LL_FUNC_H

#include <stdio.h>
#include "parameters.h"


/*!----------------------------------------------------------------------------
 @brif �]���֐�

  �����f�[�^t��LLGMN�̏o��y����]���֐�t*log(y)�����߂�
 @param [in] y(double*) ������]������f�[�^
 @param [in] t(double*) �����f�[�^
 @param [in] size(int)�@y�����t�̃T�C�Y
 @return double �]���֐�
 @attention
 @par �X�V����
   - 2020/4/21
     -��{�I�ȋ@�\�̎��� (by Tsubasa Komiyama)

*/

double Cost_Function(double *y, double *t, int size);

/*!----------------------------------------------------------------------------
 @brif ���`���̏������s���֐�

  ���`���ɂ�������͑w�C���ԑw�C�o�͑w�ł̏������`
 @param [in] ll_param(LL_PARAM) LL_PARAM�\���̂̃f�[�^
 @param [in] data(double*) ���͂���f�[�^
 @param [in] w(double***) �d�݁E�o�C�A�X
 @param [in] h(int) ����`�ϊ���̓��̓x�N�g���̎���
 @param [in] layer_in(double**) �e�w�̓���
 @param [in] layer_out(double**) �e�w�̏o��
 @return �Ȃ�
 @attention
 @par �X�V����
   - 2020/4/22
     -��{�I�ȋ@�\�̎��� (by Tsubasa Komiyama)

*/

void forward(LL_PARAM ll_param, double *data, double **w, double **layer_in, double **layer_out);

/*!----------------------------------------------------------------------------
 @brif �d�݂̍X�V���s���֐�

  �d�݂��X�V����. �����w�K�Ŏg�p.
 @param [in] ll_param(LL_PARAM) LL_PARAM�\���̂̃f�[�^
 @param [in] epsilon(double) �w�K��
 @param [in] w(double***) �d��
 @param [in] h(int*) �e�w�̑f�q�����i�[���Ă���z��
 @param [in] dE_dw(double***) �t�`���œ��������̔���
 @return �Ȃ�
 @attention
 @par �X�V����
   - 2020/4/23
     -��{�I�ȋ@�\�̎��� (by Tsubasa Komiyama)

*/

void update_w(LL_PARAM ll_param, double epsilon, double **w, double *t, double **layer_out);

/*!----------------------------------------------------------------------------
 @brif �d�݂̍X�V���s���֐�

  �S���̋��t�f�[�^�̑�������d�݂��X�V����. �ꊇ�w�K�Ŏg�p.
 @param [in] ll_param(LL_PARAM) LL_PARAM�\���̂̃f�[�^
 @param [in] epsilon(double) �w�K��
 @param [in] w(double***) �d��
 @param [in] h(int*) �e�w�̑f�q�����i�[���Ă���z��
 @param [in] dE_dw_t(double***) dE_dw�̍��v
 @param [in] batch_size(int) �f�[�^��
 @return �Ȃ�
 @attention
 @par �X�V����
   - 2020/4/23
     -��{�I�ȋ@�\�̎��� (by Tsubasa Komiyama)
*/

void batch_update_w(LL_PARAM ll_param, double epsilon, double **w, double **t, double ***layer_out, int batch_size);

/*!----------------------------------------------------------------------------
 @brif �\����LL_PARAM�̏��������s���֐�

  LL_PARAM�̃������m�ہC���������s��
 @param [in] ll_param(LL_PARAM) LL_PARAM�\���̂̃f�[�^
 @return LL_PARAM
 @attention
 @par �X�V����
   - 2020/4/21
     -��{�I�ȋ@�\�̎��� (by Tsubasa Komiyama)
*/

LL_PARAM set_param(LL_PARAM ll_param);

/*!----------------------------------------------------------------------------
 @brif ���̓x�N�g���̔���`�ϊ�

  ���̓x�N�g��x�����`�ϊ��ɂ����LLGMN�ɓK�������̓x�N�g��X�ɕϊ�����
 @param [in] input_x(double*) ���̓x�N�g��x
 @param [out] output_x(double*) �ϊ���̓��̓x�N�g��X
 @return �Ȃ�
 @attention
 @par �X�V����
   - 2020/4/21
     -��{�I�ȋ@�\�̎��� (by Tsubasa Komiyama)

*/

void Non_linear_tranform(LL_PARAM ll_param, double** input_x, double** output_x);

/*!----------------------------------------------------------------------------
 @brif �^�[�~�i���A�g���N�^

  �^�[�~�i�����[�j���O�p�̍X�V�֐�
 @param [in] input_x(double*) ���̓x�N�g��x
 @param [out] output_x(double*) �ϊ���̓��̓x�N�g��X
 @return �Ȃ�
 @attention
 @par �X�V����
   - 2020/4/25
     -��{�I�ȋ@�\�̎��� (by Tsubasa Komiyama)

*/

void Non_linear_tranform(LL_PARAM ll_param, double** input_x, double** output_x);




#endif
