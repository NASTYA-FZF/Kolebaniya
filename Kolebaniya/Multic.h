#pragma once
#include <gdiplus.h>
#include <vector>

// Multic

class Multic : public CStatic
{
	DECLARE_DYNAMIC(Multic)

public:
	Multic();
	virtual ~Multic();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	ULONG_PTR token;
	bool on_off;
	std::vector<double> center;
	double minX, minY;
	double maxX, maxY;
	std::vector<double> l0;
	std::vector<BYTE> Red, Green, Blue;
};


