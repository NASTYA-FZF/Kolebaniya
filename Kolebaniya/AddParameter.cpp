// AddParameter.cpp: файл реализации
//

#include "pch.h"
#include "Kolebaniya.h"
#include "afxdialogex.h"
#include "AddParameter.h"
using namespace Gdiplus;

// Диалоговое окно AddParameter

IMPLEMENT_DYNAMIC(AddParameter, CDialogEx)

AddParameter::AddParameter(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m(0)
	, k(0)
	, l0(0)
	, k_s(0)
	, k_v(0)
	, x0(0)
	, vx0(0)
	, my_color(_T(""))
{
	r = 0;
	g = 0;
	b = 0;

	CString str;
	my_color = L"R: "; str.Format(L"%d", r);
	my_color += str + L"; G: "; str.Format(L"%d", g);
	my_color += str + L"; B: "; str.Format(L"%d", b);
	my_color += str + L";";
}

AddParameter::~AddParameter()
{
}

void AddParameter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m);
	DDX_Text(pDX, IDC_EDIT2, k);
	DDX_Text(pDX, IDC_EDIT5, l0);
	DDX_Text(pDX, IDC_EDIT3, k_s);
	DDX_Text(pDX, IDC_EDIT4, k_v);
	DDX_Text(pDX, IDC_EDIT6, x0);
	DDX_Text(pDX, IDC_EDIT7, vx0);
	DDX_Text(pDX, IDC_EDIT8, my_color);
}


BEGIN_MESSAGE_MAP(AddParameter, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &AddParameter::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &AddParameter::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &AddParameter::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT8, &AddParameter::OnEnChangeEdit8)
END_MESSAGE_MAP()


// Обработчики сообщений AddParameter


void AddParameter::OnBnClickedCancel()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnCancel();
}


void AddParameter::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();
	CColorDialog color_dlg;

	if (color_dlg.DoModal() == IDOK)
	{
		r = GetRValue(color_dlg.GetColor());
		g = GetGValue(color_dlg.GetColor());
		b = GetBValue(color_dlg.GetColor());
	}

	CString str;
	my_color = L"R: "; str.Format(L"%d", r);
	my_color += str + L"; G: "; str.Format(L"%d", g);
	my_color += str + L"; B: "; str.Format(L"%d", b);
	my_color += str + L";";
	UpdateData(FALSE);
}


void AddParameter::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();

	if (m == 0 || k == 0 || l0 == 0)
	{
		MessageBox(L"Моделирование невозможно, потому что один (или несколько) параметров равны нулю!", L"Предупреждение", NULL);
		return;
	}

	if (x0 == 0 && vx0 == 0)
	{
		MessageBox(L"Моделирование невозможно, потому что один (или несколько) параметров равны нулю!", L"Предупреждение", NULL);
		return;
	}

	CDialogEx::OnOK();
}

void AddParameter::OnEnChangeEdit8()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// функция и вызов CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
	CColorDialog color_dlg;

	if (color_dlg.DoModal() == IDOK)
	{
		r = GetRValue(color_dlg.GetColor());
		g = GetGValue(color_dlg.GetColor());
		b = GetBValue(color_dlg.GetColor());
	}
	my_brush = new CBrush(RGB(r, g, b));
}
