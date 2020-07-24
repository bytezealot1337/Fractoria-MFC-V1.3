#pragma once
#include "afxcmn.h"


// CNRootColoringProperties dialog

class CNRootColoringProperties : public CDialog
{
	DECLARE_DYNAMIC(CNRootColoringProperties)

public:
	CNRootColoringProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNRootColoringProperties();

// Dialog Data
	enum { IDD = IDD_NROOTCOLORINGPROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	HBRUSH m_hBrush;
	BOOL m_bThemeActive;
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CString RemoveZeros(CString str);

	CTabCtrl m_cNRootColoringTab;

	void InitTab();
	void InitAttributesTab();

	void OnEnChangeFirstRootRed();
	CString m_sFirstRootRed;

	void OnEnChangeFirstRootGreen();
	CString m_sFirstRootGreen;

	void OnEnChangeFirstRootBlue();
	CString m_sFirstRootBlue;

	void OnEnChangeSecondRootRed();
	CString m_sSecondRootRed;

	void OnEnChangeSecondRootGreen();
	CString m_sSecondRootGreen;

	void OnEnChangeSecondRootBlue();
	CString m_sSecondRootBlue;

	void OnEnChangeThirdRootRed();
	CString m_sThirdRootRed;

	void OnEnChangeThirdRootGreen();
	CString m_sThirdRootGreen;

	void OnEnChangeThirdRootBlue();
	CString m_sThirdRootBlue;

};
