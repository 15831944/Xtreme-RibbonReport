#if !defined(AFX_SHELLLISTVIEW_H__218BC459_82F2_4781_889B_72318030490E__INCLUDED_)
#define AFX_SHELLLISTVIEW_H__218BC459_82F2_4781_889B_72318030490E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ShellListView.h : header file
//

#include "MainFrm.h"


/////////////////////////////////////////////////////////////////////////////
// CShellListView view


enum ENUM_LISTCTRL_MSG{ WM_USER_DROPFILESTOLISTCTRL = WM_APP + 0x0100 };

class CShellListView : public CXTPShellListView
{
protected:
	CShellListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CShellListView)

// Attributes
public:
	void SetStyle(UINT id);
	void SetSort(UINT id);
	void DownLoad();
	void UpLoad();
	void Delete();
	void Flush();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShellListView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CShellListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// Generated message map functions

protected:
	//{{AFX_MSG(CShellListView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//{{AFX_MSG(m_pShellSqlView)
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg LRESULT OnDropFilesToListCtrl(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	afx_msg void OnLVNItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLVNEndlabelEdit(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

private:
	CImageList*  m_pDragImage;
	BOOL		 m_bDragging;
	UINT	     m_itemDrag;
	CPoint       m_ptOrigin,m_ptHotSpot;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHELLLISTVIEW_H__218BC459_82F2_4781_889B_72318030490E__INCLUDED_)
