// PaneTreeCtrl.cpp: implementation of the CPaneTreeCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RibbonReport.h"
#include "MainFrm.h"

#include "PaneTreeCtrl.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLIPFORMAT CPaneTreeCtrl::m_cfItem = (CLIPFORMAT)::RegisterClipboardFormat(_T("PaneTreeCtrl"));
int CPaneTreeCtrl::m_nItemCount = 0;

BEGIN_MESSAGE_MAP(CPaneTreeCtrl, CXTPTreeCtrl)
	//{{AFX_MSG_MAP(CPaneTreeCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemExpanded)  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


int CPaneTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyleEx(0, WS_EX_STATICEDGE);

	m_ilTreeIcons.Create(16, 16, ILC_MASK|ILC_COLOR24, 0, 1);

	CBitmap bmp;
	bmp.LoadBitmap(IDB_TREE_TYPE_ICONS);

	m_ilTreeIcons.Add(&bmp, RGB(0, 255, 0));
	SetImageList(&m_ilTreeIcons, TVSIL_NORMAL);

	return 0;
}

void CPaneTreeCtrl::GetChildItemCount(HTREEITEM hItem)
{
	if(hItem && ItemHasChildren(hItem))
	{
		HTREEITEM hItemChild = GetChildItem(hItem);
		while (hItemChild)
		{
			hItemChild = GetNextSiblingItem(hItemChild);
			m_nItemCount++;
		}
	}
	else
	{
		HTREEITEM hItemRoot = GetRootItem();
		HTREEITEM hItemChild = GetChildItem(hItemRoot);
		while (hItemChild)
		{
			if(ItemHasChildren(hItemChild))
				GetChildItemCount(hItemChild);

			hItemChild = GetNextSiblingItem(hItemChild);
		}
	}
}

CPaneTreeCtrl::TreeType CPaneTreeCtrl::GetItemType(HTREEITEM hItem)
{
	if (!GetParentItem(hItem))
		return treeRoot;

	return ItemHasChildren(hItem) ? treeCategory : treeItem;
}

void CPaneTreeCtrl::ExpandItems(HTREEITEM hItem)
{
	if(hItem && ItemHasChildren(hItem))
	{
		Expand(hItem, TVE_EXPAND);
	}
	else
	{
		HTREEITEM hItemRoot = GetRootItem();
		Expand(hItemRoot, TVE_EXPAND);

		HTREEITEM hItemChild = GetChildItem(hItemRoot);
		while (hItemChild)
		{
			if(ItemHasChildren(hItemChild))
				ExpandItems(hItemChild);

			hItemChild = GetNextSiblingItem(hItemChild);
		}
	}
}

void CPaneTreeCtrl::CollapseItems(HTREEITEM hItem)
{
	if(hItem && ItemHasChildren(hItem))
	{
		Expand(hItem, TVE_COLLAPSE);
	}
	else
	{
		HTREEITEM hItemRoot = GetRootItem();
		Expand(hItemRoot, TVE_COLLAPSE);

		HTREEITEM hItemChild = GetChildItem(hItemRoot);
		while (hItemChild)
		{
			if(ItemHasChildren(hItemChild))
				ExpandItems(hItemChild);

			hItemChild = GetNextSiblingItem(hItemChild);
		}
	}
}

LRESULT CPaneTreeCtrl::WindowProc(UINT message,WPARAM wParam,LPARAM lParam)
{
	switch (message)
	{
	case WM_NCPAINT:
		{
			CXTPTreeCtrl::WindowProc(message, wParam, lParam);

			CRect rc;
			GetWindowRect(&rc);
			CWindowDC dc(this);
			rc.OffsetRect(-rc.TopLeft());
			COLORREF clr = GetXtremeColor(XPCOLOR_STATICFRAME);
			dc.Draw3dRect(rc, clr, clr);
			return TRUE;
		}
	}
	return CXTPTreeCtrl::WindowProc(message, wParam, lParam);
}

void CPaneTreeCtrl::OnItemExpanded(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTREEVIEW* pNMTreeView = (NMTREEVIEW *)pNMHDR;
	if(pNMTreeView->action == TVE_EXPAND)
		SetItemImage(pNMTreeView->itemNew.hItem, 1, 1);

	if(pNMTreeView->action == TVE_COLLAPSE)
		SetItemImage(pNMTreeView->itemNew.hItem, 0, 0);
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPControlAction* CActions::operator[](int nIndex)
{
	return GetAt(nIndex);
}

int CActions::InsertAction(CXTPControlAction* pAction)
{
	int nIndex = 0;
	for (; nIndex < GetCount(); nIndex++)
	{
		if (GetAt(nIndex)->GetID() > pAction->GetID())
			break;
	}

	m_arrActions.InsertAt(nIndex, pAction);
	return nIndex;
}

CXTPControlAction* CActions::AddAction(UINT nID, LPCTSTR lpszKey, LPCTSTR lpszCategory, UINT nImage, LPCTSTR lpszDescription, LPCTSTR lpszTooltip)
{
	CXTPControlAction* pAction = Add(nID);
	pAction->SetKey(lpszKey);
	pAction->SetCategory(lpszCategory);
	if(nImage > 0) pAction->SetIconId(nImage);
	if(_tcslen(lpszDescription)) pAction->SetDescription(lpszDescription);
	if(_tcslen(lpszTooltip)) pAction->SetTooltip(lpszTooltip);
	return pAction;
}

CXTPControlAction* CActions::AddAction(LPCTSTR lpszCaption, UINT nID, LPCTSTR lpszKey, LPCTSTR lpszCategory, UINT nImage, LPCTSTR lpszDescription, LPCTSTR lpszTooltip)
{
	CXTPControlAction* pAction = Add(nID);
	pAction->SetCaption(lpszCaption);
	pAction->SetKey(lpszKey);
	pAction->SetCategory(lpszCategory);
	if(nImage > 0) pAction->SetIconId(nImage);
	if(_tcslen(lpszDescription)) pAction->SetDescription(lpszDescription);
	if(_tcslen(lpszTooltip)) pAction->SetTooltip(lpszTooltip);
	return pAction;
}

int CActions::GetIndex(CXTPControlAction* pAction)
{
	for (int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		if (GetAt(nIndex)->GetID() == pAction->GetID())
			return nIndex;
	}

	return -1;
}

int CActions::GetIndex(UINT nID)
{
	for (int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		if (GetAt(nIndex)->GetID() == nID)
			return nIndex;
	}

	return -1;
}

void CActions::DeleteAction(CXTPControlAction* pAction)
{
	for (int i = 0; i < GetCount(); i++)
	{
		if (GetAt(i) == pAction)
		{
			m_arrActions.RemoveAt(i);

			pAction->InternalRelease();
		}
	}
}

void CActions::ReplaceActionId(CXTPControlAction* pAction, int nID)
{
	if (FindAction(nID) != 0)
		return;

	for (int i = 0; i < GetCount(); i++)
	{
		if (GetAt(i) == pAction)
		{
			m_arrActions.RemoveAt(i);

			SetActionId(pAction, nID);			
			Insert(pAction);
		}
	}
}

