
// KolebaniyaDlg.h: файл заголовка
//

#pragma once
#include "MyGraph.h"
#include "Work.h"
#include "Multic.h"
#include "AddParameter.h"
#include "PeriodOtAmpl.h"

// Диалоговое окно CKolebaniyaDlg
class CKolebaniyaDlg : public CDialogEx
{
// Создание
public:
	CKolebaniyaDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KOLEBANIYA_DIALOG };
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
	afx_msg void OnBnClickedOk();
	std::vector<runge_kutta> my;
	MyGraph graphic;
	Multic my_mult;
	static DWORD WINAPI my_thread(PVOID p);
	HANDLE thread;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	void SetGraphMinMax();
	afx_msg void OnBnClickedButton2();
	bool pause;
	bool stop;
	CButton button_pause;
	UINT_PTR for_time;
	std::vector<double> x0, vx0, m_t, k_pr, l0_pr, k_s, k_v;
	int number_exper;
	CListBox my_list_parameter;
	afx_msg void OnBnClickedButton3();
	AddParameter child;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	bool not_koleb, start_exper;
	afx_msg void OnBnClickedButton7();
	
	PeriodOtAmpl grap;

	virtual void OnCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CRITICAL_SECTION cs;
};