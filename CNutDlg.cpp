#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "CNutDlg.h"
#include "KURSACHDoc.h"

IMPLEMENT_DYNAMIC(CNutDlg, CDialogEx)

BEGIN_MESSAGE_MAP(CNutDlg, CDialogEx)
END_MESSAGE_MAP()

CNutDlg::CNutDlg(CKURSACHDoc* pDoc, CWnd* pParent)
    : CDialogEx(IDD_GAYKA_PARAMS, pParent), m_pDoc(pDoc)
{
}

void CNutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

static void SetRO(CWnd* dlg, int idc, const CString& val)
{
    dlg->SetDlgItemText(idc, val);
    CWnd* w = dlg->GetDlgItem(idc);
    if (w) ((CEdit*)w)->SetReadOnly(TRUE);
}

BOOL CNutDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    if (!m_pDoc)
        return TRUE;

    int execution = 1, variant = 1;
    m_pDoc->GetHalfCouplingParams(execution, variant);

    // variant in doc is stored as 0/1 (combo index) in this project.
    // GetHalfCouplingData expects 1/2 for variant.
    int tableVariant = variant + 1;

    std::vector<double> data = m_pDoc->GetHalfCouplingData(execution, tableVariant);

    // Safety: check size
    auto get = [&](size_t idx)->double {
        return (idx < data.size()) ? data[idx] : 0.0;
    };

    { CString __v; __v.Format(L"%.2f", get(1)); SetRO(this, IDC_EDIT_NUT_D2, __v); }
    { CString __v; __v.Format(L"%.2f", get(4)); SetRO(this, IDC_EDIT_NUT_M, __v); }
    { CString __v; __v.Format(L"%.2f", get(3)); SetRO(this, IDC_EDIT_NUT_B, __v); }

    return TRUE;
}
