// Fractoria.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CFractoriaApp:
// See Fractoria.cpp for the implementation of this class
//

class CFractoriaApp : public CWinApp
{
public:
	CFractoriaApp();


	CComboBox* pComboBox10003;	// Fractal Type ComboBox
	BOOL m_bInitAnimationFormula;


	// MAIN TAB VARIABLES AND COMPONENTS

	// Tab 1 - Geometry

	CEdit pEdit10005;
	double m_dXAxis;

	CEdit pEdit10007;
	double m_dYAxis;

	CEdit pEdit10009;
	double m_dZoomRatio;

	CEdit pEdit10011;
	double m_dRotation;

	CEdit pEdit10013;
	int m_iIteration;

	double m_dBailout;


	// Tab 2 - Formula

	CEdit pEdit11003;
	double m_dControl1;

	CEdit pEdit11005;
	double m_dControl2;

	CEdit pEdit11007;
	double m_dControl3;

	CEdit pEdit11009;
	double m_dControl4;

	CEdit pEdit11011;
	double m_dControl5;

	CEdit pEdit11013;
	double m_dControl6;


	// Tab 3 - Outside Coloring

	CStatic* pStatic12002;
	CEdit* pEdit12003;
	int m_iOutRedAdder;

	CStatic* pStatic12004;
	CEdit* pEdit12005;
	int m_iOutBlueAdder;

	CStatic* pStatic12006;
	CEdit* pEdit12007;
	int m_iOutGreenAdder;

	CStatic* pStatic12008;
	CEdit* pEdit12009;
	double m_dOutRedMult;

	CStatic* pStatic12010;
	CEdit* pEdit12011;
	double m_dOutGreenMult;

	CStatic* pStatic12012;
	CEdit* pEdit12013;
	double m_dOutBlueMult;

	CButton* pButton12015;


	// Tab 4 - Inside Coloring

	CStatic* pStatic13002;
	CEdit* pEdit13003;
	int m_iInRedAdder;

	CStatic* pStatic13004;
	CEdit* pEdit13005;
	int m_iInGreenAdder;

	CStatic* pStatic13006;
	CEdit* pEdit13007;
	int m_iInBlueAdder;

	CStatic* pStatic13008;
	CEdit* pEdit13009;
	double m_dInRedMult;

	CStatic* pStatic13010;
	CEdit* pEdit13011;
	double m_dInGreenMult;

	CStatic* pStatic13012;
	CEdit* pEdit13013;
	double m_dInBlueMult;

	CButton* pButton13015;

	
	// Progress Control Variables

	CWinThread* pThread;
	CWinThread* pPreviewThread;

	CProgressCtrl pProgress;
	int m_iProgress;
	CStatic pStatic16002;
	int m_iPercentDone;
	CStatic pStatic16003;
	double m_dRenderTime;
	CStatic pStatic16004;
	double m_dRemainingTime;


	// COLORING TAB VARIABLES AND COMPONENTS

	// Coloring Algorithm Components
	
	CComboBox* pComboBox30003;
	int m_iColoringAlgorithmCurSel;

	CStatic* pStatic30004;
	CComboBox* pComboBox30005;
	int m_i2ColorGradientCurSel;
	
	CComboBox* pComboBox30006;
	int m_iSmoothGradientCurSel;

	CStatic* pStatic30008;
	CEdit* pEdit30009;
	double m_dSmoothRedValue;
	CStatic* pStatic30010;
	CEdit* pEdit30011;
	double m_dSmoothGreenValue;
	CStatic* pStatic30012;
	CEdit* pEdit30013;
	double m_dSmoothBlueValue;

	CButton* pButton30015;
	BOOL m_bMainColoringRed;
	CButton* pButton30016;
	BOOL m_bMainColoringGreen;
	CButton* pButton30017;
	BOOL m_bMainColoringBlue;

	// Newton Root Coloring Components
	CEdit* pEdit40003;
	int m_iFirstRootRed;
	CEdit* pEdit40005;
	int m_iFirstRootGreen;
	CEdit* pEdit40007;
	int m_iFirstRootBlue;

	CEdit* pEdit40010;
	int m_iSecondRootRed;
	CEdit* pEdit40012;
	int m_iSecondRootGreen;
	CEdit* pEdit40014;
	int m_iSecondRootBlue;

	CEdit* pEdit40017;
	int m_iThirdRootRed;
	CEdit* pEdit40019;
	int m_iThirdRootGreen;
	CEdit* pEdit40021;
	int m_iThirdRootBlue;


	// ANIMATION TAB VARIABLES AND COMPONENTS

	// Animation - Geometry Components

	int m_iFrameCount;

	int m_iTotalFrame;

	int m_iFramePoint[8];

	double m_dEndXAxis[8];
	double m_dEndYAxis[8];
	int m_iTranslationFunction[8];

	double m_dEndZoomRatio[8];
	int m_iZoomRatioFunction[8];

	double m_dEndRotation[8];
	int m_iRotationFunction[8];

	int m_iEndIteration[8];
	int m_iIterationFunction[8];


	// Animation - Formula Tab Components

	CComboBox* pComboBox21005;
	CComboBox* pComboBox21009;
	CComboBox* pComboBox21013;
	CComboBox* pComboBox21017;
	CComboBox* pComboBox21021;
	CComboBox* pComboBox21025;

	double m_dAnimationControl1[8];
	int m_iAnimationControl1Function[8];
	double m_dAnimationControl2[8];
	int m_iAnimationControl2Function[8];
	double m_dAnimationControl3[8];
	int m_iAnimationControl3Function[8];
	double m_dAnimationControl4[8];
	int m_iAnimationControl4Function[8];
	double m_dAnimationControl5[8];
	int m_iAnimationControl5Function[8];
	double m_dAnimationControl6[8];
	int m_iAnimationControl6Function[8];


	// Animation - Outside Coloring Tab Components

	CComboBox* pComboBox22005;
	CComboBox* pComboBox22009;
	CComboBox* pComboBox22013;
	CComboBox* pComboBox22017;
	CComboBox* pComboBox22021;
	CComboBox* pComboBox22025;

	double m_dAnimationOutsideRedAdder[8];
	int m_iAnimationOutsideRedAdderFunction[8];
	double m_dAnimationOutsideGreenAdder[8];
	int m_iAnimationOutsideGreenAdderFunction[8];
	double m_dAnimationOutsideBlueAdder[8];
	int m_iAnimationOutsideBlueAdderFunction[8];
	double m_dAnimationOutsideRedMult[8];
	int m_iAnimationOutsideRedMultFunction[8];
	double m_dAnimationOutsideGreenMult[8];
	int m_iAnimationOutsideGreenMultFunction[8];
	double m_dAnimationOutsideBlueMult[8];
	int m_iAnimationOutsideBlueMultFunction[8];


	// Animation - Inside Coloring Tab Components

	CComboBox* pComboBox23005;
	CComboBox* pComboBox23009;
	CComboBox* pComboBox23013;
	CComboBox* pComboBox23017;
	CComboBox* pComboBox23021;
	CComboBox* pComboBox23025;

	double m_dAnimationInsideRedAdder[8];
	int m_iAnimationInsideRedAdderFunction[8];
	double m_dAnimationInsideGreenAdder[8];
	int m_iAnimationInsideGreenAdderFunction[8];
	double m_dAnimationInsideBlueAdder[8];
	int m_iAnimationInsideBlueAdderFunction[8];
	double m_dAnimationInsideRedMult[8];
	int m_iAnimationInsideRedMultFunction[8];
	double m_dAnimationInsideGreenMult[8];
	int m_iAnimationInsideGreenMultFunction[8];
	double m_dAnimationInsideBlueMult[8];
	int m_iAnimationInsideBlueMultFunction[8];

	CProgressCtrl* pAnimationProgress;
//	int m_iProgress;


	CButton* pCheckBox16005;
	BOOL m_bSwitchMode;


// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CFractoriaApp theApp;