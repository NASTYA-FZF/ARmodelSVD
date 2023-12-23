#pragma once
#include <vector>
#include <complex>
#include <fstream>
#include <algorithm>
using namespace std;

typedef complex<double> base;

class signal
{
	double fd, error, porog;
	int N, start, p, num_c;
	vector<double> sig, new_sig, sigma, sigmaAP, sigma_bez, shum, sig_with_shum;
	vector< vector<double> > AKM;

public:
	//�����������
	signal(double* A, double* f, double* fi, double FD, int otsN, int start_c, int por, bool fl, double al, int virez);
	//����������� �� ���������
	signal();
	//���
	void fft(vector<base>& a, bool invert);
	//������� �������
	//double Es_for_srav(vector<double> func);
	double Es(vector<double> func);
	//���������� ���
	void GetAKP(vector<double>& akp, bool flag);
	//�������� ������
	vector<double> GetSig();
	//�������� ���
	void Create_AKM(bool flag);
	//��������� �������
	void Create_new_signal();
	//�������� ���������� ������
	vector<double> GetNewSig();
	//SVD
	int svd_hestenes(int m_m, int n_n, double* a, double* sigma);
	//��������� ����������� �����
	vector<double> GetSigma();
	//������������ ������������� ��-������
	void Norma();
	//new svd
	int new_svd_hestenes(int m_m, int n_n, float* a, float* sigma, vector<double>& my_koef);
	//���������� SVD
	void svd_posl(bool flag);
	//�������� ����
	void Create_shum(double al);
	//��������� ������
	double Get_porog();
	//��������� ����������� ������� ��� ���������
	vector<vector<double>> GetRazn();
};