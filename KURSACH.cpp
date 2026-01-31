// KURSACH.cpp: определяет поведение классов для приложения.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "KURSACH.h"
#include "MainFrm.h"

#include "KURSACHDoc.h"
#include "KURSACHView.h"
#include "CObjectsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKURSACHApp

BEGIN_MESSAGE_MAP(CKURSACHApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CKURSACHApp::OnAppAbout)
	// Стандартные команды по работе с файлами документов
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Стандартная команда настройки печати
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	// Обработчики для меню Сборка
	ON_COMMAND(ID_PARAMETRS, &CKURSACHApp::OnParametrs)
	ON_COMMAND(ID_SBORKA, &CKURSACHApp::OnSborka)
END_MESSAGE_MAP()


// Создание CKURSACHApp

CKURSACHApp::CKURSACHApp() noexcept
{

	// поддержка диспетчера перезагрузки
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// Если приложение построено с поддержкой среды Common Language Runtime (/clr):
	//     1) Этот дополнительный параметр требуется для правильной поддержки работы диспетчера перезагрузки.
	//   2) В своем проекте для сборки необходимо добавить ссылку на System.Windows.Forms.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: замените ниже строку идентификатора приложения строкой уникального идентификатора; рекомендуемый
	// формат для строки: ИмяКомпании.ИмяПродукта.СубПродукт.СведенияОВерсии
	SetAppID(_T("KURSACH.AppID.NoVersion"));

	// TODO: добавьте код создания,
	// Размещает весь важный код инициализации в InitInstance
}

// Единственный объект CKURSACHApp

CKURSACHApp theApp;


// Инициализация CKURSACHApp

BOOL CKURSACHApp::InitInstance()
{
	// InitCommonControlsEx() требуются для Windows XP, если манифест
	// приложения использует ComCtl32.dll версии 6 или более поздней версии для включения
	// стилей отображения.  В противном случае будет возникать сбой при создании любого окна.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Выберите этот параметр для включения всех общих классов управления, которые необходимо использовать
	// в вашем приложении.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// Инициализация библиотек OLE
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// Для использования элемента управления RichEdit требуется метод AfxInitRichEdit2()
	// AfxInitRichEdit2();

	// Стандартная инициализация
	// Если эти возможности не используются и необходимо уменьшить размер
	// конечного исполняемого файла, необходимо удалить из следующего
	// конкретные процедуры инициализации, которые не требуются
	// Измените раздел реестра, в котором хранятся параметры
	// TODO: следует изменить эту строку на что-нибудь подходящее,
	// например на название организации
	SetRegistryKey(_T("Локальные приложения, созданные с помощью мастера приложений"));
	LoadStdProfileSettings(4);  // Загрузите стандартные параметры INI-файла (включая MRU)


	// Зарегистрируйте шаблоны документов приложения.  Шаблоны документов
	//  выступают в роли посредника между документами, окнами рамок и представлениями
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CKURSACHDoc),
		RUNTIME_CLASS(CMainFrame),       // основное окно рамки SDI
		RUNTIME_CLASS(CKURSACHView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Разрешить использование расширенных символов в горячих клавишах меню
	CMFCToolBar::m_bExtCharTranslation = TRUE;

	// Синтаксический разбор командной строки на стандартные команды оболочки, DDE, открытие файлов
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Команды диспетчеризации, указанные в командной строке.  Значение FALSE будет возвращено, если
	// приложение было запущено с параметром /RegServer, /Register, /Unregserver или /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Одно и только одно окно было инициализировано, поэтому отобразите и обновите его
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CKURSACHApp::ExitInstance()
{
	//TODO: обработайте дополнительные ресурсы, которые могли быть добавлены
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// Обработчики сообщений CKURSACHApp

void CKURSACHApp::OnParametrs()
{
	// Открываем диалог параметров полумуфты
	CObjectsDlg dlg;

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		CKURSACHDoc* pDoc = (CKURSACHDoc*)pMainFrame->GetActiveDocument();
		if (pDoc)
		{
			int execution, variant;
			pDoc->GetHalfCouplingParams(execution, variant);

			// Устанавливаем текущие значения в диалоге
			// (предполагается, что dlg имеет методы SetExecution и SetVariant)
			dlg.SetExecution(execution);
			dlg.SetVariant(variant);
		}
	}

	if (dlg.DoModal() == IDOK)
	{
		// Сохраняем выбранные параметры в документе
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		if (pMainFrame)
		{
			CKURSACHDoc* pDoc = (CKURSACHDoc*)pMainFrame->GetActiveDocument();
			if (pDoc)
			{
				pDoc->SetHalfCouplingParams(dlg.GetExecution(), dlg.GetVariant());
			}
		}
	}
}

void CKURSACHApp::OnSborka()
{
	// Построение сборки
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		CKURSACHDoc* pDoc = (CKURSACHDoc*)pMainFrame->GetActiveDocument();
		if (pDoc)
		{
			// Получаем текущие параметры полумуфты
			int execution, variant;
			pDoc->GetHalfCouplingParams(execution, variant);

			// Получаем данные для этих параметров
			std::vector<double> halfCouplingData = pDoc->GetHalfCouplingData(execution, variant);

			// Формируем сообщение с параметрами
			CString message;
			message.Format(_T("Сборка построена с параметрами:\n")
				_T("Исполнение: %d\n")
				_T("Вариант: %d\n")
				_T("Всего параметров полумуфты: %d"),
				execution, variant, (int)halfCouplingData.size());

			AfxMessageBox(message, MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			AfxMessageBox(_T("Документ не найден"));
		}
	}
	else
	{
		AfxMessageBox(_T("Главное окно не найдено"));
	}
}

// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

	// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// Команда приложения для запуска диалога
void CKURSACHApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
