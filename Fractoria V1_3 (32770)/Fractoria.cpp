// Fractoria.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Fractoria.h"
#include "FractoriaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFractoriaApp

BEGIN_MESSAGE_MAP(CFractoriaApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFractoriaApp construction

CFractoriaApp::CFractoriaApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CFractoriaApp object

CFractoriaApp theApp;


// CFractoriaApp initialization

BOOL CFractoriaApp::InitInstance()
{

	pThread = NULL;
	pPreviewThread = NULL;

	m_bInitAnimationFormula = TRUE;

	m_dXAxis = 0.0;
	m_dYAxis = 0.0;
	m_dZoomRatio = 1.0;
	m_dRotation = 0.0;
	m_iIteration = 25;
	m_dBailout = 4.0;

	m_iFrameCount = 1;

	m_iTotalFrame = 1;

	for(int count = 0; count < 8; count++)
	{
		m_iFramePoint[count] = 0;
		m_dEndXAxis[count] = 0.0;
		m_dEndYAxis[count] = 0.0;
		m_iTranslationFunction[count] = 0;
		m_dEndZoomRatio[count] = 1.0;
		m_iZoomRatioFunction[count] = 0;
		m_dEndRotation[count] = 0.0;
		m_iRotationFunction[count] = 0;
		m_iEndIteration[count] = 25;
	}

	m_iColoringAlgorithmCurSel = 0;
	m_i2ColorGradientCurSel = 0;

	for(int count = 0; count < 8; count++)
	{
		m_dAnimationControl1[count] = 0.0;
		m_iAnimationControl1Function[count] = 0;
		m_dAnimationControl2[count] = 0.0;
		m_iAnimationControl2Function[count] = 0;
		m_dAnimationControl3[count] = 0.0;
		m_iAnimationControl3Function[count] = 0;
		m_dAnimationControl4[count] = 0.0;
		m_iAnimationControl4Function[count] = 0;
		m_dAnimationControl5[count] = 0.0;
		m_iAnimationControl5Function[count] = 0;
		m_dAnimationControl6[count] = 0.0;
		m_iAnimationControl6Function[count] = 0;
	}

	for(int count = 0; count < 8; count++)
	{
		m_dAnimationOutsideRedAdder[count] = 255.0;
		m_iAnimationOutsideRedAdderFunction[count] = 0;
		m_dAnimationOutsideGreenAdder[count] = 255.0;
		m_iAnimationOutsideGreenAdderFunction[count] = 0;
		m_dAnimationOutsideBlueAdder[count] = 255.0;
		m_iAnimationOutsideBlueAdderFunction[count] = 0;
		m_dAnimationOutsideRedMult[count] = 0.0;
		m_iAnimationOutsideRedMultFunction[count] = 0;
		m_dAnimationOutsideGreenMult[count] = 0.0;
		m_iAnimationOutsideGreenMultFunction[count] = 0;
		m_dAnimationOutsideBlueMult[count] = 0.0;
		m_iAnimationOutsideBlueMultFunction[count] = 0;
	}

	for(int count = 0; count < 8; count++)
	{
		m_dAnimationInsideRedAdder[count] = 0.0;
		m_iAnimationInsideRedAdderFunction[count] = 0;
		m_dAnimationInsideGreenAdder[count] = 0.0;
		m_iAnimationInsideGreenAdderFunction[count] = 0;
		m_dAnimationInsideBlueAdder[count] = 0.0;
		m_iAnimationInsideBlueAdderFunction[count] = 0;
		m_dAnimationInsideRedMult[count] = 0.0;
		m_iAnimationInsideRedMultFunction[count] = 0;
		m_dAnimationInsideGreenMult[count] = 0.0;
		m_iAnimationInsideGreenMultFunction[count] = 0;
		m_dAnimationInsideBlueMult[count] = 0.0;
		m_iAnimationInsideBlueMultFunction[count] = 0;
	}

	// Coloring Algorithm Components

	m_dSmoothRedValue = 10.0;
	m_dSmoothGreenValue = 10.0;
	m_dSmoothBlueValue = 10.0;

	m_bMainColoringRed = TRUE;
	m_bMainColoringGreen = TRUE;
	m_bMainColoringBlue = TRUE;

	// Newton Root Coloring
	m_iFirstRootRed = 15;
	m_iFirstRootGreen = 0;
	m_iFirstRootBlue = 0;
	m_iSecondRootRed = 0;
	m_iSecondRootGreen = 15;
	m_iSecondRootBlue = 0;
	m_iThirdRootRed = 0;
	m_iThirdRootGreen = 0;
	m_iThirdRootBlue = 15;



	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CFractoriaDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
