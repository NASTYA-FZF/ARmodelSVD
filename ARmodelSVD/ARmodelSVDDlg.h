
// ARmodelSVDDlg.h: файл заголовка
//

#pragma once
#include "Drawer.h"
#include "Signal.h"

// Диалоговое окно CARmodelSVDDlg
class CARmodelSVDDlg : public CDialogEx
{
// Создание
public:
	CARmodelSVDDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ARMODELSVD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	double A1;
	double A2;
	double A3;
	double f1;
	double f2;
	double f3;
	double fi1;
	double fi2;
	double fi3;
	double fd;
	int N;
	Drawer drwsig;
	afx_msg void OnBnClickedDraw();
	bool canDraw;
	int start_cut;
	int num_cut;
//	int p;
//	int p;
	int por_ARmodel;
	Drawer drwpor;
	int res_por;
	CString otkl;
	bool mnogAkp;
	afx_msg void OnBnClickedCheck1();
	double A4;
	double f4;
	double fi4;
	signal s;
	double alpha;
	int without_num;
	double min_alpha;
	double max_alpha;
	double step_alpha;
	afx_msg void OnBnClickedSeries();
	vector<double> SKO, SKON;
	bool stop;
	HANDLE thr, thrN;
	CButton my_button;
	afx_msg void OnBnClickedRedraw();
};

DWORD WINAPI ThrSKO(PVOID p);