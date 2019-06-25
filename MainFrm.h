// MainFrm.h : interface of the CMainFrame class
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

#if !defined(AFX_MAINFRM_H__4BA2CE07_D84C_4A7C_B653_6C991D5C281B__INCLUDED_)
#define AFX_MAINFRM_H__4BA2CE07_D84C_4A7C_B653_6C991D5C281B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PaneProperties.h"
#include "PaneRibbon.h"
#include "PanePoints.h"
#include "ResourceManager.h"
#include "SkinPropertySheet.h"

#include "Ribbon/BackstagePageInfo.h"
#include "Ribbon/BackstagePageHelp.h"
#include "Ribbon/BackstagePageSave.h"
#include "Ribbon/BackstagePageRecent.h"
#include "Ribbon/BackstagePagePrint.h"


class CRibbonReportView;
class CReportGeneratorView;
class CChartView;
class CWebHtmlView;
class CTaskTrackView;
class CShellListView;
class CShellTreeView;
class CSearchView;

class CXTPTrayIconEx : public CXTPTrayIcon
{
	bool CreateMinimizeWnd(CWnd* /*pWndApp*/)
	{
		return true;
	}
};

class CMainFrame : public CXTPFrameWnd
{

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	void SetRibbonReportView();
	void SetReportGeneratorView();
	void SetChartView();
	void SetWebHtmlView();
	void SetTaskTrackView();
	void SetShellTreeView();
	void SetShellListView();
	void SetSearchView();
	BOOL IsActiveView(CView* pView);

	void RefreshPanes();



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

	void LoadRibbonIcons();
	void CreateBackstage();
	void CreateSystemMenuPopup();

	BOOL CreateGalleries();
	BOOL CreateRibbonBar();
	BOOL CreateMiniToolBar();
	BOOL CreateStatusBar();
	BOOL CreateMessageBar();
	BOOL CreateDockingPane();
	BOOL CreateTrayIcon();
	void InitBarToopTip();

	BOOL IsRegSoft() const;
	BOOL DelRegSoft(CTime tm) const;
	void LoadRegSoft(vector<CXTPRibbonTab*>& vecTabs);

public:
	void RedrawFrame(CWnd* pWnd);
	void SkinFrameChanged();
	void SkinSchemaChanged(int nSchema);
	void SkinThemeChanged(int nTheme);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	CXTPSplitterWnd         m_wndSplitter;
	CXTPStatusBar           m_wndStatusBar;
	CXTPDockingPaneManager  m_wndPaneManager;
	CPaneProperties         m_wndPaneProperties;
	CPaneRibbon             m_wndPaneRibbon;
	CPanePoints             m_wndPanePoints;
	CSkinPropertySheet      m_wndSkinThemes;
	CXTPTabClientWnd        m_wndTabClient;	
	CXTPRibbonBar*          m_pRibbonBar;
	CXTPStatusBarPane*      m_pStatusBarLogo;
	CXTPStatusBarPane*      m_pStatusBarTime;
	CXTPCommandBars*        m_pActiveCommandBars;
	CResourceManager*       m_pResourceManager;
	CPaneHolder*            m_pActivePane;

	CXTPControlGalleryItems* m_pItemsShapes;
	CXTPControlGalleryItems* m_pItemsColors;
	CXTPControlGalleryItems* m_pItemsFonts;
	CXTPControlGalleryItems* m_pItemsFontTextColor;
	CXTPControlGalleryItems* m_pItemsFontBackColor;
	CXTPControlGalleryItems* m_pItemsFontFace;
	CXTPControlGalleryItems* m_pItemsFontSize;
	CXTPControlGalleryItems* m_pItemsSearchdatapart;
	CXTPControlGalleryItems* m_pItemsUndo;
	CXTPControlGalleryItems* m_pItemsNumber;
	CXTPControlGalleryItems* m_pItemsStylesFormat;
	CXTPControlGalleryItems* m_pItemsStylesCell;
	CXTPControlGalleryItems* m_pItemsConditionHighlight;
	CXTPControlGalleryItems* m_pItemsConditionItemSecect;
	CXTPControlGalleryItems* m_pItemsConditionDataBars;
	CXTPControlGalleryItems* m_pItemsConditionClrValeur;
	CXTPControlGalleryItems* m_pItemsConditionIconList;

	BOOL                    m_bMinimized;
	CXTPTrayIconEx          m_wndTrayIcon;

	CString                 m_strTime;
	UINT                    m_nIDTimer;
	int                     m_nTheme;


	BOOL m_bAnimation;
	BOOL m_bLayoutRTL;
	BOOL m_bShowMiniToolbar;

	CString m_strStylesPath;
	CString m_strIniFileName;

	CXTPMessageBar m_wndMessageBar;
	UINT m_idRibbonStyle;

	UINT m_idColors;
	UINT m_idFonts;
	UINT m_idStylesCell;
	UINT m_idShape;

	CBackstagePageInfo m_pageInfo;
	CBackstagePageHelp m_pageHelp;
	CBackstagePageSave m_pageSave;
	CBackstagePageRecent m_pageRecent;
	CBackstagePagePrint m_pagePrint;

	CXTPControlPopup*       m_pPopupPoints;
	CXTPControlPopup*       m_pPopupCost;
	CXTPControlPopup*       m_pPopupMater;
	CXTPControlPopup*       m_pPopupFinance;
	CXTPControlPopup*       m_pPopupZxing;
	CXTPControlComboBox*    m_pSearchDatapart;
	CXTPControlEdit*        m_pSearchDateEdit;
	CXTPDatePickerControl*  m_pSearchDatePicker;
	COleDateTime            m_oleSearchDateTime;

	CDialogBar              m_wndSubList;
	CDialogBar              m_wndFilterEdit;
	CDialogBar              m_wndListBox;

	CRibbonReportView*      m_pRibbonReportView;
	CReportGeneratorView*   m_pReportGeneratorView;
	CChartView*             m_pChartView;
	CWebHtmlView*           m_pWebHtmlView;
	CTaskTrackView*         m_pTaskTrackView;
	CShellListView*         m_pShellListView;
	CShellTreeView*         m_pShellTreeView;
	CSearchView*            m_pSearchView;

public:
	static CArray<CXTPDockingPaneManager*, CXTPDockingPaneManager*> m_arrManagers;

// Generated message map functions
public:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
	afx_msg void OnCustomize();
	afx_msg void OnCustomizeQuickAccess();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();

	//}}AFX_MSG
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTrayIconNotify(WPARAM wParam, LPARAM lParam);

	//}}AFX_MSG
	afx_msg void OnTrayIconMixmax();
	afx_msg void OnTrayIconShowdlg();

	//}}AFX_MSG
	afx_msg void OnSkinTheme();
	afx_msg void OnSkinThemePopup(UINT id);
	afx_msg void OnUpdateSkinThemePopup(CCmdUI* pCmdUI);
	afx_msg void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);

	//}}AFX_MSG
	int m_idShellStyle,m_idShellSort;
	afx_msg void OnShellStyle(UINT id);
	afx_msg void OnUpdateShellStyle(CCmdUI* pCmdUI);
	afx_msg void OnShellSort(UINT id);
	afx_msg void OnUpdateShellSort(CCmdUI* pCmdUI);

	UINT m_idPointsmgr,m_idCostmgr,m_idMatermgr,m_idFinancemgr,m_idZxingFormat;
	afx_msg void OnBnPointsManager();
	afx_msg void OnBnCostManager();
	afx_msg void OnBnMaterManager();
	afx_msg void OnBnFinanceManager();
	afx_msg void OnBnFinanceLoan();
	afx_msg void OnBnZxingCommand();
	afx_msg void OnPointsManager(UINT id);
	afx_msg void OnUpdatePointsManager(CCmdUI* pCmdUI);
	afx_msg void OnCostManager(UINT id);
	afx_msg void OnUpdateCostManager(CCmdUI* pCmdUI);
	afx_msg void OnMaterManager(UINT id);
	afx_msg void OnUpdateMaterManager(CCmdUI* pCmdUI);
	afx_msg void OnFinanceManager(UINT id);
	afx_msg void OnUpdateFinanceManager(CCmdUI* pCmdUI);
	afx_msg void OnZxingCommand(UINT id);
	afx_msg void OnUpdateZxingCommand(CCmdUI* pCmdUI);

	//}}AFX_MSG
	afx_msg void OnHelpHelp();
	afx_msg void OnHelpReg();
	afx_msg void OnHelpNet();

	BOOL m_bOptions[6];
	afx_msg void OnOptionsRightToLeft();
	afx_msg void OnOptionsAnimation();
	afx_msg void OnFrameTheme();
	afx_msg void OnUpdateOptionsRighttoleft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionsAnimation(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFrameTheme(CCmdUI* pCmdUI);

	afx_msg void OnOptionsStyle(UINT);
	afx_msg void OnUpdateOptionsStyle(CCmdUI* pCmdUI);
	afx_msg void OnOptionsFont(UINT nID);
	afx_msg void OnUpdateOptionsFont(CCmdUI* pCmdUI);

	afx_msg void OnAutoResizeIcons();
	afx_msg void OnUpdateAutoResizeIcons(CCmdUI* pCmdUI);
	afx_msg void OnToggleGroups();
	afx_msg void OnUpdateRibbonMinimize(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRibbonExpand(CCmdUI* pCmdUI);
	afx_msg void OnRibbonSwitchReportView();
	afx_msg void OnRibbonSwitchChartView();
	afx_msg void OnUpdateRibbonSwitchReportView(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRibbonSwitchChartView(CCmdUI* pCmdUI);
	
	afx_msg void OnRibbonTabChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRibbonTabChanging(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnResetRibbonBar();
	afx_msg void ShowCustomizeDialog(int nSelectedPage);

	afx_msg void OnGalleryColors(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGalleryFonts(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateGalleryColors(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGalleryFonts(CCmdUI* pCmdUI);

	afx_msg void OnGalleryShapes(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateGalleryShapes(CCmdUI* pCmdUI);

	afx_msg void OnGalleryStyleCell(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateGalleryStyleCell(CCmdUI* pCmdUI);

	afx_msg void OnSystemCommand(UINT nID);
	afx_msg void OnExplorCommand(UINT nID);

	afx_msg void OnUpdateGallerySearchDataPart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSearchDataPart(CCmdUI* pCmdUI);

	afx_msg void OnUpdateSearchDatePopup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSearchDateEdit(CCmdUI* pCmdUI);
	afx_msg void OnSearchDateEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSearchDateButton(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

	friend class CDlgTrayIcon;
	friend class CPaneExplorer;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__4BA2CE07_D84C_4A7C_B653_6C991D5C281B__INCLUDED_)
