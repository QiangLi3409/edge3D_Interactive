#pragma once
class CVertex
{
public:
	double x;
	double y;
	double z;

	CVertex(void)
	{
		x = y = z = 0.0f;
	}

	void Set(double p_X, double p_Y, double p_Z)
	{
		x = p_X;
		y = p_Y;
		z = p_Z;
	}

	~CVertex(void)
	{
	}
};

