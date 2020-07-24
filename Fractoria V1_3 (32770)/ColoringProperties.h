#pragma once
#include "afxcmn.h"


// CColoringProperties dialog

class CColoringProperties : public CDialog
{
	DECLARE_DYNAMIC(CColoringProperties)

public:
	CColoringProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CColoringProperties();

// Dialog Data
	enum { IDD = IDD_COLORINGPROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	HBRUSH m_hBrush;
	BOOL m_bThemeActive;
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CString RemoveZeros(CString str);

	CTabCtrl m_cColoringTab;

	void InitTab();
	void InitOutsideTab();
	void InitInsideTab();

	void OnTcnSelchangeTabcontrol(NMHDR *pNMHDR, LRESULT *pResult);
	void OnChangeTab();

	void OnCbnSelchangeColoringAlgorithmCombo();
	void OnCbnSelchange2ColorGradientPropertiesCombo();
	void OnCbnSelchangeSmoothGradientPropertiesCombo();

	void OnEnChangeSmoothRedMultiplier();
	CString sSmoothRedMultiplier;
	void OnEnChangeSmoothGreenMultiplier();
	CString sSmoothGreenMultiplier;
	void OnEnChangeSmoothBlueMultiplier();
	CString sSmoothBlueMultiplier;

	void OnBnClickedMainColorRed();
	void OnBnClickedMainColorGreen();
	void OnBnClickedMainColorBlue();
};
