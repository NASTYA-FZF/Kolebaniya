
// KolebaniyaDlg.cpp: файл реализации
//

#include "pch.h"
#define _USE_MATH_DEFINES
#include "framework.h"
#include "Kolebaniya.h"
#include "KolebaniyaDlg.h"
#include "afxdialogex.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CKolebaniyaDlg



CKolebaniyaDlg::CKolebaniyaDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KOLEBANIYA_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKolebaniyaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRAPH, graphic);
	DDX_Control(pDX, IDC_MUTL, my_mult);
	DDX_Control(pDX, IDC_BUTTON2, button_pause);
	DDX_Control(pDX, IDC_LIST1, my_list_parameter);
}

BEGIN_MESSAGE_MAP(CKolebaniyaDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CKolebaniyaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CKolebaniyaDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CKolebaniyaDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CKolebaniyaDlg::OnBnClickedButton2)
ON_BN_CLICKED(IDC_BUTTON3, &CKolebaniyaDlg::OnBnClickedButton3)
ON_BN_CLICKED(IDC_BUTTON4, &CKolebaniyaDlg::OnBnClickedButton4)
ON_BN_CLICKED(IDC_BUTTON5, &CKolebaniyaDlg::OnBnClickedButton5)
ON_BN_CLICKED(IDC_BUTTON6, &CKolebaniyaDlg::OnBnClickedButton6)
ON_BN_CLICKED(IDC_BUTTON7, &CKolebaniyaDlg::OnBnClickedButton7)
ON_WM_TIMER()
END_MESSAGE_MAP()


// Обработчики сообщений CKolebaniyaDlg

BOOL CKolebaniyaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок
	graphic.on_off = true;
	my_mult.on_off = true;
	pause = false;
	not_koleb = false;
	start_exper = false;
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
	InitializeCriticalSection(&cs);
	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CKolebaniyaDlg::OnPaint()
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
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CKolebaniyaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CKolebaniyaDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	stop = false;

	if (start_exper || pause)
	{
		MessageBox(L"Закончите прошлый эксперимент", L"Предупреждение", NULL);
		return;
	}

	number_exper = my_list_parameter.GetCount();
	if (number_exper == 0)
	{
		MessageBox(L"Список пуст", L"Предупреждение", NULL);
		return;
	}

	graphic.stepX = 0.001;
	graphic.my_func = std::vector<std::vector<double>>(number_exper);
	graphic.my_x = std::vector<std::vector<double>>(number_exper);
	my_mult.center = std::vector<double>(number_exper);

	SetGraphMinMax();
	
	if (!not_koleb)
	{
		thread = CreateThread(NULL, NULL, my_thread, this, NULL, NULL);
		SetTimer(NULL, 16, NULL);
		start_exper = true;
	}
	not_koleb = false;
}

DWORD __stdcall CKolebaniyaDlg::my_thread(PVOID p)
{
	CKolebaniyaDlg* dlg = (CKolebaniyaDlg*)p;
	
	dlg->graphic.osX = L"x, м";
	dlg->graphic.osY = L"Vx, м/с";

	if (dlg->number_exper == 1) dlg->graphic.nameGraph = L"Фазовая траектория";
	else dlg->graphic.nameGraph = L"Фазовый портрет";

	double x, vx, t = 0.;
	if (!dlg->my.empty()) dlg->my.clear();
	for (int i = 0; i < dlg->number_exper; i++)
	{
		
		dlg->my.push_back(runge_kutta(dlg->graphic.stepX, dlg->x0[i], dlg->vx0[i], dlg->k_pr[i], dlg->m_t[i], dlg->k_s[i], dlg->k_v[i], dlg->l0_pr[i]));

		if (dlg->graphic.my_func[i].size() != 0) dlg->graphic.my_func[i].clear();
		if (dlg->graphic.my_x[i].size() != 0) dlg->graphic.my_x[i].clear();
		dlg->graphic.my_func[i].push_back(dlg->vx0[i]);
		dlg->graphic.my_x[i].push_back(dlg->x0[i]);
		dlg->my_mult.center[i] = dlg->x0[i];
	}
	//dlg->Invalidate(FALSE);

	while(!dlg->stop)
	{
		if (dlg->pause) continue;
		EnterCriticalSection(&dlg->cs);
		for (int model = 0; model < dlg->number_exper; model++)
		{
			dlg->my[model].algRK(t, x, vx);
			dlg->graphic.my_func[model].push_back(vx);
			dlg->graphic.my_x[model].push_back(x);
			dlg->my_mult.center[model] = x;
		}
		//dlg->Invalidate(FALSE);
		LeaveCriticalSection(&dlg->cs);
		Sleep(5);
		t += dlg->graphic.stepX;
	}

	return 0;
}


void CKolebaniyaDlg::OnBnClickedCancel()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnCancel();
}


void CKolebaniyaDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (!start_exper && !pause)
	{
		MessageBox(L"Эксперимент не начат", L"Предупреждение", NULL);
		return;
	}
	start_exper = false;
	stop = true;
	button_pause.SetWindowTextW(L"Пауза");
	pause = false;
	CloseHandle(thread);

}

void CKolebaniyaDlg::SetGraphMinMax()
{
	double dopusk = 1.2, maxX, minX, maxY, minY;
	for (int i = 0; i < number_exper; i++)
	{
		if (x0[i] == 0 && vx0[i] == 0)
		{
			not_koleb = true;
			CString str_warning; str_warning.Format(L"%d", i + 1);
			MessageBox(L"Введите начальную координату или начальную скорость, иначе колебаний не будет\nСтрока: " + str_warning, L"Предупреждение", NULL);
			return;
		}
		if (vx0[i] == 0)
		{
			my_mult.minX = graphic.minX = -fabs(x0[i]);
			my_mult.maxX = graphic.maxX = fabs(x0[i]);
			graphic.minY = -fabs(sqrt(k_pr[i] / m_t[i]) * (sqrt(x0[i] * x0[i] + l0_pr[i] * l0_pr[i]) - l0_pr[i]));
			graphic.maxY = fabs(sqrt(k_pr[i] / m_t[i]) * (sqrt(x0[i] * x0[i] + l0_pr[i] * l0_pr[i]) - l0_pr[i]));
		}
		if (x0[i] == 0)
		{
			my_mult.minX = graphic.minX = -fabs(sqrt((sqrt(m_t[i] * vx0[i] * vx0[i] / k_pr[i]) + l0_pr[i]) * (sqrt(m_t[i] * vx0[i] * vx0[i] / k_pr[i]) + l0_pr[i]) - l0_pr[i] * l0_pr[i]));
			my_mult.maxX = graphic.maxX = fabs(sqrt((sqrt(m_t[i] * vx0[i] * vx0[i] / k_pr[i]) + l0_pr[i]) * (sqrt(m_t[i] * vx0[i] * vx0[i] / k_pr[i]) + l0_pr[i]) - l0_pr[i] * l0_pr[i]));
			graphic.minY = -fabs(vx0[i]);
			graphic.maxY = fabs(vx0[i]);
		}
		if (x0[i] != 0 && vx0[i] != 0)
		{
			graphic.minY = -fabs(sqrt(vx0[i] * vx0[i] + k_pr[i] * (sqrt(x0[i] * x0[i] + l0_pr[i] * l0_pr[i]) - l0_pr[i]) * (sqrt(x0[i] * x0[i] + l0_pr[i] * l0_pr[i]) - l0_pr[i]) / m_t[i]));
			graphic.maxY = fabs(sqrt(vx0[i] * vx0[i] + k_pr[i] * (sqrt(x0[i] * x0[i] + l0_pr[i] * l0_pr[i]) - l0_pr[i]) * (sqrt(x0[i] * x0[i] + l0_pr[i] * l0_pr[i]) - l0_pr[i]) / m_t[i]));
			my_mult.minX = graphic.minX = -fabs(sqrt((sqrt(m_t[i] * graphic.maxY * graphic.maxY / k_pr[i]) + l0_pr[i]) * (sqrt(m_t[i] * graphic.maxY * graphic.maxY / k_pr[i]) + l0_pr[i]) - l0_pr[i] * l0_pr[i]));
			my_mult.maxX = graphic.maxX = fabs(sqrt((sqrt(m_t[i] * graphic.maxY * graphic.maxY / k_pr[i]) + l0_pr[i]) * (sqrt(m_t[i] * graphic.maxY * graphic.maxY / k_pr[i]) + l0_pr[i]) - l0_pr[i] * l0_pr[i]));
		}
		if (i == 0)
		{
			minX = graphic.minX;
			maxX = graphic.maxX;
			minY = graphic.minY;
			maxY = graphic.maxY;
			my_mult.maxY = l0_pr[i];
		}
		else
		{
			if (graphic.minX < minX) minX = graphic.minX;
			if (graphic.maxX > maxX) maxX = graphic.maxX;
			if (graphic.minY < minY) minY = graphic.minY;
			if (graphic.maxY > maxY) maxY = graphic.maxY;
			if (l0_pr[i] > my_mult.maxY) my_mult.maxY = l0_pr[i];
		}
	}

	my_mult.minX = graphic.minX = minX;
	my_mult.maxX = graphic.maxX = maxX;
	my_mult.minX *= dopusk;
	my_mult.maxX *= dopusk;
	graphic.minY = minY;
	graphic.maxY = maxY;
}


void CKolebaniyaDlg::OnBnClickedButton2()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (!start_exper && !pause)
	{
		MessageBox(L"Эксперимент не начат", L"Предупреждение", NULL);
		return;
	}
	if (pause)
	{
		
		number_exper = my_list_parameter.GetCount();
		if (number_exper == 0)
		{
			MessageBox(L"Список пуст", L"Предупреждение", NULL);
			return;
		}

		int last = graphic.my_x.size();
		std::vector< std::vector<double>> last_x = graphic.my_x;
		std::vector< std::vector<double>> last_vx = graphic.my_func;

		my.clear();
		graphic.my_func.clear();
		graphic.my_x.clear();
		my_mult.center.clear();

		graphic.my_func = std::vector<std::vector<double>>(number_exper);
		graphic.my_x = std::vector<std::vector<double>>(number_exper);
		my_mult.center = std::vector<double>(number_exper);

		for (int i = 0; i < number_exper; i++)
		{
			if (i < last)
			{
				my.push_back(runge_kutta(graphic.stepX, last_x[i].back(), last_vx[i].back(), k_pr[i], m_t[i], k_s[i], k_v[i], l0_pr[i]));
				graphic.my_func[i].push_back(last_vx[i].back());
				graphic.my_x[i].push_back(last_x[i].back());
				my_mult.center[0] = (last_x[i].back());
			}
			else
			{
				my.push_back(runge_kutta(graphic.stepX, x0[i], vx0[i], k_pr[i], m_t[i], k_s[i], k_v[i], l0_pr[i]));

				graphic.my_func[i].push_back(vx0[i]);
				graphic.my_x[i].push_back(x0[i]);
				my_mult.center[0] = (x0[i]);
			}
		}
		SetGraphMinMax();
		button_pause.SetWindowTextW(L"Пауза");
		pause = false;
		start_exper = true;
	}
	else
	{
		button_pause.SetWindowTextW(L"Продолжить");
		start_exper = false;
		pause = true;
	}
}

void CKolebaniyaDlg::OnBnClickedButton3()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (start_exper)
	{
		MessageBox(L"Остановите эксперимент", L"Предупреждение", NULL);
		return;
	}

	if (child.DoModal() == IDOK)
	{
		graphic.Red.push_back(child.r);
		graphic.Green.push_back(child.g);
		graphic.Blue.push_back(child.b);

		my_mult.Red.push_back(child.r);
		my_mult.Green.push_back(child.g);
		my_mult.Blue.push_back(child.b);

		m_t.push_back(child.m);
		k_pr.push_back(child.k);
		x0.push_back(child.x0);
		vx0.push_back(child.vx0);
		k_s.push_back(child.k_s);
		k_v.push_back(child.k_v);
		l0_pr.push_back(child.l0);
		my_mult.l0.push_back(child.l0);

		CString str, all;
		all = L"m = "; str.Format(L"%.2f", m_t.back());
		all += str + L"; k = "; str.Format(L"%.2f", k_pr.back());
		all += str + L"; mu = "; str.Format(L"%.2f", k_s.back());
		all += str + L"; k1 = "; str.Format(L"%.2f", k_v.back());
		all += str + L"; l0 = "; str.Format(L"%.2f", l0_pr.back());
		all += str + L"; x0 = "; str.Format(L"%.2f", x0.back());
		all += str + L"; vx0 = "; str.Format(L"%.2f", vx0.back());
		all += str + L"; R: "; str.Format(L"%d", my_mult.Red.back());
		all += str + L"; G: "; str.Format(L"%d", my_mult.Green.back());
		all += str + L"; B: "; str.Format(L"%d", my_mult.Blue.back());
		all += str + L";";
		my_list_parameter.InsertString(-1, all);
	}
	
}

void CKolebaniyaDlg::OnBnClickedButton4()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (start_exper)
	{
		MessageBox(L"Остановите эксперимент", L"Предупреждение", NULL);
		return;
	}

	if (my_list_parameter.GetCount() == 0)
	{
		MessageBox(L"Добавьте модель", L"Предупреждение", NULL);
		return;
	}

	int number = my_list_parameter.GetCurSel();

	if (number == LB_ERR)
	{
		MessageBox(L"Выделите строку", L"Предупреждение", NULL);
		return;
	}

	child.r = graphic.Red[number];
	child.g = graphic.Green[number];
	child.b = graphic.Blue[number];

	child.m = m_t[number];
	child.k = k_pr[number];
	child.x0 = x0[number];
	child.vx0 = vx0[number];
	child.k_s = k_s[number];
	child.k_v = k_v[number];
	child.l0 = l0_pr[number];

	if (child.DoModal() == IDOK)
	{

		graphic.Red[number] = child.r;
		graphic.Green[number] = child.g;
		graphic.Blue[number] = child.b;

		my_mult.Red[number] = child.r;
		my_mult.Green[number] = child.g;
		my_mult.Blue[number] = child.b;

		m_t[number] = child.m;
		k_pr[number] = child.k;
		x0[number] = child.x0;
		vx0[number] = child.vx0;
		k_s[number] = child.k_s;
		k_v[number] = child.k_v;
		l0_pr[number] = child.l0;
		my_mult.l0[number] = child.l0;

		CString str, all;
		all = L"m = "; str.Format(L"%.2f", m_t[number]);
		all += str + L"; k = "; str.Format(L"%.2f", k_pr[number]);
		all += str + L"; mu = "; str.Format(L"%.2f", k_s[number]);
		all += str + L"; k1 = "; str.Format(L"%.2f", k_v[number]);
		all += str + L"; l0 = "; str.Format(L"%.2f", l0_pr[number]);
		all += str + L"; x0 = "; str.Format(L"%.2f", x0[number]);
		all += str + L"; vx0 = "; str.Format(L"%.2f", vx0[number]);
		all += str + L"; R: "; str.Format(L"%d", my_mult.Red.back());
		all += str + L"; G: "; str.Format(L"%d", my_mult.Green.back());
		all += str + L"; B: "; str.Format(L"%d", my_mult.Blue.back());
		all += str + L";";
		my_list_parameter.DeleteString(number);
		my_list_parameter.InsertString(number, all);
	}
}


void CKolebaniyaDlg::OnBnClickedButton5()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (start_exper)
	{
		MessageBox(L"Остановите эксперимент", L"Предупреждение", NULL);
		return;
	}

	if (my_list_parameter.GetCount() == 0)
	{
		MessageBox(L"Список пуст", L"Предупреждение", NULL);
		return;
	}

	int number = my_list_parameter.GetCurSel();

	if (number == LB_ERR)
	{
		MessageBox(L"Выделите строку", L"Предупреждение", NULL);
		return;
	}

	my_list_parameter.DeleteString(number);

	graphic.Red.erase(graphic.Red.begin() + number);
	graphic.Green.erase(graphic.Green.begin() + number);
	graphic.Blue.erase(graphic.Blue.begin() + number);

	my_mult.Red.erase(my_mult.Red.begin() + number);
	my_mult.Green.erase(my_mult.Green.begin() + number);
	my_mult.Blue.erase(my_mult.Blue.begin() + number);

	m_t.erase(m_t.begin() + number);
	k_pr.erase(k_pr.begin() + number);
	x0.erase(x0.begin() + number);
	vx0.erase(vx0.begin() + number);
	k_s.erase(k_s.begin() + number);
	k_v.erase(k_v.begin() + number);
	l0_pr.erase(l0_pr.begin() + number);
	my_mult.l0.erase(my_mult.l0.begin() + number);
}


void CKolebaniyaDlg::OnBnClickedButton6()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (start_exper)
	{
		MessageBox(L"Остановите эксперимент", L"Предупреждение", NULL);
		return;
	}

	int size = my_list_parameter.GetCount();
	if (size == 0)
	{
		MessageBox(L"Список пуст", L"Предупреждение", NULL);
		return;
	}

	for (int i = 0; i < size; i++)
	{
		my_list_parameter.DeleteString(0);
	}

	graphic.Red.clear();
	graphic.Green.clear();
	graphic.Blue.clear();

	my_mult.Red.clear();
	my_mult.Green.clear();
	my_mult.Blue.clear();

	m_t.clear();
	k_pr.clear();
	x0.clear();
	vx0.clear();
	k_s.clear();
	k_v.clear();
	l0_pr.clear();

	my_mult.l0.clear();
}


void CKolebaniyaDlg::OnBnClickedButton7()
{
	// TODO: добавьте свой код обработчика уведомлений
	//CreateThread(NULL, NULL, period_thread, this, NULL, NULL);
	this->ShowWindow(SW_HIDE);
	grap.period.on_off = true;
	grap.DoModal();
	this->ShowWindow(SW_SHOW);
	graphic.on_off = true;
	my_mult.on_off = true;
}
//
//void CKolebaniyaDlg::SetGraphMinMaxPeriod(int i)
//{
//	double dopusk = 1.2, maxX, minX, maxY, minY, resMinX, resMaxX, resMinY, resMaxY;
//	if (x0[i] == 0 && vx0[i] == 0)
//	{
//		not_koleb = true;
//		CString str_warning; str_warning.Format(L"%d", i + 1);
//		MessageBox(L"Введите начальную координату или начальную скорость, иначе колебаний не будет\nСтрока: " + str_warning, L"Предупреждение", NULL);
//		return;
//	}
//	if (vx0[i] == 0)
//	{
//		resMinX = -fabs(x0[i]);
//		resMaxX = fabs(x0[i]);
//	}
//	if (x0[i] == 0)
//	{
//		resMinX = -fabs(sqrt((sqrt(m_t[i] * vx0[i] * vx0[i] / k_pr[i]) + l0_pr[i]) * (sqrt(m_t[i] * vx0[i] * vx0[i] / k_pr[i]) + l0_pr[i]) - l0_pr[i] * l0_pr[i]));
//		resMaxX = fabs(sqrt((sqrt(m_t[i] * vx0[i] * vx0[i] / k_pr[i]) + l0_pr[i]) * (sqrt(m_t[i] * vx0[i] * vx0[i] / k_pr[i]) + l0_pr[i]) - l0_pr[i] * l0_pr[i]));
//	}
//	if (x0[i] != 0 && vx0[i] != 0)
//	{
//		resMinY = -fabs(sqrt(vx0[i] * vx0[i] + k_pr[i] * (sqrt(x0[i] * x0[i] + l0_pr[i] * l0_pr[i]) - l0_pr[i]) * (sqrt(x0[i] * x0[i] + l0_pr[i] * l0_pr[i]) - l0_pr[i]) / m_t[i]));
//		resMaxY = fabs(sqrt(vx0[i] * vx0[i] + k_pr[i] * (sqrt(x0[i] * x0[i] + l0_pr[i] * l0_pr[i]) - l0_pr[i]) * (sqrt(x0[i] * x0[i] + l0_pr[i] * l0_pr[i]) - l0_pr[i]) / m_t[i]));
//		resMinX = -fabs(sqrt((sqrt(m_t[i] * resMaxY * resMaxY / k_pr[i]) + l0_pr[i]) * (sqrt(m_t[i] * resMaxY * resMaxY / k_pr[i]) + l0_pr[i]) - l0_pr[i] * l0_pr[i]));
//		resMaxX = fabs(sqrt((sqrt(m_t[i] * resMaxY * resMaxY / k_pr[i]) + l0_pr[i]) * (sqrt(m_t[i] * resMaxY * resMaxY / k_pr[i]) + l0_pr[i]) - l0_pr[i] * l0_pr[i]));
//	}
//	minX = resMinX;
//	maxX = resMaxX;
//
//	grap.period.maxX = maxX;
//	grap.period.minX = minX;
//	grap.period.minY = 0.;
//	grap.period.maxY = 2 * M_PI * sqrt(m_t[i] / k_pr[i]);
//}


void CKolebaniyaDlg::OnCancel()
{
	// TODO: добавьте специализированный код или вызов базового класса
	stop = true;
	DeleteCriticalSection(&cs);
	CDialogEx::OnCancel();
}


void CKolebaniyaDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	EnterCriticalSection(&cs);
	if (!graphic.my_func.empty() && !graphic.my_x.empty() && !my_mult.center.empty() && start_exper) Invalidate(FALSE);
	LeaveCriticalSection(&cs);
	CDialogEx::OnTimer(nIDEvent);
}
