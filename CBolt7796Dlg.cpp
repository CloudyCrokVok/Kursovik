#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "CBolt7796Dlg.h"
#include "KURSACHDoc.h"

#include <cmath>
#include <vector>

IMPLEMENT_DYNAMIC(CBolt7796Dlg, CDialogEx)

BEGIN_MESSAGE_MAP(CBolt7796Dlg, CDialogEx)
END_MESSAGE_MAP()

CBolt7796Dlg::CBolt7796Dlg(CKURSACHDoc* pDoc, CWnd* pParent)
    : CDialogEx(IDD_BOLT_7796_PARAMS, pParent), m_pDoc(pDoc)
{
}

void CBolt7796Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

static void SetRO(CWnd* dlg, int idc, const CString& val)
{
    dlg->SetDlgItemText(idc, val);
    CWnd* w = dlg->GetDlgItem(idc);
    if (w) ((CEdit*)w)->SetReadOnly(TRUE);
}

BOOL CBolt7796Dlg::OnInitDialog()
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
    const double hole_d2 = get(14);        // d2 (диаметр отверстия под болт)
    const double l1_half = get(19);        // l1 (полумуфта)
    const int n_stack = (int)(get(15) + 0.5); // n (по заданию используется в формуле длины)

    // Определяем номинальный диаметр резьбы по диаметру отверстия (d2).
    // Примеры: 8.4 -> М8, 10.5 -> М10, 13 -> М12
    auto GetNominalThreadD = [](double d2)->int {
        if (d2 <= 9.0) return 8;
        if (d2 <= 11.5) return 10;
        if (d2 <= 14.0) return 12;
        return (int)floor(d2 + 0.5);
    };
    const int d_nom = GetNominalThreadD(hole_d2);

    // Высота гайки m (по фото4). Для нужных размеров достаточно 8/10/12.
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

    // Подбор стандартной длины l по ГОСТ 7796 (см. фото5): берём минимальную l >= Lreq.
    auto PickStdLen7796 = [](int /*d*/, double req)->int {
        static const int Ls[] = { 8,10,12,14,16,18,20,22,25,28,30,32,35,38,40,45,50,55,60,65,70,75,80 };
        for (int L : Ls) if (L >= req - 1e-9) return L;
        return Ls[sizeof(Ls) / sizeof(Ls[0]) - 1];
    };
    const int bolt_l = PickStdLen7796(d_nom, Lreq);

    // k и S: берём по типоразмеру (упрощённые значения, достаточно для корректного UI).
    auto GetK = [](int d)->double {
        switch (d)
        {
        case 8:  return 5.3;
        case 10: return 6.4;
        case 12: return 7.5;
        default: return 0.0;
        }
    };
    auto GetS7796 = [](int d)->int {
        // По заданию: для 7796 S может быть 14 или 17
        if (d <= 8) return 14;
        return 17;
    };

    // Форматирование: если есть дробная часть — показываем 1 знак.
    auto FormatSmart = [](double v)->CString {
        CString s;
        double iv = floor(v + 0.5);
        if (fabs(v - iv) < 1e-6) s.Format(L"%.0f", iv);
        else s.Format(L"%.1f", v);
        return s;
    };

    // ВАЖНО (по замечанию): d2=8.4/10.5 должно быть только у болта ГОСТ 7817.
    // Для ГОСТ 7796 в поле d2 показываем 9 или 11 (по табличным значениям).
    auto GetD2_7796 = [](int d_nom)->int {
        switch (d_nom)
        {
        case 8:  return 9;
        case 10: return 11;
        case 12: return 13;
        default: return d_nom;
        }
    };
    { CString __v; __v.Format(L"%d", GetD2_7796(d_nom)); SetRO(this, IDC_EDIT_B7796_D2, __v); }
    // В поле "l1" выводим итоговую длину болта l (см. п.3 и фото5)
    { CString __v; __v.Format(L"%d", bolt_l); SetRO(this, IDC_EDIT_B7796_L1, __v); }
    SetRO(this, IDC_EDIT_B7796_K, FormatSmart(GetK(d_nom)));
    { CString __v; __v.Format(L"%d", GetS7796(d_nom)); SetRO(this, IDC_EDIT_B7796_S, __v); }

    // Сохраняем параметры, которые показали в editbox-ах, в документ
    {
        CKURSACHDoc::BoltUIParams p;
        p.d2 = (double)GetD2_7796(d_nom);
        p.l  = bolt_l;
        p.k  = GetK(d_nom);
        p.S  = GetS7796(d_nom);
        p.nominal = d_nom;
        m_pDoc->SetBolt7796Params(p);
    }

    return TRUE;
}
