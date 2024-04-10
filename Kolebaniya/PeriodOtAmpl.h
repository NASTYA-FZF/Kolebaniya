#pragma once
#include "afxdialogex.h"
#include "MyGraph.h"
#include "Work.h"


// Диалоговое окно PeriodOtAmpl

class PeriodOtAmpl : public CDialogEx
{
	DECLARE_DYNAMIC(PeriodOtAmpl)

public:
	PeriodOtAmpl(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~PeriodOtAmpl();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
//	MyGraph period;
	afx_msg void OnBnClickedButton1();
	double m;
	double k;
	double l0;
//	double k_s;
//	double k_v;
	CString colot_grap;
//	double vx0;
	double stepX0;
	double minX0;
	double maxX0;
	afx_msg void OnBnClickedButton2();
	static DWORD WINAPI period_thread(PVOID p);
	void SetGraphMinMaxPeriod(int index);
	MyGraph period;
	bool stop, pause;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	CButton button_pause;
	virtual void OnCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CRITICAL_SECTION cs;
	bool start_exper;
};
