// MainFrm.h: интерфейс класса CMainFrame
//

#pragma once

class CMainFrame : public CFrameWnd
{

protected: // создать только из сериализации
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

	// Атрибуты
public:

	// Операции
public:
	// Добавляем методы для обработки команд меню
	void OnParametrs();
	void OnShayba();
	void OnGayka();
	void OnBolt7796();
	void OnBolt7817();
	void CreatePolumufta();
	void CreateBoltGOST7796();
	void CreateBoltGOST7817();
	void CreateGayka();
	void CreateShayba();
	void CreateAssembly();
	void OnSborka();

	// Переопределение
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	// Реализация
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // встроенные члены панели элементов управления
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	CSplitterWnd      m_wndSplitter;

	// Созданные функции схемы сообщений
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};
