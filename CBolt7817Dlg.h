#pragma once
#include <afxwin.h>
#include <vector>

class CKURSACHDoc;

class CBolt7817Dlg : public CDialogEx
{
public:
    CBolt7817Dlg(CKURSACHDoc* pDoc, CWnd* pParent = nullptr);

    DECLARE_DYNAMIC(CBolt7817Dlg)
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_BOLT_7817_PARAMS };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX) override;
    virtual BOOL OnInitDialog() override;

    CKURSACHDoc* m_pDoc;

    DECLARE_MESSAGE_MAP()
};
