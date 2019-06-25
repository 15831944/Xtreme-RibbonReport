// ShellTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "RibbonReport.h"
#include "ShellTreeView.h"
#include "ShellListView.h"
#include "MainFrm.h"

#include <WinNetWk.h>  
#pragma comment(lib, "Mpr.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShellTreeView

IMPLEMENT_DYNCREATE(CShellTreeView, CXTPShellTreeView)

CShellTreeView::CShellTreeView()
{
	m_pListView = NULL;
}

CShellTreeView::~CShellTreeView()
{
}


BEGIN_MESSAGE_MAP(CShellTreeView, CXTPShellTreeView)
	//{{AFX_MSG_MAP(CShellTreeView)
 	ON_WM_NCCALCSIZE()
 	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShellTreeView drawing

void CShellTreeView::OnDraw(CDC* /*pDC*/)
{
}

void CShellTreeView::OnInitialUpdate()
{
	CXTPShellTreeView::OnInitialUpdate();
}

/////////////////////////////////////////////////////////////////////////////
// CShellTreeView diagnostics

#ifdef _DEBUG
void CShellTreeView::AssertValid() const
{
	CXTPShellTreeView::AssertValid();
}

void CShellTreeView::Dump(CDumpContext& dc) const
{
	CXTPShellTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShellTreeView message handlers

void CShellTreeView::SelectParentItem()
{
    if (::IsWindow(GetTreeCtrl().m_hWnd))
    {
        HTREEITEM htItem = GetTreeCtrl().GetSelectedItem();
        if (htItem != GetTreeCtrl().GetRootItem())
        {
            GetTreeCtrl().SelectItem(GetTreeCtrl().GetParentItem(htItem));
            GetTreeCtrl().SetFocus();
        }
    }
}

BOOL CShellTreeView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CXTPShellTreeView::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_STATICEDGE;
	
	return TRUE;
}

int CShellTreeView::GetHeaderHeight() const
{
	if (m_pListView)
	{
		CListCtrl* pListCtrl = &m_pListView->GetListCtrl();
		if (::IsWindow(pListCtrl->GetSafeHwnd()))
		{
			CHeaderCtrl* pHeader = pListCtrl->GetHeaderCtrl();
			if (::IsWindow(pHeader->GetSafeHwnd()))
			{
				CRect rc;
				pHeader->GetWindowRect(&rc);
				return rc.Height();
			}
		}
	}
	return 19; // default size.
}

void CShellTreeView::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	lpncsp->rgrc[0].top = GetHeaderHeight() + 3;
	CXTPShellTreeView::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CShellTreeView::OnNcPaint() 
{
	// code block: paint scrollbars first.
	{
		Default();
	}

	CWindowDC dc(this);

	CXTPWindowRect rWindow(this);
	rWindow.OffsetRect(-rWindow.TopLeft());

	dc.Draw3dRect(&rWindow, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));

	// draw psudo caption.
	rWindow.DeflateRect(1, 1);
	rWindow.bottom = rWindow.top + GetHeaderHeight();

	dc.FillSolidRect(rWindow.left, rWindow.bottom, rWindow.Width(), 1, GetXtremeColor(COLOR_WINDOW));

	CXTPBufferDC memDC(dc, rWindow);

	memDC.FillSolidRect(rWindow, GetXtremeColor(COLOR_3DFACE));	
	memDC.Draw3dRect(&rWindow, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));

	CXTPFontDC fontDC(&memDC, &XTPAuxData().font);

	rWindow.DeflateRect(4,2);
	memDC.SetBkMode(TRANSPARENT);
	memDC.DrawText(_T("Folders"), &rWindow, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
}

void CShellTreeView::NetConnect(CString ip, CString user, CString pwd)
{
	/*
	NETRESOURCE net_Resource;
	net_Resource.dwDisplayType = RESOURCEDISPLAYTYPE_DIRECTORY;
	net_Resource.dwScope = RESOURCE_CONNECTED;
	net_Resource.dwType = RESOURCETYPE_ANY;
	net_Resource.dwUsage = 0;
	net_Resource.lpComment = "";
	net_Resource.lpLocalName = "Z:";  //映射成本地驱动器z:
	net_Resource.lpProvider= NULL;
	strcpy_s(net_Resource.lpRemoteName,ip.GetLength()+1, ip); // \\servername\共享资源名
	DWORD dwFlags = CONNECT_UPDATE_PROFILE;
	DWORD dw = WNetAddConnection2(&net_Resource, pwd, user, dwFlags);
	switch(dw)  
	{  
	case ERROR_SUCCESS:  
		ShellExecute(NULL, TEXT("open"), TEXT("Z:"), NULL, NULL, SW_SHOWNORMAL);  
		break;
	case ERROR_ACCESS_DENIED:  
		msgErr("没有权访问！");
		break;  
	case ERROR_ALREADY_ASSIGNED:  
		ShellExecute(NULL, TEXT("open"), TEXT("Z:"), NULL, NULL, SW_SHOWNORMAL);  
		break;
	case ERROR_INVALID_ADDRESS:  
		msgErr("IP地址无效！");
		break;  
	case ERROR_NO_NETWORK:  
		msgErr("网络不可达！");
		break;  
	}  
	*/
}