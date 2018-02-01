#ifndef __ASTAR_H__
#define __ASTAR_H__ 1

#include <queue>
#include <vector>
#include <math.h>
#include <gdiplus.h>          // 导入gdiplus头文件
#pragma comment(lib,"gdiplus.lib")
#pragma comment(linker,"/STACK:102400000,1024000")

//using namespace Gdiplus;      // 使用Gdiplus命名空间，避免命名歧议

typedef struct _Point
{
	float X, Y, Z;
	_Point()
	{
		X = Y = Z = 0;
	}
	_Point(float pX, float pY, float pZ)
	{
		X = pX;
		Y = pY;
		Z = pZ;
	}
	_Point(int pX, int pY, int pZ)
	{
		X = (float)pX;
		Y = (float)pY;
		Z = (float)pZ;
	}
	bool operator == (const _Point & p)const
	{
		if (p.X == this->X && p.Y == this->Y && p.Z == this->Z)
		{
			return true;
		}
		return false;
	}
	bool operator != (const _Point & p)const
	{
		if (p.X == this->X && p.Y == this->Y && p.Z == this->Z)
		{
			return false;
		}
		return true;
	}
	bool IsZero()
	{
		if (this->X == 0 && this->Y == 0 && this->Z == 0)
		{
			return true;
		}
		return false;
	}
	void ConvertToInt()
	{
		this->X = (float)(int)this->X;
		this->Y = (float)(int)this->Y;
		this->Z = (float)(int)this->Z;
	}
}Point;

typedef struct _Chess
{
	int x;
	int y;
	int g;
	int h;
	int f;
	int LastDirection;
	struct _Chess * Root;
	bool operator < (const _Chess & c) const
	{
		return f > c.f;
	}
	_Chess()
	{
		x = y = g = h = f = LastDirection = 0;
	}
	float GetX() const
	{
		return (float)x;
	}
	float GetY() const
	{
		return (float)y;
	}
	Point GetPoint()
	{
		Point pt(x,y,0);
		return pt;
	}
}Chess;

#define MAX_PIC_X		1024
#define MAX_PIC_Y		1024
#define COLOR_WHITE		0x1
#define COLOR_BLACK		0x2
#define COLOR_OTHER		0x3
#define MAX_SEARCHPATH_DIS	150

class CAStar
{
public:
	CAStar();
	~CAStar();


	DWORD	GetDistance(const Point* p1, const Point* p2);
	VOID	InitMapArray(LPCWSTR pwszPath);
	BOOL	IsExistence(int x, int y);
	int		Heuristic(const Chess temp);
	Chess*	AStar(Chess* pStart, Chess* pEnd, DWORD dwDisorderColor);
	BOOL	GetRecentlyPoint(IN Chess* pChess, OUT UINT & uRetIndex);
	double	mult(Point& a, Point& b, Point& c);
	bool	intersect(Point& aa, Point& bb, Point& cc, Point& dd);
	BOOL	IsCut(IN Chess* pStart, IN Chess* pEnd);
	BOOL	ConvertPoint(IN Point & MapPoint OUT, Point & GamePoint);
	VOID	WritePointFile(std::vector<Point> & vlst);
	BOOL	BeginAStar(Chess* pStart, Chess* pEnd);
	BOOL	Release();
	
public:
	Point StartPoint;
	Point EndPoint;
	static int Dirs[4][2];
	static int MoreDirs[8][2];
	DWORD visited[MAX_PIC_X][MAX_PIC_Y];
	Point StartPoint_Convert;
	Point GamePoint_Convert;
	std::vector<Point> MainLineList;
	std::vector<Chess*> deletelist;
};


#endif

