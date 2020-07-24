// NRootColoringProperties.cpp : implementation file
//

#include "stdafx.h"
#include "Fractoria.h"
#include "NRootColoringProperties.h"


// CNRootColoringProperties dialog

IMPLEMENT_DYNAMIC(CNRootColoringProperties, CDialog)

CNRootColoringProperties::CNRootColoringProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CNRootColoringProperties::IDD, pParent)
{
	m_sFirstRootRed.Format(_T("%d"), theApp.m_iFirstRootRed);
	m_sFirstRootGreen.Format(_T("%d"), theApp.m_iFirstRootGreen);
	m_sFirstRootBlue.Format(_T("%d"), theApp.m_iFirstRootBlue);

	m_sSecondRootRed.Format(_T("%d"), theApp.m_iSecondRootRed);
	m_sSecondRootGreen.Format(_T("%d"), theApp.m_iSecondRootGreen);
	m_sSecondRootBlue.Format(_T("%d"), theApp.m_iSecondRootBlue);

	m_sThirdRootRed.Format(_T("%d"), theApp.m_iThirdRootRed);
	m_sThirdRootGreen.Format(_T("%d"), theApp.m_iThirdRootGreen);
	m_sThirdRootBlue.Format(_T("%d"), theApp.m_iThirdRootBlue);
}

CNRootColoringProperties::~CNRootColoringProperties()
{
}

void CNRootColoringProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NROOTCOLORINGTAB, m_cNRootColoringTab);

	InitTab();
	InitAttributesTab();

}


BEGIN_MESSAGE_MAP(CNRootColoringProperties, CDialog)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(40003, &CNRootColoringProperties::OnEnChangeFirstRootRed)
	ON_EN_CHANGE(40005, &CNRootColoringProperties::OnEnChangeFirstRootGreen)
	ON_EN_CHANGE(40007, &CNRootColoringProperties::OnEnChangeFirstRootBlue)
	ON_EN_CHANGE(40010, &CNRootColoringProperties::OnEnChangeSecondRootRed)
	ON_EN_CHANGE(40012, &CNRootColoringProperties::OnEnChangeSecondRootGreen)
	ON_EN_CHANGE(40014, &CNRootColoringProperties::OnEnChangeSecondRootBlue)
	ON_EN_CHANGE(40017, &CNRootColoringProperties::OnEnChangeThirdRootRed)
	ON_EN_CHANGE(40019, &CNRootColoringProperties::OnEnChangeThirdRootGreen)
	ON_EN_CHANGE(40021, &CNRootColoringProperties::OnEnChangeThirdRootBlue)

END_MESSAGE_MAP()


// CNRootColoringProperties message handlers

CString CNRootColoringProperties::RemoveZeros(CString str)
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

HBRUSH CNRootColoringProperties::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CNRootColoringProperties::InitTab()
{
	m_cNRootColoringTab.InsertItem(0, _T("Root Algorithm"));
}

void CNRootColoringProperties::InitAttributesTab()
{
	// Tab 1 - Outside Coloring

	CButton* pGroupBox40001 = new CButton;
	pGroupBox40001->Create(_T("First Root Area"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
		CRect(25,40,300,140) ,this, 40001);
	pGroupBox40001->SetFont(GetFont());
	pGroupBox40001->ShowWindow(SW_SHOW);


	CStatic* pStatic40002 = new CStatic;
	pStatic40002->Create(_T("Red :"), WS_CHILD|WS_VISIBLE,
		CRect(35,60,148,80), this, 40002);
	pStatic40002->SetFont(GetFont());
	pStatic40002->ShowWindow(SW_SHOW);

	theApp.pEdit40003 = new CEdit;
	theApp.pEdit40003->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,57,290,77), this, 40003);
	theApp.pEdit40003->SetFont(GetFont());
	theApp.pEdit40003->ShowWindow(SW_SHOW);
	theApp.pEdit40003->SetWindowText(m_sFirstRootRed);


	CStatic* pStatic40004 = new CStatic;
	pStatic40004->Create(_T("Green :"), WS_CHILD|WS_VISIBLE,
		CRect(35,85,148,105), this, 40004);
	pStatic40004->SetFont(GetFont());
	pStatic40004->ShowWindow(SW_SHOW);

	theApp.pEdit40005 = new CEdit;
	theApp.pEdit40005->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,82,290,102), this, 40005);
	theApp.pEdit40005->SetFont(GetFont());
	theApp.pEdit40005->ShowWindow(SW_SHOW);
	theApp.pEdit40005->SetWindowText(m_sFirstRootGreen);


	CStatic* pStatic40006 = new CStatic;
	pStatic40006->Create(_T("Blue :"), WS_CHILD|WS_VISIBLE,
		CRect(35,110,148,130), this, 40006);
	pStatic40006->SetFont(GetFont());
	pStatic40006->ShowWindow(SW_SHOW);

	theApp.pEdit40007 = new CEdit;
	theApp.pEdit40007->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,107,290,127), this, 40007);
	theApp.pEdit40007->SetFont(GetFont());
	theApp.pEdit40007->ShowWindow(SW_SHOW);
	theApp.pEdit40007->SetWindowText(m_sFirstRootBlue);


	CButton* pGroupBox40008 = new CButton;
	pGroupBox40008->Create(_T("Second Root Area"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
		CRect(25,140,300,240) ,this, 40008);
	pGroupBox40008->SetFont(GetFont());
	pGroupBox40008->ShowWindow(SW_SHOW);


	CStatic* pStatic40009 = new CStatic;
	pStatic40009->Create(_T("Red :"), WS_CHILD|WS_VISIBLE,
		CRect(35,160,148,180), this, 40009);
	pStatic40009->SetFont(GetFont());
	pStatic40009->ShowWindow(SW_SHOW);

	theApp.pEdit40010 = new CEdit;
	theApp.pEdit40010->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,157,290,177), this, 40010);
	theApp.pEdit40010->SetFont(GetFont());
	theApp.pEdit40010->ShowWindow(SW_SHOW);
	theApp.pEdit40010->SetWindowText(m_sSecondRootRed);


	CStatic* pStatic40011 = new CStatic;
	pStatic40011->Create(_T("Green :"), WS_CHILD|WS_VISIBLE,
		CRect(35,185,148,205), this, 40011);
	pStatic40011->SetFont(GetFont());
	pStatic40011->ShowWindow(SW_SHOW);

	theApp.pEdit40012 = new CEdit;
	theApp.pEdit40012->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,182,290,202), this, 40012);
	theApp.pEdit40012->SetFont(GetFont());
	theApp.pEdit40012->ShowWindow(SW_SHOW);
	theApp.pEdit40012->SetWindowText(m_sSecondRootGreen);


	CStatic* pStatic40013 = new CStatic;
	pStatic40013->Create(_T("Blue :"), WS_CHILD|WS_VISIBLE,
		CRect(35,210,148,230), this, 40013);
	pStatic40013->SetFont(GetFont());
	pStatic40013->ShowWindow(SW_SHOW);

	theApp.pEdit40014 = new CEdit;
	theApp.pEdit40014->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,207,290,227), this, 40014);
	theApp.pEdit40014->SetFont(GetFont());
	theApp.pEdit40014->ShowWindow(SW_SHOW);
	theApp.pEdit40014->SetWindowText(m_sSecondRootBlue);


	CButton* pGroupBox40015 = new CButton;
	pGroupBox40015->Create(_T("Third Root Area"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
		CRect(25,240,300,340) ,this, 40015);
	pGroupBox40015->SetFont(GetFont());
	pGroupBox40015->ShowWindow(SW_SHOW);


	CStatic* pStatic40016 = new CStatic;
	pStatic40016->Create(_T("Red :"), WS_CHILD|WS_VISIBLE,
		CRect(35,260,148,280), this, 40016);
	pStatic40016->SetFont(GetFont());
	pStatic40016->ShowWindow(SW_SHOW);

	theApp.pEdit40017 = new CEdit;
	theApp.pEdit40017->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,257,290,277), this, 40017);
	theApp.pEdit40017->SetFont(GetFont());
	theApp.pEdit40017->ShowWindow(SW_SHOW);
	theApp.pEdit40017->SetWindowText(m_sThirdRootRed);


	CStatic* pStatic40018 = new CStatic;
	pStatic40018->Create(_T("Green :"), WS_CHILD|WS_VISIBLE,
		CRect(35,285,148,305), this, 40018);
	pStatic40018->SetFont(GetFont());
	pStatic40018->ShowWindow(SW_SHOW);

	theApp.pEdit40019 = new CEdit;
	theApp.pEdit40019->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,282,290,302), this, 40019);
	theApp.pEdit40019->SetFont(GetFont());
	theApp.pEdit40019->ShowWindow(SW_SHOW);
	theApp.pEdit40019->SetWindowText(m_sThirdRootGreen);


	CStatic* pStatic40020 = new CStatic;
	pStatic40020->Create(_T("Blue :"), WS_CHILD|WS_VISIBLE,
		CRect(35,310,148,330), this, 40020);
	pStatic40020->SetFont(GetFont());
	pStatic40020->ShowWindow(SW_SHOW);

	theApp.pEdit40021 = new CEdit;
	theApp.pEdit40021->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,307,290,327), this, 40021);
	theApp.pEdit40021->SetFont(GetFont());
	theApp.pEdit40021->ShowWindow(SW_SHOW);
	theApp.pEdit40021->SetWindowText(m_sThirdRootBlue);

}

void CNRootColoringProperties::OnEnChangeFirstRootRed()
{
	CString sFirstRootRed;
	GetDlgItem(40003)->GetWindowTextW(sFirstRootRed);
	theApp.m_iFirstRootRed = _ttoi(sFirstRootRed);
}

void CNRootColoringProperties::OnEnChangeFirstRootGreen()
{
	CString sFirstRootGreen;
	GetDlgItem(40005)->GetWindowTextW(sFirstRootGreen);
	theApp.m_iFirstRootGreen = _ttoi(sFirstRootGreen);
}

void CNRootColoringProperties::OnEnChangeFirstRootBlue()
{
	CString sFirstRootBlue;
	GetDlgItem(40007)->GetWindowTextW(sFirstRootBlue);
	theApp.m_iFirstRootBlue = _ttoi(sFirstRootBlue);
}

void CNRootColoringProperties::OnEnChangeSecondRootRed()
{
	CString sSecondRootRed;
	GetDlgItem(40010)->GetWindowTextW(sSecondRootRed);
	theApp.m_iSecondRootRed = _ttoi(sSecondRootRed);
}

void CNRootColoringProperties::OnEnChangeSecondRootGreen()
{
	CString sSecondRootGreen;
	GetDlgItem(40012)->GetWindowTextW(sSecondRootGreen);
	theApp.m_iSecondRootGreen = _ttoi(sSecondRootGreen);
}

void CNRootColoringProperties::OnEnChangeSecondRootBlue()
{
	CString sSecondRootBlue;
	GetDlgItem(40014)->GetWindowTextW(sSecondRootBlue);
	theApp.m_iSecondRootBlue = _ttoi(sSecondRootBlue);
}

void CNRootColoringProperties::OnEnChangeThirdRootRed()
{
	CString sThirdRootRed;
	GetDlgItem(40017)->GetWindowTextW(sThirdRootRed);
	theApp.m_iThirdRootRed = _ttoi(sThirdRootRed);
}

void CNRootColoringProperties::OnEnChangeThirdRootGreen()
{
	CString sThirdRootGreen;
	GetDlgItem(40019)->GetWindowTextW(sThirdRootGreen);
	theApp.m_iThirdRootGreen = _ttoi(sThirdRootGreen);
}

void CNRootColoringProperties::OnEnChangeThirdRootBlue()
{
	CString sThirdRootBlue;
	GetDlgItem(40021)->GetWindowTextW(sThirdRootBlue);
	theApp.m_iThirdRootBlue = _ttoi(sThirdRootBlue);
}
