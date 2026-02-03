#pragma once
#include <afxwin.h>
#include <vector>

class CKURSACHDoc;

class CBolt7796Dlg : public CDialogEx
{
public:
    CBolt7796Dlg(CKURSACHDoc* pDoc, CWnd* pParent = nullptr);

    DECLARE_DYNAMIC(CBolt7796Dlg)
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_BOLT_7796_PARAMS };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX) override;
    virtual BOOL OnInitDialog() override;

    CKURSACHDoc* m_pDoc;

    DECLARE_MESSAGE_MAP()
};
