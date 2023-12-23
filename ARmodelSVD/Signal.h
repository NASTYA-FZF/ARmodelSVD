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
	//конструктор
	signal(double* A, double* f, double* fi, double FD, int otsN, int start_c, int por, bool fl, double al, int virez);
	//Конструктор по умолчанию
	signal();
	//БПФ
	void fft(vector<base>& a, bool invert);
	//энергия сигнала
	//double Es_for_srav(vector<double> func);
	double Es(vector<double> func);
	//вычисление АКП
	void GetAKP(vector<double>& akp, bool flag);
	//получить сигнал
	vector<double> GetSig();
	//построим АКМ
	void Create_AKM(bool flag);
	//обрезание сигнала
	void Create_new_signal();
	//получить обрезанный сигнал
	vector<double> GetNewSig();
	//SVD
	int svd_hestenes(int m_m, int n_n, double* a, double* sigma);
	//получение сингулярных чисел
	vector<double> GetSigma();
	//нормирование коэффициентов АР-модели
	void Norma();
	//new svd
	int new_svd_hestenes(int m_m, int n_n, float* a, float* sigma, vector<double>& my_koef);
	//разложение SVD
	void svd_posl(bool flag);
	//создание шума
	void Create_shum(double al);
	//получение порога
	double Get_porog();
	//получение вырезанного отрезка для отрисовки
	vector<vector<double>> GetRazn();
};