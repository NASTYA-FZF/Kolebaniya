#pragma once
#include "afxdialogex.h"

// Диалоговое окно AddParameter

class AddParameter : public CDialogEx
{
	DECLARE_DYNAMIC(AddParameter)

public:
	AddParameter(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~AddParameter();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	double m;
	double k;
	double l0;
	double k_s;
	double k_v;
	double x0;
	double vx0;
	BYTE r, g, b;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	CBrush* my_brush;
	afx_msg void OnEnChangeEdit8();
	CString my_color;
};
