#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "CBolt7817Dlg.h"
#include "KURSACHDoc.h"

IMPLEMENT_DYNAMIC(CBolt7817Dlg, CDialogEx)

BEGIN_MESSAGE_MAP(CBolt7817Dlg, CDialogEx)
END_MESSAGE_MAP()

CBolt7817Dlg::CBolt7817Dlg(CKURSACHDoc* pDoc, CWnd* pParent)
    : CDialogEx(IDD_BOLT_7817_PARAMS, pParent), m_pDoc(pDoc)
{
}

void CBolt7817Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

static void SetRO(CWnd* dlg, int idc, const CString& val)
{
    dlg->SetDlgItemText(idc, val);
    CWnd* w = dlg->GetDlgItem(idc);
    if (w) ((CEdit*)w)->SetReadOnly(TRUE);
}

BOOL CBolt7817Dlg::OnInitDialog()
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

    { CString __v; __v.Format(L"%.2f", get(16)); SetRO(this, IDC_EDIT_B7817_D2, __v); }
    { CString __v; __v.Format(L"%.2f", get(19)); SetRO(this, IDC_EDIT_B7817_L1, __v); }
    { CString __v; __v.Format(L"%.2f", get(4)); SetRO(this, IDC_EDIT_B7817_K, __v); }
    { CString __v; __v.Format(L"%.2f", get(3)); SetRO(this, IDC_EDIT_B7817_S, __v); }

    return TRUE;
}
