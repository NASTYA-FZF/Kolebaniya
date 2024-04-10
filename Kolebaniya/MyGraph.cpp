// MyGraph.cpp: файл реализации
//

#include "pch.h"
#include "MyGraph.h"
using namespace Gdiplus;

// MyGraph

IMPLEMENT_DYNAMIC(MyGraph, CStatic)

MyGraph::MyGraph()
{
	GdiplusStartupInput input;
	Status s;
	s = GdiplusStartup(&token, &input, NULL);
	on_off = true;
	maxY = 0.;
	minY = 0.;
	maxX = 0.;
	minX = 0.;
	cur = 5;
	if (s != Ok)
	{
		MessageBox(L"Не удалось нарисовать", L"Ошибка", NULL);
	}
}

MyGraph::~MyGraph()
{
	GdiplusShutdown(token);
}


BEGIN_MESSAGE_MAP(MyGraph, CStatic)
END_MESSAGE_MAP()



// Обработчики сообщений MyGraph




void MyGraph::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  Добавьте исходный код для отображения указанного элемента
	if (on_off)
	{
		on_off = false;
	}
	else
	{
		Graphics wnd(lpDrawItemStruct->hDC);
		Bitmap buffer(lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, &wnd);
		Graphics draw_in_buffer(&buffer);
		draw_in_buffer.SetSmoothingMode(SmoothingModeAntiAlias);

		otstup = 35.;
		RectF rect_graph(
			cur * otstup / 2,
			cur * otstup / 2,
			lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left - cur * otstup,
			lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top - cur * otstup
		);

		double Xscale = rect_graph.Width / (maxX - minX);
		double Yscale = rect_graph.Height / (maxY - minY);

		double help = minX;
		Matrix matr;
		if(minX < 0. && maxX > 0.) matr.Translate((lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left) * fabs(minX) / (maxX - minX) + otstup, (lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top) * fabs(maxY) / (maxY - minY) - otstup);
		else
		{
			matr.Translate(rect_graph.X, otstup + rect_graph.Height);
			Xscale = rect_graph.Width / (maxX);
			help -= 0.05 * (maxX - minX);
			minX = 0.;
		}
		matr.Scale(Xscale, -Yscale);

		PointF* point = new PointF[4];


		draw_in_buffer.Clear(Color(255, 255, 255, 255));
		Pen pen_ortho(Color(255, 0, 0, 0));

		point[0] = PointF(minX, minY);
		point[1] = PointF(minX, maxY);
		point[2] = PointF(maxX, minY);
		matr.TransformPoints(point, 3);
		draw_in_buffer.DrawLine(&pen_ortho, point[0], point[1]);
		draw_in_buffer.DrawLine(&pen_ortho, point[0], point[2]);

		Pen pen_network(Color(255, 220, 220, 220));

		int num = 12;
		double step_ver_network = (maxX - minX) / num;
		double step_hor_network = (maxY - minY) / num;

		double x = minX;
		for (int i = 0; i < num; i++)
		{
			point[0] = PointF(minX + (i + 1) * step_ver_network, minY);
			point[1] = PointF(minX + (i + 1) * step_ver_network, maxY);
			point[2] = PointF(minX, minY + (i + 1) * step_hor_network);
			point[3] = PointF(maxX, minY + (i + 1) * step_hor_network);
			matr.TransformPoints(point, 4);
			draw_in_buffer.DrawLine(&pen_network, point[0], point[1]);
			draw_in_buffer.DrawLine(&pen_network, point[2], point[3]);
		}

		FontFamily my_font_family(L"Arial");
		Gdiplus::Font my_font(&my_font_family, 14, FontStyleRegular, UnitPixel);
		SolidBrush brush_font(Color(255, 0, 0, 0));

		x = minX;
		double y = minY;
		wchar_t podpis_ortho[10];
		for (int i = 0; i < num; i++)
		{
			point[0] = PointF(x - 0.05 * fabs(minX), minY - 0.1 * fabs(minY));
			point[1] = PointF(help - 0.1 * fabs(help), y + 0.05 * fabs(maxY));
			matr.TransformPoints(point, 2);
			swprintf_s(podpis_ortho, L"%.2f", x);
			draw_in_buffer.DrawString(podpis_ortho, -1, &my_font, point[0], &brush_font);
			x += step_ver_network;

			swprintf_s(podpis_ortho, L"%.2f", y);
			draw_in_buffer.DrawString(podpis_ortho, -1, &my_font, point[1], &brush_font);
			y += step_hor_network;
		}
		
		Gdiplus::Font my_font_main(&my_font_family, 20, FontStyleRegular, UnitPixel);
		SolidBrush brush_font_main(Color(255, 0, 0, 0));
		point[0] = PointF(1.2 * help, 1.05 * maxY);
		point[1] = PointF(maxX, minY);
		point[2] = PointF(help, 1.1 * maxY);
		matr.TransformPoints(point, 3);
		draw_in_buffer.DrawString(osY, -1, &my_font, point[0], &brush_font);
		draw_in_buffer.DrawString(osX, -1, &my_font, point[1], &brush_font);
		draw_in_buffer.DrawString(nameGraph, -1, &my_font_main, point[2], &brush_font_main);

		int iter = 1;
		for (iter = 1; iter < my_func[0].size() - 1; iter++)
		{
			for (int j = 0; j < my_func.size(); j++)
			{
				Pen pen_graph({ 255, Red[j], Green[j], Blue[j] });
				point[0] = PointF((my_x[j][iter - 1]), my_func[j][iter - 1]);
				point[1] = PointF(my_x[j][iter + 1], my_func[j][iter + 1]);
				matr.TransformPoints(point, 2);
				draw_in_buffer.DrawLine(&pen_graph, point[0], point[1]);
			}
			//iter++;
		}

		

		delete[] point;
		wnd.DrawImage(&buffer, 0, 0, 0, 0, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, UnitPixel);
	}
}
