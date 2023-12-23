#include "pch.h"
#define _USE_MATH_DEFINES
#include "Signal.h"
#include <math.h>

signal::signal(double* A, double* f, double* fi, double FD, int otsN, int start_c, int por, bool fl, double al, int virez)
{
	sig.clear();
	new_sig.clear();
	AKM.clear(); sigma.clear(); sigmaAP.clear(); sigma_bez.clear();
	fd = FD;
	N = otsN;
	start = start_c;
	p = 2 * por + 1;
	num_c = virez;
	int num_sin = 4;
	porog = 0.;

	for (int i = 0; i < N; i++)
	{
		sig.push_back(0.);
		for (int j = 0; j < num_sin; j++)
		{
			sig[i] += A[j] * sin(2 * M_PI * f[j] * i / fd + fi[j]);
		}
		if (porog < sig[i]) porog = sig[i];
	}
	Create_shum(al);

	new_sig = sig_with_shum;
	for (int i = start; i < start + num_c; i++)
	{
		new_sig[i] = 0.;
	}
}

signal::signal()
{
	fd = 1.;
	N = 512;
	start = 250;
	p = 4;

	for (int i = 0; i < N; i++)
	{
		sig.push_back(0.);
		sig[i] += 1. * sin(2 * M_PI * 0.4 * i / fd);
	}

	new_sig = sig;
	for (int i = start; i < N; i++)
	{
		new_sig[i] = 0.;
	}
	//Create_AKM();
}

void signal::fft(vector<base>& a, bool invert)
{
	int n = (int)a.size();
	if (n == 1)  return;

	vector<base> a0(n / 2), a1(n / 2);
	for (int i = 0, j = 0; i < n; i += 2, ++j) {
		a0[j] = a[i];
		a1[j] = a[i + 1];
	}
	fft(a0, invert);
	fft(a1, invert);

	double ang = 2 * M_PI / n * (invert ? -1 : 1);
	base w(1), wn(cos(ang), sin(ang));
	for (int i = 0; i < n / 2; ++i) {
		a[i] = a0[i] + w * a1[i];
		a[i + n / 2] = a0[i] - w * a1[i];
		if (invert)
			a[i] /= 2, a[i + n / 2] /= 2;
		w *= wn;
	}
}

//double signal::Es_for_srav(vector<double> func)
//{
//	double Esig(0);
//	for (int i = start; i < start + num_c; i++)
//	{
//		Esig += func[i] * func[i];
//	}
//	return Esig;
//}

double signal::Es(vector<double> func)
{
	double Esig(0);
	for (int i = 0; i < N; i++)
	{
		Esig += func[i] * func[i];
	}
	return Esig;
}

void signal::GetAKP(vector<double>& akp, bool flag)
{
	akp.clear();
	double workplus, workminus;
 	for (int m = 0; m < p; m++)
	{
		workplus = 0.;
		for (int k = 0; k < start - m; k++)
		{
			workplus += new_sig[k] * new_sig[k + m];
		}
		if (flag)
		{
			akp.push_back(workplus / start);
		}
		else
		{
			akp.push_back(workplus / (start - m));
		}
	}
}

vector<double> signal::GetSig()
{
	return sig_with_shum;
}

void signal::Create_AKM(bool flag)
{
	AKM.clear();
	vector<double> akp, work;
	GetAKP(akp, flag);
	for (int i = 0; i < p; i++)
	{
		for (int j = 0; j < p; j++)
		{
			work.push_back(akp[abs(i - j)]);
		}
		AKM.push_back(work);
		work.clear();
	}
	akp.clear();
}

void signal::Create_new_signal()
{
	new_sig = sig_with_shum;
	for (int i = start; i < start + num_c; i++)
	{
		new_sig[i] = 0.;
	}

	Norma();
	for (int i = start; i < start + num_c; i++)
	{
		for (int j = 1; j < p; j++)
		{
			new_sig[i] -= new_sig[i - j] * sigmaAP[j];
		}
	}
}

vector<double> signal::GetNewSig()
{
	return new_sig;
}

int signal::svd_hestenes(int m_m, int n_n, double* a, double* sigma)
{
	//m_m - кол-во строк автокоррел€ционной матрицы
	//n_n - кол-во столбцов автокоррел€ционной матрицы
	//a - автокоррел€ционна€ матрица
	//u - лева€ сингул€рна€ матрица
	//v - права€ сингул€рна€ матрица
	//sigma - матрица с сингул€рными числами
	float* u = new float[m_m * m_m - 1];
	float* v = new float[m_m * m_m - 1];

	float thr = 0.000001f;
	int n, m, i, j, l, k, lort, iter, in, ll, kk;
	float alfa, betta, hamma, eta, t, cos0, sin0, buf, s, r;
	n = n_n;
	m = m_m;
	for (i = 0; i < n; i++)
	{
		in = i * n;
		for (j = 0; j < n; j++)
			if (i == j) v[in + j] = 1.;
			else v[in + j] = 0.;
	}
	for (i = 0; i < m; i++)
	{
		in = i * n;
		for (j = 0; j < n; j++)
		{
			u[in + j] = a[in + j];
		}
	}

	iter = 0;
	while (1)
	{
		lort = 0;
		iter++;
		for (l = 0; l < n - 1; l++)
			for (k = l + 1; k < n; k++)
			{
				alfa = 0.; betta = 0.; hamma = 0.;
				for (i = 0; i < m; i++)
				{
					in = i * n;
					ll = in + l;
					kk = in + k;
					alfa += u[ll] * u[ll];
					betta += u[kk] * u[kk];
					hamma += u[ll] * u[kk];
				}

				if (sqrt(alfa * betta) < 1.e-10)	continue;
				if (fabs(hamma) / sqrt(alfa * betta) < thr)
					continue;

				lort = 1;
				eta = (betta - alfa) / (2.f * hamma);
				t = (eta / fabs(eta)) /
					(fabs(eta) + (float)sqrt(1.f + eta * eta));
				cos0 = 1.f / (float)sqrt(1.f + t * t);
				sin0 = t * cos0;

				for (i = 0; i < m; i++)
				{
					in = i * n;
					buf = u[in + l] * cos0 - u[in + k] * sin0;
					u[in + k] = u[in + l] * sin0 + u[in + k] * cos0;
					u[in + l] = buf;

					if (i >= n) continue;
					buf = v[in + l] * cos0 - v[in + k] * sin0;
					v[in + k] = v[in + l] * sin0 + v[in + k] * cos0;
					v[in + l] = buf;
				}
			}

		if (!lort) break;
	}

	for (i = 0; i < n; i++)
	{
		s = 0.;
		for (j = 0; j < m; j++)	s += u[j * n + i] * u[j * n + i];
		s = (float)sqrt(s);
		sigma[i] = s;
		if (s < 1.e-10)	continue;
		for (j = 0; j < m; j++)	u[j * n + i] = u[j * n + i] / s;
	}
	for (i = 0; i < n - 1; i++)
		for (j = i; j < n; j++)
			if (sigma[i] < sigma[j])
			{
				r = sigma[i]; sigma[i] = sigma[j]; sigma[j] = r;
				for (k = 0; k < m; k++)
				{
					r = u[i + k * n]; u[i + k * n] = u[j + k * n]; u[j + k * n] = r;
				}
				for (k = 0; k < n; k++)
				{
					r = v[i + k * n]; v[i + k * n] = v[j + k * n]; v[j + k * n] = r;
				}
			}

	return iter;
}

vector<double> signal::GetSigma()
{
	return sigma;
}

void signal::Norma()
{
	sigmaAP.clear();
	for (int i = 0; i < p; i++) sigmaAP.push_back(sigma_bez[i] / sigma_bez[0]);
}

int signal::new_svd_hestenes(int m_m, int n_n, float* a, float* sigma, vector<double>& my_koef)
{
	float* u = new float[m_m * m_m - 1];
	float* v = new float[m_m * m_m - 1];

	double thr = 1.E-4f, nul = 1.E-16f;
	int n, m, i, j, l, k, lort, iter, in, ll, kk;
	double alfa, betta, hamma, eta, t, cos0, sin0, buf, s;
	n = n_n;
	m = m_m;
	for (i = 0; i < n; i++)
	{
		in = i * n;
		for (j = 0; j < n; j++)
			if (i == j) v[in + j] = 1.;
			else v[in + j] = 0.;
	}
	for (i = 0; i < m; i++)
	{
		in = i * n;
		for (j = 0; j < n; j++)
		{
			u[in + j] = a[in + j];
		}
	}

	iter = 0;
	while (1)
	{
		lort = 0;
		iter++;
		for (l = 0; l < n - 1; l++)
			for (k = l + 1; k < n; k++)
			{
				alfa = 0.; betta = 0.; hamma = 0.;
				for (i = 0; i < m; i++)
				{
					in = i * n;
					ll = in + l;
					kk = in + k;
					alfa += u[ll] * u[ll];
					betta += u[kk] * u[kk];
					hamma += u[ll] * u[kk];
				}

				if (sqrt(alfa * betta) < nul)	continue;
				if (fabs(hamma) / sqrt(alfa * betta) < thr) continue;

				lort = 1;
				eta = (betta - alfa) / (2.f * hamma);
				t = (double)((eta / fabs(eta)) / (fabs(eta) + sqrt(1. + eta * eta)));
				cos0 = (double)(1. / sqrt(1. + t * t));
				sin0 = t * cos0;

				for (i = 0; i < m; i++)
				{
					in = i * n;
					buf = u[in + l] * cos0 - u[in + k] * sin0;
					u[in + k] = u[in + l] * sin0 + u[in + k] * cos0;
					u[in + l] = buf;

					if (i >= n) continue;
					buf = v[in + l] * cos0 - v[in + k] * sin0;
					v[in + k] = v[in + l] * sin0 + v[in + k] * cos0;
					v[in + l] = buf;
				}
			}

		if (!lort) break;
	}

	//ofstream out("b.txt");
	//double numer;
	//for (int i = 0; i < n; i++)
	//{
	//	numer = 0.;
	//	numer += u[n * i] * u[n * i];
	//	my_koef.push_back(numer);
	//}
	//out << numer << "\n";
	//out.close();

	for (i = 0; i < n; i++)
	{
		s = 0.;
		for (j = 0; j < m; j++)	s += u[j * n + i] * u[j * n + i];
		s = (double)sqrt(s);
		sigma[i] = s;
		if (s < nul)	continue;
		for (j = 0; j < m; j++)	u[j * n + i] /= s;
	}
	//======= Sortirovka ==============
	for (i = 0; i < n - 1; i++)
		for (j = i; j < n; j++)
			if (sigma[i] < sigma[j])
			{
				s = sigma[i]; sigma[i] = sigma[j]; sigma[j] = s;
				for (k = 0; k < m; k++)
				{
					s = u[i + k * n]; u[i + k * n] = u[j + k * n]; u[j + k * n] = s;
				}
				for (k = 0; k < n; k++)
				{
					s = v[i + k * n]; v[i + k * n] = v[j + k * n]; v[j + k * n] = s;
				}
			}


	ofstream out("b.txt");
	double numer(0.);
	for (int i = 0; i < n; i++)
	{
		numer += u[n * i + n - 1] * u[n * i + n - 1];
		my_koef.push_back(u[n * i + n - 1]);
		out << my_koef[i] << "\n";
	}
	out << numer << "\n";
	out.close();


	return iter;
}

void signal::svd_posl(bool flag)
{
	Create_AKM(flag);
	float* AAA = new float[p * p];
	float* SS = new float[p];
	for (int i = 0; i < p; i++)
	{
		for (int j = 0; j < p; j++)
		{
			AAA[i * p + j] = AKM[i][j];
		}
	}
	sigma_bez.clear();
	new_svd_hestenes(p, p, AAA, SS, sigma_bez);
	for (int i = 0; i < p; i++)
	{
		sigma.push_back(SS[i]);
	}
	delete[] AAA;
	delete[] SS;
}

void signal::Create_shum(double al)
{
	sig_with_shum.clear();
	shum.clear();
	for (int i = 0; i < N; i++)
	{
		shum.push_back((double)(rand()) / RAND_MAX * 2 - 1);
	}

	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < N; j++)
		{
			shum[j] += (double)(rand()) / RAND_MAX * 2 - 1;
		}
	}

	double SumSh(0.);
	for (int k = 0; k < N; k++)
	{
		SumSh += shum[k] * shum[k];
	}

	double bet = sqrt(al * Es(sig) / SumSh);

	for (int i = 0; i < N; i++)
	{
		sig_with_shum.push_back(sig[i] + shum[i] * bet);
		//sig[i] += shum[i] * bet;
		if (porog < sig_with_shum[i]) porog = sig_with_shum[i];
	}
}

double signal::Get_porog()
{
	return porog;
}

vector<vector<double>> signal::GetRazn()
{
	vector<vector<double>> res;
	vector<double> work;
	for (int i = start; i < start + num_c; i++)
	{
		work.push_back(sig_with_shum[i]);
	}
	res.push_back(work);
	work.clear();
	for (int i = start; i < start + num_c; i++)
	{
		work.push_back(new_sig[i]);
	}
	res.push_back(work);

	return res;
}
