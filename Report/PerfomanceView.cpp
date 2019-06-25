// PerfomanceView.cpp : implementation file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2011 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RibbonReport.h"
#include "PerfomanceView.h"
#include "ReportFrame.h"
#include "ReportRecord.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPerfomanceView

IMPLEMENT_DYNCREATE(CPerfomanceView, CView)

CPerfomanceView::CPerfomanceView()
{
	m_pItem = NULL;
	m_bDblClickClose = TRUE;
}

CPerfomanceView::~CPerfomanceView()
{
}

BEGIN_MESSAGE_MAP(CPerfomanceView, CView)
	//{{AFX_MSG_MAP(CPerfomanceView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()

	//}}AFX_MSG_MAP
	ON_NOTIFY(XTP_NM_REPORT_HEADER_RCLICK, IDC_REPORT_CONTROL, OnReportColumnRClick)
	ON_NOTIFY(NM_DBLCLK, IDC_REPORT_CONTROL, OnReportItemDblClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPerfomanceView drawing

void CPerfomanceView::OnDraw(CDC*)
{
}

/////////////////////////////////////////////////////////////////////////////
// CPerfomanceView diagnostics

#ifdef _DEBUG
void CPerfomanceView::AssertValid() const
{
	CView::AssertValid();
}

void CPerfomanceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPerfomanceView message handlers

CReportRecordView* CPerfomanceView::GetTargetReportView()
{
	return (CReportRecordView*)((CReportFrame*)GetParent())->m_pOwnerView;
}

int CPerfomanceView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndReport.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|WM_VSCROLL, CRect(0, 0, 0, 0), this, IDC_REPORT_CONTROL))
	{
		TRACE(_T("Failed to create view window\n"));
		return -1;
	}

	m_wndReport.ModifyStyle(0, WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_TABSTOP);
	m_wndReport.ShowGroupBy(FALSE);
	m_wndReport.FocusSubItems(FALSE);
	m_wndReport.EnableDragDrop(_T("ReportRecordView"), xtpReportAllowDrag | xtpReportAllowDrop);
	m_wndReport.GetPaintManager()->SetColumnStyle(xtpReportColumnResource);
	m_wndReport.GetToolTipContext()->SetStyle(xtpToolTipResource);
	m_wndReport.m_bSortedDragDrop = TRUE;

	return 0;
}

void CPerfomanceView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_wndReport.GetSafeHwnd())
	{
		m_wndReport.MoveWindow(0, 0, cx, cy);
	}
}

BOOL CPerfomanceView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CPerfomanceView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	m_wndReport.SetFocus();
}

void CPerfomanceView::OnReportColumnRClick(NMHDR* pNMHDR, LRESULT* /*result*/)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNMHDR;
	if(!pItemNotify->pColumn) return;
	CPoint ptClick = pItemNotify->pt;

	CMenu menu;
	if(!menu.CreatePopupMenu()) return;

	// create main menu items
	menu.AppendMenu(MF_STRING, 501, _T("表头列表"));
	menu.AppendMenu(MF_STRING, 502, _T("数据过滤"));
	menu.AppendMenu(MF_SEPARATOR, (UINT)-1, (LPCTSTR)NULL);
	menu.AppendMenu(MF_STRING, 503, _T("子项选择"));
	menu.AppendMenu(MF_STRING, 504, _T("允许编辑"));
	menu.AppendMenu(MF_SEPARATOR, (UINT)-1, (LPCTSTR)NULL);
	menu.AppendMenu(MF_STRING, 505, _T("显示分组"));
	menu.AppendMenu(MF_STRING, 506, _T("自动列宽"));
	menu.AppendMenu(MF_SEPARATOR, (UINT)-1, (LPCTSTR)NULL);
	menu.AppendMenu(MF_STRING, 507, _T("双击关闭"));

	// track menu
	int nMenuResult = CXTPCommandBars::TrackPopupMenu(&menu, TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTALIGN |TPM_RIGHTBUTTON, ptClick.x, ptClick.y, this, NULL);

	switch (nMenuResult)
	{
	case 501:
		{
			CMainFrame* pMaainFrame = (CMainFrame*)AfxGetMainWnd();
			if (pMaainFrame)
			{
				BOOL bShow = !pMaainFrame->m_wndSubList.IsVisible();
				pMaainFrame->ShowControlBar(&pMaainFrame->m_wndSubList, bShow, FALSE);
			}
		}
		break;
	case 502:
		{
			CMainFrame* pMaainFrame = (CMainFrame*)AfxGetMainWnd();
			if (pMaainFrame)
			{
				BOOL bShow = !pMaainFrame->m_wndFilterEdit.IsVisible();
				pMaainFrame->ShowControlBar(&pMaainFrame->m_wndFilterEdit, bShow, FALSE);
			}
		}
		break;
	case 503:
		m_wndReport.FocusSubItems(!m_wndReport.IsFocusSubItems());
		break;
	case 504:
		m_wndReport.AllowEdit(!m_wndReport.IsAllowEdit());
		break;
	case 505:
		m_wndReport.ShowGroupBy(!m_wndReport.IsGroupByVisible());
		break;
	case 506:
		{
			m_wndReport.GetReportHeader()->SetAutoColumnSizing(TRUE);
			CXTPReportColumns* pColums = m_wndReport.GetColumns();
			CXTPReportHeader* pHeader = m_wndReport.GetReportHeader();
			for(int i=0; i<pColums->GetCount(); i++)
			{
				CXTPReportColumn* pColumn = pColums->GetAt(i);
				if (pColumn != NULL) pHeader->BestFit(pColumn);
			}
			m_wndReport.RedrawControl();
		}
		break;
	case 507:
		m_bDblClickClose = !m_bDblClickClose;
		break;
	}
}

void CPerfomanceView::OnReportItemDblClick(NMHDR* pNMHDR, LRESULT* /*result*/)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNMHDR;
	if(!pItemNotify->pRow || !pItemNotify->pItem) return;

	if(m_wndReport.IsFocusSubItems())
	{
		COleVariant vtValue = ((CXTPReportRecordItemVariant*)pItemNotify->pItem)->GetValue();
		if(m_pItem) m_pItem->SetValue(vtValue);
	}
	else
	{
		COleVariant vtValue = ((CXTPReportRecordItemVariant*)pItemNotify->pRow->GetRecord()->GetItem(0))->GetValue();
		if(m_pItem) m_pItem->SetValue(vtValue);
	}

	CXTPReportRecord* pSelItemRcd = m_pItem->GetRecord();
	if(!pSelItemRcd) return;

	CReportRecordView* pTargetReportView = GetTargetReportView();
	if(pTargetReportView != NULL)
	{
		CString strTable = pTargetReportView->GetCurTable();
		CString field = GetFieldName(strTable,m_pItem->GetIndex()+1);
		CString code = ((CXTPReportRecordItemVariant*)pSelItemRcd->GetItem(0))->GetValue().bstrVal;
		SetRecordset(strTable,field,code,((CXTPReportRecordItemVariant*)m_pItem)->GetValue());
		pTargetReportView->UpdateRecord(pSelItemRcd);

		if(m_bDblClickClose)
		{
			//GetParent()->SendMessage(WM_CLOSE);
			::PostMessage(GetParent()->m_hWnd, WM_CLOSE, 0, 0);
		}
	}
}

void CPerfomanceView::SetRecordItem(CXTPReportRecordItemVariant*& pItem)
{
	m_pItem = pItem;
}