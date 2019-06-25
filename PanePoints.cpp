// PanePoints.cpp: implementation of the CPanePoints class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RibbonReport.h"
#include "MainFrm.h"

#include "PanePoints.h"
#include "PropertyItemFlags.h"
#include "RibbonReportView.h"
#include "Report/PropertiesView.h"


//////////////////////////////////////////////////////////////////////////
//

CPanePoints::CPanePoints()
{
	m_pActions = new CActions();

	m_pIcons = new CXTPImageManager();

	m_pDragAction = NULL;
	m_bTreeExpand = TRUE;

	m_pResourceManager = new CResourceManager;
}

CPanePoints::~CPanePoints()
{
	m_pActions->InternalRelease();

	delete m_pIcons;

	delete m_pResourceManager;
}

BEGIN_MESSAGE_MAP(CPanePoints, CPaneHolder)
	//{{AFX_MSG_MAP(CPanePoints)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TVN_SELCHANGED, AFX_IDW_PANE_FIRST, OnTreeSelChange)
	ON_NOTIFY(NM_SETFOCUS, AFX_IDW_PANE_FIRST, OnTreeSetFocus)
	ON_NOTIFY(NM_RCLICK, AFX_IDW_PANE_FIRST, OnTreeRightClick)

	ON_NOTIFY(TVN_BEGINLABELEDIT, AFX_IDW_PANE_FIRST, OnTreeBeginLabelEdit)
	ON_NOTIFY(TVN_ENDLABELEDIT, AFX_IDW_PANE_FIRST, OnTreeEndLabelEdit)
	ON_NOTIFY(TVN_BEGINDRAG, AFX_IDW_PANE_FIRST, OnTreeBeginDrag)
	ON_NOTIFY(NM_CUSTOMDRAW, AFX_IDW_PANE_FIRST, OnTreeCustomDraw)

	ON_COMMAND(ID_PANE_RIBBON_NEW, OnRibbonNew)
	ON_COMMAND(ID_PANE_RIBBON_OPEN, OnRibbonOpen)
	ON_COMMAND(ID_PANE_RIBBON_SAVE, OnRibbonSave)
	ON_COMMAND(ID_PANE_RIBBON_EXPAND, OnRibbonExpand)
	ON_COMMAND(ID_PANE_RIBBON_COLLAPSE, OnRibbonCollapse)
	ON_COMMAND(ID_PANE_RIBBON_INIT, OnRibbonInit)
	ON_UPDATE_COMMAND_UI(ID_PANE_RIBBON_EXPAND, OnUpdateRibbonExpand)
	ON_UPDATE_COMMAND_UI(ID_PANE_RIBBON_COLLAPSE, OnUpdateRibbonCollapse)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPaneProperties message handlers

int CPanePoints::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPaneHolder::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndToolBar.CreateToolBar(WS_TABSTOP|WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS, this))
		return -1;

	if(!m_wndToolBar.LoadToolBar(IDR_PANE_RIBBON))
		return -1;

	if (!m_wndTreeCtrl.Create(WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE|TVS_HASLINES|
		TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS|TVS_EDITLABELS, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	LoadPointAction();

	return 0;
}

void CPanePoints::OnDestroy()
{
	if (!m_strFileName.IsEmpty())
	{
		SaveToFile(m_strFileName);
	}

	CPaneHolder::OnDestroy();
}

void CPanePoints::OnSize(UINT nType, int cx, int cy)
{
	CPaneHolder::OnSize(nType, cx, cy);

	CSize sz(0);
	if (m_wndToolBar.GetSafeHwnd())
	{
		sz = m_wndToolBar.CalcDockingLayout(cx, /*LM_HIDEWRAP|*/ LM_HORZDOCK|LM_HORZ | LM_COMMIT);

		m_wndToolBar.MoveWindow(0, 0, cx, sz.cy);
		m_wndToolBar.Invalidate(FALSE);
	}

	if (m_wndTreeCtrl.GetSafeHwnd())
	{
		m_wndTreeCtrl.MoveWindow(0, sz.cy, cx, cy - sz.cy);
	}
}

void CPanePoints::OnSetFocus(CWnd*)
{
	m_wndTreeCtrl.SetFocus();
}

void CPanePoints::OnTreeSelChange(NMHDR* pNMHDR, LRESULT*)
{  
	NMTREEVIEW* pNMTreeView = (NMTREEVIEW *)pNMHDR;

	RefreshItem(pNMTreeView->itemNew.hItem);

	RefreshResourceManager();

	CXTPControlAction* pAction = (CXTPControlAction*)m_wndTreeCtrl.GetItemData(pNMTreeView->itemNew.hItem);
	CString strCaption = pAction->GetCaption();
	CString strCategory = pAction->GetCategory();
	CString strDescription = pAction->GetDescription();
	CString* tag = (CString*)pAction->GetTag();

	if (m_wndTreeCtrl.GetItemType(pNMTreeView->itemNew.hItem) == CPaneTreeCtrl::treeCategory)
	{
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		if (pMainFrame->m_pRibbonReportView && pMainFrame->m_pSearchDatapart && pMainFrame->m_pItemsSearchdatapart)
		{
			CString str = m_wndTreeCtrl.GetItemText(pNMTreeView->itemNew.hItem);
			for(int i=0; i<pMainFrame->m_pItemsSearchdatapart->GetItemCount(); i++)
			{
				CXTPControlGalleryItem* pCalleryItem = pMainFrame->m_pItemsSearchdatapart->GetItem(i);
				if(pCalleryItem)
				{
					CString* str = (CString*)(DWORD_PTR)pCalleryItem->GetData();
					if(*tag == *str)
					{
						pMainFrame->m_pSearchDatapart->SetCurSel(i);

						CString sql;
						sql.Format(_T("工点编号='%s'"),*tag);

						//m_pRibbonReportView->m_wndFilterEdit.SetText(*tag);

						CString table = pMainFrame->m_pRibbonReportView->IsView() ? pMainFrame->m_pRibbonReportView->GetView(): pMainFrame->m_pRibbonReportView->GetTable();
						pMainFrame->m_pRibbonReportView->DataBindRecordset(GetRecordset(table,sql), FALSE);
						pMainFrame->m_pRibbonReportView->SetRecordItemControl();
						pMainFrame->m_pRibbonReportView->SetRecordItemEditable();
					}
				}
			}
		}
	}

	if (m_wndTreeCtrl.GetItemType(pNMTreeView->itemNew.hItem) == CPaneTreeCtrl::treeItem)
	{
		CString filePath;
		filePath.Format(_T("%s\\Excel\\%s\\%s\\"), GetCurPath(), strDescription, strCategory);
		CString fileMajor;
		fileMajor.Format(_T("%s\\Excel\\%s\\"), GetCurPath(), strDescription);
		CString fileName = strCaption+_T(".xls");
		CString fileTpPath = GetCurPath()+_T("\\Excel\\");
		CString fileTpName = _T("模板.xls");
		if(!FileExist(filePath+fileName))
		{
			BOOL ret;
			msgAsk(_T("当前文件不存在，是否创建？"), ret);
			if(ret)
			{
				if(!DirExist(fileMajor)) ModifyDir(fileMajor);
				if(!DirExist(filePath)) ModifyDir(filePath);

				CXTPExcelUtil excel;
				excel.InitExcel();
				excel.CloseAlert();
				excel.ShowExcel(FALSE);

				excel.CreateExcel(fileName,filePath, fileTpName, fileTpPath);
				int cnt = excel.GetSheetCount();
				excel.MoveSheetLast(1);
				for(int i=0; i<cnt-1;i++) excel.DeleteSheet(1);
				excel.Save();
				excel.CloseExcel();
				excel.ReleaseExcel();
				msgInf(_T("文件创建成功！"));
			}
		}
		else
		{
			ShellExecuteOpen(filePath+fileName);
		}
	}
}

void CPanePoints::OnTreeSetFocus(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	HTREEITEM hItem = m_wndTreeCtrl.GetSelectedItem();
	if (hItem) RefreshItem(hItem);

}

void CPanePoints::OnTreeRightClick(NMHDR* /*pNMHDR*/, LRESULT*)
{
	CPoint point, ptScreen;
	::GetCursorPos(&ptScreen);
	
	point = ptScreen;
	m_wndTreeCtrl.ScreenToClient(&point);
	
	TV_HITTESTINFO tvhti;
	tvhti.pt = point;
	m_wndTreeCtrl.HitTest(&tvhti);
	
	if ((tvhti.flags & (TVHT_ONITEMLABEL | TVHT_ONITEMICON)) && (tvhti.hItem != NULL))
	{
		CPaneTreeCtrl::TreeType type = m_wndTreeCtrl.GetItemType(tvhti.hItem);

		if (type == CPaneTreeCtrl::treeRoot)
		{
			CMenu menu;
			menu.LoadMenu(IDR_MENU_TREEPANE);

			UINT nCmd = TrackPopupMenu(menu.GetSubMenu(0)->GetSafeHmenu(), TPM_RETURNCMD, ptScreen.x, ptScreen.y, 0, m_hWnd, 0);

			if (nCmd == ID_PANE_TREE_ROOT_INSERT)
			{
				CString strCaption;
				CPaneTreeCtrl::m_nItemCount = 0;
				m_wndTreeCtrl.GetChildItemCount(m_wndTreeCtrl.GetRootItem());
				strCaption.Format(_T("Category%i"), CPaneTreeCtrl::m_nItemCount);

				HTREEITEM hItem = m_wndTreeCtrl.InsertItem(strCaption, 0, 0, tvhti.hItem);
				m_wndTreeCtrl.SelectItem(hItem);
			}
			
			return;
		}

		if (type == CPaneTreeCtrl::treeCategory)
		{
			CMenu menu;
			menu.LoadMenu(IDR_MENU_TREEPANE);

			UINT nCmd = TrackPopupMenu(menu.GetSubMenu(1)->GetSafeHmenu(), TPM_RETURNCMD, ptScreen.x, ptScreen.y, 0, m_hWnd, 0);

			if (nCmd == ID_PANE_TREE_ITEM_ADD)
			{
				int nID = FindLastId();
				CXTPControlAction* pAction = m_pActions->Add(nID);
				
				CString strCaption;
				strCaption.Format(_T("Action%i"), nID);
				pAction->SetCaption(strCaption);
				pAction->SetKey(strCaption);
				pAction->SetCategory(GetCategoryName(m_wndTreeCtrl.GetItemText(tvhti.hItem)));

				HTREEITEM hItem = m_wndTreeCtrl.InsertItem(strCaption, 1, 1, tvhti.hItem);
				m_wndTreeCtrl.SetItemData(hItem, (DWORD_PTR)pAction);

				m_wndTreeCtrl.SelectItem(hItem);
			}

			if (nCmd == ID_PANE_TREE_CATEGORY_DEL)
			{
				m_wndTreeCtrl.DeleteItem(tvhti.hItem);
			}
			
			return;
		}

		if (type == CPaneTreeCtrl::treeItem)
		{
			CMenu menu;
			menu.LoadMenu(IDR_MENU_TREEPANE);

			UINT nCmd = TrackPopupMenu(menu.GetSubMenu(2)->GetSafeHmenu(), TPM_RETURNCMD, ptScreen.x, ptScreen.y, 0, m_hWnd, 0);

			if (nCmd == ID_PANE_TREE_ITEM_DEL)
			{
				CXTPControlAction* pAction = (CXTPControlAction*)m_wndTreeCtrl.GetItemData(tvhti.hItem);

				m_wndTreeCtrl.DeleteItem(tvhti.hItem);
				m_pActions->DeleteAction(pAction);
			}
			
			return;
		}
	}	
}

void CPanePoints::OnTreeBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVDISPINFO ptvdi = (LPNMTVDISPINFO)pNMHDR; 

	CPaneTreeCtrl::TreeType type = m_wndTreeCtrl.GetItemType(ptvdi->item.hItem);
	
	if (type == CPaneTreeCtrl::treeRoot)
	{
		*pResult = 1;
	}
	else if (type == CPaneTreeCtrl::treeCategory)
	{
		if (GetCategoryName(m_wndTreeCtrl.GetItemText(ptvdi->item.hItem)).IsEmpty())
		{
			*pResult = 1;
		}
	}

}

void CPanePoints::OnTreeEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVDISPINFO ptvdi = (LPNMTVDISPINFO)pNMHDR; 

	if (!ptvdi->item.pszText || ptvdi->item.pszText[0] == 0)
	{
		*pResult = FALSE;
		return;
	}

	*pResult = TRUE;

	CPaneTreeCtrl::TreeType type = m_wndTreeCtrl.GetItemType(ptvdi->item.hItem);
	
	if (type == CPaneTreeCtrl::treeItem)
	{
		CXTPControlAction* pAction = (CXTPControlAction*)m_wndTreeCtrl.GetItemData(ptvdi->item.hItem);
		if (!pAction)
		{
			*pResult = FALSE;
			return;
		}
		
		pAction->SetCaption(ptvdi->item.pszText);
		RefreshItem(ptvdi->item.hItem);

	}
	else if (type == CPaneTreeCtrl::treeCategory)
	{
		if (GetCategoryName(ptvdi->item.pszText).IsEmpty())
		{
			*pResult = FALSE;
			return;
		}

		HTREEITEM hItemChild = m_wndTreeCtrl.GetChildItem(ptvdi->item.hItem);
		while (hItemChild)
		{
			CXTPControlAction* pAction = (CXTPControlAction*)m_wndTreeCtrl.GetItemData(hItemChild);

			if (pAction)
			{
				pAction->SetCategory(ptvdi->item.pszText);
			}

			hItemChild = m_wndTreeCtrl.GetNextSiblingItem(hItemChild);
		}
	}

}

void CPanePoints::OnTreeCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTVCUSTOMDRAW* lpLVCD = reinterpret_cast<NMTVCUSTOMDRAW*>(pNMHDR);
	*pResult = CDRF_DODEFAULT;

	switch (lpLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_PREPAINT | CDDS_ITEM:
		{
			HTREEITEM hItem = (HTREEITEM)lpLVCD->nmcd.dwItemSpec;
			if (m_wndTreeCtrl.GetItemType(hItem) != CPaneTreeCtrl::treeItem)
				return;
			
			CXTPControlAction* pAction = (CXTPControlAction*)m_wndTreeCtrl.GetItemData(hItem);
			if (!pAction)
				return;

			*pResult |= CDRF_NOTIFYPOSTPAINT;
			
			break;
		}

	case CDDS_POSTPAINT | CDDS_ITEM:
	case CDDS_POSTPAINT | CDDS_ITEM  | CDDS_SUBITEM:
		{
			HTREEITEM hItem = (HTREEITEM)lpLVCD->nmcd.dwItemSpec;

			CXTPControlAction* pAction = (CXTPControlAction*)m_wndTreeCtrl.GetItemData(hItem);
			if (!pAction)
				return;
			
			CSize sz(16, 16);
			
			CXTPImageManagerIcon* pIcon = m_pIcons->GetImage(pAction->GetIconId(), 16);
			
			if (!pIcon)
				return;

			CRect rc(0, 0, 0, 0);
			m_wndTreeCtrl.GetItemRect(hItem, rc, TRUE);
			
			if (!rc.IsRectEmpty())
			{
				XTPImageState imageState = xtpImageNormal;

				CPoint pt(rc.left - 16 - 2, (rc.top + rc.bottom - sz.cy) / 2);
				pIcon->Draw(CDC::FromHandle(lpLVCD->nmcd.hdc), pt, pIcon->GetIcon(imageState), sz);
			}
			
			break;
		}
	}

}

void CPanePoints::OnTreeBeginDrag(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)pNMHDR;
	
	if (m_wndTreeCtrl.GetItemType(pnmtv->itemNew.hItem) != CPaneTreeCtrl::treeItem)
		return;

	m_pDragAction = (CXTPControlAction*)m_wndTreeCtrl.GetItemData(pnmtv->itemNew.hItem);
	if (!m_pDragAction)
		return;

	COleDataSource dataSource;

	HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, 1);

	dataSource.CacheGlobalData(CPaneTreeCtrl::m_cfItem, hGlobal);


	dataSource.DoDragDrop(DROPEFFECT_COPY);

	m_pDragAction = NULL;
}

void CPanePoints::RefreshItem(HTREEITEM /*hItem*/)
{
	CMainFrame* pMaainFrame = (CMainFrame*)AfxGetMainWnd();

	if (pMaainFrame->m_pActiveCommandBars)
		pMaainFrame->m_pActiveCommandBars->SetDragControl(NULL);

	pMaainFrame->m_pActivePane = this;
	pMaainFrame->m_wndPaneProperties.Refresh(this);
}

void CPanePoints::RefreshResourceManager()
{
	m_pResourceManager->FreeAll();

	for (int i = 0; i < m_pActions->GetCount(); i++)
	{
		m_pResourceManager->Set(m_pActions->GetAt(i)->GetID(), m_pActions->GetAt(i)->GetKey());
	}
}

int CPanePoints::FindLastId()
{
	int nCount = m_pActions->GetCount();
	int nId = ID_NEXT_COMMAND_VALUE;

	for (int i = 0; i < nCount; i++)
	{
		if (m_pActions->GetAt(i)->GetID() >= nId)
		{
			nId = m_pActions->GetAt(i)->GetID() + 1;
		}
	}

	return nId;
}

CString CPanePoints::GetCategoryName(const CString& str)
{
	if (str == _T("(None)"))
		return _T("");

	return str;
}

CObject* CPanePoints::RefreshPropertyGrid(CXTPPropertyGrid* pPropertyGrid) 
{
	HTREEITEM hItem = m_wndTreeCtrl.GetSelectedItem();
	if (!hItem) 
		return NULL;

	if (m_wndTreeCtrl.GetItemType(hItem) != CPaneTreeCtrl::treeItem)
		return NULL;

	CXTPControlAction* pAction = (CXTPControlAction*)m_wndTreeCtrl.GetItemData(hItem);
	if (!pAction)
		return NULL;

	CXTPPropertyGridItem* pCategory = pPropertyGrid->AddCategory(ID_GRID_CATEGORY_APPEARANCE);

	pCategory->AddChildItem(new CXTPPropertyGridItemNumber(ID_GRID_ITEM_CONTROL_ID, pAction->GetID()));

	pCategory->AddChildItem(new CXTPPropertyGridItem(ID_GRID_ITEM_ACTION_KEY, pAction->GetKey()));

	pCategory->AddChildItem(new CXTPPropertyGridItem(ID_GRID_ITEM_ACTION_CAPTION, pAction->GetCaption()));

	pCategory->AddChildItem(new CXTPPropertyGridItem(ID_GRID_ITEM_ACTION_DESCRIPTION, pAction->GetDescription()));
	pCategory->AddChildItem(new CXTPPropertyGridItem(ID_GRID_ITEM_ACTION_TOOLTIP, pAction->GetTooltip()));

	CXTPPropertyGridItem* pItemControlCategory = pCategory->AddChildItem(new CXTPPropertyGridItem(ID_GRID_ITEM_ACTION_CATEGORY, pAction->GetCategory()));
	pItemControlCategory->SetFlags(xtpGridItemHasComboButton | xtpGridItemHasEdit);

	HTREEITEM hItemRoot = m_wndTreeCtrl.GetRootItem();
	HTREEITEM hItemChild = m_wndTreeCtrl.GetChildItem(hItemRoot);
	while (hItemChild)
	{
		CString strItem = m_wndTreeCtrl.GetItemText(hItemChild);

		if (!GetCategoryName(strItem).IsEmpty())
		{
			pItemControlCategory->GetConstraints()->AddConstraint(strItem);
		}

		hItemChild = m_wndTreeCtrl.GetNextSiblingItem(hItemChild);
	}

	pCategory->Expand();

	return pAction;
}

BOOL CPanePoints::OnPropertyGridValueChanged(CObject* pActiveObject, CXTPPropertyGridItem* pItem)
{
	CXTPControlAction* pAction = DYNAMIC_DOWNCAST(CXTPControlAction, pActiveObject);
	if (!pAction)
		return FALSE;

	HTREEITEM hItem = m_wndTreeCtrl.GetSelectedItem();
	if (m_wndTreeCtrl.GetItemData(hItem) != (DWORD_PTR)pAction)
		return FALSE;

	switch (pItem->GetID())
	{
	case ID_GRID_ITEM_ACTION_KEY:	
		pAction->SetKey(pItem->GetValue());
		return TRUE;

	case ID_GRID_ITEM_ACTION_CAPTION:
		pAction->SetCaption(pItem->GetValue());
		m_wndTreeCtrl.SetItemText(hItem, pAction->GetCaption());	
		return TRUE;

	case ID_GRID_ITEM_CONTROL_ID:
		m_pActions->ReplaceActionId(pAction, GetNumberValue(pItem));
		return TRUE;

	case ID_GRID_ITEM_ACTION_DESCRIPTION:
		pAction->SetDescription(pItem->GetValue());
		return TRUE;

	case ID_GRID_ITEM_ACTION_TOOLTIP:
		pAction->SetTooltip(pItem->GetValue());
		return TRUE;

	case ID_GRID_ITEM_ACTION_CATEGORY:
		{
			pAction->SetCategory(pItem->GetValue());

			HTREEITEM hItemRoot = m_wndTreeCtrl.GetRootItem();
			HTREEITEM hItemChild = m_wndTreeCtrl.GetChildItem(hItemRoot);

			CString strCategory = pAction->GetCategory();
			if (strCategory.IsEmpty()) strCategory = _T("(None)");

			while (hItemChild)
			{
				CString strItem = m_wndTreeCtrl.GetItemText(hItemChild);

				if (GetCategoryName(strItem) == strCategory)
				{
					break;
				}			
				hItemChild = m_wndTreeCtrl.GetNextSiblingItem(hItemChild);
			}

			if (!hItemChild)
			{
				hItemChild = m_wndTreeCtrl.InsertItem(strCategory, 0, 0, hItemRoot);
			}

			m_wndTreeCtrl.DeleteItem(hItem);

			HTREEITEM hNewItem = m_wndTreeCtrl.InsertItem(pAction->GetCaption(), 0, 0, hItemChild);
			m_wndTreeCtrl.SetItemData(hNewItem, (DWORD_PTR)pAction);

			m_wndTreeCtrl.SelectItem(hNewItem);			

		}
		return TRUE;

	}

	return FALSE;
}

void CPanePoints::OnRibbonNew()
{
	if (AfxMessageBox(_T("Are you sure you want remove all actions?"), MB_YESNO) != IDYES)
		return;

	m_wndTreeCtrl.DeleteAllItems();
	m_pActions->RemoveAll();

	HTREEITEM hItemRibbon = m_wndTreeCtrl.GetRootItem();
	if (!hItemRibbon)
		hItemRibbon = m_wndTreeCtrl.InsertItem(_T("PointList"), 0, 0);

	m_strFileName.Empty();
}

void CPanePoints::LoadPointAction()
{
	CString tb_point = g_TablePrix+_T("[tb_points]");
	CString fids = theCon.GetFields(tb_point,0,2);
	CXTPADOData adoName,adoStat;
	long rs = theCon.GetRecordCount(tb_point);
	GetRecord(adoName,_T("tb_points"),fids);
	_variant_t** name = (_variant_t**)adoName.vals[0];
	fids = theCon.GetFields(tb_point,4,7);
	GetRecord(adoStat,_T("tb_points"),fids);
	_variant_t** stat = (_variant_t**)adoStat.vals[0];


	m_pActions->RemoveAll();

	HTREEITEM hItemPointList = m_wndTreeCtrl.GetRootItem();
	if (!hItemPointList)
		hItemPointList = m_wndTreeCtrl.InsertItem(_T("PointList"), 0, 0);


	CStringArray strCaptions;
	CreateArray(strCaptions, _T("路基;桥梁;隧道;轨道;涵洞"));
	HTREEITEM hItemPoints[5];

	for(int i=0; i<5; i++)
	{
		hItemPoints[i] = m_wndTreeCtrl.InsertItem(strCaptions[i], 0, 0, hItemPointList);
		CString strKeyAction = GetDefineID(ID_PANE_POINTS_ACTION+i);
		CXTPControlAction* pAction = m_pActions->AddAction(strCaptions[i], ID_PANE_POINTS_ACTION+i, strKeyAction, _T("PointList"));
		pAction->SetDescription(_T("专业"));
		CString* tag = new CString;
		*tag = strCaptions[i];
		pAction->SetTag((DWORD_PTR)tag);
		m_wndTreeCtrl.SetItemData(hItemPoints[i], (DWORD_PTR)pAction);
	}

	for(int i=0; i<rs;i++)
	{
		CString strCode = name[i][0].bstrVal;
		CString strName = name[i][1].bstrVal;
		CString strType = name[i][2].bstrVal;
		CString strBStat(stat[i][0].bstrVal);
		CString strEStat(stat[i][1].bstrVal);
		CString strCStat(stat[i][2].bstrVal);
		double fLength = stat[i][3].dblVal;
		CString str;
		CString* tag = new CString;
		*tag = strCode;

#ifdef _UNICODE
		str = strType.Right(1);
#else
		str = strType.Right(2);
#endif // _UNICODE

		int code = _tstoi(strCode.Mid(2,3));
		if(str == _T("基"))
		{
			str.Format(_T("%s %s～%s"),strName, strBStat,strEStat);
			HTREEITEM hItemPoint = m_wndTreeCtrl.InsertItem(str, 0, 0, hItemPoints[0]);
			CString strKeyAction = GetDefineID(ID_PANE_POINTS_ACTION+code*10);
			CXTPControlAction* pAction = m_pActions->AddAction(str, ID_PANE_POINTS_ACTION+code*10, strKeyAction, strCaptions[0]);
			pAction->SetDescription(_T("工点"));
			pAction->SetTag((DWORD_PTR)tag);
			m_wndTreeCtrl.SetItemData(hItemPoint, (DWORD_PTR)pAction);

			CStringArray strActions;
			CreateArray(strActions, _T("土石方数量;主体数量;附属数量;预制数量;项外数量;计价数量"));
			HTREEITEM hItemActions[6];
			for(int j=0; j<6; j++)
			{
				hItemActions[j] = m_wndTreeCtrl.InsertItem(strActions[j], 2, 3, hItemPoint);
				strKeyAction = GetDefineID(ID_PANE_POINTS_ACTION+code*10+j+1);
				pAction = m_pActions->AddAction(strActions[j], ID_PANE_POINTS_ACTION+code*10+j+1, strKeyAction, str);
				pAction->SetDescription(strCaptions[0]);
				pAction->SetTag((DWORD_PTR)tag);
				m_wndTreeCtrl.SetItemData(hItemActions[j], (DWORD_PTR)pAction);
			}
			m_wndTreeCtrl.Expand(hItemPoint, TVE_EXPAND);

			hItemPoint = m_wndTreeCtrl.InsertItem(str, 0, 0, hItemPoints[3]);
			strKeyAction = GetDefineID(ID_PANE_POINTS_ACTION+code*10+1000);
			pAction = m_pActions->AddAction(str, ID_PANE_POINTS_ACTION+code*10+1000, strKeyAction, strCaptions[3]);
			pAction->SetDescription(_T("工点"));
			pAction->SetTag((DWORD_PTR)tag);
			m_wndTreeCtrl.SetItemData(hItemPoint, (DWORD_PTR)pAction);

			strActions.RemoveAll();
			CreateArray(strActions, _T("主体数量;附属数量;项外数量;计价数量"));
			for(int j=0; j<4; j++)
			{
				hItemActions[j] = m_wndTreeCtrl.InsertItem(strActions[j], 2, 3, hItemPoint);
				strKeyAction = GetDefineID(ID_PANE_POINTS_ACTION+code*10+j+1001);
				pAction = m_pActions->AddAction(strActions[j], ID_PANE_POINTS_ACTION+code*10+j+1001, strKeyAction, str);
				pAction->SetDescription(strCaptions[3]);
				pAction->SetTag((DWORD_PTR)tag);
				m_wndTreeCtrl.SetItemData(hItemActions[j], (DWORD_PTR)pAction);
			}
			m_wndTreeCtrl.Expand(hItemPoint, TVE_EXPAND);
		}

		if(str == _T("桥"))
		{
			str.Format(_T("%s  %s  L=%.3fm"),strName,strCStat,fLength);
			HTREEITEM hItemPoint = m_wndTreeCtrl.InsertItem(str, 0, 0, hItemPoints[1]);
			CString strKeyAction = GetDefineID(ID_PANE_POINTS_ACTION+code*10);
			CXTPControlAction* pAction = m_pActions->AddAction(str, ID_PANE_POINTS_ACTION+code*10, strKeyAction, strCaptions[1]);
			pAction->SetDescription(_T("工点"));
			pAction->SetTag((DWORD_PTR)tag);
			m_wndTreeCtrl.SetItemData(hItemPoint, (DWORD_PTR)pAction);

			CStringArray strActions;
			CreateArray(strActions, _T("主体数量;附属数量;项外数量;计价数量"));
			HTREEITEM hItemActions[4];
			for(int j=0; j<4; j++)
			{
				hItemActions[j] = m_wndTreeCtrl.InsertItem(strActions[j], 2, 3, hItemPoint);
				strKeyAction = GetDefineID(ID_PANE_POINTS_ACTION+code*10+j+1);
				pAction = m_pActions->AddAction(strActions[j], ID_PANE_POINTS_ACTION+code*10+j+1, strKeyAction, str);
				pAction->SetDescription(strCaptions[1]);
				pAction->SetTag((DWORD_PTR)tag);
				m_wndTreeCtrl.SetItemData(hItemActions[j], (DWORD_PTR)pAction);
			}
			m_wndTreeCtrl.Expand(hItemPoint, TVE_EXPAND);


			hItemPoint = m_wndTreeCtrl.InsertItem(str, 0, 0, hItemPoints[3]);
			strKeyAction = GetDefineID(ID_PANE_POINTS_ACTION+code*10+1000);
			pAction = m_pActions->AddAction(str, ID_PANE_POINTS_ACTION+code*10+1000, strKeyAction, strCaptions[3]);
			pAction->SetDescription(_T("工点"));
			pAction->SetTag((DWORD_PTR)tag);
			m_wndTreeCtrl.SetItemData(hItemPoint, (DWORD_PTR)pAction);

			strActions.RemoveAll();
			CreateArray(strActions, _T("主体数量;附属数量;项外数量;计价数量"));
			for(int j=0; j<4; j++)
			{
				hItemActions[j] = m_wndTreeCtrl.InsertItem(strActions[j], 2, 3, hItemPoint);
				strKeyAction = GetDefineID(ID_PANE_POINTS_ACTION+code*10+j+1001);
				pAction = m_pActions->AddAction(strActions[j], ID_PANE_POINTS_ACTION+code*10+j+1001, strKeyAction, str);
				pAction->SetDescription(strCaptions[3]);
				pAction->SetTag((DWORD_PTR)tag);
				m_wndTreeCtrl.SetItemData(hItemActions[j], (DWORD_PTR)pAction);
			}
			m_wndTreeCtrl.Expand(hItemPoint, TVE_EXPAND);
		}

		if(str == _T("道"))
		{
			str.Format(_T("%s  %s  L=%.3fm"),strName,strCStat,fLength);
			HTREEITEM hItemPoint = m_wndTreeCtrl.InsertItem(str, 0, 0, hItemPoints[2]);
			CString strKeyAction = GetDefineID(ID_PANE_POINTS_ACTION+code*10);
			CXTPControlAction* pAction = m_pActions->AddAction(str, ID_PANE_POINTS_ACTION+code*10, strKeyAction, strCaptions[2]);
			pAction->SetDescription(_T("工点"));
			pAction->SetTag((DWORD_PTR)tag);
			m_wndTreeCtrl.SetItemData(hItemPoint, (DWORD_PTR)pAction);

			CStringArray strActions;
			CreateArray(strActions, _T("主体数量;附属数量;项外数量;计价数量"));
			HTREEITEM hItemActions[4];
			for(int j=0; j<4; j++)
			{
				hItemActions[j] = m_wndTreeCtrl.InsertItem(strActions[j], 2, 3, hItemPoint);
				strKeyAction = GetDefineID(ID_PANE_POINTS_ACTION+code*10+j+1);
				pAction = m_pActions->AddAction(strActions[j], ID_PANE_POINTS_ACTION+code*10+j+1, strKeyAction, str);
				pAction->SetDescription(strCaptions[2]);
				pAction->SetTag((DWORD_PTR)tag);
				m_wndTreeCtrl.SetItemData(hItemActions[j], (DWORD_PTR)pAction);
			}
			m_wndTreeCtrl.Expand(hItemPoint, TVE_EXPAND);


			hItemPoint = m_wndTreeCtrl.InsertItem(str, 0, 0, hItemPoints[3]);
			strKeyAction = GetDefineID(ID_PANE_POINTS_ACTION+code*10+1000);
			pAction = m_pActions->AddAction(str, ID_PANE_POINTS_ACTION+code*10+1000, strKeyAction, strCaptions[3]);
			pAction->SetDescription(_T("工点"));
			pAction->SetTag((DWORD_PTR)tag);
			m_wndTreeCtrl.SetItemData(hItemPoint, (DWORD_PTR)pAction);

			strActions.RemoveAll();
			CreateArray(strActions, _T("主体数量;附属数量;项外数量;计价数量"));
			for(int j=0; j<4; j++)
			{
				hItemActions[j] = m_wndTreeCtrl.InsertItem(strActions[j], 2, 3, hItemPoint);
				strKeyAction = GetDefineID(ID_PANE_POINTS_ACTION+code*10+j+1001);
				pAction = m_pActions->AddAction(strActions[j], ID_PANE_POINTS_ACTION+code*10+j+1001, strKeyAction, str);
				pAction->SetDescription(strCaptions[3]);
				pAction->SetTag((DWORD_PTR)tag);
				m_wndTreeCtrl.SetItemData(hItemActions[j], (DWORD_PTR)pAction);
			}
			m_wndTreeCtrl.Expand(hItemPoint, TVE_EXPAND);
		}

		if(str == _T("涵"))
		{
			str.Format(_T("%s  %s  L=%.3fm"),strName,strCStat,fLength);
			HTREEITEM hItemPoint = m_wndTreeCtrl.InsertItem(str, 0, 0, hItemPoints[4]);
			CString strKeyAction = GetDefineID(ID_PANE_POINTS_ACTION+code*10);
			CXTPControlAction* pAction = m_pActions->AddAction(str, ID_PANE_POINTS_ACTION+code*10, strKeyAction, strCaptions[4]);
			pAction->SetDescription(_T("工点"));
			pAction->SetTag((DWORD_PTR)tag);
			m_wndTreeCtrl.SetItemData(hItemPoint, (DWORD_PTR)pAction);

			CStringArray strActions;
			CreateArray(strActions, _T("主体数量;附属数量;项外数量;计价数量"));
			HTREEITEM hItemActions[4];
			for(int j=0; j<4; j++)
			{
				hItemActions[j] = m_wndTreeCtrl.InsertItem(strActions[j], 2, 3, hItemPoint);
				strKeyAction = GetDefineID(ID_PANE_POINTS_ACTION+code*10+j+1);
				pAction = m_pActions->AddAction(strActions[j], ID_PANE_POINTS_ACTION+code*10+j+1, strKeyAction, str);
				pAction->SetDescription(strCaptions[4]);
				pAction->SetTag((DWORD_PTR)tag);
				m_wndTreeCtrl.SetItemData(hItemActions[j], (DWORD_PTR)pAction);
			}
			m_wndTreeCtrl.Expand(hItemPoint, TVE_EXPAND);
		}
	}

	m_wndTreeCtrl.Expand(hItemPointList, TVE_EXPAND);
	for(int i=0; i<5; i++) m_wndTreeCtrl.Expand(hItemPoints[i], TVE_EXPAND);
}

void CPanePoints::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsLoading())
	{
		pPX->m_dwData = (DWORD_PTR)m_pActions;
		m_wndTreeCtrl.SetRedraw(FALSE);
		m_wndTreeCtrl.DeleteAllItems();
		m_pActions->RemoveAll();

		HTREEITEM hItemPointList = m_wndTreeCtrl.GetRootItem();
		if (!hItemPointList)
			hItemPointList = m_wndTreeCtrl.InsertItem(_T("PointList"), 0, 0);

		CXTPPropExchangeEnumeratorPtr pEnumeratorMajor(pPX->GetEnumerator(_T("Major")));
		POSITION posEnumMajor = pEnumeratorMajor->GetPosition(m_pActions->GetCount());

		for(int i=0; posEnumMajor != NULL; i++)
		{
			CXTPPropExchangeSection secMajor(pEnumeratorMajor->GetNext(posEnumMajor));
			CString strMajor;
			PX_String(&secMajor, _T("Major"), strMajor);
			int nActionId;
			PX_Int(&secMajor, _T("ActionId"), nActionId);
			int nIconId;
			PX_Int(&secMajor, _T("IconId"), nIconId);
			CString strActionCaption;
			PX_String(&secMajor, _T("Caption"), strActionCaption);
			CString strActionCategory;
			PX_String(&secMajor, _T("Category"), strActionCategory);
			CString strDescription;
			PX_String(&secMajor, _T("DescriptionText"), strDescription);
			CString strActionKey;
			PX_String(&secMajor, _T("Key"), strActionKey);
			CString* tag = new CString;
			PX_String(&secMajor, _T("Tag"), *tag);
			CXTPControlAction* pAction = m_pActions->AddAction(strActionCaption, nActionId, strActionKey, strActionCategory, nIconId);
			pAction->SetTag((DWORD_PTR)tag);

			HTREEITEM hItemMajor = m_wndTreeCtrl.InsertItem(strMajor, 0, 0, hItemPointList);
			m_wndTreeCtrl.SetItemData(hItemMajor, (DWORD_PTR)pAction);

			CXTPPropExchangeEnumeratorPtr pEnumeratorPoint(secMajor->GetEnumerator(_T("Point")));
			POSITION posEnumPoint = pEnumeratorPoint->GetPosition(m_pActions->GetCount());

			for(int j=0; posEnumPoint != NULL; j++)
			{
				CXTPPropExchangeSection secPoint(pEnumeratorPoint->GetNext(posEnumPoint));
				CString strPoint;
				PX_String(&secPoint, _T("Point"), strPoint);
				int nActionId;
				PX_Int(&secPoint, _T("ActionId"), nActionId);
				int nIconId;
				PX_Int(&secPoint, _T("IconId"), nIconId);
				CString strActionCaption;
				PX_String(&secPoint, _T("Caption"), strActionCaption);
				CString strActionCategory;
				PX_String(&secPoint, _T("Category"), strActionCategory);
				CString strDescription;
				PX_String(&secPoint, _T("DescriptionText"), strDescription);
				CString strActionKey;
				PX_String(&secPoint, _T("Key"), strActionKey);
				CString* tag = new CString;
				PX_String(&secPoint, _T("Tag"), *tag);
				CXTPControlAction* pAction = m_pActions->AddAction(strActionCaption, nActionId, strActionKey, strActionCategory, nIconId);
				pAction->SetTag((DWORD_PTR)tag);

				HTREEITEM hItemPoint = m_wndTreeCtrl.InsertItem(strPoint, 0, 0, hItemMajor);
				m_wndTreeCtrl.SetItemData(hItemPoint, (DWORD_PTR)pAction);

				CXTPPropExchangeEnumeratorPtr pEnumeratorAction(secPoint->GetEnumerator(_T("Action")));
				POSITION posEnumAction = pEnumeratorAction->GetPosition(m_pActions->GetCount());

				for(int k=0; posEnumAction != NULL; k++)
				{
					CXTPPropExchangeSection secAction(pEnumeratorAction->GetNext(posEnumAction));
					CString strAction;
					PX_String(&secAction, _T("Action"), strAction);
					int nActionId;
					PX_Int(&secAction, _T("ActionId"), nActionId);
					int nIconId;
					PX_Int(&secAction, _T("IconId"), nIconId);
					CString strActionCaption;
					PX_String(&secAction, _T("Caption"), strActionCaption);
					CString strActionCategory;
					PX_String(&secAction, _T("Category"), strActionCategory);
					CString strDescription;
					PX_String(&secAction, _T("DescriptionText"), strDescription);
					CString strActionKey;
					PX_String(&secAction, _T("Key"), strActionKey);
					CString* tag = new CString;
					PX_String(&secAction, _T("Tag"), *tag);
					CXTPControlAction* pAction = m_pActions->AddAction(strActionCaption, nActionId, strActionKey, strActionCategory, nIconId, strDescription);
					pAction->SetTag((DWORD_PTR)tag);

					HTREEITEM hItemAction = m_wndTreeCtrl.InsertItem(strAction, 2, 3, hItemPoint);
					m_wndTreeCtrl.SetItemData(hItemAction, (DWORD_PTR)pAction);
				}

				m_wndTreeCtrl.Expand(hItemPoint, TVE_EXPAND);
			}

			m_wndTreeCtrl.Expand(hItemMajor, TVE_EXPAND);
		}

		m_wndTreeCtrl.Expand(hItemPointList, TVE_EXPAND);

		m_wndTreeCtrl.SetRedraw(TRUE);
	}
	else
	{
		HTREEITEM hItemPoints = m_wndTreeCtrl.GetRootItem();
		HTREEITEM hItemMajor = m_wndTreeCtrl.GetChildItem(hItemPoints);

		CXTPPropExchangeEnumeratorPtr pEnumeratorMajor(pPX->GetEnumerator(_T("Major")));
		CPaneTreeCtrl::m_nItemCount = 0;
		m_wndTreeCtrl.GetChildItemCount(hItemPoints);
		POSITION posEnumMajor = pEnumeratorMajor->GetPosition(CPaneTreeCtrl::m_nItemCount);

		for(int i=0; posEnumMajor && hItemMajor; i++)
		{
			CXTPPropExchangeSection secMajor(pEnumeratorMajor->GetNext(posEnumMajor));
			CString strMajor = GetCategoryName(m_wndTreeCtrl.GetItemText(hItemMajor));
			PX_String(&secMajor, _T("Major"), strMajor);
			CXTPControlAction* pAction = (CXTPControlAction*)m_wndTreeCtrl.GetItemData(hItemMajor);
			int nIdAction = pAction->GetID();
			PX_Int(&secMajor, _T("ActionId"), nIdAction);
			PX_Object(&secMajor, pAction, RUNTIME_CLASS(CXTPControlAction));
			CString* tag = (CString*)pAction->GetTag();
			PX_String(&secMajor, _T("Tag"), *tag);
			int nIndexAction = m_pActions->GetIndex(pAction);
			PX_Int(&secMajor, _T("ActionIndex"), nIndexAction);

			HTREEITEM hItemPoint = m_wndTreeCtrl.GetChildItem(hItemMajor);

			CXTPPropExchangeEnumeratorPtr pEnumeratorPoint(secMajor->GetEnumerator(_T("Point")));
			CPaneTreeCtrl::m_nItemCount = 0;
			m_wndTreeCtrl.GetChildItemCount(hItemMajor);
			POSITION posEnumPoint = pEnumeratorPoint->GetPosition(CPaneTreeCtrl::m_nItemCount);

			for(int j=0; posEnumPoint && hItemPoint; j++)
			{
				CXTPPropExchangeSection secPoint(pEnumeratorPoint->GetNext(posEnumPoint));
				CString strPoint = GetCategoryName(m_wndTreeCtrl.GetItemText(hItemPoint));
				PX_String(&secPoint, _T("Point"), strPoint);
				CXTPControlAction* pAction = (CXTPControlAction*)m_wndTreeCtrl.GetItemData(hItemPoint);
				int nIdAction = pAction->GetID();
				PX_Int(&secPoint, _T("ActionId"), nIdAction);
				PX_Object(&secPoint, pAction, RUNTIME_CLASS(CXTPControlAction));
				CString* tag = (CString*)pAction->GetTag();
				PX_String(&secPoint, _T("Tag"), *tag);
				int nIndexAction = m_pActions->GetIndex(pAction);
				PX_Int(&secPoint, _T("ActionIndex"), nIndexAction);

				HTREEITEM hItemAction = m_wndTreeCtrl.GetChildItem(hItemPoint);

				CXTPPropExchangeEnumeratorPtr pEnumeratorAction(secPoint->GetEnumerator(_T("Action")));
				CPaneTreeCtrl::m_nItemCount = 0;
				m_wndTreeCtrl.GetChildItemCount(hItemPoint);
				POSITION posEnumAction = pEnumeratorAction->GetPosition(CPaneTreeCtrl::m_nItemCount);

				for(int k=0; posEnumAction && hItemAction; k++)
				{
					CXTPPropExchangeSection secAction(pEnumeratorAction->GetNext(posEnumAction));
					CString strAction = GetCategoryName(m_wndTreeCtrl.GetItemText(hItemAction));
					PX_String(&secAction, _T("Action"), strAction);
					CXTPControlAction* pAction = (CXTPControlAction*)m_wndTreeCtrl.GetItemData(hItemAction);
					int nIdAction = pAction->GetID();
					PX_Int(&secAction, _T("ActionId"), nIdAction);
					PX_Object(&secAction, pAction, RUNTIME_CLASS(CXTPControlAction));
					CString* tag = (CString*)pAction->GetTag();
					PX_String(&secAction, _T("Tag"), *tag);
					int nIndexAction = m_pActions->GetIndex(pAction);
					PX_Int(&secAction, _T("ActionIndex"), nIndexAction);

					hItemAction = m_wndTreeCtrl.GetNextSiblingItem(hItemAction);
				}

				hItemPoint = m_wndTreeCtrl.GetNextSiblingItem(hItemPoint);
			}

			hItemMajor = m_wndTreeCtrl.GetNextSiblingItem(hItemMajor);
		}
	}
}

BOOL CPanePoints::LoadFromFile(CString strFileName)
{
	CXTPPropExchangeXMLNode px(TRUE, NULL, _T("PointsActions"));
	if (!px.LoadFromFile(strFileName))
		return FALSE;

	CXTPPropExchangeSection secRibbon(px.GetSection(_T("PointList")));
	DoPropExchange(&secRibbon);

	m_strFileName = strFileName;

	return TRUE;

}

void CPanePoints::OnRibbonOpen()
{
	CString strFilter = _T("XML Document (*.xml)|*.xml|All files (*.*)|*.*||");

	CFileDialog fd(TRUE, _T("xml"), NULL, OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, strFilter);
	if (fd.DoModal() != IDOK)
		return;

	if (!LoadFromFile(fd.GetPathName()))
		return;

	AfxGetApp()->WriteProfileString(_T("ActionsPoints"), _T("CurrentFile"), fd.GetPathName());

	msgInf(_T("工点树配置导入成功！"));
}

void CPanePoints::SaveToFile(CString strFileName)
{
	CXTPPropExchangeXMLNode px(FALSE, 0, _T("PointsActions"));

	CXTPPropExchangeSection secRibbon(px.GetSection(_T("PointList")));
	DoPropExchange(&secRibbon);

	px.SaveToFile(strFileName);

	m_strFileName = strFileName;
}

void CPanePoints::OnRibbonSave()
{
	CString strFilter = _T("XML Document (*.xml)|*.xml|All files (*.*)|*.*||");

	CFileDialog fd(FALSE, _T("xml"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter);
	if (fd.DoModal() != IDOK)
		return;

	SaveToFile(fd.GetPathName());

	AfxGetApp()->WriteProfileString(_T("ActionsLibrary"), _T("CurrentFile"), fd.GetPathName());

	msgInf(_T("工点树配置保存成功！"));
}

void CPanePoints::OnRibbonExpand()
{
	m_wndTreeCtrl.ExpandItems();
	m_bTreeExpand = TRUE;
}

void CPanePoints::OnRibbonCollapse()
{
	m_wndTreeCtrl.CollapseItems();
	m_bTreeExpand = FALSE;
}

void CPanePoints::OnRibbonInit()
{
	BOOL ret;
	msgAsk(_T("是否初始化工点列表？"), ret);
	if(ret)
	{
		LoadPointAction();
		msgInf(_T("初始化工点列表成功！"));
	}
}

void CPanePoints::OnUpdateRibbonExpand(CCmdUI* pCmdUI)
{
	CXTPControl* pControl = CXTPControl::FromUI(pCmdUI);

	pCmdUI->Enable(TRUE);

	if (pControl)
		pControl->SetVisible(!m_bTreeExpand);
}

void CPanePoints::OnUpdateRibbonCollapse(CCmdUI* pCmdUI)
{
	CXTPControl* pControl = CXTPControl::FromUI(pCmdUI);

	pCmdUI->Enable(TRUE);

	if (pControl)
		pControl->SetVisible(m_bTreeExpand);
}
