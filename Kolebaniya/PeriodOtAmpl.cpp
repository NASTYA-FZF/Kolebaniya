// PeriodOtAmpl.cpp: файл реализации
//

#include "pch.h"
#include "Kolebaniya.h"
#include "afxdialogex.h"
#include "PeriodOtAmpl.h"


// Диалоговое окно PeriodOtAmpl

IMPLEMENT_DYNAMIC(PeriodOtAmpl, CDialogEx)

PeriodOtAmpl::PeriodOtAmpl(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m(0)
	, k(0)
	, l0(0)
	, colot_grap(_T(""))
	, stepX0(0)
	, minX0(0)
	, maxX0(0)
{
	period.Red.push_back(0);
	period.Green.push_back(0);
	period.Blue.push_back(0);

	CString str;
	colot_grap = L"R: "; str.Format(L"%d", period.Red[0]);
	colot_grap += str + L"; G: "; str.Format(L"%d", period.Green[0]);
	colot_grap += str + L"; B: "; str.Format(L"%d", period.Blue[0]);
	colot_grap += str + L";";

	stop = pause = false;

	InitializeCriticalSection(&cs);
	start_exper = false;
}

PeriodOtAmpl::~PeriodOtAmpl()
{
}

void PeriodOtAmpl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m);
	DDX_Text(pDX, IDC_EDIT2, k);
	DDX_Text(pDX, IDC_EDIT5, l0);
	DDX_Text(pDX, IDC_EDIT8, colot_grap);
	DDX_Text(pDX, IDC_EDIT10, stepX0);
	DDX_Text(pDX, IDC_EDIT12, minX0);
	DDX_Text(pDX, IDC_EDIT11, maxX0);

	DDX_Control(pDX, IDC_PERIOD, period);
	DDX_Control(pDX, IDC_BUTTON4, button_pause);
}


BEGIN_MESSAGE_MAP(PeriodOtAmpl, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &PeriodOtAmpl::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &PeriodOtAmpl::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &PeriodOtAmpl::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &PeriodOtAmpl::OnBnClickedButton4)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Обработчики сообщений PeriodOtAmpl


void PeriodOtAmpl::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();

	if (m == 0 || k == 0 || l0 == 0 || stepX0 == 0)
	{
		MessageBox(L"Моделирование невозможно, потому что один (или несколько) параметров равны нулю!", L"Предупреждение", NULL);
		return;
	}

	if ((maxX0 - minX0) <= 0)
	{
		MessageBox(L"Моделирование невозможно, потому что значение 'от' должно быть меньше значения 'до'!", L"Предупреждение", NULL);
		return;
	}

	if ((maxX0 - minX0) / stepX0 < 1)
	{
		MessageBox(L"Моделирование невозможно, потому что неверно выбран шаг!", L"Предупреждение", NULL);
		return;
	}

	stop = false;
	start_exper = true;
	CreateThread(NULL, NULL, period_thread, this, NULL, NULL);
	SetTimer(NULL, 16, NULL);
}


void PeriodOtAmpl::OnBnClickedButton2()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();
	CColorDialog color_dlg;

	if (!period.Red.empty()) period.Red.clear();
	if (!period.Green.empty()) period.Green.clear();
	if (!period.Blue.empty()) period.Blue.clear();

	if (color_dlg.DoModal() == IDOK)
	{
		period.Red.push_back(GetRValue(color_dlg.GetColor()));
		period.Green.push_back(GetGValue(color_dlg.GetColor()));
		period.Blue.push_back(GetBValue(color_dlg.GetColor()));
	}

	CString str;
	colot_grap = L"R: "; str.Format(L"%d", period.Red[0]);
	colot_grap += str + L"; G: "; str.Format(L"%d", period.Green[0]);
	colot_grap += str + L"; B: "; str.Format(L"%d", period.Blue[0]);
	colot_grap += str + L";";
	UpdateData(FALSE);
}

DWORD __stdcall PeriodOtAmpl::period_thread(PVOID p)
{
	PeriodOtAmpl* dlg = (PeriodOtAmpl*)p;

	double x, vx, t = 0., kx, step_t = 0.001;
	runge_kutta my;

	dlg->period.osX = L"x0, м";
	dlg->period.osY = L"T, с";
	dlg->period.nameGraph = L"Зависимость периода от амплитуды";

	dlg->period.stepX = dlg->stepX0;
	if (dlg->period.my_func.size() != 0) dlg->period.my_func.clear();
	if (dlg->period.my_x.size() != 0) dlg->period.my_x.clear();

	dlg->period.my_func.push_back(std::vector<double>());
	dlg->period.my_x.push_back(std::vector<double>());
	kx = dlg->minX0;

	dlg->period.cur = 3;

	dlg->period.minY = 0.;
	
	dlg->period.maxX = dlg->maxX0;
	dlg->period.minX = dlg->minX0;

	while(kx < dlg->maxX0)
	{
		if (dlg->pause) continue;

		if (dlg->stop) return 0;

		t = 0.;
		my = runge_kutta(step_t, fabs(kx), 0., dlg->k, dlg->m, 0., 0., dlg->l0);

		EnterCriticalSection(&dlg->cs);
		dlg->period.my_x[0].push_back(kx);
		my.algRK(t, x, vx);
		t += step_t;

		while (vx < 0)
		{
			my.algRK(t, x, vx);
			t += step_t;
		}

		dlg->period.my_func[0].push_back(2 * t);
		kx += dlg->stepX0;
		dlg->period.maxY = dlg->period.my_func[0][0];
		LeaveCriticalSection(&dlg->cs);
		//dlg->Invalidate(FALSE);
		Sleep(5);
	}
	dlg->period.maxY = dlg->period.my_func[0][0];
	//dlg->Invalidate(FALSE);

	return 0;
}

void PeriodOtAmpl::SetGraphMinMaxPeriod(int index)
{

}


void PeriodOtAmpl::OnBnClickedButton3()
{
	// TODO: добавьте свой код обработчика уведомлений
	stop = true;
	button_pause.SetWindowTextW(L"Пауза");
	pause = false;
}


void PeriodOtAmpl::OnBnClickedButton4()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (pause)
	{
		button_pause.SetWindowTextW(L"Пауза");
		pause = false;
	}
	else
	{
		button_pause.SetWindowTextW(L"Продолжить");
		pause = true;
	}
}


void PeriodOtAmpl::OnCancel()
{
	// TODO: добавьте специализированный код или вызов базового класса
	stop = true;
	button_pause.SetWindowTextW(L"Пауза");
	pause = false;
	DeleteCriticalSection(&cs);
	CDialogEx::OnCancel();
}


void PeriodOtAmpl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	EnterCriticalSection(&cs);
	if (!period.my_func.empty() && !period.my_x.empty() && start_exper) Invalidate(FALSE);
	LeaveCriticalSection(&cs);
	CDialogEx::OnTimer(nIDEvent);
}
