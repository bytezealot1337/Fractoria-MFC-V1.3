// AnimationProperties.cpp : implementation file
//

#include "stdafx.h"
#include "Fractoria.h"
#include "AnimationProperties.h"
#include "FractoriaDlg.h"


// CAnimationProperties dialog

IMPLEMENT_DYNAMIC(CAnimationProperties, CDialog)

CAnimationProperties::CAnimationProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CAnimationProperties::IDD, pParent)
{
	m_iTriggerPointCombo = 0;

	m_sTotalFrame.Format(_T("%d"), theApp.m_iTotalFrame);

	m_sFrame.Format(_T("%d"), theApp.m_iFramePoint[0]);

	m_sEndXAxis.Format(_T("%lf"), theApp.m_dEndXAxis[0]);
	m_sEndXAxis = RemoveZeros(m_sEndXAxis);
	
	m_sEndYAxis.Format(_T("%lf"), theApp.m_dEndYAxis[0]);
	m_sEndYAxis = RemoveZeros(m_sEndYAxis);
	
	m_sAnimationControl1.Format(_T("%lf"), theApp.m_dAnimationControl1[0]);
	m_sAnimationControl1 = RemoveZeros(m_sAnimationControl1);

	m_sAnimationControl2.Format(_T("%lf"), theApp.m_dAnimationControl2[0]);
	m_sAnimationControl2 = RemoveZeros(m_sAnimationControl2);

	m_sAnimationControl3.Format(_T("%lf"), theApp.m_dAnimationControl3[0]);
	m_sAnimationControl3 = RemoveZeros(m_sAnimationControl3);

	m_sAnimationControl4.Format(_T("%lf"), theApp.m_dAnimationControl4[0]);
	m_sAnimationControl4 = RemoveZeros(m_sAnimationControl4);

	m_sAnimationControl5.Format(_T("%lf"), theApp.m_dAnimationControl5[0]);
	m_sAnimationControl5 = RemoveZeros(m_sAnimationControl5);

	m_sAnimationControl6.Format(_T("%lf"), theApp.m_dAnimationControl6[0]);
	m_sAnimationControl6 = RemoveZeros(m_sAnimationControl6);

	m_iAnimationControl1Combo = theApp.m_iAnimationControl1Function[0];
	m_iAnimationControl2Combo = theApp.m_iAnimationControl2Function[0];
	m_iAnimationControl3Combo = theApp.m_iAnimationControl3Function[0];
	m_iAnimationControl4Combo = theApp.m_iAnimationControl4Function[0];
	m_iAnimationControl5Combo = theApp.m_iAnimationControl5Function[0];
	m_iAnimationControl6Combo = theApp.m_iAnimationControl6Function[0];

}

CAnimationProperties::~CAnimationProperties()
{
}

void CAnimationProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANIMATIONTAB, m_cAnimationTab);

	InitTab();
	InitGeometryTab();
	InitFormulaTab();
	InitOutsideTab();
	InitInsideTab();

	OnCbnSelchangeTriggerPointCombo();

	InitFormulaControls();

}


BEGIN_MESSAGE_MAP(CAnimationProperties, CDialog)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TCN_SELCHANGE, IDC_ANIMATIONTAB, &CAnimationProperties::OnTcnSelchangeTabcontrol)
	ON_EN_CHANGE(20003, &CAnimationProperties::OnEnChangeTotalFrame)
	ON_CBN_SELCHANGE(20005, &CAnimationProperties::OnCbnSelchangeTriggerPointCombo)
	ON_EN_CHANGE(20007, &CAnimationProperties::OnEnChangeFrame)
	ON_EN_CHANGE(20009, &CAnimationProperties::OnEnChangeEndXAxis)
	ON_EN_CHANGE(20011, &CAnimationProperties::OnEnChangeEndYAxis)
	ON_CBN_SELCHANGE(20013, &CAnimationProperties::OnCbnSelchangeTranslationFunctionCombo)
	ON_EN_CHANGE(20015, &CAnimationProperties::OnEnChangeEndZoomRatio)
	ON_CBN_SELCHANGE(20017, &CAnimationProperties::OnCbnSelchangeZoomRatioFunctionCombo)
	ON_EN_CHANGE(20019, &CAnimationProperties::OnEnChangeEndRotation)
	ON_CBN_SELCHANGE(20021, &CAnimationProperties::OnCbnSelchangeRotationFunctionCombo)
	ON_EN_CHANGE(20023, &CAnimationProperties::OnEnChangeEndIteration)
	ON_CBN_SELCHANGE(20025, &CAnimationProperties::OnCbnSelchangeIterationFunctionCombo)

	ON_EN_CHANGE(21003, &CAnimationProperties::OnChangeAnimationControl1)
	ON_CBN_SELCHANGE(21005, &CAnimationProperties::OnCbnSelchangeAnimationControl1Combo)
	ON_EN_CHANGE(21007, &CAnimationProperties::OnChangeAnimationControl2)
	ON_CBN_SELCHANGE(21009, &CAnimationProperties::OnCbnSelchangeAnimationControl2Combo)
	ON_EN_CHANGE(21011, &CAnimationProperties::OnChangeAnimationControl3)
	ON_CBN_SELCHANGE(21013, &CAnimationProperties::OnCbnSelchangeAnimationControl3Combo)
	ON_EN_CHANGE(21015, &CAnimationProperties::OnChangeAnimationControl4)
	ON_CBN_SELCHANGE(21017, &CAnimationProperties::OnCbnSelchangeAnimationControl4Combo)
	ON_EN_CHANGE(21019, &CAnimationProperties::OnChangeAnimationControl5)
	ON_CBN_SELCHANGE(21021, &CAnimationProperties::OnCbnSelchangeAnimationControl5Combo)
	ON_EN_CHANGE(21023, &CAnimationProperties::OnChangeAnimationControl6)
	ON_CBN_SELCHANGE(21025, &CAnimationProperties::OnCbnSelchangeAnimationControl6Combo)

	ON_EN_CHANGE(22003, &CAnimationProperties::OnChangeAnimationOutsideRedAdder)
	ON_CBN_SELCHANGE(22005, &CAnimationProperties::OnCbnSelchangeAnimationOutsideRedAdderCombo)
	ON_EN_CHANGE(22007, &CAnimationProperties::OnChangeAnimationOutsideGreenAdder)
	ON_CBN_SELCHANGE(22009, &CAnimationProperties::OnCbnSelchangeAnimationOutsideGreenAdderCombo)
	ON_EN_CHANGE(22011, &CAnimationProperties::OnChangeAnimationOutsideBlueAdder)
	ON_CBN_SELCHANGE(22013, &CAnimationProperties::OnCbnSelchangeAnimationOutsideBlueAdderCombo)
	ON_EN_CHANGE(22015, &CAnimationProperties::OnChangeAnimationOutsideRedMult)
	ON_CBN_SELCHANGE(22017, &CAnimationProperties::OnCbnSelchangeAnimationOutsideRedMultCombo)
	ON_EN_CHANGE(22019, &CAnimationProperties::OnChangeAnimationOutsideGreenMult)
	ON_CBN_SELCHANGE(22021, &CAnimationProperties::OnCbnSelchangeAnimationOutsideGreenMultCombo)
	ON_EN_CHANGE(22023, &CAnimationProperties::OnChangeAnimationOutsideBlueMult)
	ON_CBN_SELCHANGE(22025, &CAnimationProperties::OnCbnSelchangeAnimationOutsideBlueMultCombo)

	ON_EN_CHANGE(23003, &CAnimationProperties::OnChangeAnimationInsideRedAdder)
	ON_CBN_SELCHANGE(23005, &CAnimationProperties::OnCbnSelchangeAnimationInsideRedAdderCombo)
	ON_EN_CHANGE(23007, &CAnimationProperties::OnChangeAnimationInsideGreenAdder)
	ON_CBN_SELCHANGE(23009, &CAnimationProperties::OnCbnSelchangeAnimationInsideGreenAdderCombo)
	ON_EN_CHANGE(23011, &CAnimationProperties::OnChangeAnimationInsideBlueAdder)
	ON_CBN_SELCHANGE(23013, &CAnimationProperties::OnCbnSelchangeAnimationInsideBlueAdderCombo)
	ON_EN_CHANGE(23015, &CAnimationProperties::OnChangeAnimationInsideRedMult)
	ON_CBN_SELCHANGE(23017, &CAnimationProperties::OnCbnSelchangeAnimationInsideRedMultCombo)
	ON_EN_CHANGE(23019, &CAnimationProperties::OnChangeAnimationInsideGreenMult)
	ON_CBN_SELCHANGE(23021, &CAnimationProperties::OnCbnSelchangeAnimationInsideGreenMultCombo)
	ON_EN_CHANGE(23023, &CAnimationProperties::OnChangeAnimationInsideBlueMult)
	ON_CBN_SELCHANGE(23025, &CAnimationProperties::OnCbnSelchangeAnimationInsideBlueMultCombo)
END_MESSAGE_MAP()


// CAnimationProperties message handlers

CString CAnimationProperties::RemoveZeros(CString str)
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

HBRUSH CAnimationProperties::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CAnimationProperties::InitTab()
{
	m_cAnimationTab.InsertItem(0, _T("Geometry"));
	m_cAnimationTab.InsertItem(1, _T("Formula"));
	m_cAnimationTab.InsertItem(2, _T("Outside"));
	m_cAnimationTab.InsertItem(3, _T("Inside"));
}

void CAnimationProperties::InitGeometryTab()
{
	// Tab 1 - Geometry

	CButton* pGroupBox20001 = new CButton;
	pGroupBox20001->Create(_T("Properties"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
		CRect(25,40,300,390) ,this, 20001);
	pGroupBox20001->SetFont(GetFont());
	pGroupBox20001->ShowWindow(SW_SHOW);

	CStatic* pStatic20002 = new CStatic;
	pStatic20002->Create(_T("Total Frame Number :"), WS_CHILD|WS_VISIBLE,
		CRect(35,60,148,80), this, 20002);
	pStatic20002->SetFont(GetFont());
	pStatic20002->ShowWindow(SW_SHOW);

	CEdit* pEdit20003 = new CEdit;
	pEdit20003->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,57,290,77), this, 20003);
	pEdit20003->SetFont(GetFont());
	pEdit20003->ShowWindow(SW_SHOW);
	pEdit20003->SetWindowText(m_sTotalFrame);

	CStatic* pStatic20004 = new CStatic;
	pStatic20004->Create(_T("Trigger Point :"), WS_CHILD|WS_VISIBLE,
		CRect(35,90,148,110), this, 20004);
	pStatic20004->SetFont(GetFont());
	pStatic20004->ShowWindow(SW_SHOW);

	// Trigger Point ComboBox
	pComboBox20005 = new CComboBox;
	pComboBox20005->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,87,290,107), this, 20005);
	pComboBox20005->SetFont(GetFont());
	pComboBox20005->ShowWindow(SW_SHOW);
	
	pComboBox20005->AddString(_T("1"));
	pComboBox20005->AddString(_T("2"));
	pComboBox20005->AddString(_T("3"));
	pComboBox20005->AddString(_T("4"));
	pComboBox20005->AddString(_T("5"));
	pComboBox20005->AddString(_T("6"));
	pComboBox20005->AddString(_T("7"));
	pComboBox20005->AddString(_T("8"));

	pComboBox20005->SetCurSel(0);


	CStatic* pStatic20006 = new CStatic;
	pStatic20006->Create(_T("Trigger Frame :"), WS_CHILD|WS_VISIBLE,
		CRect(35,115,148,135), this, 20006);
	pStatic20006->SetFont(GetFont());
	pStatic20006->ShowWindow(SW_SHOW);

	pEdit20007 = new CEdit;
	pEdit20007->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,112,290,132), this, 20007);
	pEdit20007->SetFont(GetFont());
	pEdit20007->ShowWindow(SW_SHOW);
	pEdit20007->SetWindowText(m_sFrame);

	CStatic* pStatic20008 = new CStatic;
	pStatic20008->Create(_T("End X-Axis Center :"), WS_CHILD|WS_VISIBLE,
		CRect(35,145,148,165), this, 20008);
	pStatic20008->SetFont(GetFont());
	pStatic20008->ShowWindow(SW_SHOW);

	CEdit* pEdit20009 = new CEdit;
	pEdit20009->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,142,290,162), this, 20009);
	pEdit20009->SetFont(GetFont());
	pEdit20009->ShowWindow(SW_SHOW);
	pEdit20009->SetWindowText(m_sEndXAxis);

	CStatic* pStatic20010 = new CStatic;
	pStatic20010->Create(_T("End Y-Axis Center :"), WS_CHILD|WS_VISIBLE,
		CRect(35,170,148,190), this, 20010);
	pStatic20010->SetFont(GetFont());
	pStatic20010->ShowWindow(SW_SHOW);

	CEdit* pEdit20011 = new CEdit;
	pEdit20011->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,167,290,187), this, 20011);
	pEdit20011->SetFont(GetFont());
	pEdit20011->ShowWindow(SW_SHOW);
	pEdit20011->SetWindowText(m_sEndYAxis);

	CStatic* pStatic20012 = new CStatic;
	pStatic20012->Create(_T("Translation Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,195,148,215), this, 20012);
	pStatic20012->SetFont(GetFont());
	pStatic20012->ShowWindow(SW_SHOW);

	pComboBox20013 = new CComboBox;
	pComboBox20013->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,192,290,212), this, 20013);
	pComboBox20013->SetFont(GetFont());
	pComboBox20013->ShowWindow(SW_SHOW);
	
	pComboBox20013->AddString(_T("Linear"));
	pComboBox20013->AddString(_T("Positive Quadratic"));
	pComboBox20013->AddString(_T("Negative Quadratic"));
	pComboBox20013->AddString(_T("Negative Cosine"));

	pComboBox20013->SetCurSel(0);
	

	CStatic* pStatic20014 = new CStatic;
	pStatic20014->Create(_T("End Zoom Ratio :"), WS_CHILD|WS_VISIBLE,
		CRect(35,225,148,245), this, 20014);
	pStatic20014->SetFont(GetFont());
	pStatic20014->ShowWindow(SW_SHOW);

	CEdit* pEdit20015 = new CEdit;
	pEdit20015->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,222,290,242), this, 20015);
	pEdit20015->SetFont(GetFont());
	pEdit20015->ShowWindow(SW_SHOW);
	pEdit20015->SetWindowText(m_sEndZoomRatio);

	CStatic* pStatic20016 = new CStatic;
	pStatic20016->Create(_T("Zoom Ratio Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,250,148,270), this, 20016);
	pStatic20016->SetFont(GetFont());
	pStatic20016->ShowWindow(SW_SHOW);

	pComboBox20017 = new CComboBox;
	pComboBox20017->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,247,290,267), this, 20017);
	pComboBox20017->SetFont(GetFont());
	pComboBox20017->ShowWindow(SW_SHOW);
	
	pComboBox20017->AddString(_T("Linear"));
	pComboBox20017->AddString(_T("Summation"));
	pComboBox20017->AddString(_T("Positive Quadratic"));
	pComboBox20017->AddString(_T("Negative Quadratic"));

	pComboBox20017->SetCurSel(0);
	

	CStatic* pStatic20018 = new CStatic;
	pStatic20018->Create(_T("End Rotation :"), WS_CHILD|WS_VISIBLE,
		CRect(35,280,148,300), this, 20018);
	pStatic20018->SetFont(GetFont());
	pStatic20018->ShowWindow(SW_SHOW);

	CEdit* pEdit20019 = new CEdit;
	pEdit20019->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,277,290,297), this, 20019);
	pEdit20019->SetFont(GetFont());
	pEdit20019->ShowWindow(SW_SHOW);
	pEdit20019->SetWindowText(m_sEndRotation);

	CStatic* pStatic20020 = new CStatic;
	pStatic20020->Create(_T("Rotation Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,305,148,325), this, 20020);
	pStatic20020->SetFont(GetFont());
	pStatic20020->ShowWindow(SW_SHOW);

	pComboBox20021 = new CComboBox;
	pComboBox20021->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,302,290,322), this, 20021);
	pComboBox20021->SetFont(GetFont());
	pComboBox20021->ShowWindow(SW_SHOW);
	
	pComboBox20021->AddString(_T("Linear"));
	pComboBox20021->AddString(_T("Positive Quadratic"));
	pComboBox20021->AddString(_T("Negative Quadratic"));

	pComboBox20021->SetCurSel(0);
	

	CStatic* pStatic20022 = new CStatic;
	pStatic20022->Create(_T("End Iteration :"), WS_CHILD|WS_VISIBLE,
		CRect(35,335,148,355), this, 20022);
	pStatic20022->SetFont(GetFont());
	pStatic20022->ShowWindow(SW_SHOW);

	CEdit* pEdit20023 = new CEdit;
	pEdit20023->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,332,290,352), this, 20023);
	pEdit20023->SetFont(GetFont());
	pEdit20023->ShowWindow(SW_SHOW);
	CString sEndIteration;
	sEndIteration.Format(_T("%d"), theApp.m_iIteration);
	pEdit20023->SetWindowText(sEndIteration);
	for(int count = 0; count < 8; count++)
	{
		theApp.m_iEndIteration[count] = theApp.m_iIteration;
	}

	CStatic* pStatic20024 = new CStatic;
	pStatic20024->Create(_T("Iteration Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,360,148,380), this, 20024);
	pStatic20024->SetFont(GetFont());
	pStatic20024->ShowWindow(SW_SHOW);

	pComboBox20025 = new CComboBox;
	pComboBox20025->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,357,290,377), this, 20025);
	pComboBox20025->SetFont(GetFont());
	pComboBox20025->ShowWindow(SW_SHOW);

	pComboBox20025->AddString(_T("Linear"));
	pComboBox20025->AddString(_T("Positive Quadratic"));
	pComboBox20025->AddString(_T("Negative Quadratic"));

	pComboBox20025->SetCurSel(0);

}

void CAnimationProperties::InitFormulaTab()
{
	// Tab 2 - Formula

	CButton* pGroupBox21001 = new CButton;
	pGroupBox21001->Create(_T("Properties"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
		CRect(25,40,300,390) ,this, 21001);
	pGroupBox21001->SetFont(GetFont());
	pGroupBox21001->ShowWindow(SW_HIDE);

	CStatic* pStatic21002 = new CStatic;
	pStatic21002->Create(_T("Control #1 :"), WS_CHILD|WS_VISIBLE,
		CRect(35,60,148,80), this, 21002);
	pStatic21002->SetFont(GetFont());
	pStatic21002->ShowWindow(SW_HIDE);

	CEdit* pEdit21003 = new CEdit;
	pEdit21003->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,57,290,77), this, 21003);
	pEdit21003->SetFont(GetFont());
	pEdit21003->ShowWindow(SW_HIDE);
	pEdit21003->SetWindowText(m_sAnimationControl1);

	CStatic* pStatic21004 = new CStatic;
	pStatic21004->Create(_T("Control #1 Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,85,148,105), this, 21004);
	pStatic21004->SetFont(GetFont());
	pStatic21004->ShowWindow(SW_HIDE);

	theApp.pComboBox21005 = new CComboBox;
	theApp.pComboBox21005->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,82,290,102), this, 21005);
	theApp.pComboBox21005->SetFont(GetFont());
	theApp.pComboBox21005->ShowWindow(SW_HIDE);

	theApp.pComboBox21005->AddString(_T("Linear"));
	theApp.pComboBox21005->AddString(_T("Positive Quadratic"));
	theApp.pComboBox21005->AddString(_T("Negative Quadratic"));
	theApp.pComboBox21005->AddString(_T("Negative Cosine"));

	theApp.pComboBox21005->SetCurSel(m_iAnimationControl1Combo);

	CStatic* pStatic21006 = new CStatic;
	pStatic21006->Create(_T("Control #2 :"), WS_CHILD|WS_VISIBLE,
		CRect(35,115,148,135), this, 21006);
	pStatic21006->SetFont(GetFont());
	pStatic21006->ShowWindow(SW_HIDE);

	CEdit* pEdit21007 = new CEdit;
	pEdit21007->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,112,290,132), this, 21007);
	pEdit21007->SetFont(GetFont());
	pEdit21007->ShowWindow(SW_HIDE);
	pEdit21007->SetWindowText(m_sAnimationControl2);

	CStatic* pStatic21008 = new CStatic;
	pStatic21008->Create(_T("Control #2 Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,140,148,160), this, 21008);
	pStatic21008->SetFont(GetFont());
	pStatic21008->ShowWindow(SW_HIDE);

	theApp.pComboBox21009 = new CComboBox;
	theApp.pComboBox21009->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,137,290,157), this, 21009);
	theApp.pComboBox21009->SetFont(GetFont());
	theApp.pComboBox21009->ShowWindow(SW_HIDE);
	
	theApp.pComboBox21009->AddString(_T("Linear"));
	theApp.pComboBox21009->AddString(_T("Positive Quadratic"));
	theApp.pComboBox21009->AddString(_T("Negative Quadratic"));
	theApp.pComboBox21009->AddString(_T("Negative Cosine"));

	theApp.pComboBox21009->SetCurSel(m_iAnimationControl2Combo);

	CStatic* pStatic21010 = new CStatic;
	pStatic21010->Create(_T("Control #3 :"), WS_CHILD|WS_VISIBLE,
		CRect(35,170,148,190), this, 21010);
	pStatic21010->SetFont(GetFont());
	pStatic21010->ShowWindow(SW_HIDE);

	CEdit* pEdit21011 = new CEdit;
	pEdit21011->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,167,290,187), this, 21011);
	pEdit21011->SetFont(GetFont());
	pEdit21011->ShowWindow(SW_HIDE);
	pEdit21011->SetWindowText(m_sAnimationControl3);

	CStatic* pStatic21012 = new CStatic;
	pStatic21012->Create(_T("Control #3 Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,195,148,215), this, 21012);
	pStatic21012->SetFont(GetFont());
	pStatic21012->ShowWindow(SW_HIDE);

	theApp.pComboBox21013 = new CComboBox;
	theApp.pComboBox21013->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,192,290,212), this, 21013);
	theApp.pComboBox21013->SetFont(GetFont());
	theApp.pComboBox21013->ShowWindow(SW_HIDE);

	theApp.pComboBox21013->AddString(_T("Linear"));
	theApp.pComboBox21013->AddString(_T("Positive Quadratic"));
	theApp.pComboBox21013->AddString(_T("Negative Quadratic"));
	theApp.pComboBox21013->AddString(_T("Negative Cosine"));

	theApp.pComboBox21013->SetCurSel(m_iAnimationControl3Combo);

	CStatic* pStatic21014 = new CStatic;
	pStatic21014->Create(_T("Control #4 :"), WS_CHILD|WS_VISIBLE,
		CRect(35,225,148,245), this, 21014);
	pStatic21014->SetFont(GetFont());
	pStatic21014->ShowWindow(SW_HIDE);

	CEdit* pEdit21015 = new CEdit;
	pEdit21015->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,222,290,242), this, 21015);
	pEdit21015->SetFont(GetFont());
	pEdit21015->ShowWindow(SW_HIDE);
	pEdit21015->SetWindowText(m_sAnimationControl4);

	CStatic* pStatic21016 = new CStatic;
	pStatic21016->Create(_T("Control #4 Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,250,148,270), this, 21016);
	pStatic21016->SetFont(GetFont());
	pStatic21016->ShowWindow(SW_HIDE);

	theApp.pComboBox21017 = new CComboBox;
	theApp.pComboBox21017->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,247,290,267), this, 21017);
	theApp.pComboBox21017->SetFont(GetFont());
	theApp.pComboBox21017->ShowWindow(SW_HIDE);

	theApp.pComboBox21017->AddString(_T("Linear"));
	theApp.pComboBox21017->AddString(_T("Positive Quadratic"));
	theApp.pComboBox21017->AddString(_T("Negative Quadratic"));
	theApp.pComboBox21017->AddString(_T("Negative Cosine"));

	theApp.pComboBox21017->SetCurSel(m_iAnimationControl4Combo);

	CStatic* pStatic21018 = new CStatic;
	pStatic21018->Create(_T("Control #5 :"), WS_CHILD|WS_VISIBLE,
		CRect(35,280,148,300), this, 21018);
	pStatic21018->SetFont(GetFont());
	pStatic21018->ShowWindow(SW_HIDE);

	CEdit* pEdit21019 = new CEdit;
	pEdit21019->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,277,290,297), this, 21019);
	pEdit21019->SetFont(GetFont());
	pEdit21019->ShowWindow(SW_HIDE);
	pEdit21019->SetWindowText(m_sAnimationControl5);

	CStatic* pStatic21020 = new CStatic;
	pStatic21020->Create(_T("Control #5 Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,305,148,325), this, 21020);
	pStatic21020->SetFont(GetFont());
	pStatic21020->ShowWindow(SW_HIDE);

	theApp.pComboBox21021 = new CComboBox;
	theApp.pComboBox21021->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,302,290,322), this, 21021);
	theApp.pComboBox21021->SetFont(GetFont());
	theApp.pComboBox21021->ShowWindow(SW_HIDE);

	theApp.pComboBox21021->AddString(_T("Linear"));
	theApp.pComboBox21021->AddString(_T("Positive Quadratic"));
	theApp.pComboBox21021->AddString(_T("Negative Quadratic"));
	theApp.pComboBox21021->AddString(_T("Negative Cosine"));

	theApp.pComboBox21021->SetCurSel(m_iAnimationControl5Combo);

	CStatic* pStatic21022 = new CStatic;
	pStatic21022->Create(_T("Control #6 :"), WS_CHILD|WS_VISIBLE,
		CRect(35,335,148,355), this, 21022);
	pStatic21022->SetFont(GetFont());
	pStatic21022->ShowWindow(SW_HIDE);

	CEdit* pEdit21023 = new CEdit;
	pEdit21023->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,332,290,352), this, 21023);
	pEdit21023->SetFont(GetFont());
	pEdit21023->ShowWindow(SW_HIDE);
	pEdit21023->SetWindowText(m_sAnimationControl6);

	CStatic* pStatic21024 = new CStatic;
	pStatic21024->Create(_T("Control #6 Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,360,148,380), this, 21024);
	pStatic21024->SetFont(GetFont());
	pStatic21024->ShowWindow(SW_HIDE);

	theApp.pComboBox21025 = new CComboBox;
	theApp.pComboBox21025->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,357,290,377), this, 21025);
	theApp.pComboBox21025->SetFont(GetFont());
	theApp.pComboBox21025->ShowWindow(SW_HIDE);

	theApp.pComboBox21025->AddString(_T("Linear"));
	theApp.pComboBox21025->AddString(_T("Positive Quadratic"));
	theApp.pComboBox21025->AddString(_T("Negative Quadratic"));
	theApp.pComboBox21025->AddString(_T("Negative Cosine"));

	theApp.pComboBox21025->SetCurSel(m_iAnimationControl6Combo);

}

void CAnimationProperties::InitOutsideTab()
{
	// Tab 3 - Outside Coloring

	CButton* pGroupBox22001 = new CButton;
	pGroupBox22001->Create(_T("Properties"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
		CRect(25,40,300,390) ,this, 22001);
	pGroupBox22001->SetFont(GetFont());
	pGroupBox22001->ShowWindow(SW_HIDE);

	CStatic* pStatic22002 = new CStatic;
	pStatic22002->Create(_T("End Red Adder :"), WS_CHILD|WS_VISIBLE,
		CRect(35,60,148,80), this, 22002);
	pStatic22002->SetFont(GetFont());
	pStatic22002->ShowWindow(SW_HIDE);

	CEdit* pEdit22003 = new CEdit;
	pEdit22003->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,57,290,77), this, 22003);
	pEdit22003->SetFont(GetFont());
	pEdit22003->ShowWindow(SW_HIDE);
	pEdit22003->SetWindowText(m_sAnimationOutsideRedAdder);

	CStatic* pStatic22004 = new CStatic;
	pStatic22004->Create(_T("Red Add. Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,85,148,105), this, 22004);
	pStatic22004->SetFont(GetFont());
	pStatic22004->ShowWindow(SW_HIDE);

	theApp.pComboBox22005 = new CComboBox;
	theApp.pComboBox22005->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,82,290,102), this, 22005);
	theApp.pComboBox22005->SetFont(GetFont());
	theApp.pComboBox22005->ShowWindow(SW_HIDE);

	theApp.pComboBox22005->AddString(_T("Linear"));
	theApp.pComboBox22005->AddString(_T("Positive Quadratic"));
	theApp.pComboBox22005->AddString(_T("Negative Quadratic"));

	theApp.pComboBox22005->SetCurSel(0);

	CStatic* pStatic22006 = new CStatic;
	pStatic22006->Create(_T("End Green Adder :"), WS_CHILD|WS_VISIBLE,
		CRect(35,115,148,135), this, 22006);
	pStatic22006->SetFont(GetFont());
	pStatic22006->ShowWindow(SW_HIDE);

	CEdit* pEdit22007 = new CEdit;
	pEdit22007->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,112,290,132), this, 22007);
	pEdit22007->SetFont(GetFont());
	pEdit22007->ShowWindow(SW_HIDE);
	pEdit22007->SetWindowText(m_sAnimationOutsideGreenAdder);

	CStatic* pStatic22008 = new CStatic;
	pStatic22008->Create(_T("Green Add. Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,140,148,160), this, 22008);
	pStatic22008->SetFont(GetFont());
	pStatic22008->ShowWindow(SW_HIDE);

	theApp.pComboBox22009 = new CComboBox;
	theApp.pComboBox22009->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,137,290,157), this, 22009);
	theApp.pComboBox22009->SetFont(GetFont());
	theApp.pComboBox22009->ShowWindow(SW_HIDE);
	
	theApp.pComboBox22009->AddString(_T("Linear"));
	theApp.pComboBox22009->AddString(_T("Positive Quadratic"));
	theApp.pComboBox22009->AddString(_T("Negative Quadratic"));

	theApp.pComboBox22009->SetCurSel(0);

	CStatic* pStatic22010 = new CStatic;
	pStatic22010->Create(_T("End Blue Adder :"), WS_CHILD|WS_VISIBLE,
		CRect(35,170,148,190), this, 22010);
	pStatic22010->SetFont(GetFont());
	pStatic22010->ShowWindow(SW_HIDE);

	CEdit* pEdit22011 = new CEdit;
	pEdit22011->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,167,290,187), this, 22011);
	pEdit22011->SetFont(GetFont());
	pEdit22011->ShowWindow(SW_HIDE);
	pEdit22011->SetWindowText(m_sAnimationOutsideBlueAdder);

	CStatic* pStatic22012 = new CStatic;
	pStatic22012->Create(_T("Blue Add. Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,195,148,215), this, 22012);
	pStatic22012->SetFont(GetFont());
	pStatic22012->ShowWindow(SW_HIDE);

	theApp.pComboBox22013 = new CComboBox;
	theApp.pComboBox22013->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,192,290,212), this, 22013);
	theApp.pComboBox22013->SetFont(GetFont());
	theApp.pComboBox22013->ShowWindow(SW_HIDE);

	theApp.pComboBox22013->AddString(_T("Linear"));
	theApp.pComboBox22013->AddString(_T("Positive Quadratic"));
	theApp.pComboBox22013->AddString(_T("Negative Quadratic"));

	theApp.pComboBox22013->SetCurSel(0);

	CStatic* pStatic22014 = new CStatic;
	pStatic22014->Create(_T("End Red Mult. :"), WS_CHILD|WS_VISIBLE,
		CRect(35,225,148,245), this, 22014);
	pStatic22014->SetFont(GetFont());
	pStatic22014->ShowWindow(SW_HIDE);

	CEdit* pEdit22015 = new CEdit;
	pEdit22015->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,222,290,242), this, 22015);
	pEdit22015->SetFont(GetFont());
	pEdit22015->ShowWindow(SW_HIDE);
	pEdit22015->SetWindowText(m_sAnimationOutsideRedMult);

	CStatic* pStatic22016 = new CStatic;
	pStatic22016->Create(_T("Red Mult. Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,250,148,270), this, 22016);
	pStatic22016->SetFont(GetFont());
	pStatic22016->ShowWindow(SW_HIDE);

	theApp.pComboBox22017 = new CComboBox;
	theApp.pComboBox22017->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,247,290,267), this, 22017);
	theApp.pComboBox22017->SetFont(GetFont());
	theApp.pComboBox22017->ShowWindow(SW_HIDE);

	theApp.pComboBox22017->AddString(_T("Linear"));
	theApp.pComboBox22017->AddString(_T("Positive Quadratic"));
	theApp.pComboBox22017->AddString(_T("Negative Quadratic"));

	theApp.pComboBox22017->SetCurSel(0);

	CStatic* pStatic22018 = new CStatic;
	pStatic22018->Create(_T("End Green Mult. :"), WS_CHILD|WS_VISIBLE,
		CRect(35,280,148,300), this, 22018);
	pStatic22018->SetFont(GetFont());
	pStatic22018->ShowWindow(SW_HIDE);

	CEdit* pEdit22019 = new CEdit;
	pEdit22019->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,277,290,297), this, 22019);
	pEdit22019->SetFont(GetFont());
	pEdit22019->ShowWindow(SW_HIDE);
	pEdit22019->SetWindowText(m_sAnimationOutsideGreenMult);

	CStatic* pStatic22020 = new CStatic;
	pStatic22020->Create(_T("Green Mult. Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,305,148,325), this, 22020);
	pStatic22020->SetFont(GetFont());
	pStatic22020->ShowWindow(SW_HIDE);

	theApp.pComboBox22021 = new CComboBox;
	theApp.pComboBox22021->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,302,290,322), this, 22021);
	theApp.pComboBox22021->SetFont(GetFont());
	theApp.pComboBox22021->ShowWindow(SW_HIDE);

	theApp.pComboBox22021->AddString(_T("Linear"));
	theApp.pComboBox22021->AddString(_T("Positive Quadratic"));
	theApp.pComboBox22021->AddString(_T("Negative Quadratic"));

	theApp.pComboBox22021->SetCurSel(0);

	CStatic* pStatic22022 = new CStatic;
	pStatic22022->Create(_T("End Blue Mult. :"), WS_CHILD|WS_VISIBLE,
		CRect(35,335,148,355), this, 22022);
	pStatic22022->SetFont(GetFont());
	pStatic22022->ShowWindow(SW_HIDE);

	CEdit* pEdit22023 = new CEdit;
	pEdit22023->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,332,290,352), this, 22023);
	pEdit22023->SetFont(GetFont());
	pEdit22023->ShowWindow(SW_HIDE);
	pEdit22023->SetWindowText(m_sAnimationOutsideBlueMult);

	CStatic* pStatic22024 = new CStatic;
	pStatic22024->Create(_T("Blue Mult. Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,360,148,380), this, 22024);
	pStatic22024->SetFont(GetFont());
	pStatic22024->ShowWindow(SW_HIDE);

	theApp.pComboBox22025 = new CComboBox;
	theApp.pComboBox22025->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,357,290,377), this, 22025);
	theApp.pComboBox22025->SetFont(GetFont());
	theApp.pComboBox22025->ShowWindow(SW_HIDE);

	theApp.pComboBox22025->AddString(_T("Linear"));
	theApp.pComboBox22025->AddString(_T("Positive Quadratic"));
	theApp.pComboBox22025->AddString(_T("Negative Quadratic"));

	theApp.pComboBox22025->SetCurSel(0);
}

void CAnimationProperties::InitInsideTab()
{
	// Tab 4 - Inside Coloring

	CButton* pGroupBox23001 = new CButton;
	pGroupBox23001->Create(_T("Properties"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
		CRect(25,40,300,390) ,this, 23001);
	pGroupBox23001->SetFont(GetFont());
	pGroupBox23001->ShowWindow(SW_HIDE);

	CStatic* pStatic23002 = new CStatic;
	pStatic23002->Create(_T("End Red Adder :"), WS_CHILD|WS_VISIBLE,
		CRect(35,60,148,80), this, 23002);
	pStatic23002->SetFont(GetFont());
	pStatic23002->ShowWindow(SW_HIDE);

	CEdit* pEdit23003 = new CEdit;
	pEdit23003->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,57,290,77), this, 23003);
	pEdit23003->SetFont(GetFont());
	pEdit23003->ShowWindow(SW_HIDE);
	pEdit23003->SetWindowText(m_sAnimationInsideRedAdder);

	CStatic* pStatic23004 = new CStatic;
	pStatic23004->Create(_T("Red Add. Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,85,148,105), this, 23004);
	pStatic23004->SetFont(GetFont());
	pStatic23004->ShowWindow(SW_HIDE);

	theApp.pComboBox23005 = new CComboBox;
	theApp.pComboBox23005->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,82,290,102), this, 23005);
	theApp.pComboBox23005->SetFont(GetFont());
	theApp.pComboBox23005->ShowWindow(SW_HIDE);

	theApp.pComboBox23005->AddString(_T("Linear"));
	theApp.pComboBox23005->AddString(_T("Positive Quadratic"));
	theApp.pComboBox23005->AddString(_T("Negative Quadratic"));

	theApp.pComboBox23005->SetCurSel(0);

	CStatic* pStatic23006 = new CStatic;
	pStatic23006->Create(_T("End Green Adder :"), WS_CHILD|WS_VISIBLE,
		CRect(35,115,148,135), this, 23006);
	pStatic23006->SetFont(GetFont());
	pStatic23006->ShowWindow(SW_HIDE);

	CEdit* pEdit23007 = new CEdit;
	pEdit23007->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,112,290,132), this, 23007);
	pEdit23007->SetFont(GetFont());
	pEdit23007->ShowWindow(SW_HIDE);
	pEdit23007->SetWindowText(m_sAnimationInsideGreenAdder);

	CStatic* pStatic23008 = new CStatic;
	pStatic23008->Create(_T("Green Add. Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,140,148,160), this, 23008);
	pStatic23008->SetFont(GetFont());
	pStatic23008->ShowWindow(SW_HIDE);

	theApp.pComboBox23009 = new CComboBox;
	theApp.pComboBox23009->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,137,290,157), this, 23009);
	theApp.pComboBox23009->SetFont(GetFont());
	theApp.pComboBox23009->ShowWindow(SW_HIDE);
	
	theApp.pComboBox23009->AddString(_T("Linear"));
	theApp.pComboBox23009->AddString(_T("Positive Quadratic"));
	theApp.pComboBox23009->AddString(_T("Negative Quadratic"));

	theApp.pComboBox23009->SetCurSel(0);

	CStatic* pStatic23010 = new CStatic;
	pStatic23010->Create(_T("End Blue Adder :"), WS_CHILD|WS_VISIBLE,
		CRect(35,170,148,190), this, 23010);
	pStatic23010->SetFont(GetFont());
	pStatic23010->ShowWindow(SW_HIDE);

	CEdit* pEdit23011 = new CEdit;
	pEdit23011->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,167,290,187), this, 23011);
	pEdit23011->SetFont(GetFont());
	pEdit23011->ShowWindow(SW_HIDE);
	pEdit23011->SetWindowText(m_sAnimationInsideBlueAdder);

	CStatic* pStatic23012 = new CStatic;
	pStatic23012->Create(_T("Blue Add. Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,195,148,215), this, 23012);
	pStatic23012->SetFont(GetFont());
	pStatic23012->ShowWindow(SW_HIDE);

	theApp.pComboBox23013 = new CComboBox;
	theApp.pComboBox23013->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,192,290,212), this, 23013);
	theApp.pComboBox23013->SetFont(GetFont());
	theApp.pComboBox23013->ShowWindow(SW_HIDE);

	theApp.pComboBox23013->AddString(_T("Linear"));
	theApp.pComboBox23013->AddString(_T("Positive Quadratic"));
	theApp.pComboBox23013->AddString(_T("Negative Quadratic"));

	theApp.pComboBox23013->SetCurSel(0);

	CStatic* pStatic23014 = new CStatic;
	pStatic23014->Create(_T("End Red Mult. :"), WS_CHILD|WS_VISIBLE,
		CRect(35,225,148,245), this, 23014);
	pStatic23014->SetFont(GetFont());
	pStatic23014->ShowWindow(SW_HIDE);

	CEdit* pEdit23015 = new CEdit;
	pEdit23015->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,222,290,242), this, 23015);
	pEdit23015->SetFont(GetFont());
	pEdit23015->ShowWindow(SW_HIDE);
	pEdit23015->SetWindowText(m_sAnimationInsideRedMult);

	CStatic* pStatic23016 = new CStatic;
	pStatic23016->Create(_T("Red Mult. Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,250,148,270), this, 23016);
	pStatic23016->SetFont(GetFont());
	pStatic23016->ShowWindow(SW_HIDE);

	theApp.pComboBox23017 = new CComboBox;
	theApp.pComboBox23017->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,247,290,267), this, 23017);
	theApp.pComboBox23017->SetFont(GetFont());
	theApp.pComboBox23017->ShowWindow(SW_HIDE);

	theApp.pComboBox23017->AddString(_T("Linear"));
	theApp.pComboBox23017->AddString(_T("Positive Quadratic"));
	theApp.pComboBox23017->AddString(_T("Negative Quadratic"));

	theApp.pComboBox23017->SetCurSel(0);

	CStatic* pStatic23018 = new CStatic;
	pStatic23018->Create(_T("End Green Mult. :"), WS_CHILD|WS_VISIBLE,
		CRect(35,280,148,300), this, 23018);
	pStatic23018->SetFont(GetFont());
	pStatic23018->ShowWindow(SW_HIDE);

	CEdit* pEdit23019 = new CEdit;
	pEdit23019->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,277,290,297), this, 23019);
	pEdit23019->SetFont(GetFont());
	pEdit23019->ShowWindow(SW_HIDE);
	pEdit23019->SetWindowText(m_sAnimationInsideGreenMult);

	CStatic* pStatic23020 = new CStatic;
	pStatic23020->Create(_T("Green Mult. Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,305,148,325), this, 23020);
	pStatic23020->SetFont(GetFont());
	pStatic23020->ShowWindow(SW_HIDE);

	theApp.pComboBox23021 = new CComboBox;
	theApp.pComboBox23021->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,302,290,322), this, 23021);
	theApp.pComboBox23021->SetFont(GetFont());
	theApp.pComboBox23021->ShowWindow(SW_HIDE);

	theApp.pComboBox23021->AddString(_T("Linear"));
	theApp.pComboBox23021->AddString(_T("Positive Quadratic"));
	theApp.pComboBox23021->AddString(_T("Negative Quadratic"));

	theApp.pComboBox23021->SetCurSel(0);

	CStatic* pStatic23022 = new CStatic;
	pStatic23022->Create(_T("End Blue Mult. :"), WS_CHILD|WS_VISIBLE,
		CRect(35,335,148,355), this, 23022);
	pStatic23022->SetFont(GetFont());
	pStatic23022->ShowWindow(SW_HIDE);

	CEdit* pEdit23023 = new CEdit;
	pEdit23023->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(150,332,290,352), this, 23023);
	pEdit23023->SetFont(GetFont());
	pEdit23023->ShowWindow(SW_HIDE);
	pEdit23023->SetWindowText(m_sAnimationInsideBlueMult);

	CStatic* pStatic23024 = new CStatic;
	pStatic23024->Create(_T("Blue Mult. Function :"), WS_CHILD|WS_VISIBLE,
		CRect(35,360,148,380), this, 23024);
	pStatic23024->SetFont(GetFont());
	pStatic23024->ShowWindow(SW_HIDE);

	theApp.pComboBox23025 = new CComboBox;
	theApp.pComboBox23025->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWN,
		CRect(150,357,290,377), this, 23025);
	theApp.pComboBox23025->SetFont(GetFont());
	theApp.pComboBox23025->ShowWindow(SW_HIDE);

	theApp.pComboBox23025->AddString(_T("Linear"));
	theApp.pComboBox23025->AddString(_T("Positive Quadratic"));
	theApp.pComboBox23025->AddString(_T("Negative Quadratic"));

	theApp.pComboBox23025->SetCurSel(0);
}

void CAnimationProperties::InitFormulaControls()
{
	int iFractalSelection = theApp.pComboBox10003->GetCurSel();


	if(iFractalSelection == 0)	// Mandelbrot
	{
		GetDlgItem(21002)->SetWindowTextW(_T("End X-Start :"));
		GetDlgItem(21004)->SetWindowTextW(_T("X-Start Function :"));
		GetDlgItem(21006)->SetWindowTextW(_T("End Y-Start :"));
		GetDlgItem(21008)->SetWindowTextW(_T("Y-Start Function :"));
		GetDlgItem(21010)->SetWindowTextW(_T("End Z-Exponent :"));
		GetDlgItem(21012)->SetWindowTextW(_T("Z-Exponent Function :"));

		for(int count = 21014; count <= 21025; count++)
		{
			GetDlgItem(count)->EnableWindow(FALSE);
		}

		if(theApp.m_bInitAnimationFormula == TRUE)
		{
			CString sControl1;
			sControl1.Format(_T("%lf"), theApp.m_dControl1);
			sControl1 = RemoveZeros(sControl1);
			GetDlgItem(21003)->SetWindowTextW(sControl1);
			theApp.pComboBox21005->SetCurSel(0);

			CString sControl2;
			sControl2.Format(_T("%lf"), theApp.m_dControl2);
			sControl2 = RemoveZeros(sControl2);
			GetDlgItem(21007)->SetWindowTextW(sControl2);
			theApp.pComboBox21009->SetCurSel(0);

			CString sControl3;
			sControl3.Format(_T("%lf"), theApp.m_dControl3);
			sControl3 = RemoveZeros(sControl3);
			GetDlgItem(21011)->SetWindowTextW(sControl3);
			theApp.pComboBox21013->SetCurSel(0);

			GetDlgItem(21015)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21017->SetCurSel(0);
			GetDlgItem(21019)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21021->SetCurSel(0);
			GetDlgItem(21023)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21025->SetCurSel(0);

			for(int count = 0; count < 8; count++)
			{
				theApp.m_dAnimationControl1[count] = theApp.m_dControl1;
				theApp.m_iAnimationControl1Function[count] = 0;
				theApp.m_dAnimationControl2[count] = theApp.m_dControl2;
				theApp.m_iAnimationControl2Function[count] = 0;
				theApp.m_dAnimationControl3[count] = theApp.m_dControl3;
				theApp.m_iAnimationControl3Function[count] = 0;
				theApp.m_dAnimationControl4[count] = 0.0;
				theApp.m_iAnimationControl4Function[count] = 0;
				theApp.m_dAnimationControl5[count] = 0.0;
				theApp.m_iAnimationControl5Function[count] = 0;
				theApp.m_dAnimationControl6[count] = 0.0;
				theApp.m_iAnimationControl6Function[count] = 0;
			}

			theApp.m_bInitAnimationFormula = FALSE;
		}
	}

	else if(iFractalSelection == 1)	// Mandelbrot Quick
	{
		GetDlgItem(21002)->SetWindowTextW(_T("End X-Start :"));
		GetDlgItem(21004)->SetWindowTextW(_T("X-Start Function :"));
		GetDlgItem(21006)->SetWindowTextW(_T("End Y-Start :"));
		GetDlgItem(21008)->SetWindowTextW(_T("Y-Start Function :"));

		for(int count = 21010; count <= 21025; count++)
		{
			GetDlgItem(count)->EnableWindow(FALSE);
		}

		if(theApp.m_bInitAnimationFormula == TRUE)
		{
			CString sControl1;
			sControl1.Format(_T("%lf"), theApp.m_dControl1);
			sControl1 = RemoveZeros(sControl1);
			GetDlgItem(21003)->SetWindowTextW(sControl1);
			theApp.pComboBox21005->SetCurSel(0);

			CString sControl2;
			sControl2.Format(_T("%lf"), theApp.m_dControl2);
			sControl2 = RemoveZeros(sControl2);
			GetDlgItem(21007)->SetWindowTextW(sControl2);
			theApp.pComboBox21009->SetCurSel(0);

			GetDlgItem(21011)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21013->SetCurSel(0);
			GetDlgItem(21015)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21017->SetCurSel(0);
			GetDlgItem(21019)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21021->SetCurSel(0);
			GetDlgItem(21023)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21025->SetCurSel(0);

			for(int count = 0; count < 8; count++)
			{
				theApp.m_dAnimationControl1[count] = theApp.m_dControl1;
				theApp.m_iAnimationControl1Function[count] = 0;
				theApp.m_dAnimationControl2[count] = theApp.m_dControl2;
				theApp.m_iAnimationControl2Function[count] = 0;
				theApp.m_dAnimationControl3[count] = 0.0;
				theApp.m_iAnimationControl3Function[count] = 0;
				theApp.m_dAnimationControl4[count] = 0.0;
				theApp.m_iAnimationControl4Function[count] = 0;
				theApp.m_dAnimationControl5[count] = 0.0;
				theApp.m_iAnimationControl5Function[count] = 0;
				theApp.m_dAnimationControl6[count] = 0.0;
				theApp.m_iAnimationControl6Function[count] = 0;
			}

			theApp.m_bInitAnimationFormula = FALSE;
		}
	}

	else if(iFractalSelection == 2)	// Julia
	{
		GetDlgItem(21002)->SetWindowTextW(_T("End Seed (Re) :"));
		GetDlgItem(21004)->SetWindowTextW(_T("Seed (Re) Function :"));
		GetDlgItem(21006)->SetWindowTextW(_T("End Seed (Im) :"));
		GetDlgItem(21008)->SetWindowTextW(_T("Seed (Im) Function :"));
		GetDlgItem(21010)->SetWindowTextW(_T("End Z-Exponent :"));
		GetDlgItem(21012)->SetWindowTextW(_T("Z-Exponent Function :"));

		for(int count = 21014; count <= 21025; count++)
		{
			GetDlgItem(count)->EnableWindow(FALSE);
		}

		if(theApp.m_bInitAnimationFormula == TRUE)
		{
			CString sControl1;
			sControl1.Format(_T("%lf"), theApp.m_dControl1);
			sControl1 = RemoveZeros(sControl1);
			GetDlgItem(21003)->SetWindowTextW(sControl1);
			theApp.pComboBox21005->SetCurSel(0);

			CString sControl2;
			sControl2.Format(_T("%lf"), theApp.m_dControl2);
			sControl2 = RemoveZeros(sControl2);
			GetDlgItem(21007)->SetWindowTextW(sControl2);
			theApp.pComboBox21009->SetCurSel(0);

			CString sControl3;
			sControl3.Format(_T("%lf"), theApp.m_dControl3);
			sControl3 = RemoveZeros(sControl3);
			GetDlgItem(21011)->SetWindowTextW(sControl3);
			theApp.pComboBox21013->SetCurSel(0);

			GetDlgItem(21015)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21017->SetCurSel(0);
			GetDlgItem(21019)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21021->SetCurSel(0);
			GetDlgItem(21023)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21025->SetCurSel(0);

			for(int count = 0; count < 8; count++)
			{
				theApp.m_dAnimationControl1[count] = theApp.m_dControl1;
				theApp.m_iAnimationControl1Function[count] = 0;
				theApp.m_dAnimationControl2[count] = theApp.m_dControl2;
				theApp.m_iAnimationControl2Function[count] = 0;
				theApp.m_dAnimationControl3[count] = theApp.m_dControl3;
				theApp.m_iAnimationControl3Function[count] = 0;
				theApp.m_dAnimationControl4[count] = 0.0;
				theApp.m_iAnimationControl4Function[count] = 0;
				theApp.m_dAnimationControl5[count] = 0.0;
				theApp.m_iAnimationControl5Function[count] = 0;
				theApp.m_dAnimationControl6[count] = 0.0;
				theApp.m_iAnimationControl6Function[count] = 0;
			}

			theApp.m_bInitAnimationFormula = FALSE;
		}
	}

	else if(iFractalSelection == 3)	// Julia Quick
	{
		GetDlgItem(21002)->SetWindowTextW(_T("End Seed (Re) :"));
		GetDlgItem(21004)->SetWindowTextW(_T("Seed (Re) Function :"));
		GetDlgItem(21006)->SetWindowTextW(_T("End Seed (Im) :"));
		GetDlgItem(21008)->SetWindowTextW(_T("Seed (Im) Function :"));

		for(int count = 21010; count <= 21025; count++)
		{
			GetDlgItem(count)->EnableWindow(FALSE);
		}

		if(theApp.m_bInitAnimationFormula == TRUE)
		{
			CString sControl1;
			sControl1.Format(_T("%lf"), theApp.m_dControl1);
			sControl1 = RemoveZeros(sControl1);
			GetDlgItem(21003)->SetWindowTextW(sControl1);
			theApp.pComboBox21005->SetCurSel(0);

			CString sControl2;
			sControl2.Format(_T("%lf"), theApp.m_dControl2);
			sControl2 = RemoveZeros(sControl2);
			GetDlgItem(21007)->SetWindowTextW(sControl2);
			theApp.pComboBox21009->SetCurSel(0);

			GetDlgItem(21011)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21013->SetCurSel(0);
			GetDlgItem(21015)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21017->SetCurSel(0);
			GetDlgItem(21019)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21021->SetCurSel(0);
			GetDlgItem(21023)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21025->SetCurSel(0);

			for(int count = 0; count < 8; count++)
			{
				theApp.m_dAnimationControl1[count] = theApp.m_dControl1;
				theApp.m_iAnimationControl1Function[count] = 0;
				theApp.m_dAnimationControl2[count] = theApp.m_dControl2;
				theApp.m_iAnimationControl2Function[count] = 0;
				theApp.m_dAnimationControl3[count] = 0.0;
				theApp.m_iAnimationControl3Function[count] = 0;
				theApp.m_dAnimationControl4[count] = 0.0;
				theApp.m_iAnimationControl4Function[count] = 0;
				theApp.m_dAnimationControl5[count] = 0.0;
				theApp.m_iAnimationControl5Function[count] = 0;
				theApp.m_dAnimationControl6[count] = 0.0;
				theApp.m_iAnimationControl6Function[count] = 0;
			}

			theApp.m_bInitAnimationFormula = FALSE;
		}
	}

	else if(iFractalSelection == 4)	// Newton
	{
		GetDlgItem(21002)->SetWindowTextW(_T("End Relax (Re) :"));
		GetDlgItem(21004)->SetWindowTextW(_T("Relax (Re) Function :"));
		GetDlgItem(21006)->SetWindowTextW(_T("End Relax (Im) :"));
		GetDlgItem(21008)->SetWindowTextW(_T("Relax (Im) Function :"));
		GetDlgItem(21010)->SetWindowTextW(_T("End Z-Power :"));
		GetDlgItem(21012)->SetWindowTextW(_T("Z-Power Function :"));

		for(int count = 21014; count <= 21025; count++)
		{
			GetDlgItem(count)->EnableWindow(FALSE);
		}

		if(theApp.m_bInitAnimationFormula == TRUE)
		{
			CString sControl1;
			sControl1.Format(_T("%lf"), theApp.m_dControl1);
			sControl1 = RemoveZeros(sControl1);
			GetDlgItem(21003)->SetWindowTextW(sControl1);
			theApp.pComboBox21005->SetCurSel(0);

			CString sControl2;
			sControl2.Format(_T("%lf"), theApp.m_dControl2);
			sControl2 = RemoveZeros(sControl2);
			GetDlgItem(21007)->SetWindowTextW(sControl2);
			theApp.pComboBox21009->SetCurSel(0);

			CString sControl3;
			sControl3.Format(_T("%lf"), theApp.m_dControl3);
			sControl3 = RemoveZeros(sControl3);
			GetDlgItem(21011)->SetWindowTextW(sControl3);
			theApp.pComboBox21013->SetCurSel(0);

			GetDlgItem(21015)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21017->SetCurSel(0);
			GetDlgItem(21019)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21021->SetCurSel(0);
			GetDlgItem(21023)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21025->SetCurSel(0);

			for(int count = 0; count < 8; count++)
			{
				theApp.m_dAnimationControl1[count] = theApp.m_dControl1;
				theApp.m_iAnimationControl1Function[count] = 0;
				theApp.m_dAnimationControl2[count] = theApp.m_dControl2;
				theApp.m_iAnimationControl2Function[count] = 0;
				theApp.m_dAnimationControl3[count] = theApp.m_dControl3;
				theApp.m_iAnimationControl3Function[count] = 0;
				theApp.m_dAnimationControl4[count] = 0.0;
				theApp.m_iAnimationControl4Function[count] = 0;
				theApp.m_dAnimationControl5[count] = 0.0;
				theApp.m_iAnimationControl5Function[count] = 0;
				theApp.m_dAnimationControl6[count] = 0.0;
				theApp.m_iAnimationControl6Function[count] = 0;
			}

			theApp.m_bInitAnimationFormula = FALSE;
		}
	}

	else if(iFractalSelection == 5)	// Newton (Root)
	{
		GetDlgItem(21002)->SetWindowTextW(_T("End Relax (Re) :"));
		GetDlgItem(21004)->SetWindowTextW(_T("Relax (Re) Function :"));
		GetDlgItem(21006)->SetWindowTextW(_T("End Relax (Im) :"));
		GetDlgItem(21008)->SetWindowTextW(_T("Relax (Im) Function :"));
		GetDlgItem(21010)->SetWindowTextW(_T("End Tolerance :"));
		GetDlgItem(21012)->SetWindowTextW(_T("Tolerance Function :"));

		for(int count = 21014; count <= 21025; count++)
		{
			GetDlgItem(count)->EnableWindow(FALSE);
		}

		if(theApp.m_bInitAnimationFormula == TRUE)
		{
			CString sControl1;
			sControl1.Format(_T("%lf"), theApp.m_dControl1);
			sControl1 = RemoveZeros(sControl1);
			GetDlgItem(21003)->SetWindowTextW(sControl1);
			theApp.pComboBox21005->SetCurSel(0);

			CString sControl2;
			sControl2.Format(_T("%lf"), theApp.m_dControl2);
			sControl2 = RemoveZeros(sControl2);
			GetDlgItem(21007)->SetWindowTextW(sControl2);
			theApp.pComboBox21009->SetCurSel(0);

			CString sControl3;
			sControl3.Format(_T("%lf"), theApp.m_dControl3);
			sControl3 = RemoveZeros(sControl3);
			GetDlgItem(21011)->SetWindowTextW(sControl3);
			theApp.pComboBox21013->SetCurSel(0);

			GetDlgItem(21015)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21017->SetCurSel(0);
			GetDlgItem(21019)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21021->SetCurSel(0);
			GetDlgItem(21023)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21025->SetCurSel(0);

			for(int count = 0; count < 8; count++)
			{
				theApp.m_dAnimationControl1[count] = theApp.m_dControl1;
				theApp.m_iAnimationControl1Function[count] = 0;
				theApp.m_dAnimationControl2[count] = theApp.m_dControl2;
				theApp.m_iAnimationControl2Function[count] = 0;
				theApp.m_dAnimationControl3[count] = theApp.m_dControl3;
				theApp.m_iAnimationControl3Function[count] = 0;
				theApp.m_dAnimationControl4[count] = 0.0;
				theApp.m_iAnimationControl4Function[count] = 0;
				theApp.m_dAnimationControl5[count] = 0.0;
				theApp.m_iAnimationControl5Function[count] = 0;
				theApp.m_dAnimationControl6[count] = 0.0;
				theApp.m_iAnimationControl6Function[count] = 0;
			}

			theApp.m_bInitAnimationFormula = FALSE;
		}
	}

	else if(iFractalSelection == 6)	// Nova
	{
		GetDlgItem(21002)->SetWindowTextW(_T("End Seed (Re) :"));
		GetDlgItem(21004)->SetWindowTextW(_T("Seed (Re) Function :"));
		GetDlgItem(21006)->SetWindowTextW(_T("End Seed (Im) :"));
		GetDlgItem(21008)->SetWindowTextW(_T("Seed (Im) Function :"));
		GetDlgItem(21010)->SetWindowTextW(_T("End Relax (Re) :"));
		GetDlgItem(21012)->SetWindowTextW(_T("Relax (Re) Function :"));
		GetDlgItem(21014)->SetWindowTextW(_T("End Relax (Im) :"));
		GetDlgItem(21016)->SetWindowTextW(_T("Relax (Im) Function :"));
		GetDlgItem(21018)->SetWindowTextW(_T("End Z-Power :"));
		GetDlgItem(21020)->SetWindowTextW(_T("Z-Power Function :"));
		GetDlgItem(21022)->SetWindowTextW(_T("End Deriv. Mult. :"));
		GetDlgItem(21024)->SetWindowTextW(_T("Deriv. Mult. Function :"));

		if(theApp.m_bInitAnimationFormula == TRUE)
		{
			CString sControl1;
			sControl1.Format(_T("%lf"), theApp.m_dControl1);
			sControl1 = RemoveZeros(sControl1);
			GetDlgItem(21003)->SetWindowTextW(sControl1);
			theApp.pComboBox21005->SetCurSel(0);

			CString sControl2;
			sControl2.Format(_T("%lf"), theApp.m_dControl2);
			sControl2 = RemoveZeros(sControl2);
			GetDlgItem(21007)->SetWindowTextW(sControl2);
			theApp.pComboBox21009->SetCurSel(0);

			CString sControl3;
			sControl3.Format(_T("%lf"), theApp.m_dControl3);
			sControl3 = RemoveZeros(sControl3);
			GetDlgItem(21011)->SetWindowTextW(sControl3);
			theApp.pComboBox21013->SetCurSel(0);

			CString sControl4;
			sControl4.Format(_T("%lf"), theApp.m_dControl4);
			sControl4 = RemoveZeros(sControl4);
			GetDlgItem(21015)->SetWindowTextW(sControl4);
			theApp.pComboBox21017->SetCurSel(0);

			CString sControl5;
			sControl5.Format(_T("%lf"), theApp.m_dControl5);
			sControl5 = RemoveZeros(sControl5);
			GetDlgItem(21019)->SetWindowTextW(sControl5);
			theApp.pComboBox21021->SetCurSel(0);

			CString sControl6;
			sControl6.Format(_T("%lf"), theApp.m_dControl6);
			sControl6 = RemoveZeros(sControl6);
			GetDlgItem(21023)->SetWindowTextW(sControl6);
			theApp.pComboBox21025->SetCurSel(0);

			for(int count = 0; count < 8; count++)
			{
				theApp.m_dAnimationControl1[count] = theApp.m_dControl1;
				theApp.m_iAnimationControl1Function[count] = 0;
				theApp.m_dAnimationControl2[count] = theApp.m_dControl2;
				theApp.m_iAnimationControl2Function[count] = 0;
				theApp.m_dAnimationControl3[count] = theApp.m_dControl3;
				theApp.m_iAnimationControl3Function[count] = 0;
				theApp.m_dAnimationControl4[count] = -theApp.m_dControl4;
				theApp.m_iAnimationControl4Function[count] = 0;
				theApp.m_dAnimationControl5[count] = theApp.m_dControl5;
				theApp.m_iAnimationControl5Function[count] = 0;
				theApp.m_dAnimationControl6[count] = theApp.m_dControl6;
				theApp.m_iAnimationControl6Function[count] = 0;
			}

			theApp.m_bInitAnimationFormula = FALSE;
		}
	}

	else if(iFractalSelection == 7)	// Nova (Special)
	{
		GetDlgItem(21002)->SetWindowTextW(_T("End Relax (Re) :"));
		GetDlgItem(21004)->SetWindowTextW(_T("Relax (Re) Function :"));
		GetDlgItem(21006)->SetWindowTextW(_T("End Relax (Im) :"));
		GetDlgItem(21008)->SetWindowTextW(_T("Relax (Im) Function :"));
		GetDlgItem(21010)->SetWindowTextW(_T("End Z-Power :"));
		GetDlgItem(21012)->SetWindowTextW(_T("Z-Power Function :"));
		GetDlgItem(21014)->SetWindowTextW(_T("End Deriv. Mult. :"));
		GetDlgItem(21016)->SetWindowTextW(_T("Deriv. Mult. Function :"));

		for(int count = 21018; count <= 21025; count++)
		{
			GetDlgItem(count)->EnableWindow(FALSE);
		}

		if(theApp.m_bInitAnimationFormula == TRUE)
		{
			CString sControl1;
			sControl1.Format(_T("%lf"), theApp.m_dControl1);
			sControl1 = RemoveZeros(sControl1);
			GetDlgItem(21003)->SetWindowTextW(sControl1);
			theApp.pComboBox21005->SetCurSel(0);

			CString sControl2;
			sControl2.Format(_T("%lf"), theApp.m_dControl2);
			sControl2 = RemoveZeros(sControl2);
			GetDlgItem(21007)->SetWindowTextW(sControl2);
			theApp.pComboBox21009->SetCurSel(0);

			CString sControl3;
			sControl3.Format(_T("%lf"), theApp.m_dControl3);
			sControl3 = RemoveZeros(sControl3);
			GetDlgItem(21011)->SetWindowTextW(sControl3);
			theApp.pComboBox21013->SetCurSel(0);

			CString sControl4;
			sControl4.Format(_T("%lf"), theApp.m_dControl4);
			sControl4 = RemoveZeros(sControl4);
			GetDlgItem(21015)->SetWindowTextW(sControl4);
			theApp.pComboBox21017->SetCurSel(0);

			GetDlgItem(21019)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21021->SetCurSel(0);
			GetDlgItem(21023)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21025->SetCurSel(0);

			for(int count = 0; count < 8; count++)
			{
				theApp.m_dAnimationControl1[count] = theApp.m_dControl1;
				theApp.m_iAnimationControl1Function[count] = 0;
				theApp.m_dAnimationControl2[count] = -theApp.m_dControl2;
				theApp.m_iAnimationControl2Function[count] = 0;
				theApp.m_dAnimationControl3[count] = theApp.m_dControl3;
				theApp.m_iAnimationControl3Function[count] = 0;
				theApp.m_dAnimationControl4[count] = theApp.m_dControl4;
				theApp.m_iAnimationControl4Function[count] = 0;
				theApp.m_dAnimationControl5[count] = 0.0;
				theApp.m_iAnimationControl5Function[count] = 0;
				theApp.m_dAnimationControl6[count] = 0.0;
				theApp.m_iAnimationControl6Function[count] = 0;
			}

			theApp.m_bInitAnimationFormula = FALSE;
		}
	}

	else if(iFractalSelection == 8)	// Glynn
	{
		GetDlgItem(21002)->SetWindowTextW(_T("End Seed (Re) :"));
		GetDlgItem(21004)->SetWindowTextW(_T("Seed (Re) Function :"));
		GetDlgItem(21006)->SetWindowTextW(_T("End Seed (Im) :"));
		GetDlgItem(21008)->SetWindowTextW(_T("Seed (Im) Function :"));
		GetDlgItem(21010)->SetWindowTextW(_T("End Z-Power :"));
		GetDlgItem(21012)->SetWindowTextW(_T("Z-Power Function :"));

		for(int count = 21014; count <= 21025; count++)
		{
			GetDlgItem(count)->EnableWindow(FALSE);
		}

		if(theApp.m_bInitAnimationFormula == TRUE)
		{
			CString sControl1;
			sControl1.Format(_T("%lf"), theApp.m_dControl1);
			sControl1 = RemoveZeros(sControl1);
			GetDlgItem(21003)->SetWindowTextW(sControl1);
			theApp.pComboBox21005->SetCurSel(0);

			CString sControl2;
			sControl2.Format(_T("%lf"), theApp.m_dControl2);
			sControl2 = RemoveZeros(sControl2);
			GetDlgItem(21007)->SetWindowTextW(sControl2);
			theApp.pComboBox21009->SetCurSel(0);

			CString sControl3;
			sControl3.Format(_T("%lf"), theApp.m_dControl3);
			sControl3 = RemoveZeros(sControl3);
			GetDlgItem(21011)->SetWindowTextW(sControl3);
			theApp.pComboBox21013->SetCurSel(0);

			GetDlgItem(21015)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21017->SetCurSel(0);
			GetDlgItem(21019)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21021->SetCurSel(0);
			GetDlgItem(21023)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21025->SetCurSel(0);

			for(int count = 0; count < 8; count++)
			{
				theApp.m_dAnimationControl1[count] = theApp.m_dControl1;
				theApp.m_iAnimationControl1Function[count] = 0;
				theApp.m_dAnimationControl2[count] = theApp.m_dControl2;
				theApp.m_iAnimationControl2Function[count] = 0;
				theApp.m_dAnimationControl3[count] = theApp.m_dControl3;
				theApp.m_iAnimationControl3Function[count] = 0;
				theApp.m_dAnimationControl4[count] = 0.0;
				theApp.m_iAnimationControl4Function[count] = 0;
				theApp.m_dAnimationControl5[count] = 0.0;
				theApp.m_iAnimationControl5Function[count] = 0;
				theApp.m_dAnimationControl6[count] = 0.0;
				theApp.m_iAnimationControl6Function[count] = 0;
			}

			theApp.m_bInitAnimationFormula = FALSE;
		}
	}

	else if(iFractalSelection == 9)	// Phoenix
	{
		GetDlgItem(21002)->SetWindowTextW(_T("End P1 (Re) :"));
		GetDlgItem(21004)->SetWindowTextW(_T("P1 (Re) Function :"));
		GetDlgItem(21006)->SetWindowTextW(_T("End P1 (Im) :"));
		GetDlgItem(21008)->SetWindowTextW(_T("P1 (Im) Function :"));
		GetDlgItem(21010)->SetWindowTextW(_T("End P2 (Re) :"));
		GetDlgItem(21012)->SetWindowTextW(_T("P2 (Re) Function :"));
		GetDlgItem(21014)->SetWindowTextW(_T("End P2 (Im) :"));
		GetDlgItem(21016)->SetWindowTextW(_T("P2 (Im) Function :"));
		GetDlgItem(21018)->SetWindowTextW(_T("End Z-Power :"));
		GetDlgItem(21020)->SetWindowTextW(_T("Z-Power Function :"));

		for(int count = 21022; count <= 21025; count++)
		{
			GetDlgItem(count)->EnableWindow(FALSE);
		}

		if(theApp.m_bInitAnimationFormula == TRUE)
		{
			CString sControl1;
			sControl1.Format(_T("%lf"), theApp.m_dControl1);
			sControl1 = RemoveZeros(sControl1);
			GetDlgItem(21003)->SetWindowTextW(sControl1);
			theApp.pComboBox21005->SetCurSel(0);

			CString sControl2;
			sControl2.Format(_T("%lf"), theApp.m_dControl2);
			sControl2 = RemoveZeros(sControl2);
			GetDlgItem(21007)->SetWindowTextW(sControl2);
			theApp.pComboBox21009->SetCurSel(0);

			CString sControl3;
			sControl3.Format(_T("%lf"), theApp.m_dControl3);
			sControl3 = RemoveZeros(sControl3);
			GetDlgItem(21011)->SetWindowTextW(sControl3);
			theApp.pComboBox21013->SetCurSel(0);

			CString sControl4;
			sControl4.Format(_T("%lf"), theApp.m_dControl4);
			sControl4 = RemoveZeros(sControl4);
			GetDlgItem(21015)->SetWindowTextW(sControl4);
			theApp.pComboBox21017->SetCurSel(0);

			CString sControl5;
			sControl5.Format(_T("%lf"), theApp.m_dControl5);
			sControl5 = RemoveZeros(sControl5);
			GetDlgItem(21019)->SetWindowTextW(sControl5);
			theApp.pComboBox21021->SetCurSel(0);

			GetDlgItem(21023)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21025->SetCurSel(0);

			for(int count = 0; count < 8; count++)
			{
				theApp.m_dAnimationControl1[count] = theApp.m_dControl1;
				theApp.m_iAnimationControl1Function[count] = 0;
				theApp.m_dAnimationControl2[count] = theApp.m_dControl2;
				theApp.m_iAnimationControl2Function[count] = 0;
				theApp.m_dAnimationControl3[count] = theApp.m_dControl3;
				theApp.m_iAnimationControl3Function[count] = 0;
				theApp.m_dAnimationControl4[count] = theApp.m_dControl4;
				theApp.m_iAnimationControl4Function[count] = 0;
				theApp.m_dAnimationControl5[count] = theApp.m_dControl5;
				theApp.m_iAnimationControl5Function[count] = 0;
				theApp.m_dAnimationControl6[count] = 0.0;
				theApp.m_iAnimationControl6Function[count] = 0;
			}

			theApp.m_bInitAnimationFormula = FALSE;
		}
	}

	else if(iFractalSelection == 10)	// Cos(Tan(z))
	{
		GetDlgItem(21002)->SetWindowTextW(_T("End Seed (Re) :"));
		GetDlgItem(21004)->SetWindowTextW(_T("Seed (Re) Function :"));
		GetDlgItem(21006)->SetWindowTextW(_T("End Seed (Im) :"));
		GetDlgItem(21008)->SetWindowTextW(_T("Seed (Im) Function :"));

		for(int count = 21010; count <= 21025; count++)
		{
			GetDlgItem(count)->EnableWindow(FALSE);
		}

		if(theApp.m_bInitAnimationFormula == TRUE)
		{
			CString sControl1;
			sControl1.Format(_T("%lf"), theApp.m_dControl1);
			sControl1 = RemoveZeros(sControl1);
			GetDlgItem(21003)->SetWindowTextW(sControl1);
			theApp.pComboBox21005->SetCurSel(0);

			CString sControl2;
			sControl2.Format(_T("%lf"), theApp.m_dControl2);
			sControl2 = RemoveZeros(sControl2);
			GetDlgItem(21007)->SetWindowTextW(sControl2);
			theApp.pComboBox21009->SetCurSel(0);

			GetDlgItem(21011)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21013->SetCurSel(0);
			GetDlgItem(21015)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21017->SetCurSel(0);
			GetDlgItem(21019)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21021->SetCurSel(0);
			GetDlgItem(21023)->SetWindowTextW(_T("N/A"));
			theApp.pComboBox21025->SetCurSel(0);

			for(int count = 0; count < 8; count++)
			{
				theApp.m_dAnimationControl1[count] = theApp.m_dControl1;
				theApp.m_iAnimationControl1Function[count] = 0;
				theApp.m_dAnimationControl2[count] = theApp.m_dControl2;
				theApp.m_iAnimationControl2Function[count] = 0;
				theApp.m_dAnimationControl3[count] = 0.0;
				theApp.m_iAnimationControl3Function[count] = 0;
				theApp.m_dAnimationControl4[count] = 0.0;
				theApp.m_iAnimationControl4Function[count] = 0;
				theApp.m_dAnimationControl5[count] = 0.0;
				theApp.m_iAnimationControl5Function[count] = 0;
				theApp.m_dAnimationControl6[count] = 0.0;
				theApp.m_iAnimationControl6Function[count] = 0;
			}

			theApp.m_bInitAnimationFormula = FALSE;
		}
	}

}

void CAnimationProperties::OnTcnSelchangeTabcontrol(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnChangeTab();
}

void CAnimationProperties::OnChangeTab()
{
	if(m_cAnimationTab.GetCurFocus() == 0)
	{
		for(int nIDCount = 20001; nIDCount <= 20025; nIDCount++)
		{
			GetDlgItem(nIDCount)->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		for(int nIDCount = 20001; nIDCount <= 20025; nIDCount++)
		{
			GetDlgItem(nIDCount)->ShowWindow(SW_HIDE);
		}
	}

	if(m_cAnimationTab.GetCurFocus() == 1)
	{
		for(int nIDCount = 21001; nIDCount <= 21025; nIDCount++)
		{
			GetDlgItem(nIDCount)->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		for(int nIDCount = 21001; nIDCount <= 21025; nIDCount++)
		{
			GetDlgItem(nIDCount)->ShowWindow(SW_HIDE);
		}
	}

	if(m_cAnimationTab.GetCurFocus() == 2)
	{
		for(int nIDCount = 22001; nIDCount <= 22025; nIDCount++)
		{
			GetDlgItem(nIDCount)->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		for(int nIDCount = 22001; nIDCount <= 22025; nIDCount++)
		{
			GetDlgItem(nIDCount)->ShowWindow(SW_HIDE);
		}
	}

	if(m_cAnimationTab.GetCurFocus() == 3)
	{
		for(int nIDCount = 23001; nIDCount <= 23025; nIDCount++)
		{
			GetDlgItem(nIDCount)->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		for(int nIDCount = 23001; nIDCount <= 23025; nIDCount++)
		{
			GetDlgItem(nIDCount)->ShowWindow(SW_HIDE);
		}
	}

}

BOOL CAnimationProperties::PreTranslateMessage(MSG* pMsg) 
{
    if(pMsg->message==WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_F5)
		{
			m_cAnimationTab.SetCurSel(0);
			OnChangeTab();
		}
        if(pMsg->wParam == VK_F6)
		{
            m_cAnimationTab.SetCurSel(1);
			OnChangeTab();
		}
        if(pMsg->wParam == VK_F7)
		{
            m_cAnimationTab.SetCurSel(2);
			OnChangeTab();
		}
        if(pMsg->wParam == VK_F8)
		{
            m_cAnimationTab.SetCurSel(3);
			OnChangeTab();
		}

    }	
    return CDialog::PreTranslateMessage(pMsg);
}

void CAnimationProperties::OnEnChangeTotalFrame()
{
	CString totalFrameStr;
	GetDlgItem(20003)->GetWindowTextW(totalFrameStr);
	theApp.m_iTotalFrame = _ttoi(totalFrameStr);
}

void CAnimationProperties::OnCbnSelchangeTriggerPointCombo()
{
	m_iTriggerPointCombo = pComboBox20005->GetCurSel();


	// Animation - Geometry Tab

	m_sFrame.Format(_T("%d"), theApp.m_iFramePoint[m_iTriggerPointCombo]);
	GetDlgItem(20007)->SetWindowText(m_sFrame);

	m_sEndXAxis.Format(_T("%lf"), theApp.m_dEndXAxis[m_iTriggerPointCombo]);
	m_sEndXAxis = RemoveZeros(m_sEndXAxis);
	GetDlgItem(20009)->SetWindowText(m_sEndXAxis);

	m_sEndYAxis.Format(_T("%lf"), theApp.m_dEndYAxis[m_iTriggerPointCombo]);
	m_sEndYAxis = RemoveZeros(m_sEndYAxis);
	GetDlgItem(20011)->SetWindowText(m_sEndYAxis);

	m_iTranslationFunctionCombo = theApp.m_iTranslationFunction[m_iTriggerPointCombo];
	pComboBox20013->SetCurSel(m_iTranslationFunctionCombo);

	m_sEndZoomRatio.Format(_T("%lf"), theApp.m_dEndZoomRatio[m_iTriggerPointCombo]);
	m_sEndZoomRatio = RemoveZeros(m_sEndZoomRatio);
	GetDlgItem(20015)->SetWindowText(m_sEndZoomRatio);

	m_iZoomRatioFunctionCombo = theApp.m_iZoomRatioFunction[m_iTriggerPointCombo];
	pComboBox20017->SetCurSel(m_iZoomRatioFunctionCombo);

	m_sEndRotation.Format(_T("%lf"), theApp.m_dEndRotation[m_iTriggerPointCombo]);
	m_sEndRotation = RemoveZeros(m_sEndRotation);
	GetDlgItem(20019)->SetWindowText(m_sEndRotation);

	m_iRotationFunctionCombo = theApp.m_iRotationFunction[m_iTriggerPointCombo];
	pComboBox20021->SetCurSel(m_iRotationFunctionCombo);

	m_sEndIteration.Format(_T("%d"), theApp.m_iEndIteration[m_iTriggerPointCombo]);
	GetDlgItem(20023)->SetWindowText(m_sEndIteration);

	m_iIterationFunctionCombo = theApp.m_iIterationFunction[m_iTriggerPointCombo];
	pComboBox20025->SetCurSel(m_iIterationFunctionCombo);


	// Animation - Formula Tab

	m_sAnimationControl1.Format(_T("%lf"), theApp.m_dAnimationControl1[m_iTriggerPointCombo]);
	m_sAnimationControl1 = RemoveZeros(m_sAnimationControl1);
	GetDlgItem(21003)->SetWindowTextW(m_sAnimationControl1);

	m_iAnimationControl1Combo = theApp.m_iAnimationControl1Function[m_iTriggerPointCombo];
	theApp.pComboBox21005->SetCurSel(m_iAnimationControl1Combo);

	m_sAnimationControl2.Format(_T("%lf"), theApp.m_dAnimationControl2[m_iTriggerPointCombo]);
	m_sAnimationControl2 = RemoveZeros(m_sAnimationControl2);
	GetDlgItem(21007)->SetWindowTextW(m_sAnimationControl2);

	m_iAnimationControl2Combo = theApp.m_iAnimationControl2Function[m_iTriggerPointCombo];
	theApp.pComboBox21009->SetCurSel(m_iAnimationControl2Combo);

	m_sAnimationControl3.Format(_T("%lf"), theApp.m_dAnimationControl3[m_iTriggerPointCombo]);
	m_sAnimationControl3 = RemoveZeros(m_sAnimationControl3);
	GetDlgItem(21011)->SetWindowTextW(m_sAnimationControl3);

	m_iAnimationControl3Combo = theApp.m_iAnimationControl3Function[m_iTriggerPointCombo];
	theApp.pComboBox21013->SetCurSel(m_iAnimationControl3Combo);

	m_sAnimationControl4.Format(_T("%lf"), theApp.m_dAnimationControl4[m_iTriggerPointCombo]);
	m_sAnimationControl4 = RemoveZeros(m_sAnimationControl4);
	GetDlgItem(21015)->SetWindowTextW(m_sAnimationControl4);

	m_iAnimationControl4Combo = theApp.m_iAnimationControl4Function[m_iTriggerPointCombo];
	theApp.pComboBox21017->SetCurSel(m_iAnimationControl4Combo);

	m_sAnimationControl5.Format(_T("%lf"), theApp.m_dAnimationControl5[m_iTriggerPointCombo]);
	m_sAnimationControl5 = RemoveZeros(m_sAnimationControl5);
	GetDlgItem(21019)->SetWindowTextW(m_sAnimationControl5);

	m_iAnimationControl5Combo = theApp.m_iAnimationControl5Function[m_iTriggerPointCombo];
	theApp.pComboBox21021->SetCurSel(m_iAnimationControl5Combo);

	m_sAnimationControl6.Format(_T("%lf"), theApp.m_dAnimationControl6[m_iTriggerPointCombo]);
	m_sAnimationControl6 = RemoveZeros(m_sAnimationControl6);
	GetDlgItem(21023)->SetWindowTextW(m_sAnimationControl6);

	m_iAnimationControl6Combo = theApp.m_iAnimationControl6Function[m_iTriggerPointCombo];
	theApp.pComboBox21025->SetCurSel(m_iAnimationControl6Combo);


	// Animation - Outside Coloring Tab

	m_sAnimationOutsideRedAdder.Format(_T("%lf"), theApp.m_dAnimationOutsideRedAdder[m_iTriggerPointCombo]);
	m_sAnimationOutsideRedAdder = RemoveZeros(m_sAnimationOutsideRedAdder);
	GetDlgItem(22003)->SetWindowTextW(m_sAnimationOutsideRedAdder);

	m_iAnimationOutsideRedAdderCombo = theApp.m_iAnimationOutsideRedAdderFunction[m_iTriggerPointCombo];
	theApp.pComboBox22005->SetCurSel(m_iAnimationOutsideRedAdderCombo);

	m_sAnimationOutsideGreenAdder.Format(_T("%lf"), theApp.m_dAnimationOutsideGreenAdder[m_iTriggerPointCombo]);
	m_sAnimationOutsideGreenAdder = RemoveZeros(m_sAnimationOutsideGreenAdder);
	GetDlgItem(22007)->SetWindowTextW(m_sAnimationOutsideGreenAdder);

	m_iAnimationOutsideGreenAdderCombo = theApp.m_iAnimationOutsideGreenAdderFunction[m_iTriggerPointCombo];
	theApp.pComboBox22009->SetCurSel(m_iAnimationOutsideGreenAdderCombo);

	m_sAnimationOutsideBlueAdder.Format(_T("%lf"), theApp.m_dAnimationOutsideBlueAdder[m_iTriggerPointCombo]);
	m_sAnimationOutsideBlueAdder = RemoveZeros(m_sAnimationOutsideBlueAdder);
	GetDlgItem(22011)->SetWindowTextW(m_sAnimationOutsideBlueAdder);

	m_iAnimationOutsideBlueAdderCombo = theApp.m_iAnimationOutsideBlueAdderFunction[m_iTriggerPointCombo];
	theApp.pComboBox22013->SetCurSel(m_iAnimationOutsideBlueAdderCombo);

	m_sAnimationOutsideRedMult.Format(_T("%lf"), theApp.m_dAnimationOutsideRedMult[m_iTriggerPointCombo]);
	m_sAnimationOutsideRedMult = RemoveZeros(m_sAnimationOutsideRedMult);
	GetDlgItem(22015)->SetWindowTextW(m_sAnimationOutsideRedMult);

	m_iAnimationOutsideRedMultCombo = theApp.m_iAnimationOutsideRedMultFunction[m_iTriggerPointCombo];
	theApp.pComboBox22017->SetCurSel(m_iAnimationOutsideRedMultCombo);

	m_sAnimationOutsideGreenMult.Format(_T("%lf"), theApp.m_dAnimationOutsideGreenMult[m_iTriggerPointCombo]);
	m_sAnimationOutsideGreenMult = RemoveZeros(m_sAnimationOutsideGreenMult);
	GetDlgItem(22019)->SetWindowTextW(m_sAnimationOutsideGreenMult);

	m_iAnimationOutsideGreenMultCombo = theApp.m_iAnimationOutsideGreenMultFunction[m_iTriggerPointCombo];
	theApp.pComboBox22021->SetCurSel(m_iAnimationOutsideGreenMultCombo);

	m_sAnimationOutsideBlueMult.Format(_T("%lf"), theApp.m_dAnimationOutsideBlueMult[m_iTriggerPointCombo]);
	m_sAnimationOutsideBlueMult = RemoveZeros(m_sAnimationOutsideBlueMult);
	GetDlgItem(22023)->SetWindowTextW(m_sAnimationOutsideBlueMult);

	m_iAnimationOutsideBlueMultCombo = theApp.m_iAnimationOutsideBlueMultFunction[m_iTriggerPointCombo];
	theApp.pComboBox22025->SetCurSel(m_iAnimationOutsideBlueMultCombo);

	
	// Animation - Inside Coloring Tab

	m_sAnimationInsideRedAdder.Format(_T("%lf"), theApp.m_dAnimationInsideRedAdder[m_iTriggerPointCombo]);
	m_sAnimationInsideRedAdder = RemoveZeros(m_sAnimationInsideRedAdder);
	GetDlgItem(23003)->SetWindowTextW(m_sAnimationInsideRedAdder);

	m_iAnimationInsideRedAdderCombo = theApp.m_iAnimationInsideRedAdderFunction[m_iTriggerPointCombo];
	theApp.pComboBox23005->SetCurSel(m_iAnimationInsideRedAdderCombo);

	m_sAnimationInsideGreenAdder.Format(_T("%lf"), theApp.m_dAnimationInsideGreenAdder[m_iTriggerPointCombo]);
	m_sAnimationInsideGreenAdder = RemoveZeros(m_sAnimationInsideGreenAdder);
	GetDlgItem(23007)->SetWindowTextW(m_sAnimationInsideGreenAdder);

	m_iAnimationInsideGreenAdderCombo = theApp.m_iAnimationInsideGreenAdderFunction[m_iTriggerPointCombo];
	theApp.pComboBox23009->SetCurSel(m_iAnimationInsideGreenAdderCombo);

	m_sAnimationInsideBlueAdder.Format(_T("%lf"), theApp.m_dAnimationInsideBlueAdder[m_iTriggerPointCombo]);
	m_sAnimationInsideBlueAdder = RemoveZeros(m_sAnimationInsideBlueAdder);
	GetDlgItem(23011)->SetWindowTextW(m_sAnimationInsideBlueAdder);

	m_iAnimationInsideBlueAdderCombo = theApp.m_iAnimationInsideBlueAdderFunction[m_iTriggerPointCombo];
	theApp.pComboBox23013->SetCurSel(m_iAnimationInsideBlueAdderCombo);

	m_sAnimationInsideRedMult.Format(_T("%lf"), theApp.m_dAnimationInsideRedMult[m_iTriggerPointCombo]);
	m_sAnimationInsideRedMult = RemoveZeros(m_sAnimationInsideRedMult);
	GetDlgItem(23015)->SetWindowTextW(m_sAnimationInsideRedMult);

	m_iAnimationInsideRedMultCombo = theApp.m_iAnimationInsideRedMultFunction[m_iTriggerPointCombo];
	theApp.pComboBox23017->SetCurSel(m_iAnimationInsideRedMultCombo);

	m_sAnimationInsideGreenMult.Format(_T("%lf"), theApp.m_dAnimationInsideGreenMult[m_iTriggerPointCombo]);
	m_sAnimationInsideGreenMult = RemoveZeros(m_sAnimationInsideGreenMult);
	GetDlgItem(23019)->SetWindowTextW(m_sAnimationInsideGreenMult);

	m_iAnimationInsideGreenMultCombo = theApp.m_iAnimationInsideGreenMultFunction[m_iTriggerPointCombo];
	theApp.pComboBox23021->SetCurSel(m_iAnimationInsideGreenMultCombo);

	m_sAnimationInsideBlueMult.Format(_T("%lf"), theApp.m_dAnimationInsideBlueMult[m_iTriggerPointCombo]);
	m_sAnimationInsideBlueMult = RemoveZeros(m_sAnimationInsideBlueMult);
	GetDlgItem(23023)->SetWindowTextW(m_sAnimationInsideBlueMult);

	m_iAnimationInsideBlueMultCombo = theApp.m_iAnimationInsideBlueMultFunction[m_iTriggerPointCombo];
	theApp.pComboBox23025->SetCurSel(m_iAnimationInsideBlueMultCombo);

}

void CAnimationProperties::OnEnChangeFrame()
{
	CString frameStr;
	GetDlgItem(20007)->GetWindowTextW(frameStr);
	theApp.m_iFramePoint[m_iTriggerPointCombo] = _ttoi(frameStr);
}

void CAnimationProperties::OnEnChangeEndXAxis()
{
	CString sXAxis;
	GetDlgItem(20009)->GetWindowTextW(sXAxis);
	theApp.m_dEndXAxis[m_iTriggerPointCombo] = wcstod(sXAxis, NULL);
}

void CAnimationProperties::OnEnChangeEndYAxis()
{
	CString sYAxis;
	GetDlgItem(20011)->GetWindowTextW(sYAxis);
	theApp.m_dEndYAxis[m_iTriggerPointCombo] = wcstod(sYAxis, NULL);
}

void CAnimationProperties::OnCbnSelchangeTranslationFunctionCombo()
{
	m_iTranslationFunctionCombo = pComboBox20013->GetCurSel();

	theApp.m_iTranslationFunction[m_iTriggerPointCombo] = m_iTranslationFunctionCombo;
}

void CAnimationProperties::OnEnChangeEndZoomRatio()
{
	CString sEndZoomRatio;
	GetDlgItem(20015)->GetWindowTextW(sEndZoomRatio);
	theApp.m_dEndZoomRatio[m_iTriggerPointCombo] = wcstod(sEndZoomRatio, NULL);
}

void CAnimationProperties::OnCbnSelchangeZoomRatioFunctionCombo()
{
	m_iZoomRatioFunctionCombo = pComboBox20017->GetCurSel();

	theApp.m_iZoomRatioFunction[m_iTriggerPointCombo] = m_iZoomRatioFunctionCombo;
}

void CAnimationProperties::OnEnChangeEndRotation()
{
	CString sEndRotation;
	GetDlgItem(20019)->GetWindowTextW(sEndRotation);
	theApp.m_dEndRotation[m_iTriggerPointCombo] = wcstod(sEndRotation, NULL);
}

void CAnimationProperties::OnCbnSelchangeRotationFunctionCombo()
{
	m_iRotationFunctionCombo = pComboBox20021->GetCurSel();

	theApp.m_iRotationFunction[m_iTriggerPointCombo] = m_iRotationFunctionCombo;
}

void CAnimationProperties::OnEnChangeEndIteration()
{
	CString sEndIteration;
	GetDlgItem(20023)->GetWindowTextW(sEndIteration);
	theApp.m_iEndIteration[m_iTriggerPointCombo] = _ttoi(sEndIteration);

}

void CAnimationProperties::OnCbnSelchangeIterationFunctionCombo()
{
	m_iIterationFunctionCombo = pComboBox20025->GetCurSel();

	theApp.m_iIterationFunction[m_iTriggerPointCombo] = m_iIterationFunctionCombo;

}

void CAnimationProperties::OnChangeAnimationControl1()
{
	CString sAnimationControl1;
	GetDlgItem(21003)->GetWindowTextW(sAnimationControl1);
	theApp.m_dAnimationControl1[m_iTriggerPointCombo] = wcstod(sAnimationControl1, NULL);
}

void CAnimationProperties::OnCbnSelchangeAnimationControl1Combo()
{
	m_iAnimationControl1Combo = theApp.pComboBox21005->GetCurSel();

	theApp.m_iAnimationControl1Function[m_iTriggerPointCombo] = m_iAnimationControl1Combo;
}

void CAnimationProperties::OnChangeAnimationControl2()
{
	CString sAnimationControl2;
	GetDlgItem(21007)->GetWindowTextW(sAnimationControl2);
	theApp.m_dAnimationControl2[m_iTriggerPointCombo] = wcstod(sAnimationControl2, NULL);
}

void CAnimationProperties::OnCbnSelchangeAnimationControl2Combo()
{
	m_iAnimationControl2Combo = theApp.pComboBox21009->GetCurSel();

	theApp.m_iAnimationControl2Function[m_iTriggerPointCombo] = m_iAnimationControl2Combo;
}

void CAnimationProperties::OnChangeAnimationControl3()
{
	CString sAnimationControl3;
	GetDlgItem(21011)->GetWindowTextW(sAnimationControl3);
	theApp.m_dAnimationControl3[m_iTriggerPointCombo] = wcstod(sAnimationControl3, NULL);
}

void CAnimationProperties::OnCbnSelchangeAnimationControl3Combo()
{
	m_iAnimationControl3Combo = theApp.pComboBox21013->GetCurSel();

	theApp.m_iAnimationControl3Function[m_iTriggerPointCombo] = m_iAnimationControl3Combo;
}

void CAnimationProperties::OnChangeAnimationControl4()
{
	CString sAnimationControl4;
	GetDlgItem(21015)->GetWindowTextW(sAnimationControl4);
	theApp.m_dAnimationControl4[m_iTriggerPointCombo] = wcstod(sAnimationControl4, NULL);
}

void CAnimationProperties::OnCbnSelchangeAnimationControl4Combo()
{
	m_iAnimationControl4Combo = theApp.pComboBox21017->GetCurSel();

	theApp.m_iAnimationControl4Function[m_iTriggerPointCombo] = m_iAnimationControl4Combo;
}

void CAnimationProperties::OnChangeAnimationControl5()
{
	CString sAnimationControl5;
	GetDlgItem(21019)->GetWindowTextW(sAnimationControl5);
	theApp.m_dAnimationControl5[m_iTriggerPointCombo] = wcstod(sAnimationControl5, NULL);
}

void CAnimationProperties::OnCbnSelchangeAnimationControl5Combo()
{
	m_iAnimationControl5Combo = theApp.pComboBox21021->GetCurSel();

	theApp.m_iAnimationControl5Function[m_iTriggerPointCombo] = m_iAnimationControl5Combo;
}

void CAnimationProperties::OnChangeAnimationControl6()
{
	CString sAnimationControl6;
	GetDlgItem(21023)->GetWindowTextW(sAnimationControl6);
	theApp.m_dAnimationControl6[m_iTriggerPointCombo] = wcstod(sAnimationControl6, NULL);
}

void CAnimationProperties::OnCbnSelchangeAnimationControl6Combo()
{
	m_iAnimationControl6Combo = theApp.pComboBox21025->GetCurSel();

	theApp.m_iAnimationControl6Function[m_iTriggerPointCombo] = m_iAnimationControl6Combo;
}

void CAnimationProperties::OnChangeAnimationOutsideRedAdder()
{
	CString sAnimationOutsideRedAdder;
	GetDlgItem(22003)->GetWindowTextW(sAnimationOutsideRedAdder);
	theApp.m_dAnimationOutsideRedAdder[m_iTriggerPointCombo] = wcstod(sAnimationOutsideRedAdder, NULL);
}

void CAnimationProperties::OnCbnSelchangeAnimationOutsideRedAdderCombo()
{
	m_iAnimationOutsideRedAdderCombo = theApp.pComboBox22005->GetCurSel();

	theApp.m_iAnimationOutsideRedAdderFunction[m_iTriggerPointCombo] = m_iAnimationOutsideRedAdderCombo;
}

void CAnimationProperties::OnChangeAnimationOutsideGreenAdder()
{
	CString sAnimationOutsideGreenAdder;
	GetDlgItem(22007)->GetWindowTextW(sAnimationOutsideGreenAdder);
	theApp.m_dAnimationOutsideGreenAdder[m_iTriggerPointCombo] = wcstod(sAnimationOutsideGreenAdder, NULL);
}

void CAnimationProperties::OnCbnSelchangeAnimationOutsideGreenAdderCombo()
{
	m_iAnimationOutsideGreenAdderCombo = theApp.pComboBox22009->GetCurSel();

	theApp.m_iAnimationOutsideGreenAdderFunction[m_iTriggerPointCombo] = m_iAnimationOutsideGreenAdderCombo;
}

void CAnimationProperties::OnChangeAnimationOutsideBlueAdder()
{
	CString sAnimationOutsideBlueAdder;
	GetDlgItem(22011)->GetWindowTextW(sAnimationOutsideBlueAdder);
	theApp.m_dAnimationOutsideBlueAdder[m_iTriggerPointCombo] = wcstod(sAnimationOutsideBlueAdder, NULL);
}

void CAnimationProperties::OnCbnSelchangeAnimationOutsideBlueAdderCombo()
{
	m_iAnimationOutsideBlueAdderCombo = theApp.pComboBox22013->GetCurSel();

	theApp.m_iAnimationOutsideBlueAdderFunction[m_iTriggerPointCombo] = m_iAnimationOutsideBlueAdderCombo;
}

void CAnimationProperties::OnChangeAnimationOutsideRedMult()
{
	CString sAnimationOutsideRedMult;
	GetDlgItem(22015)->GetWindowTextW(sAnimationOutsideRedMult);
	theApp.m_dAnimationOutsideRedMult[m_iTriggerPointCombo] = wcstod(sAnimationOutsideRedMult, NULL);
}

void CAnimationProperties::OnCbnSelchangeAnimationOutsideRedMultCombo()
{
	m_iAnimationOutsideRedMultCombo = theApp.pComboBox22017->GetCurSel();

	theApp.m_iAnimationOutsideRedMultFunction[m_iTriggerPointCombo] = m_iAnimationOutsideRedMultCombo;
}

void CAnimationProperties::OnChangeAnimationOutsideGreenMult()
{
	CString sAnimationOutsideGreenMult;
	GetDlgItem(22019)->GetWindowTextW(sAnimationOutsideGreenMult);
	theApp.m_dAnimationOutsideGreenMult[m_iTriggerPointCombo] = wcstod(sAnimationOutsideGreenMult, NULL);
}

void CAnimationProperties::OnCbnSelchangeAnimationOutsideGreenMultCombo()
{
	m_iAnimationOutsideGreenMultCombo = theApp.pComboBox22021->GetCurSel();

	theApp.m_iAnimationOutsideGreenMultFunction[m_iTriggerPointCombo] = m_iAnimationOutsideGreenMultCombo;
}

void CAnimationProperties::OnChangeAnimationOutsideBlueMult()
{
	CString sAnimationOutsideBlueMult;
	GetDlgItem(22023)->GetWindowTextW(sAnimationOutsideBlueMult);
	theApp.m_dAnimationOutsideBlueMult[m_iTriggerPointCombo] = wcstod(sAnimationOutsideBlueMult, NULL);
}

void CAnimationProperties::OnCbnSelchangeAnimationOutsideBlueMultCombo()
{
	m_iAnimationOutsideBlueMultCombo = theApp.pComboBox22025->GetCurSel();

	theApp.m_iAnimationOutsideBlueMultFunction[m_iTriggerPointCombo] = m_iAnimationOutsideBlueMultCombo;
}

void CAnimationProperties::OnChangeAnimationInsideRedAdder()
{
	CString sAnimationInsideRedAdder;
	GetDlgItem(23003)->GetWindowTextW(sAnimationInsideRedAdder);
	theApp.m_dAnimationInsideRedAdder[m_iTriggerPointCombo] = wcstod(sAnimationInsideRedAdder, NULL);
}

void CAnimationProperties::OnCbnSelchangeAnimationInsideRedAdderCombo()
{
	m_iAnimationInsideRedAdderCombo = theApp.pComboBox23005->GetCurSel();

	theApp.m_iAnimationInsideRedAdderFunction[m_iTriggerPointCombo] = m_iAnimationInsideRedAdderCombo;
}

void CAnimationProperties::OnChangeAnimationInsideGreenAdder()
{
	CString sAnimationInsideGreenAdder;
	GetDlgItem(23007)->GetWindowTextW(sAnimationInsideGreenAdder);
	theApp.m_dAnimationInsideGreenAdder[m_iTriggerPointCombo] = wcstod(sAnimationInsideGreenAdder, NULL);
}

void CAnimationProperties::OnCbnSelchangeAnimationInsideGreenAdderCombo()
{
	m_iAnimationInsideGreenAdderCombo = theApp.pComboBox23009->GetCurSel();

	theApp.m_iAnimationInsideGreenAdderFunction[m_iTriggerPointCombo] = m_iAnimationInsideGreenAdderCombo;
}

void CAnimationProperties::OnChangeAnimationInsideBlueAdder()
{
	CString sAnimationInsideBlueAdder;
	GetDlgItem(23011)->GetWindowTextW(sAnimationInsideBlueAdder);
	theApp.m_dAnimationInsideBlueAdder[m_iTriggerPointCombo] = wcstod(sAnimationInsideBlueAdder, NULL);
}

void CAnimationProperties::OnCbnSelchangeAnimationInsideBlueAdderCombo()
{
	m_iAnimationInsideBlueAdderCombo = theApp.pComboBox23013->GetCurSel();

	theApp.m_iAnimationInsideBlueAdderFunction[m_iTriggerPointCombo] = m_iAnimationInsideBlueAdderCombo;
}

void CAnimationProperties::OnChangeAnimationInsideRedMult()
{
	CString sAnimationInsideRedMult;
	GetDlgItem(23015)->GetWindowTextW(sAnimationInsideRedMult);
	theApp.m_dAnimationInsideRedMult[m_iTriggerPointCombo] = wcstod(sAnimationInsideRedMult, NULL);
}

void CAnimationProperties::OnCbnSelchangeAnimationInsideRedMultCombo()
{
	m_iAnimationInsideRedMultCombo = theApp.pComboBox23017->GetCurSel();

	theApp.m_iAnimationInsideRedMultFunction[m_iTriggerPointCombo] = m_iAnimationInsideRedMultCombo;
}

void CAnimationProperties::OnChangeAnimationInsideGreenMult()
{
	CString sAnimationInsideGreenMult;
	GetDlgItem(23019)->GetWindowTextW(sAnimationInsideGreenMult);
	theApp.m_dAnimationInsideGreenMult[m_iTriggerPointCombo] = wcstod(sAnimationInsideGreenMult, NULL);
}

void CAnimationProperties::OnCbnSelchangeAnimationInsideGreenMultCombo()
{
	m_iAnimationInsideGreenMultCombo = theApp.pComboBox23021->GetCurSel();

	theApp.m_iAnimationInsideGreenMultFunction[m_iTriggerPointCombo] = m_iAnimationInsideGreenMultCombo;
}

void CAnimationProperties::OnChangeAnimationInsideBlueMult()
{
	CString sAnimationInsideBlueMult;
	GetDlgItem(23023)->GetWindowTextW(sAnimationInsideBlueMult);
	theApp.m_dAnimationInsideBlueMult[m_iTriggerPointCombo] = wcstod(sAnimationInsideBlueMult, NULL);
}

void CAnimationProperties::OnCbnSelchangeAnimationInsideBlueMultCombo()
{
	m_iAnimationInsideBlueMultCombo = theApp.pComboBox23025->GetCurSel();

	theApp.m_iAnimationInsideBlueMultFunction[m_iTriggerPointCombo] = m_iAnimationInsideBlueMultCombo;
}
