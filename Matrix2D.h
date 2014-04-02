#pragma once

class CMatrix2D
{
public:
	bool* matrix;

	int m_sizeX;
	int m_sizeY;

	CMatrix2D(int p_sizeX, int p_sizeY)
	{
		m_sizeX = p_sizeX;
		m_sizeY = p_sizeY;

		matrix = new bool[m_sizeX*m_sizeY];
	}

	bool Get(int x, int y)
	{
		return matrix[(y*m_sizeY)+x];
	}

	void Set(int x, int y, bool v)
	{
		matrix[(y*m_sizeY)+x] = v;
	}

	void Resize(int p_sizeX, int p_sizeY)
	{
		delete [] matrix;

		m_sizeX = p_sizeX;
		m_sizeY = p_sizeY;

		matrix = new bool[m_sizeX*m_sizeY];
	}

	~CMatrix2D(void)
	{
		delete [] matrix;
	}
};

