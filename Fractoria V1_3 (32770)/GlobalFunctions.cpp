// GlobalFunctions.cpp : implementation file
//

#include "stdafx.h"
#include "Fractoria.h"
#include "BinFile.h"
#include "GlobalFunctions.h"
#include "FractoriaDlg.h"


#define DEGtoRAD 0.01745329251994329576923690768489
#define PI 3.14159265358979323846264338327950288419


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;


double Round(double dbVal, int nPlaces)
{
    const double dbShift = pow(10.0, nPlaces);
    return  floor(dbVal * dbShift + 0.5) / dbShift; 
}

CString RemoveZeros(CString str)
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

UINT SetMarqueeOn(LPVOID Param)
{
	theApp.pProgress.SetMarquee(TRUE,20);

	return 0;
}

UINT OnMandelbrot(LPVOID Param)
{
	if(theApp.m_bSwitchMode == TRUE)
	{
		theApp.pThread = AfxBeginThread(OnJulia,NULL,THREAD_PRIORITY_NORMAL,0,0,NULL);
	}

	else if(theApp.m_bSwitchMode == FALSE)
	{
		CWnd* pWnd = CWnd::GetSafeOwner(0,0);
		CClientDC dc(pWnd);

		double a, b, t, tBU = 0, x, y, power;

		int xPix, yPix;
		double bailout = 4.0;

		int iOutRed, iOutGreen, iOutBlue;
		int iInRed, iInGreen, iInBlue;

		double xStart, yStart;
		double hyp, hx, hy = 0.0;
		double theta;

		complex<double> z;

		int width = 900;
		int height = 600;
		int xOffset = 3, yOffset = 3;

		clock_t start, temp, finish;

		double dXAxis = theApp.m_dXAxis;
		double dYAxis = theApp.m_dYAxis;
		double dZoomRatio = theApp.m_dZoomRatio;
		double dRotation = theApp.m_dRotation;
		int iIteration = theApp.m_iIteration;
		double dBailout = theApp.m_dBailout;

		int iInRedAdder = theApp.m_iInRedAdder;
		int iInGreenAdder = theApp.m_iInGreenAdder;
		int iInBlueAdder = theApp.m_iInBlueAdder;
		double dInRedMult = theApp.m_dInRedMult;
		double dInGreenMult = theApp.m_dInGreenMult;
		double dInBlueMult = theApp.m_dInBlueMult;

		int iOutRedAdder = theApp.m_iOutRedAdder;
		int iOutGreenAdder = theApp.m_iOutGreenAdder;
		int iOutBlueAdder = theApp.m_iOutBlueAdder;
		double dOutRedMult = theApp.m_dOutRedMult;
		double dOutGreenMult = theApp.m_dOutGreenMult;
		double dOutBlueMult = theApp.m_dOutBlueMult;


		a = theApp.m_dControl1;
		b = theApp.m_dControl2;
		power = theApp.m_dControl3;


		theApp.m_dRenderTime = 0;

		CString sRenderTime;
		sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
		sRenderTime = RemoveZeros(sRenderTime);
		theApp.pStatic16003.SetWindowTextW(sRenderTime);

		theApp.m_iProgress = -1;
		OnProgress();

		start = clock();


		hyp = sqrt(pow(3.0,2) + pow(2.0,2));
		theta = atan(2.0/3.0);

		xStart = dXAxis - hyp * cos(theta + (dRotation*DEGtoRAD))/dZoomRatio;
		yStart = dYAxis + hyp * sin(theta + (dRotation*DEGtoRAD))/dZoomRatio;


		for(yPix = 0; yPix < height; yPix++)
		{
			x = xStart - hy*sin(dRotation*DEGtoRAD);
			y = yStart - hy*cos(dRotation*DEGtoRAD);
			
			hx = 0.0;

			for(xPix = 0; xPix < width; xPix++)
			{
				z = complex<double>(a,b);

				for (t = 0; t < iIteration; t++)
				{
					complex<double> c(x,y);

					z = pow(z, power) + c;

					bailout = abs(z);

					if(bailout > dBailout)
					{
						tBU = t;
						t = iIteration;
					}
				}

				if(theApp.m_iColoringAlgorithmCurSel == 0)
				{
					iOutRed = (int)(dOutRedMult * bailout);
					iOutGreen = (int)(dOutGreenMult * bailout);
					iOutBlue = (int)(dOutBlueMult * bailout);

					if(iOutRed > 255){iOutRed = 255;}
					if(iOutGreen > 255){iOutGreen = 255;}
					if(iOutBlue > 255){iOutBlue = 255;}
				}
				else if(theApp.m_iColoringAlgorithmCurSel == 1)
				{
					if(theApp.m_i2ColorGradientCurSel == 0)
					{
						// Black to White
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = (int)(tBU*2);
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU >= 256)
						{
							iOutRed = 255;
							iOutGreen = 255;
							iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 1)
					{
						// Red to White
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
						}
						else if(tBU < 1024)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
						}
						else if(tBU < 2048)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
						}
						else if(tBU < 4096)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 2)
					{
						// Green to White
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = (int)(tBU*2);
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
						}
						else if(tBU < 1024)
						{
							iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
						}
						else if(tBU < 2048)
						{
							iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
						}
						else if(tBU < 4096)
						{
							iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 3)
					{
						// Blue to White
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU < 512)
						{
							iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutBlue = 255;
						}
						else if(tBU < 1024)
						{
							iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutBlue = 255;
						}
						else if(tBU < 2048)
						{
							iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutBlue = 255;
						}
						else if(tBU < 4096)
						{
							iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutBlue = 255;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 4)
					{
						// Yellow to White
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = (int)(tBU*2);
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = 255;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
						}
						else if(tBU < 1024)
						{
							iOutRed = 255;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
						}
						else if(tBU < 2048)
						{
							iOutRed = 255;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
						}
						else if(tBU < 4096)
						{
							iOutRed = 255;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 5)
					{
						// Purple to White
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = 0;
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = 0;
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU < 512)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutBlue = 255;
						}
						else if(tBU < 1024)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutBlue = 255;
						}
						else if(tBU < 2048)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutBlue = 255;
						}
						else if(tBU < 4096)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutBlue = 255;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 6)
					{
						// Cyan to White
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = (int)(tBU*2);
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU < 512)
						{
							iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutGreen = 255;
							iOutBlue = 255;
						}
						else if(tBU < 1024)
						{
							iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutGreen = 255;
							iOutBlue = 255;
						}
						else if(tBU < 2048)
						{
							iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutGreen = 255;
							iOutBlue = 255;
						}
						else if(tBU < 4096)
						{
							iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutGreen = 255;
							iOutBlue = 255;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 7)
					{
						// Red to Yellow
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutBlue = 0;
						}
						else if(tBU < 1024)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutBlue = 0;
						}
						else if(tBU < 2048)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutBlue = 0;
						}
						else if(tBU < 4096)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutBlue = 0;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 8)
					{
						// Red to Purple
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = 255;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
						}
						else if(tBU < 1024)
						{
							iOutRed = 255;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
						}
						else if(tBU < 2048)
						{
							iOutRed = 255;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
						}
						else if(tBU < 4096)
						{
							iOutRed = 255;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255;
							iOutGreen = 0;
							iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 9)
					{
						// Green to Yellow
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = int(tBU*2);
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutGreen = 255;
							iOutBlue = 0;
						}
						else if(tBU < 1024)
						{
							iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutGreen = 255;
							iOutBlue = 0;
						}
						else if(tBU < 2048)
						{
							iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutGreen = 255;
							iOutBlue = 0;
						}
						else if(tBU < 4096)
						{
							iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutGreen = 255;
							iOutBlue = 0;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 10)
					{
						// Green to Cyan
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = (int)(tBU*2);
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = 0;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
						}
						else if(tBU < 1024)
						{
							iOutRed = 0;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
						}
						else if(tBU < 2048)
						{
							iOutRed = 0;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
						}
						else if(tBU < 4096)
						{
							iOutRed = 0;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
						}
						else if(tBU >= 4096)
						{
							iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 11)
					{
						// Blue to Purple
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU < 512)
						{
							iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutGreen = 0;
							iOutBlue = 255;
						}
						else if(tBU < 1024)
						{
							iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutGreen = 0;
							iOutBlue = 255;
						}
						else if(tBU < 2048)
						{
							iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutGreen = 0;
							iOutBlue = 255;
						}
						else if(tBU < 4096)
						{
							iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutGreen = 0;
							iOutBlue = 255;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 12)
					{
						// Blue to Cyan
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU < 512)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutBlue = 255;
						}
						else if(tBU < 1024)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutBlue = 255;
						}
						else if(tBU < 2048)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutBlue = 255;
						}
						else if(tBU < 4096)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutBlue = 255;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
						}
					}

				}
				else if(theApp.m_iColoringAlgorithmCurSel == 2)
				{
					if(theApp.m_iSmoothGradientCurSel == 0)
					{// Smooth Coloring Function

						iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+1 - log(log(bailout))/log(power)));
						iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+1 - log(log(bailout))/log(power)));
						iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+1 - log(log(bailout))/log(power)));
					}
					else if(theApp.m_iSmoothGradientCurSel == 1)
					{
						if(theApp.m_bMainColoringRed == TRUE)
						{
							iOutRed = (int)(100 - theApp.m_dSmoothRedValue
											* (tBU+1 - log(log(bailout))/log(power)));
						}
						else
						{
							iOutRed = (int)(theApp.m_dSmoothRedValue
											* (tBU+1 - log(log(bailout))/log(power)));
						}

						if(theApp.m_bMainColoringGreen == TRUE)
						{
							iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
											* (tBU+1 - log(log(bailout))/log(power)));
						}
						else
						{
							iOutGreen = (int)(theApp.m_dSmoothGreenValue
											* (tBU+1 - log(log(bailout))/log(power)));
						}

						if(theApp.m_bMainColoringBlue == TRUE)
						{
							iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
											* (tBU+1 - log(log(bailout))/log(power)));
						}
						else
						{
							iOutBlue = (int)(theApp.m_dSmoothBlueValue
											* (tBU+1 - log(log(bailout))/log(power)));
						}
					}
				}


				iInRed = (int)(dInRedMult * bailout);
				iInGreen = (int)(dInGreenMult * bailout);
				iInBlue = (int)(dInBlueMult * bailout);

				if(iInRed > 255){iInRed = 255;}
				if(iInGreen > 255){iInGreen = 255;}
				if(iInBlue > 255){iInBlue = 255;}


				if(bailout < dBailout)
				{
					dc.SetPixel(xOffset+xPix,
								yOffset+yPix,
								RGB(iInRed + iInRedAdder,
									iInGreen + iInGreenAdder,
									iInBlue + iInBlueAdder));
				}
				else
				{
					dc.SetPixel(xOffset+xPix,
								yOffset+yPix,
								RGB(iOutRed + iOutRedAdder,
									iOutGreen + iOutGreenAdder,
									iOutBlue + iOutBlueAdder));
				}

				hx = (6.0/(double)width)/dZoomRatio;

				x = x + hx*cos(dRotation*DEGtoRAD);
				y = y - hx*sin(dRotation*DEGtoRAD);
			}

			hy = hy + (4.0/(double)height)/dZoomRatio;

			temp = clock();
			theApp.m_dRenderTime = Round(((double)(temp - start) / CLOCKS_PER_SEC), 3);
			OnProgress();

		}

		finish = clock();

		theApp.m_dRenderTime = Round(((double)(finish - start) / CLOCKS_PER_SEC), 3);

		sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
		sRenderTime = RemoveZeros(sRenderTime);
		theApp.pStatic16003.SetWindowTextW(sRenderTime);

		theApp.pProgress.SetMarquee(FALSE,0);
		theApp.pProgress.Invalidate(TRUE);
	}

	theApp.pThread = NULL;

	return 0;
}

UINT OnMandelbrotQuick(LPVOID Param)
{
	if(theApp.m_bSwitchMode == TRUE)
	{
		theApp.pThread = AfxBeginThread(OnJuliaQuick,NULL,THREAD_PRIORITY_NORMAL,0,0,NULL);
	}

	else if(theApp.m_bSwitchMode == FALSE)
	{
		CWnd* pWnd = CWnd::GetSafeOwner(0,0);
		CClientDC dc(pWnd);

		double a, b, d, q, t, tBU = 0, x, y;
		double aStart, bStart;

		int xPix, yPix;
		double bailout = 4.0;

		int iOutRed, iOutGreen, iOutBlue;
		int iInRed, iInGreen, iInBlue;

		double xStart, yStart;
		double hyp, hx, hy = 0.0;
		double theta;

		int width = 900;
		int height = 600;
		int xOffset = 3, yOffset = 3;

		clock_t start, temp, finish;

		double dXAxis = theApp.m_dXAxis;
		double dYAxis = theApp.m_dYAxis;
		double dZoomRatio = theApp.m_dZoomRatio;
		double dRotation = theApp.m_dRotation;
		int iIteration = theApp.m_iIteration;
		double dBailout = theApp.m_dBailout;

		int iInRedAdder = theApp.m_iInRedAdder;
		int iInGreenAdder = theApp.m_iInGreenAdder;
		int iInBlueAdder = theApp.m_iInBlueAdder;
		double dInRedMult = theApp.m_dInRedMult;
		double dInGreenMult = theApp.m_dInGreenMult;
		double dInBlueMult = theApp.m_dInBlueMult;

		int iOutRedAdder = theApp.m_iOutRedAdder;
		int iOutGreenAdder = theApp.m_iOutGreenAdder;
		int iOutBlueAdder = theApp.m_iOutBlueAdder;
		double dOutRedMult = theApp.m_dOutRedMult;
		double dOutGreenMult = theApp.m_dOutGreenMult;
		double dOutBlueMult = theApp.m_dOutBlueMult;



		aStart = theApp.m_dControl1;
		bStart = theApp.m_dControl2;

		theApp.m_dRenderTime = 0;

		CString sRenderTime;
		sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
		sRenderTime = RemoveZeros(sRenderTime);
		theApp.pStatic16003.SetWindowTextW(sRenderTime);

		theApp.m_iProgress = -1;
		OnProgress();

		start = clock();


		hyp = sqrt(pow(3.0,2) + pow(2.0,2));
		theta = atan(2.0/3.0);

		xStart = dXAxis - hyp * cos(theta + (dRotation*DEGtoRAD))/dZoomRatio;
		yStart = dYAxis + hyp * sin(theta + (dRotation*DEGtoRAD))/dZoomRatio;


		for(yPix = 0; yPix < height; yPix++)
		{
			x = xStart - hy*sin(dRotation*DEGtoRAD);
			y = yStart - hy*cos(dRotation*DEGtoRAD);
			
			hx = 0.0;

			for(xPix = 0; xPix < width; xPix++)
			{
				a = aStart;
				b = bStart;

				for (t = 0; t < iIteration; t++)
				{
					q = a;
		  			a = (a * a) - (b * b) + x;
					b = 2 * (q * b) + y;

					d = (a * a) + (b * b);

					bailout = (sqrt(d));

					if(bailout > dBailout)
					{
						tBU = t;
						t = iIteration;
					}
				}

				if(theApp.m_iColoringAlgorithmCurSel == 0)
				{
					iOutRed = (int)(dOutRedMult * bailout);
					iOutGreen = (int)(dOutGreenMult * bailout);
					iOutBlue = (int)(dOutBlueMult * bailout);

					if(iOutRed > 255){iOutRed = 255;}
					if(iOutGreen > 255){iOutGreen = 255;}
					if(iOutBlue > 255){iOutBlue = 255;}
				}
				else if(theApp.m_iColoringAlgorithmCurSel == 1)
				{
					if(theApp.m_i2ColorGradientCurSel == 0)
					{
						// Black to White
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = (int)(tBU*2);
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU >= 256)
						{
							iOutRed = 255;
							iOutGreen = 255;
							iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 1)
					{
						// Red to White
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
						}
						else if(tBU < 1024)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
						}
						else if(tBU < 2048)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
						}
						else if(tBU < 4096)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 2)
					{
						// Green to White
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = (int)(tBU*2);
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
						}
						else if(tBU < 1024)
						{
							iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
						}
						else if(tBU < 2048)
						{
							iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
						}
						else if(tBU < 4096)
						{
							iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 3)
					{
						// Blue to White
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU < 512)
						{
							iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutBlue = 255;
						}
						else if(tBU < 1024)
						{
							iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutBlue = 255;
						}
						else if(tBU < 2048)
						{
							iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutBlue = 255;
						}
						else if(tBU < 4096)
						{
							iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutBlue = 255;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 4)
					{
						// Yellow to White
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = (int)(tBU*2);
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = 255;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
						}
						else if(tBU < 1024)
						{
							iOutRed = 255;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
						}
						else if(tBU < 2048)
						{
							iOutRed = 255;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
						}
						else if(tBU < 4096)
						{
							iOutRed = 255;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 5)
					{
						// Purple to White
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = 0;
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = 0;
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU < 512)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutBlue = 255;
						}
						else if(tBU < 1024)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutBlue = 255;
						}
						else if(tBU < 2048)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutBlue = 255;
						}
						else if(tBU < 4096)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutBlue = 255;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 6)
					{
						// Cyan to White
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = (int)(tBU*2);
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU < 512)
						{
							iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutGreen = 255;
							iOutBlue = 255;
						}
						else if(tBU < 1024)
						{
							iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutGreen = 255;
							iOutBlue = 255;
						}
						else if(tBU < 2048)
						{
							iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutGreen = 255;
							iOutBlue = 255;
						}
						else if(tBU < 4096)
						{
							iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutGreen = 255;
							iOutBlue = 255;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 7)
					{
						// Red to Yellow
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutBlue = 0;
						}
						else if(tBU < 1024)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutBlue = 0;
						}
						else if(tBU < 2048)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutBlue = 0;
						}
						else if(tBU < 4096)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutBlue = 0;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 8)
					{
						// Red to Purple
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = 255;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
						}
						else if(tBU < 1024)
						{
							iOutRed = 255;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
						}
						else if(tBU < 2048)
						{
							iOutRed = 255;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
						}
						else if(tBU < 4096)
						{
							iOutRed = 255;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255;
							iOutGreen = 0;
							iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 9)
					{
						// Green to Yellow
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = int(tBU*2);
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutGreen = 255;
							iOutBlue = 0;
						}
						else if(tBU < 1024)
						{
							iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutGreen = 255;
							iOutBlue = 0;
						}
						else if(tBU < 2048)
						{
							iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutGreen = 255;
							iOutBlue = 0;
						}
						else if(tBU < 4096)
						{
							iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutGreen = 255;
							iOutBlue = 0;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 10)
					{
						// Green to Cyan
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = (int)(tBU*2);
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = 0;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
						}
						else if(tBU < 1024)
						{
							iOutRed = 0;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
						}
						else if(tBU < 2048)
						{
							iOutRed = 0;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
						}
						else if(tBU < 4096)
						{
							iOutRed = 0;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
						}
						else if(tBU >= 4096)
						{
							iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 11)
					{
						// Blue to Purple
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU < 512)
						{
							iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutGreen = 0;
							iOutBlue = 255;
						}
						else if(tBU < 1024)
						{
							iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutGreen = 0;
							iOutBlue = 255;
						}
						else if(tBU < 2048)
						{
							iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutGreen = 0;
							iOutBlue = 255;
						}
						else if(tBU < 4096)
						{
							iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutGreen = 0;
							iOutBlue = 255;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 12)
					{
						// Blue to Cyan
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU < 512)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutBlue = 255;
						}
						else if(tBU < 1024)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutBlue = 255;
						}
						else if(tBU < 2048)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutBlue = 255;
						}
						else if(tBU < 4096)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutBlue = 255;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
						}
					}

				}
				else if(theApp.m_iColoringAlgorithmCurSel == 2)
				{
					if(theApp.m_iSmoothGradientCurSel == 0)
					{// Smooth Coloring Function

						iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+1 - log(log(bailout))/log(2.0)));
						iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+1 - log(log(bailout))/log(2.0)));
						iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+1 - log(log(bailout))/log(2.0)));
					}
					else if(theApp.m_iSmoothGradientCurSel == 1)
					{
						if(theApp.m_bMainColoringRed == TRUE)
						{
							iOutRed = (int)(100 - theApp.m_dSmoothRedValue
											* (tBU+1 - log(log(bailout))/log(2.0)));
						}
						else
						{
							iOutRed = (int)(theApp.m_dSmoothRedValue
											* (tBU+1 - log(log(bailout))/log(2.0)));
						}

						if(theApp.m_bMainColoringGreen == TRUE)
						{
							iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
											* (tBU+1 - log(log(bailout))/log(2.0)));
						}
						else
						{
							iOutGreen = (int)(theApp.m_dSmoothGreenValue
											* (tBU+1 - log(log(bailout))/log(2.0)));
						}

						if(theApp.m_bMainColoringBlue == TRUE)
						{
							iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
											* (tBU+1 - log(log(bailout))/log(2.0)));
						}
						else
						{
							iOutBlue = (int)(theApp.m_dSmoothBlueValue
											* (tBU+1 - log(log(bailout))/log(2.0)));
						}
					}
				}


				iInRed = (int)(theApp.m_dInRedMult * bailout);
				iInGreen = (int)(theApp.m_dInGreenMult * bailout);
				iInBlue = (int)(theApp.m_dInBlueMult * bailout);

				if(iInRed > 255){iInRed = 255;}
				if(iInGreen > 255){iInGreen = 255;}
				if(iInBlue > 255){iInBlue = 255;}


				if(bailout < dBailout)
				{
					dc.SetPixel(xOffset+xPix,
								yOffset+yPix,
								RGB(iInRed + iInRedAdder,
									iInGreen + iInGreenAdder,
									iInBlue + iInBlueAdder));
				}
				else
				{
					dc.SetPixel(xOffset+xPix,
								yOffset+yPix,
								RGB(iOutRed + iOutRedAdder,
									iOutGreen + iOutGreenAdder,
									iOutBlue + iOutBlueAdder));
				}

				hx = (6.0/(double)width)/dZoomRatio;

				x = x + hx*cos(dRotation*DEGtoRAD);
				y = y - hx*sin(dRotation*DEGtoRAD);
			}

			hy = hy + (4.0/(double)height)/dZoomRatio;

			temp = clock();
			theApp.m_dRenderTime = Round(((double)(temp - start) / CLOCKS_PER_SEC), 3);
			OnProgress();

		}

		finish = clock();

		theApp.m_dRenderTime = Round(((double)(finish - start) / CLOCKS_PER_SEC), 3);

		sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
		sRenderTime = RemoveZeros(sRenderTime);
		theApp.pStatic16003.SetWindowTextW(sRenderTime);

		theApp.pProgress.SetMarquee(FALSE,0);
		theApp.pProgress.Invalidate(TRUE);
	}
	
	theApp.pThread = NULL;

	return 0;
}

UINT OnJulia(LPVOID Param)
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double a, b, t, tBU = 0, x, y, power;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 900;
	int height = 600;
	int xOffset = 3, yOffset = 3;

	clock_t start, temp, finish;

	double dXAxis = theApp.m_dXAxis;
	double dYAxis = theApp.m_dYAxis;
	double dZoomRatio = theApp.m_dZoomRatio;
	double dRotation = theApp.m_dRotation;
	int iIteration = theApp.m_iIteration;
	double dBailout = theApp.m_dBailout;

	int iInRedAdder = theApp.m_iInRedAdder;
	int iInGreenAdder = theApp.m_iInGreenAdder;
	int iInBlueAdder = theApp.m_iInBlueAdder;
	double dInRedMult = theApp.m_dInRedMult;
	double dInGreenMult = theApp.m_dInGreenMult;
	double dInBlueMult = theApp.m_dInBlueMult;

	int iOutRedAdder = theApp.m_iOutRedAdder;
	int iOutGreenAdder = theApp.m_iOutGreenAdder;
	int iOutBlueAdder = theApp.m_iOutBlueAdder;
	double dOutRedMult = theApp.m_dOutRedMult;
	double dOutGreenMult = theApp.m_dOutGreenMult;
	double dOutBlueMult = theApp.m_dOutBlueMult;



	a = theApp.m_dControl1;
	b = theApp.m_dControl2;
	power = theApp.m_dControl3;
	
	complex<double> c(a,b);

	theApp.m_dRenderTime = 0;

	CString sRenderTime;
	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	theApp.m_iProgress = -1;
	OnProgress();

	start = clock();


	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = dXAxis - hyp * cos(theta + (dRotation*DEGtoRAD))/dZoomRatio;
	yStart = dYAxis + hyp * sin(theta + (dRotation*DEGtoRAD))/dZoomRatio;


	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(dRotation*DEGtoRAD);
		y = yStart - hy*cos(dRotation*DEGtoRAD);
		
		hx = 0.0;

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			for (t = 0; t < iIteration; t++)
			{
				z = pow(z, power) + c;

				bailout = abs(z);

				if(bailout > dBailout)
				{
					tBU = t;
					t = iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(dOutRedMult * bailout);
				iOutGreen = (int)(dOutGreenMult * bailout);
				iOutBlue = (int)(dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+2 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}


			if(bailout < dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + iInRedAdder,
								iInGreen + iInGreenAdder,
								iInBlue + iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + iOutRedAdder,
								iOutGreen + iOutGreenAdder,
								iOutBlue + iOutBlueAdder));
			}

			hx = (6.0/(double)width)/dZoomRatio;

			x = x + hx*cos(dRotation*DEGtoRAD);
			y = y - hx*sin(dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/dZoomRatio;

		temp = clock();
		theApp.m_dRenderTime = Round(((double)(temp - start) / CLOCKS_PER_SEC), 3);
		OnProgress();

	}

	finish = clock();

	theApp.m_dRenderTime = Round(((double)(finish - start) / CLOCKS_PER_SEC), 3);

	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	OnProgress();

	theApp.pProgress.SetMarquee(FALSE,0);
	theApp.pProgress.Invalidate(TRUE);
	
	theApp.pThread = NULL;

	return 0;
}

UINT OnJuliaQuick(LPVOID Param)
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double a, b, d, e, f, q, t, tBU = 0, x, y;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	int width = 900;
	int height = 600;
	int xOffset = 3, yOffset = 3;

	clock_t start, temp, finish;

	double dXAxis = theApp.m_dXAxis;
	double dYAxis = theApp.m_dYAxis;
	double dZoomRatio = theApp.m_dZoomRatio;
	double dRotation = theApp.m_dRotation;
	int iIteration = theApp.m_iIteration;
	double dBailout = theApp.m_dBailout;

	int iInRedAdder = theApp.m_iInRedAdder;
	int iInGreenAdder = theApp.m_iInGreenAdder;
	int iInBlueAdder = theApp.m_iInBlueAdder;
	double dInRedMult = theApp.m_dInRedMult;
	double dInGreenMult = theApp.m_dInGreenMult;
	double dInBlueMult = theApp.m_dInBlueMult;

	int iOutRedAdder = theApp.m_iOutRedAdder;
	int iOutGreenAdder = theApp.m_iOutGreenAdder;
	int iOutBlueAdder = theApp.m_iOutBlueAdder;
	double dOutRedMult = theApp.m_dOutRedMult;
	double dOutGreenMult = theApp.m_dOutGreenMult;
	double dOutBlueMult = theApp.m_dOutBlueMult;



	a = theApp.m_dControl1;
	b = theApp.m_dControl2;

	theApp.m_dRenderTime = 0;

	CString sRenderTime;
	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	theApp.m_iProgress = -1;
	OnProgress();

	start = clock();


	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = dXAxis - hyp * cos(theta + (dRotation*DEGtoRAD))/dZoomRatio;
	yStart = dYAxis + hyp * sin(theta + (dRotation*DEGtoRAD))/dZoomRatio;


	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(dRotation*DEGtoRAD);
		y = yStart - hy*cos(dRotation*DEGtoRAD);
		
		hx = 0.0;

		for(xPix = 0; xPix < width; xPix++)
		{
			e = x;
			f = y;

			for (t = 0; t < iIteration; t++)
			{
				q = e;
	  			e = (e * e) - (f * f) + a;
					
				f = 2 * (q * f) + b;

				d = (e * e) + (f * f);

				bailout = (sqrt(d));

				if(bailout > dBailout)
				{
					tBU = t;
					t = iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(dOutRedMult * bailout);
				iOutGreen = (int)(dOutGreenMult * bailout);
				iOutBlue = (int)(dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+2 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}


			if(bailout < dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + iInRedAdder,
								iInGreen + iInGreenAdder,
								iInBlue + iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + iOutRedAdder,
								iOutGreen + iOutGreenAdder,
								iOutBlue + iOutBlueAdder));
			}

			hx = (6.0/(double)width)/dZoomRatio;

			x = x + hx*cos(dRotation*DEGtoRAD);
			y = y - hx*sin(dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/dZoomRatio;

		temp = clock();
		theApp.m_dRenderTime = Round(((double)(temp - start) / CLOCKS_PER_SEC), 3);
		OnProgress();

	}

	finish = clock();

	theApp.m_dRenderTime = Round(((double)(finish - start) / CLOCKS_PER_SEC), 3);

	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	OnProgress();

	theApp.pProgress.SetMarquee(FALSE,0);
	theApp.pProgress.Invalidate(TRUE);

	theApp.pThread = NULL;

	return 0;
}

UINT OnNewton(LPVOID Param)
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double t, x, y, power = 3;
	double dRelaxReal, dRelaxImag;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 900;
	int height = 600;
	int xOffset = 3, yOffset = 3;

	clock_t start, temp, finish;

	double dXAxis = theApp.m_dXAxis;
	double dYAxis = theApp.m_dYAxis;
	double dZoomRatio = theApp.m_dZoomRatio;
	double dRotation = theApp.m_dRotation;
	int iIteration = theApp.m_iIteration;
	double dBailout = theApp.m_dBailout;

	int iInRedAdder = theApp.m_iInRedAdder;
	int iInGreenAdder = theApp.m_iInGreenAdder;
	int iInBlueAdder = theApp.m_iInBlueAdder;
	double dInRedMult = theApp.m_dInRedMult;
	double dInGreenMult = theApp.m_dInGreenMult;
	double dInBlueMult = theApp.m_dInBlueMult;

	int iOutRedAdder = theApp.m_iOutRedAdder;
	int iOutGreenAdder = theApp.m_iOutGreenAdder;
	int iOutBlueAdder = theApp.m_iOutBlueAdder;
	double dOutRedMult = theApp.m_dOutRedMult;
	double dOutGreenMult = theApp.m_dOutGreenMult;
	double dOutBlueMult = theApp.m_dOutBlueMult;



	dRelaxReal = theApp.m_dControl1;
	dRelaxImag = theApp.m_dControl2;
	power = theApp.m_dControl3;

	theApp.m_dRenderTime = 0;

	CString sRenderTime;
	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	theApp.m_iProgress = -1;
	OnProgress();

	start = clock();


	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = dXAxis - hyp * cos(theta + (dRotation*DEGtoRAD))/dZoomRatio;
	yStart = dYAxis + hyp * sin(theta + (dRotation*DEGtoRAD))/dZoomRatio;

	z = complex<double>(xStart,yStart);

	complex<double> zGreen = complex<double>(-0.5, sin(2*3.1416/3));
	complex<double> zBlue = complex<double>(-0.5, -sin(2*3.1416/3));

	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(dRotation*DEGtoRAD);
		y = yStart - hy*cos(dRotation*DEGtoRAD);
		
		hx = 0.0;

		z = complex<double>(x,y);

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			for(t = 0; t < iIteration; t++)
			{
				z = z - complex<double>(dRelaxReal, dRelaxImag)
					* (pow(z,power)-1.0)/((double)power*pow(z,power-1.0));

				bailout = abs(z);

				if(bailout > dBailout)
				{
					t = iIteration;
				}
			}

			iOutRed = (int)(dOutRedMult * bailout);
			iOutGreen = (int)(dOutGreenMult * bailout);
			iOutBlue = (int)(dOutBlueMult * bailout);

			if(iOutRed > 255){iOutRed = 255;}
			if(iOutGreen > 255){iOutGreen = 255;}
			if(iOutBlue > 255){iOutBlue = 255;}


			iInRed = (int)(dInRedMult * bailout);
			iInGreen = (int)(dInGreenMult * bailout);
			iInBlue = (int)(dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}

			if(bailout < dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + iInRedAdder,
								iInGreen + iInGreenAdder,
								iInBlue + iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + iOutRedAdder,
								iOutGreen + iOutGreenAdder,
								iOutBlue + iOutBlueAdder));
			}

			hx = (6.0/(double)width)/dZoomRatio;

			x = x + hx*cos(dRotation*DEGtoRAD);
			y = y - hx*sin(dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/dZoomRatio;

		temp = clock();
		theApp.m_dRenderTime = Round(((double)(temp - start) / CLOCKS_PER_SEC), 3);
		OnProgress();

	}

	finish = clock();

	theApp.m_dRenderTime = Round(((double)(finish - start) / CLOCKS_PER_SEC), 3);

	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	OnProgress();

	theApp.pProgress.SetMarquee(FALSE,0);
	theApp.pProgress.Invalidate(TRUE);
	
	theApp.pThread = NULL;

	return 0;
}

UINT OnNewtonRoot(LPVOID Param)
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double t, x, y, power = 3.0;

	int xPix, yPix;
	double bailout = 4.0;

	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 900;
	int height = 600;
	int xOffset = 3, yOffset = 3;

	clock_t start, temp, finish;

	double dXAxis = theApp.m_dXAxis;
	double dYAxis = theApp.m_dYAxis;
	double dZoomRatio = theApp.m_dZoomRatio;
	double dRotation = theApp.m_dRotation;
	int iIteration = theApp.m_iIteration;
	double dBailout = theApp.m_dBailout;

	int iInRedAdder = theApp.m_iInRedAdder;
	int iInGreenAdder = theApp.m_iInGreenAdder;
	int iInBlueAdder = theApp.m_iInBlueAdder;
	double dInRedMult = theApp.m_dInRedMult;
	double dInGreenMult = theApp.m_dInGreenMult;
	double dInBlueMult = theApp.m_dInBlueMult;

	int iOutRedAdder = theApp.m_iOutRedAdder;
	int iOutGreenAdder = theApp.m_iOutGreenAdder;
	int iOutBlueAdder = theApp.m_iOutBlueAdder;
	double dOutRedMult = theApp.m_dOutRedMult;
	double dOutGreenMult = theApp.m_dOutGreenMult;
	double dOutBlueMult = theApp.m_dOutBlueMult;



	double dRelaxReal = theApp.m_dControl1;
	double dRelaxImag = theApp.m_dControl2;
	double tol = theApp.m_dControl3;

	theApp.m_dRenderTime = 0;

	CString sRenderTime;
	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	theApp.m_iProgress = -1;
	OnProgress();

	start = clock();


	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = dXAxis - hyp * cos(theta + (dRotation*DEGtoRAD))/dZoomRatio;
	yStart = dYAxis + hyp * sin(theta + (dRotation*DEGtoRAD))/dZoomRatio;

	z = complex<double>(xStart,yStart);

	complex<double> zGreen = complex<double>(-0.5, sin(2*3.1416/3));
	complex<double> zBlue = complex<double>(-0.5, -sin(2*3.1416/3));

	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(dRotation*DEGtoRAD);
		y = yStart - hy*cos(dRotation*DEGtoRAD);
		
		hx = 0.0;

		z = complex<double>(x,y);

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			t = 0;

			while((t < iIteration)
				&& (abs(z-1.0) >= tol)
				&& (abs(z-zGreen) >= tol)
				&& (abs(z-zBlue) >= tol))
			{
				z = z - complex<double>(dRelaxReal,dRelaxImag) * (z*z*z-1.0) / ((double)3.0*z*z);

				bailout = abs(z);

				t++;
			}

			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}

			if(abs(z - 1.0) < tol)
			{
				dc.SetPixel(xOffset+xPix, yOffset+yPix, RGB(t*theApp.m_iFirstRootRed + iInRedAdder,
															t*theApp.m_iFirstRootGreen + iInGreenAdder,
															t*theApp.m_iFirstRootBlue + iInBlueAdder));
			}
			else
			{
				iInRed = 0;
			}

			if(abs(z - zGreen) <= tol)
			{
				dc.SetPixel(xOffset+xPix, yOffset+yPix, RGB(t*theApp.m_iSecondRootRed + iInRedAdder,
															t*theApp.m_iSecondRootGreen + iInGreenAdder,
															t*theApp.m_iSecondRootBlue + iInBlueAdder));
			}
			else
			{
				iInGreen = 0;
			}

			if(abs(z - zBlue) <= tol)
			{
				dc.SetPixel(xOffset+xPix, yOffset+yPix, RGB(t*theApp.m_iThirdRootRed + iInRedAdder,
															t*theApp.m_iThirdRootGreen + iInGreenAdder,
															t*theApp.m_iThirdRootBlue + iInBlueAdder));
			}
			else
			{
				iInBlue = 0;
			}


			hx = (6.0/(double)width)/dZoomRatio;

			x = x + hx*cos(dRotation*DEGtoRAD);
			y = y - hx*sin(dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/dZoomRatio;

		temp = clock();
		theApp.m_dRenderTime = Round(((double)(temp - start) / CLOCKS_PER_SEC), 3);
		OnProgress();

	}

	finish = clock();

	theApp.m_dRenderTime = Round(((double)(finish - start) / CLOCKS_PER_SEC), 3);

	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	OnProgress();

	theApp.pProgress.SetMarquee(FALSE,0);
	theApp.pProgress.Invalidate(TRUE);
	
	theApp.pThread = NULL;

	return 0;
}

UINT OnNova(LPVOID Param)
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double t, tBU = 0, x, y;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 900;
	int height = 600;
	int xOffset = 3, yOffset = 3;

	clock_t start, temp, finish;

	double dXAxis = theApp.m_dXAxis;
	double dYAxis = theApp.m_dYAxis;
	double dZoomRatio = theApp.m_dZoomRatio;
	double dRotation = theApp.m_dRotation;
	int iIteration = theApp.m_iIteration;
	double dBailout = theApp.m_dBailout;

	int iInRedAdder = theApp.m_iInRedAdder;
	int iInGreenAdder = theApp.m_iInGreenAdder;
	int iInBlueAdder = theApp.m_iInBlueAdder;
	double dInRedMult = theApp.m_dInRedMult;
	double dInGreenMult = theApp.m_dInGreenMult;
	double dInBlueMult = theApp.m_dInBlueMult;

	int iOutRedAdder = theApp.m_iOutRedAdder;
	int iOutGreenAdder = theApp.m_iOutGreenAdder;
	int iOutBlueAdder = theApp.m_iOutBlueAdder;
	double dOutRedMult = theApp.m_dOutRedMult;
	double dOutGreenMult = theApp.m_dOutGreenMult;
	double dOutBlueMult = theApp.m_dOutBlueMult;


	double dSeeda = theApp.m_dControl1;
	double dSeedB = theApp.m_dControl2;

	double dRelaxReal = theApp.m_dControl3;
	double dRelaxImag = theApp.m_dControl4;

	double power = theApp.m_dControl5;

	double dDerivMult = theApp.m_dControl6;
	

	theApp.m_dRenderTime = 0;

	CString sRenderTime;
	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	theApp.m_iProgress = -1;
	OnProgress();

	start = clock();


	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = dXAxis - hyp * cos(theta + (dRotation*DEGtoRAD))/dZoomRatio;
	yStart = dYAxis + hyp * sin(theta + (dRotation*DEGtoRAD))/dZoomRatio;

	z = complex<double>(xStart,yStart);

	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(dRotation*DEGtoRAD);
		y = yStart - hy*cos(dRotation*DEGtoRAD);
		
		hx = 0.0;

		z = complex<double>(x,y);

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			for(t = 0; t < iIteration; t++)
			{
				z = z - complex<double>(dRelaxReal, dRelaxImag)
						* (pow(z,power)-1.0)/(dDerivMult*power*pow(z,power-1.0))
						+ complex<double>(dSeeda, dSeedB);

				bailout = abs(z);

				if(bailout > dBailout)
				{
					tBU = t;
					t = iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(dOutRedMult * bailout);
				iOutGreen = (int)(dOutGreenMult * bailout);
				iOutBlue = (int)(dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+1 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}

			if(bailout < dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + iInRedAdder,
								iInGreen + iInGreenAdder,
								iInBlue + iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + iOutRedAdder,
								iOutGreen + iOutGreenAdder,
								iOutBlue + iOutBlueAdder));
			}

			hx = (6.0/(double)width)/dZoomRatio;

			x = x + hx*cos(dRotation*DEGtoRAD);
			y = y - hx*sin(dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/dZoomRatio;

		temp = clock();
		theApp.m_dRenderTime = Round(((double)(temp - start) / CLOCKS_PER_SEC), 3);
		OnProgress();

	}

	finish = clock();

	theApp.m_dRenderTime = Round(((double)(finish - start) / CLOCKS_PER_SEC), 3);

	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	OnProgress();

	theApp.pProgress.SetMarquee(FALSE,0);
	theApp.pProgress.Invalidate(TRUE);
	
	theApp.pThread = NULL;

	return 0;
}

UINT OnNovaSpecial(LPVOID Param)
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double t, tBU = 0, x, y;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 900;
	int height = 600;
	int xOffset = 3, yOffset = 3;

	clock_t start, temp, finish;

	double dXAxis = theApp.m_dXAxis;
	double dYAxis = theApp.m_dYAxis;
	double dZoomRatio = theApp.m_dZoomRatio;
	double dRotation = theApp.m_dRotation;
	int iIteration = theApp.m_iIteration;
	double dBailout = theApp.m_dBailout;

	int iInRedAdder = theApp.m_iInRedAdder;
	int iInGreenAdder = theApp.m_iInGreenAdder;
	int iInBlueAdder = theApp.m_iInBlueAdder;
	double dInRedMult = theApp.m_dInRedMult;
	double dInGreenMult = theApp.m_dInGreenMult;
	double dInBlueMult = theApp.m_dInBlueMult;

	int iOutRedAdder = theApp.m_iOutRedAdder;
	int iOutGreenAdder = theApp.m_iOutGreenAdder;
	int iOutBlueAdder = theApp.m_iOutBlueAdder;
	double dOutRedMult = theApp.m_dOutRedMult;
	double dOutGreenMult = theApp.m_dOutGreenMult;
	double dOutBlueMult = theApp.m_dOutBlueMult;


	double dRelaxReal = theApp.m_dControl1;
	double dRelaxImag = theApp.m_dControl2;

	double power = theApp.m_dControl3;

	double dDerivMult = theApp.m_dControl4;
	

	theApp.m_dRenderTime = 0;

	CString sRenderTime;
	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	theApp.m_iProgress = -1;
	OnProgress();

	start = clock();


	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = dXAxis - hyp * cos(theta + (dRotation*DEGtoRAD))/dZoomRatio;
	yStart = dYAxis + hyp * sin(theta + (dRotation*DEGtoRAD))/dZoomRatio;

	z = complex<double>(xStart,yStart);

	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(dRotation*DEGtoRAD);
		y = yStart - hy*cos(dRotation*DEGtoRAD);
		
		hx = 0.0;

		z = complex<double>(x,y);

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			for(t = 0; t < iIteration; t++)
			{
				z = z - complex<double>(dRelaxReal, dRelaxImag)
						* (pow(z,power)-1.0)/(dDerivMult*power*pow(z,power-1.0))
						+ complex<double>(x,y);

				bailout = abs(z);

				if(bailout > dBailout)
				{
					tBU = t;
					t = iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(dOutRedMult * bailout);
				iOutGreen = (int)(dOutGreenMult * bailout);
				iOutBlue = (int)(dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+1 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}

			if(bailout < dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + iInRedAdder,
								iInGreen + iInGreenAdder,
								iInBlue + iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + iOutRedAdder,
								iOutGreen + iOutGreenAdder,
								iOutBlue + iOutBlueAdder));
			}

			hx = (6.0/(double)width)/dZoomRatio;

			x = x + hx*cos(dRotation*DEGtoRAD);
			y = y - hx*sin(dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/dZoomRatio;

		temp = clock();
		theApp.m_dRenderTime = Round(((double)(temp - start) / CLOCKS_PER_SEC), 3);
		OnProgress();

	}

	finish = clock();

	theApp.m_dRenderTime = Round(((double)(finish - start) / CLOCKS_PER_SEC), 3);

	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	OnProgress();

	theApp.pProgress.SetMarquee(FALSE,0);
	theApp.pProgress.Invalidate(TRUE);
	
	theApp.pThread = NULL;

	return 0;
}

UINT OnGlynn(LPVOID Param)
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double a, b, t, tBU = 0, x, y, power;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 900;
	int height = 600;
	int xOffset = 3, yOffset = 3;

	clock_t start, temp, finish;

	double dXAxis = theApp.m_dXAxis;
	double dYAxis = theApp.m_dYAxis;
	double dZoomRatio = theApp.m_dZoomRatio;
	double dRotation = theApp.m_dRotation;
	int iIteration = theApp.m_iIteration;
	double dBailout = theApp.m_dBailout;

	int iInRedAdder = theApp.m_iInRedAdder;
	int iInGreenAdder = theApp.m_iInGreenAdder;
	int iInBlueAdder = theApp.m_iInBlueAdder;
	double dInRedMult = theApp.m_dInRedMult;
	double dInGreenMult = theApp.m_dInGreenMult;
	double dInBlueMult = theApp.m_dInBlueMult;

	int iOutRedAdder = theApp.m_iOutRedAdder;
	int iOutGreenAdder = theApp.m_iOutGreenAdder;
	int iOutBlueAdder = theApp.m_iOutBlueAdder;
	double dOutRedMult = theApp.m_dOutRedMult;
	double dOutGreenMult = theApp.m_dOutGreenMult;
	double dOutBlueMult = theApp.m_dOutBlueMult;



	a = theApp.m_dControl1;
	b = theApp.m_dControl2;
	power = theApp.m_dControl3;
	
	complex<double> c(a,b);

	theApp.m_dRenderTime = 0;

	CString sRenderTime;
	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	theApp.m_iProgress = -1;
	OnProgress();

	start = clock();


	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = dXAxis - hyp * cos(theta + (dRotation*DEGtoRAD))/dZoomRatio;
	yStart = dYAxis + hyp * sin(theta + (dRotation*DEGtoRAD))/dZoomRatio;


	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(dRotation*DEGtoRAD);
		y = yStart - hy*cos(dRotation*DEGtoRAD);
		
		hx = 0.0;

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			for (t = 0; t < iIteration; t++)
			{
				z = pow(z, power) + c;

				bailout = abs(z);

				if(bailout > dBailout)
				{
					tBU = t;
					t = iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(dOutRedMult * bailout);
				iOutGreen = (int)(dOutGreenMult * bailout);
				iOutBlue = (int)(dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+1 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}


			if(bailout < dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + iInRedAdder,
								iInGreen + iInGreenAdder,
								iInBlue + iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + iOutRedAdder,
								iOutGreen + iOutGreenAdder,
								iOutBlue + iOutBlueAdder));
			}

			hx = (6.0/(double)width)/dZoomRatio;

			x = x + hx*cos(dRotation*DEGtoRAD);
			y = y - hx*sin(dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/dZoomRatio;

		temp = clock();

		theApp.m_dRenderTime = Round(((double)(temp - start) / CLOCKS_PER_SEC), 3);

		OnProgress();

	}

	finish = clock();

	theApp.m_dRenderTime = Round(((double)(finish - start) / CLOCKS_PER_SEC), 3);

	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	OnProgress();

	theApp.pProgress.SetMarquee(FALSE,0);
	theApp.pProgress.Invalidate(TRUE);
	
	theApp.pThread = NULL;

	return 0;
}

UINT OnPhoenix(LPVOID Param)
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double t, tBU = 0, x, y, power;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z, zBU, zBU2;

	int width = 900;
	int height = 600;
	int xOffset = 3, yOffset = 3;

	clock_t start, temp, finish;

	double dXAxis = theApp.m_dXAxis;
	double dYAxis = theApp.m_dYAxis;
	double dZoomRatio = theApp.m_dZoomRatio;
	double dRotation = theApp.m_dRotation;
	int iIteration = theApp.m_iIteration;
	double dBailout = theApp.m_dBailout;

	int iInRedAdder = theApp.m_iInRedAdder;
	int iInGreenAdder = theApp.m_iInGreenAdder;
	int iInBlueAdder = theApp.m_iInBlueAdder;
	double dInRedMult = theApp.m_dInRedMult;
	double dInGreenMult = theApp.m_dInGreenMult;
	double dInBlueMult = theApp.m_dInBlueMult;

	int iOutRedAdder = theApp.m_iOutRedAdder;
	int iOutGreenAdder = theApp.m_iOutGreenAdder;
	int iOutBlueAdder = theApp.m_iOutBlueAdder;
	double dOutRedMult = theApp.m_dOutRedMult;
	double dOutGreenMult = theApp.m_dOutGreenMult;
	double dOutBlueMult = theApp.m_dOutBlueMult;


	complex<double> p1(theApp.m_dControl1, theApp.m_dControl2);
	complex<double> p2(theApp.m_dControl3, theApp.m_dControl4);

	power = theApp.m_dControl5;

	theApp.m_dRenderTime = 0;

	CString sRenderTime;
	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	theApp.m_iProgress = -1;
	OnProgress();

	start = clock();


	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = dXAxis - hyp * cos(theta + (dRotation*DEGtoRAD))/dZoomRatio;
	yStart = dYAxis + hyp * sin(theta + (dRotation*DEGtoRAD))/dZoomRatio;


	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(dRotation*DEGtoRAD);
		y = yStart - hy*cos(dRotation*DEGtoRAD);
		
		hx = 0.0;

		for(xPix = 0; xPix < width; xPix++)
		{
//			z = complex<double>(0.0,0.0);
			z = complex<double>(x,y);

			for (t = 0; t < iIteration; t++)
			{
				if(t == 0)
				{
					zBU = z;
					zBU2 = complex<double>(0.0,0.0);
				}
				else
				{
					zBU2 = zBU;
					zBU = z;
				}

				z = pow(z, power) + p1 + real(p2)*zBU2 + imag(p2)*zBU2;

				bailout = abs(z);

				if(bailout > dBailout)
				{
					tBU = t;
					t = iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(dOutRedMult * bailout);
				iOutGreen = (int)(dOutGreenMult * bailout);
				iOutBlue = (int)(dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+2 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}


			if(bailout < 4.0)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + iInRedAdder,
								iInGreen + iInGreenAdder,
								iInBlue + iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + iOutRedAdder,
								iOutGreen + iOutGreenAdder,
								iOutBlue + iOutBlueAdder));
			}

			hx = (6.0/(double)width)/dZoomRatio;

			x = x + hx*cos(dRotation*DEGtoRAD);
			y = y - hx*sin(dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/dZoomRatio;

		temp = clock();
		theApp.m_dRenderTime = Round(((double)(temp - start) / CLOCKS_PER_SEC), 3);
		OnProgress();

	}

	finish = clock();

	theApp.m_dRenderTime = Round(((double)(finish - start) / CLOCKS_PER_SEC), 3);

	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	OnProgress();

	theApp.pProgress.SetMarquee(FALSE,0);
	theApp.pProgress.Invalidate(TRUE);
	
	theApp.pThread = NULL;

	return 0;
}

void OnProgress()
{
	theApp.m_iProgress++;

	theApp.m_iPercentDone = int(theApp.m_iProgress/6.0);
	
	if(theApp.m_iProgress % 5 == 0)
	{
		theApp.m_dRemainingTime = Round((theApp.m_dRenderTime
			/ double(theApp.m_iPercentDone)) * 100.0 - theApp.m_dRenderTime, 1);
	}

	CString sPercentDone;
	sPercentDone.Format(_T("%d"), theApp.m_iPercentDone);
	theApp.pStatic16002.SetWindowTextW(sPercentDone);

	CString sRenderTime;
	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	if( (theApp.m_dRemainingTime < 1000000000) && (theApp.m_dRemainingTime >= 0) )
	{
		CString sRemainingTime;
		sRemainingTime.Format(_T("%lf"), theApp.m_dRemainingTime);
		sRemainingTime = RemoveZeros(sRemainingTime);
		theApp.pStatic16004.SetWindowTextW(sRemainingTime);
	}
	else
	{
		CString sRemainingTime;
		sRemainingTime.Format(_T("%lf"), 0);
		sRemainingTime = RemoveZeros(sRemainingTime);
		theApp.pStatic16004.SetWindowTextW(sRemainingTime);
	}
}

UINT OnCosTanZ(LPVOID Param)
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double a, b, t, tBU = 0, x, y, power;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 900;
	int height = 600;
	int xOffset = 3, yOffset = 3;

	clock_t start, temp, finish;

	double dXAxis = theApp.m_dXAxis;
	double dYAxis = theApp.m_dYAxis;
	double dZoomRatio = theApp.m_dZoomRatio;
	double dRotation = theApp.m_dRotation;
	int iIteration = theApp.m_iIteration;
	double dBailout = theApp.m_dBailout;

	int iInRedAdder = theApp.m_iInRedAdder;
	int iInGreenAdder = theApp.m_iInGreenAdder;
	int iInBlueAdder = theApp.m_iInBlueAdder;
	double dInRedMult = theApp.m_dInRedMult;
	double dInGreenMult = theApp.m_dInGreenMult;
	double dInBlueMult = theApp.m_dInBlueMult;

	int iOutRedAdder = theApp.m_iOutRedAdder;
	int iOutGreenAdder = theApp.m_iOutGreenAdder;
	int iOutBlueAdder = theApp.m_iOutBlueAdder;
	double dOutRedMult = theApp.m_dOutRedMult;
	double dOutGreenMult = theApp.m_dOutGreenMult;
	double dOutBlueMult = theApp.m_dOutBlueMult;



	a = theApp.m_dControl1;
	b = theApp.m_dControl2;
	power = theApp.m_dControl3;
	
	complex<double> c(a,b);

	theApp.m_dRenderTime = 0;

	CString sRenderTime;
	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	theApp.m_iProgress = -1;
	OnProgress();

	start = clock();


	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = dXAxis - hyp * cos(theta + (dRotation*DEGtoRAD))/dZoomRatio;
	yStart = dYAxis + hyp * sin(theta + (dRotation*DEGtoRAD))/dZoomRatio;


	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(dRotation*DEGtoRAD);
		y = yStart - hy*cos(dRotation*DEGtoRAD);
		
		hx = 0.0;

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			for (t = 0; t < iIteration; t++)
			{
				z = cos(tan(z)) + c;

				bailout = abs(z);

				if(bailout > dBailout)
				{
					tBU = t;
					t = iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(dOutRedMult * bailout);
				iOutGreen = (int)(dOutGreenMult * bailout);
				iOutBlue = (int)(dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+2 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}


			if(bailout < dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + iInRedAdder,
								iInGreen + iInGreenAdder,
								iInBlue + iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + iOutRedAdder,
								iOutGreen + iOutGreenAdder,
								iOutBlue + iOutBlueAdder));
			}

			hx = (6.0/(double)width)/dZoomRatio;

			x = x + hx*cos(dRotation*DEGtoRAD);
			y = y - hx*sin(dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/dZoomRatio;

		temp = clock();
		theApp.m_dRenderTime = Round(((double)(temp - start) / CLOCKS_PER_SEC), 3);
		OnProgress();

	}

	finish = clock();

	theApp.m_dRenderTime = Round(((double)(finish - start) / CLOCKS_PER_SEC), 3);

	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	OnProgress();

	theApp.pProgress.SetMarquee(FALSE,0);
	theApp.pProgress.Invalidate(TRUE);
	
	theApp.pThread = NULL;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////

UINT OnPreviewMandelbrot(LPVOID Param)
{
	if(theApp.m_bSwitchMode == TRUE)
	{
		theApp.pThread = AfxBeginThread(OnPreviewJulia,NULL,THREAD_PRIORITY_NORMAL,0,0,NULL);
	}

	else if(theApp.m_bSwitchMode == FALSE)
	{
		CWnd* pWnd = CWnd::GetSafeOwner(0,0);
		CClientDC dc(pWnd);

		double a, b, t, tBU = 0, x, y, power;

		int xPix, yPix;
		double bailout = 4.0;

		int iOutRed, iOutGreen, iOutBlue;
		int iInRed, iInGreen, iInBlue;

		double xStart, yStart;
		double hyp, hx, hy = 0.0;
		double theta;

		complex<double> z;

		int width = 180;
		int height = 120;
		int xOffset = 953;
		int yOffset = 270;



		a = theApp.m_dControl1;
		b = theApp.m_dControl2;
		power = theApp.m_dControl3;

		hyp = sqrt(pow(3.0,2) + pow(2.0,2));
		theta = atan(2.0/3.0);

		xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
		yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;


		for(yPix = 0; yPix < height; yPix++)
		{
			x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
			y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
			
			hx = 0.0;

			for(xPix = 0; xPix < width; xPix++)
			{
				z = complex<double>(a,b);

				for (t = 0; t < theApp.m_iIteration; t++)
				{
					complex<double> c(x,y);

					z = pow(z, power) + c;

					bailout = abs(z);

					if(bailout > theApp.m_dBailout)
					{
						tBU = t;
						t = theApp.m_iIteration;
					}
				}

				if(theApp.m_iColoringAlgorithmCurSel == 0)
				{
					iOutRed = (int)(theApp.m_dOutRedMult * bailout);
					iOutGreen = (int)(theApp.m_dOutGreenMult * bailout);
					iOutBlue = (int)(theApp.m_dOutBlueMult * bailout);

					if(iOutRed > 255){iOutRed = 255;}
					if(iOutGreen > 255){iOutGreen = 255;}
					if(iOutBlue > 255){iOutBlue = 255;}
				}
				else if(theApp.m_iColoringAlgorithmCurSel == 1)
				{
					if(theApp.m_i2ColorGradientCurSel == 0)
					{
						// Black to White
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = (int)(tBU*2);
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU >= 256)
						{
							iOutRed = 255;
							iOutGreen = 255;
							iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 1)
					{
						// Red to White
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
						}
						else if(tBU < 1024)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
						}
						else if(tBU < 2048)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
						}
						else if(tBU < 4096)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 2)
					{
						// Green to White
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = (int)(tBU*2);
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
						}
						else if(tBU < 1024)
						{
							iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
						}
						else if(tBU < 2048)
						{
							iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
						}
						else if(tBU < 4096)
						{
							iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 3)
					{
						// Blue to White
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU < 512)
						{
							iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutBlue = 255;
						}
						else if(tBU < 1024)
						{
							iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutBlue = 255;
						}
						else if(tBU < 2048)
						{
							iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutBlue = 255;
						}
						else if(tBU < 4096)
						{
							iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutBlue = 255;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 4)
					{
						// Yellow to White
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = (int)(tBU*2);
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = 255;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
						}
						else if(tBU < 1024)
						{
							iOutRed = 255;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
						}
						else if(tBU < 2048)
						{
							iOutRed = 255;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
						}
						else if(tBU < 4096)
						{
							iOutRed = 255;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 5)
					{
						// Purple to White
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = 0;
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = 0;
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU < 512)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutBlue = 255;
						}
						else if(tBU < 1024)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutBlue = 255;
						}
						else if(tBU < 2048)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutBlue = 255;
						}
						else if(tBU < 4096)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutBlue = 255;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 6)
					{
						// Cyan to White
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = (int)(tBU*2);
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU < 512)
						{
							iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutGreen = 255;
							iOutBlue = 255;
						}
						else if(tBU < 1024)
						{
							iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutGreen = 255;
							iOutBlue = 255;
						}
						else if(tBU < 2048)
						{
							iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutGreen = 255;
							iOutBlue = 255;
						}
						else if(tBU < 4096)
						{
							iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutGreen = 255;
							iOutBlue = 255;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 7)
					{
						// Red to Yellow
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutBlue = 0;
						}
						else if(tBU < 1024)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutBlue = 0;
						}
						else if(tBU < 2048)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutBlue = 0;
						}
						else if(tBU < 4096)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutBlue = 0;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 8)
					{
						// Red to Purple
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = 255;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
						}
						else if(tBU < 1024)
						{
							iOutRed = 255;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
						}
						else if(tBU < 2048)
						{
							iOutRed = 255;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
						}
						else if(tBU < 4096)
						{
							iOutRed = 255;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255;
							iOutGreen = 0;
							iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 9)
					{
						// Green to Yellow
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = int(tBU*2);
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutGreen = 255;
							iOutBlue = 0;
						}
						else if(tBU < 1024)
						{
							iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutGreen = 255;
							iOutBlue = 0;
						}
						else if(tBU < 2048)
						{
							iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutGreen = 255;
							iOutBlue = 0;
						}
						else if(tBU < 4096)
						{
							iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutGreen = 255;
							iOutBlue = 0;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 10)
					{
						// Green to Cyan
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = (int)(tBU*2);
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = 0;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
						}
						else if(tBU < 1024)
						{
							iOutRed = 0;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
						}
						else if(tBU < 2048)
						{
							iOutRed = 0;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
						}
						else if(tBU < 4096)
						{
							iOutRed = 0;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
						}
						else if(tBU >= 4096)
						{
							iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 11)
					{
						// Blue to Purple
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU < 512)
						{
							iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutGreen = 0;
							iOutBlue = 255;
						}
						else if(tBU < 1024)
						{
							iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutGreen = 0;
							iOutBlue = 255;
						}
						else if(tBU < 2048)
						{
							iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutGreen = 0;
							iOutBlue = 255;
						}
						else if(tBU < 4096)
						{
							iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutGreen = 0;
							iOutBlue = 255;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 12)
					{
						// Blue to Cyan
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU < 512)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutBlue = 255;
						}
						else if(tBU < 1024)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutBlue = 255;
						}
						else if(tBU < 2048)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutBlue = 255;
						}
						else if(tBU < 4096)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutBlue = 255;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
						}
					}

				}
				else if(theApp.m_iColoringAlgorithmCurSel == 2)
				{
					if(theApp.m_iSmoothGradientCurSel == 0)
					{// Smooth Coloring Function

						iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+1 - log(log(bailout))/log(2.0)));
						iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+1 - log(log(bailout))/log(2.0)));
						iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+1 - log(log(bailout))/log(2.0)));
					}
					else if(theApp.m_iSmoothGradientCurSel == 1)
					{
						if(theApp.m_bMainColoringRed == TRUE)
						{
							iOutRed = (int)(100 - theApp.m_dSmoothRedValue
											* (tBU+1 - log(log(bailout))/log(2.0)));
						}
						else
						{
							iOutRed = (int)(theApp.m_dSmoothRedValue
											* (tBU+1 - log(log(bailout))/log(2.0)));
						}

						if(theApp.m_bMainColoringGreen == TRUE)
						{
							iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
											* (tBU+1 - log(log(bailout))/log(2.0)));
						}
						else
						{
							iOutGreen = (int)(theApp.m_dSmoothGreenValue
											* (tBU+1 - log(log(bailout))/log(2.0)));
						}

						if(theApp.m_bMainColoringBlue == TRUE)
						{
							iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
											* (tBU+1 - log(log(bailout))/log(2.0)));
						}
						else
						{
							iOutBlue = (int)(theApp.m_dSmoothBlueValue
											* (tBU+1 - log(log(bailout))/log(2.0)));
						}
					}
				}


				iInRed = (int)(theApp.m_dInRedMult * bailout);
				iInGreen = (int)(theApp.m_dInGreenMult * bailout);
				iInBlue = (int)(theApp.m_dInBlueMult * bailout);

				if(iInRed > 255){iInRed = 255;}
				if(iInGreen > 255){iInGreen = 255;}
				if(iInBlue > 255){iInBlue = 255;}


				if(bailout < theApp.m_dBailout)
				{
					dc.SetPixel(xOffset+xPix,
								yOffset+yPix,
								RGB(iInRed + theApp.m_iInRedAdder,
									iInGreen + theApp.m_iInGreenAdder,
									iInBlue + theApp.m_iInBlueAdder));
				}
				else
				{
					dc.SetPixel(xOffset+xPix,
								yOffset+yPix,
								RGB(iOutRed + theApp.m_iOutRedAdder,
									iOutGreen + theApp.m_iOutGreenAdder,
									iOutBlue + theApp.m_iOutBlueAdder));
				}

				hx = (6.0/(double)width)/theApp.m_dZoomRatio;

				x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
				y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);

			}

			hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
		}
	}

	return 0;
}

UINT OnPreviewMandelbrotQuick(LPVOID Param)
{
	if(theApp.m_bSwitchMode == TRUE)
	{
		theApp.pThread = AfxBeginThread(OnPreviewJuliaQuick,NULL,THREAD_PRIORITY_NORMAL,0,0,NULL);
	}

	else if(theApp.m_bSwitchMode == FALSE)
	{
		CWnd* pWnd = CWnd::GetSafeOwner(0,0);
		CClientDC dc(pWnd);

		double a, b, d, q, t, tBU = 0, x, y;
		double aStart, bStart;

		int xPix, yPix;
		double bailout = 4.0;

		int iOutRed, iOutGreen, iOutBlue;
		int iInRed, iInGreen, iInBlue;

		double xStart, yStart;
		double hyp, hx, hy = 0.0;
		double theta;

		int width = 180;
		int height = 120;
		int xOffset = 953;
		int yOffset = 270;



		aStart = theApp.m_dControl1;
		bStart = theApp.m_dControl2;

		hyp = sqrt(pow(3.0,2) + pow(2.0,2));
		theta = atan(2.0/3.0);

		xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
		yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;


		for(yPix = 0; yPix < height; yPix++)
		{
			x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
			y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
			
			hx = 0.0;

			for(xPix = 0; xPix < width; xPix++)
			{
				a = aStart;
				b = bStart;

				for (t = 0; t < theApp.m_iIteration; t++)
				{
					q = a;
		  			a = (a * a) - (b * b) + x;
					b = 2 * (q * b) + y;

					d = (a * a) + (b * b);

					bailout = (sqrt(d));

					if(bailout > theApp.m_dBailout)
					{
						tBU = t;
						t = theApp.m_iIteration;
					}
				}

				if(theApp.m_iColoringAlgorithmCurSel == 0)
				{
					iOutRed = (int)(theApp.m_dOutRedMult * bailout);
					iOutGreen = (int)(theApp.m_dOutGreenMult * bailout);
					iOutBlue = (int)(theApp.m_dOutBlueMult * bailout);

					if(iOutRed > 255){iOutRed = 255;}
					if(iOutGreen > 255){iOutGreen = 255;}
					if(iOutBlue > 255){iOutBlue = 255;}
				}
				else if(theApp.m_iColoringAlgorithmCurSel == 1)
				{
					if(theApp.m_i2ColorGradientCurSel == 0)
					{
						// Black to White
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = (int)(tBU*2);
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU >= 256)
						{
							iOutRed = 255;
							iOutGreen = 255;
							iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 1)
					{
						// Red to White
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
						}
						else if(tBU < 1024)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
						}
						else if(tBU < 2048)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
						}
						else if(tBU < 4096)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 2)
					{
						// Green to White
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = (int)(tBU*2);
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
						}
						else if(tBU < 1024)
						{
							iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
						}
						else if(tBU < 2048)
						{
							iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
						}
						else if(tBU < 4096)
						{
							iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 3)
					{
						// Blue to White
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU < 512)
						{
							iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutBlue = 255;
						}
						else if(tBU < 1024)
						{
							iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutBlue = 255;
						}
						else if(tBU < 2048)
						{
							iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutBlue = 255;
						}
						else if(tBU < 4096)
						{
							iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutBlue = 255;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 4)
					{
						// Yellow to White
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = (int)(tBU*2);
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = 255;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
						}
						else if(tBU < 1024)
						{
							iOutRed = 255;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
						}
						else if(tBU < 2048)
						{
							iOutRed = 255;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
						}
						else if(tBU < 4096)
						{
							iOutRed = 255;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 5)
					{
						// Purple to White
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = 0;
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = 0;
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU < 512)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutBlue = 255;
						}
						else if(tBU < 1024)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutBlue = 255;
						}
						else if(tBU < 2048)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutBlue = 255;
						}
						else if(tBU < 4096)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutBlue = 255;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 6)
					{
						// Cyan to White
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = (int)(tBU*2);
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU < 512)
						{
							iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutGreen = 255;
							iOutBlue = 255;
						}
						else if(tBU < 1024)
						{
							iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutGreen = 255;
							iOutBlue = 255;
						}
						else if(tBU < 2048)
						{
							iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutGreen = 255;
							iOutBlue = 255;
						}
						else if(tBU < 4096)
						{
							iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutGreen = 255;
							iOutBlue = 255;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 7)
					{
						// Red to Yellow
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutBlue = 0;
						}
						else if(tBU < 1024)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutBlue = 0;
						}
						else if(tBU < 2048)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutBlue = 0;
						}
						else if(tBU < 4096)
						{
							iOutRed = 255;
							iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutBlue = 0;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 8)
					{
						// Red to Purple
						if(tBU < 64)
						{
							iOutRed = (int)(tBU*2);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = (int)(((tBU-62)*128)/126);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutGreen = 0;
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = 255;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
						}
						else if(tBU < 1024)
						{
							iOutRed = 255;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
						}
						else if(tBU < 2048)
						{
							iOutRed = 255;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
						}
						else if(tBU < 4096)
						{
							iOutRed = 255;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255;
							iOutGreen = 0;
							iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 9)
					{
						// Green to Yellow
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = int(tBU*2);
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutGreen = 255;
							iOutBlue = 0;
						}
						else if(tBU < 1024)
						{
							iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutGreen = 255;
							iOutBlue = 0;
						}
						else if(tBU < 2048)
						{
							iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutGreen = 255;
							iOutBlue = 0;
						}
						else if(tBU < 4096)
						{
							iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutGreen = 255;
							iOutBlue = 0;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 10)
					{
						// Green to Cyan
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = (int)(tBU*2);
							iOutBlue = 0;
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = (int)(((tBU-62)*128)/126);
							iOutBlue = 0;
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
							iOutBlue = 0;
						}
						else if(tBU < 512)
						{
							iOutRed = 0;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
						}
						else if(tBU < 1024)
						{
							iOutRed = 0;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
						}
						else if(tBU < 2048)
						{
							iOutRed = 0;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
						}
						else if(tBU < 4096)
						{
							iOutRed = 0;
							iOutGreen = 255;
							iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
						}
						else if(tBU >= 4096)
						{
							iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 11)
					{
						// Blue to Purple
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU < 512)
						{
							iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutGreen = 0;
							iOutBlue = 255;
						}
						else if(tBU < 1024)
						{
							iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutGreen = 0;
							iOutBlue = 255;
						}
						else if(tBU < 2048)
						{
							iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutGreen = 0;
							iOutBlue = 255;
						}
						else if(tBU < 4096)
						{
							iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutGreen = 0;
							iOutBlue = 255;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
						}
					}
					else if(theApp.m_i2ColorGradientCurSel == 12)
					{
						// Blue to Cyan
						if(tBU < 64)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(tBU*2);
						}
						else if(tBU < 128)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)(((tBU-62)*128)/126);
						}
						else if(tBU < 256)
						{
							iOutRed = 0;
							iOutGreen = 0;
							iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
						}
						else if(tBU < 512)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
							iOutBlue = 255;
						}
						else if(tBU < 1024)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
							iOutBlue = 255;
						}
						else if(tBU < 2048)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
							iOutBlue = 255;
						}
						else if(tBU < 4096)
						{
							iOutRed = 0;
							iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
							iOutBlue = 255;
						}
						else if(tBU >= 4096)
						{
							iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
						}
					}

				}
				else if(theApp.m_iColoringAlgorithmCurSel == 2)
				{
					if(theApp.m_iSmoothGradientCurSel == 0)
					{// Smooth Coloring Function

						iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+1 - log(log(bailout))/log(2.0)));
						iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+1 - log(log(bailout))/log(2.0)));
						iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+1 - log(log(bailout))/log(2.0)));
					}
					else if(theApp.m_iSmoothGradientCurSel == 1)
					{
						if(theApp.m_bMainColoringRed == TRUE)
						{
							iOutRed = (int)(100 - theApp.m_dSmoothRedValue
											* (tBU+1 - log(log(bailout))/log(2.0)));
						}
						else
						{
							iOutRed = (int)(theApp.m_dSmoothRedValue
											* (tBU+1 - log(log(bailout))/log(2.0)));
						}

						if(theApp.m_bMainColoringGreen == TRUE)
						{
							iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
											* (tBU+1 - log(log(bailout))/log(2.0)));
						}
						else
						{
							iOutGreen = (int)(theApp.m_dSmoothGreenValue
											* (tBU+1 - log(log(bailout))/log(2.0)));
						}

						if(theApp.m_bMainColoringBlue == TRUE)
						{
							iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
											* (tBU+1 - log(log(bailout))/log(2.0)));
						}
						else
						{
							iOutBlue = (int)(theApp.m_dSmoothBlueValue
											* (tBU+1 - log(log(bailout))/log(2.0)));
						}
					}
				}


				iInRed = (int)(theApp.m_dInRedMult * bailout);
				iInGreen = (int)(theApp.m_dInGreenMult * bailout);
				iInBlue = (int)(theApp.m_dInBlueMult * bailout);

				if(iInRed > 255){iInRed = 255;}
				if(iInGreen > 255){iInGreen = 255;}
				if(iInBlue > 255){iInBlue = 255;}


				if(bailout < theApp.m_dBailout)
				{
					dc.SetPixel(xOffset+xPix,
								yOffset+yPix,
								RGB(iInRed + theApp.m_iInRedAdder,
									iInGreen + theApp.m_iInGreenAdder,
									iInBlue + theApp.m_iInBlueAdder));
				}
				else
				{
					dc.SetPixel(xOffset+xPix,
								yOffset+yPix,
								RGB(iOutRed + theApp.m_iOutRedAdder,
									iOutGreen + theApp.m_iOutGreenAdder,
									iOutBlue + theApp.m_iOutBlueAdder));
				}

				hx = (6.0/(double)width)/theApp.m_dZoomRatio;

				x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
				y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);
			}

			hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
		}
	}

	return 0;
}

UINT OnPreviewJulia(LPVOID Param)
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double a, b, t, tBU = 0, x, y, power;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 180;
	int height = 120;
	int xOffset = 953;
	int yOffset = 270;



	a = theApp.m_dControl1;
	b = theApp.m_dControl2;
	power = theApp.m_dControl3;

	complex<double> c(a,b);

	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
	yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;


	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
		y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
		
		hx = 0.0;

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			for (t = 0; t < theApp.m_iIteration; t++)
			{
				z = pow(z, power) + c;

				bailout = abs(z);

				if(bailout > theApp.m_dBailout)
				{
					tBU = t;
					t = theApp.m_iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(theApp.m_dOutRedMult * bailout);
				iOutGreen = (int)(theApp.m_dOutGreenMult * bailout);
				iOutBlue = (int)(theApp.m_dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+2 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}


			if(bailout < theApp.m_dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + theApp.m_iInRedAdder,
								iInGreen + theApp.m_iInGreenAdder,
								iInBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + theApp.m_iOutRedAdder,
								iOutGreen + theApp.m_iOutGreenAdder,
								iOutBlue + theApp.m_iOutBlueAdder));
			}

			hx = (6.0/(double)width)/theApp.m_dZoomRatio;

			x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
			y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
	}

	return 0;
}

UINT OnPreviewJuliaQuick(LPVOID Param)
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double a, b, d, e, f, q, t, tBU = 0, x, y;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	int width = 180;
	int height = 120;
	int xOffset = 953;
	int yOffset = 270;



	a = theApp.m_dControl1;
	b = theApp.m_dControl2;

	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
	yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;


	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
		y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
		
		hx = 0.0;

		for(xPix = 0; xPix < width; xPix++)
		{
			e = x;
			f = y;

			for (t = 0; t < theApp.m_iIteration; t++)
			{
				q = e;
	  			e = (e * e) - (f * f) + a;
					
				f = 2 * (q * f) + b;

				d = (e * e) + (f * f);

				bailout = (sqrt(d));

				if(bailout > theApp.m_dBailout)
				{
					tBU = t;
					t = theApp.m_iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(theApp.m_dOutRedMult * bailout);
				iOutGreen = (int)(theApp.m_dOutGreenMult * bailout);
				iOutBlue = (int)(theApp.m_dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+2 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}


			if(bailout < theApp.m_dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + theApp.m_iInRedAdder,
								iInGreen + theApp.m_iInGreenAdder,
								iInBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + theApp.m_iOutRedAdder,
								iOutGreen + theApp.m_iOutGreenAdder,
								iOutBlue + theApp.m_iOutBlueAdder));
			}

			hx = (6.0/(double)width)/theApp.m_dZoomRatio;

			x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
			y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
	}

	return 0;
}

UINT OnPreviewNewton(LPVOID Param)
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double t, x, y;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 180;
	int height = 120;
	int xOffset = 953;
	int yOffset = 270;



	double dRelaxReal = theApp.m_dControl1;
	double dRelaxImag = theApp.m_dControl2;
	double power = theApp.m_dControl3;

	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
	yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;

	z = complex<double>(xStart,yStart);

	complex<double> zGreen = complex<double>(-0.5, sin(2*3.1416/3));
	complex<double> zBlue = complex<double>(-0.5, -sin(2*3.1416/3));

	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
		y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
		
		hx = 0.0;

		z = complex<double>(x,y);

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			for(t = 0; t < theApp.m_iIteration; t++)
			{
				z = z - complex<double>(dRelaxReal, dRelaxImag)
					* (pow(z,power)-1.0)/((double)power*pow(z,power-1.0));

				bailout = abs(z);

				if(bailout > theApp.m_dBailout)
				{
					t = theApp.m_iIteration;
				}
			}

			iOutRed = (int)(theApp.m_dOutRedMult * bailout);
			iOutGreen = (int)(theApp.m_dOutGreenMult * bailout);
			iOutBlue = (int)(theApp.m_dOutBlueMult * bailout);

			if(iOutRed > 255){iOutRed = 255;}
			if(iOutGreen > 255){iOutGreen = 255;}
			if(iOutBlue > 255){iOutBlue = 255;}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}

			if(bailout < theApp.m_dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + theApp.m_iInRedAdder,
								iInGreen + theApp.m_iInGreenAdder,
								iInBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + theApp.m_iOutRedAdder,
								iOutGreen + theApp.m_iOutGreenAdder,
								iOutBlue + theApp.m_iOutBlueAdder));
			}

			hx = (6.0/(double)width)/theApp.m_dZoomRatio;

			x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
			y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
	}

	return 0;
}

UINT OnPreviewNewtonRoot(LPVOID Param)
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double t, x, y;

	int xPix, yPix;
	double bailout = 4.0;

	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 180;
	int height = 120;
	int xOffset = 953;
	int yOffset = 270;



	double dRelaxReal = theApp.m_dControl1;
	double dRelaxImag = theApp.m_dControl2;
	double tol = theApp.m_dControl3;

	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
	yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;

	z = complex<double>(xStart,yStart);

	complex<double> zGreen = complex<double>(-0.5, sin(2*3.1416/3));
	complex<double> zBlue = complex<double>(-0.5, -sin(2*3.1416/3));

	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
		y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
		
		hx = 0.0;

		z = complex<double>(x,y);

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			t = 0;

			while((t < theApp.m_iIteration) && (abs(z-1.0) >= tol) && (abs(z-zGreen) >= tol) && (abs(z-zBlue) >= tol))
			{
				z = z - complex<double>(dRelaxReal,dRelaxImag) * (z*z*z-1.0) / ((double)3.0*z*z);

				bailout = abs(z);

				t++;
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}

			if(abs(z - 1.0) < tol)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(t*theApp.m_iFirstRootRed + theApp.m_iInRedAdder,
								t*theApp.m_iFirstRootGreen + theApp.m_iInGreenAdder,
								t*theApp.m_iFirstRootBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				iInRed = 0;
			}

			if(abs(z - zGreen) <= tol)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(t*theApp.m_iSecondRootRed + theApp.m_iInRedAdder,
							t*theApp.m_iSecondRootGreen + theApp.m_iInGreenAdder,
							t*theApp.m_iSecondRootBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				iInGreen = 0;
			}

			if(abs(z - zBlue) <= tol)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(t*theApp.m_iThirdRootRed + theApp.m_iInRedAdder,
							t*theApp.m_iThirdRootGreen + theApp.m_iInGreenAdder,
							t*theApp.m_iThirdRootBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				iInBlue = 0;
			}


			hx = (6.0/(double)width)/theApp.m_dZoomRatio;

			x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
			y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
	}

	return 0;
}

UINT OnPreviewNova(LPVOID Param)
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double t, tBU = 0, x, y;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 180;
	int height = 120;
	int xOffset = 953;
	int yOffset = 270;



	double dSeeda = theApp.m_dControl1;
	double dSeedB = theApp.m_dControl2;

	double dRelaxReal = theApp.m_dControl3;
	double dRelaxImag = theApp.m_dControl4;

	double power = theApp.m_dControl5;

	double dDerivMult = theApp.m_dControl6;
	
	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
	yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;

	z = complex<double>(xStart,yStart);

	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
		y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
		
		hx = 0.0;

		z = complex<double>(x,y);

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			for(t = 0; t < theApp.m_iIteration; t++)
			{
				z = z - complex<double>(dRelaxReal, dRelaxImag)
						* (pow(z,power)-1.0)/(dDerivMult*power*pow(z,power-1.0))
						+ complex<double>(dSeeda, dSeedB);

				bailout = abs(z);

				if(bailout > theApp.m_dBailout)
				{
					tBU = t;
					t = theApp.m_iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(theApp.m_dOutRedMult * bailout);
				iOutGreen = (int)(theApp.m_dOutGreenMult * bailout);
				iOutBlue = (int)(theApp.m_dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+1 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}

			if(bailout < theApp.m_dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + theApp.m_iInRedAdder,
								iInGreen + theApp.m_iInGreenAdder,
								iInBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + theApp.m_iOutRedAdder,
								iOutGreen + theApp.m_iOutGreenAdder,
								iOutBlue + theApp.m_iOutBlueAdder));
			}

			hx = (6.0/(double)width)/theApp.m_dZoomRatio;

			x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
			y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
	}

	return 0;
}

UINT OnPreviewNovaSpecial(LPVOID Param)
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double t, tBU = 0, x, y;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 180;
	int height = 120;
	int xOffset = 953;
	int yOffset = 270;



	double dRelaxReal = theApp.m_dControl1;
	double dRelaxImag = theApp.m_dControl2;

	double power = theApp.m_dControl3;

	double dDerivMult = theApp.m_dControl4;
	
	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
	yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;

	z = complex<double>(xStart,yStart);

	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
		y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
		
		hx = 0.0;

		z = complex<double>(x,y);

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			for(t = 0; t < theApp.m_iIteration; t++)
			{
				z = z - complex<double>(dRelaxReal, dRelaxImag)
						* (pow(z,power)-1.0)/(dDerivMult*power*pow(z,power-1.0))
						+ complex<double>(x,y);

				bailout = abs(z);

				if(bailout > theApp.m_dBailout)
				{
					tBU = t;
					t = theApp.m_iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(theApp.m_dOutRedMult * bailout);
				iOutGreen = (int)(theApp.m_dOutGreenMult * bailout);
				iOutBlue = (int)(theApp.m_dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+1 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}

			if(bailout < theApp.m_dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + theApp.m_iInRedAdder,
								iInGreen + theApp.m_iInGreenAdder,
								iInBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + theApp.m_iOutRedAdder,
								iOutGreen + theApp.m_iOutGreenAdder,
								iOutBlue + theApp.m_iOutBlueAdder));
			}

			hx = (6.0/(double)width)/theApp.m_dZoomRatio;

			x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
			y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
	}

	return 0;
}

UINT OnPreviewGlynn(LPVOID Param)
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double a, b, t, tBU = 0, x, y, power;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 180;
	int height = 120;
	int xOffset = 953;
	int yOffset = 270;



	a = theApp.m_dControl1;
	b = theApp.m_dControl2;
	power = theApp.m_dControl3;

	complex<double> c(a,b);

	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
	yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;


	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
		y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
		
		hx = 0.0;

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			for (t = 0; t < theApp.m_iIteration; t++)
			{
				z = pow(z, power) + c;

				bailout = abs(z);

				if(bailout > theApp.m_dBailout)
				{
					tBU = t;
					t = theApp.m_iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(theApp.m_dOutRedMult * bailout);
				iOutGreen = (int)(theApp.m_dOutGreenMult * bailout);
				iOutBlue = (int)(theApp.m_dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+1 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}


			if(bailout < theApp.m_dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + theApp.m_iInRedAdder,
								iInGreen + theApp.m_iInGreenAdder,
								iInBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + theApp.m_iOutRedAdder,
								iOutGreen + theApp.m_iOutGreenAdder,
								iOutBlue + theApp.m_iOutBlueAdder));
			}

			hx = (6.0/(double)width)/theApp.m_dZoomRatio;

			x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
			y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
	}

	return 0;
}

UINT OnPreviewPhoenix(LPVOID Param)
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double t, tBU = 0, x, y, power;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z, zBU, zBU2;

	int width = 180;
	int height = 120;
	int xOffset = 953;
	int yOffset = 270;



	complex<double> p1(theApp.m_dControl1, theApp.m_dControl2);
	complex<double> p2(theApp.m_dControl3, theApp.m_dControl4);

	power = theApp.m_dControl5;

	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
	yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;


	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
		y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
		
		hx = 0.0;

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			for (t = 0; t < theApp.m_iIteration; t++)
			{
				if(t == 0)
				{
					zBU = z;
					zBU2 = complex<double>(0.0,0.0);
				}
				else
				{
					zBU2 = zBU;
					zBU = z;
				}

				z = pow(z, power) + p1 + real(p2)*zBU2 + imag(p2)*zBU2;

				bailout = abs(z);

				if(bailout > theApp.m_dBailout)
				{
					tBU = t;
					t = theApp.m_iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(theApp.m_dOutRedMult * bailout);
				iOutGreen = (int)(theApp.m_dOutGreenMult * bailout);
				iOutBlue = (int)(theApp.m_dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+2 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}


			if(bailout < theApp.m_dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + theApp.m_iInRedAdder,
								iInGreen + theApp.m_iInGreenAdder,
								iInBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + theApp.m_iOutRedAdder,
								iOutGreen + theApp.m_iOutGreenAdder,
								iOutBlue + theApp.m_iOutBlueAdder));
			}

			hx = (6.0/(double)width)/theApp.m_dZoomRatio;

			x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
			y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
	}

	return 0;
}

UINT OnPreviewCosTanZ(LPVOID Param)
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double a, b, t, tBU = 0, x, y, power;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 180;
	int height = 120;
	int xOffset = 953;
	int yOffset = 270;



	a = theApp.m_dControl1;
	b = theApp.m_dControl2;
	power = theApp.m_dControl3;

	complex<double> c(a,b);

	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
	yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;


	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
		y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
		
		hx = 0.0;

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			for (t = 0; t < theApp.m_iIteration; t++)
			{
				z = cos(tan(z)) + c;

				bailout = abs(z);

				if(bailout > theApp.m_dBailout)
				{
					tBU = t;
					t = theApp.m_iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(theApp.m_dOutRedMult * bailout);
				iOutGreen = (int)(theApp.m_dOutGreenMult * bailout);
				iOutBlue = (int)(theApp.m_dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+2 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}


			if(bailout < theApp.m_dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + theApp.m_iInRedAdder,
								iInGreen + theApp.m_iInGreenAdder,
								iInBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + theApp.m_iOutRedAdder,
								iOutGreen + theApp.m_iOutGreenAdder,
								iOutBlue + theApp.m_iOutBlueAdder));
			}

			hx = (6.0/(double)width)/theApp.m_dZoomRatio;

			x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
			y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////

BOOL WndToBmp(CDC *pDC, CString szFile)
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

HANDLE DDBToDIB(CBitmap &bitmap, DWORD dwCompression, CPalette *pPal)
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

	// ifa palette has not been supplied use defaul palette
	hPal = (HPALETTE) pPal->GetSafeHandle();
	if(hPal==NULL)
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

	if(!hDIB){
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

	// ifthe driver did not fill in the biSizeImage field, then compute it
	// Each scan line of the image is aligned on a DWORD (32bit) boundary
	if(bi.biSizeImage == 0){
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) 
						* bi.biHeight;

		// ifa compression scheme is used the result may infact be larger
		// Increase the size to account for this.
		if(dwCompression != BI_RGB)
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}

	// Realloc the buffer so that it can hold all the bits
	dwLen += bi.biSizeImage;
	if(handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
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

UINT OnAnimationThread(LPVOID Param)
{
	int sum = 0;
	int triggerPoint = 1;
	char fileName[32];
	
	HWND hWnd = FindWindow(_T("#32770"),_T("Fractoria"));
	HDC hDC = GetDC(hWnd);
	CDC *pDC = CDC::FromHandle(hDC);

	theApp.m_iFrameCount = 1;

	clock_t start, temp, finish;
	start = clock();

	OnAnimationRender();


	CString fname(_itoa(1, fileName, 10));
	fname = TEXT("00000") + fname + TEXT(".bmp");

	WndToBmp(pDC,fname);

	int iPercent = (int)(100*theApp.m_iFrameCount/theApp.m_iTotalFrame);
	CString sPercent;
	sPercent.Format(_T("%d"), iPercent);
	theApp.pStatic16002.SetWindowTextW(sPercent);


	temp = clock();
	theApp.m_dRenderTime = Round(((double)(temp - start) / CLOCKS_PER_SEC), 3);

	CString sRenderTime;
	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	double m_iRemainingTime = Round(theApp.m_dRenderTime
								/ theApp.m_iFrameCount*theApp.m_iTotalFrame-theApp.m_dRenderTime,3);

	CString sRemainingTime;
	sRemainingTime.Format(_T("%lf"), m_iRemainingTime);
	sRemainingTime = RemoveZeros(sRemainingTime);
	theApp.pStatic16004.SetWindowTextW(sRemainingTime);


	// Geometry Back-Up Variables
	double dXAxisBU = theApp.m_dXAxis;
	double dYAxisBU = theApp.m_dYAxis;
	double dZoomRatioBU = theApp.m_dZoomRatio;
	double dRotationBU = theApp.m_dRotation;
	int iIterationBU = theApp.m_iIteration;
	double dIteration = iIterationBU;
	
	// Formulae Back-Up Variables
	double dControl1BU = theApp.m_dControl1;
	double dControl2BU = theApp.m_dControl2;
	double dControl3BU = theApp.m_dControl3;
	double dControl4BU = theApp.m_dControl4;
	double dControl5BU = theApp.m_dControl5;
	double dControl6BU = theApp.m_dControl6;

	// Animation Back-Up Variables
	int iOutRedAdderBU = theApp.m_iOutRedAdder;
	double dOutRedAdder = iOutRedAdderBU;
	int iOutGreenAdderBU = theApp.m_iOutGreenAdder;
	double dOutGreenAdder = iOutGreenAdderBU;
	int iOutBlueAdderBU = theApp.m_iOutBlueAdder;
	double dOutBlueAdder = iOutBlueAdderBU;
	double dOutRedMultBU = theApp.m_dOutRedMult;
	double dOutGreenMultBU = theApp.m_dOutGreenMult;
	double dOutBlueMultBU = theApp.m_dOutBlueMult;
	int iInRedAdderBU = theApp.m_iInRedAdder;
	double dInRedAdder = iInRedAdderBU;
	int iInGreenAdderBU = theApp.m_iInGreenAdder;
	double dInGreenAdder = iInGreenAdderBU;
	int iInBlueAdderBU = theApp.m_iInBlueAdder;
	double dInBlueAdder = iInBlueAdderBU;
	double dInRedMultBU = theApp.m_dInRedMult;
	double dInGreenMultBU = theApp.m_dInGreenMult;
	double dInBlueMultBU = theApp.m_dInBlueMult;


	for(theApp.m_iFrameCount = 2; theApp.m_iFrameCount <= theApp.m_iTotalFrame; theApp.m_iFrameCount++)
	{

		if(theApp.m_iFrameCount <= theApp.m_iFramePoint[0])
		{// First Trigger Point Functions

			if(theApp.m_iTranslationFunction[0] == 0)
			{// Linear Translation Function

				theApp.m_dXAxis += (theApp.m_dEndXAxis[0] - dXAxisBU)/(double)(theApp.m_iFramePoint[0]-1);
					
				theApp.m_dYAxis += (theApp.m_dEndYAxis[0] - dYAxisBU)/(double)(theApp.m_iFramePoint[0]-1);

			}
			else if(theApp.m_iTranslationFunction[0] == 1)
			{// Positive Quadratic Translation Function

				double counter = (double)(theApp.m_iFrameCount-1);

				double aX = (theApp.m_dEndXAxis[0]-dXAxisBU)/pow((theApp.m_iFramePoint[0]-1),2.0);

				theApp.m_dXAxis = aX*counter*counter + dXAxisBU;

				double aY = (theApp.m_dEndYAxis[0]-dYAxisBU)/pow((theApp.m_iFramePoint[0]-1),2.0);

				theApp.m_dYAxis = aY*counter*counter + dYAxisBU;

			}
			else if(theApp.m_iTranslationFunction[0] == 2)
			{// Negative Quadratic Translation Function

				double counter = (double)(theApp.m_iFrameCount-1);

				double bX = (theApp.m_dEndXAxis[0]-dXAxisBU)/((theApp.m_iFramePoint[0]-1)/2.0);
				double aX = -bX/((theApp.m_iFramePoint[0]-1)*2.0);

				theApp.m_dXAxis = aX*counter*counter + bX*counter + dXAxisBU;

				double bY = (theApp.m_dEndYAxis[0]-dYAxisBU)/((theApp.m_iFramePoint[0]-1)/2.0);
				double aY = -bY/((theApp.m_iFramePoint[0]-1)*2.0);

				theApp.m_dYAxis = aY*counter*counter + bY*counter + dYAxisBU;

			}
			else if(theApp.m_iTranslationFunction[0] == 3)
			{// Negative Cosine Translation Function

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dXAxis = dXAxisBU + (theApp.m_dEndXAxis[0]-dXAxisBU)
							*(-cos(PI*counter/(theApp.m_iFramePoint[0]-1))+1)/2;

				theApp.m_dYAxis = dYAxisBU + (theApp.m_dEndYAxis[0]-dYAxisBU)
							*(-cos(PI*counter/(theApp.m_iFramePoint[0]-1))+1)/2;

			}

			if(theApp.m_iRotationFunction[0] == 0)
			{// Linear Rotation Function

				theApp.m_dRotation += (theApp.m_dEndRotation[0]
									- dRotationBU)/(double)(theApp.m_iFramePoint[0]-1);

			}
			else if(theApp.m_iRotationFunction[0] == 1)
			{// Positive Quadratic Rotation Function

				double counter = (double)(theApp.m_iFrameCount-1);

				double a = (theApp.m_dEndRotation[0]-dRotationBU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2);

				theApp.m_dRotation = a*counter*counter + dRotationBU;

			}
			else if(theApp.m_iRotationFunction[0] == 2)
			{// Negative Quadratic Rotation Function

				double counter = (double)(theApp.m_iFrameCount-1);

				double b = (theApp.m_dEndRotation[0]-dRotationBU)
							/ ((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);

				theApp.m_dRotation = a*counter*counter + b*counter + dRotationBU;

			}

			if(theApp.m_iZoomRatioFunction[0] == 0)
			{// Linear Zoom Ratio Function

				theApp.m_dZoomRatio += (theApp.m_dEndZoomRatio[0]-dZoomRatioBU)
										/ (double)(theApp.m_iFramePoint[0]-1);

			}
			else if(theApp.m_iZoomRatioFunction[0] == 1)
			{// Summation Zoom Ratio Function

				sum += theApp.m_iFrameCount;

				theApp.m_dZoomRatio = dZoomRatioBU + sum*(theApp.m_dEndZoomRatio[0]-dZoomRatioBU)
													/ (double)((theApp.m_iFramePoint[0]-1)
													* (theApp.m_iFramePoint[0]+2)/2.0);

			}
			else if(theApp.m_iZoomRatioFunction[0] == 2)
			{// Positive Quadratic Zoom Ratio Function

				double a = (theApp.m_dEndZoomRatio[0]-dZoomRatioBU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2);

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dZoomRatio = a*counter*counter + dZoomRatioBU;

			}
			else if(theApp.m_iZoomRatioFunction[0] == 3)
			{// Negative Quadratic Zoom Ratio Function

				double b = (theApp.m_dEndZoomRatio[0]-dZoomRatioBU)
							/ ((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dZoomRatio = a*counter*counter + b*counter + dZoomRatioBU;

			}

			if(theApp.m_iIterationFunction[0] == 0)
			{// Linear Iteration Function

				dIteration += (double)((double)(theApp.m_iEndIteration[0]-iIterationBU)
								/ (double)(theApp.m_iFramePoint[0]-1));

				theApp.m_iIteration = (int)(dIteration+0.5);

			}			
			else if(theApp.m_iIterationFunction[0] == 1)
			{// Positive Quadratic Iteration Function

				double a = (double)((theApp.m_iEndIteration[0]-iIterationBU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2));

				double counter = (double)(theApp.m_iFrameCount-1);

				dIteration = a*counter*counter + iIterationBU;

				theApp.m_iIteration = (int)(dIteration+0.5);

			}
			else if(theApp.m_iIterationFunction[0] == 2)
			{// Negative Quadratic Iteration Function

				double b = (theApp.m_iEndIteration[0]-iIterationBU)
							/ ((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);

				double counter = (double)(theApp.m_iFrameCount-1);

				dIteration = a*counter*counter + b*counter + iIterationBU;

				theApp.m_iIteration = (int)(dIteration+0.5);

			}


			// Formula Control Functions

			if(theApp.m_iAnimationControl1Function[0] == 0)
			{// Linear Control #1 Function

				theApp.m_dControl1 += (theApp.m_dAnimationControl1[0]-dControl1BU)
										/ (theApp.m_iFramePoint[0]-1);

			}
			else if(theApp.m_iAnimationControl1Function[0] == 1)
			{// Positive Quadratic Control #1 Function

				double a = (theApp.m_dAnimationControl1[0]-dControl1BU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2);

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dControl1 = a*counter*counter + dControl1BU;

			}
			else if(theApp.m_iAnimationControl1Function[0] == 2)
			{// Negative Quadratic Control #1 Function

				double b = (theApp.m_dAnimationControl1[0]-dControl1BU)
							/ ((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);
				
				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dControl1 = a*counter*counter + b*counter + dControl1BU;

			}
			else if(theApp.m_iAnimationControl1Function[0] == 3)
			{// Negative Cosine Control #1 Function

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dControl1 = dControl1BU + (theApp.m_dAnimationControl1[0]-dControl1BU)
									*(-cos(PI*counter/(theApp.m_iFramePoint[0]-1))+1)/2;

			}

			if(theApp.m_iAnimationControl2Function[0] == 0)
			{// Linear Control #2 Function

				theApp.m_dControl2 += (theApp.m_dAnimationControl2[0]-dControl2BU)
										/ (theApp.m_iFramePoint[0]-1);

			}
			else if(theApp.m_iAnimationControl2Function[0] == 1)
			{// Positive Quadratic Control #2 Function

				double a = (theApp.m_dAnimationControl2[0]-dControl2BU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2);

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dControl2 = a*counter*counter + dControl2BU;

			}
			else if(theApp.m_iAnimationControl2Function[0] == 2)
			{// Negative Quadratic Control #2 Function

				double b = (theApp.m_dAnimationControl2[0]-dControl2BU)
							/ ((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);
				
				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dControl2 = a*counter*counter + b*counter + dControl2BU;

			}
			else if(theApp.m_iAnimationControl2Function[0] == 3)
			{// Negative Cosine Control #2 Function

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dControl2 = dControl2BU + (theApp.m_dAnimationControl2[0]-dControl2BU)
									* (-cos(PI*counter/(theApp.m_iFramePoint[0]-1))+1)/2;

			}

			if(theApp.m_iAnimationControl3Function[0] == 0)
			{// Linear Control #3 Function

				theApp.m_dControl3 += (theApp.m_dAnimationControl3[0]-dControl3BU)
										/ (theApp.m_iFramePoint[0]-1);

			}
			else if(theApp.m_iAnimationControl3Function[0] == 1)
			{// Positive Quadratic Control #3 Function

				double a = (theApp.m_dAnimationControl3[0]-dControl3BU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2);

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dControl3 = a*counter*counter + dControl3BU;

			}
			else if(theApp.m_iAnimationControl3Function[0] == 2)
			{// Negative Quadratic Control #3 Function

				double b = (theApp.m_dAnimationControl3[0]-dControl3BU)
							/ ((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);
				
				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dControl3 = a*counter*counter + b*counter + dControl3BU;

			}
			else if(theApp.m_iAnimationControl3Function[0] == 3)
			{// Negative Cosine Control #3 Function

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dControl3 = dControl3BU + (theApp.m_dAnimationControl3[0]-dControl3BU)
									* (-cos(PI*counter/(theApp.m_iFramePoint[0]-1))+1)/2;

			}

			if(theApp.m_iAnimationControl4Function[0] == 0)
			{// Linear Control #4 Function

				theApp.m_dControl4 += (theApp.m_dAnimationControl4[0]-dControl4BU)
										/ (theApp.m_iFramePoint[0]-1);

			}
			else if(theApp.m_iAnimationControl4Function[0] == 1)
			{// Positive Quadratic Control #4 Function

				double a = (theApp.m_dAnimationControl4[0]-dControl4BU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2);

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dControl4 = a*counter*counter + dControl4BU;

			}
			else if(theApp.m_iAnimationControl4Function[0] == 2)
			{// Negative Quadratic Control #4 Function

				double b = (theApp.m_dAnimationControl4[0]-dControl4BU)
								/((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);
				
				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dControl4 = a*counter*counter + b*counter + dControl4BU;

			}
			else if(theApp.m_iAnimationControl4Function[0] == 3)
			{// Negative Cosine Control #4 Function

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dControl4 = dControl4BU + (theApp.m_dAnimationControl4[0]-dControl4BU)
									* (-cos(PI*counter/(theApp.m_iFramePoint[0]-1))+1)/2;

			}

			if(theApp.m_iAnimationControl5Function[0] == 0)
			{// Linear Control #5 Function

				theApp.m_dControl5 += (theApp.m_dAnimationControl5[0]-dControl5BU)
										/ (theApp.m_iFramePoint[0]-1);

			}
			else if(theApp.m_iAnimationControl5Function[0] == 1)
			{// Positive Quadratic Control #5 Function

				double a = (theApp.m_dAnimationControl5[0]-dControl5BU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2);

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dControl5 = a*counter*counter + dControl5BU;

			}
			else if(theApp.m_iAnimationControl5Function[0] == 2)
			{// Negative Quadratic Control #5 Function

				double b = (theApp.m_dAnimationControl5[0]-dControl5BU)
								/((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);
				
				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dControl5 = a*counter*counter + b*counter + dControl5BU;

			}
			else if(theApp.m_iAnimationControl5Function[0] == 3)
			{// Negative Cosine Control #5 Function

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dControl5 = dControl5BU + (theApp.m_dAnimationControl5[0]-dControl5BU)
									*(-cos(PI*counter/(theApp.m_iFramePoint[0]-1))+1)/2;

			}

			if(theApp.m_iAnimationControl6Function[0] == 0)
			{// Linear Control #6 Function

				theApp.m_dControl6 += (theApp.m_dAnimationControl6[0]-dControl6BU)
										/ (theApp.m_iFramePoint[0]-1);

			}
			else if(theApp.m_iAnimationControl6Function[0] == 1)
			{// Positive Quadratic Control #6 Function

				double a = (theApp.m_dAnimationControl6[0]-dControl6BU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2);

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dControl6 = a*counter*counter + dControl6BU;

			}
			else if(theApp.m_iAnimationControl6Function[0] == 2)
			{// Negative Quadratic Control #6 Function

				double b = (theApp.m_dAnimationControl6[0]-dControl6BU)
							/ ((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);
				
				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dControl6 = a*counter*counter + b*counter + dControl6BU;

			}
			else if(theApp.m_iAnimationControl6Function[0] == 3)
			{// Negative Cosine Control #6 Function

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dControl6 = dControl6BU + (theApp.m_dAnimationControl6[0]-dControl6BU)
									*(-cos(PI*counter/(theApp.m_iFramePoint[0]-1))+1)/2;

			}


			// Outside Coloring Functions

			if(theApp.m_iAnimationOutsideRedAdderFunction[0] == 0)
			{// Linear Outside Red Adder Function

				dOutRedAdder += (double)((double)(theApp.m_dAnimationOutsideRedAdder[0]
								- iOutRedAdderBU) / (double)(theApp.m_iFramePoint[0]-1));

				theApp.m_iOutRedAdder = (int)(dOutRedAdder+0.5);

			}
			else if(theApp.m_iAnimationOutsideRedAdderFunction[0] == 1)
			{// Positive Quadratic Outside Red Adder Function

				double a = (double)((theApp.m_dAnimationOutsideRedAdder[0]-iOutRedAdderBU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2));

				double counter = (double)(theApp.m_iFrameCount-1);

				dOutRedAdder = a*counter*counter + iOutRedAdderBU;

				theApp.m_iOutRedAdder = (int)(dOutRedAdder+0.5);

			}
			else if(theApp.m_iAnimationOutsideRedAdderFunction[0] == 2)
			{// Negative Quadratic Outside Red Adder Function

				double b = (theApp.m_dAnimationOutsideRedAdder[0]-iOutRedAdderBU)
							/ ((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);

				double counter = (double)(theApp.m_iFrameCount-1);

				dOutRedAdder = a*counter*counter + b*counter + iOutRedAdderBU;

				theApp.m_iOutRedAdder = (int)(dOutRedAdder+0.5);

			}

			if(theApp.m_iAnimationOutsideGreenAdderFunction[0] == 0)
			{// Linear Outside Green Adder Function

				dOutGreenAdder += (double)((double)(theApp.m_dAnimationOutsideGreenAdder[0]
								- iOutGreenAdderBU) / (double)(theApp.m_iFramePoint[0]-1));

				theApp.m_iOutGreenAdder = (int)(dOutGreenAdder+0.5);

			}
			else if(theApp.m_iAnimationOutsideGreenAdderFunction[0] == 1)
			{// Positive Quadratic Outside Green Adder Function

				double a = (double)((theApp.m_dAnimationOutsideGreenAdder[0]-iOutGreenAdderBU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2));

				double counter = (double)(theApp.m_iFrameCount-1);

				dOutGreenAdder = a*counter*counter + iOutGreenAdderBU;

				theApp.m_iOutGreenAdder = (int)(dOutGreenAdder+0.5);

			}
			else if(theApp.m_iAnimationOutsideGreenAdderFunction[0] == 2)
			{// Negative Quadratic Outside Green Adder Function

				double b = (theApp.m_dAnimationOutsideGreenAdder[0]-iOutGreenAdderBU)
							/ ((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);

				double counter = (double)(theApp.m_iFrameCount-1);

				dOutGreenAdder = a*counter*counter + b*counter + iOutGreenAdderBU;

				theApp.m_iOutGreenAdder = (int)(dOutGreenAdder+0.5);

			}

			if(theApp.m_iAnimationOutsideBlueAdderFunction[0] == 0)
			{// Linear Outside Blue Adder Function

				dOutBlueAdder += (double)((double)(theApp.m_dAnimationOutsideBlueAdder[0]
								- iOutBlueAdderBU) / (double)(theApp.m_iFramePoint[0]-1));

				theApp.m_iOutBlueAdder = (int)(dOutBlueAdder+0.5);

			}
			else if(theApp.m_iAnimationOutsideBlueAdderFunction[0] == 1)
			{// Positive Quadratic Outside Blue Adder Function

				double a = (double)((theApp.m_dAnimationOutsideBlueAdder[0]-iOutBlueAdderBU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2));

				double counter = (double)(theApp.m_iFrameCount-1);

				dOutBlueAdder = a*counter*counter + iOutBlueAdderBU;

				theApp.m_iOutBlueAdder = (int)(dOutBlueAdder+0.5);

			}
			else if(theApp.m_iAnimationOutsideBlueAdderFunction[0] == 2)
			{// Negative Quadratic Outside Blue Adder Function

				double b = (theApp.m_dAnimationOutsideBlueAdder[0]-iOutBlueAdderBU)
							/ ((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);

				double counter = (double)(theApp.m_iFrameCount-1);

				dOutBlueAdder = a*counter*counter + b*counter + iOutBlueAdderBU;

				theApp.m_iOutBlueAdder = (int)(dOutBlueAdder+0.5);

			}


			if(theApp.m_iAnimationOutsideRedMultFunction[0] == 0)
			{// Linear Outside Red Multiplier Function

				theApp.m_dOutRedMult += (theApp.m_dAnimationOutsideRedMult[0]-dOutRedMultBU)
										/ (double)(theApp.m_iFramePoint[0]-1);

			}
			else if(theApp.m_iAnimationOutsideRedMultFunction[0] == 1)
			{// Positive Quadratic Outside Red Multiplier Function

				double a = (theApp.m_dAnimationOutsideRedMult[0]-dOutRedMultBU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2);

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dOutRedMult = a*counter*counter + dOutRedMultBU;

			}
			else if(theApp.m_iAnimationOutsideRedMultFunction[0] == 2)
			{// Negative Quadratic Outside Red Multiplier Function

				double b = (theApp.m_dAnimationOutsideRedMult[0]-dOutRedMultBU)
							/ ((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dOutRedMult = a*counter*counter + b*counter + dOutRedMultBU;

			}

			if(theApp.m_iAnimationOutsideGreenMultFunction[0] == 0)
			{// Linear Outside Green Multiplier Function

				theApp.m_dOutGreenMult += (theApp.m_dAnimationOutsideGreenMult[0]-dOutGreenMultBU)
										/ (double)(theApp.m_iFramePoint[0]-1);

			}
			else if(theApp.m_iAnimationOutsideGreenMultFunction[0] == 1)
			{// Positive Quadratic Outside Green Multiplier Function

				double a = (theApp.m_dAnimationOutsideGreenMult[0]-dOutGreenMultBU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2);

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dOutGreenMult = a*counter*counter + dOutGreenMultBU;

			}
			else if(theApp.m_iAnimationOutsideGreenMultFunction[0] == 2)
			{// Negative Quadratic Outside Green Multiplier Function

				double b = (theApp.m_dAnimationOutsideGreenMult[0]-dOutGreenMultBU)
							/ ((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dOutGreenMult = a*counter*counter + b*counter + dOutGreenMultBU;

			}

			if(theApp.m_iAnimationOutsideBlueMultFunction[0] == 0)
			{// Linear Outside Blue Multiplier Function

				theApp.m_dOutBlueMult += (theApp.m_dAnimationOutsideBlueMult[0]-dOutBlueMultBU)
										/ (double)(theApp.m_iFramePoint[0]-1);

			}
			else if(theApp.m_iAnimationOutsideBlueMultFunction[0] == 1)
			{// Positive Quadratic Outside Blue Multiplier Function

				double a = (theApp.m_dAnimationOutsideBlueMult[0]-dOutBlueMultBU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2);

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dOutBlueMult = a*counter*counter + dOutBlueMultBU;

			}
			else if(theApp.m_iAnimationOutsideBlueMultFunction[0] == 2)
			{// Negative Quadratic Outside Blue Multiplier Function

				double b = (theApp.m_dAnimationOutsideBlueMult[0]-dOutBlueMultBU)
							/ ((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dOutBlueMult = a*counter*counter + b*counter + dOutBlueMultBU;

			}


			// Inside Coloring Functions

			if(theApp.m_iAnimationInsideRedAdderFunction[0] == 0)
			{// Linear Inside Red Adder Function

				dInRedAdder += (double)((double)(theApp.m_dAnimationInsideRedAdder[0]
								- iInRedAdderBU) / (double)(theApp.m_iFramePoint[0]-1));

				theApp.m_iInRedAdder = (int)(dInRedAdder+0.5);

			}
			else if(theApp.m_iAnimationInsideRedAdderFunction[0] == 1)
			{// Positive Quadratic Inside Red Adder Function

				double a = (double)((theApp.m_dAnimationInsideRedAdder[0]-iInRedAdderBU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2));

				double counter = (double)(theApp.m_iFrameCount-1);

				dInRedAdder = a*counter*counter + iInRedAdderBU;

				theApp.m_iInRedAdder = (int)(dInRedAdder+0.5);

			}
			else if(theApp.m_iAnimationInsideRedAdderFunction[0] == 2)
			{// Negative Quadratic Inside Red Adder Function

				double b = (theApp.m_dAnimationInsideRedAdder[0]-iInRedAdderBU)
							/ ((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);

				double counter = (double)(theApp.m_iFrameCount-1);

				dInRedAdder = a*counter*counter + b*counter + iInRedAdderBU;

				theApp.m_iInRedAdder = (int)(dInRedAdder+0.5);

			}

			if(theApp.m_iAnimationInsideGreenAdderFunction[0] == 0)
			{// Linear Inside Green Adder Function

				dInGreenAdder += (double)((double)(theApp.m_dAnimationInsideGreenAdder[0]
								- iInGreenAdderBU) / (double)(theApp.m_iFramePoint[0]-1));

				theApp.m_iInGreenAdder = (int)(dInGreenAdder+0.5);

			}
			else if(theApp.m_iAnimationInsideGreenAdderFunction[0] == 1)
			{// Positive Quadratic Inside Green Adder Function

				double a = (double)((theApp.m_dAnimationInsideGreenAdder[0]-iInGreenAdderBU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2));

				double counter = (double)(theApp.m_iFrameCount-1);

				dInGreenAdder = a*counter*counter + iInGreenAdderBU;

				theApp.m_iInGreenAdder = (int)(dInGreenAdder+0.5);

			}
			else if(theApp.m_iAnimationInsideGreenAdderFunction[0] == 2)
			{// Negative Quadratic Inside Green Adder Function

				double b = (theApp.m_dAnimationInsideGreenAdder[0]-iInGreenAdderBU)
							/ ((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);

				double counter = (double)(theApp.m_iFrameCount-1);

				dInGreenAdder = a*counter*counter + b*counter + iInGreenAdderBU;

				theApp.m_iInGreenAdder = (int)(dInGreenAdder+0.5);

			}

			if(theApp.m_iAnimationInsideBlueAdderFunction[0] == 0)
			{// Linear Inside Blue Adder Function

				dInBlueAdder += (double)((double)(theApp.m_dAnimationInsideBlueAdder[0]
								- iInBlueAdderBU) / (double)(theApp.m_iFramePoint[0]-1));

				theApp.m_iInBlueAdder = (int)(dInBlueAdder+0.5);

			}
			else if(theApp.m_iAnimationInsideBlueAdderFunction[0] == 1)
			{// Positive Quadratic Inside Blue Adder Function

				double a = (double)((theApp.m_dAnimationInsideBlueAdder[0]-iInBlueAdderBU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2));

				double counter = (double)(theApp.m_iFrameCount-1);

				dInBlueAdder = a*counter*counter + iInBlueAdderBU;

				theApp.m_iInBlueAdder = (int)(dInBlueAdder+0.5);

			}
			else if(theApp.m_iAnimationInsideBlueAdderFunction[0] == 2)
			{// Negative Quadratic Inside Blue Adder Function

				double b = (theApp.m_dAnimationInsideBlueAdder[0]-iInBlueAdderBU)
							/ ((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);

				double counter = (double)(theApp.m_iFrameCount-1);

				dInBlueAdder = a*counter*counter + b*counter + iInBlueAdderBU;

				theApp.m_iInBlueAdder = (int)(dInBlueAdder+0.5);

			}


			if(theApp.m_iAnimationInsideRedMultFunction[0] == 0)
			{// Linear Inside Red Multiplier Function

				theApp.m_dInRedMult += (theApp.m_dAnimationInsideRedMult[0]-dInRedMultBU)
										/ (double)(theApp.m_iFramePoint[0]-1);

			}
			else if(theApp.m_iAnimationInsideRedMultFunction[0] == 1)
			{// Positive Quadratic Inside Red Multiplier Function

				double a = (theApp.m_dAnimationInsideRedMult[0]-dInRedMultBU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2);

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dInRedMult = a*counter*counter + dInRedMultBU;

			}
			else if(theApp.m_iAnimationInsideRedMultFunction[0] == 2)
			{// Negative Quadratic Inside Red Multiplier Function

				double b = (theApp.m_dAnimationInsideRedMult[0]-dInRedMultBU)
							/ ((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dInRedMult = a*counter*counter + b*counter + dInRedMultBU;

			}

			if(theApp.m_iAnimationInsideGreenMultFunction[0] == 0)
			{// Linear Inside Green Multiplier Function

				theApp.m_dInGreenMult += (theApp.m_dAnimationInsideGreenMult[0]-dInGreenMultBU)
										/ (double)(theApp.m_iFramePoint[0]-1);

			}
			else if(theApp.m_iAnimationInsideGreenMultFunction[0] == 1)
			{// Positive Quadratic Inside Green Multiplier Function

				double a = (theApp.m_dAnimationInsideGreenMult[0]-dInGreenMultBU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2);

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dInGreenMult = a*counter*counter + dInGreenMultBU;

			}
			else if(theApp.m_iAnimationInsideGreenMultFunction[0] == 2)
			{// Negative Quadratic Inside Green Multiplier Function

				double b = (theApp.m_dAnimationInsideGreenMult[0]-dInGreenMultBU)
							/ ((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dInGreenMult = a*counter*counter + b*counter + dInGreenMultBU;

			}

			if(theApp.m_iAnimationInsideBlueMultFunction[0] == 0)
			{// Linear Inside Blue Multiplier Function

				theApp.m_dInBlueMult += (theApp.m_dAnimationInsideBlueMult[0]-dInBlueMultBU)
										/ (double)(theApp.m_iFramePoint[0]-1);

			}
			else if(theApp.m_iAnimationInsideBlueMultFunction[0] == 1)
			{// Positive Quadratic Inside Blue Multiplier Function

				double a = (theApp.m_dAnimationInsideBlueMult[0]-dInBlueMultBU)
							/ pow((double)(theApp.m_iFramePoint[0]-1),2);

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dInBlueMult = a*counter*counter + dInBlueMultBU;

			}
			else if(theApp.m_iAnimationInsideBlueMultFunction[0] == 2)
			{// Negative Quadratic Inside Blue Multiplier Function

				double b = (theApp.m_dAnimationInsideBlueMult[0]-dInBlueMultBU)
							/ ((theApp.m_iFramePoint[0]-1)/2.0);

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);

				double counter = (double)(theApp.m_iFrameCount-1);

				theApp.m_dInBlueMult = a*counter*counter + b*counter + dInBlueMultBU;

			}

		}


		else if( (theApp.m_iFrameCount > theApp.m_iFramePoint[0])
				&& (theApp.m_iFrameCount <= theApp.m_iFramePoint[1]) )
		{
			triggerPoint = 1;
		}
		else if( (theApp.m_iFrameCount > theApp.m_iFramePoint[1])
				&& (theApp.m_iFrameCount <= theApp.m_iFramePoint[2]) )
		{
			triggerPoint = 2;
		}
		else if( (theApp.m_iFrameCount > theApp.m_iFramePoint[2])
				&& (theApp.m_iFrameCount <= theApp.m_iFramePoint[3]) )
		{
			triggerPoint = 3;
		}
		else if( (theApp.m_iFrameCount > theApp.m_iFramePoint[3])
				&& (theApp.m_iFrameCount <= theApp.m_iFramePoint[4]) )
		{
			triggerPoint = 4;
		}
		else if( (theApp.m_iFrameCount > theApp.m_iFramePoint[4])
				&& (theApp.m_iFrameCount <= theApp.m_iFramePoint[5]) )
		{
			triggerPoint = 5;
		}
		else if( (theApp.m_iFrameCount > theApp.m_iFramePoint[5])
				&& (theApp.m_iFrameCount <= theApp.m_iFramePoint[6]) )
		{
			triggerPoint = 6;
		}
		else if( (theApp.m_iFrameCount > theApp.m_iFramePoint[6])
				&& (theApp.m_iFrameCount <= theApp.m_iFramePoint[7]) )
		{
			triggerPoint = 7;
		}


		if( (theApp.m_iFrameCount > theApp.m_iFramePoint[triggerPoint-1])
			&& (theApp.m_iFrameCount <= theApp.m_iFramePoint[triggerPoint]) )
		{// Trigger Point Functions

			if(theApp.m_iTranslationFunction[triggerPoint] == 0)
			{// Linear Translation Function

				theApp.m_dXAxis += (theApp.m_dEndXAxis[triggerPoint] - theApp.m_dEndXAxis[triggerPoint-1])
							/(double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]);
					
				theApp.m_dYAxis += (theApp.m_dEndYAxis[triggerPoint] - theApp.m_dEndYAxis[triggerPoint-1])
							/(double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]);

			}
			else if(theApp.m_iTranslationFunction[triggerPoint] == 1)
			{// Positive Quadratic Translation Function

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				double aX = (theApp.m_dEndXAxis[triggerPoint]-theApp.m_dEndXAxis[triggerPoint-1])
							/ pow((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2.0);

				theApp.m_dXAxis = aX*counter*counter + theApp.m_dEndXAxis[triggerPoint-1];

				double aY = (theApp.m_dEndYAxis[triggerPoint]-theApp.m_dEndYAxis[triggerPoint-1])
							/pow((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2.0);

				theApp.m_dYAxis = aY*counter*counter + theApp.m_dEndYAxis[triggerPoint-1];

			}
			else if(theApp.m_iTranslationFunction[triggerPoint] == 2)
			{// Negative Quadratic Translation Function

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				double bX = (theApp.m_dEndXAxis[triggerPoint]-theApp.m_dEndXAxis[triggerPoint-1])
							/ ((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);

				double aX = -bX/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				theApp.m_dXAxis = aX*counter*counter + bX*counter + theApp.m_dEndXAxis[triggerPoint-1];

				double bY = (theApp.m_dEndYAxis[triggerPoint]-theApp.m_dEndYAxis[triggerPoint-1])
							/ ((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);

				double aY = -bY/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				theApp.m_dYAxis = aY*counter*counter + bY*counter + theApp.m_dEndYAxis[triggerPoint-1];

			}
			else if(theApp.m_iTranslationFunction[triggerPoint] == 3)
			{// Negative Cosine Translation Function

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dXAxis = theApp.m_dEndXAxis[triggerPoint-1]
									+ (theApp.m_dEndXAxis[triggerPoint]
									- theApp.m_dEndXAxis[triggerPoint-1])
									* (-cos(PI*counter/(theApp.m_iFramePoint[triggerPoint]
									- theApp.m_iFramePoint[triggerPoint-1]))+1)/2;

				theApp.m_dYAxis = theApp.m_dEndYAxis[triggerPoint-1]
									+ (theApp.m_dEndYAxis[triggerPoint]
									- theApp.m_dEndYAxis[triggerPoint-1])
									* (-cos(PI*counter/(theApp.m_iFramePoint[triggerPoint]
									- theApp.m_iFramePoint[triggerPoint-1]))+1)/2;

			}

			if(theApp.m_iRotationFunction[triggerPoint] == 0)
			{// Linear Rotation Function

				theApp.m_dRotation += (theApp.m_dEndRotation[triggerPoint]
										- theApp.m_dEndRotation[triggerPoint-1])
										/ (double)(theApp.m_iFramePoint[triggerPoint]
										- theApp.m_iFramePoint[triggerPoint-1]);

			}
			else if(theApp.m_iRotationFunction[triggerPoint] == 1)
			{// Positive Quadratic Rotation Function

				double counter = (double)(theApp.m_iFrameCount - theApp.m_iFramePoint[triggerPoint-1]);

				double a = (theApp.m_dEndRotation[triggerPoint]
							- theApp.m_dEndRotation[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2);

				theApp.m_dRotation = a*counter*counter + theApp.m_dEndRotation[triggerPoint-1];

			}
			else if(theApp.m_iRotationFunction[triggerPoint] == 2)
			{// Negative Quadratic Rotation Function

				double counter = (double)(theApp.m_iFrameCount - theApp.m_iFramePoint[triggerPoint-1]);

				double b = (theApp.m_dEndRotation[triggerPoint]
							- theApp.m_dEndRotation[triggerPoint-1])
							/ ((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);

				double a = -b/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				theApp.m_dRotation = a*counter*counter + b*counter
									+ theApp.m_dEndRotation[triggerPoint-1];

			}

			if(theApp.m_iZoomRatioFunction[triggerPoint] == 0)
			{// Linear Zoom Ratio Function

				theApp.m_dZoomRatio += (theApp.m_dEndZoomRatio[triggerPoint]
										- theApp.m_dEndZoomRatio[triggerPoint-1])
										/ (double)(theApp.m_iFramePoint[triggerPoint]
										- theApp.m_iFramePoint[triggerPoint-1]);

			}
			else if(theApp.m_iZoomRatioFunction[triggerPoint] == 1)
			{// Summation Zoom Ratio Function

				theApp.m_dZoomRatio += (theApp.m_iFrameCount - theApp.m_iFramePoint[triggerPoint-1])
								*(theApp.m_dEndZoomRatio[triggerPoint]
								- theApp.m_dEndZoomRatio[triggerPoint-1])
								/ (double)((theApp.m_iFramePoint[triggerPoint]
								- theApp.m_iFramePoint[triggerPoint-1])
								* (theApp.m_iFramePoint[triggerPoint]
								- theApp.m_iFramePoint[triggerPoint-1]+1)/2.0);

			}
			else if(theApp.m_iZoomRatioFunction[triggerPoint] == 2)
			{// Positive Quadratic Zoom Ratio Function

				double counter = (double)(theApp.m_iFrameCount - theApp.m_iFramePoint[triggerPoint-1]);

				double a = (theApp.m_dEndZoomRatio[triggerPoint]
							- theApp.m_dEndZoomRatio[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2);

				theApp.m_dZoomRatio = a*counter*counter + theApp.m_dEndZoomRatio[triggerPoint-1];

			}
			else if(theApp.m_iZoomRatioFunction[triggerPoint] == 3)
			{// Negative Quadratic Zoom Ratio Function

				double b = (theApp.m_dEndZoomRatio[triggerPoint]
							- theApp.m_dEndZoomRatio[triggerPoint-1])
							/ ((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);

				double a = -b/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				double counter = (double)(theApp.m_iFrameCount - theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dZoomRatio = a*counter*counter + b*counter
										+ theApp.m_dEndZoomRatio[triggerPoint-1];

			}

			if(theApp.m_iIterationFunction[triggerPoint] == 0)
			{// Linear Iteration Function

				dIteration += (double)((double)(theApp.m_iEndIteration[triggerPoint]
								- theApp.m_iEndIteration[triggerPoint-1])
								/ (double)(theApp.m_iFramePoint[triggerPoint]
								- theApp.m_iFramePoint[triggerPoint-1]));

				theApp.m_iIteration = (int)(dIteration+0.5);

			}
			else if(theApp.m_iIterationFunction[triggerPoint] == 1)
			{// Positive Quadratic Iteration Function

				double a = (double)((theApp.m_iEndIteration[triggerPoint]
							- theApp.m_iEndIteration[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2));

				double counter = (double)(theApp.m_iFrameCount - theApp.m_iFramePoint[triggerPoint-1]);

				dIteration = a*counter*counter + theApp.m_iEndIteration[triggerPoint-1];

				theApp.m_iIteration = (int)(dIteration+0.5);

			}
			else if(theApp.m_iIterationFunction[triggerPoint] == 2)
			{// Negative Quadratic Iteration Function

				double b = (double)((theApp.m_iEndIteration[triggerPoint]
							- theApp.m_iEndIteration[triggerPoint-1])
							/ ((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0));

				double a = -b/((theApp.m_iFramePoint[0]-1)*2.0);

				double counter = (double)(theApp.m_iFrameCount - theApp.m_iFramePoint[triggerPoint-1]);

				dIteration = a*counter*counter + b*counter + theApp.m_iEndIteration[triggerPoint-1];

				theApp.m_iIteration = (int)(dIteration+0.5);

			}


			// Formula Control Functions

			if(theApp.m_iAnimationControl1Function[triggerPoint] == 0)
			{// Linear Control #1 Function

				theApp.m_dControl1 += (theApp.m_dAnimationControl1[triggerPoint]
										- theApp.m_dAnimationControl1[triggerPoint-1])
										/ (theApp.m_iFramePoint[triggerPoint]
										- theApp.m_iFramePoint[triggerPoint-1]);

			}
			else if(theApp.m_iAnimationControl1Function[triggerPoint] == 1)
			{// Positive Quadratic Control #1 Function

				double counter = (double)(theApp.m_iFrameCount - theApp.m_iFramePoint[triggerPoint-1]);

				double a = (theApp.m_dAnimationControl1[triggerPoint]
							- theApp.m_dAnimationControl1[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2);

				theApp.m_dControl1 = a*counter*counter + theApp.m_dAnimationControl1[triggerPoint-1];

			}
			else if(theApp.m_iAnimationControl1Function[triggerPoint] == 2)
			{// Negative Quadratic Control #1 Function

				double b = (theApp.m_dAnimationControl1[triggerPoint]
							- theApp.m_dAnimationControl1[triggerPoint-1])
							/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);

				double a = -b/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				double counter = (double)(theApp.m_iFrameCount - theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dControl1 = a*counter*counter + b*counter
								+ theApp.m_dAnimationControl1[triggerPoint-1];

			}
			else if(theApp.m_iAnimationControl1Function[triggerPoint] == 3)
			{// Negative Cosine Control #1 Function

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dControl1 = theApp.m_dAnimationControl1[triggerPoint-1]
									+ (theApp.m_dAnimationControl1[triggerPoint]
									- theApp.m_dAnimationControl1[triggerPoint-1])
									* (-cos(PI*counter/(theApp.m_iFramePoint[triggerPoint]
									- theApp.m_iFramePoint[triggerPoint-1]))+1)/2;

			}

			if(theApp.m_iAnimationControl2Function[triggerPoint] == 0)
			{// Linear Control #2 Function

				theApp.m_dControl2 += (theApp.m_dAnimationControl2[triggerPoint]
										- theApp.m_dAnimationControl2[triggerPoint-1])
										/ (theApp.m_iFramePoint[triggerPoint]
										- theApp.m_iFramePoint[triggerPoint-1]);

			}
			else if(theApp.m_iAnimationControl2Function[triggerPoint] == 1)
			{// Positive Quadratic Control #2 Function

				double counter = (double)(theApp.m_iFrameCount - theApp.m_iFramePoint[triggerPoint-1]);

				double a = (theApp.m_dAnimationControl2[triggerPoint]
							- theApp.m_dAnimationControl2[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2);

				theApp.m_dControl2 = a*counter*counter + theApp.m_dAnimationControl2[triggerPoint-1];

			}
			else if(theApp.m_iAnimationControl2Function[triggerPoint] == 2)
			{// Negative Quadratic Control #2 Function

				double b = (theApp.m_dAnimationControl2[triggerPoint]
							- theApp.m_dAnimationControl2[triggerPoint-1])
							/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);

				double a = -b/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				double counter = (double)(theApp.m_iFrameCount - theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dControl2 = a*counter*counter + b*counter
								+ theApp.m_dAnimationControl2[triggerPoint-1];

			}
			else if(theApp.m_iAnimationControl2Function[triggerPoint] == 3)
			{// Negative Cosine Control #2 Function

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dControl2 = theApp.m_dAnimationControl2[triggerPoint-1]
									+ (theApp.m_dAnimationControl2[triggerPoint]
									- theApp.m_dAnimationControl2[triggerPoint-1])
									* (-cos(PI*counter/(theApp.m_iFramePoint[triggerPoint]
									- theApp.m_iFramePoint[triggerPoint-1]))+1)/2;

			}

			if(theApp.m_iAnimationControl3Function[triggerPoint] == 0)
			{// Linear Control #3 Function

				theApp.m_dControl3 += (theApp.m_dAnimationControl3[triggerPoint]
										- theApp.m_dAnimationControl3[triggerPoint-1])
										/ (theApp.m_iFramePoint[triggerPoint]
										- theApp.m_iFramePoint[triggerPoint-1]);

			}
			else if(theApp.m_iAnimationControl3Function[triggerPoint] == 1)
			{// Positive Quadratic Control #3 Function

				double counter = (double)(theApp.m_iFrameCount - theApp.m_iFramePoint[triggerPoint-1]);

				double a = (theApp.m_dAnimationControl3[triggerPoint]
							- theApp.m_dAnimationControl3[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2);

				theApp.m_dControl3 = a*counter*counter + theApp.m_dAnimationControl3[triggerPoint-1];

			}
			else if(theApp.m_iAnimationControl3Function[triggerPoint] == 2)
			{// Negative Quadratic Control #3 Function

				double b = (theApp.m_dAnimationControl3[triggerPoint]
							- theApp.m_dAnimationControl3[triggerPoint-1])
							/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);

				double a = -b/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				double counter = (double)(theApp.m_iFrameCount - theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dControl3 = a*counter*counter + b*counter
								+ theApp.m_dAnimationControl3[triggerPoint-1];

			}
			else if(theApp.m_iAnimationControl3Function[triggerPoint] == 3)
			{// Negative Cosine Control #3 Function

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dControl3 = theApp.m_dAnimationControl3[triggerPoint-1]
									+ (theApp.m_dAnimationControl3[triggerPoint]
									- theApp.m_dAnimationControl3[triggerPoint-1])
									* (-cos(PI*counter/(theApp.m_iFramePoint[triggerPoint]
									- theApp.m_iFramePoint[triggerPoint-1]))+1)/2;

			}

			if(theApp.m_iAnimationControl4Function[triggerPoint] == 0)
			{// Linear Control #4 Function

				theApp.m_dControl4 += (theApp.m_dAnimationControl4[triggerPoint]
										- theApp.m_dAnimationControl4[triggerPoint-1])
										/ (theApp.m_iFramePoint[triggerPoint]
										- theApp.m_iFramePoint[triggerPoint-1]);

			}
			else if(theApp.m_iAnimationControl4Function[triggerPoint] == 1)
			{// Positive Quadratic Control #4 Function

				double counter = (double)(theApp.m_iFrameCount - theApp.m_iFramePoint[triggerPoint-1]);

				double a = (theApp.m_dAnimationControl4[triggerPoint]
							- theApp.m_dAnimationControl4[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2);

				theApp.m_dControl4 = a*counter*counter + theApp.m_dAnimationControl4[triggerPoint-1];

			}
			else if(theApp.m_iAnimationControl4Function[triggerPoint] == 2)
			{// Negative Quadratic Control #4 Function

				double b = (theApp.m_dAnimationControl4[triggerPoint]
							- theApp.m_dAnimationControl4[triggerPoint-1])
							/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);

				double a = -b/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				double counter = (double)(theApp.m_iFrameCount - theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dControl4 = a*counter*counter + b*counter
								+ theApp.m_dAnimationControl4[triggerPoint-1];

			}
			else if(theApp.m_iAnimationControl4Function[triggerPoint] == 3)
			{// Negative Cosine Control #4 Function

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dControl4 = theApp.m_dAnimationControl4[triggerPoint-1]
									+ (theApp.m_dAnimationControl4[triggerPoint]
									- theApp.m_dAnimationControl4[triggerPoint-1])
									* (-cos(PI*counter/(theApp.m_iFramePoint[triggerPoint]
									- theApp.m_iFramePoint[triggerPoint-1]))+1)/2;

			}

			if(theApp.m_iAnimationControl5Function[triggerPoint] == 0)
			{// Linear Control #5 Function

				theApp.m_dControl5 += (theApp.m_dAnimationControl5[triggerPoint]
										- theApp.m_dAnimationControl5[triggerPoint-1])
										/ (theApp.m_iFramePoint[triggerPoint]
										- theApp.m_iFramePoint[triggerPoint-1]);

			}
			else if(theApp.m_iAnimationControl5Function[triggerPoint] == 1)
			{// Positive Quadratic Control #5 Function

				double counter = (double)(theApp.m_iFrameCount - theApp.m_iFramePoint[triggerPoint-1]);

				double a = (theApp.m_dAnimationControl5[triggerPoint]
							- theApp.m_dAnimationControl5[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2);

				theApp.m_dControl5 = a*counter*counter + theApp.m_dAnimationControl5[triggerPoint-1];

			}
			else if(theApp.m_iAnimationControl5Function[triggerPoint] == 2)
			{// Negative Quadratic Control #5 Function

				double b = (theApp.m_dAnimationControl5[triggerPoint]
							- theApp.m_dAnimationControl5[triggerPoint-1])
							/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);

				double a = -b/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				double counter = (double)(theApp.m_iFrameCount - theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dControl5 = a*counter*counter + b*counter
								+ theApp.m_dAnimationControl5[triggerPoint-1];

			}
			else if(theApp.m_iAnimationControl5Function[triggerPoint] == 3)
			{// Negative Cosine Control #5 Function

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dControl5 = theApp.m_dAnimationControl5[triggerPoint-1]
									+ (theApp.m_dAnimationControl5[triggerPoint]
									- theApp.m_dAnimationControl5[triggerPoint-1])
									* (-cos(PI*counter/(theApp.m_iFramePoint[triggerPoint]
									- theApp.m_iFramePoint[triggerPoint-1]))+1)/2;

			}

			if(theApp.m_iAnimationControl6Function[triggerPoint] == 0)
			{// Linear Control #6 Function

				theApp.m_dControl6 += (theApp.m_dAnimationControl6[triggerPoint]
										- theApp.m_dAnimationControl6[triggerPoint-1])
										/ (theApp.m_iFramePoint[triggerPoint]
										- theApp.m_iFramePoint[triggerPoint-1]);

			}
			else if(theApp.m_iAnimationControl6Function[triggerPoint] == 1)
			{// Positive Quadratic Control #6 Function

				double counter = (double)(theApp.m_iFrameCount - theApp.m_iFramePoint[triggerPoint-1]);

				double a = (theApp.m_dAnimationControl6[triggerPoint]
							- theApp.m_dAnimationControl6[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2);

				theApp.m_dControl6 = a*counter*counter + theApp.m_dAnimationControl6[triggerPoint-1];

			}
			else if(theApp.m_iAnimationControl6Function[triggerPoint] == 2)
			{// Negative Quadratic Control #6 Function

				double b = (theApp.m_dAnimationControl6[triggerPoint]
							- theApp.m_dAnimationControl6[triggerPoint-1])
							/ ((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);

				double a = -b/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				double counter = (double)(theApp.m_iFrameCount - theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dControl6 = a*counter*counter + b*counter
								+ theApp.m_dAnimationControl6[triggerPoint-1];

			}
			else if(theApp.m_iAnimationControl6Function[triggerPoint] == 3)
			{// Negative Cosine Control #6 Function

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dControl6 = theApp.m_dAnimationControl6[triggerPoint-1]
									+ (theApp.m_dAnimationControl6[triggerPoint]
									- theApp.m_dAnimationControl6[triggerPoint-1])
									* (-cos(PI*counter/(theApp.m_iFramePoint[triggerPoint]
									- theApp.m_iFramePoint[triggerPoint-1]))+1)/2;

			}


			// Outside Coloring Functions

			if(theApp.m_iAnimationOutsideRedAdderFunction[triggerPoint] == 0)
			{// Linear Outside Red Adder Function

				dOutRedAdder += (double)((double)(theApp.m_dAnimationOutsideRedAdder[triggerPoint]
								- theApp.m_dAnimationOutsideRedAdder[triggerPoint-1])
								/ (theApp.m_iFramePoint[triggerPoint]
								- theApp.m_iFramePoint[triggerPoint-1]));

				theApp.m_iOutRedAdder = (int)(dOutRedAdder+0.5);

			}
			else if(theApp.m_iAnimationOutsideRedAdderFunction[triggerPoint] == 1)
			{// Positive Quadratic Outside Red Adder Function

				double a = (double)((theApp.m_dAnimationOutsideRedAdder[triggerPoint]
							- theApp.m_dAnimationOutsideRedAdder[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2));

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				dOutRedAdder = a*counter*counter + theApp.m_dAnimationOutsideRedAdder[triggerPoint-1];

				theApp.m_iOutRedAdder = (int)(dOutRedAdder+0.5);

			}
			else if(theApp.m_iAnimationOutsideRedAdderFunction[triggerPoint] == 2)
			{// Negative Quadratic Outside Red Adder Function

				double b = (theApp.m_dAnimationOutsideRedAdder[triggerPoint]
							- theApp.m_dAnimationOutsideRedAdder[triggerPoint-1])
							/ ((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);

				double a = -b/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				dOutRedAdder = a*counter*counter + b*counter
								+ theApp.m_dAnimationOutsideRedAdder[triggerPoint-1];

				theApp.m_iOutRedAdder = (int)(dOutRedAdder+0.5);

			}

			if(theApp.m_iAnimationOutsideGreenAdderFunction[triggerPoint] == 0)
			{// Linear Outside Green Adder Function

				dOutGreenAdder += (double)((double)(theApp.m_dAnimationOutsideGreenAdder[triggerPoint]
								- theApp.m_dAnimationOutsideGreenAdder[triggerPoint-1])
								/ (theApp.m_iFramePoint[triggerPoint]
								- theApp.m_iFramePoint[triggerPoint-1]));

				theApp.m_iOutGreenAdder = (int)(dOutGreenAdder+0.5);

			}
			else if(theApp.m_iAnimationOutsideGreenAdderFunction[triggerPoint] == 1)
			{// Positive Quadratic Outside Green Adder Function

				double a = (double)((theApp.m_dAnimationOutsideGreenAdder[triggerPoint]
							- theApp.m_dAnimationOutsideGreenAdder[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2));

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				dOutGreenAdder = a*counter*counter + theApp.m_dAnimationOutsideGreenAdder[triggerPoint-1];

				theApp.m_iOutGreenAdder = (int)(dOutGreenAdder+0.5);

			}
			else if(theApp.m_iAnimationOutsideGreenAdderFunction[triggerPoint] == 2)
			{// Negative Quadratic Outside Green Adder Function

				double b = (theApp.m_dAnimationOutsideGreenAdder[triggerPoint]
							- theApp.m_dAnimationOutsideGreenAdder[triggerPoint-1])
							/ ((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);

				double a = -b/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				dOutGreenAdder = a*counter*counter + b*counter
								+ theApp.m_dAnimationOutsideGreenAdder[triggerPoint-1];

				theApp.m_iOutGreenAdder = (int)(dOutGreenAdder+0.5);

			}

			if(theApp.m_iAnimationOutsideBlueAdderFunction[triggerPoint] == 0)
			{// Linear Outside Blue Adder Function

				dOutBlueAdder += (double)((double)(theApp.m_dAnimationOutsideBlueAdder[triggerPoint]
								- theApp.m_dAnimationOutsideBlueAdder[triggerPoint-1])
								/ (theApp.m_iFramePoint[triggerPoint]
								- theApp.m_iFramePoint[triggerPoint-1]));

				theApp.m_iOutBlueAdder = (int)(dOutBlueAdder+0.5);

			}
			else if(theApp.m_iAnimationOutsideBlueAdderFunction[triggerPoint] == 1)
			{// Positive Quadratic Outside Blue Adder Function

				double a = (double)((theApp.m_dAnimationOutsideBlueAdder[triggerPoint]
							- theApp.m_dAnimationOutsideBlueAdder[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2));

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				dOutBlueAdder = a*counter*counter + theApp.m_dAnimationOutsideBlueAdder[triggerPoint-1];

				theApp.m_iOutBlueAdder = (int)(dOutBlueAdder+0.5);

			}
			else if(theApp.m_iAnimationOutsideBlueAdderFunction[triggerPoint] == 2)
			{// Negative Quadratic Outside Blue Adder Function

				double b = (theApp.m_dAnimationOutsideBlueAdder[triggerPoint]
							- theApp.m_dAnimationOutsideBlueAdder[triggerPoint-1])
							/ ((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);

				double a = -b/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				dOutBlueAdder = a*counter*counter + b*counter
								+ theApp.m_dAnimationOutsideBlueAdder[triggerPoint-1];

				theApp.m_iOutBlueAdder = (int)(dOutBlueAdder+0.5);

			}

			if(theApp.m_iAnimationOutsideRedMultFunction[triggerPoint] == 0)
			{// Linear Outside Red Multiplier Function

				theApp.m_dOutRedMult += (theApp.m_dAnimationOutsideRedMult[triggerPoint]
										- theApp.m_dAnimationOutsideRedMult[triggerPoint-1])
										/ (double)(theApp.m_iFramePoint[triggerPoint]
										- theApp.m_iFramePoint[triggerPoint-1]);

			}
			else if(theApp.m_iAnimationOutsideRedMultFunction[triggerPoint] == 1)
			{// Positive Quadratic Outside Red Multiplier Function

				double a = (theApp.m_dAnimationOutsideRedMult[triggerPoint]
							- theApp.m_dAnimationOutsideRedMult[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2);

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dOutRedMult = a*counter*counter 
										+ theApp.m_dAnimationOutsideRedMult[triggerPoint-1];

			}
			else if(theApp.m_iAnimationOutsideRedMultFunction[triggerPoint] == 2)
			{// Negative Quadratic Outside Red Multiplier Function

				double b = (theApp.m_dAnimationOutsideRedMult[triggerPoint]
							- theApp.m_dAnimationOutsideRedMult[triggerPoint-1])
							/ ((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);


				double a = -b/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dOutRedMult = a*counter*counter + b*counter
										+ theApp.m_dAnimationOutsideRedMult[triggerPoint-1];

			}

			if(theApp.m_iAnimationOutsideGreenMultFunction[triggerPoint] == 0)
			{// Linear Outside Green Multiplier Function

				theApp.m_dOutGreenMult += (theApp.m_dAnimationOutsideGreenMult[triggerPoint]
										- theApp.m_dAnimationOutsideGreenMult[triggerPoint-1])
										/ (double)(theApp.m_iFramePoint[triggerPoint]
										- theApp.m_iFramePoint[triggerPoint-1]);

			}
			else if(theApp.m_iAnimationOutsideGreenMultFunction[triggerPoint] == 1)
			{// Positive Quadratic Outside Green Multiplier Function

				double a = (theApp.m_dAnimationOutsideGreenMult[triggerPoint]
							- theApp.m_dAnimationOutsideGreenMult[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2);

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dOutGreenMult = a*counter*counter 
										+ theApp.m_dAnimationOutsideGreenMult[triggerPoint-1];

			}
			else if(theApp.m_iAnimationOutsideGreenMultFunction[triggerPoint] == 2)
			{// Negative Quadratic Outside Green Multiplier Function

				double b = (theApp.m_dAnimationOutsideGreenMult[triggerPoint]
							- theApp.m_dAnimationOutsideGreenMult[triggerPoint-1])
							/ ((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);


				double a = -b/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dOutGreenMult = a*counter*counter + b*counter
										+ theApp.m_dAnimationOutsideGreenMult[triggerPoint-1];

			}

			if(theApp.m_iAnimationOutsideBlueMultFunction[triggerPoint] == 0)
			{// Linear Outside Blue Multiplier Function

				theApp.m_dOutBlueMult += (theApp.m_dAnimationOutsideBlueMult[triggerPoint]
										- theApp.m_dAnimationOutsideBlueMult[triggerPoint-1])
										/ (double)(theApp.m_iFramePoint[triggerPoint]
										- theApp.m_iFramePoint[triggerPoint-1]);

			}
			else if(theApp.m_iAnimationOutsideBlueMultFunction[triggerPoint] == 1)
			{// Positive Quadratic Outside Blue Multiplier Function

				double a = (theApp.m_dAnimationOutsideBlueMult[triggerPoint]
							- theApp.m_dAnimationOutsideBlueMult[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2);

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dOutBlueMult = a*counter*counter 
										+ theApp.m_dAnimationOutsideBlueMult[triggerPoint-1];

			}
			else if(theApp.m_iAnimationOutsideBlueMultFunction[triggerPoint] == 2)
			{// Negative Quadratic Outside Blue Multiplier Function

				double b = (theApp.m_dAnimationOutsideBlueMult[triggerPoint]
							- theApp.m_dAnimationOutsideBlueMult[triggerPoint-1])
							/ ((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);


				double a = -b/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dOutBlueMult = a*counter*counter + b*counter
										+ theApp.m_dAnimationOutsideBlueMult[triggerPoint-1];

			}


			// Inside Coloring Functions

			if(theApp.m_iAnimationInsideRedAdderFunction[triggerPoint] == 0)
			{// Linear Inside Red Adder Function

				dInRedAdder += (double)((double)(theApp.m_dAnimationInsideRedAdder[triggerPoint]
								- theApp.m_dAnimationInsideRedAdder[triggerPoint-1])
								/ (theApp.m_iFramePoint[triggerPoint]
								- theApp.m_iFramePoint[triggerPoint-1]));

				theApp.m_iInRedAdder = (int)(dInRedAdder+0.5);

			}
			else if(theApp.m_iAnimationInsideRedAdderFunction[triggerPoint] == 1)
			{// Positive Quadratic Inside Red Adder Function

				double a = (double)((theApp.m_dAnimationInsideRedAdder[triggerPoint]
							- theApp.m_dAnimationInsideRedAdder[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2));

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				dInRedAdder = a*counter*counter + theApp.m_dAnimationInsideRedAdder[triggerPoint-1];

				theApp.m_iInRedAdder = (int)(dInRedAdder+0.5);

			}
			else if(theApp.m_iAnimationInsideRedAdderFunction[triggerPoint] == 2)
			{// Negative Quadratic Inside Red Adder Function

				double b = (theApp.m_dAnimationInsideRedAdder[triggerPoint]
							- theApp.m_dAnimationInsideRedAdder[triggerPoint-1])
							/ ((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);

				double a = -b/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				dInRedAdder = a*counter*counter + b*counter
								+ theApp.m_dAnimationInsideRedAdder[triggerPoint-1];

				theApp.m_iInRedAdder = (int)(dInRedAdder+0.5);

			}

			if(theApp.m_iAnimationInsideGreenAdderFunction[triggerPoint] == 0)
			{// Linear Inside Green Adder Function

				dInGreenAdder += (double)((double)(theApp.m_dAnimationInsideGreenAdder[triggerPoint]
								- theApp.m_dAnimationInsideGreenAdder[triggerPoint-1])
								/ (theApp.m_iFramePoint[triggerPoint]
								- theApp.m_iFramePoint[triggerPoint-1]));

				theApp.m_iInGreenAdder = (int)(dInGreenAdder+0.5);

			}
			else if(theApp.m_iAnimationInsideGreenAdderFunction[triggerPoint] == 1)
			{// Positive Quadratic Inside Green Adder Function

				double a = (double)((theApp.m_dAnimationInsideGreenAdder[triggerPoint]
							- theApp.m_dAnimationInsideGreenAdder[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2));

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				dInGreenAdder = a*counter*counter + theApp.m_dAnimationInsideGreenAdder[triggerPoint-1];

				theApp.m_iInGreenAdder = (int)(dInGreenAdder+0.5);

			}
			else if(theApp.m_iAnimationInsideGreenAdderFunction[triggerPoint] == 2)
			{// Negative Quadratic Inside Green Adder Function

				double b = (theApp.m_dAnimationInsideGreenAdder[triggerPoint]
							- theApp.m_dAnimationInsideGreenAdder[triggerPoint-1])
							/ ((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);

				double a = -b/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				dInGreenAdder = a*counter*counter + b*counter
								+ theApp.m_dAnimationInsideGreenAdder[triggerPoint-1];

				theApp.m_iInGreenAdder = (int)(dInGreenAdder+0.5);

			}

			if(theApp.m_iAnimationInsideBlueAdderFunction[triggerPoint] == 0)
			{// Linear Inside Blue Adder Function

				dInBlueAdder += (double)((double)(theApp.m_dAnimationInsideBlueAdder[triggerPoint]
								- theApp.m_dAnimationInsideBlueAdder[triggerPoint-1])
								/ (theApp.m_iFramePoint[triggerPoint]
								- theApp.m_iFramePoint[triggerPoint-1]));

				theApp.m_iInBlueAdder = (int)(dInBlueAdder+0.5);

			}
			else if(theApp.m_iAnimationInsideBlueAdderFunction[triggerPoint] == 1)
			{// Positive Quadratic Inside Blue Adder Function

				double a = (double)((theApp.m_dAnimationInsideBlueAdder[triggerPoint]
							- theApp.m_dAnimationInsideBlueAdder[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2));

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				dInBlueAdder = a*counter*counter + theApp.m_dAnimationInsideBlueAdder[triggerPoint-1];

				theApp.m_iInBlueAdder = (int)(dInBlueAdder+0.5);

			}
			else if(theApp.m_iAnimationInsideBlueAdderFunction[triggerPoint] == 2)
			{// Negative Quadratic Inside Blue Adder Function

				double b = (theApp.m_dAnimationInsideBlueAdder[triggerPoint]
							- theApp.m_dAnimationInsideBlueAdder[triggerPoint-1])
							/ ((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);

				double a = -b/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				dInBlueAdder = a*counter*counter + b*counter
								+ theApp.m_dAnimationInsideBlueAdder[triggerPoint-1];

				theApp.m_iInBlueAdder = (int)(dInBlueAdder+0.5);

			}

			if(theApp.m_iAnimationInsideRedMultFunction[triggerPoint] == 0)
			{// Linear Inside Red Multiplier Function

				theApp.m_dInRedMult += (theApp.m_dAnimationInsideRedMult[triggerPoint]
										- theApp.m_dAnimationInsideRedMult[triggerPoint-1])
										/ (double)(theApp.m_iFramePoint[triggerPoint]
										- theApp.m_iFramePoint[triggerPoint-1]);

			}
			else if(theApp.m_iAnimationInsideRedMultFunction[triggerPoint] == 1)
			{// Positive Quadratic Inside Red Multiplier Function

				double a = (theApp.m_dAnimationInsideRedMult[triggerPoint]
							- theApp.m_dAnimationInsideRedMult[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2);

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dInRedMult = a*counter*counter 
										+ theApp.m_dAnimationInsideRedMult[triggerPoint-1];

			}
			else if(theApp.m_iAnimationInsideRedMultFunction[triggerPoint] == 2)
			{// Negative Quadratic Inside Red Multiplier Function

				double b = (theApp.m_dAnimationInsideRedMult[triggerPoint]
							- theApp.m_dAnimationInsideRedMult[triggerPoint-1])
							/ ((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);


				double a = -b/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dInRedMult = a*counter*counter + b*counter
										+ theApp.m_dAnimationInsideRedMult[triggerPoint-1];

			}

			if(theApp.m_iAnimationInsideGreenMultFunction[triggerPoint] == 0)
			{// Linear Inside Green Multiplier Function

				theApp.m_dInGreenMult += (theApp.m_dAnimationInsideGreenMult[triggerPoint]
										- theApp.m_dAnimationInsideGreenMult[triggerPoint-1])
										/ (double)(theApp.m_iFramePoint[triggerPoint]
										- theApp.m_iFramePoint[triggerPoint-1]);

			}
			else if(theApp.m_iAnimationInsideGreenMultFunction[triggerPoint] == 1)
			{// Positive Quadratic Inside Green Multiplier Function

				double a = (theApp.m_dAnimationInsideGreenMult[triggerPoint]
							- theApp.m_dAnimationInsideGreenMult[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2);

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dInGreenMult = a*counter*counter 
										+ theApp.m_dAnimationInsideGreenMult[triggerPoint-1];

			}
			else if(theApp.m_iAnimationInsideGreenMultFunction[triggerPoint] == 2)
			{// Negative Quadratic Inside Green Multiplier Function

				double b = (theApp.m_dAnimationInsideGreenMult[triggerPoint]
							- theApp.m_dAnimationInsideGreenMult[triggerPoint-1])
							/ ((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);


				double a = -b/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dInGreenMult = a*counter*counter + b*counter
										+ theApp.m_dAnimationInsideGreenMult[triggerPoint-1];

			}

			if(theApp.m_iAnimationInsideBlueMultFunction[triggerPoint] == 0)
			{// Linear Inside Blue Multiplier Function

				theApp.m_dInBlueMult += (theApp.m_dAnimationInsideBlueMult[triggerPoint]
										- theApp.m_dAnimationInsideBlueMult[triggerPoint-1])
										/ (double)(theApp.m_iFramePoint[triggerPoint]
										- theApp.m_iFramePoint[triggerPoint-1]);

			}
			else if(theApp.m_iAnimationInsideBlueMultFunction[triggerPoint] == 1)
			{// Positive Quadratic Inside Blue Multiplier Function

				double a = (theApp.m_dAnimationInsideBlueMult[triggerPoint]
							- theApp.m_dAnimationInsideBlueMult[triggerPoint-1])
							/ pow((double)(theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1]),2);

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dInBlueMult = a*counter*counter 
										+ theApp.m_dAnimationInsideBlueMult[triggerPoint-1];

			}
			else if(theApp.m_iAnimationInsideBlueMultFunction[triggerPoint] == 2)
			{// Negative Quadratic Inside Blue Multiplier Function

				double b = (theApp.m_dAnimationInsideBlueMult[triggerPoint]
							- theApp.m_dAnimationInsideBlueMult[triggerPoint-1])
							/ ((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])/2.0);


				double a = -b/((theApp.m_iFramePoint[triggerPoint]
							- theApp.m_iFramePoint[triggerPoint-1])*2.0);

				double counter = (double)(theApp.m_iFrameCount-theApp.m_iFramePoint[triggerPoint-1]);

				theApp.m_dInBlueMult = a*counter*counter + b*counter
										+ theApp.m_dAnimationInsideBlueMult[triggerPoint-1];

			}

		}


		UpdateMainTabData();


		OnAnimationRender();


		int iPercent = (int)(100*theApp.m_iFrameCount/theApp.m_iTotalFrame);
		CString sPercent;
		sPercent.Format(_T("%d"), iPercent);
		theApp.pStatic16002.SetWindowTextW(sPercent);

		temp = clock();
		theApp.m_dRenderTime = Round(((double)(temp - start) / CLOCKS_PER_SEC), 3);
		sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
		sRenderTime = RemoveZeros(sRenderTime);
		theApp.pStatic16003.SetWindowTextW(sRenderTime);

		m_iRemainingTime = Round(theApp.m_dRenderTime
							/ theApp.m_iFrameCount*theApp.m_iTotalFrame - theApp.m_dRenderTime,3);
		CString sRemainingTime;
		sRemainingTime.Format(_T("%lf"), m_iRemainingTime);
		sRemainingTime = RemoveZeros(sRemainingTime);
		theApp.pStatic16004.SetWindowTextW(sRemainingTime);


		CString fname(_itoa(theApp.m_iFrameCount, fileName, 10));

		if(theApp.m_iFrameCount < 10)
		{
			fname = TEXT("00000") + fname + TEXT(".bmp");
		}
		else if(theApp.m_iFrameCount < 100)
		{
			fname = TEXT("0000") + fname + TEXT(".bmp");
		}
		else if(theApp.m_iFrameCount < 1000)
		{
			fname = TEXT("000") + fname + TEXT(".bmp");
		}
		else if(theApp.m_iFrameCount < 10000)
		{
			fname = TEXT("00") + fname + TEXT(".bmp");
		}
		else if(theApp.m_iFrameCount < 100000)
		{
			fname = TEXT("0") + fname + TEXT(".bmp");
		}
		else if(theApp.m_iFrameCount < 1000000)
		{
			fname = fname + TEXT(".bmp");
		}

		WndToBmp(pDC,fname);


		UpdateMainTabData();

	}

	ReleaseDC(hWnd,hDC);

	finish = clock();
	theApp.m_dRenderTime = Round(((double)(finish - start) / CLOCKS_PER_SEC), 3);
	sRenderTime.Format(_T("%lf"), theApp.m_dRenderTime);
	sRenderTime = RemoveZeros(sRenderTime);
	theApp.pStatic16003.SetWindowTextW(sRenderTime);

	theApp.pStatic16004.SetWindowTextW(_T("0"));


	UpdateMainTabData();


	theApp.pProgress.SetMarquee(FALSE,0);
	theApp.pProgress.Invalidate(TRUE);

	theApp.pThread = NULL;

	return 0;
}

void OnAnimationRender()
{
	int iFractalType = theApp.pComboBox10003->GetCurSel();

	if(iFractalType == 0)
	{
		OnAnimationMandelbrot();
	}
	else if(iFractalType == 1)
	{
		OnAnimationMandelbrotQuick();
	}
	else if(iFractalType == 2)
	{
		OnAnimationJulia();
	}
	else if(iFractalType == 3)
	{
		OnAnimationJuliaQuick();
	}
	else if(iFractalType == 4)
	{
		OnAnimationNewton();
	}
	else if(iFractalType == 5)
	{
		OnAnimationNewtonRoot();
	}
	else if(iFractalType == 6)
	{
		OnAnimationNova();
	}
	else if(iFractalType == 7)
	{
		OnAnimationNovaSpecial();
	}
	else if(iFractalType == 8)
	{
		OnAnimationGlynn();
	}
	else if(iFractalType == 9)
	{
		OnAnimationPhoenix();
	}
	else if(iFractalType == 10)
	{
		OnAnimationCosTanZ();
	}

}

void OnAnimationMandelbrot()
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double a, b, t, tBU = 0, x, y, power;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 900;
	int height = 600;
	int xOffset = 3, yOffset = 3;



	a = theApp.m_dControl1;
	b = theApp.m_dControl2;
	power = theApp.m_dControl3;

	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
	yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;


	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
		y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
		
		hx = 0.0;

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(a,b);

			for (t = 0; t < theApp.m_iIteration; t++)
			{
				complex<double> c(x,y);

				z = pow(z, power) + c;

				bailout = abs(z);

				if(bailout > theApp.m_dBailout)
				{
					tBU = t;
					t = theApp.m_iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(theApp.m_dOutRedMult * bailout);
				iOutGreen = (int)(theApp.m_dOutGreenMult * bailout);
				iOutBlue = (int)(theApp.m_dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+1 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}


			if(bailout < theApp.m_dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + theApp.m_iInRedAdder,
								iInGreen + theApp.m_iInGreenAdder,
								iInBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + theApp.m_iOutRedAdder,
								iOutGreen + theApp.m_iOutGreenAdder,
								iOutBlue + theApp.m_iOutBlueAdder));
			}

			hx = (6.0/(double)width)/theApp.m_dZoomRatio;

			x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
			y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);

		}

		hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
	}

}

void OnAnimationMandelbrotQuick()
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double a, b, d, q, t, tBU = 0, x, y;
	double aStart, bStart;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	int width = 900;
	int height = 600;
	int xOffset = 3, yOffset = 3;



	aStart = theApp.m_dControl1;
	bStart = theApp.m_dControl2;

	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
	yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;


	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
		y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
		
		hx = 0.0;

		for(xPix = 0; xPix < width; xPix++)
		{
			a = aStart;
			b = bStart;

			for (t = 0; t < theApp.m_iIteration; t++)
			{
				q = a;
		  		a = (a * a) - (b * b) + x;
				b = 2 * (q * b) + y;

				d = (a * a) + (b * b);

				bailout = (sqrt(d));

				if(bailout > theApp.m_dBailout)
				{
					tBU = t;
					t = theApp.m_iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(theApp.m_dOutRedMult * bailout);
				iOutGreen = (int)(theApp.m_dOutGreenMult * bailout);
				iOutBlue = (int)(theApp.m_dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+1 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}


			if(bailout < theApp.m_dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + theApp.m_iInRedAdder,
								iInGreen + theApp.m_iInGreenAdder,
								iInBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + theApp.m_iOutRedAdder,
								iOutGreen + theApp.m_iOutGreenAdder,
								iOutBlue + theApp.m_iOutBlueAdder));
			}

			hx = (6.0/(double)width)/theApp.m_dZoomRatio;

			x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
			y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
	}

}

void OnAnimationJulia()
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double a, b, t, tBU = 0, x, y, power;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 900;
	int height = 600;
	int xOffset = 3, yOffset = 3;



	a = theApp.m_dControl1;
	b = theApp.m_dControl2;
	power = theApp.m_dControl3;

	complex<double> c(a,b);

	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
	yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;


	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
		y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
		
		hx = 0.0;

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			for (t = 0; t < theApp.m_iIteration; t++)
			{
				z = pow(z, power) + c;

				bailout = abs(z);

				if(bailout > theApp.m_dBailout)
				{
					tBU = t;
					t = theApp.m_iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(theApp.m_dOutRedMult * bailout);
				iOutGreen = (int)(theApp.m_dOutGreenMult * bailout);
				iOutBlue = (int)(theApp.m_dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+2 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}


			if(bailout < theApp.m_dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + theApp.m_iInRedAdder,
								iInGreen + theApp.m_iInGreenAdder,
								iInBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + theApp.m_iOutRedAdder,
								iOutGreen + theApp.m_iOutGreenAdder,
								iOutBlue + theApp.m_iOutBlueAdder));
			}

			hx = (6.0/(double)width)/theApp.m_dZoomRatio;

			x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
			y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
	}

}

void OnAnimationJuliaQuick()
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double a, b, d, e, f, q, t, tBU = 0, x, y;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	int width = 900;
	int height = 600;
	int xOffset = 3, yOffset = 3;



	a = theApp.m_dControl1;
	b = theApp.m_dControl2;

	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
	yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;


	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
		y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
		
		hx = 0.0;

		for(xPix = 0; xPix < width; xPix++)
		{
			e = x;
			f = y;

			for (t = 0; t < theApp.m_iIteration; t++)
			{
				q = e;
	  			e = (e * e) - (f * f) + a;
					
				f = 2 * (q * f) + b;

				d = (e * e) + (f * f);

				bailout = (sqrt(d));

				if(bailout > theApp.m_dBailout)
				{
					tBU = t;
					t = theApp.m_iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(theApp.m_dOutRedMult * bailout);
				iOutGreen = (int)(theApp.m_dOutGreenMult * bailout);
				iOutBlue = (int)(theApp.m_dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+2 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}


			if(bailout < theApp.m_dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + theApp.m_iInRedAdder,
								iInGreen + theApp.m_iInGreenAdder,
								iInBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + theApp.m_iOutRedAdder,
								iOutGreen + theApp.m_iOutGreenAdder,
								iOutBlue + theApp.m_iOutBlueAdder));
			}

			hx = (6.0/(double)width)/theApp.m_dZoomRatio;

			x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
			y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
	}

}

void OnAnimationNewton()
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double t, x, y;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 900;
	int height = 600;
	int xOffset = 3, yOffset = 3;



	double dRelaxReal = theApp.m_dControl1;
	double dRelaxImag = theApp.m_dControl2;
	double power = theApp.m_dControl3;

	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
	yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;

	z = complex<double>(xStart,yStart);

	complex<double> zGreen = complex<double>(-0.5, sin(2*3.1416/3));
	complex<double> zBlue = complex<double>(-0.5, -sin(2*3.1416/3));

	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
		y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
		
		hx = 0.0;

		z = complex<double>(x,y);

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			for(t = 0; t < theApp.m_iIteration; t++)
			{
				z = z - complex<double>(dRelaxReal, dRelaxImag)
					* (pow(z,power)-1.0)/((double)power*pow(z,power-1.0));

				bailout = abs(z);

				if(bailout > theApp.m_dBailout)
				{
					t = theApp.m_iIteration;
				}
			}

			iOutRed = (int)(theApp.m_dOutRedMult * bailout);
			iOutGreen = (int)(theApp.m_dOutGreenMult * bailout);
			iOutBlue = (int)(theApp.m_dOutBlueMult * bailout);

			if(iOutRed > 255){iOutRed = 255;}
			if(iOutGreen > 255){iOutGreen = 255;}
			if(iOutBlue > 255){iOutBlue = 255;}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}

			if(bailout < theApp.m_dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + theApp.m_iInRedAdder,
								iInGreen + theApp.m_iInGreenAdder,
								iInBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + theApp.m_iOutRedAdder,
								iOutGreen + theApp.m_iOutGreenAdder,
								iOutBlue + theApp.m_iOutBlueAdder));
			}

			hx = (6.0/(double)width)/theApp.m_dZoomRatio;

			x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
			y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
	}

}

void OnAnimationNewtonRoot()
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double t, x, y;

	int xPix, yPix;
	double bailout = 4.0;

	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 900;
	int height = 600;
	int xOffset = 3, yOffset = 3;



	double dRelaxReal = theApp.m_dControl1;
	double dRelaxImag = theApp.m_dControl2;
	double tol = theApp.m_dControl3;

	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
	yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;

	z = complex<double>(xStart,yStart);

	complex<double> zGreen = complex<double>(-0.5, sin(2*3.1416/3));
	complex<double> zBlue = complex<double>(-0.5, -sin(2*3.1416/3));

	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
		y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
		
		hx = 0.0;

		z = complex<double>(x,y);

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			t = 0;

			while((t < theApp.m_iIteration) && (abs(z-1.0) >= tol) && (abs(z-zGreen) >= tol) && (abs(z-zBlue) >= tol))
			{
				z = z - complex<double>(dRelaxReal,dRelaxImag) * (z*z*z-1.0) / ((double)3.0*z*z);

				bailout = abs(z);

				t++;
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}

			if(abs(z - 1.0) < tol)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(t*theApp.m_iFirstRootRed + theApp.m_iInRedAdder,
								t*theApp.m_iFirstRootGreen + theApp.m_iInGreenAdder,
								t*theApp.m_iFirstRootBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				iInRed = 0;
			}

			if(abs(z - zGreen) <= tol)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(t*theApp.m_iSecondRootRed + theApp.m_iInRedAdder,
							t*theApp.m_iSecondRootGreen + theApp.m_iInGreenAdder,
							t*theApp.m_iSecondRootBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				iInGreen = 0;
			}

			if(abs(z - zBlue) <= tol)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(t*theApp.m_iThirdRootRed + theApp.m_iInRedAdder,
							t*theApp.m_iThirdRootGreen + theApp.m_iInGreenAdder,
							t*theApp.m_iThirdRootBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				iInBlue = 0;
			}


			hx = (6.0/(double)width)/theApp.m_dZoomRatio;

			x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
			y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
	}

}

void OnAnimationNova()
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double t, tBU = 0, x, y;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 900;
	int height = 600;
	int xOffset = 3, yOffset = 3;



	double dSeeda = theApp.m_dControl1;
	double dSeedB = theApp.m_dControl2;

	double dRelaxReal = theApp.m_dControl3;
	double dRelaxImag = theApp.m_dControl4;

	double power = theApp.m_dControl5;

	double dDerivMult = theApp.m_dControl6;
	
	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
	yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;

	z = complex<double>(xStart,yStart);

	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
		y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
		
		hx = 0.0;

		z = complex<double>(x,y);

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			for(t = 0; t < theApp.m_iIteration; t++)
			{
				z = z - complex<double>(dRelaxReal, dRelaxImag)
						* (pow(z,power)-1.0)/(dDerivMult*power*pow(z,power-1.0))
						+ complex<double>(dSeeda, dSeedB);

				bailout = abs(z);

				if(bailout > theApp.m_dBailout)
				{
					tBU = t;
					t = theApp.m_iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(theApp.m_dOutRedMult * bailout);
				iOutGreen = (int)(theApp.m_dOutGreenMult * bailout);
				iOutBlue = (int)(theApp.m_dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+1 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}

			if(bailout < theApp.m_dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + theApp.m_iInRedAdder,
								iInGreen + theApp.m_iInGreenAdder,
								iInBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + theApp.m_iOutRedAdder,
								iOutGreen + theApp.m_iOutGreenAdder,
								iOutBlue + theApp.m_iOutBlueAdder));
			}

			hx = (6.0/(double)width)/theApp.m_dZoomRatio;

			x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
			y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
	}

}

void OnAnimationNovaSpecial()
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double t, tBU = 0, x, y;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 900;
	int height = 600;
	int xOffset = 3, yOffset = 3;



	double dRelaxReal = theApp.m_dControl1;
	double dRelaxImag = theApp.m_dControl2;

	double power = theApp.m_dControl3;

	double dDerivMult = theApp.m_dControl4;
	
	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
	yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;

	z = complex<double>(xStart,yStart);

	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
		y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
		
		hx = 0.0;

		z = complex<double>(x,y);

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			for(t = 0; t < theApp.m_iIteration; t++)
			{
				z = z - complex<double>(dRelaxReal, dRelaxImag)
						* (pow(z,power)-1.0)/(dDerivMult*power*pow(z,power-1.0))
						+ complex<double>(x,y);

				bailout = abs(z);

				if(bailout > theApp.m_dBailout)
				{
					tBU = t;
					t = theApp.m_iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(theApp.m_dOutRedMult * bailout);
				iOutGreen = (int)(theApp.m_dOutGreenMult * bailout);
				iOutBlue = (int)(theApp.m_dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+1 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}

			if(bailout < theApp.m_dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + theApp.m_iInRedAdder,
								iInGreen + theApp.m_iInGreenAdder,
								iInBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + theApp.m_iOutRedAdder,
								iOutGreen + theApp.m_iOutGreenAdder,
								iOutBlue + theApp.m_iOutBlueAdder));
			}

			hx = (6.0/(double)width)/theApp.m_dZoomRatio;

			x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
			y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
	}

}

void OnAnimationGlynn()
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double a, b, t, tBU = 0, x, y, power;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 900;
	int height = 600;
	int xOffset = 3, yOffset = 3;



	a = theApp.m_dControl1;
	b = theApp.m_dControl2;
	power = theApp.m_dControl3;

	complex<double> c(a,b);

	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
	yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;


	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
		y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
		
		hx = 0.0;

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			for (t = 0; t < theApp.m_iIteration; t++)
			{
				z = pow(z, power) + c;

				bailout = abs(z);

				if(bailout > theApp.m_dBailout)
				{
					tBU = t;
					t = theApp.m_iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(theApp.m_dOutRedMult * bailout);
				iOutGreen = (int)(theApp.m_dOutGreenMult * bailout);
				iOutBlue = (int)(theApp.m_dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+1 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+1 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+1 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}


			if(bailout < theApp.m_dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + theApp.m_iInRedAdder,
								iInGreen + theApp.m_iInGreenAdder,
								iInBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + theApp.m_iOutRedAdder,
								iOutGreen + theApp.m_iOutGreenAdder,
								iOutBlue + theApp.m_iOutBlueAdder));
			}

			hx = (6.0/(double)width)/theApp.m_dZoomRatio;

			x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
			y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
	}

}

void OnAnimationPhoenix()
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double t, tBU = 0, x, y, power;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z, zBU, zBU2;

	int width = 900;
	int height = 600;
	int xOffset = 3, yOffset = 3;



	complex<double> p1(theApp.m_dControl1, theApp.m_dControl2);
	complex<double> p2(theApp.m_dControl3, theApp.m_dControl4);

	power = theApp.m_dControl5;

	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
	yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;


	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
		y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
		
		hx = 0.0;

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			for (t = 0; t < theApp.m_iIteration; t++)
			{
				if(t == 0)
				{
					zBU = z;
					zBU2 = complex<double>(0.0,0.0);
				}
				else
				{
					zBU2 = zBU;
					zBU = z;
				}

				z = pow(z, power) + p1 + real(p2)*zBU2 + imag(p2)*zBU2;

				bailout = abs(z);

				if(bailout > theApp.m_dBailout)
				{
					tBU = t;
					t = theApp.m_iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(theApp.m_dOutRedMult * bailout);
				iOutGreen = (int)(theApp.m_dOutGreenMult * bailout);
				iOutBlue = (int)(theApp.m_dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+2 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}


			if(bailout < theApp.m_dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + theApp.m_iInRedAdder,
								iInGreen + theApp.m_iInGreenAdder,
								iInBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + theApp.m_iOutRedAdder,
								iOutGreen + theApp.m_iOutGreenAdder,
								iOutBlue + theApp.m_iOutBlueAdder));
			}

			hx = (6.0/(double)width)/theApp.m_dZoomRatio;

			x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
			y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
	}

}

void OnAnimationCosTanZ()
{
	CWnd* pWnd = CWnd::GetSafeOwner(0,0);
	CClientDC dc(pWnd);

	double a, b, t, tBU = 0, x, y, power;

	int xPix, yPix;
	double bailout = 4.0;

	int iOutRed, iOutGreen, iOutBlue;
	int iInRed, iInGreen, iInBlue;

	double xStart, yStart;
	double hyp, hx, hy = 0.0;
	double theta;

	complex<double> z;

	int width = 900;
	int height = 600;
	int xOffset = 3, yOffset = 3;



	a = theApp.m_dControl1;
	b = theApp.m_dControl2;
	power = theApp.m_dControl3;

	complex<double> c(a,b);

	hyp = sqrt(pow(3.0,2) + pow(2.0,2));
	theta = atan(2.0/3.0);

	xStart = theApp.m_dXAxis - hyp * cos(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;
	yStart = theApp.m_dYAxis + hyp * sin(theta + (theApp.m_dRotation*DEGtoRAD))/theApp.m_dZoomRatio;


	for(yPix = 0; yPix < height; yPix++)
	{
		x = xStart - hy*sin(theApp.m_dRotation*DEGtoRAD);
		y = yStart - hy*cos(theApp.m_dRotation*DEGtoRAD);
		
		hx = 0.0;

		for(xPix = 0; xPix < width; xPix++)
		{
			z = complex<double>(x,y);

			for (t = 0; t < theApp.m_iIteration; t++)
			{
				z = cos(tan(z)) + c;

				bailout = abs(z);

				if(bailout > theApp.m_dBailout)
				{
					tBU = t;
					t = theApp.m_iIteration;
				}
			}

			if(theApp.m_iColoringAlgorithmCurSel == 0)
			{
				iOutRed = (int)(theApp.m_dOutRedMult * bailout);
				iOutGreen = (int)(theApp.m_dOutGreenMult * bailout);
				iOutBlue = (int)(theApp.m_dOutBlueMult * bailout);

				if(iOutRed > 255){iOutRed = 255;}
				if(iOutGreen > 255){iOutGreen = 255;}
				if(iOutBlue > 255){iOutBlue = 255;}
			}
			else if(theApp.m_iColoringAlgorithmCurSel == 1)
			{
				if(theApp.m_i2ColorGradientCurSel == 0)
				{
					// Black to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU >= 256)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 1)
				{
					// Red to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 2)
				{
					// Green to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 3)
				{
					// Blue to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 4)
				{
					// Yellow to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 5)
				{
					// Purple to White
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 6)
				{
					// Cyan to White
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 7)
				{
					// Red to Yellow
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 8)
				{
					// Red to Purple
					if(tBU < 64)
					{
						iOutRed = (int)(tBU*2);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = (int)(((tBU-62)*128)/126);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutGreen = 0;
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255;
						iOutGreen = 0;
						iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 9)
				{
					// Green to Yellow
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = int(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 255;
						iOutBlue = 0;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 255; iOutBlue = 0;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 10)
				{
					// Green to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = (int)(tBU*2);
						iOutBlue = 0;
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = (int)(((tBU-62)*128)/126);
						iOutBlue = 0;
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 128) * 62) / 127) + 193);
						iOutBlue = 0;
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 256) * 62) / 255) + 1);
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 512) * 63) / 511) + 64);
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 1024) * 63) / 1023) + 128);
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = 255;
						iOutBlue = (int)((((tBU - 2048) * 63) / 2047) + 192);
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 11)
				{
					// Blue to Purple
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutGreen = 0;
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 255; iOutGreen = 0; iOutBlue = 255;
					}
				}
				else if(theApp.m_i2ColorGradientCurSel == 12)
				{
					// Blue to Cyan
					if(tBU < 64)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(tBU*2);
					}
					else if(tBU < 128)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)(((tBU-62)*128)/126);
					}
					else if(tBU < 256)
					{
						iOutRed = 0;
						iOutGreen = 0;
						iOutBlue = (int)((((tBU - 128) * 62) / 127) + 193);
					}
					else if(tBU < 512)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 256) * 62) / 255) + 1);
						iOutBlue = 255;
					}
					else if(tBU < 1024)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 512) * 63) / 511) + 64);
						iOutBlue = 255;
					}
					else if(tBU < 2048)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 1024) * 63) / 1023) + 128);
						iOutBlue = 255;
					}
					else if(tBU < 4096)
					{
						iOutRed = 0;
						iOutGreen = (int)((((tBU - 2048) * 63) / 2047) + 192);
						iOutBlue = 255;
					}
					else if(tBU >= 4096)
					{
						iOutRed = 0; iOutGreen = 255; iOutBlue = 255;
					}
				}

			}
			else if(theApp.m_iColoringAlgorithmCurSel == 2)
			{
				if(theApp.m_iSmoothGradientCurSel == 0)
				{// Smooth Coloring Function

					iOutRed = (int)(theApp.m_dSmoothRedValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutGreen = (int)(theApp.m_dSmoothGreenValue*(tBU+2 - log(log(bailout))/log(2.0)));
					iOutBlue = (int)(theApp.m_dSmoothBlueValue*(tBU+2 - log(log(bailout))/log(2.0)));
				}
				else if(theApp.m_iSmoothGradientCurSel == 1)
				{
					if(theApp.m_bMainColoringRed == TRUE)
					{
						iOutRed = (int)(100 - theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutRed = (int)(theApp.m_dSmoothRedValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringGreen == TRUE)
					{
						iOutGreen = (int)(100 - theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutGreen = (int)(theApp.m_dSmoothGreenValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}

					if(theApp.m_bMainColoringBlue == TRUE)
					{
						iOutBlue = (int)(100 - theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
					else
					{
						iOutBlue = (int)(theApp.m_dSmoothBlueValue
										* (tBU+2 - log(log(bailout))/log(2.0)));
					}
				}
			}


			iInRed = (int)(theApp.m_dInRedMult * bailout);
			iInGreen = (int)(theApp.m_dInGreenMult * bailout);
			iInBlue = (int)(theApp.m_dInBlueMult * bailout);

			if(iInRed > 255){iInRed = 255;}
			if(iInGreen > 255){iInGreen = 255;}
			if(iInBlue > 255){iInBlue = 255;}


			if(bailout < theApp.m_dBailout)
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iInRed + theApp.m_iInRedAdder,
								iInGreen + theApp.m_iInGreenAdder,
								iInBlue + theApp.m_iInBlueAdder));
			}
			else
			{
				dc.SetPixel(xOffset+xPix,
							yOffset+yPix,
							RGB(iOutRed + theApp.m_iOutRedAdder,
								iOutGreen + theApp.m_iOutGreenAdder,
								iOutBlue + theApp.m_iOutBlueAdder));
			}

			hx = (6.0/(double)width)/theApp.m_dZoomRatio;

			x = x + hx*cos(theApp.m_dRotation*DEGtoRAD);
			y = y - hx*sin(theApp.m_dRotation*DEGtoRAD);
		}

		hy = hy + (4.0/(double)height)/theApp.m_dZoomRatio;
	}

}

void UpdateMainTabData()
{
	// Geometry Tab Controls

	CString sXAxisStr;
	sXAxisStr.Format(_T("%lf"), theApp.m_dXAxis);
	sXAxisStr = RemoveZeros(sXAxisStr);
	theApp.pEdit10005.SetWindowText(sXAxisStr);

	CString sYAxisStr;
	sYAxisStr.Format(_T("%lf"), theApp.m_dYAxis);
	sYAxisStr = RemoveZeros(sYAxisStr);
	theApp.pEdit10007.SetWindowText(sYAxisStr);

	CString sZoomRatioStr;
	sZoomRatioStr.Format(_T("%lf"), theApp.m_dZoomRatio);
	sZoomRatioStr = RemoveZeros(sZoomRatioStr);
	theApp.pEdit10009.SetWindowText(sZoomRatioStr);

	CString sRotationStr;
	sRotationStr.Format(_T("%lf"), theApp.m_dRotation);
	sRotationStr = RemoveZeros(sRotationStr);
	theApp.pEdit10011.SetWindowText(sRotationStr);

	CString sIterationStr;
	sIterationStr.Format(_T("%d"), theApp.m_iIteration);
	theApp.pEdit10013.SetWindowText(sIterationStr);


	// Formula Tab Controls

	CString sControl1Str;
	sControl1Str.Format(_T("%lf"), theApp.m_dControl1);
	sControl1Str = RemoveZeros(sControl1Str);
	theApp.pEdit11003.SetWindowText(sControl1Str);

	CString sControl2Str;
	sControl2Str.Format(_T("%lf"), theApp.m_dControl2);
	sControl2Str = RemoveZeros(sControl2Str);
	theApp.pEdit11005.SetWindowText(sControl2Str);

	CString sControl3Str;
	sControl3Str.Format(_T("%lf"), theApp.m_dControl3);
	sControl3Str = RemoveZeros(sControl3Str);
	theApp.pEdit11007.SetWindowText(sControl3Str);

	CString sControl4Str;
	sControl4Str.Format(_T("%lf"), theApp.m_dControl4);
	sControl4Str = RemoveZeros(sControl4Str);
	theApp.pEdit11009.SetWindowText(sControl4Str);

	CString sControl5Str;
	sControl5Str.Format(_T("%lf"), theApp.m_dControl5);
	sControl5Str = RemoveZeros(sControl5Str);
	theApp.pEdit11011.SetWindowText(sControl5Str);

	CString sControl6Str;
	sControl6Str.Format(_T("%lf"), theApp.m_dControl6);
	sControl6Str = RemoveZeros(sControl6Str);
	theApp.pEdit11013.SetWindowText(sControl6Str);


	// Outaide Coloring Tab Controls

	CString sOutRedAdder;
	sOutRedAdder.Format(_T("%d"), theApp.m_iOutRedAdder);
	theApp.pEdit12003->SetWindowText(sOutRedAdder);

	CString sOutGreenAdder;
	sOutGreenAdder.Format(_T("%d"), theApp.m_iOutGreenAdder);
	theApp.pEdit12005->SetWindowText(sOutGreenAdder);

	CString sOutBlueAdder;
	sOutBlueAdder.Format(_T("%d"), theApp.m_iOutBlueAdder);
	theApp.pEdit12007->SetWindowText(sOutBlueAdder);

	CString sOutRedMult;
	sOutRedMult.Format(_T("%lf"), theApp.m_dOutRedMult);
	sOutRedMult = RemoveZeros(sOutRedMult);
	theApp.pEdit12009->SetWindowText(sOutRedMult);

	CString sOutGreenMult;
	sOutGreenMult.Format(_T("%lf"), theApp.m_dOutGreenMult);
	sOutGreenMult = RemoveZeros(sOutGreenMult);
	theApp.pEdit12011->SetWindowText(sOutGreenMult);

	CString sOutBlueMult;
	sOutBlueMult.Format(_T("%lf"), theApp.m_dOutBlueMult);
	sOutBlueMult = RemoveZeros(sOutBlueMult);
	theApp.pEdit12013->SetWindowText(sOutBlueMult);


	// Inaide Coloring Tab Controls

	CString sInRedAdder;
	sInRedAdder.Format(_T("%d"), theApp.m_iInRedAdder);
	theApp.pEdit13003->SetWindowText(sInRedAdder);

	CString sInGreenAdder;
	sInGreenAdder.Format(_T("%d"), theApp.m_iInGreenAdder);
	theApp.pEdit13005->SetWindowText(sInGreenAdder);

	CString sInBlueAdder;
	sInBlueAdder.Format(_T("%d"), theApp.m_iInBlueAdder);
	theApp.pEdit13007->SetWindowText(sInBlueAdder);

	CString sInRedMult;
	sInRedMult.Format(_T("%lf"), theApp.m_dInRedMult);
	sInRedMult = RemoveZeros(sInRedMult);
	theApp.pEdit13009->SetWindowText(sInRedMult);

	CString sInGreenMult;
	sInGreenMult.Format(_T("%lf"), theApp.m_dInGreenMult);
	sInGreenMult = RemoveZeros(sInGreenMult);
	theApp.pEdit13011->SetWindowText(sInGreenMult);

	CString sInBlueMult;
	sInBlueMult.Format(_T("%lf"), theApp.m_dInBlueMult);
	sInBlueMult = RemoveZeros(sInBlueMult);
	theApp.pEdit13013->SetWindowText(sInBlueMult);

}
