// FractoriaDlg.h : header file
//

#pragma once
#include "afxcmn.h"


// CFractoriaDlg dialog
class CFractoriaDlg : public CDialog
{
// Construction
public:
	CFractoriaDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FRACTORIA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	HBRUSH m_hBrush;
	BOOL m_bThemeActive;
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CTabCtrl m_cMainTab;
	void OnTcnSelchangeTabcontrol(NMHDR *pNMHDR, LRESULT *pResult);
	void OnChangeTab();

	double Round(double dbVal, int nPlaces /* = 0 */);

	CString RemoveZeros(CString str);

	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonDown(UINT nFlags, CPoint point);
	double m_dCoordX;
	double m_dCoordY;
	double m_dCoordXRounded;
	double m_dCoordYRounded;

	afx_msg void OnBnClickedCancel();
	
	afx_msg void OnHelpAbout();

	afx_msg void OnCbnSelchangeFractalTypeCombo();

	CEdit* pEdit10005;
	afx_msg void OnEnChangeRealAxis();
	double m_dXAxis;
	CEdit* pEdit10007;
	afx_msg void OnEnChangeImagAxis();
	double m_dYAxis;
	CEdit* pEdit10009;
	afx_msg void OnEnChangeZoomRatio();
	double m_dZoomRatio;
	CEdit* pEdit10011;
	afx_msg void OnEnChangeRotation();
	double m_dRotation;
	CEdit* pEdit10013;
	afx_msg void OnEnChangeIteration();
	int m_iIteration;
	afx_msg void OnEnChangeBailout();
	double m_dBailout;

	afx_msg void OnEnChangeControl1();
	double m_dControl1;
	afx_msg void OnEnChangeControl2();
	double m_dControl2;
	afx_msg void OnEnChangeControl3();
	double m_dControl3;
	afx_msg void OnEnChangeControl4();
	double m_dControl4;
	afx_msg void OnEnChangeControl5();
	double m_dControl5;
	afx_msg void OnEnChangeControl6();
	double m_dControl6;

	afx_msg void OnEnChangeOutRedAdder();
	int m_iOutRedAdder;
	afx_msg void OnEnChangeOutGreenAdder();
	int m_iOutGreenAdder;
	afx_msg void OnEnChangeOutBlueAdder();
	int m_iOutBlueAdder;
	afx_msg void OnEnChangeOutRedMult();
	double m_dOutRedMult;
	afx_msg void OnEnChangeOutGreenMult();
	double m_dOutGreenMult;
	afx_msg void OnEnChangeOutBlueMult();
	double m_dOutBlueMult;

	afx_msg void OnEnChangeInRedAdder();
	int m_iInRedAdder;
	afx_msg void OnEnChangeInGreenAdder();
	int m_iInGreenAdder;
	afx_msg void OnEnChangeInBlueAdder();
	int m_iInBlueAdder;
	afx_msg void OnEnChangeInRedMult();
	double m_dInRedMult;
	afx_msg void OnEnChangeInGreenMult();
	double m_dInGreenMult;
	afx_msg void OnEnChangeInBlueMult();
	double m_dInBlueMult;

	BOOL PreTranslateMessage(MSG* pMsg);
	void OnPreview();

	void OnBnClickedRender();

	afx_msg void OnBnClickedOutsideColorPalette();
	afx_msg void OnBnClickedInsideColorPalette();

	afx_msg void OnFileSaveImage();
	BOOL WndToBmp(CDC *pDC, CString szFile);
	HANDLE DDBToDIB(CBitmap &bitmap, DWORD dwCompression, CPalette *pPal);

	afx_msg void OnPropertiesAnimation();

	CString m_sFractalTypeStr;

	afx_msg void OnBnClickedAnimation();
//	void OnAnimation();

	afx_msg void OnBnClickedSuspendRender();

//	void OnAnimationRender();


	void OnRButtonDown(UINT nFlags, CPoint point);
	void OnBnClickedSwitchModeFractal();

	afx_msg void OnRenderingColoring();

	void EnableAllMainTabControls();
};
