#pragma once
#include <afxwin.h>
#include <vector>

class CKURSACHDoc;

class CNutDlg : public CDialogEx
{
public:
    CNutDlg(CKURSACHDoc* pDoc, CWnd* pParent = nullptr);

    DECLARE_DYNAMIC(CNutDlg)
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_GAYKA_PARAMS };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX) override;
    virtual BOOL OnInitDialog() override;

    CKURSACHDoc* m_pDoc;

    DECLARE_MESSAGE_MAP()
};
