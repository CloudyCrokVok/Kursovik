#include "pch.h"
#include "framework.h"
#include "KURSACH.h"
#include "KURSACHDoc.h"
#include "resource.h"
#include "CShaybaDlg.h"
#include <cmath>

IMPLEMENT_DYNAMIC(CShaybaDlg, CDialogEx)

CShaybaDlg::CShaybaDlg(CKURSACHDoc* pDoc, CWnd* pParent)
	: CDialogEx(IDD_SHAYBA_PARAMS, pParent), m_pDoc(pDoc)
{
}

void CShaybaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CShaybaDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CShaybaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// По умолчанию пусто
	double d2 = 0.0, d4 = 0.0;
	int n = 0;

	if (m_pDoc)
	{
		int execution = 0, variant = 0;
		m_pDoc->GetHalfCouplingParams(execution, variant);

		// ВНИМАНИЕ: в вашей таблице HalfCouplingData поле exec хранит 1/2,
		// а в UI вариант у вас идёт как 0/1. Поэтому приводим к 1/2.
		int tableVariant = variant;
		if (tableVariant == 0 || tableVariant == 1)
			tableVariant = variant + 1;

		std::vector<double> data = m_pDoc->GetHalfCouplingData(execution, tableVariant);

		// Индексы по вашей функции GetHalfCouplingData():
		// 14: d2, 15: n, 16: d4
		if ((int)data.size() > 16)
		{
			d2 = data[14];
			n = (int)(data[15] + 0.5);
			d4 = data[16];
		}
	}

	m_d2.Format((fabs(d2 - floor(d2 + 0.5)) < 1e-6) ? L"%.0f" : L"%.1f", d2);
	m_n.Format(L"%d", n);
	m_d4.Format((fabs(d4 - floor(d4 + 0.5)) < 1e-6) ? L"%.0f" : L"%.1f", d4);

	SetDlgItemText(IDC_EDIT_SH_D2, m_d2);
	SetDlgItemText(IDC_EDIT_SH_N, m_n);
	SetDlgItemText(IDC_EDIT_SH_D4, m_d4);

	// На всякий случай ещё раз в коде делаем ReadOnly
	((CEdit*)GetDlgItem(IDC_EDIT_SH_D2))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_SH_N))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_SH_D4))->SetReadOnly(TRUE);

	return TRUE;
}
