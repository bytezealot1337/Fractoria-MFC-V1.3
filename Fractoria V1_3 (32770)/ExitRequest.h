#pragma once


// CExitRequest dialog

class CExitRequest : public CDialog
{
	DECLARE_DYNAMIC(CExitRequest)

public:
	CExitRequest(CWnd* pParent = NULL);   // standard constructor
	virtual ~CExitRequest();

// Dialog Data
	enum { IDD = IDD_EXITREQUEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
