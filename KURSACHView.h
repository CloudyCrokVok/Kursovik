
// KURSACHView.h: интерфейс класса CKURSACHView
//

#pragma once
#include "KURSACHDoc.h"

class CKURSACHView : public CView
{
protected: // создать только из сериализации
	CKURSACHView() noexcept;
	DECLARE_DYNCREATE(CKURSACHView)

// Атрибуты
public:
	CKURSACHDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Реализация
public:
	virtual ~CKURSACHView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // версия отладки в KURSACHView.cpp
inline CKURSACHDoc* CKURSACHView::GetDocument() const
   { return reinterpret_cast<CKURSACHDoc*>(m_pDocument); }
#endif

