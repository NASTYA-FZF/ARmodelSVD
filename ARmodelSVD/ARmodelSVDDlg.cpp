
// ARmodelSVDDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "ARmodelSVD.h"
#include "ARmodelSVDDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CARmodelSVDDlg



CARmodelSVDDlg::CARmodelSVDDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ARMODELSVD_DIALOG, pParent)
	, A1(1)
	, A2(1)
	, A3(0.5)
	, f1(0.15)
	, f2(0.18)
	, f3(0.3)
	, fi1(0)
	, fi2(0)
	, fi3(0)
	, fd(0.6)
	, N(512)
	, start_cut(500)
	, num_cut(10)
	, por_ARmodel(9)
	, res_por(5)
	, otkl(_T(""))
	, A4(0.5)
	, f4(0.4)
	, fi4(0)
	, alpha(0)
	, without_num(10)
	, min_alpha(0)
	, max_alpha(0.5)
	, step_alpha(0.05)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CARmodelSVDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT6, A1);
	DDX_Text(pDX, IDC_EDIT4, A2);
	DDX_Text(pDX, IDC_EDIT3, A3);
	DDX_Text(pDX, IDC_EDIT8, f1);
	DDX_Text(pDX, IDC_EDIT7, f2);
	DDX_Text(pDX, IDC_EDIT5, f3);
	DDX_Text(pDX, IDC_EDIT11, fi1);
	DDX_Text(pDX, IDC_EDIT10, fi2);
	DDX_Text(pDX, IDC_EDIT9, fi3);
	DDX_Text(pDX, IDC_EDIT1, fd);
	DDX_Text(pDX, IDC_EDIT2, N);
	DDX_Text(pDX, IDC_EDIT12, start_cut);
	DDX_Text(pDX, IDC_EDIT14, por_ARmodel);
	DDX_Text(pDX, IDC_EDIT16, otkl);
	DDX_Text(pDX, IDC_EDIT17, A4);
	DDX_Text(pDX, IDC_EDIT18, f4);
	DDX_Text(pDX, IDC_EDIT19, fi4);
	DDX_Text(pDX, IDC_EDIT13, alpha);
	DDX_Text(pDX, IDC_EDIT15, without_num);
	DDX_Text(pDX, IDC_EDIT20, min_alpha);
	DDX_Text(pDX, IDC_EDIT21, max_alpha);
	DDX_Text(pDX, IDC_EDIT22, step_alpha);
	DDX_Control(pDX, IDC_SERIES, my_button);
}

BEGIN_MESSAGE_MAP(CARmodelSVDDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_DRAW, &CARmodelSVDDlg::OnBnClickedDraw)
	ON_BN_CLICKED(IDC_CHECK1, &CARmodelSVDDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_SERIES, &CARmodelSVDDlg::OnBnClickedSeries)
	ON_BN_CLICKED(IDC_REDRAW, &CARmodelSVDDlg::OnBnClickedRedraw)
END_MESSAGE_MAP()


// Обработчики сообщений CARmodelSVDDlg

BOOL CARmodelSVDDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
	drwsig.Create(GetDlgItem(IDC_SIGNAL)->GetSafeHwnd());
	drwpor.Create(GetDlgItem(IDC_PORYADOK)->GetSafeHwnd());
	canDraw = false;
	mnogAkp = false;
	stop = true;

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CARmodelSVDDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		if (canDraw) drwsig.ReDraw();
		canDraw = true;
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CARmodelSVDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CARmodelSVDDlg::OnBnClickedDraw()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	double A[] = { A1, A2, A3, A4 };
	double f[] = { f1, f2, f3, f4 };
	double fi[] = { fi1, fi2, fi3, fi4 };
	s = signal(A, f, fi, fd, N, start_cut, por_ARmodel, mnogAkp, alpha, without_num);

	double ot;
	while (1)
	{
		s.Create_shum(alpha);
		s.svd_posl(mnogAkp);
		s.Create_new_signal();
		auto SIG = s.GetSig(), NSIG = s.GetNewSig();
		vector<double> srav;
		for (int i = 0; i < N; i++) srav.push_back(SIG[i] - NSIG[i]);
		ot = sqrt(s.Es(srav));
		if (ot < 10 * s.Get_porog()) break;
	}
	otkl.Format(L"%.2f", ot);
	UpdateData(FALSE);
	auto result = s.GetRazn();
	drwsig.DrawTwoSig(result[0], result[1], L"t", L"A", start_cut / fd, (start_cut + without_num - 1) / fd, 1 / fd);
}


void CARmodelSVDDlg::OnBnClickedCheck1()
{
	// TODO: добавьте свой код обработчика уведомлений
	mnogAkp = !mnogAkp;
}

void CARmodelSVDDlg::OnBnClickedSeries()
{
	CloseHandle(thr);
	if (stop)
	{
		thr = CreateThread(NULL, NULL, ThrSKO, this, NULL, NULL);
		my_button.SetWindowTextW(L"Остановить");

	}
	else
	{
		TerminateThread(thr, NULL);
		CloseHandle(thr);
		my_button.SetWindowTextW(L"Зависимость");
	}
	stop = !stop;
}

DWORD __stdcall ThrSKO(PVOID p)
{
	CARmodelSVDDlg* dlg = (CARmodelSVDDlg*)p;
	//dlg->SKO.clear(); 
	dlg->UpdateData(TRUE);
	double A[] = { dlg->A1, dlg->A2, dlg->A3, dlg->A4 };
	double f[] = { dlg->f1, dlg->f2, dlg->f3, dlg->f4 };
	double fi[] = { dlg->fi1, dlg->fi2, dlg->fi3, dlg->fi4 };
	double ot, srav_ot, max_porog, ot1, srav_ot1;
	int max_realization = 1000;
	vector<double> srav, srav1, sko, sko1;
	for (double al = dlg->min_alpha; al <= dlg->max_alpha; al += dlg->step_alpha)
	{
		ot = 0.;
		ot1 = 0.;
		for (int realization = 0; realization < max_realization; realization++)
		{
			srav.clear();
			srav1.clear();
			dlg->s = signal(A, f, fi, dlg->fd, dlg->N, dlg->start_cut, dlg->por_ARmodel, dlg->mnogAkp, al, dlg->without_num);
			max_porog = dlg->s.Get_porog();

			dlg->s.svd_posl(false);
			dlg->s.Create_new_signal();
			auto SIG = dlg->s.GetSig(), NSIG = dlg->s.GetNewSig();
			for (int i = 0; i < dlg->N; i++) srav.push_back(SIG[i] - NSIG[i]);
			srav_ot = dlg->s.Es(srav);
			if (srav_ot > 10 * max_porog)
			{
				realization--;
				continue;
			}
			else
			{
				ot += srav_ot;
			}

			dlg->s.svd_posl(true);
			dlg->s.Create_new_signal();
			auto SIG1 = dlg->s.GetSig(), NSIG1 = dlg->s.GetNewSig();
			for (int i = 0; i < dlg->N; i++) srav1.push_back(SIG1[i] - NSIG1[i]);
			srav_ot1 = dlg->s.Es(srav1);
			if (srav_ot1 > 10 * max_porog)
			{
				realization--;
				continue;
			}
			else
			{
				ot1 += srav_ot1;
			}
		}
		sko.push_back(sqrt(ot / max_realization));
		sko1.push_back(sqrt(ot1 / max_realization));
		dlg->drwpor.DrawTwoSig(sko, sko1, L"a", L"StanO", dlg->min_alpha, dlg->max_alpha, dlg->step_alpha);
	}
	dlg->my_button.SetWindowTextW(L"Зависимость");
	dlg->stop = !dlg->stop;
	return 0;
}

void CARmodelSVDDlg::OnBnClickedRedraw()
{
	// TODO: добавьте свой код обработчика уведомлений
	double ot;
	while (1)
	{
		s.svd_posl(mnogAkp);
		s.Create_new_signal();
		auto SIG = s.GetSig(), NSIG = s.GetNewSig();
		vector<double> srav;
		for (int i = 0; i < N; i++) srav.push_back(SIG[i] - NSIG[i]);
		ot = sqrt(s.Es(srav));
		if (ot < 10 * s.Get_porog()) break;
	}
	otkl.Format(L"%.2f", ot);
	UpdateData(FALSE);
	auto result = s.GetRazn();
	drwsig.DrawTwoSig(result[0], result[1], L"t", L"A", start_cut / fd, (start_cut + without_num - 1) / fd, 1 / fd);
}
