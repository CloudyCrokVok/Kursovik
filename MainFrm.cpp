// MainFrm.cpp: реализация класса CMainFrame
//

#include "pch.h"
#include "framework.h"
#include "KURSACH.h"
#include "KURSACHView.h"
#include "CMyTreeView.h"
#include "CObjectsDlg.h"
#include "CShaybaDlg.h"
#include "CBolt7817Dlg.h"
#include "CBolt7796Dlg.h"
#include "CNutDlg.h"
#include "KURSACHDoc.h"
#include "MainFrm.h"

// --- KOMPAS-3D API5 подключение (вариант A: двойные \\) ---
#include "C:\\Program Files\\ASCON\\KOMPAS-3D v23 Study\\SDK\\Include\\ksConstants.h"
#include "C:\\Program Files\\ASCON\\KOMPAS-3D v23 Study\\SDK\\Include\\ksConstants3D.h"

#import "C:\\Program Files\\ASCON\\KOMPAS-3D v23 Study\\SDK\\lib\\kAPI5.tlb" \
    rename("GetObject", "KompasGetObject") \
    rename("min",       "KompasMin") \
    rename("max",       "KompasMax")

#include <comdef.h>
#include <oleauto.h>
// -----------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)


// ---- KOMPAS-3D запуск (без параметров/построения) ----
static bool SetDispatchBoolProperty(IDispatch* disp, LPCOLESTR name, bool value)
{
    if (!disp) return false;

    DISPID dispid;
    HRESULT hr = disp->GetIDsOfNames(IID_NULL, const_cast<LPOLESTR*>(&name), 1,
                                    LOCALE_USER_DEFAULT, &dispid);
    if (FAILED(hr)) return false;

    VARIANTARG var;
    VariantInit(&var);
    var.vt = VT_BOOL;
    var.boolVal = value ? VARIANT_TRUE : VARIANT_FALSE;

    DISPPARAMS params{};
    params.cArgs = 1;
    params.rgvarg = &var;
    params.cNamedArgs = 1;
    DISPID dispidNamed = DISPID_PROPERTYPUT;
    params.rgdispidNamedArgs = &dispidNamed;

    hr = disp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT,
                      DISPATCH_PROPERTYPUT, &params, nullptr, nullptr, nullptr);

    VariantClear(&var);
    return SUCCEEDED(hr);
}

static bool StartKompasAPI5()
{
    // MFC обычно уже инициализирует OLE через AfxOleInit(), но на всякий случай:
    HRESULT hrCo = CoInitialize(nullptr);

    CLSID clsid{};
    HRESULT hr = CLSIDFromProgID(L"KOMPAS.Application.5", &clsid);
    if (FAILED(hr))
    {
        if (SUCCEEDED(hrCo)) CoUninitialize();
        return false;
    }

    IUnknown* unk = nullptr;
    hr = GetActiveObject(clsid, nullptr, &unk);

    IDispatch* disp = nullptr;
    if (SUCCEEDED(hr) && unk)
    {
        unk->QueryInterface(IID_IDispatch, (void**)&disp);
        unk->Release();
    }
    else
    {
        hr = CoCreateInstance(clsid, nullptr, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&disp);
        if (FAILED(hr) || !disp)
        {
            if (SUCCEEDED(hrCo)) CoUninitialize();
            return false;
        }
    }

    // делаем видимым
    SetDispatchBoolProperty(disp, L"Visible", true);
    disp->Release();

    if (SUCCEEDED(hrCo)) CoUninitialize();
    return true;
}
// ------------------------------------------------------

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	// Добавляем обработчики для меню Сборка
	ON_COMMAND(ID_PARAMETRS, &CMainFrame::OnParametrs)
	ON_COMMAND(ID_SBORKA, &CMainFrame::OnSborka)
	ON_COMMAND(ID_SHAYBA, &CMainFrame::OnShayba)
	ON_COMMAND(ID_GAYKA, &CMainFrame::OnGayka)
	ON_COMMAND(ID_BOLT_7796, &CMainFrame::OnBolt7796)
	ON_COMMAND(ID_BOLT_7817, &CMainFrame::OnBolt7817)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // индикатор строки состояния
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// Создание или уничтожение CMainFrame

CMainFrame::CMainFrame() noexcept
{
	// TODO: добавьте код инициализации члена
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Не удалось создать панель инструментов\n");
		return -1;      // не удалось создать
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Не удалось создать строку состояния\n");
		return -1;      // не удалось создать
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	// TODO: Удалите эти три строки, если не собираетесь закреплять панель инструментов
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return TRUE;
}

// Диагностика CMainFrame

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// Обработчики сообщений CMainFrame

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: добавьте специализированный код или вызов базового класса
	m_wndSplitter.CreateStatic(this, 1, 2);
	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyTreeView), CSize(300, 0), pContext);
	m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CKURSACHView), CSize(0, 0), pContext);
	return TRUE;
}

void CMainFrame::OnParametrs()
{
	// Открываем диалог параметров полумуфты
	CObjectsDlg dlg;

	CKURSACHDoc* pDoc = (CKURSACHDoc*)GetActiveDocument();
	if (pDoc)
	{
		int execution, variant;
		pDoc->GetHalfCouplingParams(execution, variant);

		// Устанавливаем текущие значения в диалоге
		dlg.SetExecution(execution);
		dlg.SetVariant(variant);

		// Устанавливаем документ в диалоге
		dlg.SetDocument(pDoc);

		// Получаем текущие данные и устанавливаем их в диалог
		std::vector<double> currentData = pDoc->GetHalfCouplingData(execution, variant);
		if (!currentData.empty())
		{
			dlg.SetDocumentData(currentData);
		}
	}

	if (dlg.DoModal() == IDOK)
	{
		// Сохраняем выбранные параметры в документе
		if (pDoc)
		{
			int execution = dlg.GetExecution();
			int variant = dlg.GetVariant();

			pDoc->SetHalfCouplingParams(execution, variant);

			// Обновляем статус бар с выбранными параметрами
			CString statusText;
			statusText.Format(_T("Исполнение: %d, Вариант: %d"), execution, variant);
			m_wndStatusBar.SetPaneText(0, statusText);
		}
	}
}

void CMainFrame::OnShayba()
{
	// Получаем документ безопасным способом (через активное представление)
	CKURSACHDoc* pDoc = nullptr;
	CView* pView = GetActiveView();
	if (pView)
		pDoc = (CKURSACHDoc*)pView->GetDocument();

	CShaybaDlg dlg(pDoc, this);
	dlg.DoModal();
}

void CMainFrame::OnSborka()
{
	// Запуск КОМПАС-3D (API5)
	if (!StartKompasAPI5())
	{
		MessageBox(_T("Не удалось запустить/подключиться к КОМПАС-3D (API5)."), _T("Ошибка"), MB_OK | MB_ICONERROR);
		return;
	}


	// Построение сборки
	CKURSACHDoc* pDoc = (CKURSACHDoc*)GetActiveDocument();
	if (pDoc)
	{
		// Получаем текущие параметры полумуфты
		int execution, variant;
		pDoc->GetHalfCouplingParams(execution, variant);

		// Получаем данные для этих параметров
		std::vector<double> halfCouplingData = pDoc->GetHalfCouplingData(execution, variant);

		if (!halfCouplingData.empty())
		{
			// Формируем сообщение с параметрами
			CString message;
			message.Format(_T("Сборка построена успешно!\n\n")
				_T("Параметры полумуфты:\n")
				_T("Исполнение: %d\n")
				_T("Вариант: %d\n")
				_T("Момент (M): %.1f Н·м\n")
				_T("Диаметр (d): %.0f мм\n")
				_T("Масса: %.2f кг"),
				execution, variant,
				halfCouplingData[0], // M
				halfCouplingData[1], // d
				halfCouplingData[22]); // mass

			MessageBox(message, _T("Сборка"), MB_OK | MB_ICONINFORMATION);

			// Обновляем статус бар
			CString statusText;
			statusText.Format(_T("Сборка построена. M=%.1f, d=%.0f"),
				halfCouplingData[0], halfCouplingData[1]);
			m_wndStatusBar.SetPaneText(0, statusText);
		}
		else
		{
			MessageBox(_T("Данные для выбранных параметров не найдены"),
				_T("Ошибка"), MB_OK | MB_ICONERROR);
		}
	}
	else
	{
		MessageBox(_T("Документ не найден"), _T("Ошибка"), MB_OK | MB_ICONERROR);
	}
}

void CMainFrame::OnGayka()
{
	// Получаем документ безопасным способом (через активное представление)
	CKURSACHDoc* pDoc = nullptr;
	CView* pView = GetActiveView();
	if (pView)
		pDoc = (CKURSACHDoc*)pView->GetDocument();

	CNutDlg dlg(pDoc, this);
	dlg.DoModal();
}

void CMainFrame::OnBolt7796()
{
	// Получаем документ безопасным способом (через активное представление)
	CKURSACHDoc* pDoc = nullptr;
	CView* pView = GetActiveView();
	if (pView)
		pDoc = (CKURSACHDoc*)pView->GetDocument();

	CBolt7796Dlg dlg(pDoc, this);
	dlg.DoModal();
}

void CMainFrame::OnBolt7817()
{
	// Получаем документ безопасным способом (через активное представление)
	CKURSACHDoc* pDoc = nullptr;
	CView* pView = GetActiveView();
	if (pView)
		pDoc = (CKURSACHDoc*)pView->GetDocument();

	CBolt7817Dlg dlg(pDoc, this);
	dlg.DoModal();
}
