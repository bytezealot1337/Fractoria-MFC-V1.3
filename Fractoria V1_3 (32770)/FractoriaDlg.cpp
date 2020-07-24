// FractoriaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BinFile.h"
#include "Fractoria.h"
#include "FractoriaDlg.h"
#include "GlobalFunctions.h"
#include "ExitRequest.h"
#include "AnimationProperties.h"
#include "ColoringProperties.h"
#include "NRootColoringProperties.h"


#define DEGtoRAD 0.01745329251994329576923690768489
#define PI 3.14159265358979323846264338327950288419


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CFractoriaDlg dialog




CFractoriaDlg::CFractoriaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFractoriaDlg::IDD, pParent)
	, m_dCoordXRounded(0)
	, m_dCoordYRounded(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFractoriaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAINTAB, m_cMainTab);
	DDX_Text(pDX, IDC_COORDXROUNDED, m_dCoordXRounded);
	DDX_Text(pDX, IDC_COORDYROUNDED, m_dCoordYRounded);
}

BEGIN_MESSAGE_MAP(CFractoriaDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAINTAB, &CFractoriaDlg::OnTcnSelchangeTabcontrol)
	ON_BN_CLICKED(IDCANCEL, &CFractoriaDlg::OnBnClickedCancel)
	ON_COMMAND(ID_HELP_ABOUT, &CFractoriaDlg::OnHelpAbout)

	ON_CBN_SELCHANGE(10003, &CFractoriaDlg::OnCbnSelchangeFractalTypeCombo)
	ON_EN_CHANGE(10005, &CFractoriaDlg::OnEnChangeRealAxis)
	ON_EN_CHANGE(10007, &CFractoriaDlg::OnEnChangeImagAxis)
	ON_EN_CHANGE(10009, &CFractoriaDlg::OnEnChangeZoomRatio)
	ON_EN_CHANGE(10011, &CFractoriaDlg::OnEnChangeRotation)
	ON_EN_CHANGE(10013, &CFractoriaDlg::OnEnChangeIteration)
	ON_EN_CHANGE(10015, &CFractoriaDlg::OnEnChangeBailout)

	ON_EN_CHANGE(11003, &CFractoriaDlg::OnEnChangeControl1)
	ON_EN_CHANGE(11005, &CFractoriaDlg::OnEnChangeControl2)
	ON_EN_CHANGE(11007, &CFractoriaDlg::OnEnChangeControl3)
	ON_EN_CHANGE(11009, &CFractoriaDlg::OnEnChangeControl4)
	ON_EN_CHANGE(11011, &CFractoriaDlg::OnEnChangeControl5)
	ON_EN_CHANGE(11013, &CFractoriaDlg::OnEnChangeControl6)

	ON_EN_CHANGE(12003, &CFractoriaDlg::OnEnChangeOutRedAdder)
	ON_EN_CHANGE(12005, &CFractoriaDlg::OnEnChangeOutGreenAdder)
	ON_EN_CHANGE(12007, &CFractoriaDlg::OnEnChangeOutBlueAdder)
	ON_EN_CHANGE(12009, &CFractoriaDlg::OnEnChangeOutRedMult)
	ON_EN_CHANGE(12011, &CFractoriaDlg::OnEnChangeOutGreenMult)
	ON_EN_CHANGE(12013, &CFractoriaDlg::OnEnChangeOutBlueMult)
	ON_BN_CLICKED(12015, &CFractoriaDlg::OnBnClickedOutsideColorPalette)

	ON_EN_CHANGE(13003, &CFractoriaDlg::OnEnChangeInRedAdder)
	ON_EN_CHANGE(13005, &CFractoriaDlg::OnEnChangeInGreenAdder)
	ON_EN_CHANGE(13007, &CFractoriaDlg::OnEnChangeInBlueAdder)
	ON_EN_CHANGE(13009, &CFractoriaDlg::OnEnChangeInRedMult)
	ON_EN_CHANGE(13011, &CFractoriaDlg::OnEnChangeInGreenMult)
	ON_EN_CHANGE(13013, &CFractoriaDlg::OnEnChangeInBlueMult)
	ON_BN_CLICKED(13015, &CFractoriaDlg::OnBnClickedInsideColorPalette)

	ON_BN_CLICKED(ID_RENDER, &CFractoriaDlg::OnBnClickedRender)
	ON_COMMAND(ID_FILE_SAVEIMAGE, &CFractoriaDlg::OnFileSaveImage)
	ON_COMMAND(ID_PROPERTIES_ANIMATION, &CFractoriaDlg::OnPropertiesAnimation)
	ON_BN_CLICKED(IDC_ANIMATION, &CFractoriaDlg::OnBnClickedAnimation)
	ON_BN_CLICKED(IDC_CANCELRENDER, &CFractoriaDlg::OnBnClickedSuspendRender)
	ON_BN_CLICKED(16005, &CFractoriaDlg::OnBnClickedSwitchModeFractal)
	ON_COMMAND(ID_RENDERING_COLORING, &CFractoriaDlg::OnRenderingColoring)
END_MESSAGE_MAP()


// CFractoriaDlg message handlers

BOOL CFractoriaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_cMainTab.InsertItem(0,_T("Geometry"));
	m_cMainTab.InsertItem(1,_T("Formula"));
	m_cMainTab.InsertItem(2,_T("Outside"));
	m_cMainTab.InsertItem(3,_T("Inside"));


	// Tab 1 - Geometry

	CButton* pGroupBox10001 = new CButton;
	pGroupBox10001->Create(_T("Properties"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
		CRect(925,40,1160,250) ,this, 10001);
	pGroupBox10001->SetFont(GetFont());
	pGroupBox10001->ShowWindow(SW_SHOW);

	CStatic* pStatic10002 = new CStatic;
	pStatic10002->Create(_T("Fractal Type :"), WS_CHILD|WS_VISIBLE,
		CRect(935,60,1013,80), this, 10002);
	pStatic10002->SetFont(GetFont());
	pStatic10002->ShowWindow(SW_SHOW);

	// Fractal Type ComboBox
	theApp.pComboBox10003 = new CComboBox;
	theApp.pComboBox10003->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWNLIST,
		CRect(1015,57,1150,77), this, 10003);
	theApp.pComboBox10003->SetFont(GetFont());
	theApp.pComboBox10003->ShowWindow(SW_SHOW);
	
	theApp.pComboBox10003->AddString(_T("Mandelbrot"));
	theApp.pComboBox10003->AddString(_T("Mandelbrot (Quick)"));
	theApp.pComboBox10003->AddString(_T("Julia"));
	theApp.pComboBox10003->AddString(_T("Julia (Quick)"));
	theApp.pComboBox10003->AddString(_T("Newton"));
	theApp.pComboBox10003->AddString(_T("Newton (Root)"));
	theApp.pComboBox10003->AddString(_T("Nova"));
	theApp.pComboBox10003->AddString(_T("Nova (Special)"));
	theApp.pComboBox10003->AddString(_T("Glynn"));
	theApp.pComboBox10003->AddString(_T("Phoenix"));
	theApp.pComboBox10003->AddString(_T("Cos(Tan(z))"));

	theApp.pComboBox10003->SetCurSel(0);


	CStatic* pStatic10004 = new CStatic;
	pStatic10004->Create(_T("Center (Re) :"), WS_CHILD|WS_VISIBLE,
		CRect(935,95,1013,115), this, 10004);
	pStatic10004->SetFont(GetFont());
	pStatic10004->ShowWindow(SW_SHOW);

	theApp.pEdit10005.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,92,1150,112), this, 10005);
	theApp.pEdit10005.SetFont(GetFont());
	theApp.pEdit10005.ShowWindow(SW_SHOW);
	theApp.pEdit10005.SetWindowText(_T("0.0"));

	CStatic* pStatic10006 = new CStatic;
	pStatic10006->Create(_T("Center (Im) :"), WS_CHILD|WS_VISIBLE,
		CRect(935,120,1013,140), this, 10006);
	pStatic10006->SetFont(GetFont());
	pStatic10006->ShowWindow(SW_SHOW);

	theApp.pEdit10007.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,117,1150,137), this, 10007);
	theApp.pEdit10007.SetFont(GetFont());
	theApp.pEdit10007.ShowWindow(SW_SHOW);
	theApp.pEdit10007.SetWindowText(_T("0.0"));

	CStatic* pStatic10008 = new CStatic;
	pStatic10008->Create(_T("Zoom Ratio :"), WS_CHILD|WS_VISIBLE,
		CRect(935,145,1013,165), this, 10008);
	pStatic10008->SetFont(GetFont());
	pStatic10008->ShowWindow(SW_SHOW);

	theApp.pEdit10009.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,142,1150,162), this, 10009);
	theApp.pEdit10009.SetFont(GetFont());
	theApp.pEdit10009.ShowWindow(SW_SHOW);
	theApp.pEdit10009.SetWindowText(_T("1.0"));

	CStatic* pStatic10010 = new CStatic;
	pStatic10010->Create(_T("Rotation :"), WS_CHILD|WS_VISIBLE,
		CRect(935,170,1013,190), this, 10010);
	pStatic10010->SetFont(GetFont());
	pStatic10010->ShowWindow(SW_SHOW);

	theApp.pEdit10011.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,167,1150,187), this, 10011);
	theApp.pEdit10011.SetFont(GetFont());
	theApp.pEdit10011.ShowWindow(SW_SHOW);
	theApp.pEdit10011.SetWindowText(_T("0.0"));

	CStatic* pStatic10012 = new CStatic;
	pStatic10012->Create(_T("Iteration :"), WS_CHILD|WS_VISIBLE,
		CRect(935,195,1013,215), this, 10012);
	pStatic10012->SetFont(GetFont());
	pStatic10012->ShowWindow(SW_SHOW);

	theApp.pEdit10013.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,193,1150,212), this, 10013);
	theApp.pEdit10013.SetFont(GetFont());
	theApp.pEdit10013.ShowWindow(SW_SHOW);
	theApp.pEdit10013.SetWindowText(_T("25"));

	CStatic* pStatic10014 = new CStatic;
	pStatic10014->Create(_T("Bailout :"), WS_CHILD|WS_VISIBLE,
		CRect(935,220,1013,240), this, 10014);
	pStatic10014->SetFont(GetFont());
	pStatic10014->ShowWindow(SW_SHOW);

	CEdit* pEdit10015 = new CEdit;
	pEdit10015->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,217,1150,237), this, 10015);
	pEdit10015->SetFont(GetFont());
	pEdit10015->ShowWindow(SW_SHOW);
	pEdit10015->SetWindowText(_T("4.0"));


	// Tab 2 - Formula

	CButton* pGroupBox11001 = new CButton;
	pGroupBox11001->Create(_T("Formula"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
		CRect(925,40,1160,250) ,this, 11001);
	pGroupBox11001->SetFont(GetFont());
	pGroupBox11001->ShowWindow(SW_HIDE);

	CStatic* pStatic11002 = new CStatic;
	pStatic11002->Create(_T("Control #1 :"), WS_CHILD|WS_VISIBLE,
		CRect(935,95,1013,115), this, 11002);
	pStatic11002->SetFont(GetFont());
	pStatic11002->ShowWindow(SW_HIDE);

	theApp.pEdit11003.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,92,1150,112), this, 11003);
	theApp.pEdit11003.SetFont(GetFont());
	theApp.pEdit11003.ShowWindow(SW_HIDE);
	theApp.pEdit11003.SetWindowText(_T("0.0"));

	CStatic* pStatic11004 = new CStatic;
	pStatic11004->Create(_T("Control #2 :"), WS_CHILD|WS_VISIBLE,
		CRect(935,120,1013,140), this, 11004);
	pStatic11004->SetFont(GetFont());
	pStatic11004->ShowWindow(SW_HIDE);

	theApp.pEdit11005.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,117,1150,137), this, 11005);
	theApp.pEdit11005.SetFont(GetFont());
	theApp.pEdit11005.ShowWindow(SW_HIDE);
	theApp.pEdit11005.SetWindowText(_T("0.0"));

	CStatic* pStatic11006 = new CStatic;
	pStatic11006->Create(_T("Control #3 :"), WS_CHILD|WS_VISIBLE,
		CRect(935,145,1013,165), this, 11006);
	pStatic11006->SetFont(GetFont());
	pStatic11006->ShowWindow(SW_HIDE);

	theApp.pEdit11007.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,142,1150,162), this, 11007);
	theApp.pEdit11007.SetFont(GetFont());
	theApp.pEdit11007.ShowWindow(SW_HIDE);
	theApp.pEdit11007.SetWindowText(_T("0.0"));

	CStatic* pStatic11008 = new CStatic;
	pStatic11008->Create(_T("Control #4 :"), WS_CHILD|WS_VISIBLE,
		CRect(935,170,1013,190), this, 11008);
	pStatic11008->SetFont(GetFont());
	pStatic11008->ShowWindow(SW_HIDE);

	theApp.pEdit11009.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,167,1150,187), this, 11009);
	theApp.pEdit11009.SetFont(GetFont());
	theApp.pEdit11009.ShowWindow(SW_HIDE);
	theApp.pEdit11009.SetWindowText(_T("0.0"));

	CStatic* pStatic11010 = new CStatic;
	pStatic11010->Create(_T("Control #5 :"), WS_CHILD|WS_VISIBLE,
		CRect(935,195,1013,215), this, 11010);
	pStatic11010->SetFont(GetFont());
	pStatic11010->ShowWindow(SW_HIDE);

	theApp.pEdit11011.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,193,1150,212), this, 11011);
	theApp.pEdit11011.SetFont(GetFont());
	theApp.pEdit11011.ShowWindow(SW_HIDE);
	theApp.pEdit11011.SetWindowText(_T("0.0"));

	CStatic* pStatic11012 = new CStatic;
	pStatic11012->Create(_T("Control #6 :"), WS_CHILD|WS_VISIBLE,
		CRect(935,220,1013,240), this, 11012);
	pStatic11012->SetFont(GetFont());
	pStatic11012->ShowWindow(SW_HIDE);

	theApp.pEdit11013.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,217,1150,237), this, 11013);
	theApp.pEdit11013.SetFont(GetFont());
	theApp.pEdit11013.ShowWindow(SW_HIDE);
	theApp.pEdit11013.SetWindowText(_T("0.0"));


	// Tab 3 - Outside Coloring

	CButton* pGroupBox12001 = new CButton;
	pGroupBox12001->Create(_T("Outside Color Adder"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
		CRect(925,40,1160,250) ,this, 12001);
	pGroupBox12001->SetFont(GetFont());
	pGroupBox12001->ShowWindow(SW_HIDE);

	theApp.pButton12015 = new CButton;
	theApp.pButton12015->Create(_T("Color Palette"),WS_CHILD|WS_VISIBLE|WS_TABSTOP,
		CRect(1050,38,1125,57) ,this, 12015);
	theApp.pButton12015->SetFont(GetFont());
	theApp.pButton12015->ShowWindow(SW_HIDE);

	theApp.pStatic12002 = new CStatic;
	theApp.pStatic12002->Create(_T("Red :"), WS_CHILD|WS_VISIBLE,
		CRect(935,65,1013,85), this, 12002);
	theApp.pStatic12002->SetFont(GetFont());
	theApp.pStatic12002->ShowWindow(SW_HIDE);

	theApp.pEdit12003 = new CEdit;
	theApp.pEdit12003->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,62,1150,82), this, 12003);
	theApp.pEdit12003->SetFont(GetFont());
	theApp.pEdit12003->ShowWindow(SW_HIDE);
	theApp.pEdit12003->SetWindowText(_T("255"));

	theApp.pStatic12004 = new CStatic;
	theApp.pStatic12004->Create(_T("Green :"), WS_CHILD|WS_VISIBLE,
		CRect(935,90,1013,110), this, 12004);
	theApp.pStatic12004->SetFont(GetFont());
	theApp.pStatic12004->ShowWindow(SW_HIDE);

	theApp.pEdit12005 = new CEdit;
	theApp.pEdit12005->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,87,1150,107), this, 12005);
	theApp.pEdit12005->SetFont(GetFont());
	theApp.pEdit12005->ShowWindow(SW_HIDE);
	theApp.pEdit12005->SetWindowText(_T("255"));

	theApp.pStatic12006 = new CStatic;
	theApp.pStatic12006->Create(_T("Blue :"), WS_CHILD|WS_VISIBLE,
		CRect(935,115,1013,135), this, 12006);
	theApp.pStatic12006->SetFont(GetFont());
	theApp.pStatic12006->ShowWindow(SW_HIDE);

	theApp.pEdit12007 = new CEdit;
	theApp.pEdit12007->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,112,1150,132), this, 12007);
	theApp.pEdit12007->SetFont(GetFont());
	theApp.pEdit12007->ShowWindow(SW_HIDE);
	theApp.pEdit12007->SetWindowText(_T("255"));


	theApp.pStatic12008 = new CStatic;
	theApp.pStatic12008->Create(_T("Red :"), WS_CHILD|WS_VISIBLE,
		CRect(935,169,1013,189), this, 12008);
	theApp.pStatic12008->SetFont(GetFont());
	theApp.pStatic12008->ShowWindow(SW_HIDE);

	theApp.pEdit12009 = new CEdit;
	theApp.pEdit12009->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,166,1150,186), this, 12009);
	theApp.pEdit12009->SetFont(GetFont());
	theApp.pEdit12009->ShowWindow(SW_HIDE);
	theApp.pEdit12009->SetWindowText(_T("0.0"));

	theApp.pStatic12010 = new CStatic;
	theApp.pStatic12010->Create(_T("Green :"), WS_CHILD|WS_VISIBLE,
		CRect(935,194,1013,214), this, 12010);
	theApp.pStatic12010->SetFont(GetFont());
	theApp.pStatic12010->ShowWindow(SW_HIDE);

	theApp.pEdit12011 = new CEdit;
	theApp.pEdit12011->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,191,1150,211), this, 12011);
	theApp.pEdit12011->SetFont(GetFont());
	theApp.pEdit12011->ShowWindow(SW_HIDE);
	theApp.pEdit12011->SetWindowText(_T("0.0"));

	theApp.pStatic12012 = new CStatic;
	theApp.pStatic12012->Create(_T("Blue :"), WS_CHILD|WS_VISIBLE,
		CRect(935,219,1013,239), this, 12012);
	theApp.pStatic12012->SetFont(GetFont());
	theApp.pStatic12012->ShowWindow(SW_HIDE);

	theApp.pEdit12013 = new CEdit;
	theApp.pEdit12013->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,216,1150,236), this, 12013);
	theApp.pEdit12013->SetFont(GetFont());
	theApp.pEdit12013->ShowWindow(SW_HIDE);
	theApp.pEdit12013->SetWindowText(_T("0.0"));

	CButton* pGroupBox12014 = new CButton;
	pGroupBox12014->Create(_T("Outside Color Multiplier"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
		CRect(925,143,1160,250) ,this, 12014);
	pGroupBox12014->SetFont(GetFont());
	pGroupBox12014->ShowWindow(SW_HIDE);


	// Tab 4 - Inside Coloring

	CButton* pGroupBox13001 = new CButton;
	pGroupBox13001->Create(_T("Inside Color Adder"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
		CRect(925,40,1160,250) ,this, 13001);
	pGroupBox13001->SetFont(GetFont());
	pGroupBox13001->ShowWindow(SW_HIDE);

	theApp.pButton13015 = new CButton;
	theApp.pButton13015->Create(_T("Color Palette"),WS_CHILD|WS_VISIBLE|WS_TABSTOP,
		CRect(1050,38,1125,57) ,this, 13015);
	theApp.pButton13015->SetFont(GetFont());
	theApp.pButton13015->ShowWindow(SW_HIDE);

	theApp.pStatic13002 = new CStatic;
	theApp.pStatic13002->Create(_T("Red :"), WS_CHILD|WS_VISIBLE,
		CRect(935,65,1013,85), this, 13002);
	theApp.pStatic13002->SetFont(GetFont());
	theApp.pStatic13002->ShowWindow(SW_HIDE);

	theApp.pEdit13003 = new CEdit;
	theApp.pEdit13003->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,62,1150,82), this, 13003);
	theApp.pEdit13003->SetFont(GetFont());
	theApp.pEdit13003->ShowWindow(SW_HIDE);
	theApp.pEdit13003->SetWindowText(_T("0"));

	theApp.pStatic13004 = new CStatic;
	theApp.pStatic13004->Create(_T("Green :"), WS_CHILD|WS_VISIBLE,
		CRect(935,90,1013,110), this, 13004);
	theApp.pStatic13004->SetFont(GetFont());
	theApp.pStatic13004->ShowWindow(SW_HIDE);

	theApp.pEdit13005 = new CEdit;
	theApp.pEdit13005->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,87,1150,107), this, 13005);
	theApp.pEdit13005->SetFont(GetFont());
	theApp.pEdit13005->ShowWindow(SW_HIDE);
	theApp.pEdit13005->SetWindowText(_T("0"));

	theApp.pStatic13006 = new CStatic;
	theApp.pStatic13006->Create(_T("Blue :"), WS_CHILD|WS_VISIBLE,
		CRect(935,115,1013,135), this, 13006);
	theApp.pStatic13006->SetFont(GetFont());
	theApp.pStatic13006->ShowWindow(SW_HIDE);

	theApp.pEdit13007 = new CEdit;
	theApp.pEdit13007->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,112,1150,132), this, 13007);
	theApp.pEdit13007->SetFont(GetFont());
	theApp.pEdit13007->ShowWindow(SW_HIDE);
	theApp.pEdit13007->SetWindowText(_T("0"));


	theApp.pStatic13008 = new CStatic;
	theApp.pStatic13008->Create(_T("Red :"), WS_CHILD|WS_VISIBLE,
		CRect(935,169,1013,189), this, 13008);
	theApp.pStatic13008->SetFont(GetFont());
	theApp.pStatic13008->ShowWindow(SW_HIDE);

	theApp.pEdit13009 = new CEdit;
	theApp.pEdit13009->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,166,1150,186), this, 13009);
	theApp.pEdit13009->SetFont(GetFont());
	theApp.pEdit13009->ShowWindow(SW_HIDE);
	theApp.pEdit13009->SetWindowText(_T("0.0"));

	theApp.pStatic13010 = new CStatic;
	theApp.pStatic13010->Create(_T("Green :"), WS_CHILD|WS_VISIBLE,
		CRect(935,194,1013,214), this, 13010);
	theApp.pStatic13010->SetFont(GetFont());
	theApp.pStatic13010->ShowWindow(SW_HIDE);

	theApp.pEdit13011 = new CEdit;
	theApp.pEdit13011->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,191,1150,211), this, 13011);
	theApp.pEdit13011->SetFont(GetFont());
	theApp.pEdit13011->ShowWindow(SW_HIDE);
	theApp.pEdit13011->SetWindowText(_T("0.0"));

	theApp.pStatic13012 = new CStatic;
	theApp.pStatic13012->Create(_T("Blue :"), WS_CHILD|WS_VISIBLE,
		CRect(935,219,1013,239), this, 13012);
	theApp.pStatic13012->SetFont(GetFont());
	theApp.pStatic13012->ShowWindow(SW_HIDE);

	theApp.pEdit13013 = new CEdit;
	theApp.pEdit13013->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL,
		CRect(1015,216,1150,236), this, 13013);
	theApp.pEdit13013->SetFont(GetFont());
	theApp.pEdit13013->ShowWindow(SW_HIDE);
	theApp.pEdit13013->SetWindowText(_T("0.0"));

	CButton* pGroupBox13014 = new CButton;
	pGroupBox13014->Create(_T("Inside Color Multiplier"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
		CRect(925,143,1160,250) ,this, 13014);
	pGroupBox13014->SetFont(GetFont());
	pGroupBox13014->ShowWindow(SW_HIDE);


	CButton* pGroupBox14001 = new CButton;
	pGroupBox14001->Create(_T("Preview  (F9 Key)"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
		CRect(925,250,1160,405) ,this, 14001);
	pGroupBox14001->SetFont(GetFont());
	pGroupBox14001->ShowWindow(SW_SHOW);

	CButton* pGroupBox15001 = new CButton;
	pGroupBox15001->Create(_T(""),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
		CRect(925,423,1160,568) ,this, 15001);
	pGroupBox15001->SetFont(GetFont());
	pGroupBox15001->ShowWindow(SW_SHOW);

	CButton* pGroupBox16001 = new CButton;
	pGroupBox16001->Create(_T(""),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
		CRect(925,478,1160,568) ,this, 16001);
	pGroupBox16001->SetFont(GetFont());
	pGroupBox16001->ShowWindow(SW_SHOW);


	theApp.pProgress.Create(WS_CHILD | WS_VISIBLE | PBS_MARQUEE,
		     CRect(935, 495, 1106, 510), this, 0x16);
	theApp.pProgress.SetFont(GetFont());

	theApp.pStatic16002.Create(_T("0"), WS_CHILD|WS_VISIBLE,
		CRect(1115,496,1135,516), this, 16002);
	theApp.pStatic16002.SetFont(GetFont());
	theApp.pStatic16002.ShowWindow(SW_SHOW);

	theApp.pStatic16003.Create(_T("0"), WS_CHILD|WS_VISIBLE,
		CRect(1060,546,1150,566), this, 16003);
	theApp.pStatic16003.SetFont(GetFont());
	theApp.pStatic16003.ShowWindow(SW_SHOW);

	theApp.pStatic16004.Create(_T("0"), WS_CHILD|WS_VISIBLE,
		CRect(1060,523,1150,543), this, 16004);
	theApp.pStatic16004.SetFont(GetFont());
	theApp.pStatic16004.ShowWindow(SW_SHOW);



	theApp.pCheckBox16005 = new CButton;
	theApp.pCheckBox16005->Create(_T("  Switch Mode Fractal"),WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|WS_TABSTOP,
		CRect(935,408,1160,428) ,this, 16005);
	theApp.pCheckBox16005->SetFont(GetFont());
	theApp.pCheckBox16005->ShowWindow(SW_SHOW);


	///////////////////////////////////////////////////////////////////

	OnCbnSelchangeFractalTypeCombo();

	GetDlgItem(10003)->GetWindowTextW(m_sFractalTypeStr);

	theApp.m_bSwitchMode = FALSE;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFractoriaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFractoriaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFractoriaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFractoriaDlg::OnTcnSelchangeTabcontrol(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnChangeTab();
}

void CFractoriaDlg::OnChangeTab()
{
	if(m_cMainTab.GetCurFocus() == 0)
	{
		for(int nIDCount = 10001; nIDCount <= 10015; nIDCount++)
		{
			GetDlgItem(nIDCount)->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		for(int nIDCount = 10001; nIDCount <= 10015; nIDCount++)
		{
			GetDlgItem(nIDCount)->ShowWindow(SW_HIDE);
		}
	}

	if(m_cMainTab.GetCurFocus() == 1)
	{
		for(int nIDCount = 11001; nIDCount <= 11013; nIDCount++)
		{
			GetDlgItem(nIDCount)->ShowWindow(SW_SHOW);
		}

		GetDlgItem(10002)->ShowWindow(SW_SHOW);
		GetDlgItem(10003)->ShowWindow(SW_SHOW);
	}
	else
	{
		for(int nIDCount = 11001; nIDCount <= 11013; nIDCount++)
		{
			GetDlgItem(nIDCount)->ShowWindow(SW_HIDE);
		}
	}

	if(m_cMainTab.GetCurFocus() == 2)
	{
		for(int nIDCount = 12001; nIDCount <= 12015; nIDCount++)
		{
			GetDlgItem(nIDCount)->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		for(int nIDCount = 12001; nIDCount <= 12015; nIDCount++)
		{
			GetDlgItem(nIDCount)->ShowWindow(SW_HIDE);
		}
	}

	if(m_cMainTab.GetCurFocus() == 3)
	{
		for(int nIDCount = 13001; nIDCount <= 13015; nIDCount++)
		{
			GetDlgItem(nIDCount)->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		for(int nIDCount = 13001; nIDCount <= 13015; nIDCount++)
		{
			GetDlgItem(nIDCount)->ShowWindow(SW_HIDE);
		}
	}
}

double CFractoriaDlg::Round(double dbVal, int nPlaces)
{
    const double dbShift = pow(10.0, nPlaces);
    return  floor(dbVal * dbShift + 0.5) / dbShift; 
}

CString CFractoriaDlg::RemoveZeros(CString str)
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

void CFractoriaDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	int iXAxis;
	int iYAxis;
	double xCenter;
	double yCenter;
	double m_dCoordXBU;
	double m_dCoordYBU;

	if( (point.x >= 3) && (point.x < 903) )
	{
		if( (point.y >= 3) && (point.y < 603) )
		{
//			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));

			iXAxis = (point.x - 3) - 450;
			iYAxis = (point.y - 3) - 300;

			xCenter = theApp.m_dXAxis;
			yCenter = theApp.m_dYAxis;

			m_dCoordXBU = + ((6.0 / theApp.m_dZoomRatio) * double(iXAxis) / 900.0);
			m_dCoordYBU = - ((4.0 / theApp.m_dZoomRatio) * double(iYAxis) / 600.0);

/* NE */	if( (iXAxis > 0) && (iYAxis < 0) )
			{
				double hyp = sqrt(pow(m_dCoordXBU,2) + pow(m_dCoordYBU,2));
				double theta = atan(m_dCoordYBU / m_dCoordXBU);

				m_dCoordX = ( xCenter + hyp * cos(theta - theApp.m_dRotation*DEGtoRAD) );
				m_dCoordY = ( yCenter + hyp * sin(theta - theApp.m_dRotation*DEGtoRAD) );
			}

/* SE */	else if( (iXAxis > 0) && (iYAxis > 0) )
			{
				double hyp = sqrt(pow(m_dCoordXBU,2) + pow(m_dCoordYBU,2));
				double theta = atan(m_dCoordYBU / m_dCoordXBU);

				m_dCoordX = ( xCenter + hyp * cos(theta - theApp.m_dRotation*DEGtoRAD) );
				m_dCoordY = ( yCenter + hyp * sin(theta - theApp.m_dRotation*DEGtoRAD) );
			}

/* SW */	else if( (iXAxis < 0) && (iYAxis > 0) )
			{
				double hyp = sqrt(pow(m_dCoordXBU,2) + pow(m_dCoordYBU,2));
				double theta = atan(m_dCoordYBU / m_dCoordXBU);

				m_dCoordX = ( xCenter - hyp * cos(theta - theApp.m_dRotation*DEGtoRAD) );
				m_dCoordY = ( yCenter - hyp * sin(theta - theApp.m_dRotation*DEGtoRAD) );
			}

/* NW */	else if( (iXAxis < 0) && (iYAxis < 0) )
			{
				double hyp = sqrt(pow(m_dCoordXBU,2) + pow(m_dCoordYBU,2));
				double theta = atan(m_dCoordYBU / m_dCoordXBU);

				m_dCoordX = ( xCenter - hyp * cos(theta - theApp.m_dRotation*DEGtoRAD) );
				m_dCoordY = ( yCenter - hyp * sin(theta - theApp.m_dRotation*DEGtoRAD) );
			}

/* E+0 */	else if( (iXAxis == 0) && (iYAxis >= 0) )
			{
				double hyp = sqrt(pow(m_dCoordXBU,2) + pow(m_dCoordYBU,2));
				double theta = atan(m_dCoordYBU / m_dCoordXBU);

				m_dCoordX = ( xCenter + hyp * cos(theta - theApp.m_dRotation*DEGtoRAD) );
				m_dCoordY = ( yCenter + hyp * sin(theta - theApp.m_dRotation*DEGtoRAD) );
			}
/* W */		else if( (iXAxis == 0) && (iYAxis < 0) )
			{
				double hyp = sqrt(pow(m_dCoordXBU,2) + pow(m_dCoordYBU,2));
				double theta = atan(m_dCoordYBU / m_dCoordXBU);

				m_dCoordX = ( xCenter - hyp * cos(theta - theApp.m_dRotation*DEGtoRAD) );
				m_dCoordY = ( yCenter - hyp * sin(theta - theApp.m_dRotation*DEGtoRAD) );
			}
				
			m_dCoordXRounded = Round(m_dCoordX, 16);
			m_dCoordYRounded = Round(m_dCoordY, 16);

			UpdateData(FALSE);
		}
		else
		{
			m_dCoordXRounded = Round(theApp.m_dXAxis, 16);
			m_dCoordYRounded = Round(theApp.m_dYAxis, 16);

			UpdateData(FALSE);
		}
	}
	else
	{
		m_dCoordXRounded = Round(theApp.m_dXAxis, 16);
		m_dCoordYRounded = Round(theApp.m_dYAxis, 16);

		UpdateData(FALSE);
	}
}

void CFractoriaDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if( (point.x >= 3) && (point.x < 903) )
	{
		if( (point.y >= 3) && (point.y < 603) )
		{
			CString m_sXAxis;
			CString m_sYAxis;

			m_sXAxis.Format(_T("%.16lf"), m_dCoordX);
			m_sXAxis = RemoveZeros(m_sXAxis);
			GetDlgItem(10005)->SetWindowTextW(m_sXAxis);

			m_sYAxis.Format(_T("%.16lf"), m_dCoordY);
			m_sYAxis = RemoveZeros(m_sYAxis);
			GetDlgItem(10007)->SetWindowTextW(m_sYAxis);
		}
	}
}

void CFractoriaDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	if( (point.x >= 3) && (point.x < 903) )
	{
		if( (point.y >= 3) && (point.y < 603) )
		{
			if(theApp.m_bSwitchMode == TRUE)
			{
				m_dControl1 = m_dCoordX;
				m_dControl2 = m_dCoordY;

				CString sXAxis;
				CString sYAxis;

				sXAxis.Format(_T("%.16lf"), m_dControl1);
				sXAxis = RemoveZeros(sXAxis);
				GetDlgItem(11003)->SetWindowTextW(sXAxis);

				sYAxis.Format(_T("%.16lf"), m_dControl2);
				sYAxis = RemoveZeros(sYAxis);
				GetDlgItem(11005)->SetWindowTextW(sYAxis);
			}
		}
	}
}

HBRUSH CFractoriaDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CFractoriaDlg::OnBnClickedCancel()
{
	CExitRequest exitRequestDlg;

	if(exitRequestDlg.DoModal() == IDOK)
	{
//		exitRequestDlg.DestroyWindow();
		OnCancel();
	}	
}

void CFractoriaDlg::OnHelpAbout()
{
	CAboutDlg aboutDlgObj;

	aboutDlgObj.DoModal();
}

void CFractoriaDlg::OnCbnSelchangeFractalTypeCombo()
{
	UpdateData(TRUE);

	CString fractalTypeStr;
	GetDlgItem(10003)->GetWindowText(fractalTypeStr);

	if(fractalTypeStr == "Mandelbrot")
	{
		theApp.pCheckBox16005->EnableWindow(TRUE);

		if(theApp.m_bSwitchMode == FALSE)
		{
			GetDlgItem(11002)->SetWindowTextW(_T("Start (Re) :"));
			GetDlgItem(11004)->SetWindowTextW(_T("Start (Im) :"));
			GetDlgItem(11006)->SetWindowTextW(_T("Power of Z :"));
			GetDlgItem(11008)->SetWindowTextW(_T("Control #4 :"));
			GetDlgItem(11010)->SetWindowTextW(_T("Control #5 :"));
			GetDlgItem(11012)->SetWindowTextW(_T("Control #6 :"));

			GetDlgItem(11003)->SetWindowTextW(_T("0.0"));
			GetDlgItem(11005)->SetWindowTextW(_T("0.0"));
			GetDlgItem(11007)->SetWindowTextW(_T("2.0"));
			GetDlgItem(11009)->SetWindowTextW(_T("N/A"));
			GetDlgItem(11011)->SetWindowTextW(_T("N/A"));
			GetDlgItem(11013)->SetWindowTextW(_T("N/A"));

			for(int count1 = 11002; count1 <= 11007; count1++)
			{
				GetDlgItem(count1)->EnableWindow(TRUE);
			}
			for(int count2 = 11008; count2 <= 11013; count2++)
			{
				GetDlgItem(count2)->EnableWindow(FALSE);
			}
		}
		else if(theApp.m_bSwitchMode == TRUE)
		{
			GetDlgItem(11002)->SetWindowTextW(_T("Seed (Re) :"));
			GetDlgItem(11004)->SetWindowTextW(_T("Seed (Im) :"));
			GetDlgItem(11006)->SetWindowTextW(_T("Power of Z :"));
			GetDlgItem(11008)->SetWindowTextW(_T("Control #4 :"));
			GetDlgItem(11010)->SetWindowTextW(_T("Control #5 :"));
			GetDlgItem(11012)->SetWindowTextW(_T("Control #6 :"));

			GetDlgItem(11003)->SetWindowTextW(_T("0.0"));
			GetDlgItem(11005)->SetWindowTextW(_T("0.0"));

			CString powerStr;
			powerStr.Format(_T("%lf"), theApp.m_dControl3);
			powerStr = RemoveZeros(powerStr);
			GetDlgItem(11007)->SetWindowTextW(powerStr);

			GetDlgItem(11009)->SetWindowTextW(_T("N/A"));
			GetDlgItem(11011)->SetWindowTextW(_T("N/A"));
			GetDlgItem(11013)->SetWindowTextW(_T("N/A"));

			for(int count1 = 11002; count1 <= 11007; count1++)
			{
				GetDlgItem(count1)->EnableWindow(TRUE);
			}
			for(int count2 = 11008; count2 <= 11013; count2++)
			{
				GetDlgItem(count2)->EnableWindow(FALSE);
			}
		}

		EnableAllMainTabControls();
	}

	else if(fractalTypeStr == "Mandelbrot (Quick)")
	{
		theApp.pCheckBox16005->EnableWindow(TRUE);

		if(theApp.m_bSwitchMode == FALSE)
		{
			GetDlgItem(11002)->SetWindowTextW(_T("Start (X) :"));
			GetDlgItem(11004)->SetWindowTextW(_T("Start (Y) :"));
			GetDlgItem(11006)->SetWindowTextW(_T("Control #3 :"));
			GetDlgItem(11008)->SetWindowTextW(_T("Control #4 :"));
			GetDlgItem(11010)->SetWindowTextW(_T("Control #5 :"));
			GetDlgItem(11012)->SetWindowTextW(_T("Control #6 :"));

			GetDlgItem(11003)->SetWindowTextW(_T("0.0"));
			GetDlgItem(11005)->SetWindowTextW(_T("0.0"));
			GetDlgItem(11007)->SetWindowTextW(_T("N/A"));
			GetDlgItem(11009)->SetWindowTextW(_T("N/A"));
			GetDlgItem(11011)->SetWindowTextW(_T("N/A"));
			GetDlgItem(11013)->SetWindowTextW(_T("N/A"));

			for(int count1 = 11002; count1 <= 11005; count1++)
			{
				GetDlgItem(count1)->EnableWindow(TRUE);
			}
			for(int count2 = 11006; count2 <= 11013; count2++)
			{
				GetDlgItem(count2)->EnableWindow(FALSE);
			}
		}
		else if(theApp.m_bSwitchMode == TRUE)
		{
			GetDlgItem(11002)->SetWindowTextW(_T("Seed (Re) :"));
			GetDlgItem(11004)->SetWindowTextW(_T("Seed (Im) :"));
			GetDlgItem(11006)->SetWindowTextW(_T("Control #3 :"));
			GetDlgItem(11008)->SetWindowTextW(_T("Control #4 :"));
			GetDlgItem(11010)->SetWindowTextW(_T("Control #5 :"));
			GetDlgItem(11012)->SetWindowTextW(_T("Control #6 :"));

			GetDlgItem(11003)->SetWindowTextW(_T("0.0"));
			GetDlgItem(11005)->SetWindowTextW(_T("0.0"));
			GetDlgItem(11007)->SetWindowTextW(_T("N/A"));
			GetDlgItem(11009)->SetWindowTextW(_T("N/A"));
			GetDlgItem(11011)->SetWindowTextW(_T("N/A"));
			GetDlgItem(11013)->SetWindowTextW(_T("N/A"));

			for(int count1 = 11002; count1 <= 11005; count1++)
			{
				GetDlgItem(count1)->EnableWindow(TRUE);
			}
			for(int count2 = 11006; count2 <= 11013; count2++)
			{
				GetDlgItem(count2)->EnableWindow(FALSE);
			}
		}

		EnableAllMainTabControls();
	}

	else if(fractalTypeStr == "Julia")
	{
		theApp.pCheckBox16005->EnableWindow(FALSE);
		theApp.pCheckBox16005->SetCheck(FALSE);

		GetDlgItem(11002)->SetWindowTextW(_T("Seed (Re) :"));
		GetDlgItem(11004)->SetWindowTextW(_T("Seed (Im) :"));
		GetDlgItem(11006)->SetWindowTextW(_T("Power of Z :"));
		GetDlgItem(11008)->SetWindowTextW(_T("Control #4 :"));
		GetDlgItem(11010)->SetWindowTextW(_T("Control #5 :"));
		GetDlgItem(11012)->SetWindowTextW(_T("Control #6 :"));

		GetDlgItem(11003)->SetWindowTextW(_T("-0.235"));
		GetDlgItem(11005)->SetWindowTextW(_T("0.8"));
		GetDlgItem(11007)->SetWindowTextW(_T("2.0"));
		GetDlgItem(11009)->SetWindowTextW(_T("N/A"));
		GetDlgItem(11011)->SetWindowTextW(_T("N/A"));
		GetDlgItem(11013)->SetWindowTextW(_T("N/A"));

		for(int count1 = 11002; count1 <= 11007; count1++)
		{
			GetDlgItem(count1)->EnableWindow(TRUE);
		}
		for(int count2 = 11008; count2 <= 11013; count2++)
		{
			GetDlgItem(count2)->EnableWindow(FALSE);
		}

		EnableAllMainTabControls();
	}

	else if(fractalTypeStr == "Julia (Quick)")
	{
		theApp.pCheckBox16005->EnableWindow(FALSE);
		theApp.pCheckBox16005->SetCheck(FALSE);

		GetDlgItem(11002)->SetWindowTextW(_T("Seed (Re) :"));
		GetDlgItem(11004)->SetWindowTextW(_T("Seed (Im) :"));
		GetDlgItem(11006)->SetWindowTextW(_T("Control #3 :"));
		GetDlgItem(11008)->SetWindowTextW(_T("Control #4 :"));
		GetDlgItem(11010)->SetWindowTextW(_T("Control #5 :"));
		GetDlgItem(11012)->SetWindowTextW(_T("Control #6 :"));

		GetDlgItem(11003)->SetWindowTextW(_T("-0.235"));
		GetDlgItem(11005)->SetWindowTextW(_T("0.8"));
		GetDlgItem(11007)->SetWindowTextW(_T("N/A"));
		GetDlgItem(11009)->SetWindowTextW(_T("N/A"));
		GetDlgItem(11011)->SetWindowTextW(_T("N/A"));
		GetDlgItem(11013)->SetWindowTextW(_T("N/A"));

		for(int count1 = 11002; count1 <= 11005; count1++)
		{
			GetDlgItem(count1)->EnableWindow(TRUE);
		}
		for(int count2 = 11006; count2 <= 11013; count2++)
		{
			GetDlgItem(count2)->EnableWindow(FALSE);
		}

		EnableAllMainTabControls();
	}

	else if(fractalTypeStr == "Newton")
	{
		theApp.pCheckBox16005->EnableWindow(FALSE);
		theApp.pCheckBox16005->SetCheck(FALSE);

		GetDlgItem(11002)->SetWindowTextW(_T("Relax (Re) :"));
		GetDlgItem(11004)->SetWindowTextW(_T("Relax (Im) :"));
		GetDlgItem(11006)->SetWindowTextW(_T("Power of Z :"));
		GetDlgItem(11008)->SetWindowTextW(_T("Control #4 :"));
		GetDlgItem(11010)->SetWindowTextW(_T("Control #5 :"));
		GetDlgItem(11012)->SetWindowTextW(_T("Control #6 :"));

		GetDlgItem(11003)->SetWindowTextW(_T("1.0"));
		GetDlgItem(11005)->SetWindowTextW(_T("0.0"));
		GetDlgItem(11007)->SetWindowTextW(_T("3.0"));
		GetDlgItem(11009)->SetWindowTextW(_T("N/A"));
		GetDlgItem(11011)->SetWindowTextW(_T("N/A"));
		GetDlgItem(11013)->SetWindowTextW(_T("N/A"));

		for(int count1 = 11002; count1 <= 11007; count1++)
		{
			GetDlgItem(count1)->EnableWindow(TRUE);
		}
		for(int count2 = 11008; count2 <= 11013; count2++)
		{
			GetDlgItem(count2)->EnableWindow(FALSE);
		}

		EnableAllMainTabControls();
	}

	else if(fractalTypeStr == "Newton (Root)")
	{
		theApp.pCheckBox16005->EnableWindow(FALSE);
		theApp.pCheckBox16005->SetCheck(FALSE);

		GetDlgItem(11002)->SetWindowTextW(_T("Relax (Re) :"));
		GetDlgItem(11004)->SetWindowTextW(_T("Relax (Im) :"));
		GetDlgItem(11006)->SetWindowTextW(_T("Tolerance :"));
		GetDlgItem(11008)->SetWindowTextW(_T("Control #4 :"));
		GetDlgItem(11010)->SetWindowTextW(_T("Control #5 :"));
		GetDlgItem(11012)->SetWindowTextW(_T("Control #6 :"));

		GetDlgItem(11003)->SetWindowTextW(_T("1.0"));
		GetDlgItem(11005)->SetWindowTextW(_T("0.0"));
		GetDlgItem(11007)->SetWindowTextW(_T("0.0001"));
		GetDlgItem(11009)->SetWindowTextW(_T("N/A"));
		GetDlgItem(11011)->SetWindowTextW(_T("N/A"));
		GetDlgItem(11013)->SetWindowTextW(_T("N/A"));

		for(int count1 = 11002; count1 <= 11007; count1++)
		{
			GetDlgItem(count1)->EnableWindow(TRUE);
		}
		for(int count2 = 11008; count2 <= 11013; count2++)
		{
			GetDlgItem(count2)->EnableWindow(FALSE);
		}

		theApp.m_iIteration = 25;
		theApp.pEdit10013.SetWindowText(_T("25"));

		theApp.pStatic12002->EnableWindow(FALSE);
		theApp.m_iOutRedAdder = 255;
		theApp.pEdit12003->SetWindowText(_T("N/A"));
		theApp.pEdit12003->EnableWindow(FALSE);

		theApp.pStatic12004->EnableWindow(FALSE);
		theApp.m_iOutGreenAdder = 255;
		theApp.pEdit12005->SetWindowText(_T("N/A"));
		theApp.pEdit12005->EnableWindow(FALSE);

		theApp.pStatic12006->EnableWindow(FALSE);
		theApp.m_iOutBlueAdder = 255;
		theApp.pEdit12007->SetWindowText(_T("N/A"));
		theApp.pEdit12007->EnableWindow(FALSE);

		theApp.pStatic12008->EnableWindow(FALSE);
		theApp.pEdit12009->EnableWindow(FALSE);
		theApp.m_dOutRedMult = 0.0;
		theApp.pEdit12009->SetWindowText(_T("N/A"));

		theApp.pStatic12010->EnableWindow(FALSE);
		theApp.pEdit12011->EnableWindow(FALSE);
		theApp.m_dOutGreenMult = 0.0;
		theApp.pEdit12011->SetWindowText(_T("N/A"));

		theApp.pStatic12012->EnableWindow(FALSE);
		theApp.pEdit12013->EnableWindow(FALSE);
		theApp.m_dOutBlueMult = 0.0;
		theApp.pEdit12013->SetWindowText(_T("N/A"));

		theApp.pButton12015->EnableWindow(FALSE);


		theApp.pStatic13002->EnableWindow(TRUE);
		theApp.m_iInRedAdder = 0;
		theApp.pEdit13003->SetWindowText(_T("0"));
		theApp.pEdit13003->EnableWindow(TRUE);

		theApp.pStatic13004->EnableWindow(TRUE);
		theApp.m_iInGreenAdder = 0;
		theApp.pEdit13005->SetWindowText(_T("0"));
		theApp.pEdit13005->EnableWindow(TRUE);

		theApp.pStatic13006->EnableWindow(TRUE);
		theApp.m_iInBlueAdder = 0;
		theApp.pEdit13007->SetWindowText(_T("0"));
		theApp.pEdit13007->EnableWindow(TRUE);

		theApp.pStatic13008->EnableWindow(FALSE);
		theApp.pEdit13009->EnableWindow(FALSE);
		theApp.m_dInRedMult = 0.0;
		theApp.pEdit13009->SetWindowText(_T("N/A"));

		theApp.pStatic13010->EnableWindow(FALSE);
		theApp.pEdit13011->EnableWindow(FALSE);
		theApp.m_dInGreenMult = 0.0;
		theApp.pEdit13011->SetWindowText(_T("N/A"));

		theApp.pStatic13012->EnableWindow(FALSE);
		theApp.pEdit13013->EnableWindow(FALSE);
		theApp.m_dInBlueMult = 0.0;
		theApp.pEdit13013->SetWindowText(_T("N/A"));

		theApp.pButton13015->EnableWindow(FALSE);

	}

	else if(fractalTypeStr == "Nova")
	{
		theApp.pCheckBox16005->EnableWindow(FALSE);
		theApp.pCheckBox16005->SetCheck(FALSE);

		GetDlgItem(11002)->SetWindowTextW(_T("Seed (Re) :"));
		GetDlgItem(11004)->SetWindowTextW(_T("Seed (Im) :"));
		GetDlgItem(11006)->SetWindowTextW(_T("Relax (Re) :"));
		GetDlgItem(11008)->SetWindowTextW(_T("Relax (Im) :"));
		GetDlgItem(11010)->SetWindowTextW(_T("Power of Z :"));
		GetDlgItem(11012)->SetWindowTextW(_T("Deriv. Mult. :"));

		GetDlgItem(11003)->SetWindowTextW(_T("1.0"));
		GetDlgItem(11005)->SetWindowTextW(_T("1.0"));
		GetDlgItem(11007)->SetWindowTextW(_T("3.0"));
		GetDlgItem(11009)->SetWindowTextW(_T("2.0"));
		GetDlgItem(11011)->SetWindowTextW(_T("3.0"));
		GetDlgItem(11013)->SetWindowTextW(_T("1.0"));

		for(int count1 = 11002; count1 <= 11013; count1++)
		{
			GetDlgItem(count1)->EnableWindow(TRUE);
		}

		EnableAllMainTabControls();
	}

	else if(fractalTypeStr == "Nova (Special)")
	{
		theApp.pCheckBox16005->EnableWindow(FALSE);
		theApp.pCheckBox16005->SetCheck(FALSE);

		GetDlgItem(11002)->SetWindowTextW(_T("Relax (Re) :"));
		GetDlgItem(11004)->SetWindowTextW(_T("Relax (Im) :"));
		GetDlgItem(11006)->SetWindowTextW(_T("Power of Z :"));
		GetDlgItem(11008)->SetWindowTextW(_T("Deriv. Mult. :"));
		GetDlgItem(11010)->SetWindowTextW(_T("Control #5 :"));
		GetDlgItem(11012)->SetWindowTextW(_T("Control #6 :"));

		GetDlgItem(11003)->SetWindowTextW(_T("7.0"));
		GetDlgItem(11005)->SetWindowTextW(_T("-3.0"));
		GetDlgItem(11007)->SetWindowTextW(_T("3.0"));
		GetDlgItem(11009)->SetWindowTextW(_T("2.0"));
		GetDlgItem(11011)->SetWindowTextW(_T("N/A"));
		GetDlgItem(11013)->SetWindowTextW(_T("N/A"));

		for(int count1 = 11002; count1 <= 11009; count1++)
		{
			GetDlgItem(count1)->EnableWindow(TRUE);
		}
		for(int count2 = 11010; count2 <= 11013; count2++)
		{
			GetDlgItem(count2)->EnableWindow(FALSE);
		}

		EnableAllMainTabControls();
	}

	else if(fractalTypeStr == "Glynn")
	{
		theApp.pCheckBox16005->EnableWindow(FALSE);
		theApp.pCheckBox16005->SetCheck(FALSE);

		GetDlgItem(11002)->SetWindowTextW(_T("Seed (Re) :"));
		GetDlgItem(11004)->SetWindowTextW(_T("Seed (Im) :"));
		GetDlgItem(11006)->SetWindowTextW(_T("Power of Z :"));
		GetDlgItem(11008)->SetWindowTextW(_T("Control #4 :"));
		GetDlgItem(11010)->SetWindowTextW(_T("Control #5 :"));
		GetDlgItem(11012)->SetWindowTextW(_T("Control #6 :"));

		GetDlgItem(11003)->SetWindowTextW(_T("-0.2"));
		GetDlgItem(11005)->SetWindowTextW(_T("0.0"));
		GetDlgItem(11007)->SetWindowTextW(_T("1.5"));
		GetDlgItem(11009)->SetWindowTextW(_T("N/A"));
		GetDlgItem(11011)->SetWindowTextW(_T("N/A"));
		GetDlgItem(11013)->SetWindowTextW(_T("N/A"));

		for(int count1 = 11002; count1 <= 11007; count1++)
		{
			GetDlgItem(count1)->EnableWindow(TRUE);
		}
		for(int count2 = 11008; count2 <= 11013; count2++)
		{
			GetDlgItem(count2)->EnableWindow(FALSE);
		}

		EnableAllMainTabControls();
	}

	else if(fractalTypeStr == "Phoenix")
	{
		theApp.pCheckBox16005->EnableWindow(FALSE);
		theApp.pCheckBox16005->SetCheck(FALSE);

		GetDlgItem(11002)->SetWindowTextW(_T("P1 (Re) :"));
		GetDlgItem(11004)->SetWindowTextW(_T("P1 (Im) :"));
		GetDlgItem(11006)->SetWindowTextW(_T("P2 (Re) :"));
		GetDlgItem(11008)->SetWindowTextW(_T("P2 (Im) :"));
		GetDlgItem(11010)->SetWindowTextW(_T("Power of Z :"));
		GetDlgItem(11012)->SetWindowTextW(_T("Control #6 :"));

		GetDlgItem(11003)->SetWindowTextW(_T("0.56667"));
		GetDlgItem(11005)->SetWindowTextW(_T("0.0"));
		GetDlgItem(11007)->SetWindowTextW(_T("0.0"));
		GetDlgItem(11009)->SetWindowTextW(_T("-0.5"));
		GetDlgItem(11011)->SetWindowTextW(_T("2.0"));
		GetDlgItem(11013)->SetWindowTextW(_T("N/A"));

		for(int count1 = 11002; count1 <= 11011; count1++)
		{
			GetDlgItem(count1)->EnableWindow(TRUE);
		}
		for(int count2 = 11012; count2 <= 11013; count2++)
		{
			GetDlgItem(count2)->EnableWindow(FALSE);
		}

		EnableAllMainTabControls();
	}

	else if(fractalTypeStr == "Cos(Tan(z))")
	{
		theApp.pCheckBox16005->EnableWindow(FALSE);
		theApp.pCheckBox16005->SetCheck(FALSE);

		GetDlgItem(11002)->SetWindowTextW(_T("Seed (Re) :"));
		GetDlgItem(11004)->SetWindowTextW(_T("Seed (Im) :"));
		GetDlgItem(11006)->SetWindowTextW(_T("Control #3 :"));
		GetDlgItem(11008)->SetWindowTextW(_T("Control #4 :"));
		GetDlgItem(11010)->SetWindowTextW(_T("Control #5 :"));
		GetDlgItem(11012)->SetWindowTextW(_T("Control #6 :"));

		GetDlgItem(11003)->SetWindowTextW(_T("0.0"));
		GetDlgItem(11005)->SetWindowTextW(_T("0.0"));
		GetDlgItem(11007)->SetWindowTextW(_T("N/A"));
		GetDlgItem(11009)->SetWindowTextW(_T("N/A"));
		GetDlgItem(11011)->SetWindowTextW(_T("N/A"));
		GetDlgItem(11013)->SetWindowTextW(_T("N/A"));

		for(int count1 = 11002; count1 <= 11005; count1++)
		{
			GetDlgItem(count1)->EnableWindow(TRUE);
		}
		for(int count2 = 11006; count2 <= 11013; count2++)
		{
			GetDlgItem(count2)->EnableWindow(FALSE);
		}

		EnableAllMainTabControls();
	}

	theApp.m_bInitAnimationFormula = TRUE;

//	GetDlgItem(ID_RENDER)->SetFocus();
}

void CFractoriaDlg::OnEnChangeRealAxis()
{
	CString RealAxisStr;
	GetDlgItem(10005)->GetWindowText(RealAxisStr);
	theApp.m_dXAxis = wcstod(RealAxisStr, NULL);

	m_dCoordXRounded = theApp.m_dXAxis;
	UpdateData(FALSE);
}

void CFractoriaDlg::OnEnChangeImagAxis()
{
	CString ImagAxisStr;
	GetDlgItem(10007)->GetWindowText(ImagAxisStr);
	theApp.m_dYAxis = wcstod(ImagAxisStr, NULL);

	m_dCoordYRounded = theApp.m_dYAxis;
	UpdateData(FALSE);
}

void CFractoriaDlg::OnEnChangeZoomRatio()
{
	CString ZoomRatioStr;
	GetDlgItem(10009)->GetWindowText(ZoomRatioStr);
	theApp.m_dZoomRatio = wcstod(ZoomRatioStr, NULL);
}

void CFractoriaDlg::OnEnChangeRotation()
{
	CString RotationStr;
	GetDlgItem(10011)->GetWindowText(RotationStr);
	theApp.m_dRotation = wcstod(RotationStr, NULL);
}

void CFractoriaDlg::OnEnChangeIteration()
{
	CString IterStr;
	GetDlgItem(10013)->GetWindowText(IterStr);
	theApp.m_iIteration = _ttoi(IterStr);
}

void CFractoriaDlg::OnEnChangeBailout()
{
	CString BailoutStr;
	GetDlgItem(10015)->GetWindowText(BailoutStr);
	theApp.m_dBailout = wcstod(BailoutStr, NULL);
}

void CFractoriaDlg::OnEnChangeControl1()
{
	CString control1Str;
	GetDlgItem(11003)->GetWindowText(control1Str);
	theApp.m_dControl1 = wcstod(control1Str, NULL);
}

void CFractoriaDlg::OnEnChangeControl2()
{
	CString control2Str;
	GetDlgItem(11005)->GetWindowText(control2Str);
	theApp.m_dControl2 = wcstod(control2Str, NULL);
}

void CFractoriaDlg::OnEnChangeControl3()
{
	CString control3Str;
	GetDlgItem(11007)->GetWindowText(control3Str);
	theApp.m_dControl3 = wcstod(control3Str, NULL);
}

void CFractoriaDlg::OnEnChangeControl4()
{
	CString control4Str;
	GetDlgItem(11009)->GetWindowText(control4Str);
	theApp.m_dControl4 = wcstod(control4Str, NULL);
}

void CFractoriaDlg::OnEnChangeControl5()
{
	CString control5Str;
	GetDlgItem(11011)->GetWindowText(control5Str);
	theApp.m_dControl5 = wcstod(control5Str, NULL);
}

void CFractoriaDlg::OnEnChangeControl6()
{
	CString control6Str;
	GetDlgItem(11013)->GetWindowText(control6Str);
	theApp.m_dControl6 = wcstod(control6Str, NULL);
}

void CFractoriaDlg::OnEnChangeOutRedAdder()
{
	CString OutRedAdderStr;
	GetDlgItem(12003)->GetWindowText(OutRedAdderStr);
	theApp.m_iOutRedAdder = _ttoi(OutRedAdderStr);
}

void CFractoriaDlg::OnEnChangeOutGreenAdder()
{
	CString OutGreenAdderStr;
	GetDlgItem(12005)->GetWindowText(OutGreenAdderStr);
	theApp.m_iOutGreenAdder = _ttoi(OutGreenAdderStr);
}

void CFractoriaDlg::OnEnChangeOutBlueAdder()
{
	CString OutBlueAdderStr;
	GetDlgItem(12007)->GetWindowText(OutBlueAdderStr);
	theApp.m_iOutBlueAdder = _ttoi(OutBlueAdderStr);
}

void CFractoriaDlg::OnEnChangeOutRedMult()
{
	CString OutRedMultStr;
	GetDlgItem(12009)->GetWindowText(OutRedMultStr);
	theApp.m_dOutRedMult = wcstod(OutRedMultStr, NULL);
}

void CFractoriaDlg::OnEnChangeOutGreenMult()
{
	CString OutGreenMultStr;
	GetDlgItem(12011)->GetWindowText(OutGreenMultStr);
	theApp.m_dOutGreenMult = wcstod(OutGreenMultStr, NULL);
}

void CFractoriaDlg::OnEnChangeOutBlueMult()
{
	CString OutBlueMultStr;
	GetDlgItem(12013)->GetWindowText(OutBlueMultStr);
	theApp.m_dOutBlueMult = wcstod(OutBlueMultStr, NULL);
}

void CFractoriaDlg::OnEnChangeInRedAdder()
{
	CString InRedAdderStr;
	GetDlgItem(13003)->GetWindowText(InRedAdderStr);
	theApp.m_iInRedAdder = _ttoi(InRedAdderStr);
}

void CFractoriaDlg::OnEnChangeInGreenAdder()
{
	CString InGreenAdderStr;
	GetDlgItem(13005)->GetWindowText(InGreenAdderStr);
	theApp.m_iInGreenAdder = _ttoi(InGreenAdderStr);
}

void CFractoriaDlg::OnEnChangeInBlueAdder()
{
	CString InBlueAdderStr;
	GetDlgItem(13007)->GetWindowText(InBlueAdderStr);
	theApp.m_iInBlueAdder = _ttoi(InBlueAdderStr);
}

void CFractoriaDlg::OnEnChangeInRedMult()
{
	CString InRedMultStr;
	GetDlgItem(13009)->GetWindowText(InRedMultStr);
	theApp.m_dInRedMult = wcstod(InRedMultStr, NULL);
}

void CFractoriaDlg::OnEnChangeInGreenMult()
{
	CString InGreenMultStr;
	GetDlgItem(13011)->GetWindowText(InGreenMultStr);
	theApp.m_dInGreenMult = wcstod(InGreenMultStr, NULL);
}

void CFractoriaDlg::OnEnChangeInBlueMult()
{
	CString InBlueMultStr;
	GetDlgItem(13013)->GetWindowText(InBlueMultStr);
	theApp.m_dInBlueMult = wcstod(InBlueMultStr, NULL);
}

BOOL CFractoriaDlg::PreTranslateMessage(MSG* pMsg) 
{
    if(pMsg->message==WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_F9)
		{
            OnPreview();
		}
        if(pMsg->wParam == VK_F5)
		{
			m_cMainTab.SetCurSel(0);
			OnChangeTab();
		}
        if(pMsg->wParam == VK_F6)
		{
            m_cMainTab.SetCurSel(1);
			OnChangeTab();
		}
        if(pMsg->wParam == VK_F7)
		{
            m_cMainTab.SetCurSel(2);
			OnChangeTab();
		}
        if(pMsg->wParam == VK_F8)
		{
            m_cMainTab.SetCurSel(3);
			OnChangeTab();
		}

    }	
    return CDialog::PreTranslateMessage(pMsg);
}

void CFractoriaDlg::OnPreview()
{
	int iFractalTypePreview;
	iFractalTypePreview = theApp.pComboBox10003->GetCurSel();

	if(theApp.pPreviewThread != NULL)
	{
		theApp.pPreviewThread->SuspendThread();
	}

	if(iFractalTypePreview == 0)	// Mandelbrot
	{
		theApp.pPreviewThread = AfxBeginThread(OnPreviewMandelbrot,NULL,THREAD_PRIORITY_NORMAL,0,0,NULL);
	}
	else if(iFractalTypePreview == 1)	// Mandelbrot Quick
	{
		theApp.pPreviewThread = AfxBeginThread(OnPreviewMandelbrotQuick,NULL,THREAD_PRIORITY_NORMAL,0,0,NULL);
	}
	else if(iFractalTypePreview == 2)	// Julia
	{
		theApp.pPreviewThread = AfxBeginThread(OnPreviewJulia,NULL,THREAD_PRIORITY_NORMAL,0,0,NULL);
	}
	else if(iFractalTypePreview == 3)	// Julia Quick
	{
		theApp.pPreviewThread = AfxBeginThread(OnPreviewJuliaQuick,NULL,THREAD_PRIORITY_NORMAL,0,0,NULL);
	}
	else if(iFractalTypePreview == 4)	// Newton
	{
		theApp.pPreviewThread = AfxBeginThread(OnPreviewNewton,NULL,THREAD_PRIORITY_NORMAL,0,0,NULL);
	}
	else if(iFractalTypePreview == 5)	// Newton Root
	{
		theApp.pPreviewThread = AfxBeginThread(OnPreviewNewtonRoot,NULL,THREAD_PRIORITY_NORMAL,0,0,NULL);
	}
	else if(iFractalTypePreview == 6)	// Nova
	{
		theApp.pPreviewThread = AfxBeginThread(OnPreviewNova,NULL,THREAD_PRIORITY_NORMAL,0,0,NULL);
	}
	else if(iFractalTypePreview == 7)	// Nova Special
	{
		theApp.pPreviewThread = AfxBeginThread(OnPreviewNovaSpecial,NULL,THREAD_PRIORITY_NORMAL,0,0,NULL);
	}
	else if(iFractalTypePreview == 8)	// Glynn
	{
		theApp.pPreviewThread = AfxBeginThread(OnPreviewGlynn,NULL,THREAD_PRIORITY_NORMAL,0,0,NULL);
	}
	else if(iFractalTypePreview == 9)	// Phoenix
	{
		theApp.pPreviewThread = AfxBeginThread(OnPreviewPhoenix,NULL,THREAD_PRIORITY_NORMAL,0,0,NULL);
	}
	else if(iFractalTypePreview == 10)	// Cos(Tan(z))
	{
		theApp.pPreviewThread = AfxBeginThread(OnPreviewCosTanZ,NULL,THREAD_PRIORITY_NORMAL,0,0,NULL);
	}
}

void CFractoriaDlg::OnBnClickedRender()
{
	int iFractalType;
	iFractalType = theApp.pComboBox10003->GetCurSel();

	
	if(theApp.pThread != NULL)
	{
		theApp.pThread->SuspendThread();
	}

	AfxBeginThread(SetMarqueeOn,NULL,THREAD_PRIORITY_LOWEST,0,0,NULL);

	if(iFractalType == 0)	// Mandelbrot
	{
		theApp.pThread = AfxBeginThread(OnMandelbrot,NULL,THREAD_PRIORITY_HIGHEST,0,0,NULL);
	}
	else if(iFractalType == 1)	// Mandelbrot Quick
	{
		theApp.pThread = AfxBeginThread(OnMandelbrotQuick,NULL,THREAD_PRIORITY_HIGHEST,0,0,NULL);
	}
	else if(iFractalType == 2)	// Julia
	{
		theApp.pThread = AfxBeginThread(OnJulia,NULL,THREAD_PRIORITY_HIGHEST,0,0,NULL);
	}
	else if(iFractalType == 3)	// Julia Quick
	{
		theApp.pThread = AfxBeginThread(OnJuliaQuick,NULL,THREAD_PRIORITY_HIGHEST,0,0,NULL);
//		theApp.pThread = AfxBeginThread(OnJuliaQuick,NULL,THREAD_PRIORITY_HIGHEST,0,CREATE_SUSPENDED,NULL);
//		theApp.pThread->m_bAutoDelete = FALSE;
//		theApp.pThread->ResumeThread();
	}
	else if(iFractalType == 4)	// Newton
	{
		theApp.pThread = AfxBeginThread(OnNewton,NULL,THREAD_PRIORITY_HIGHEST,0,0,NULL);
	}
	else if(iFractalType == 5)	// Newton Root
	{
		theApp.pThread = AfxBeginThread(OnNewtonRoot,NULL,THREAD_PRIORITY_HIGHEST,0,0,NULL);
	}
	else if(iFractalType == 6)	// Nova
	{
		theApp.pThread = AfxBeginThread(OnNova,NULL,THREAD_PRIORITY_HIGHEST,0,0,NULL);
	}
	else if(iFractalType == 7)	// Nova Special
	{
		theApp.pThread = AfxBeginThread(OnNovaSpecial,NULL,THREAD_PRIORITY_HIGHEST,0,0,NULL);
	}
	else if(iFractalType == 8)	// Glynn
	{
		theApp.pThread = AfxBeginThread(OnGlynn,NULL,THREAD_PRIORITY_HIGHEST,0,0,NULL);
	}
	else if(iFractalType == 9)	// Phoenix
	{
		theApp.pThread = AfxBeginThread(OnPhoenix,NULL,THREAD_PRIORITY_HIGHEST,0,0,NULL);
	}
	else if(iFractalType == 10)	// Cos(Tan(z))
	{
		theApp.pThread = AfxBeginThread(OnCosTanZ,NULL,THREAD_PRIORITY_HIGHEST,0,0,NULL);
	}
}

void CFractoriaDlg::OnBnClickedOutsideColorPalette()
{
	CString sRed;
	GetDlgItem(12003)->GetWindowText(sRed);
	int iRed = _ttoi(sRed);

	CString sGreen;
	GetDlgItem(12005)->GetWindowText(sGreen);
	int iGreen = _ttoi(sGreen);

	CString sBlue;
	GetDlgItem(12007)->GetWindowText(sBlue);
	int iBlue = _ttoi(sBlue);

	CColorDialog coldlg(RGB(iRed,iGreen,iBlue), CC_FULLOPEN | CC_RGBINIT, this);

	int nResponse = (int)coldlg.DoModal();

	if (nResponse == IDOK)
	{
		coldlg.GetColor();

		iRed = GetRValue(coldlg.GetColor());
		sRed.Format(_T("%d"), iRed);
		GetDlgItem(12003)->SetWindowText(sRed);

		iGreen = GetGValue(coldlg.GetColor());
		sGreen.Format(_T("%d"), iGreen);
		GetDlgItem(12005)->SetWindowText(sGreen);

		iBlue = GetBValue(coldlg.GetColor());
		sBlue.Format(_T("%d"), iBlue);
		GetDlgItem(12007)->SetWindowText(sBlue);

		GetDlgItem(ID_RENDER)->SetFocus();
	}
}

void CFractoriaDlg::OnBnClickedInsideColorPalette()
{
	CString sRed;
	GetDlgItem(13003)->GetWindowText(sRed);
	int iRed = _ttoi(sRed);

	CString sGreen;
	GetDlgItem(13005)->GetWindowText(sGreen);
	int iGreen = _ttoi(sGreen);

	CString sBlue;
	GetDlgItem(13007)->GetWindowText(sBlue);
	int iBlue = _ttoi(sBlue);

	CColorDialog coldlg(RGB(iRed,iGreen,iBlue), CC_FULLOPEN | CC_RGBINIT, this);

	int nResponse = (int)coldlg.DoModal();

	if (nResponse == IDOK)
	{
		coldlg.GetColor();

		iRed = GetRValue(coldlg.GetColor());
		sRed.Format(_T("%d"), iRed);
		GetDlgItem(13003)->SetWindowText(sRed);

		iGreen = GetGValue(coldlg.GetColor());
		sGreen.Format(_T("%d"), iGreen);
		GetDlgItem(13005)->SetWindowText(sGreen);

		iBlue = GetBValue(coldlg.GetColor());
		sBlue.Format(_T("%d"), iBlue);
		GetDlgItem(13007)->SetWindowText(sBlue);

		GetDlgItem(ID_RENDER)->SetFocus();
	}
}

void CFractoriaDlg::OnFileSaveImage()
{

	CDC *pDC;
	pDC = GetDC();
//***********************************************
//will captutre current active window

	CString str;
	str = "fractal.bmp";

	// szFilters is a text string that includes two file name filters: 
	// "*.my" for "MyType Files" and "*.*' for "All Files." 
	TCHAR szFilters[]= _T("Bitmap Files (*.bmp)|*.bmp|All Files (*.*)|*.*||"); 
 
	// Create an Open dialog; the default file name extension is ".my". 
	CFileDialog fileDlg(FALSE, _T("bmp"), _T("Untitled.bmp"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters); 
 
	// Display the file dialog. When user clicks OK, fileDlg.DoModal()  
	// returns IDOK. 
	if(fileDlg.DoModal() == IDOK)
	{
		CString pathName = fileDlg.GetPathName(); 
 
		// Implement opening and reading file in here. 
 
		// Change the window's title to the opened file's title. 

//		CString fileName = fileDlg.GetFileTitle(); 
 
//		SetWindowText(fileName); 

		WndToBmp(pDC,pathName);
	
		ReleaseDC(pDC);
	} 
}

BOOL CFractoriaDlg::WndToBmp(CDC *pDC, CString szFile)
{
//*****This Function was written by Shailesh Kanzariya....***********
//it will capture wnd and save into bmp file
	CString fname=szFile;
	CBitmap bmp,*pOldBmp;
	CRect rect;
	CWnd *pWnd;
	BOOL flg=0;
	CPalette pal;
	LOGPALETTE *pLp;

	if(pDC==NULL)
	{
//		AfxMessageBox("DC Not Found");
		return FALSE;
	}
	pWnd=pDC->GetWindow();

	pWnd->GetClientRect(&rect);

	if(fname.IsEmpty())
		return FALSE;

	CDC memdc;
	
	memdc.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC,900,600);
	pOldBmp=memdc.SelectObject(&bmp);
	memdc.BitBlt(-3,-3,903,603,pDC,0,0,SRCCOPY);

	if(pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
	{
		int nSize;
		nSize=sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * 256;
		pLp=(LOGPALETTE*) new BYTE[nSize];
		pLp->palVersion=0x300;
		pLp->palNumEntries=256;
		GetSystemPaletteEntries(pDC->m_hDC,0,256,pLp->palPalEntry);
		pal.CreatePalette(pLp);
		delete [] pLp;
	}
	
	/////////////////////////***
	memdc.SelectObject(pOldBmp);
	HANDLE hDIB=DDBToDIB(bmp,BI_RGB,&pal);
	if(hDIB==NULL)
		return FALSE;
//*************************************
	CFile m_file;
	if(!m_file.Open(fname,CFile::modeWrite | CFile::modeCreate,NULL))
		return FALSE;
	else
		flg=1;
	BITMAPFILEHEADER hdr;
	LPBITMAPINFOHEADER lpbi;
	
	lpbi=(LPBITMAPINFOHEADER ) hDIB;
	
	int nColors= 1 << lpbi->biBitCount;
	
	hdr.bfType= ((WORD) ('M' << 8) | 'B');
	hdr.bfSize=GlobalSize(hDIB) + sizeof(hdr);
	hdr.bfReserved1=0;
	hdr.bfReserved2=0;
	hdr.bfOffBits=(DWORD) (sizeof(hdr) + lpbi->biSize + nColors * sizeof(RGBQUAD));
	m_file.Write(&hdr,sizeof(hdr));
	m_file.Write(lpbi,GlobalSize(hDIB));
	m_file.Close();
//**************************************
	CBinFile  m_tempFile;
	BYTE dummy=0;//14
	BYTE pBuf[14];

	BOOL fres=m_tempFile.Open(fname,CFile::modeReadWrite | CFile::typeBinary);

	if(fres==0)
		return FALSE;

	UINT tt=m_tempFile.Read(pBuf,14);
	pBuf[13]=dummy;//will replace from 04 to 00
	m_tempFile.SeekToBegin();
	m_tempFile.Write(pBuf,14);

	m_tempFile.Close();

	return flg;
//*****This Function was written by Shailesh Kanzariya....***********
//it will capture wnd and save into bmp file

}

HANDLE CFractoriaDlg::DDBToDIB(CBitmap &bitmap, DWORD dwCompression, CPalette *pPal)
{

	BITMAP bm;
	BITMAPINFOHEADER bi;
	LPBITMAPINFOHEADER lpbi;
	DWORD dwLen;
	HANDLE hDIB;
	HANDLE handle;
	HDC hDC;
	HPALETTE hPal;


	ASSERT( bitmap.GetSafeHandle() );

	// The function has no arg for bitfields
	if( dwCompression == BI_BITFIELDS )
		return NULL;

	// If a palette has not been supplied use defaul palette
	hPal = (HPALETTE) pPal->GetSafeHandle();
	if (hPal==NULL)
		hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

	// Get bitmap information
	bitmap.GetObject(sizeof(bm),(LPSTR)&bm);

	// Initialize the bitmapinfoheader
	bi.biSize		= sizeof(BITMAPINFOHEADER);
	bi.biWidth		= bm.bmWidth;
	bi.biHeight 		= bm.bmHeight;
	bi.biPlanes 		= 1;
	bi.biBitCount		= bm.bmPlanes * bm.bmBitsPixel;			//bm.bmPlanes * bm.bmBitsPixel;
	bi.biCompression	= dwCompression;
	bi.biSizeImage		= 0;
	bi.biXPelsPerMeter	= 0;
	bi.biYPelsPerMeter	= 0;
	bi.biClrUsed		= 0;
	bi.biClrImportant	= 0;

	// Compute the size of the  infoheader and the color table
	int nColors = (1 << bi.biBitCount);
	if( nColors > 256 ) 
		nColors = 0;
	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

	// We need a device context to get the DIB from
	hDC = ::GetDC(NULL);
	hPal = SelectPalette(hDC,hPal,FALSE);
	RealizePalette(hDC);

	// Allocate enough memory to hold bitmapinfoheader and color table
	hDIB = GlobalAlloc(GMEM_FIXED,dwLen);

	if (!hDIB){
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	*lpbi = bi;

	// Call GetDIBits with a NULL lpBits param, so the device driver 
	// will calculate the biSizeImage field 
	GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(), 0L, (DWORD)bi.biHeight,
			(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	// If the driver did not fill in the biSizeImage field, then compute it
	// Each scan line of the image is aligned on a DWORD (32bit) boundary
	if (bi.biSizeImage == 0){
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) 
						* bi.biHeight;

		// If a compression scheme is used the result may infact be larger
		// Increase the size to account for this.
		if (dwCompression != BI_RGB)
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}

	// Realloc the buffer so that it can hold all the bits
	dwLen += bi.biSizeImage;
	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
		hDIB = handle;
	else{
		GlobalFree(hDIB);

		// Reselect the original palette
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	// Get the bitmap bits
	lpbi = (LPBITMAPINFOHEADER)hDIB;

	// FINALLY get the DIB
	BOOL bGotBits = GetDIBits( hDC, (HBITMAP)bitmap.GetSafeHandle(),
				0L,				// Start scan line
				(DWORD)bi.biHeight,		// # of scan lines
				(LPBYTE)lpbi 			// address for bitmap bits
				+ (bi.biSize + nColors * sizeof(RGBQUAD)),
				(LPBITMAPINFO)lpbi,		// address of bitmapinfo
				(DWORD)DIB_RGB_COLORS);		// Use RGB for color table

	if( !bGotBits )
	{
		GlobalFree(hDIB);
		
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	SelectPalette(hDC,hPal,FALSE);
	::ReleaseDC(NULL,hDC);

	return hDIB;
}

void CFractoriaDlg::OnBnClickedSuspendRender()
{
	if(theApp.pThread != NULL)
	{
		theApp.pThread->SuspendThread();

		theApp.pProgress.SetMarquee(FALSE,0);
		theApp.pProgress.Invalidate(TRUE);
	}

	theApp.pThread = NULL;
}

void CFractoriaDlg::OnBnClickedSwitchModeFractal()
{
	theApp.m_bSwitchMode = theApp.pCheckBox16005->GetCheck();

	OnCbnSelchangeFractalTypeCombo();
}

void CFractoriaDlg::OnPropertiesAnimation()
{
	CAnimationProperties animationPropertiesObj;
	animationPropertiesObj.DoModal();
}

void CFractoriaDlg::OnBnClickedAnimation()
{
	theApp.pStatic16002.SetWindowTextW(_T("0"));
	theApp.pStatic16003.SetWindowTextW(_T("0"));
	theApp.pStatic16004.SetWindowTextW(_T("0"));

	AfxBeginThread(SetMarqueeOn,NULL,THREAD_PRIORITY_LOWEST,0,0,NULL);

	theApp.pThread = AfxBeginThread(OnAnimationThread,NULL,THREAD_PRIORITY_HIGHEST,0,0,NULL);
}

void CFractoriaDlg::OnRenderingColoring()
{
	int iFractalType;
	iFractalType = theApp.pComboBox10003->GetCurSel();

	if(iFractalType == 5)
	{
		CNRootColoringProperties nRootcoloringPropertiesObj;
		nRootcoloringPropertiesObj.DoModal();
	}
	else
	{
		CColoringProperties coloringPropertiesObj;
		coloringPropertiesObj.DoModal();
	}
}

void CFractoriaDlg::EnableAllMainTabControls()
{
//	theApp.m_iIteration = 25;
//	theApp.pEdit10013.SetWindowText(_T("25"));

	theApp.pStatic12002->EnableWindow(TRUE);
	theApp.m_iOutRedAdder = 255;
	theApp.pEdit12003->SetWindowText(_T("255"));
	theApp.pEdit12003->EnableWindow(TRUE);

	theApp.pStatic12004->EnableWindow(TRUE);
	theApp.m_iOutGreenAdder = 255;
	theApp.pEdit12005->SetWindowText(_T("255"));
	theApp.pEdit12005->EnableWindow(TRUE);

	theApp.pStatic12006->EnableWindow(TRUE);
	theApp.m_iOutBlueAdder = 255;
	theApp.pEdit12007->SetWindowText(_T("255"));
	theApp.pEdit12007->EnableWindow(TRUE);

	theApp.pStatic12008->EnableWindow(TRUE);
	theApp.pEdit12009->EnableWindow(TRUE);
	theApp.m_dOutRedMult = 0.0;
	theApp.pEdit12009->SetWindowText(_T("0.0"));

	theApp.pStatic12010->EnableWindow(TRUE);
	theApp.pEdit12011->EnableWindow(TRUE);
	theApp.m_dOutGreenMult = 0.0;
	theApp.pEdit12011->SetWindowText(_T("0.0"));

	theApp.pStatic12012->EnableWindow(TRUE);
	theApp.pEdit12013->EnableWindow(TRUE);
	theApp.m_dOutBlueMult = 0.0;
	theApp.pEdit12013->SetWindowText(_T("0.0"));

	theApp.pButton12015->EnableWindow(TRUE);


	theApp.pStatic13002->EnableWindow(TRUE);
	theApp.m_iInRedAdder = 0;
	theApp.pEdit13003->SetWindowText(_T("0"));
	theApp.pEdit13003->EnableWindow(TRUE);

	theApp.pStatic13004->EnableWindow(TRUE);
	theApp.m_iInGreenAdder = 0;
	theApp.pEdit13005->SetWindowText(_T("0"));
	theApp.pEdit13005->EnableWindow(TRUE);

	theApp.pStatic13006->EnableWindow(TRUE);
	theApp.m_iInBlueAdder = 0;
	theApp.pEdit13007->SetWindowText(_T("0"));
	theApp.pEdit13007->EnableWindow(TRUE);

	theApp.pStatic13008->EnableWindow(TRUE);
	theApp.pEdit13009->EnableWindow(TRUE);
	theApp.m_dInRedMult = 0.0;
	theApp.pEdit13009->SetWindowText(_T("0.0"));

	theApp.pStatic13010->EnableWindow(TRUE);
	theApp.pEdit13011->EnableWindow(TRUE);
	theApp.m_dInGreenMult = 0.0;
	theApp.pEdit13011->SetWindowText(_T("0.0"));

	theApp.pStatic13012->EnableWindow(TRUE);
	theApp.pEdit13013->EnableWindow(TRUE);
	theApp.m_dInBlueMult = 0.0;
	theApp.pEdit13013->SetWindowText(_T("0.0"));

	theApp.pButton13015->EnableWindow(TRUE);

}
