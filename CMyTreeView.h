#pragma once
#include <afxcview.h>

class CMyTreeView : public CTreeView
{
	DECLARE_DYNCREATE(CMyTreeView)

protected:
	CMyTreeView();

public:
	virtual ~CMyTreeView();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);

private:
	HTREEITEM m_hRoot;
	HTREEITEM m_hHalfCoupling;
	HTREEITEM m_hBolt7817;
	HTREEITEM m_hBolt7796;
	HTREEITEM m_hNut;
	HTREEITEM m_hWasher;
};