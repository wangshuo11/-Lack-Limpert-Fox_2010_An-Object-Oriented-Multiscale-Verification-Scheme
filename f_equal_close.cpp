/*��Ҫ����Ѱ��δ������forecast��cells�о���truth��cell�����cell�����վͽ�ԭ��ƥ��, b��ʾcell����Ŀ��
visit��¼�Ƿ񱻷��ʣ�0����δ�з��ʣ�(Cx, Cy)����truth��cell�����ĵ㣬
Zjk_intensity�����forecast��cells������(Cx, Cy)����ˮǿ����Сֵ���ֵ�Լ���ֵ��Cmin, Cmax, Cavg��*/
#include<Eigen/Dense>
#include<iostream>
#include<opencv2/opencv.hpp>
#include<Eigen/Core>
#include<complex>

using namespace std;
using namespace Eigen;

int f_equal_close(float Cx, float Cy, MatrixXf Zjk_intensity, int visit[], int b)
{
	int loc = -1;        //��ʼ��λ��loc = -1
	float min_dis = 0;   //��ʼ������min_dis = 0;
	for (int i = 0; i < b; i++) {
		cout << visit[i] << endl;
		float t_Cx = Zjk_intensity(0, i);    //(tx_CX, ty_Cy)��Ӧ��forecast��i��cell����������
		float t_Cy = Zjk_intensity(1, i);
		if (visit[i] == 0) {               // �жϸ�λ���Ƿ��Ѿ���ƥ��
			if (loc == -1) {               //����ǵ�һ���ҵ�û���ʹ���cell����loc��min_dis
				loc = i;                   //��Ǵ�λ��
				min_dis = sqrt((Cx - t_Cx)*(Cx - t_Cx) + (Cy - t_Cy)*(Cy - t_Cy));  //���¾���
			}
			else {
				if (min_dis > sqrt((Cx - t_Cx)*(Cx - t_Cx) + (Cy - t_Cy)*(Cy - t_Cy))) //�ҵ������������δ�����ʣ������loc��min_dis
				{
					loc = i;                               ////��Ǵ�λ��
					min_dis = sqrt((Cx - t_Cx)*(Cx - t_Cx) + (Cy - t_Cy)*(Cy - t_Cy));  //���¾���
				}
			}
		}
	}
	visit[loc] = 1;     //��Ǳ��β������forecast��cellΪ1�����Ѿ�����
	return loc;
}


