#include "pch.h"
#include "framework.h"
#include "KURSACH.h"
#include "CMyTreeView.h"
#include "KURSACHDoc.h"
#include "KURSACHView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMyTreeView, CTreeView)

BEGIN_MESSAGE_MAP(CMyTreeView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CMyTreeView::OnSelchanged)
END_MESSAGE_MAP()

CMyTreeView::CMyTreeView()
{
}

CMyTreeView::~CMyTreeView()
{
}

void CMyTreeView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	CTreeCtrl& treeCtrl = GetTreeCtrl();

	// Создаем корневой элемент - Сборка
	m_hRoot = treeCtrl.InsertItem(_T("Сборка"));

	// Добавляем элементы сборки
	m_hHalfCoupling = treeCtrl.InsertItem(_T("Полумуфта"), m_hRoot);
	m_hBolt7817 = treeCtrl.InsertItem(_T("Болт ГОСТ 7817"), m_hRoot);
	m_hBolt7796 = treeCtrl.InsertItem(_T("Болт ГОСТ 7796"), m_hRoot);
	m_hNut = treeCtrl.InsertItem(_T("Гайка"), m_hRoot);
	m_hWasher = treeCtrl.InsertItem(_T("Шайба"), m_hRoot);

	// Разворачиваем дерево
	treeCtrl.Expand(m_hRoot, TVE_EXPAND);
}

void CMyTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	CTreeCtrl& treeCtrl = GetTreeCtrl();
	HTREEITEM hSelected = pNMTreeView->itemNew.hItem;

	CKURSACHDoc* pDoc = (CKURSACHDoc*)GetDocument();
	if (!pDoc)
		return;

	CString imagePath;

	if (hSelected == m_hHalfCoupling)
	{
		imagePath = _T("Polumufta.bmp");
	}
	else if (hSelected == m_hBolt7817)
	{
		imagePath = _T("Bolt_GOST7817.bmp");
	}
	else if (hSelected == m_hBolt7796)
	{
		imagePath = _T("Bolt_GOST7796.bmp");
	}
	else if (hSelected == m_hNut)
	{
		imagePath = _T("Gayka.bmp");
	}
	else if (hSelected == m_hWasher)
	{
		imagePath = _T("Shayba.bmp");
	}

	// Устанавливаем путь к изображению в документе
	pDoc->SetCurrentImagePath(imagePath);

	// Обновляем вид для отображения изображения
	pDoc->UpdateAllViews(NULL);

	*pResult = 0;
}
