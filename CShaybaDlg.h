#pragma once
#include <afxwin.h>
#include <vector>

class CKURSACHDoc;

class CShaybaDlg : public CDialogEx
{
public:
	CShaybaDlg(CKURSACHDoc* pDoc, CWnd* pParent = nullptr);



	DECLARE_DYNAMIC(CShaybaDlg)
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHAYBA_PARAMS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;

	CKURSACHDoc* m_pDoc = nullptr;

	CString m_d2;
	CString m_n;
	CString m_d4;

	DECLARE_MESSAGE_MAP()
};
