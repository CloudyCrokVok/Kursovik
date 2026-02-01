#pragma once
#include "afxdialogex.h"
#include <vector>

// Предварительное объявление класса документа
class CKURSACHDoc;

// CObjectsDlg dialog
class CObjectsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CObjectsDlg)

public:
	CObjectsDlg(CWnd* pParent = nullptr);
	virtual ~CObjectsDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	// Методы для установки/получения значений
	void SetExecution(int execution);
	int GetExecution() const;
	void SetVariant(int variant);
	int GetVariant() const;

	// Данные из документа
	void SetDocumentData(const std::vector<double>& data);

	// Установка указателя на документ
	void SetDocument(CKURSACHDoc* pDoc) { m_pDoc = pDoc; }

	CComboBox m_comboExecution;
	CComboBox m_comboVariant;

	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedOk();

private:
	int m_selectedExecution;
	int m_selectedVariant;
	std::vector<double> m_currentData;
	CKURSACHDoc* m_pDoc;  // Указатель на документ для получения данных

	// Указатели на поля для отображения данных
	CEdit* m_editM;
	CEdit* m_editD;
	CEdit* m_editB;
	CEdit* m_editH;
	CEdit* m_editD1;
	CEdit* m_editMass;
	CEdit* m_editD0Val;
	CEdit* m_editD1Val;
	CEdit* m_editL;
	CEdit* m_editL1;
	CEdit* m_editL2;
	CEdit* m_editDParam;
	CEdit* m_editR;
	CEdit* m_editD2;
	CEdit* m_editN;
	CEdit* m_editD4;
	CEdit* m_editN3;
	CEdit* m_editExecVal;

	// Обновление отображаемых данных
	void UpdateDisplayedData();
	// Обновление данных из документа
	void UpdateDataFromDocument();
};
