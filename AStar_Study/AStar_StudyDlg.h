
// AStar_StudyDlg.h : 头文件
//

#pragma once


// CAStar_StudyDlg 对话框
class CAStar_StudyDlg : public CDialogEx
{
// 构造
public:
	CAStar_StudyDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ASTAR_STUDY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnStnClickedStaticMap();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
