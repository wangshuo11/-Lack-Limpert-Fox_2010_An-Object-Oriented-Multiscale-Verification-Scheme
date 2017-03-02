/* f_cells()��Ҫ�����ÿ������cell��(x + i*sin(angle)*y...)��������ʾ����Z,
�ڼ����ÿ��cell��Z_intensity(Cx, Cy, Cmin, Cmax, Cavg)��Cell������(Cx, Cy)����ˮǿ����Сֵ���ֵ�Լ���ֵ��Cmin, Cmax, Cavg��*/
#include<Eigen/Dense>   
#include<complex>          //������ͷ�ļ�
#include<iostream>
#include<math.h>
#include <opencv2/opencv.hpp>     //opencv ͼ����ͷ�ļ�   

using namespace std;           //�����ռ�
using namespace Eigen;
using namespace cv;


MatrixXf f_intensities(char *filename);
MatrixXf f_angle(MatrixXf boundary, float Cx, float Cy);
MatrixXcf f_Z(MatrixXf Res);
MatrixXcf f_cells(char *filename);
MatrixXf f_small(MatrixXcf Zjk, MatrixXcf Zj, MatrixXf Zjk_intensity, MatrixXf Zj_intensity);
MatrixXf f_equal(MatrixXcf Zjk, MatrixXcf Zj, MatrixXf Zjk_intensity, MatrixXf Zj_intensity);
MatrixXf f_big(MatrixXcf Zjk, MatrixXcf Zj, MatrixXf Zjk_intensity, MatrixXf Zj_intensity);
int f_havecells(MatrixXf Zj);

int main()
 {  
	 float flag, SEmin, SEmax, SEavg, SEd, SEr, SEt, RSE, D;
	char filename_fcst[256] = "anal_17.dat";
	char filename_anal[256] = "fcst_17.dat";
	
	MatrixXf  Zj_intensity= f_intensities(filename_anal).transpose();
	MatrixXf Zjk_intensity = f_intensities(filename_fcst).transpose();
	//����truth��cells��forecast ��cells�Ĳ�ֵ����Ϊ�����������
	MatrixXf res;
	if (f_havecells(Zj_intensity) == 0 ||f_havecells(Zjk_intensity) == 0) {
		for (int i = 0; i < 9; i++) {
			cout << -1 << " ";
		}
		cout << endl;
		return 0;
	}
	int Zjkb = Zjk_intensity.cols();        //����������ʹ��Zj.clos(),��Ϊ����MatrixXcf ����
	int Zjb = Zj_intensity.cols();
	MatrixXcf Zj = f_cells(filename_anal);
	MatrixXcf Zjk = f_cells(filename_fcst);
	 if (Zjkb == Zjb) { 
		// truth ��cells����forecast��cellsʱ
		res=f_equal(Zjk, Zj, Zjk_intensity, Zj_intensity);
		flag = 0;             ///����(Zjkb > Zjb) flag��ʾfalse, (Zjkb == Zjb)flag��ʾhit��flag(Zjkb < Zjb)flag��ʾmiss
	}
	else if (Zjkb < Zjb) {
		//truth ��cells��forecast��cells��ʱ
		  res= f_big(Zjk, Zj, Zjk_intensity, Zj_intensity);
		     flag = -1;
	}

	else if (Zjkb>Zjb) {      
		//truth ��cells��forecast��cells��ʱ
		 res = f_small(Zjk, Zj, Zjk_intensity, Zj_intensity);
	     flag = 1;
	}
	cout << flag << " " << res << endl;
	return 0;
}