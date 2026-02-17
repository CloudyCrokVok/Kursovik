#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "CBolt7817Dlg.h"
#include "KURSACHDoc.h"

#include <cmath>
#include <vector>

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

    // Достаём параметры из таблицы полумуфты
    const double hole_d2 = get(14);           // d2 (диаметр отверстия под болт)
    const double l1_half = get(19);           // l1 (полумуфта)
    const int n_stack = (int)(get(15) + 0.5); // n (по заданию используется в формуле длины)

    // Определяем номинальный диаметр резьбы по диаметру отверстия (d2).
    auto GetNominalThreadD = [](double d2)->int {
        if (d2 <= 9.0) return 8;
        if (d2 <= 11.5) return 10;
        if (d2 <= 14.0) return 12;
        return (int)floor(d2 + 0.5);
    };
    const int d_nom = GetNominalThreadD(hole_d2);

    // Высота гайки m (по фото4)
    auto GetNutM = [](int d)->double {
        switch (d)
        {
        case 8:  return 4.0;
        case 10: return 5.0;
        case 12: return 6.0;
        default: return 0.0;
        }
    };
    const double nut_m = GetNutM(d_nom);

    // Длина болта по заданию: Lreq = 2*l1 + n + m
    const double Lreq = 2.0 * l1_half + (double)n_stack + nut_m;

    // Подбор стандартной длины l по ГОСТ 7817 (см. фото6): берём минимальную l >= Lreq.
    auto PickStdLen7817 = [](int /*d*/, double req)->int {
        static const int Ls[] = { 18,20,22,25,28,30,32,35,38,40,45,48,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,130,140,150,160,170,180 };
        for (int L : Ls) if (L >= req - 1e-9) return L;
        return Ls[sizeof(Ls) / sizeof(Ls[0]) - 1];
    };
    const int bolt_l = PickStdLen7817(d_nom, Lreq);

    auto GetK = [](int d)->double {
        switch (d)
        {
        case 8:  return 5.3;
        case 10: return 6.4;
        case 12: return 7.5;
        default: return 0.0;
        }
    };
    auto GetS7817 = [](int d)->int {
        // По заданию: для 7817 S может быть 12 или 14
        if (d <= 8) return 12;
        return 14;
    };

    auto FormatSmart = [](double v)->CString {
        CString s;
        double iv = floor(v + 0.5);
        if (fabs(v - iv) < 1e-6) s.Format(L"%.0f", iv);
        else s.Format(L"%.1f", v);
        return s;
    };

    SetRO(this, IDC_EDIT_B7817_D2, FormatSmart(hole_d2));
    // В поле "l1" выводим итоговую длину болта l (см. п.3 и фото6)
    { CString __v; __v.Format(L"%d", bolt_l); SetRO(this, IDC_EDIT_B7817_L1, __v); }
    SetRO(this, IDC_EDIT_B7817_K, FormatSmart(GetK(d_nom)));
    { CString __v; __v.Format(L"%d", GetS7817(d_nom)); SetRO(this, IDC_EDIT_B7817_S, __v); }

    // Сохраняем параметры, которые показали в editbox-ах, в документ
    {
        CKURSACHDoc::BoltUIParams p;
        p.d2 = hole_d2;
        p.l  = bolt_l;
        p.k  = GetK(d_nom);
        p.S  = GetS7817(d_nom);
        p.nominal = d_nom;
        m_pDoc->SetBolt7817Params(p);
    }

    return TRUE;
}
