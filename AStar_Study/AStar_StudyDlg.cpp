
// AStar_StudyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AStar_Study.h"
#include "AStar_StudyDlg.h"
#include "afxdialogex.h"
#include "AStar.h"
using namespace std;
#include <gdiplus.h>          // 导入gdiplus头文件
#pragma comment(lib,"gdiplus.lib")

using namespace Gdiplus;      // 使用Gdiplus命名空间，避免命名歧议
#ifdef _DEBUG
//#define new DEBUG_NEW
#endif


// CAStar_StudyDlg 对话框

///定义结构///////////////////////////////////////////////////////////////////////


const int Max_X = 1024;//环境的大小
const int Max_Y = 1024;
int Finish_X;//终点
int Finish_Y;
int Start_X;//起点
int Start_Y;	//  上    右     下   左
int Dirs[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};//方向
///////////左上，上，右上，左，右，下左，下，下右///////////////////////////////////////
int MoreDirs[8][2] = {{-1,-1},{0,-1},{1,-1},{-1,0},{1,0},{-1,1},{0,1},{1,1}};
bool visited[Max_X][Max_Y];
bool BackupVistied[Max_X][Max_Y];
priority_queue<Chess> que;
vector<Chess*> deletelist;
Gdiplus::Bitmap *p_bmp;
ULONG_PTR m_pGdiToken;
GdiplusStartupInput m_gdiplusStartupInput;


CAStar_StudyDlg::CAStar_StudyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAStar_StudyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAStar_StudyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAStar_StudyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CAStar_StudyDlg::OnBnClickedButton1)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CAStar_StudyDlg 消息处理程序
CDC memDC;
CDC * pDC = NULL;
BOOL CAStar_StudyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAStar_StudyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAStar_StudyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void InitMapArray()
{
	int Width = p_bmp->GetWidth();
	int Height = p_bmp->GetHeight();

	for (int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Height; j++)
		{
			Color colorOld;
			p_bmp->GetPixel(i,j,&colorOld);	
			int nR = colorOld.GetB();
			int nG = colorOld.GetG();
			int nB = colorOld.GetR();

			if (nR == 255 && nG == 255 && nB == 255) //全白
			{
				visited[i][j] = false;
			}
			else// if(nR == 0 && nG == 0 && nB == 0)	//黑
			{
				visited[i][j] = true;
			}
			/*else
			{
				visited[i][j] = true; //AfxMessageBox(L"有其他颜色的存在!");
			}*/
		}
	}
	memcpy(BackupVistied,visited,sizeof(visited));
}

void CAStar_StudyDlg::OnBnClickedButton1()
{
	GdiplusStartupInput m_gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);

	CString g_szCurPath = L"fff.bmp";
	//AfxMessageBox(g_szCurPath);
	//CString g_szCurPath = szCurPath.Left(L"C:\\fff.bmp");

	p_bmp = new Bitmap(g_szCurPath);
	InitMapArray();
	HBITMAP hb = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),L"fff.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	CStatic * pStatic = (CStatic *)GetDlgItem(IDC_STATIC_MAP);
	pStatic->ModifyStyle(0xF,SS_BITMAP,SS_CENTERIMAGE);
	pStatic->SetBitmap(hb);
}
//判断经过方向运算后是否存在这个棋盘
bool IsExistence(int x,int y)
{
	if (x < 0 || y < 0 || x >= Max_X || y >= Max_Y)
	{
		return false;
	}
	return true;
}

int Heuristic(const Chess temp)
{
	//2种方式的计算,A*的关键就是求H的值
	return (int)sqrt((float)(abs(temp.x - Finish_X) + abs(temp.y - Finish_Y))) ;
	//return  (abs(temp.x - Finish_X) + abs(temp.y - Finish_Y)) * 10;
}

Chess * node = nullptr;

int iSumCount = 0;


void AStar()
{
	//Color color(255,255,0,0);
	while(!que.empty())//当队列不为空
	{
		Chess * temp1 = new Chess;
		deletelist.push_back(temp1);
		*temp1 = que.top();//获取f最大的节点
		que.pop();//出栈
		if (!visited[temp1->x][temp1->y])
		{
			continue;
		}
		visited[temp1->x][temp1->y] = false;//将本路径设置为不通.就是相当于加入到关闭列表
		if (temp1->x == Finish_X && temp1->y == Finish_Y)//是否到达终点了
		{
			que.push(*temp1);
			node = temp1;
			return;
		}
		for (int i = 0;i < 4;i++)
		{
			//4个方向
			Chess * temp2 = new Chess;
			deletelist.push_back(temp2);
			temp2->x = temp1->x + Dirs[i][0];
			temp2->y = temp1->y + Dirs[i][1];
			//获取f最大的节点的任意一个方向.判断经过任意方向后是否存在棋盘，并且路是通的
			if (IsExistence(temp2->x,temp2->y) && visited[temp2->x][temp2->y] /*&& !TraverseVlist(temp2)*/)
			{
				temp2->g = temp1->g + 23;
				temp2->h = Heuristic(*temp2);
				temp2->f = temp2->g + temp2->h;
				temp2->Root = temp1;//记录父节点用来画出完整的路线
				que.push(*temp2);
			}
		}
	}
	//Invalidate();
	//::MessageBox(NULL,_T("无法算出路径"),_T("123"),0);
}
int nColorCount = 0;
typedef struct _OldMap
{
	COLORREF colr;
	CPoint cp;
	CPoint point;
}OldMap;
OldMap StartMapPoint;
OldMap EndMapPoint;
void CAStar_StudyDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	CStatic* pStatic = (CStatic *)GetDlgItem(IDC_STATIC_MAP);
	CRect rect;
	pStatic->GetWindowRect(&rect);
	ScreenToClient(&rect);
	if (rect.PtInRect(point))
	{
		//在图片里面
		if (nColorCount < 2)
		{
			nColorCount++;
			COLORREF colorf = RGB(255,0,0);
			if (nColorCount == 1)
			{
				StartMapPoint.colr = this->GetDC()->GetPixel(point.x,point.y);
				StartMapPoint.cp = point;
				StartMapPoint.point.x = point.x - rect.left;
				StartMapPoint.point.y = point.y - rect.top;
			}
			else if (nColorCount == 2)
			{
				EndMapPoint.colr = this->GetDC()->GetPixel(point.x,point.y);
				EndMapPoint.cp = point;
				EndMapPoint.point.x = point.x - rect.left;
				EndMapPoint.point.y = point.y - rect.top;

				
				memcpy(visited,BackupVistied,sizeof(visited));
				Start_X = StartMapPoint.point.x;
				Start_Y = StartMapPoint.point.y;
				Finish_X = EndMapPoint.point.x;
				Finish_Y = EndMapPoint.point.y;
				Chess temp;
				temp.x = Start_X;
				temp.y = Start_Y;
				temp.g = 0;
				temp.h = Heuristic(temp);
				temp.f = temp.g + temp.h;
				temp.Root = NULL;
				while(!que.empty())
				{
					que.pop();
				}
				que.push(temp);
				DWORD dwStart = GetTickCount();
				AStar();
				DWORD dwEnd = GetTickCount();
				iSumCount = 0;
				CString strTemp;
				strTemp.Format(L"寻找路径成功,耗时:%dms",dwEnd-dwStart);
				::MessageBox(NULL,strTemp,L"提示",MB_OK);

				iSumCount = 0;
				while(!IsBadCodePtr(FARPROC(node)) && node != NULL && node->Root != NULL)
				{
					this->GetDC()->SetPixel(rect.left + node->x,rect.top + node->y,colorf);
					//str.Format(L"x=%d,y=%d",node->x,node->y);
					//AfxMessageBox(str);
					iSumCount++;
					node = node->Root;
				}
			}
			
			this->GetDC()->SetPixel(point.x,point.y,colorf);
		}
		else
		{
			nColorCount = 0;
			this->GetDC()->SetPixel(StartMapPoint.cp.x,StartMapPoint.cp.y,StartMapPoint.colr);
			this->GetDC()->SetPixel(EndMapPoint.cp.x,EndMapPoint.cp.y,EndMapPoint.colr);
			OnLButtonUp(nFlags,point);
		}
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}
