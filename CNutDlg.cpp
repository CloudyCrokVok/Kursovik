#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "CNutDlg.h"
#include "KURSACHDoc.h"

#include <cmath>

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

    // Для гайки (ГОСТ 15522) по заданию используем:
    //  - d2: 9 или 11 (соответствует болту ГОСТ 7796)
    //  - m: 4/5/6 (высота гайки)
        const double hole_d2 = get(14);
    auto GetNominalThreadD = [](double d2)->int {
        if (d2 <= 9.0) return 8;
        if (d2 <= 11.5) return 10;
        if (d2 <= 14.0) return 12;
        return (int)floor(d2 + 0.5);
    };
    const int d_nom = GetNominalThreadD(hole_d2);

    // По фото4: высота гайки m: 4/5/6
    auto GetM = [](int d)->double {
        switch (d)
        {
        case 8:  return 4.0;
        case 10: return 5.0;
        case 12: return 6.0;
        default: return 0.0;
        }
    };

    // По фото4: размер под ключ S: 12/14/17
    auto GetS = [](int d)->int {
        switch (d)
        {
        case 8:  return 12;
        case 10: return 14;
        case 12: return 17;
        default: return 0;
        }
    };

    // d2 для гайки (под болт 7796): 9 или 11 (а НЕ 8.4/10.5)
    auto GetNutD2 = [](int d_nom)->int {
        switch (d_nom)
        {
        case 8:  return 9;
        case 10: return 11;
        case 12: return 13;
        default: return d_nom;
        }
    };

    // d2 (для гайки показываем 9/11 по ГОСТ 7796)
    { CString __v; __v.Format(L"%d", GetNutD2(d_nom)); SetRO(this, IDC_EDIT_NUT_D2, __v); }

    // m — высота гайки (4/5/6)
    { CString __v; __v.Format(L"%.1f", GetM(d_nom)); SetRO(this, IDC_EDIT_NUT_M, __v); }
        // S — размер под ключ (12/14/17)
    { CString __v; __v.Format(L"%d", GetS(d_nom)); SetRO(this, IDC_EDIT_NUT_S, __v); }

    // Сохраняем параметры гайки, показанные в editbox-ах, в документ
    {
        CKURSACHDoc::NutUIParams p;
        p.nominal = d_nom;
        p.m = GetM(d_nom);
        p.S = GetS(d_nom);
        m_pDoc->SetNut15521Params(p);
    }

    return TRUE;
}
