#pragma once
#include "afxcmn.h"


// CAnimationProperties dialog

class CAnimationProperties : public CDialog
{
	DECLARE_DYNAMIC(CAnimationProperties)

public:
	CAnimationProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAnimationProperties();

// Dialog Data
	enum { IDD = IDD_ANIMATIONPROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


public:

	HBRUSH m_hBrush;
	BOOL m_bThemeActive;
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CString RemoveZeros(CString str);

	CTabCtrl m_cAnimationTab;

	void InitTab();
	void InitGeometryTab();
	void InitFormulaTab();
	void InitOutsideTab();
	void InitInsideTab();
	void InitFormulaControls();

	void OnTcnSelchangeTabcontrol(NMHDR *pNMHDR, LRESULT *pResult);
	void OnChangeTab();

	BOOL CAnimationProperties::PreTranslateMessage(MSG* pMsg);

	void OnEnChangeTotalFrame();
	CString m_sTotalFrame;

	CComboBox* pComboBox20005;
	void OnCbnSelchangeTriggerPointCombo();
	int m_iTriggerPointCombo;

	CEdit* pEdit20007;
	void OnEnChangeFrame();
	CString m_sFrame;

	void OnEnChangeEndXAxis();
	CString m_sEndXAxis;
	void OnEnChangeEndYAxis();
	CString m_sEndYAxis;

	CComboBox* pComboBox20013;
	void OnCbnSelchangeTranslationFunctionCombo();
	int m_iTranslationFunctionCombo;

	void OnEnChangeEndZoomRatio();
	CString m_sEndZoomRatio;

	CComboBox* pComboBox20017;
	void OnCbnSelchangeZoomRatioFunctionCombo();
	int m_iZoomRatioFunctionCombo;

	void OnEnChangeEndRotation();
	CString m_sEndRotation;

	CComboBox* pComboBox20021;
	void OnCbnSelchangeRotationFunctionCombo();
	int m_iRotationFunctionCombo;

	void CAnimationProperties::OnEnChangeEndIteration();
	CString m_sEndIteration;

	CComboBox* pComboBox20025;
	void OnCbnSelchangeIterationFunctionCombo();
	int m_iIterationFunctionCombo;

	void OnChangeAnimationControl1();
	CString m_sAnimationControl1;
	void OnCbnSelchangeAnimationControl1Combo();
	int m_iAnimationControl1Combo;

	void OnChangeAnimationControl2();
	CString m_sAnimationControl2;
	void OnCbnSelchangeAnimationControl2Combo();
	int m_iAnimationControl2Combo;

	void OnChangeAnimationControl3();
	CString m_sAnimationControl3;
	void OnCbnSelchangeAnimationControl3Combo();
	int m_iAnimationControl3Combo;

	void OnChangeAnimationControl4();
	CString m_sAnimationControl4;
	void OnCbnSelchangeAnimationControl4Combo();
	int m_iAnimationControl4Combo;

	void OnChangeAnimationControl5();
	CString m_sAnimationControl5;
	void OnCbnSelchangeAnimationControl5Combo();
	int m_iAnimationControl5Combo;

	void OnChangeAnimationControl6();
	CString m_sAnimationControl6;
	void OnCbnSelchangeAnimationControl6Combo();
	int m_iAnimationControl6Combo;


	void OnChangeAnimationOutsideRedAdder();
	CString m_sAnimationOutsideRedAdder;
	void OnCbnSelchangeAnimationOutsideRedAdderCombo();
	int m_iAnimationOutsideRedAdderCombo;

	void OnChangeAnimationOutsideGreenAdder();
	CString m_sAnimationOutsideGreenAdder;
	void OnCbnSelchangeAnimationOutsideGreenAdderCombo();
	int m_iAnimationOutsideGreenAdderCombo;

	void OnChangeAnimationOutsideBlueAdder();
	CString m_sAnimationOutsideBlueAdder;
	void OnCbnSelchangeAnimationOutsideBlueAdderCombo();
	int m_iAnimationOutsideBlueAdderCombo;

	void OnChangeAnimationOutsideRedMult();
	CString m_sAnimationOutsideRedMult;
	void OnCbnSelchangeAnimationOutsideRedMultCombo();
	int m_iAnimationOutsideRedMultCombo;

	void OnChangeAnimationOutsideGreenMult();
	CString m_sAnimationOutsideGreenMult;
	void OnCbnSelchangeAnimationOutsideGreenMultCombo();
	int m_iAnimationOutsideGreenMultCombo;

	void OnChangeAnimationOutsideBlueMult();
	CString m_sAnimationOutsideBlueMult;
	void OnCbnSelchangeAnimationOutsideBlueMultCombo();
	int m_iAnimationOutsideBlueMultCombo;


	void OnChangeAnimationInsideRedAdder();
	CString m_sAnimationInsideRedAdder;
	void OnCbnSelchangeAnimationInsideRedAdderCombo();
	int m_iAnimationInsideRedAdderCombo;

	void OnChangeAnimationInsideGreenAdder();
	CString m_sAnimationInsideGreenAdder;
	void OnCbnSelchangeAnimationInsideGreenAdderCombo();
	int m_iAnimationInsideGreenAdderCombo;

	void OnChangeAnimationInsideBlueAdder();
	CString m_sAnimationInsideBlueAdder;
	void OnCbnSelchangeAnimationInsideBlueAdderCombo();
	int m_iAnimationInsideBlueAdderCombo;

	void OnChangeAnimationInsideRedMult();
	CString m_sAnimationInsideRedMult;
	void OnCbnSelchangeAnimationInsideRedMultCombo();
	int m_iAnimationInsideRedMultCombo;

	void OnChangeAnimationInsideGreenMult();
	CString m_sAnimationInsideGreenMult;
	void OnCbnSelchangeAnimationInsideGreenMultCombo();
	int m_iAnimationInsideGreenMultCombo;

	void OnChangeAnimationInsideBlueMult();
	CString m_sAnimationInsideBlueMult;
	void OnCbnSelchangeAnimationInsideBlueMultCombo();
	int m_iAnimationInsideBlueMultCombo;

	afx_msg void OnBnClickedResetAnimationControls();
};
