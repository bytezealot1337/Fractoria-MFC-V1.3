// ColoringProperties.cpp : implementation file
//

#include "stdafx.h"
#include "Fractoria.h"
#include "ColoringProperties.h"


// CColoringProperties dialog

IMPLEMENT_DYNAMIC(CColoringProperties, CDialog)

CColoringProperties::CColoringProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CColoringProperties::IDD, pParent)
{
	sSmoothRedMultiplier.Format(_T("%lf"), theApp.m_dSmoothRedValue);
	sSmoothRedMultiplier = RemoveZeros(sSmoothRedMultiplier);
	
	sSmoothGreenMultiplier.Format(_T("%lf"), theApp.m_dSmoothGreenValue);
	sSmoothGreenMultiplier = RemoveZeros(sSmoothGreenMultiplier);
	
	sSmoothBlueMultiplier.Format(_T("%lf"), theApp.m_dSmoothBlueValue);
	sSmoothBlueMultiplier = RemoveZeros(sSmoothBlueMultiplier);
}

CColoringProperties::~CColoringProperties()
{
}

void CColoringProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLORINGTAB, m_cColoringTab);

	InitTab();
	InitOutsideTab();
	InitInsideTab();

	OnCbnSelchangeColoringAlgorithmCombo();
}


BEGIN_MESSAGE_MAP(CColoringProperties, CDialog)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TCN_SELCHANGE, IDC_COLORINGTAB, &CColoringProperties::OnTcnSelchangeTabcontrol)
	ON_CBN_SELCHANGE(30003, &CColoringProperties::OnCbnSelchangeColoringAlgorithmCombo)
	ON_CBN_SELCHANGE(30005, &CColoringProperties::OnCbnSelchange2ColorGradientPropertiesCombo)
	ON_CBN_SELCHANGE(30006, &CColoringProperties::OnCbnSelchangeSmoothGradientPropertiesCombo)
	ON_EN_CHANGE(30009, &CColoringProperties::OnEnChangeSmoothRedMultiplier)
	ON_EN_CHANGE(30011, &CColoringProperties::OnEnChangeSmoothGreenMultiplier)
	ON_EN_CHANGE(30013, &CColoringProperties::OnEnChangeSmoothBlueMultiplier)
	ON_BN_CLICKED(30015, &CColoringProperties::OnBnClickedMainColorRed)
	ON_BN_CLICKED(30016, &CColoringProperties::OnBnClickedMainColorGreen)
	ON_BN_CLICKED(30017, &CColoringProperties::OnBnClickedMainColorBlue)
END_MESSAGE_MAP()


// CColoringProperties message handlers

CString CColoringProperties::RemoveZeros(CString str)
{
	int count = 0, count2 = 0;
	int length;


	length = str.GetLength();

	while(count < length)
	{
		if(str.GetAt(count) == '.')
		{
			count2 = 0;

			while( (str[length-1-count2] == '0') && (str[length-2-count2] != '.') )
			{
				str.SetAt(length-1-count2, NULL);

				count2++;
			}
		}

		count++;
	}

	return str;
}

HBRUSH CColoringProperties::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if(m_bThemeActive && nCtlColor == CTLCOLOR_STATIC)
	{
		CRect rc;
		// Set the background mode to transparent
		::SetBkMode(pDC->m_hDC,TRANSPARENT);
		return m_hBrush;
		//return ::CreateSolidBrush(RGB(255,0,0));
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CColoringProperties::InitTab()
{
	m_cColoringTab.InsertItem(0, _T("Outside"));
//	m_cColoringTab.InsertItem(1, _T("Inside"));
}

void CColoringProperties::InitOutsideTab()
{
	// Tab 1 - Outside Coloring

	CButton* pGroupBox30001 = new CButton;
	pGroupBox30001->Create(_T("Properties"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
		CRect(25,40,300,120) ,this, 30001);
	pGroupBox30001->SetFont(GetFont());
	pGroupBox30001->ShowWindow(SW_SHOW);


	CStatic* pStatic30002 = new CStatic;
	pStatic30002->Create(_T("Coloring Algorithm :"), WS_CHILD|WS_VISIBLE,
		CRect(35,60,148,80), this, 30002);
	pStatic30002->SetFont(GetFont());
	pStatic30002->ShowWindow(SW_SHOW);

	// Coloring Algorithm ComboBox
	theApp.pComboBox30003 = new CComboBox;
	theApp.pComboBox30003->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,57,290,77), this, 30003);
	theApp.pComboBox30003->SetFont(GetFont());
	theApp.pComboBox30003->ShowWindow(SW_SHOW);
	
	theApp.pComboBox30003->AddString(_T("Bailout Linear"));
	theApp.pComboBox30003->AddString(_T("2-Color Gradient"));
	theApp.pComboBox30003->AddString(_T("Smooth Normalization"));

	theApp.pComboBox30003->SetCurSel(theApp.m_iColoringAlgorithmCurSel);

	theApp.pStatic30004 = new CStatic;
	theApp.pStatic30004->Create(_T("Algorithm Attributes :"), WS_CHILD|WS_VISIBLE,
		CRect(35,90,148,110), this, 30004);
	theApp.pStatic30004->SetFont(GetFont());
	theApp.pStatic30004->ShowWindow(SW_SHOW);

	// Coloring Algorithm ComboBox
	theApp.pComboBox30005 = new CComboBox;
	theApp.pComboBox30005->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,87,290,107), this, 30005);
	theApp.pComboBox30005->SetFont(GetFont());
	theApp.pComboBox30005->ShowWindow(SW_SHOW);
	
	theApp.pComboBox30005->AddString(_T("Black to White"));
	theApp.pComboBox30005->AddString(_T("Red to White"));
	theApp.pComboBox30005->AddString(_T("Green to White"));
	theApp.pComboBox30005->AddString(_T("Blue to White"));
	theApp.pComboBox30005->AddString(_T("Yellow to White"));
	theApp.pComboBox30005->AddString(_T("Purple to White"));
	theApp.pComboBox30005->AddString(_T("Cyan to White"));
	theApp.pComboBox30005->AddString(_T("Red to Yellow"));
	theApp.pComboBox30005->AddString(_T("Red to Purple"));
	theApp.pComboBox30005->AddString(_T("Green to Yellow"));
	theApp.pComboBox30005->AddString(_T("Green to Cyan"));
	theApp.pComboBox30005->AddString(_T("Blue to Purple"));
	theApp.pComboBox30005->AddString(_T("Blue to Cyan"));

	theApp.pComboBox30005->SetCurSel(theApp.m_i2ColorGradientCurSel);


	theApp.pComboBox30006 = new CComboBox;
	theApp.pComboBox30006->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,87,290,107), this, 30006);
	theApp.pComboBox30006->SetFont(GetFont());
	theApp.pComboBox30006->ShowWindow(SW_HIDE);
	
	theApp.pComboBox30006->AddString(_T("Regular"));
	theApp.pComboBox30006->AddString(_T("Reverse"));

	theApp.pComboBox30006->SetCurSel(theApp.m_iSmoothGradientCurSel);


	CButton* pGroupBox30007 = new CButton;
	pGroupBox30007->Create(_T("Values"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
		CRect(25,120,300,220) ,this, 30007);
	pGroupBox30007->SetFont(GetFont());
	pGroupBox30007->ShowWindow(SW_SHOW);


	theApp.pStatic30008 = new CStatic;
	theApp.pStatic30008->Create(_T("Red Multiplier :"), WS_CHILD|WS_VISIBLE,
		CRect(35,140,148,160), this, 30008);
	theApp.pStatic30008->SetFont(GetFont());
	theApp.pStatic30008->ShowWindow(SW_SHOW);

	theApp.pEdit30009 = new CEdit;
	theApp.pEdit30009->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,137,290,157), this, 30009);
	theApp.pEdit30009->SetFont(GetFont());
	theApp.pEdit30009->ShowWindow(SW_SHOW);
	theApp.pEdit30009->SetWindowText(sSmoothRedMultiplier);

	theApp.pStatic30010 = new CStatic;
	theApp.pStatic30010->Create(_T("Green Multiplier :"), WS_CHILD|WS_VISIBLE,
		CRect(35,165,148,185), this, 30010);
	theApp.pStatic30010->SetFont(GetFont());
	theApp.pStatic30010->ShowWindow(SW_SHOW);

	theApp.pEdit30011 = new CEdit;
	theApp.pEdit30011->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,162,290,182), this, 30011);
	theApp.pEdit30011->SetFont(GetFont());
	theApp.pEdit30011->ShowWindow(SW_SHOW);
	theApp.pEdit30011->SetWindowText(sSmoothGreenMultiplier);

	theApp.pStatic30012 = new CStatic;
	theApp.pStatic30012->Create(_T("Blue Multiplier :"), WS_CHILD|WS_VISIBLE,
		CRect(35,190,148,210), this, 30012);
	theApp.pStatic30012->SetFont(GetFont());
	theApp.pStatic30012->ShowWindow(SW_SHOW);

	theApp.pEdit30013 = new CEdit;
	theApp.pEdit30013->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,187,290,207), this, 30013);
	theApp.pEdit30013->SetFont(GetFont());
	theApp.pEdit30013->ShowWindow(SW_SHOW);
	theApp.pEdit30013->SetWindowText(sSmoothBlueMultiplier);


	CButton* pGroupBox30014 = new CButton;
	pGroupBox30014->Create(_T("Main Color"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
		CRect(25,220,300,270) ,this, 30014);
	pGroupBox30014->SetFont(GetFont());
	pGroupBox30014->ShowWindow(SW_SHOW);


	theApp.pButton30015 = new CButton;
	theApp.pButton30015->Create(_T(" Red"),WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|WS_TABSTOP,
		CRect(45,240,100,260) ,this, 30015);
	theApp.pButton30015->SetFont(GetFont());
	theApp.pButton30015->ShowWindow(SW_SHOW);
	theApp.pButton30015->SetCheck(theApp.m_bMainColoringRed);

	theApp.pButton30016 = new CButton;
	theApp.pButton30016->Create(_T(" Green"),WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|WS_TABSTOP,
		CRect(130,240,200,260) ,this, 30016);
	theApp.pButton30016->SetFont(GetFont());
	theApp.pButton30016->ShowWindow(SW_SHOW);
	theApp.pButton30016->SetCheck(theApp.m_bMainColoringGreen);

	theApp.pButton30017 = new CButton;
	theApp.pButton30017->Create(_T(" Blue"),WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|WS_TABSTOP,
		CRect(230,240,275,260) ,this, 30017);
	theApp.pButton30017->SetFont(GetFont());
	theApp.pButton30017->ShowWindow(SW_SHOW);
	theApp.pButton30017->SetCheck(theApp.m_bMainColoringBlue);

}

void CColoringProperties::InitInsideTab()
{
	// Tab 2 - Inside Coloring

	CButton* pGroupBox31001 = new CButton;
	pGroupBox31001->Create(_T("Properties"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
		CRect(25,40,300,270) ,this, 31001);
	pGroupBox31001->SetFont(GetFont());
	pGroupBox31001->ShowWindow(SW_HIDE);

}

void CColoringProperties::OnTcnSelchangeTabcontrol(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnChangeTab();

	OnCbnSelchangeColoringAlgorithmCombo();
}

void CColoringProperties::OnChangeTab()
{
	if(m_cColoringTab.GetCurFocus() == 0)
	{
		for(int nIDCount = 30001; nIDCount <= 30017; nIDCount++)
		{
			GetDlgItem(nIDCount)->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		for(int nIDCount = 30001; nIDCount <= 30017; nIDCount++)
		{
			GetDlgItem(nIDCount)->ShowWindow(SW_HIDE);
		}
	}

	if(m_cColoringTab.GetCurFocus() == 1)
	{
		for(int nIDCount = 31001; nIDCount <= 31001; nIDCount++)
		{
			GetDlgItem(nIDCount)->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		for(int nIDCount = 31001; nIDCount <= 31001; nIDCount++)
		{
			GetDlgItem(nIDCount)->ShowWindow(SW_HIDE);
		}
	}
}

void CColoringProperties::OnCbnSelchangeColoringAlgorithmCombo()
{
	theApp.m_iColoringAlgorithmCurSel = theApp.pComboBox30003->GetCurSel();

	if(m_cColoringTab.GetCurFocus() == 0)
	{
		if(theApp.m_iColoringAlgorithmCurSel == 0)
		{
			theApp.pStatic12008->EnableWindow(TRUE);
			theApp.pEdit12009->EnableWindow(TRUE);
			theApp.pStatic12010->EnableWindow(TRUE);
			theApp.pEdit12011->EnableWindow(TRUE);
			theApp.pStatic12012->EnableWindow(TRUE);
			theApp.pEdit12013->EnableWindow(TRUE);

			theApp.pStatic30004->ShowWindow(TRUE);
			theApp.pStatic30004->EnableWindow(FALSE);

			theApp.pComboBox30005->ShowWindow(TRUE);
			theApp.pComboBox30005->EnableWindow(FALSE);

			theApp.pComboBox30006->ShowWindow(FALSE);
			theApp.pComboBox30006->EnableWindow(FALSE);

			theApp.pStatic30008->EnableWindow(FALSE);
			theApp.pEdit30009->EnableWindow(FALSE);
			theApp.pStatic30010->EnableWindow(FALSE);
			theApp.pEdit30011->EnableWindow(FALSE);
			theApp.pStatic30012->EnableWindow(FALSE);
			theApp.pEdit30013->EnableWindow(FALSE);

			theApp.m_iIteration = 25;
			theApp.pEdit10013.SetWindowText(_T("25"));

			theApp.m_iOutRedAdder = 255;
			theApp.pEdit12003->SetWindowText(_T("255"));

			theApp.m_iOutGreenAdder = 255;
			theApp.pEdit12005->SetWindowText(_T("255"));

			theApp.m_iOutBlueAdder = 255;
			theApp.pEdit12007->SetWindowText(_T("255"));

		}
		else if(theApp.m_iColoringAlgorithmCurSel == 1)
		{
			theApp.pStatic12008->EnableWindow(FALSE);
			theApp.pEdit12009->EnableWindow(FALSE);
			theApp.pStatic12010->EnableWindow(FALSE);
			theApp.pEdit12011->EnableWindow(FALSE);
			theApp.pStatic12012->EnableWindow(FALSE);
			theApp.pEdit12013->EnableWindow(FALSE);

			theApp.pStatic30004->ShowWindow(TRUE);
			theApp.pStatic30004->EnableWindow(TRUE);

			theApp.pComboBox30005->ShowWindow(TRUE);
			theApp.pComboBox30005->EnableWindow(TRUE);

			theApp.pComboBox30006->ShowWindow(FALSE);
			theApp.pComboBox30006->EnableWindow(FALSE);

			theApp.pStatic30008->EnableWindow(FALSE);
			theApp.pEdit30009->EnableWindow(FALSE);
			theApp.pStatic30010->EnableWindow(FALSE);
			theApp.pEdit30011->EnableWindow(FALSE);
			theApp.pStatic30012->EnableWindow(FALSE);
			theApp.pEdit30013->EnableWindow(FALSE);

			theApp.m_iIteration = 5000;
			theApp.pEdit10013.SetWindowText(_T("5000"));

			theApp.m_iOutRedAdder = 0;
			theApp.pEdit12003->SetWindowText(_T("0"));

			theApp.m_iOutGreenAdder = 0;
			theApp.pEdit12005->SetWindowText(_T("0"));

			theApp.m_iOutBlueAdder = 0;
			theApp.pEdit12007->SetWindowText(_T("0"));

		}
		else if(theApp.m_iColoringAlgorithmCurSel == 2)
		{
			theApp.pStatic12008->EnableWindow(FALSE);
			theApp.pEdit12009->EnableWindow(FALSE);
			theApp.pStatic12010->EnableWindow(FALSE);
			theApp.pEdit12011->EnableWindow(FALSE);
			theApp.pStatic12012->EnableWindow(FALSE);
			theApp.pEdit12013->EnableWindow(FALSE);

			theApp.pStatic30004->ShowWindow(TRUE);
			theApp.pStatic30004->EnableWindow(TRUE);

			theApp.pComboBox30005->ShowWindow(FALSE);
			theApp.pComboBox30005->EnableWindow(FALSE);

			theApp.pComboBox30006->ShowWindow(TRUE);
			theApp.pComboBox30006->EnableWindow(TRUE);

			theApp.pStatic30008->EnableWindow(TRUE);
			theApp.pEdit30009->EnableWindow(TRUE);
			theApp.pStatic30010->EnableWindow(TRUE);
			theApp.pEdit30011->EnableWindow(TRUE);
			theApp.pStatic30012->EnableWindow(TRUE);
			theApp.pEdit30013->EnableWindow(TRUE);

			theApp.m_iIteration = 250;
			theApp.pEdit10013.SetWindowText(_T("250"));

			theApp.m_iOutRedAdder = 255;
			theApp.pEdit12003->SetWindowText(_T("255"));

			theApp.m_iOutGreenAdder = 255;
			theApp.pEdit12005->SetWindowText(_T("255"));

			theApp.m_iOutBlueAdder = 255;
			theApp.pEdit12007->SetWindowText(_T("255"));

		}
	}

	if( (theApp.m_iSmoothGradientCurSel == 0) || (theApp.m_iSmoothGradientCurSel == 1) )
	{
		theApp.pButton30015->EnableWindow(FALSE);
		theApp.pButton30016->EnableWindow(FALSE);
		theApp.pButton30017->EnableWindow(FALSE);
	}

	if( (theApp.m_iColoringAlgorithmCurSel == 2) && (theApp.m_iSmoothGradientCurSel == 0) )
	{
		theApp.pButton30015->EnableWindow(FALSE);
		theApp.pButton30016->EnableWindow(FALSE);
		theApp.pButton30017->EnableWindow(FALSE);
	}

	if( (theApp.m_iColoringAlgorithmCurSel == 2) && (theApp.m_iSmoothGradientCurSel == 1) )
	{
		theApp.pButton30015->EnableWindow(TRUE);
		theApp.pButton30016->EnableWindow(TRUE);
		theApp.pButton30017->EnableWindow(TRUE);
	}

}

void CColoringProperties::OnCbnSelchange2ColorGradientPropertiesCombo()
{
	theApp.m_i2ColorGradientCurSel = theApp.pComboBox30005->GetCurSel();
}

void CColoringProperties::OnCbnSelchangeSmoothGradientPropertiesCombo()
{
	theApp.m_iSmoothGradientCurSel = theApp.pComboBox30006->GetCurSel();

	if( (theApp.m_iColoringAlgorithmCurSel == 2) && (theApp.m_iSmoothGradientCurSel == 0) )
	{
		theApp.pButton30015->EnableWindow(FALSE);
		theApp.pButton30016->EnableWindow(FALSE);
		theApp.pButton30017->EnableWindow(FALSE);
	}

	if( (theApp.m_iColoringAlgorithmCurSel == 2) && (theApp.m_iSmoothGradientCurSel == 1) )
	{
		theApp.pButton30015->EnableWindow(TRUE);
		theApp.pButton30016->EnableWindow(TRUE);
		theApp.pButton30017->EnableWindow(TRUE);
	}

}

void CColoringProperties::OnEnChangeSmoothRedMultiplier()
{
	CString sRed;
	GetDlgItem(30009)->GetWindowTextW(sRed);
	theApp.m_dSmoothRedValue = wcstod(sRed, NULL);
}

void CColoringProperties::OnEnChangeSmoothGreenMultiplier()
{
	CString sGreen;
	GetDlgItem(30011)->GetWindowTextW(sGreen);
	theApp.m_dSmoothGreenValue = wcstod(sGreen, NULL);
}

void CColoringProperties::OnEnChangeSmoothBlueMultiplier()
{
	CString sBlue;
	GetDlgItem(30013)->GetWindowTextW(sBlue);
	theApp.m_dSmoothBlueValue = wcstod(sBlue, NULL);
}

void CColoringProperties::OnBnClickedMainColorRed()
{
	theApp.m_bMainColoringRed = theApp.pButton30015->GetCheck();
}

void CColoringProperties::OnBnClickedMainColorGreen()
{
	theApp.m_bMainColoringGreen = theApp.pButton30016->GetCheck();
}

void CColoringProperties::OnBnClickedMainColorBlue()
{
	theApp.m_bMainColoringBlue = theApp.pButton30017->GetCheck();
}
