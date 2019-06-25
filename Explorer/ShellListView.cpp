// ShellListView.cpp : implementation file
//

#include "stdafx.h"
#include "RibbonReport.h"
#include "ShellListView.h"
#include "ShellTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShellListView

IMPLEMENT_DYNCREATE(CShellListView, CXTPShellListView)

CShellListView::CShellListView()
{
	m_bSortColor = true;

	COLORREF crSortBack(GetXtremeColor(COLOR_WINDOW));
	crSortBack = RGB(max(0, GetRValue(crSortBack) - 8), max(0, GetGValue(crSortBack) - 8), max(0, GetBValue(crSortBack) - 8));

	SetSortTextColor(GetXtremeColor(COLOR_WINDOWTEXT));
	SetSortBackColor(crSortBack);

	m_bDragging = FALSE;
	m_itemDrag = -1;
}

CShellListView::~CShellListView()
{
}


BEGIN_MESSAGE_MAP(CShellListView, CXTPShellListView)
	//{{AFX_MSG_MAP(CShellListView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_DROPFILES()
	ON_MESSAGE(WM_USER_DROPFILESTOLISTCTRL, OnDropFilesToListCtrl)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnLVNItemChanged)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnLVNEndlabelEdit)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CShellListView diagnostics

#ifdef _DEBUG
void CShellListView::AssertValid() const
{
	CXTPShellListView::AssertValid();
}

void CShellListView::Dump(CDumpContext& dc) const
{
	CXTPShellListView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CShellListView message handlers

BOOL CShellListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CXTPShellListView::PreCreateWindow(cs))
		return FALSE;

	cs.style |= WS_VISIBLE|WS_TABSTOP|LVS_AUTOARRANGE|
		LVS_ICON|LVS_EDITLABELS|LVS_SHOWSELALWAYS;
	cs.dwExStyle |= WS_EX_STATICEDGE|WS_EX_ACCEPTFILES|LVS_EX_FULLROWSELECT;
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CShellListView message handlers

void CShellListView::OnInitialUpdate()
{
	CXTPShellListView::OnInitialUpdate();
	m_pListCtrl->ModifyStyle(LVS_TYPEMASK,LVS_ICON & LVS_TYPEMASK);
}

void CShellListView::OnDropFiles(HDROP hDropInfo)
{
	xtpchar lpszPathName[_MAX_PATH] = {0};
	UINT  nNumOfFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0); //文件的个数
	for ( UINT nIndex=0 ; nIndex< nNumOfFiles; ++nIndex )
	{
		DragQueryFile(hDropInfo, nIndex, lpszPathName, _MAX_PATH);  //得到文件名
		// 把得到的文件名传给父窗口
		LPARAM lParam = (LPARAM)lpszPathName;
		SendMessage(WM_USER_DROPFILESTOLISTCTRL,0,lParam);
	}
	DragFinish(hDropInfo);

	CXTPShellListView::OnDropFiles(hDropInfo);
}

LRESULT CShellListView::OnDropFilesToListCtrl(WPARAM wParam, LPARAM lParam)
{
	CXTPString strSrcPathName((char*)lParam);
	int idx = strSrcPathName.ReverseFind('\\');
	if(idx != -1)
	{
		CString strFileName = strSrcPathName.Mid(idx, strSrcPathName.GetLength()-idx);
		idx = g_CurDirectory.ReverseFind('.');
		if(idx != -1)
		{
			idx = g_CurDirectory.ReverseFind('\\');
			g_CurDirectory = g_CurDirectory.Mid(0, idx);
		}
		CXTPString strDestPathName = g_CurDirectory+strFileName;
		CopyFile(strSrcPathName, strDestPathName,TRUE);
		Flush();
		return TRUE;
	}

	return FALSE;
}

void CShellListView::OnLVNItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if(pNMLV->iItem != -1)
	{
		GetItemPath(pNMLV->iItem, g_CurDirectory);

		//CXTPTaskPanel* pPanel = GetTaskPanel();
		//CXTPTaskPanelGroup* pGroupDetails = pPanel->FindGroup(ID_TASKGROUP_DETAILS);
		//ASSERT_OBJ(pGroupDetails);
		//pGroupDetails->GetItems()->Clear();

		//CString strName = m_pListCtrl->GetItemText(pNMLV->iItem,2);
		//CXTPTaskPanelGroupItem* pItemSelected = pGroupDetails->AddTextItem(strName);
		//pItemSelected->SetBold(TRUE);
		//pItemSelected->GetMargins().SetRectEmpty();

		//strName = m_pListCtrl->GetItemText(pNMLV->iItem,0);
		//pGroupDetails->AddTextItem(strName);
		//strName = m_pListCtrl->GetItemText(pNMLV->iItem,1);
		//pGroupDetails->AddTextItem(strName);
		//strName = m_pListCtrl->GetItemText(pNMLV->iItem,3);
		//pGroupDetails->AddTextItem(strName);
	}

	*pResult = 0;
}

void CShellListView::OnLVNEndlabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	LVITEM& item = pDispInfo->item;
	CEdit* pEdit = m_pListCtrl->EditLabel(item.iItem);
	if(!pEdit) return;
	CString str;
	pEdit->GetWindowText(str);

	CXTPString strSrcPathName(g_CurDirectory);
	int idx = g_CurDirectory.ReverseFind('\\');
	CString strFileTitle = g_CurDirectory.Mid(idx+1, g_CurDirectory.GetLength()-idx);

	g_CurDirectory.Replace(strFileTitle,str);
	CXTPString strDestPathName = g_CurDirectory;
	CopyFile(strSrcPathName, strDestPathName,TRUE);
	DeleteFile(strSrcPathName);
	Flush();

	*pResult = 0;
}

void CShellListView::SetStyle(UINT id)
{
	DWORD dwStyle = m_pListCtrl->GetStyle() & LVS_TYPEMASK;
	switch (id)
	{
	case ID_SHELL_STYLE_ICON:
		dwStyle = LVS_ICON;
		break;
	case ID_SHELL_STYLE_SMALLICON:
		dwStyle = LVS_SMALLICON;
		break;
	case ID_SHELL_STYLE_LIST:
		dwStyle = LVS_LIST;
		break;
	case ID_SHELL_STYLE_REPORT:
		dwStyle = LVS_REPORT;
		break;
	}

	m_pListCtrl->ModifyStyle(LVS_TYPEMASK,dwStyle & LVS_TYPEMASK);
}

void CShellListView::SetSort(UINT id)
{
	switch (id)
	{
	case ID_SHELL_SORT_NAME:
		SortList(0,true);
		break;
	case ID_SHELL_SORT_TYPE:
		SortList(2,true);
		break;
	case ID_SHELL_SORT_SIZE:
		SortList(1,true);
		break;
	case ID_SHELL_SORT_DATE:
		SortList(3,true);
		break;
	}
}

void CShellListView::DownLoad()
{
	int idx = g_CurDirectory.ReverseFind('.');
	if(idx != -1)
	{
		idx = g_CurDirectory.ReverseFind('\\');
		CString strFileName = g_CurDirectory.Mid(idx+1, g_CurDirectory.GetLength()-idx);
		CFileDialog ldFile(FALSE,NULL,strFileName);
		if (ldFile.DoModal() == IDOK)
		{
			CXTPString strDestPathName(ldFile.GetPathName());
			CXTPString strSrcPathName(g_CurDirectory);
			CopyFile(strSrcPathName, strDestPathName,TRUE);
		}
	}
}

void CShellListView::UpLoad()
{
	CFileDialog ldFile(TRUE,_T(""),_T("*.*"));
	if (ldFile.DoModal() == IDOK)
	{
		CXTPString strSrcPathName(ldFile.GetPathName());
		int idx = g_CurDirectory.ReverseFind('.');
		if(idx != -1)
		{
			idx = g_CurDirectory.ReverseFind('\\');
			g_CurDirectory = g_CurDirectory.Mid(0, idx);
		}
		CXTPString strDestPathName = g_CurDirectory+_T("\\")+ldFile.GetFileName();
		CopyFile(strSrcPathName, strDestPathName,TRUE);
		Flush();
	}
}

void CShellListView::Delete()
{
	CXTPString strSrcPathName(g_CurDirectory);
	DeleteFile(strSrcPathName);
	Flush();
}

void CShellListView::Flush()
{
	CMainFrame* pMaainFrame = (CMainFrame*)AfxGetMainWnd();
	CShellTreeView* pShellTreeView = pMaainFrame->m_pShellTreeView;
	if(!pShellTreeView) return;
	if(g_CurDirectory.ReverseFind('.') != -1)
	{
		int idx = g_CurDirectory.ReverseFind('\\');
		g_CurDirectory = g_CurDirectory.Mid(0, idx);
	}
	pShellTreeView->TunnelTree(g_CurDirectory);
}