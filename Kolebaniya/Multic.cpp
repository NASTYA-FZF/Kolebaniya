// Multic.cpp: файл реализации
//

#include "pch.h"
#define _USE_MATH_DEFINES
#include "Kolebaniya.h"
#include "Multic.h"
#include <math.h>
using namespace Gdiplus;

// Multic

IMPLEMENT_DYNAMIC(Multic, CStatic)

Multic::Multic()
{
	GdiplusStartupInput input;
	Status s;
	s = GdiplusStartup(&token, &input, NULL);
	if (s != Ok)
	{
		MessageBox(L"Не удалось нарисовать", L"Ошибка", NULL);
	}
	on_off = true;
}

Multic::~Multic()
{
	GdiplusShutdown(token);
}


BEGIN_MESSAGE_MAP(Multic, CStatic)
END_MESSAGE_MAP()



// Обработчики сообщений Multic




void Multic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  Добавьте исходный код для отображения указанного элемента
	if (on_off) on_off = false;
	else
	{
		Graphics wnd(lpDrawItemStruct->hDC);
		Bitmap buffer(lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, &wnd);
		Graphics draw_in_buffer(&buffer);

		//draw_in_buffer.SetSmoothingMode(SmoothingModeAntiAlias);
		draw_in_buffer.Clear(Color(255, 255, 255, 255));

		double otstup = 100.;
		RectF rect_mult(
			otstup,
			0.,
			lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left,
			lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top - otstup
		);

		double size_teloX = 0.1 * (maxX - minX);
		minY = 0.;
		double size_teloY = 0.1 * (maxY - minY);

		double Xscale = rect_mult.Width / (maxX - minX);
		double Yscale = rect_mult.Height / (maxY - minY);

		Matrix matr;
		matr.Translate(rect_mult.Width / 2, 0.);
		matr.Scale(Xscale, Yscale);

		PointF* point = new PointF[2];

		for (int j = 0; j < center.size(); j++)
		{
			Pen pr({ 255, Red[j], Green[j], Blue[j]});
			double otklX = 0.05 * (maxX - minX), otklY = 0.05 * (maxY - minY);
			int deleniya = 25;
			double pt = center[j] / deleniya;
			RectF ell;

			for (int i = 0; i < deleniya; i++)
			{
				/*point[0] = PointF(pt - center[j] / (2 * deleniya), (pt - center[j] / deleniya) * l0[j] / center[j]);
				point[1] = PointF(pt - center[j] / (2 * deleniya) + otklX, (pt - center[j] / deleniya) * l0[j] / center[j] + otklY);
				matr.TransformPoints(point, 2);
				ell = RectF(point[0].X, point[0].Y, point[1].X - point[0].X, point[1].Y - point[0].Y);
				draw_in_buffer.DrawEllipse(&pr, ell);*/
				point[0] = PointF(pt - center[j] / deleniya, (pt - center[j] / deleniya) * l0[j] / center[j]);
				point[1] = PointF(pt, pt * l0[j] / center[j]);
				matr.TransformPoints(point, 2);
				draw_in_buffer.DrawLine(&pr, point[0], point[1]);
				pt += center[j] / deleniya;
			}

			point[0] = PointF(center[j] - size_teloX / 2, l0[j]);
			point[1] = PointF(center[j] - size_teloX / 2 + size_teloX, l0[j] + size_teloY);
			matr.TransformPoints(point, 2);
			//draw_in_buffer.DrawRectangle(&pr, RectF(point[0].X, point[0].Y, point[1].X - point[0].X, point[1].Y - point[0].Y));
			SolidBrush* brush_telo = new SolidBrush({ 255, Red[j], Green[j], Blue[j]});
			draw_in_buffer.FillRectangle(brush_telo, RectF(point[0].X, point[0].Y, point[1].X - point[0].X, point[1].Y - point[0].Y));
			maxY += size_teloY;
			point[0] = PointF(minX, l0[j] + size_teloY);
			point[1] = PointF(maxX, l0[j] + size_teloY);
			matr.TransformPoints(point, 2);
			draw_in_buffer.DrawLine(&pr, point[0], point[1]);
			maxY -= size_teloY;
		}

		FontFamily my_font_family(L"Arial");
		Gdiplus::Font my_font(&my_font_family, 14, FontStyleRegular, UnitPixel);
		SolidBrush brush_font(Color(255, 0, 0, 0));

		wchar_t podpis_ortho[10];
		int number = 12;
		double step_x = (maxX - minX) / number, x = minX;
		for (int i = 0; i < 12; i++)
		{
			point[0] = PointF(x, maxY + size_teloY + 0.05 * fabs(maxY + size_teloY));
			matr.TransformPoints(point, 1);
			swprintf_s(podpis_ortho, L"%.1f", x);
			draw_in_buffer.DrawString(podpis_ortho, -1, &my_font, point[0], &brush_font);
			x += step_x;
		}

		delete[] point;
		wnd.DrawImage(&buffer, 0, 0, 0, 0, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, UnitPixel);
	}
}
