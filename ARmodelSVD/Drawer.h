#pragma once
#define _SILENCE_AMP_DEPRECATION_WARNINGS
#include <afxwin.h>
#include <vector>
#include <amp_graphics.h>
//#include "Analys.h"

using namespace std;

class Drawer
{
	// Прямоугольник области рисования.
	CRect frame;
	// Указатель на объект окна, обалсти рисования.
	CWnd * wnd;
	// Контекст рисования, привязанный к области рисования.
	CDC * dc;
	CDC* dc_forOnPaint;
	// Контекст рисования, привязанный к памяти.
	CDC memDC;
//	CDC memDC_forOnPaint;
	// Память для контекста рисования memDC.
	CBitmap bmp;
//	CBitmap bmp_forOnPaint;
	// Флаг для отслеживания состояния инициализации класса.
	bool init;
public:
	// Проинициализировать объект класса на основе HWND.
	void Create(HWND hWnd);

	vector<double> convert_range(vector <double>& data, double outmax, double outmin, double inmax, double inmin);

	double convert_range_d(double data, double outmax, double outmin, double inmax, double inmin);

	// Нарисовать сигнал.
	void DrawPoints(vector<double>& data, CString Ox, CString Oy, double data_x_min, double data_x_max, double step_x, bool flag); //flag = true - points, flag = false - graph

	double MaxF(vector<double> y);

	double MinF(vector<double> y);

	void DrawTwoSig(vector<double>& data, vector<double>& data1, CString Ox, CString Oy, double data_x_min, double data_x_max, double step_x);

	void ReDraw();
	//void DrawSr(vector<double>& data, signal s, CString Ox, CString Oy, double data_x_min, double data_x_max, double step_x);
};

