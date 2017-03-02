/*��truth ��cells��Ŀ����forest��cells��Ŀ��ʹ��f_small������������[SEmin, SEmax, SEavg, SEd, SEr, SEt, RSE, D]
 ����������ÿ��truth��cell����Ҫƥ���ԭ�򣬽�truth��cells�ֽ�Ϊ���ڵģ�f_equal()����С�ڵ����ȥ�����f_small()��;
���ʱ������forecast��cells����η��ʣ�*/
//��truth ��cells��forest��cells��Ŀ��ȣ�ʹ��f_equal������������[SEmin,SEmax,SEavg,SEd,SEr,SEt,RSE,D]
#include<Eigen/Dense>
#include<iostream>
#include<opencv2/opencv.hpp>
#include<Eigen/Core>
#include<complex>

using namespace std;
using namespace Eigen;

int f_equal_close(float Cx, float Cy, MatrixXf Zjk_intensity, int visit[], int b);
MatrixXf f_res(MatrixXcf Zjk, MatrixXcf Zj, float Cavg_Zjk, float Cavg_Zj, float Cmin_Zjk, float Cmin_Zj, float Cmax_Zjk, float Cmax_Zj);
MatrixXf f_bigtoequal(MatrixXcf Zjk, MatrixXcf Zj, MatrixXf Zjk_intensity, MatrixXf Zj_intensity);
MatrixXf f_small(MatrixXcf Zjk, MatrixXcf Zj, MatrixXf Zjk_intensity, MatrixXf Zj_intensity);
MatrixXf f_big(MatrixXcf Zjk, MatrixXcf Zj, MatrixXf Zjk_intensity, MatrixXf Zj_intensity)
{
	float SEmin, SEmax, SEavg, SEd, SEr, SEt, RSE, D;
	int bj = Zj.cols(); 
	int bjk = Zjk.cols();  // Zjk��ÿһ�д洢��һ��cells��������Ϣ����˾���Zj������b����cells����Ŀ
	MatrixXf T_res(1, 8);      // Tres()�����ۼ�truth�ĵ�i��cell��forecast�ĵ�i��cell�����
	T_res.setZero();
	int k = bj / bjk; //�������forecast��cellsƥ�����
	int m = bj%bjk;  //k��ʾtruth��cells����Ŀ��forcast��cells��Ŀ��k����m��ʾ���������ǿ���ʹ��k��f_equal()��1��f_small()���
	for (int i = 0; i <k; i++) {
		//�ҳ�λ��Ϊloc��δ���ʵľ���truth�ĵ�i��cell������(Cx, Cy)�����forecst��cell ����match
		MatrixXcf t_Zj(Zj.rows(), bjk);
		for (int j = 0; j < Zj.rows(); j++) {
			for (int c = bjk*i; c <  (i + 1)*bjk; c++) {
				t_Zj(j,c-bjk*i )._Val[0] = Zj(j, c)._Val[0];
				t_Zj(j, c-bjk*i)._Val[1] = Zj(j, c)._Val[1];
			}
		}
		MatrixXf t_Zj_intensity(Zj_intensity.rows(), bjk);
		for (int j = 0; j < Zj_intensity.rows(); j++) {
			for (int c = bjk*i; c < (i + 1)*bjk; c++) {
				t_Zj_intensity(j, c - bjk*i) = Zj_intensity(j, c);
			}
		}
		//f_bigtoequal()����������������¸��������ۼ�
		MatrixXf Res= f_bigtoequal(Zjk, t_Zj, Zjk_intensity, t_Zj_intensity);
		for (int m = 0; m < 8; m++) {
			T_res(0, m) = T_res(0, m) + Res(0, m);
		}
	}
	if (m > 0) { 
		//ѭ����ֵ����t_Zj��t_Zjk��������ʽ��,���ں������
		MatrixXcf t_Zj(Zj.rows(), m);
		for (int j = 0; j < Zj.rows(); j++) {
			for (int c = bjk*k; c < bj; c++) {
				t_Zj(j, c - bjk*k)._Val[0] = Zj(j, c)._Val[0];
				t_Zj(j, c - bjk*k)._Val[1] = Zj(j, c)._Val[1];
			}
		}
		MatrixXf t_Zj_intensity(Zjk_intensity.rows(), m);
		for (int j = 0; j < Zj_intensity.rows(); j++) {
			for (int c = bjk*k; c < bj; c++) {
				t_Zj_intensity(j, c - bjk*k) = Zj_intensity(j, c);
			}
		}

		// f_bigtoequal()����������������¸��������ۼ�
		MatrixXf Res = f_bigtoequal(Zjk, t_Zj, Zjk_intensity, t_Zj_intensity);
		for (int m = 0; m < 8; m++) {
			T_res(0, m) = T_res(0, m) + Res(0, m);
		}
	}
	SEmin = T_res(0, 0);
	SEmax = T_res(0, 1);
	SEavg = T_res(0, 2);
	//���չ�ʽSEd, SEt, SErȡ��ֵ
	SEd = T_res(0, 3) / bj;
	SEr = T_res(0, 4) / bj;
	SEt = T_res(0, 5) / bj;
	RSE = T_res(0, 6);
	D = T_res(0, 7);

	//�����ȫ������Res_alue()
	MatrixXf res_value(1, 8);
	res_value << SEmin,SEmax,SEavg,SEd,SEr,SEt,RSE,D;
	return res_value;
}
