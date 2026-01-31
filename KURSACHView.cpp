// KURSACHView.cpp: реализация класса CKURSACHView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "KURSACH.h"
#endif

#include "KURSACHDoc.h"
#include "KURSACHView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKURSACHView

IMPLEMENT_DYNCREATE(CKURSACHView, CView)

BEGIN_MESSAGE_MAP(CKURSACHView, CView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// Создание или уничтожение CKURSACHView

CKURSACHView::CKURSACHView() noexcept
{
	// TODO: добавьте код создания

}

CKURSACHView::~CKURSACHView()
{
}

BOOL CKURSACHView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CKURSACHView

void CKURSACHView::OnDraw(CDC* pDC)
{
	CKURSACHDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Получаем путь к текущему изображению
	CString imagePath = pDoc->GetCurrentImagePath();

	if (!imagePath.IsEmpty())
	{
		// Загружаем и отображаем изображение
		HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, imagePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		if (hBitmap)
		{
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);

			HBITMAP hOldBitmap = (HBITMAP)memDC.SelectObject(hBitmap);

			BITMAP bitmap;
			GetObject(hBitmap, sizeof(BITMAP), &bitmap);

			pDC->BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, &memDC, 0, 0, SRCCOPY);

			memDC.SelectObject(hOldBitmap);
			DeleteObject(hBitmap);
		}
		else
		{
			// Если изображение не загрузилось, выводим сообщение
			pDC->TextOut(10, 10, _T("Изображение не найдено: ") + imagePath);
		}
	}
	else
	{
		// TODO: добавьте здесь код отрисовки для собственных данных
		pDC->TextOut(10, 10, _T("Выберите элемент сборки для просмотра чертежа"));
	}
}


// Печать CKURSACHView

BOOL CKURSACHView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CKURSACHView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью
}

void CKURSACHView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}


// Диагностика CKURSACHView

#ifdef _DEBUG
void CKURSACHView::AssertValid() const
{
	CView::AssertValid();
}

void CKURSACHView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKURSACHDoc* CKURSACHView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKURSACHDoc)));
	return (CKURSACHDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CKURSACHView
