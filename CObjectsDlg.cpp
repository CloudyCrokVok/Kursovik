#include "pch.h"
#include "framework.h"
#include "KURSACH.h"
#include "CObjectsDlg.h"
#include "afxdialogex.h"

// CObjectsDlg dialog
IMPLEMENT_DYNAMIC(CObjectsDlg, CDialogEx)

CObjectsDlg::CObjectsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_selectedExecution(0)
	, m_selectedVariant(0)
	, m_editM(nullptr)
	, m_editD(nullptr)
	, m_editB(nullptr)
	, m_editH(nullptr)
	, m_editD1(nullptr)
	, m_editMass(nullptr)
{
}

CObjectsDlg::~CObjectsDlg()
{
}

void CObjectsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboExecution);
	DDX_Control(pDX, IDC_COMBO2, m_comboVariant);
}

BEGIN_MESSAGE_MAP(CObjectsDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CObjectsDlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CObjectsDlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDOK, &CObjectsDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CObjectsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Получаем указатели на элементы редактирования
	m_editM = (CEdit*)GetDlgItem(IDC_EDIT_M);
	m_editD = (CEdit*)GetDlgItem(IDC_EDIT_D);
	m_editB = (CEdit*)GetDlgItem(IDC_EDIT_B);
	m_editH = (CEdit*)GetDlgItem(IDC_EDIT_H);
	m_editD1 = (CEdit*)GetDlgItem(IDC_EDIT_D1);
	m_editMass = (CEdit*)GetDlgItem(IDC_EDIT_MASS);

	// Делаем поля только для чтения
	if (m_editM) m_editM->SetReadOnly(TRUE);
	if (m_editD) m_editD->SetReadOnly(TRUE);
	if (m_editB) m_editB->SetReadOnly(TRUE);
	if (m_editH) m_editH->SetReadOnly(TRUE);
	if (m_editD1) m_editD1->SetReadOnly(TRUE);
	if (m_editMass) m_editMass->SetReadOnly(TRUE);

	// Заполняем комбобокс исполнений (7 вариантов по возрастанию)
	m_comboExecution.AddString(_T("16"));
	m_comboExecution.AddString(_T("31.5"));
	m_comboExecution.AddString(_T("63"));
	m_comboExecution.AddString(_T("125"));
	m_comboExecution.AddString(_T("160"));
	m_comboExecution.AddString(_T("250"));
	m_comboExecution.AddString(_T("400"));

	// Устанавливаем текущее исполнение
	if (m_selectedExecution >= 0 && m_selectedExecution < 7)
	{
		m_comboExecution.SetCurSel(m_selectedExecution);
	}
	else
	{
		m_comboExecution.SetCurSel(0);
		m_selectedExecution = 0;
	}

	// Заполняем комбобокс вариантов для выбранного исполнения
	OnCbnSelchangeCombo1();

	// Устанавливаем текущий вариант
	if (m_selectedVariant >= 0 && m_selectedVariant < 2)
	{
		m_comboVariant.SetCurSel(m_selectedVariant);
	}
	else
	{
		m_comboVariant.SetCurSel(0);
		m_selectedVariant = 0;
	}

	// Обновляем отображаемые данные
	UpdateDisplayedData();

	return TRUE;
}

void CObjectsDlg::OnCbnSelchangeCombo1()
{
	m_selectedExecution = m_comboExecution.GetCurSel();
	m_comboVariant.ResetContent();

	// Для каждого исполнения доступно 2 варианта
	m_comboVariant.AddString(_T("1"));
	m_comboVariant.AddString(_T("2"));

	m_comboVariant.SetCurSel(0);
	m_selectedVariant = 0;

	// Обновляем отображаемые данные
	UpdateDisplayedData();
}

void CObjectsDlg::OnCbnSelchangeCombo2()
{
	m_selectedVariant = m_comboVariant.GetCurSel();

	// Обновляем отображаемые данные
	UpdateDisplayedData();
}

void CObjectsDlg::SetExecution(int execution)
{
	// Преобразуем исполнение в индекс комбобокса
	if (execution == 16) m_selectedExecution = 0;
	else if (execution == 31) m_selectedExecution = 1;  // 31.5
	else if (execution == 63) m_selectedExecution = 2;
	else if (execution == 125) m_selectedExecution = 3;
	else if (execution == 160) m_selectedExecution = 4;
	else if (execution == 250) m_selectedExecution = 5;
	else if (execution == 400) m_selectedExecution = 6;
	else m_selectedExecution = 0;
}

int CObjectsDlg::GetExecution() const
{
	// Преобразуем индекс комбобокса в значение исполнения
	switch (m_selectedExecution)
	{
	case 0: return 16;
	case 1: return 31;  // 31.5
	case 2: return 63;
	case 3: return 125;
	case 4: return 160;
	case 5: return 250;
	case 6: return 400;
	default: return 16;
	}
}

void CObjectsDlg::SetVariant(int variant)
{
	m_selectedVariant = variant - 1;
	if (m_selectedVariant < 0) m_selectedVariant = 0;
	if (m_selectedVariant > 1) m_selectedVariant = 1;
}

int CObjectsDlg::GetVariant() const
{
	return m_selectedVariant + 1;
}

void CObjectsDlg::SetDocumentData(const std::vector<double>& data)
{
	m_currentData = data;
	UpdateDisplayedData();
}

void CObjectsDlg::UpdateDisplayedData()
{
	if (m_currentData.size() >= 23)  // У нас 23 параметра
	{
		CString strValue;

		// M (исполнение)
		strValue.Format(_T("%.1f"), m_currentData[0]);
		if (m_editM) m_editM->SetWindowText(strValue);

		// d (диаметр)
		strValue.Format(_T("%.0f"), m_currentData[1]);
		if (m_editD) m_editD->SetWindowText(strValue);

		// exec (исполнение)
		strValue.Format(_T("%.0f"), m_currentData[2]);
		// Если нужно отобразить exec

		// B
		strValue.Format(_T("%.0f"), m_currentData[3]);
		if (m_editB) m_editB->SetWindowText(strValue);

		// h
		if (m_currentData[4] > 0)
			strValue.Format(_T("%.1f"), m_currentData[4]);
		else
			strValue = _T("N/A");
		if (m_editH) m_editH->SetWindowText(strValue);

		// h_dev
		// m_currentData[5]

		// b1_num
		// m_currentData[6]

		// b1_den
		// m_currentData[7]

		// b2_num
		// m_currentData[8]

		// b2_den
		// m_currentData[9]

		// D
		strValue.Format(_T("%.0f"), m_currentData[10]);
		if (m_editDParam) m_editDParam->SetWindowText(strValue);

		// D1
		strValue.Format(_T("%.0f"), m_currentData[11]);
		if (m_editD1) m_editD1->SetWindowText(strValue);

		// d0
		strValue.Format(_T("%.0f"), m_currentData[12]);
		if (m_editD0Val) m_editD0Val->SetWindowText(strValue);

		// d1
		strValue.Format(_T("%.0f"), m_currentData[13]);
		if (m_editD1Val) m_editD1Val->SetWindowText(strValue);

		// d2
		// m_currentData[14]

		// n
		// m_currentData[15]

		// d4
		// m_currentData[16]

		// n3
		// m_currentData[17]

		// l
		strValue.Format(_T("%.0f"), m_currentData[18]);
		if (m_editL) m_editL->SetWindowText(strValue);

		// l1
		strValue.Format(_T("%.0f"), m_currentData[19]);
		if (m_editL1) m_editL1->SetWindowText(strValue);

		// l2
		strValue.Format(_T("%.0f"), m_currentData[20]);
		if (m_editL2) m_editL2->SetWindowText(strValue);

		// R
		strValue.Format(_T("%.1f"), m_currentData[21]);
		if (m_editR) m_editR->SetWindowText(strValue);

		// mass
		strValue.Format(_T("%.2f"), m_currentData[22]);
		if (m_editMass) m_editMass->SetWindowText(strValue);
	}
}

void CObjectsDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}
