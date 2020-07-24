// ExitRequest.cpp : implementation file
//

#include "stdafx.h"
#include "Fractoria.h"
#include "ExitRequest.h"


// CExitRequest dialog

IMPLEMENT_DYNAMIC(CExitRequest, CDialog)

CExitRequest::CExitRequest(CWnd* pParent /*=NULL*/)
	: CDialog(CExitRequest::IDD, pParent)
{

}

CExitRequest::~CExitRequest()
{
}

void CExitRequest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CExitRequest, CDialog)
END_MESSAGE_MAP()


// CExitRequest message handlers
