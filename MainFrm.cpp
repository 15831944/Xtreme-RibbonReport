// MainFrm.cpp : implementation of the CMainFrame class
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
#include "MainFrm.h"

#include "RibbonReportView.h"
#include "ChartView.h"
#include "WebHtmlView.h"
#include "Explorer/ShellTreeView.h"
#include "Explorer/ShellListView.h"
#include "Explorer/SearchView.h"
#include "TaskTrack/TaskTrackView.h"

#include "Report/ReportRecordView.h"
#include "Report/ReportGeneratorView.h"

#include "DlgReg.h"
#include "DlgSetPwd.h"
#include "DlgRegUser.h"
#include "DlgTrayIcon.h"
#include "DlgMethod.h"
#include "DlgLoan.h"
#include "AppCaption.h"

#include "Ribbon/Galleryitems.h"
#include "Ribbon/CustomizePageGeneral.h"
#include "Ribbon/CustomizePageQuickAccessToolbar.h"
#include "Ribbon/CustomizePageRibbon.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define _SAVE_TO_XML

CArray<CXTPDockingPaneManager*, CXTPDockingPaneManager*> CMainFrame::m_arrManagers;


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CXTPFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CXTPFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_XTP_CREATECONTROL()
	ON_WM_GETMINMAXINFO()
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_MESSAGE(TIN_XTP_TRAYICON, OnTrayIconNotify)
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_COMMAND(ID_APP_OPTIONS, OnCustomize)
	ON_COMMAND(XTP_ID_RIBBONCUSTOMIZE, OnCustomizeQuickAccess)
	ON_WM_TIMER()

	//}}AFX_MSG_MAP
	ON_COMMAND(ID_TRAYICON_MIX_MAX, OnTrayIconMixmax)
	ON_COMMAND(ID_TRAYICON_SHOWDLG, OnTrayIconShowdlg)

	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_SHELL_STYLE_ICON, ID_SHELL_STYLE_REPORT, OnShellStyle)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SHELL_STYLE_ICON, ID_SHELL_STYLE_REPORT, OnUpdateShellStyle)
	ON_COMMAND_RANGE(ID_SHELL_SORT_NAME, ID_SHELL_SORT_DATE, OnShellSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SHELL_SORT_NAME, ID_SHELL_SORT_DATE, OnUpdateShellSort)


	//}}AFX_MSG_MAP
	ON_COMMAND(ID_POINTSMGR_TABLE, OnBnPointsManager)
	ON_COMMAND(ID_COSTMGR_TABLE, OnBnCostManager)
	ON_COMMAND(ID_MATERMGR_TABLE, OnBnMaterManager)
	ON_COMMAND(ID_FINANCEMGR_TABLE, OnBnFinanceManager)
	ON_COMMAND(ID_FINANCEMGR_LOAN, OnBnFinanceLoan)
	ON_COMMAND(ID_ZXING_FORMAT, OnBnZxingCommand)
	ON_COMMAND_RANGE(ID_TABLE_ROAD, ID_TABLE_TEMP, OnPointsManager)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TABLE_ROAD, ID_TABLE_TEMP, OnUpdatePointsManager)
	ON_COMMAND_RANGE(ID_TABLE_QTY, ID_TABLE_LIST, OnCostManager)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TABLE_QTY, ID_TABLE_LIST, OnUpdateCostManager)
	ON_COMMAND_RANGE(ID_TABLE_EMPLOY, ID_TABLE_RETSTOCK, OnMaterManager)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TABLE_EMPLOY, ID_TABLE_RETSTOCK, OnUpdateMaterManager)
	ON_COMMAND_RANGE(ID_TABLE_MEMBER, ID_TABLE_OUTLAY, OnFinanceManager)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TABLE_MEMBER, ID_TABLE_OUTLAY, OnUpdateFinanceManager)
	ON_COMMAND_RANGE(ID_ZXING_EAN13, ID_ZXING_QRCODEMID, OnZxingCommand)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ZXING_EAN13, ID_ZXING_QRCODEMID, OnUpdateZxingCommand)

	//}}AFX_MSG_MAP
	ON_COMMAND(ID_THEME_SKIN, OnSkinTheme)
	ON_COMMAND_RANGE(ID_THEME_OFFICE2007_BLUE, ID_THEME_ZUNE, OnSkinThemePopup)
	ON_UPDATE_COMMAND_UI_RANGE(ID_THEME_OFFICE2007_BLUE, ID_THEME_ZUNE, OnUpdateSkinThemePopup)

	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP_HELP, OnHelpHelp)
	ON_COMMAND(ID_HELP_REG, OnHelpReg)
	ON_COMMAND(ID_HELP_NET, OnHelpNet)

	//}}AFX_MSG_MAP
	ON_COMMAND_EX(ID_REPORT_FILTEREDIT, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_REPORT_FILTEREDIT, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_REPORT_SUBLIST, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_REPORT_SUBLIST, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_REPORT_LISTBOX, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_REPORT_LISTBOX, OnUpdateControlBarMenu)

	//}}AFX_MSG_MAP
	ON_COMMAND(ID_OPTIONS_RIGHTTOLEFT, OnOptionsRightToLeft)
	ON_COMMAND(ID_OPTIONS_ANIMATION, OnOptionsAnimation)
	ON_COMMAND(ID_OPTIONS_FRAMETHEME, OnFrameTheme)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_RIGHTTOLEFT, OnUpdateOptionsRighttoleft)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ANIMATION, OnUpdateOptionsAnimation)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_FRAMETHEME, OnUpdateFrameTheme)

	ON_COMMAND_RANGE(ID_OPTIONS_STYLE_BLUE2007, ID_OPTIONS_STYLE_SYSTEM, OnOptionsStyle)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OPTIONS_STYLE_BLUE2007, ID_OPTIONS_STYLE_SYSTEM, OnUpdateOptionsStyle)

	ON_COMMAND(ID_OPTIONS_FONT_AUTORESIZEICONS, OnAutoResizeIcons)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_FONT_AUTORESIZEICONS, OnUpdateAutoResizeIcons)
	ON_COMMAND_RANGE(ID_OPTIONS_FONT_SYSTEM, ID_OPTIONS_FONT_EXTRALARGE, OnOptionsFont)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OPTIONS_FONT_SYSTEM, ID_OPTIONS_FONT_EXTRALARGE, OnUpdateOptionsFont)

	ON_COMMAND(XTP_ID_RIBBONCUSTOMIZE_MINIMIZE, OnToggleGroups)
	ON_COMMAND(ID_RIBBON_MINIMIZE, OnToggleGroups)
	ON_COMMAND(ID_RIBBON_EXPAND, OnToggleGroups)
	ON_UPDATE_COMMAND_UI(ID_RIBBON_MINIMIZE, OnUpdateRibbonMinimize)
	ON_UPDATE_COMMAND_UI(ID_RIBBON_EXPAND, OnUpdateRibbonExpand)
	ON_COMMAND(ID_RIBBON_SWITCH_REPORTVIEW, OnRibbonSwitchChartView)
	ON_COMMAND(ID_RIBBON_SWITCH_CHARTVIEW, OnRibbonSwitchReportView)
	ON_UPDATE_COMMAND_UI(ID_RIBBON_SWITCH_REPORTVIEW, OnUpdateRibbonSwitchReportView)
	ON_UPDATE_COMMAND_UI(ID_RIBBON_SWITCH_CHARTVIEW, OnUpdateRibbonSwitchChartView)

	//}}AFX_MSG_MAP
	ON_COMMAND_EX(ID_VIEW_MESSAGE_BAR, CXTPFrameWnd::OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MESSAGE_BAR, CXTPFrameWnd::OnUpdateControlBarMenu)

	ON_NOTIFY(TCN_SELCHANGE, XTP_ID_RIBBONCONTROLTAB, OnRibbonTabChanged)
	ON_NOTIFY(TCN_SELCHANGING, XTP_ID_RIBBONCONTROLTAB, OnRibbonTabChanging)

	ON_MESSAGE_VOID(WM_XTP_CUSTOMIZATION_RIBBONRESET, OnResetRibbonBar)

	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_DATASET_DATBACKUP, ID_PROGRAM_REPORT, OnSystemCommand)
	ON_COMMAND_RANGE(ID_EXPLOR_SEARCH, ID_EXPLOR_FLUSH, OnExplorCommand)

	ON_XTP_EXECUTE(ID_GALLERY_COLORS, OnGalleryColors)
	ON_XTP_EXECUTE(ID_GALLERY_FONTS, OnGalleryFonts)
	ON_UPDATE_COMMAND_UI(ID_GALLERY_COLORS, OnUpdateGalleryColors)
	ON_UPDATE_COMMAND_UI(ID_GALLERY_FONTS, OnUpdateGalleryFonts)

	ON_XTP_EXECUTE(ID_GALLERY_SHAPES, OnGalleryShapes)
	ON_UPDATE_COMMAND_UI(ID_GALLERY_SHAPES, OnUpdateGalleryShapes)

	ON_XTP_EXECUTE(ID_GALLERY_STYLES_CELL, OnGalleryStyleCell)
	ON_UPDATE_COMMAND_UI(ID_GALLERY_STYLES_CELL, OnUpdateGalleryStyleCell)

	ON_UPDATE_COMMAND_UI(ID_GALLERY_SEARCH_DATAPART, OnUpdateGallerySearchDataPart)	
	ON_UPDATE_COMMAND_UI(ID_POINTSMGR_DATAPART, OnUpdateSearchDataPart)
	
	ON_UPDATE_COMMAND_UI(ID_POINTSMGR_DATEPOPUP, OnUpdateSearchDatePopup)
	ON_UPDATE_COMMAND_UI(ID_POINTSMGR_DATEEDIT, OnUpdateSearchDateEdit)
	ON_NOTIFY(XTP_FN_SPINUP, ID_POINTSMGR_DATEEDIT, OnSearchDateEdit)
	ON_NOTIFY(XTP_FN_SPINDOWN, ID_POINTSMGR_DATEEDIT, OnSearchDateEdit)
	//ON_NOTIFY(XTP_FN_BUDDYBUTTONCLICK, ID_POINTSMGR_DATEEDIT, OnSearchDateButton)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static int nIDPaneIcons[] =
{
	IDR_PANE_PROPERTIES, 
	IDR_PANE_RIBBON,
	IDR_PANE_POINTS
};

static UINT nIDTrayIcons[] =
{
	IDI_TRAYGRAY,
	IDI_TRAYFALE,
	IDI_TRAYTRUE,
	IDI_TRAYFALE
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
:m_idPointsmgr(0),
m_idCostmgr(0),
m_idMatermgr(0),
m_idFinancemgr(0),
m_idZxingFormat(0),
m_idShellStyle(0),
m_idShellSort(0),
m_nTheme(0),
m_bMinimized(FALSE)
{ 
	m_pRibbonBar            = NULL;
	m_pStatusBarLogo        = NULL;
	m_pStatusBarTime        = NULL;
	m_pActiveCommandBars    = NULL;
	m_pResourceManager      = NULL;
	m_pActivePane           = NULL;

	m_pPopupPoints          = NULL;
	m_pPopupCost            = NULL;
	m_pPopupMater           = NULL;
	m_pPopupFinance         = NULL;
	m_pPopupZxing           = NULL;
	m_pSearchDatapart       = NULL;
	m_pSearchDateEdit       = NULL;
	m_pSearchDatePicker     = NULL;

	m_pRibbonReportView     = NULL;
	m_pReportGeneratorView  = NULL;
	m_pChartView            = NULL;
	m_pWebHtmlView          = NULL;
	m_pTaskTrackView        = NULL;
	m_pSearchView           = NULL;
	m_pShellTreeView        = NULL;
	m_pShellListView        = NULL;

	TCHAR szStylesPath[_MAX_PATH];

	VERIFY(::GetModuleFileName(
		AfxGetApp()->m_hInstance, szStylesPath, _MAX_PATH));		

	CString strStylesPath(szStylesPath);
	int nIndex  = strStylesPath.ReverseFind(_T('\\'));
	if (nIndex > 0) {
		strStylesPath = strStylesPath.Left(nIndex);
	}
	else {
		strStylesPath.Empty();
	}
	m_strStylesPath = strStylesPath + _T("\\Styles\\");

	// get path of executable	
	LPTSTR lpszExt = _tcsrchr(szStylesPath, '.');
	lstrcpy(lpszExt, _T(".xml"));

	m_strIniFileName = szStylesPath;

	m_bAnimation = TRUE;
	m_bLayoutRTL = FALSE;

	ZeroMemory(&m_bOptions, sizeof(m_bOptions));


	m_pItemsSearchdatapart     = NULL;
	m_pItemsShapes             = NULL;
	m_pItemsColors             = NULL;
	m_pItemsFonts              = NULL;
	m_pItemsFontTextColor      = NULL;
	m_pItemsFontBackColor      = NULL;
	m_pItemsFontFace           = NULL;
	m_pItemsFontSize           = NULL;
	m_pItemsNumber             = NULL;
	m_pItemsStylesFormat       = NULL;
	m_pItemsStylesCell         = NULL;
	m_pItemsUndo               = NULL;
	m_pItemsConditionHighlight = NULL;
	m_pItemsConditionItemSecect= NULL;
	m_pItemsConditionDataBars  = NULL;
	m_pItemsConditionClrValeur = NULL;
	m_pItemsConditionIconList  = NULL;

	m_idRibbonStyle = ID_OPTIONS_STYLE_AQUA2007;
	m_idShape = -1;
	m_idColors = -1;
	m_idFonts = -1;
	m_idStylesCell = -1;
	m_idShape = -1;

	m_bShowMiniToolbar = TRUE;

	m_strTime = CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));
}

CMainFrame::~CMainFrame()
{
	::KillTimer(NULL, m_nIDTimer);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!InitCommandBars())
		return -1;

	LoadRibbonIcons();
	InitBarToopTip();
	CreateGalleries();
	m_nIDTimer = SetTimer(1, 1000, NULL);

	if (!CreateStatusBar())
		return -1;

	if (!CreateMiniToolBar())
		return -1;

	if (!CreateRibbonBar())
		return -1;

	if (!CreateTrayIcon())
		return -1;

	if (!CreateDockingPane())
		return -1;

	// Initialize dialog bar m_wndSubList
	if (!m_wndSubList.Create(this, IDD_BAR_SUBLIST,
		CBRS_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE, ID_REPORT_SUBLIST))
		return -1;      // fail to create

	// Initialize dialog bar m_wndFilterEdit
	if (!m_wndFilterEdit.Create(this, IDD_BAR_FILTEREDIT,
		CBRS_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE, ID_REPORT_FILTEREDIT))
		return -1;      // fail to create

	// Initialize dialog bar m_wndListBox
	if (!m_wndListBox.Create(this, IDD_BAR_LISTBOX,
		CBRS_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE, ID_REPORT_LISTBOX))
		return -1;      // fail to create

	// docking for field chooser
	m_wndSubList.EnableDocking(CBRS_ALIGN_ANY);
	m_wndSubList.SetWindowText(_T("Field Chooser"));

	ShowControlBar(&m_wndSubList, FALSE, FALSE);
	FloatControlBar(&m_wndSubList, CPoint(400, GetSystemMetrics(SM_CYSCREEN) / 3));

	// docking for filter editing
	m_wndFilterEdit.EnableDocking(CBRS_ALIGN_TOP);
	m_wndFilterEdit.SetWindowText(_T("Filter text"));

	ShowControlBar(&m_wndFilterEdit, FALSE, FALSE);
	FloatControlBar(&m_wndFilterEdit, CPoint(400, GetSystemMetrics(SM_CYSCREEN) / 3));

	// docking for field chooser
	m_wndListBox.EnableDocking(CBRS_ALIGN_ANY);
	m_wndListBox.SetWindowText(_T("Data Chooser"));

	ShowControlBar(&m_wndListBox, FALSE, FALSE);
	FloatControlBar(&m_wndListBox, CPoint(400, GetSystemMetrics(SM_CYSCREEN) / 3));

	SetRibbonReportView();

#ifndef _SAVE_TO_XML
	//LoadCommandBars(_T("CommandBars_Ribbon"));
#else
	CXTPPropExchangeXMLNode px(TRUE, 0, _T("Settings"));

	if (px.LoadFromFile(m_strIniFileName))
	{
		CXTPPropExchangeSection pxCommandBars(px.GetSection(_T("CommandBars")));
		XTP_COMMANDBARS_PROPEXCHANGE_PARAM param; 
		param.bSerializeControls = TRUE;
		param.bSerializeOptions = TRUE;
		GetCommandBars()->DoPropExchange(&pxCommandBars, &param);
	}
#endif
	
	XTPSkinManager()->SetApplyOptions(xtpSkinApplyFrame | xtpSkinApplyColors | xtpSkinApplyMetrics | xtpSkinApplyMenus);
	SkinSchemaChanged(m_nTheme);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CXTPFrameWnd::PreCreateWindow(cs))
		return FALSE;

	cs.lpszClass = _T("XTPMainFrame");
	CXTPDrawHelpers::RegisterWndClass(AfxGetInstanceHandle(), cs.lpszClass,
		CS_DBLCLKS, AfxGetApp()->LoadIcon(IDR_MAINFRAME));


	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~FWS_ADDTOTITLE;  // 把cs.style中的FWS_ADDTOTITLE风格去掉 
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	cs.dwExStyle |= WS_EX_CONTROLPARENT;

	cs.cx = 800;
	cs.cy = 600;

	return TRUE;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if (CXTPFrameWnd::PreTranslateMessage(pMsg))
		return TRUE;

	return PreTranslateInput(pMsg);
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	m_wndSplitter.CreateStatic(this, 1, 2);
	m_wndSplitter.ModifyStyle(0, WS_EX_CONTROLPARENT|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
	m_wndSplitter.SetSplitterStyle(XTP_SPLIT_NOFULLDRAG | XTP_SPLIT_NOSIZE | XTP_SPLIT_NOBORDER);
	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CShellTreeView), CSize(180, 0), pContext);
	m_wndSplitter.CreateView(0, 1, pContext->m_pNewViewClass, CSize(300, 0), pContext);

	return TRUE;
}

BOOL CMainFrame::CreateDockingPane()
{
	m_wndPaneManager.InstallDockingPanes(this);
	//m_wndPaneManager.SetTheme(xtpPaneThemeResource);

	m_arrManagers.Add(&m_wndPaneManager);

	// Create docking panes.
	CXTPDockingPane* pPaneProperties = m_wndPaneManager.CreatePane(
		IDR_PANE_PROPERTIES, CRect(0, 0,200, 120), xtpPaneDockRight);
	pPaneProperties->Hide();

	CXTPDockingPane* pPaneRibbon = m_wndPaneManager.CreatePane(
		IDR_PANE_RIBBON, CRect(0, 0,200, 120), xtpPaneDockRight, pPaneProperties);
	pPaneRibbon->Hide();

	CXTPDockingPane* pPanePoints = m_wndPaneManager.CreatePane(
		IDR_PANE_POINTS, CRect(0, 0,200, 120), xtpPaneDockLeft, pPaneRibbon);
	pPanePoints->Hide();

	m_wndPaneManager.SetIcons(IDB_PANE_ICONS, nIDPaneIcons, _countof(nIDPaneIcons), RGB(255, 255, 255));
	CXTPDockingPaneLayout layoutNormal(&m_wndPaneManager);
	if (layoutNormal.Load(_T("StandardLayout")))
	{
		if (layoutNormal.GetPaneList().GetCount() == 6)
			m_wndPaneManager.SetLayout(&layoutNormal);
	}

	m_wndPaneManager.SetAlphaDockingContext(TRUE);
	m_wndPaneManager.SetShowDockingContextStickers(TRUE);
	m_wndPaneManager.SetDockingContextStickerStyle(xtpPaneStickerStyleVisualStudio2005);
	m_wndPaneManager.SetShowContentsWhileDragging();

	EnableDocking(CBRS_ALIGN_ANY);

	return TRUE;
}

BOOL CMainFrame::CreateTrayIcon()
{
	// Create the tray icon.
	if (!m_wndTrayIcon.Create(
		_T("Project Manager System v3.0!"), // Toolktip text
		this,                               // Parent window
		IDI_TRAYTRUE,                       // Icon resource ID
		IDR_CONTENT_TRAYICON,               // Resource ID of popup menu
		ID_TRAYICON_MIX_MAX,                // Resource ID of dblclick icon
		false))                             // True if default menu item is located by position
	{
		TRACE0("Failed to create tray icon\n");
		return FALSE;
	}

	m_wndTrayIcon.StartAnimation();
	m_wndTrayIcon.SetAnimationIcons(nIDTrayIcons, _countof(nIDTrayIcons));

	return TRUE;
}

BOOL CMainFrame::CreateMessageBar()
{
	if (!m_wndMessageBar.Create(GetCommandBars()))
		return FALSE;

	m_wndMessageBar.AddButton(SC_CLOSE, NULL, _T("关闭版本提示警告"));
	m_wndMessageBar.AddButton(XTP_ID_CUSTOMIZE, _T("选项..."), _T("显示更多设置"));

	XTPImageManager()->SetIcon(ID_MESSAGEBAR_WARNING, ID_MESSAGEBAR_WARNING);

#ifndef _XTP_INCLUDE_MARKUP
	m_wndMessageBar.SetMessage(_T("安全警告. 该版本为试用版本，某些功能已被禁用！"));
#else
	m_wndMessageBar.EnableMarkup();
	m_wndMessageBar.SetMessage(
		_T("<StackPanel Orientation='Horizontal'>")
		_T("        <Image Source='900'/>")
		_T("		<TextBlock Padding='3, 0, 0, 0' VerticalAlignment='Center'><Bold>安全警告</Bold></TextBlock>")
		_T("		<TextBlock Padding='10, 0, 0, 0' VerticalAlignment='Center'>该版本为试用版本，某些功能已被禁用！</TextBlock>")
		_T("</StackPanel>"));
#endif

	return TRUE;
}

BOOL CMainFrame::CreateStatusBar()
{
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return FALSE;      // fail to create
	}

	m_pStatusBarLogo = m_wndStatusBar.AddIndicator(ID_INDICATOR_LOGO, 0);
	m_wndStatusBar.EnableMarkup(TRUE);
	m_pStatusBarLogo->SetText(_T("<TextBlock VerticalAlignment='Center'><Bold><Run Foreground='#365f91'>RibbonReport</Run> <Run Foreground='Black'>Software</Run></Bold></TextBlock>"));
	m_pStatusBarLogo->SetStyle(m_pStatusBarLogo->GetStyle() | SBPS_POPOUT);
	m_pStatusBarTime = m_wndStatusBar.AddIndicator(ID_INDICATOR_TIME);
	m_pStatusBarTime->SetStyle(m_pStatusBarTime->GetStyle() | SBPS_POPOUT);

	CXTPToolTipContext* pToolTipContext = m_wndStatusBar.GetToolTipContext();
	//pToolTipContext->SetStyle(xtpToolTipResource);
	pToolTipContext->SetFont(m_wndStatusBar.GetPaintManager()->GetIconFont());

	return TRUE;
}

BOOL CMainFrame::CreateGalleries()
{
	m_pItemsShapes = CXTPControlGalleryItems::CreateItems(GetCommandBars(), ID_GALLERY_SHAPES);
	m_pItemsShapes->SetItemSize(CSize(20, 20));

	m_pItemsShapes->GetImageManager()->SetIcons(ID_GALLERY_SHAPES, 0, 0, CSize(20, 20));

	int nShape = 0;
	CXTPControlGalleryItem* pItem = m_pItemsShapes->AddLabel(0);
	pItem->SetCaption(_T("线条"));
	for (; nShape < 12; nShape++)
		m_pItemsShapes->AddItem(nShape, nShape);

	pItem = m_pItemsShapes->AddLabel(0);
	pItem->SetCaption(_T("图快"));
	for (; nShape < 12 + 32; nShape++)
		m_pItemsShapes->AddItem(nShape, nShape);

	pItem = m_pItemsShapes->AddLabel(0);
	pItem->SetCaption(_T("箭头"));
	for (; nShape < 12 + 32 + 27; nShape++)
		m_pItemsShapes->AddItem(nShape, nShape);

	pItem = m_pItemsShapes->AddLabel(0);
	pItem->SetCaption(_T("流程图"));
	for (; nShape < 12 + 32 + 27 + 28; nShape++)
		m_pItemsShapes->AddItem(nShape, nShape);

	pItem = m_pItemsShapes->AddLabel(0);
	pItem->SetCaption(_T("标注"));
	for (; nShape < 12 + 32 + 27 + 28 + 20; nShape++)
		m_pItemsShapes->AddItem(nShape, nShape);

	pItem = m_pItemsShapes->AddLabel(0);
	pItem->SetCaption(_T("星与旗帜"));
	for (; nShape < 12 + 32 + 27 + 28 + 20 + 16; nShape++)
		m_pItemsShapes->AddItem(nShape, nShape);


	m_pItemsConditionHighlight = CXTPControlGalleryItems::CreateItems(GetCommandBars(), ID_GALLERY_CONDITION_HIGHLIGHT);
	UINT uiGalleryCondition[] = {ID_CONDITION_LARGE,ID_CONDITION_SMALL,ID_CONDITION_BETWEEN,ID_CONDITION_EQUAL
		,ID_CONDITION_TEXT,ID_CONDITION_DATE,ID_CONDITION_REPATE,

		ID_CONDITION_MAX10,ID_CONDITION_MAX10P,ID_CONDITION_MIN10,
		ID_CONDITION_MIN10P,ID_CONDITION_UPAVERAGE,ID_CONDITION_DOWNAVERAGE,

		ID_CONDITION_BLUE,ID_CONDITION_GREEN,ID_CONDITION_RED,
		ID_CONDITION_YELLOW,ID_CONDITION_ORANGE,ID_CONDITION_PURPLE,

		ID_CONDITION_GYR,ID_CONDITION_RYG,ID_CONDITION_BYR,ID_CONDITION_RYB,
		ID_CONDITION_YR,ID_CONDITION_RY,ID_CONDITION_YG,ID_CONDITION_GY};
	m_pItemsConditionHighlight->GetImageManager()->SetIcons(ID_GALLERY_CONDITION_HIGHLIGHT, uiGalleryCondition, _countof(uiGalleryCondition), CSize(32, 32));
	m_pItemsConditionHighlight->SetItemSize(CSize(150, 40));
	int nStyleCondition = 0;
	for (; nStyleCondition < 7; nStyleCondition++)
	{
		CXTPImageManagerIcon* pImage = m_pItemsConditionHighlight->GetImageManager()->GetImage(uiGalleryCondition[nStyleCondition], 32);
		m_pItemsConditionHighlight->AddItem(new CGalleryItemVariant(uiGalleryCondition[nStyleCondition], pImage));
	}


	m_pItemsConditionItemSecect = CXTPControlGalleryItems::CreateItems(GetCommandBars(), ID_GALLERY_CONDITION_ITEMSELECT);
	m_pItemsConditionItemSecect->SetItemSize(CSize(150, 40));
	for (; nStyleCondition < 7+6; nStyleCondition++)
	{
		CXTPImageManagerIcon* pImage = m_pItemsConditionHighlight->GetImageManager()->GetImage(uiGalleryCondition[nStyleCondition], 32);
		m_pItemsConditionItemSecect->AddItem(new CGalleryItemVariant(uiGalleryCondition[nStyleCondition], pImage));
	}


	m_pItemsConditionDataBars = CXTPControlGalleryItems::CreateItems(GetCommandBars(), ID_GALLERY_CONDITION_DATABARS);
	m_pItemsConditionDataBars->SetItemSize(CSize(40, 40));
	for (; nStyleCondition < 7+6+6; nStyleCondition++)
	{
		CXTPImageManagerIcon* pImage = m_pItemsConditionHighlight->GetImageManager()->GetImage(uiGalleryCondition[nStyleCondition], 32);
		m_pItemsConditionDataBars->AddItem(new CGalleryItemVariant(uiGalleryCondition[nStyleCondition], _T(""), pImage));
	}


	m_pItemsConditionClrValeur = CXTPControlGalleryItems::CreateItems(GetCommandBars(), ID_GALLERY_CONDITION_CLRVALEUR);
	m_pItemsConditionClrValeur->SetItemSize(CSize(40, 40));
	for (; nStyleCondition < 7+6+6+8; nStyleCondition++)
	{
		CXTPImageManagerIcon* pImage = m_pItemsConditionHighlight->GetImageManager()->GetImage(uiGalleryCondition[nStyleCondition], 32);
		m_pItemsConditionClrValeur->AddItem(new CGalleryItemVariant(uiGalleryCondition[nStyleCondition], _T(""), pImage));
	}


	m_pItemsConditionIconList = CXTPControlGalleryItems::CreateItems(GetCommandBars(), ID_GALLERY_CONDITION_ICONLIST);
	UINT uiGalleryIconList[17];
	for(int i=0; i<_countof(uiGalleryIconList); i++) uiGalleryIconList[i]=35000+i;
	m_pItemsConditionIconList->GetImageManager()->SetIcons(ID_GALLERY_CONDITION_ICONLIST, uiGalleryIconList, _countof(uiGalleryIconList), CSize(24*5, 24));
	m_pItemsConditionIconList->SetItemSize(CSize(24*5+2, 26));
	for (int i = 0; i < _countof(uiGalleryIconList); i++)
	{
		CXTPImageManagerIcon* pImage = m_pItemsConditionIconList->GetImageManager()->GetImage(uiGalleryIconList[i], 24);
		m_pItemsConditionIconList->AddItem(new CGalleryItemVariant(uiGalleryIconList[i], _T(""), pImage));
	}

	
	m_pItemsStylesFormat = CXTPControlGalleryItems::CreateItems(GetCommandBars(), ID_GALLERY_STYLES_FORMAT);
	m_pItemsStylesFormat->SetItemSize(CSize(65, 50));
	m_pItemsStylesFormat->GetImageManager()->SetIcons(ID_GALLERY_STYLES_FORMAT, 0, 0, CSize(63, 48));

	int nStyleFormat = 0;
	pItem = m_pItemsStylesFormat->AddLabel(0);
	pItem->SetCaption(_T("淡色"));
	for (; nStyleFormat < 21; nStyleFormat++)
		m_pItemsStylesFormat->AddItem(nStyleFormat, nStyleFormat);

	pItem = m_pItemsStylesFormat->AddLabel(0);
	pItem->SetCaption(_T("中等深浅"));
	for (; nStyleFormat < 21+28; nStyleFormat++)
		m_pItemsStylesFormat->AddItem(nStyleFormat, nStyleFormat);

	pItem = m_pItemsStylesFormat->AddLabel(0);
	pItem->SetCaption(_T("深色"));
	for (; nStyleFormat < 21+28+11; nStyleFormat++)
		m_pItemsStylesFormat->AddItem(nStyleFormat, nStyleFormat);


	m_pItemsStylesCell = CXTPControlGalleryItems::CreateItems(GetCommandBars(), ID_GALLERY_STYLES_CELL);
	m_pItemsStylesCell->SetItemSize(CSize(104, 26));
	m_pItemsStylesCell->GetImageManager()->SetIcons(ID_GALLERY_STYLES_CELL, 0, 0, CSize(102, 24));

	int nStylescell = 0;
	pItem = m_pItemsStylesCell->AddLabel(0);
	pItem->SetCaption(_T("好、差和适中"));
	for (; nStylescell < 4; nStylescell++)
		m_pItemsStylesCell->AddItem(nStylescell, nStylescell);

	pItem = m_pItemsStylesCell->AddLabel(0);
	pItem->SetCaption(_T("数据和模型"));
	for (; nStylescell < 4 + 8; nStylescell++)
		m_pItemsStylesCell->AddItem(nStylescell, nStylescell);

	pItem = m_pItemsStylesCell->AddLabel(0);
	pItem->SetCaption(_T("标题"));
	for (; nStylescell < 4 + 8 + 6; nStylescell++)
		m_pItemsStylesCell->AddItem(nStylescell, nStylescell);

	pItem = m_pItemsStylesCell->AddLabel(0);
	pItem->SetCaption(_T("主题单元格样式"));
	for (; nStylescell < 4 + 8 + 6 + 24; nStylescell++)
		m_pItemsStylesCell->AddItem(nStylescell, nStylescell);

	pItem = m_pItemsStylesCell->AddLabel(0);
	pItem->SetCaption(_T("数字格式"));
	for (; nStylescell < 4 + 8 + 6 + 24 + 5; nStylescell++)
		m_pItemsStylesCell->AddItem(nStylescell, nStylescell);

	
	m_pItemsNumber = CXTPControlGalleryItems::CreateItems(GetCommandBars(), ID_GALLERY_NUMBER);
	UINT uiNumber[] = {ID_NRFMT_GENERAL, ID_NRFMT_NUMBER, ID_NRFMT_CURRENCY, ID_NRFMT_ACCOUNTANT, ID_NRFMT_SHORTDATE, 
		ID_NRFMT_LONGDATE, ID_NRFMT_TIME,ID_NRFMT_PERCENT,ID_NRFMT_FRACTION,ID_NRFMT_SCIENCE,ID_NRFMT_TEXT}; 
	m_pItemsNumber->GetImageManager()->SetIcons(ID_GALLERY_NUMBER, uiNumber, _countof(uiNumber), CSize(32, 32));
	m_pItemsNumber->SetItemSize(CSize(150, 40));
	for(int i=0; i<_countof(uiNumber); i++)
	{
		CXTPImageManagerIcon* pImage = m_pItemsNumber->GetImageManager()->GetImage(uiNumber[i], 32);
		m_pItemsNumber->AddItem(new CGalleryItemVariant(uiNumber[i], pImage));
	}


	m_idColors = 0;
	m_pItemsColors = CXTPControlGalleryItems::CreateItems(GetCommandBars(), ID_GALLERY_COLORS);
	m_pItemsColors->SetItemSize(CSize(0, 20));

	pItem = m_pItemsColors->AddLabel(0);
	pItem->SetCaption(_T("Built-In"));
	m_pItemsColors->AddItem(new CGalleryItemThemeColors(0, _T("Office")));
	m_pItemsColors->AddItem(new CGalleryItemThemeColors(1, _T("Grayscale")));
	m_pItemsColors->AddItem(new CGalleryItemThemeColors(2, _T("Apex")));
	m_pItemsColors->AddItem(new CGalleryItemThemeColors(3, _T("Aspect")));
	m_pItemsColors->AddItem(new CGalleryItemThemeColors(4, _T("Civic")));
	m_pItemsColors->AddItem(new CGalleryItemThemeColors(5, _T("Concourse")));
	m_pItemsColors->AddItem(new CGalleryItemThemeColors(6, _T("Currency")));
	m_pItemsColors->AddItem(new CGalleryItemThemeColors(7, _T("Deluxe")));


	m_idFonts = 0;
	m_pItemsFonts = CXTPControlGalleryItems::CreateItems(GetCommandBars(), ID_GALLERY_FONTS);
	m_pItemsFonts->SetItemSize(CSize(0, 60));

	pItem = m_pItemsFonts->AddLabel(0);
	pItem->SetCaption(_T("Built-In"));
	m_pItemsFonts->AddItem(new CGalleryItemThemeFonts(0, _T("Office")));
	m_pItemsFonts->AddItem(new CGalleryItemThemeFonts(1, _T("Apex")));
	m_pItemsFonts->AddItem(new CGalleryItemThemeFonts(2, _T("Aspect")));
	m_pItemsFonts->AddItem(new CGalleryItemThemeFonts(3, _T("Civic")));
	m_pItemsFonts->AddItem(new CGalleryItemThemeFonts(4, _T("Concourse")));
	m_pItemsFonts->AddItem(new CGalleryItemThemeFonts(5, _T("Currency")));
	m_pItemsFonts->AddItem(new CGalleryItemThemeFonts(6, _T("Deluxe")));
	m_pItemsFonts->AddItem(new CGalleryItemThemeFonts(7, _T("Equity")));


	m_pItemsFontTextColor = CXTPControlGalleryItems::CreateItems(GetCommandBars(), ID_GALLERY_FONT_FACECOLOR);
	m_pItemsFontTextColor->SetItemSize(CSize(17, 17 ));

	pItem = m_pItemsFontTextColor->AddLabel(0);
	pItem->SetCaption(_T("Theme Colors"));
	CGalleryItemFontColor::AddThemeColors(m_pItemsFontTextColor, 0);

	pItem = m_pItemsFontTextColor->AddLabel(0);
	pItem->SetCaption(_T("Standard Colors"));
	CGalleryItemFontColor::AddStandardColors(m_pItemsFontTextColor);


	m_pItemsFontBackColor = CXTPControlGalleryItems::CreateItems(GetCommandBars(), ID_GALLERY_FONT_BACKCOLOR);
	m_pItemsFontBackColor->SetItemSize(CSize(30, 30));
	CGalleryItemFontColor::AddStandardBackColors(m_pItemsFontBackColor);


	m_pItemsFontFace = CXTPControlGalleryItems::CreateItems(GetCommandBars(), ID_GALLERY_FONT_FACE);
	m_pItemsFontFace->SetItemSize(CSize(0, 26));
	CGalleryItemFontFace::AddFontItems(m_pItemsFontFace);


	m_pItemsFontSize = CXTPControlGalleryItems::CreateItems(GetCommandBars(), ID_GALLERY_FONT_SIZE);
	m_pItemsFontSize->SetItemSize(CSize(0, 17));
	m_pItemsFontSize->AddItem(_T("8"));
	m_pItemsFontSize->AddItem(_T("9"));
	m_pItemsFontSize->AddItem(_T("10"));
	m_pItemsFontSize->AddItem(_T("11"));
	m_pItemsFontSize->AddItem(_T("12"));
	m_pItemsFontSize->AddItem(_T("14"));
	m_pItemsFontSize->AddItem(_T("16"));
	m_pItemsFontSize->AddItem(_T("18"));
	m_pItemsFontSize->AddItem(_T("20"));
	m_pItemsFontSize->AddItem(_T("22"));
	m_pItemsFontSize->AddItem(_T("24"));
	m_pItemsFontSize->AddItem(_T("26"));
	m_pItemsFontSize->AddItem(_T("28"));
	m_pItemsFontSize->AddItem(_T("36"));
	m_pItemsFontSize->AddItem(_T("48"));
	m_pItemsFontSize->AddItem(_T("72"));


	m_pItemsSearchdatapart = CXTPControlGalleryItems::CreateItems(GetCommandBars(), ID_GALLERY_SEARCH_DATAPART);
	m_pItemsSearchdatapart->SetItemSize(CSize(0, 20));


	m_pItemsUndo = CXTPControlGalleryItems::CreateItems(GetCommandBars(), ID_EDIT_UNDO);
	m_pItemsUndo->SetItemSize(CSize(0, 21));
	m_pItemsUndo->ClipItems(FALSE);

	return TRUE;
}

BOOL CMainFrame::CreateMiniToolBar()
{
	CXTPCommandBars* pCommandBars = GetCommandBars();

	CXTPMiniToolBar* pMiniToolBar = CXTPMiniToolBar::CreateMiniToolBar(pCommandBars);
	pMiniToolBar->SetBarID(IDR_MENU_MINITOOLBAR);
	pMiniToolBar->SetTitle(_T("MiniBar"));

	CXTPControlComboBox* pComboFont = new CXTPControlComboBox();
	pMiniToolBar->GetControls()->Add(pComboFont, ID_FONT_FACE);

	pComboFont->SetDropDownListStyle();
	pComboFont->EnableAutoComplete();
	pComboFont->SetWidth(64);

	CXTPPopupBar* pPopupBarGallery = CXTPControlComboBoxGalleryPopupBar::CreateComboBoxGalleryPopupBar(pCommandBars);

	pComboFont->SetCommandBar(pPopupBarGallery);

	CXTPControlGallery* pControlGallery = new CXTPControlGallery();
	pControlGallery->SetControlSize(CSize(290, 508));
	pControlGallery->SetResizable(FALSE, TRUE);
	pControlGallery->ShowLabels(TRUE);
	pControlGallery->SetItems(m_pItemsFontFace);

	pPopupBarGallery->GetControls()->Add(pControlGallery, ID_GALLERY_FONT_FACE);

	pPopupBarGallery->InternalRelease();



	pComboFont = new CXTPControlComboBox();
	pMiniToolBar->GetControls()->Add(pComboFont, ID_FONT_SIZE);

	pComboFont->SetDropDownListStyle();
	pComboFont->SetWidth(45);

	pPopupBarGallery = CXTPControlComboBoxGalleryPopupBar::CreateComboBoxGalleryPopupBar(pCommandBars);

	pComboFont->SetCommandBar(pPopupBarGallery);

	pControlGallery = new CXTPControlGallery();
	pControlGallery->SetControlSize(CSize(42, 16 * 17));
	pControlGallery->SetResizable(FALSE, TRUE);
	pControlGallery->SetItems(m_pItemsFontSize);

	pPopupBarGallery->GetControls()->Add(pControlGallery, ID_GALLERY_FONT_SIZE);

	pPopupBarGallery->InternalRelease();

	pMiniToolBar->GetControls()->Add(xtpControlButton, ID_FONT_GROW);
	pMiniToolBar->GetControls()->Add(xtpControlButton, ID_FONT_SHRINK);
	pMiniToolBar->GetControls()->Add(xtpControlButtonPopup, ID_STYLES_CELL);

	pMiniToolBar->GetControls()->Add(xtpControlButton, ID_FONT_BOLD);
	pMiniToolBar->GetControls()->Add(xtpControlButton, ID_FONT_ITALIC);

	pMiniToolBar->GetControls()->Add(xtpControlButton, ID_ALIGNMENT_LEFT);
	pMiniToolBar->GetControls()->Add(xtpControlButton, ID_ALIGNMENT_CENTER);
	pMiniToolBar->GetControls()->Add(xtpControlButton, ID_ALIGNMENT_RIGHT);

	pMiniToolBar->GetControls()->Add(xtpControlButton, ID_ALIGNMENT_INCREASEINDENT);
	pMiniToolBar->GetControls()->Add(xtpControlButton, ID_ALIGNMENT_DECREASEINDENT);
	pMiniToolBar->GetControls()->Add(xtpControlButton, ID_ALIGNMENT_UP);


	pMiniToolBar->GetControls()->CreateOriginalControls();

	pCommandBars->GetContextMenus()->Add(pMiniToolBar);
	pMiniToolBar->SetWidth(22 * 8 + 4);

	return TRUE;
}

BOOL CMainFrame::CreateRibbonBar()
{
	try
	{
		CXTPCommandBars* pCommandBars = GetCommandBars();

		CMenu menu;
		menu.Attach(::GetMenu(m_hWnd));
		SetMenu(NULL);

		m_pRibbonBar = (CXTPRibbonBar*)pCommandBars->Add(_T("The Ribbon"), xtpBarTop, RUNTIME_CLASS(CXTPRibbonBar));
		if (!m_pRibbonBar) return FALSE;

		m_pRibbonBar->EnableDocking();

		CXTPControlPopup* pControlFile = (CXTPControlPopup*)m_pRibbonBar->AddSystemButton(ID_MENU_FILE);
		pControlFile->SetIconId(IDP_RIBBON_GEAR);

		UINT uCommand = {IDP_RIBBON_GEAR};
		pCommandBars->GetImageManager()->SetIcons(IDP_RIBBON_GEAR, &uCommand, 1, CSize(0, 0), xtpImageNormal);

		CreateBackstage();

		vector<CXTPRibbonTab*> vecTabs;

		//开始面板
		CXTPRibbonTab* pTabHome = m_pRibbonBar->AddTab(ID_TAB_HOME);

		CXTPRibbonGroup* pGroupDatabase = pTabHome->AddGroup(ID_GROUP_DATABASE);
		pGroupDatabase->ShowOptionButton();
		pGroupDatabase->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupDatabase->GetControlGroupOption()->SetID(ID_GROUP_DATABASE_OPTION);
		pGroupDatabase->SetControlsCentering();

		CXTPControlPopup* pControlDataSql = (CXTPControlPopup*)pGroupDatabase->Add(xtpControlSplitButtonPopup, ID_DATABASE_TABLE);
		pControlDataSql->GetCommandBar()->GetControls()->Add(xtpControlButton, ID_DATABASE_MSSQL);
		pControlDataSql->GetCommandBar()->GetControls()->Add(xtpControlButton, ID_DATABASE_EXCEL);
		pControlDataSql->GetCommandBar()->GetControls()->Add(xtpControlButton, ID_DATABASE_ACCESS);

		CXTPRibbonGroup* pGroupClipborad = pTabHome->AddGroup(ID_GROUP_CLIPBOARD);
		pGroupClipborad->ShowOptionButton();
		pGroupClipborad->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupClipborad->GetControlGroupOption()->SetID(ID_GROUP_CLIPBOARD_OPTION);
		pGroupClipborad->SetControlsCentering();

		CXTPControlPopup* pControlPaste = (CXTPControlPopup*)pGroupClipborad->Add(xtpControlSplitButtonPopup, ID_EDIT_PASTE);
		pControlPaste->GetCommandBar()->GetControls()->Add(xtpControlButton, ID_EDIT_PASTE);
		pControlPaste->GetCommandBar()->GetControls()->Add(xtpControlButton, ID_EDIT_PASTE_SPECIAL);
		CXTPControl* pControlCut = pGroupClipborad->Add(xtpControlButton, ID_EDIT_CUT);
		pControlCut->SetKeyboardTip(_T("X"));

		pGroupClipborad->Add(xtpControlButton, ID_EDIT_COPY);	
		pGroupClipborad->Add(xtpControlButton, ID_EDIT_FORMAT_PAINTER);
		pControlPaste->SetKeyboardTip(_T("V"));

		CXTPRibbonGroup* pGroupFont = pTabHome->AddGroup(ID_GROUP_FONT);
		pGroupFont->SetControlsGrouping();
		pGroupFont->ShowOptionButton();
		pGroupFont->GetControlGroupOption()->SetID(ID_GROUP_FONT_OPTION);

		CXTPControlComboBox* pComboFont = new CXTPControlComboBox();
		pGroupFont->Add(pComboFont, ID_FONT_FACE);

		pComboFont->SetDropDownListStyle();
		pComboFont->EnableAutoComplete();
		pComboFont->SetWidth(130);

		CXTPPopupBar* pPopupBarGallery = CXTPControlComboBoxGalleryPopupBar::CreateComboBoxGalleryPopupBar(pCommandBars);
		pComboFont->SetCommandBar(pPopupBarGallery);

		CXTPControlGallery* pControlGallery = new CXTPControlGallery();
		pControlGallery->SetControlSize(CSize(290, 26*15));
		pControlGallery->SetResizable(FALSE, TRUE);
		pControlGallery->ShowLabels(TRUE);
		pControlGallery->SetItems(m_pItemsFontFace);
		pControlGallery->SetResizable(FALSE,FALSE);

		pPopupBarGallery->GetControls()->Add(pControlGallery, ID_GALLERY_FONT_FACE);
		pPopupBarGallery->GetControls()->Add(new CXTPControlButton(), ID_FONT_OTHER);
		pPopupBarGallery->InternalRelease();

		pComboFont = new CXTPControlComboBox();
		pGroupFont->Add(pComboFont, ID_FONT_SIZE);

		pComboFont->SetDropDownListStyle();
		pComboFont->SetWidth(45);

		pPopupBarGallery = CXTPControlComboBoxGalleryPopupBar::CreateComboBoxGalleryPopupBar(pCommandBars);
		pComboFont->SetCommandBar(pPopupBarGallery);

		pControlGallery = new CXTPControlGallery();
		pControlGallery->SetControlSize(CSize(42, 17 * 15));
		pControlGallery->SetResizable(FALSE, TRUE);
		pControlGallery->SetItems(m_pItemsFontSize);
		pControlGallery->SetResizable(FALSE,FALSE);

		pPopupBarGallery->GetControls()->Add(pControlGallery, ID_GALLERY_FONT_SIZE);
		pPopupBarGallery->InternalRelease();

		CXTPControl* pControl = pGroupFont->Add(xtpControlButton, ID_FONT_GROW);
		pControl->SetBeginGroup(TRUE);
		pGroupFont->Add(xtpControlButton, ID_FONT_SHRINK);

		pControl = pGroupFont->Add(xtpControlButton, ID_FONT_CLEAR);
		pControl->SetBeginGroup(TRUE);

		pGroupFont->Add(xtpControlButton, ID_FONT_BOLD)->SetBeginGroup(TRUE);
		pGroupFont->Add(xtpControlButton, ID_FONT_ITALIC);
		pGroupFont->Add(xtpControlSplitButtonPopup, ID_FONT_UNDERLINE);	

		pGroupFont->Add(xtpControlSplitButtonPopup, ID_FONT_BORDERS)->SetBeginGroup(TRUE);

		pGroupFont->Add(xtpControlButton, ID_FONT_SUBSCRIPT)->SetBeginGroup(TRUE);
		pGroupFont->Add(xtpControlButton, ID_FONT_SUPERSCRIPT);	
		pControl = pGroupFont->Add(xtpControlSplitButtonPopup, ID_FONT_CHARFIELD);
		pControl->SetStyle(xtpButtonIcon);

		CXTPControlPopupColor* pPopupColor = (CXTPControlPopupColor*)pGroupFont->Add(new CXTPControlPopupColor(), ID_FONT_BACKCOLOR);
		pPopupColor->SetBeginGroup(TRUE);
		CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());
		CMenu menuColor;
		menuColor.LoadMenu(ID_FONT_BACKCOLOR);
		pPopupBar->LoadMenu(menuColor.GetSubMenu(0));
		pPopupColor->SetCommandBar(pPopupBar);
		pPopupBar->InternalRelease();

		pPopupColor = (CXTPControlPopupColor*)pGroupFont->Add(new CXTPControlPopupColor, ID_FONT_FACECOLOR);
		pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());

		CXTPControlButton *pButton = (CXTPControlButton *)pPopupBar->GetControls()->Add(new CXTPControlButton(), XTP_IDS_AUTOMATIC);

		pControlGallery = new CXTPControlGallery();
		pControlGallery->ShowLabels(TRUE);
		pControlGallery->ShowScrollBar(FALSE);
		pControlGallery->SetItems(m_pItemsFontTextColor);
		pControlGallery->ShowScrollBar(FALSE);
		pControlGallery->SetResizable(FALSE,FALSE);

		pControlGallery->SetControlSize( 7, 10, CSize(17, 15 ) );
		pControlGallery->SetHeight(pControlGallery->GetHeight() + 2); // + additional padding

		pPopupBar->GetControls()->Add(pControlGallery, ID_GALLERY_FONT_FACECOLOR);
		pPopupBar->GetControls()->Add(new CXTPControlButton(), XTP_IDS_MORE_COLORS);

		pPopupColor->SetCommandBar(pPopupBar);
		pPopupBar->EnableCustomization(FALSE);
		pPopupBar->InternalRelease();

		CXTPRibbonGroup* pGroupAlignment = pTabHome->AddGroup(ID_GROUP_ALIGNMENT);
		pGroupAlignment->SetControlsGrouping();
		pGroupAlignment->ShowOptionButton();
		pGroupAlignment->GetControlGroupOption()->SetID(ID_GROUP_ALIGNMENT_OPTION);

		pGroupAlignment->Add(xtpControlButton, ID_ALIGNMENT_UP);
		pGroupAlignment->Add(xtpControlButton, ID_ALIGNMENT_MIDDLE);
		pGroupAlignment->Add(xtpControlButton, ID_ALIGNMENT_DOWN);

		pGroupAlignment->Add(xtpControlSplitButtonPopup, ID_ALIGNMENT_DIRECT)->SetBeginGroup(TRUE);
		pGroupAlignment->Add(xtpControlButton, ID_ALIGNMENT_SORT);

		pControl = pGroupAlignment->Add(xtpControlButton, ID_ALIGNMENT_WRAP);
		pControl->SetBeginGroup(TRUE);
		pControl->SetStyle(xtpButtonIconAndCaption);

		pGroupAlignment->Add(xtpControlButton, ID_ALIGNMENT_LEFT)->SetBeginGroup(TRUE);
		pGroupAlignment->Add(xtpControlButton, ID_ALIGNMENT_CENTER);
		pGroupAlignment->Add(xtpControlButton, ID_ALIGNMENT_RIGHT);

		pGroupAlignment->Add(xtpControlButton, ID_ALIGNMENT_DECREASEINDENT)->SetBeginGroup(TRUE);
		pGroupAlignment->Add(xtpControlButton, ID_ALIGNMENT_INCREASEINDENT);

		CXTPControlPopup* pPopupMerge = (CXTPControlPopup*)pGroupAlignment->Add(xtpControlSplitButtonPopup, ID_ALIGNMENT_MERGE);
		pPopupMerge->SetBeginGroup(TRUE);
		pPopupMerge->SetStyle(xtpButtonIconAndCaption);
		pPopupMerge->SetFlags(xtpFlagManualUpdate);

		CXTPRibbonGroup* pGroupNumber = pTabHome->AddGroup(ID_GROUP_NUMBER);
		pGroupNumber->SetControlsGrouping();
		pGroupNumber->ShowOptionButton();
		pGroupNumber->GetControlGroupOption()->SetID(ID_GROUP_NUMBER_OPTION);

		CXTPControlComboBox* pComboNumber = new CXTPControlComboBox();
		pGroupNumber->Add(pComboNumber, ID_NUMBER_FORMAT);
		pComboNumber->SetDropDownListStyle();
		pComboNumber->EnableAutoComplete();
		pComboNumber->SetWidth(120);

		pPopupBarGallery = CXTPControlComboBoxGalleryPopupBar::CreateComboBoxGalleryPopupBar(pCommandBars);
		pComboNumber->SetCommandBar(pPopupBarGallery);

		pControlGallery = new CXTPControlGallery();
		pControlGallery->SetControlSize(CSize(120, 40*8));
		pControlGallery->SetResizable(FALSE, TRUE);
		pControlGallery->ShowLabels(TRUE);
		pControlGallery->SetItems(m_pItemsNumber);
		pControlGallery->SetResizable(FALSE,FALSE);

		pPopupBarGallery->GetControls()->Add(pControlGallery, ID_GALLERY_NUMBER);
		pPopupBarGallery->GetControls()->Add(new CXTPControlButton(), ID_NRFMT_OTHER);
		pPopupBarGallery->InternalRelease();

		CXTPControlPopup* pPopupAccountant = (CXTPControlPopup*)pGroupNumber->Add(xtpControlSplitButtonPopup, ID_NUMBER_ACCOUNTANT);
		pPopupAccountant->SetBeginGroup(TRUE);
		pPopupAccountant->SetFlags(xtpFlagManualUpdate);
		pGroupNumber->Add(xtpControlButton, ID_NUMBER_PERCENT);
		pGroupNumber->Add(xtpControlButton, ID_NUMBER_KILOBITGAP);
		pGroupNumber->Add(xtpControlButton, ID_NUMBER_INPRECISION);
		pGroupNumber->Add(xtpControlButton, ID_NUMBER_ENPRECISION);

		CXTPRibbonGroup* pGroupStyles = pTabHome->AddGroup(ID_GROUP_STYLES);
		pGroupStyles->SetControlsCentering(TRUE);
		pGroupStyles->ShowOptionButton();
		pGroupStyles->GetControlGroupOption()->SetID(ID_GROUP_STYLES_OPTION);

		CXTPControlPopup* pPopupCondition = (CXTPControlPopup*)pGroupStyles->Add(xtpControlSplitButtonPopup, ID_STYLES_CONDITION);
		pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());
		pPopupCondition->SetCommandBar(pPopupBar);
		pPopupBar->InternalRelease();
		CMenu menuCondition;
		menuCondition.LoadMenu(ID_STYLES_CONDITION);
		pPopupBar->LoadMenu(menuCondition.GetSubMenu(0));
		pPopupBar->SetIconSize(CSize(32, 32));

		pGroupStyles->Add(xtpControlSplitButtonPopup, ID_STYLES_FORMAT)->SetBeginGroup(TRUE);
		pControlGallery = (CXTPControlGallery*)pGroupStyles->Add(new CXTPControlGallery(), ID_STYLES_CELL);
		pControlGallery->SetBeginGroup(TRUE);
		pControlGallery->SetControlSize(CSize(230, 60));
		pControlGallery->SetResizable();
		pControlGallery->ShowBorders(TRUE);
		pControlGallery->SetItemsMargin(2, 2, 2, 2);
		pControlGallery->ShowLabels(FALSE);
		pControlGallery->SetItems(m_pItemsStylesCell);
		pControlGallery->SetResizable(FALSE,FALSE);

		CMenu menuStylescell;
		menuStylescell.LoadMenu(ID_STYLES_CELL);

		pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());
		pPopupBar->LoadMenu(menuStylescell.GetSubMenu(0));

		pControlGallery->SetCommandBar(pPopupBar);
		pPopupBar->EnableAnimation();
		pPopupBar->InternalRelease();

		CXTPRibbonGroup* pGroupCell = pTabHome->AddGroup(ID_GROUP_CELL);
		pGroupCell->SetControlsCentering();
		pGroupCell->ShowOptionButton();
		pGroupCell->GetControlGroupOption()->SetID(ID_GROUP_CELL_OPTION);

		pGroupCell->Add(xtpControlSplitButtonPopup, ID_CELL_INSERT)->SetBeginGroup(TRUE);
		pGroupCell->Add(xtpControlSplitButtonPopup, ID_CELL_DELETE)->SetBeginGroup(TRUE);
		pGroupCell->Add(xtpControlSplitButtonPopup, ID_CELL_FORMAT)->SetBeginGroup(TRUE);

		CXTPRibbonGroup* pGroupModify = pTabHome->AddGroup(ID_GROUP_MODIFY);
		pGroupModify->SetControlsCentering();
		pGroupModify->ShowOptionButton();
		pGroupModify->GetControlGroupOption()->SetID(ID_GROUP_MODIFY_OPTION);

		pGroupModify->Add(xtpControlSplitButtonPopup, ID_MODIFY_SUM);
		pGroupModify->Add(xtpControlSplitButtonPopup, ID_MODIFY_FILL);
		pGroupModify->Add(xtpControlSplitButtonPopup, ID_MODIFY_CLEAR);

		pGroupModify->Add(xtpControlSplitButtonPopup, ID_MODIFY_SORTFILTER)->SetBeginGroup(TRUE);
		pGroupModify->Add(xtpControlSplitButtonPopup, ID_MODIFY_FINDSELECT)->SetBeginGroup(TRUE);
		m_pPopupZxing = (CXTPControlPopup*)pGroupModify->Add(xtpControlSplitButtonPopup, ID_ZXING_FORMAT);
		m_pPopupZxing->SetBeginGroup(TRUE);
		m_pPopupZxing->SetFlags(xtpFlagManualUpdate);


		//系统面板
		CXTPRibbonTab* pTabSystem = m_pRibbonBar->AddTab(ID_TAB_SYSTEM);
		vecTabs.push_back(pTabSystem);

		CXTPRibbonGroup* pGroupDataset = pTabSystem->AddGroup(ID_GROUP_DATASET);
		pGroupDataset->ShowOptionButton();
		pGroupDataset->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupDataset->GetControlGroupOption()->SetID(ID_GROUP_DATASET_OPTION);
		pGroupDataset->SetControlsCentering();

		CXTPControlPopup* pControlDatabackup = (CXTPControlPopup*)pGroupDataset->Add(xtpControlSplitButtonPopup, ID_DATASET_DATBACKUP);
		pControlDatabackup->GetCommandBar()->GetControls()->Add(xtpControlButton, ID_DATASET_DATRESTORE);
		pControlDatabackup->GetCommandBar()->GetControls()->Add(xtpControlButton, ID_DATASET_DATCLEAR);

		CXTPControlPopup* pControlRegsur = (CXTPControlPopup*)pGroupDataset->Add(xtpControlSplitButtonPopup, ID_USERMGR_REGUSER);
		pControlRegsur->GetCommandBar()->GetControls()->Add(xtpControlButton, ID_USERMGR_SETUSER);
		pControlRegsur->GetCommandBar()->GetControls()->Add(xtpControlButton, ID_USERMGR_DELUSER);
		pControlRegsur->GetCommandBar()->GetControls()->Add(xtpControlButton, ID_USERMGR_USERINFO);
		pControlRegsur->GetCommandBar()->GetControls()->Add(xtpControlButton, ID_USERMGR_AUTOLOGIN);
		pControlRegsur->SetBeginGroup(TRUE);

		CXTPRibbonGroup* pGroupProgram = pTabSystem->AddGroup(ID_GROUP_PROGRAM);
		pGroupProgram->ShowOptionButton();
		pGroupProgram->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupProgram->GetControlGroupOption()->SetID(ID_GROUP_PROGRAM_OPTION);
		pGroupProgram->SetControlsCentering();

		pGroupProgram->Add(xtpControlButton, ID_PROGRAM_TASKTRACK);
		pGroupProgram->Add(xtpControlButton, ID_PROGRAM_METHOD);
		pGroupProgram->Add(xtpControlButton, ID_PROGRAM_CHART);
		pGroupProgram->Add(xtpControlButton, ID_PROGRAM_REPORT);

		CXTPRibbonGroup* pGroupShapes = pTabSystem->AddGroup(ID_GROUP_SHAPES);
		pGroupShapes->ShowOptionButton();
		pGroupShapes->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupShapes->GetControlGroupOption()->SetID(ID_GROUP_SHAPES_OPTION);
		pGroupShapes->SetControlsCentering();

		pControlGallery = (CXTPControlGallery*)pGroupShapes->Add(new CXTPControlGallery(), ID_GALLERY_SHAPES);	
		pControlGallery->SetControlSize(CSize(200, 60));
		pControlGallery->SetResizable();
		pControlGallery->ShowBorders(TRUE);
		pControlGallery->SetItemsMargin(0, -1, 0, -1);
		pControlGallery->ShowLabels(FALSE);
		pControlGallery->SetItems(m_pItemsShapes);
		pControlGallery->SetResizable(FALSE,FALSE);

		CMenu menuShapes;
		menuShapes.LoadMenu(ID_GALLERY_SHAPES);

		pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());
		pPopupBar->LoadMenu(menuShapes.GetSubMenu(0));

		pControlGallery->SetCommandBar(pPopupBar);
		pPopupBar->EnableAnimation();
		pPopupBar->InternalRelease();

		//工点表
		CXTPRibbonTab* pTabPointsmgr = m_pRibbonBar->AddTab(ID_TAB_POINTSMGR);
		vecTabs.push_back(pTabPointsmgr);

		CXTPRibbonGroup* pGroupReport = pTabPointsmgr->AddGroup(ID_GROUP_REPORT_SET);
		pGroupReport->ShowOptionButton();
		pGroupReport->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupReport->GetControlGroupOption()->SetID(ID_GROUP_REPORT_SET_OPTION);
		pGroupReport->SetControlsCentering();

		pGroupReport->Add(xtpControlButton, ID_REPORT_SET_EDITABLE);
		pGroupReport->Add(xtpControlButton, ID_REPORT_SET_ROWHEAD);
		pGroupReport->Add(xtpControlButton, ID_REPORT_SET_ROWFOOT);

		CXTPRibbonGroup* pGroupModift = pTabPointsmgr->AddGroup(ID_GROUP_REPORT_MODIFY);
		pGroupModift->ShowOptionButton();
		pGroupModift->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupModift->GetControlGroupOption()->SetID(ID_GROUP_REPORT_MODIFY_OPTION);
		pGroupModift->SetControlsCentering();

		pGroupModift->Add(xtpControlButton, ID_REPORT_MODIFY_ADD);
		pGroupModift->Add(xtpControlButton, ID_REPORT_MODIFY_SET);
		pGroupModift->Add(xtpControlButton, ID_REPORT_MODIFY_DEL);

		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_BACKUP)->SetBeginGroup(TRUE);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_RESTORE);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_DROP);

		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_SUBLIST)->SetBeginGroup(TRUE);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_FILTEREDIT);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_FILTERCLEAR);

		CXTPRibbonGroup* pGroupPointsdata = pTabPointsmgr->AddGroup(ID_GROUP_POINTSMGR);
		pGroupPointsdata->SetControlsCentering();
		pGroupPointsdata->ShowOptionButton();
		pGroupPointsdata->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupPointsdata->GetControlGroupOption()->SetID(ID_GROUP_POINTSMGR_OPTION);

		pGroupPointsdata->Add(xtpControlButton, ID_POINTSMGR_POINTS);
		pGroupPointsdata->Add(xtpControlButton, ID_POINTSMGR_LEDGER);

		m_pPopupPoints = (CXTPControlPopup*)pGroupPointsdata->Add(xtpControlSplitButtonPopup, ID_POINTSMGR_TABLE);
		m_pPopupPoints->SetFlags(xtpFlagManualUpdate);

		pGroupPointsdata->Add(xtpControlButton, IDR_PANE_PROPERTIES)->SetBeginGroup(TRUE);
		pGroupPointsdata->Add(xtpControlButton, IDR_PANE_RIBBON);
		pGroupPointsdata->Add(xtpControlButton, IDR_PANE_POINTS);

		CXTPRibbonGroup* pGroupPointsserch = pTabPointsmgr->AddGroup(ID_GROUP_SEARCH);
		pGroupPointsserch->SetControlsGrouping();
		pGroupPointsserch->ShowOptionButton();
		pGroupPointsserch->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupPointsserch->GetControlGroupOption()->SetID(ID_GROUP_SEARCH_OPTION);

		m_pSearchDatapart = new CXTPControlComboBox();
		pGroupPointsserch->Add(m_pSearchDatapart, ID_POINTSMGR_DATAPART)->SetStyle(xtpButtonIconAndCaption);
		m_pSearchDatapart->SetDropDownListStyle(FALSE);
		m_pSearchDatapart->SetWidth(250);
		m_pSearchDatapart->SetStyle(xtpButtonIconAndCaption);
		m_pSearchDatapart->SetEditHint(_T("选择项目工点段落"));

		pPopupBarGallery = CXTPControlComboBoxGalleryPopupBar::CreateComboBoxGalleryPopupBar(pCommandBars);
		m_pSearchDatapart->SetCommandBar(pPopupBarGallery);

		pControlGallery = new CXTPControlGallery();
		pControlGallery->SetControlSize(CSize(250, 20*15));
		pControlGallery->SetResizable(FALSE, TRUE);
		pControlGallery->ShowLabels(TRUE);
		pControlGallery->SetItems(m_pItemsSearchdatapart);

		pPopupBarGallery->GetControls()->Add(pControlGallery, ID_GALLERY_SEARCH_DATAPART);
		pPopupBarGallery->InternalRelease();

		m_pSearchDateEdit = (CXTPControlEdit*)pGroupPointsserch->Add(xtpControlEdit, ID_POINTSMGR_DATEEDIT);
		m_pSearchDateEdit->SetWidth(230);
		m_pSearchDateEdit->ShowSpinButtons();
		//m_pSearchDateEdit->ShowBuddyButton(ID_POINTSMGR_DATEBUTTON);
		m_pSearchDateEdit->SetBeginGroup(TRUE);
		m_pSearchDateEdit->SetStyle(xtpButtonIconAndCaption);
		m_pSearchDateEdit->SetEditHint(_T("选择查询日期时间"));

		CXTPControlPopup* pControlDtaepopup = (CXTPControlPopup*)pGroupPointsserch->Add(xtpControlButtonPopup, ID_POINTSMGR_DATEPOPUP);
		pControlDtaepopup->SetStyle(xtpButtonIcon);

		pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());

		m_pSearchDatePicker = new CXTPDatePickerControl;
		if (!m_pSearchDatePicker->Create(WS_TABSTOP | WS_CHILD | WS_BORDER | DTS_SHORTDATEFORMAT,
			CRect(0, 0, 205, 205), this, ID_POINTSMGR_DATEPICKER)) return FALSE;
		m_pSearchDatePicker->SetButtonsVisible(TRUE, FALSE);
		m_pSearchDatePicker->SetShowWeekNumbers(TRUE);
		CXTPControlCustom* pCustomDateTime = CXTPControlCustom::CreateControlCustom(m_pSearchDatePicker);
		pCustomDateTime->SetFlags(xtpFlagManualUpdate);

		pPopupBar->GetControls()->Add(pCustomDateTime, ID_POINTSMGR_DATECTRL);
		pControlDtaepopup->SetCommandBar(pPopupBar);
		pPopupBar->EnableCustomization(FALSE);
		pPopupBar->InternalRelease();


		//成本控制
		CXTPRibbonTab* pTabCostmgr = m_pRibbonBar->AddTab(ID_TAB_COSTMGR);
		vecTabs.push_back(pTabCostmgr);

		pGroupReport = pTabCostmgr->AddGroup(ID_GROUP_REPORT_SET);
		pGroupReport->ShowOptionButton();
		pGroupReport->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupReport->GetControlGroupOption()->SetID(ID_GROUP_REPORT_SET_OPTION);
		pGroupReport->SetControlsCentering();

		pGroupReport->Add(xtpControlButton, ID_REPORT_SET_EDITABLE);
		pGroupReport->Add(xtpControlButton, ID_REPORT_SET_ROWHEAD);
		pGroupReport->Add(xtpControlButton, ID_REPORT_SET_ROWFOOT);

		pGroupModift = pTabCostmgr->AddGroup(ID_GROUP_REPORT_MODIFY);
		pGroupModift->ShowOptionButton();
		pGroupModift->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupModift->GetControlGroupOption()->SetID(ID_GROUP_REPORT_MODIFY_OPTION);
		pGroupModift->SetControlsCentering();

		pGroupModift->Add(xtpControlButton, ID_REPORT_MODIFY_ADD);
		pGroupModift->Add(xtpControlButton, ID_REPORT_MODIFY_SET);
		pGroupModift->Add(xtpControlButton, ID_REPORT_MODIFY_DEL);

		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_BACKUP)->SetBeginGroup(TRUE);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_RESTORE);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_DROP);

		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_SUBLIST)->SetBeginGroup(TRUE);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_FILTEREDIT);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_FILTERCLEAR);

		CXTPRibbonGroup* pGroupCostdata = pTabCostmgr->AddGroup(ID_GROUP_COSTMGR);
		pGroupCostdata->ShowOptionButton();
		pGroupCostdata->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupCostdata->GetControlGroupOption()->SetID(ID_GROUP_COSTMGR_OPTION);
		pGroupCostdata->SetControlsCentering();

		m_pPopupCost = (CXTPControlPopup*)pGroupCostdata->Add(xtpControlSplitButtonPopup, ID_COSTMGR_TABLE);
		m_pPopupCost->SetFlags(xtpFlagManualUpdate);

		pGroupCostdata->Add(xtpControlButton, ID_COSTMGR_DAT);
		pGroupCostdata->Add(xtpControlButton, ID_COSTMGR_SUM)->SetBeginGroup(TRUE);
		pGroupCostdata->Add(xtpControlButton, ID_COSTMGR_ALL);
		pGroupCostdata->Add(xtpControlButton, ID_REPORT_GENERATOR)->SetBeginGroup(TRUE);


		//物资设备
		CXTPRibbonTab* pTabMatermgr = m_pRibbonBar->AddTab(ID_TAB_MATERMGR);
		vecTabs.push_back(pTabMatermgr);

		pGroupReport = pTabMatermgr->AddGroup(ID_GROUP_REPORT_SET);
		pGroupReport->ShowOptionButton();
		pGroupReport->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupReport->GetControlGroupOption()->SetID(ID_GROUP_REPORT_SET_OPTION);
		pGroupReport->SetControlsCentering();

		pGroupReport->Add(xtpControlButton, ID_REPORT_SET_EDITABLE);
		pGroupReport->Add(xtpControlButton, ID_REPORT_SET_ROWHEAD);	
		pGroupReport->Add(xtpControlButton, ID_REPORT_SET_ROWFOOT);

		pGroupModift = pTabMatermgr->AddGroup(ID_GROUP_REPORT_MODIFY);
		pGroupModift->ShowOptionButton();
		pGroupModift->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupModift->GetControlGroupOption()->SetID(ID_GROUP_REPORT_MODIFY_OPTION);
		pGroupModift->SetControlsCentering();

		pGroupModift->Add(xtpControlButton, ID_REPORT_MODIFY_ADD);
		pGroupModift->Add(xtpControlButton, ID_REPORT_MODIFY_SET);
		pGroupModift->Add(xtpControlButton, ID_REPORT_MODIFY_DEL);

		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_BACKUP)->SetBeginGroup(TRUE);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_RESTORE);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_DROP);

		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_SUBLIST)->SetBeginGroup(TRUE);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_FILTEREDIT);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_FILTERCLEAR);

		CXTPRibbonGroup* pGroupMaterdata = pTabMatermgr->AddGroup(ID_GROUP_MATERMGR);
		pGroupMaterdata->ShowOptionButton();
		pGroupMaterdata->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupMaterdata->GetControlGroupOption()->SetID(ID_GROUP_MATERMGR_OPTION);
		pGroupMaterdata->SetControlsCentering();

		m_pPopupMater = (CXTPControlPopup*)pGroupMaterdata->Add(xtpControlSplitButtonPopup, ID_MATERMGR_TABLE);
		m_pPopupMater->SetFlags(xtpFlagManualUpdate);

		pGroupMaterdata->Add(xtpControlButton, ID_MATERMGR_STOCK)->SetBeginGroup(TRUE);
		pGroupMaterdata->Add(xtpControlButton, ID_MATERMGR_CHECK);


		//财务管理
		CXTPRibbonTab* pTabFinancemgr = m_pRibbonBar->AddTab(ID_TAB_FINANCEMGR);
		vecTabs.push_back(pTabFinancemgr);

		pGroupReport = pTabFinancemgr->AddGroup(ID_GROUP_REPORT_SET);
		pGroupReport->ShowOptionButton();
		pGroupReport->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupReport->GetControlGroupOption()->SetID(ID_GROUP_REPORT_SET_OPTION);
		pGroupReport->SetControlsCentering();

		pGroupReport->Add(xtpControlButton, ID_REPORT_SET_EDITABLE);
		pGroupReport->Add(xtpControlButton, ID_REPORT_SET_ROWHEAD);	
		pGroupReport->Add(xtpControlButton, ID_REPORT_SET_ROWFOOT);

		pGroupModift = pTabFinancemgr->AddGroup(ID_GROUP_REPORT_MODIFY);
		pGroupModift->ShowOptionButton();
		pGroupModift->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupModift->GetControlGroupOption()->SetID(ID_GROUP_REPORT_MODIFY_OPTION);
		pGroupModift->SetControlsCentering();

		pGroupModift->Add(xtpControlButton, ID_REPORT_MODIFY_ADD);
		pGroupModift->Add(xtpControlButton, ID_REPORT_MODIFY_SET);
		pGroupModift->Add(xtpControlButton, ID_REPORT_MODIFY_DEL);

		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_BACKUP)->SetBeginGroup(TRUE);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_RESTORE);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_DROP);

		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_SUBLIST)->SetBeginGroup(TRUE);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_FILTEREDIT);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_FILTERCLEAR);

		CXTPRibbonGroup* pGroupFinancedata = pTabFinancemgr->AddGroup(ID_GROUP_FINANCEMGR);
		pGroupFinancedata->ShowOptionButton();
		pGroupFinancedata->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupFinancedata->GetControlGroupOption()->SetID(ID_GROUP_FINANCEMGR_OPTION);
		pGroupFinancedata->SetControlsCentering();

		m_pPopupFinance = (CXTPControlPopup*)pGroupFinancedata->Add(xtpControlSplitButtonPopup, ID_FINANCEMGR_TABLE);
		m_pPopupFinance->SetFlags(xtpFlagManualUpdate);

		pGroupFinancedata->Add(xtpControlButton, ID_FINANCEMGR_MGE)->SetBeginGroup(TRUE);
		pGroupFinancedata->Add(xtpControlButton, ID_FINANCEMGR_SUM);
		pGroupFinancedata->Add(xtpControlButton, ID_FINANCEMGR_RATE)->SetBeginGroup(TRUE);
		pGroupFinancedata->Add(xtpControlButton, ID_FINANCEMGR_LOAN)->SetBeginGroup(TRUE);


		//无砟轨道
		CXTPRibbonTab* pTabCrtsmgr = m_pRibbonBar->AddTab(ID_TAB_CRTSMGR);
		vecTabs.push_back(pTabCrtsmgr);

		pGroupReport = pTabCrtsmgr->AddGroup(ID_GROUP_REPORT_SET);
		pGroupReport->ShowOptionButton();
		pGroupReport->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupReport->GetControlGroupOption()->SetID(ID_GROUP_REPORT_SET_OPTION);
		pGroupReport->SetControlsCentering();

		pGroupReport->Add(xtpControlButton, ID_REPORT_SET_EDITABLE);
		pGroupReport->Add(xtpControlButton, ID_REPORT_SET_ROWHEAD);
		pGroupReport->Add(xtpControlButton, ID_REPORT_SET_ROWFOOT);

		pGroupModift = pTabCrtsmgr->AddGroup(ID_GROUP_REPORT_MODIFY);
		pGroupModift->ShowOptionButton();
		pGroupModift->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupModift->GetControlGroupOption()->SetID(ID_GROUP_REPORT_MODIFY_OPTION);
		pGroupModift->SetControlsCentering();

		pGroupModift->Add(xtpControlButton, ID_REPORT_MODIFY_ADD);
		pGroupModift->Add(xtpControlButton, ID_REPORT_MODIFY_SET);
		pGroupModift->Add(xtpControlButton, ID_REPORT_MODIFY_DEL);

		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_BACKUP)->SetBeginGroup(TRUE);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_RESTORE);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_DROP);

		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_SUBLIST)->SetBeginGroup(TRUE);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_FILTEREDIT);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_FILTERCLEAR);

		CXTPRibbonGroup* pGroupSlabdata = pTabCrtsmgr->AddGroup(ID_GROUP_CRTSMGR_SLAB);
		pGroupSlabdata->ShowOptionButton();
		pGroupSlabdata->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupSlabdata->GetControlGroupOption()->SetID(ID_GROUP_CRTSMGR_SLAB_OPTION);
		pGroupSlabdata->SetControlsCentering();

		pGroupSlabdata->Add(xtpControlButton, ID_CRTSMGR_SLAB_DAT);
		pGroupSlabdata->Add(xtpControlButton, ID_CRTSMGR_SLAB_SUM)->SetBeginGroup(TRUE);
		pGroupSlabdata->Add(xtpControlButton, ID_CRTSMGR_SLAB_ALL);
		pGroupSlabdata->Add(xtpControlButton, ID_CRTSMGR_SLAB_CNT);
		pGroupSlabdata->Add(xtpControlButton, ID_CRTSMGR_SLAB_HORIZON)->SetBeginGroup(TRUE);
		pGroupSlabdata->Add(xtpControlButton, ID_CRTSMGR_SLAB_VERTICAL);
		pGroupSlabdata->Add(xtpControlButton, ID_CRTSMGR_SLAB_MILEAGE);
		pGroupSlabdata->Add(xtpControlButton, ID_CRTSMGR_SLAB_RIDE_IN)->SetBeginGroup(TRUE);
		pGroupSlabdata->Add(xtpControlButton, ID_CRTSMGR_SLAB_RIDE_OUT);
		pGroupSlabdata->Add(xtpControlButton, ID_CRTSMGR_SLAB_RATE);

		CXTPRibbonGroup* pGroupRaildata = pTabCrtsmgr->AddGroup(ID_GROUP_CRTSMGR_RAIL);
		pGroupRaildata->ShowOptionButton();
		pGroupRaildata->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupRaildata->GetControlGroupOption()->SetID(ID_GROUP_CRTSMGR_RAIL_OPTION);
		pGroupRaildata->SetControlsCentering();

		pControl = pGroupRaildata->Add(xtpControlButton, ID_CRTSMGR_RAIL_MGE);
		pControl->SetBeginGroup(TRUE);
		pGroupRaildata->Add(xtpControlButton, ID_CRTSMGR_RAIL_CAL);
		pGroupRaildata->Add(xtpControlButton, ID_CRTSMGR_RAIL_PLATE)->SetBeginGroup(TRUE);
		pGroupRaildata->Add(xtpControlButton, ID_CRTSMGR_RAIL_ADJUST);
		pGroupRaildata->Add(xtpControlButton, ID_CRTSMGR_RAIL_ALL)->SetBeginGroup(TRUE);
		pGroupRaildata->Add(xtpControlButton, ID_CRTSMGR_RAIL_WFP);
		pGroupRaildata->Add(xtpControlButton, ID_CRTSMGR_RAIL_ZW6);
		pGroupRaildata->Add(xtpControlButton, ID_CRTSMGR_RAIL_RIDE)->SetBeginGroup(TRUE);
		pGroupRaildata->Add(xtpControlButton, ID_CRTSMGR_RAIL_RATE);


		//克缺整治
		CXTPRibbonTab* pTabProblemmgr = m_pRibbonBar->AddTab(ID_TAB_PROBLEMMGR);
		vecTabs.push_back(pTabProblemmgr);

		pGroupReport = pTabProblemmgr->AddGroup(ID_GROUP_REPORT_SET);
		pGroupReport->ShowOptionButton();
		pGroupReport->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupReport->GetControlGroupOption()->SetID(ID_GROUP_REPORT_SET_OPTION);
		pGroupReport->SetControlsCentering();

		pGroupReport->Add(xtpControlButton, ID_REPORT_SET_EDITABLE);
		pGroupReport->Add(xtpControlButton, ID_REPORT_SET_ROWHEAD);
		pGroupReport->Add(xtpControlButton, ID_REPORT_SET_ROWFOOT);

		pGroupModift = pTabProblemmgr->AddGroup(ID_GROUP_REPORT_MODIFY);
		pGroupModift->ShowOptionButton();
		pGroupModift->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupModift->GetControlGroupOption()->SetID(ID_GROUP_REPORT_MODIFY_OPTION);
		pGroupModift->SetControlsCentering();

		pGroupModift->Add(xtpControlButton, ID_REPORT_MODIFY_ADD);
		pGroupModift->Add(xtpControlButton, ID_REPORT_MODIFY_SET);
		pGroupModift->Add(xtpControlButton, ID_REPORT_MODIFY_DEL);

		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_BACKUP)->SetBeginGroup(TRUE);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_RESTORE);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_DROP);

		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_SUBLIST)->SetBeginGroup(TRUE);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_FILTEREDIT);
		pGroupModift->Add(xtpControlButton, ID_REPORT_SET_FILTERCLEAR);

		CXTPRibbonGroup* pGroupProblemdata = pTabProblemmgr->AddGroup(ID_GROUP_PROBLEMMGR);
		pGroupProblemdata->ShowOptionButton();
		pGroupProblemdata->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupProblemdata->GetControlGroupOption()->SetID(ID_GROUP_PROBLEMMGR_OPTION);
		pGroupProblemdata->SetControlsCentering();

		pGroupProblemdata->Add(xtpControlButton, ID_PROBLEMMGR_DAT);
		pGroupProblemdata->Add(xtpControlButton, ID_PROBLEMMGR_SUM);
		pGroupProblemdata->Add(xtpControlButton, ID_PROBLEMMGR_ALL);
		pGroupProblemdata->Add(xtpControlButton, ID_PROBLEMMGR_ROAD)->SetBeginGroup(TRUE);
		pGroupProblemdata->Add(xtpControlButton, ID_PROBLEMMGR_BRIDGE);
		pGroupProblemdata->Add(xtpControlButton, ID_PROBLEMMGR_TUNNEL);
		pGroupProblemdata->Add(xtpControlButton, ID_PROBLEMMGR_ORBITAL)->SetBeginGroup(TRUE);
		pGroupProblemdata->Add(xtpControlButton, ID_PROBLEMMGR_BARRIER);
		pGroupProblemdata->Add(xtpControlButton, ID_PROBLEMMGR_GROUD)->SetBeginGroup(TRUE);
		pGroupProblemdata->Add(xtpControlButton, ID_PROBLEMMGR_MEASURE);
		pGroupProblemdata->Add(xtpControlButton, ID_PROBLEMMGR_SUM_)->SetBeginGroup(TRUE);
		pGroupProblemdata->Add(xtpControlButton, ID_PROBLEMMGR_ALL_);
		pGroupProblemdata->Add(xtpControlButton, ID_PROBLEMMGR_CRT);

		//资源管理器
		CXTPRibbonTab* pTabExplor = m_pRibbonBar->AddTab(ID_TAB_EXPLOR);

		CXTPRibbonGroup* pGroupSearch = pTabExplor->AddGroup(ID_GROUP_EXPLOR);
		pGroupSearch->ShowOptionButton();
		pGroupSearch->SetIconId(ID_GROUP_BUTTONPOPUP);
		pGroupSearch->GetControlGroupOption()->SetID(ID_GROUP_EXPLOR_OPTION);
		pGroupSearch->SetControlsCentering();

		pGroupSearch->Add(xtpControlButton, ID_EXPLOR_SEARCH);
		pGroupSearch->Add(xtpControlButton, ID_EXPLOR_DIRROOT)->SetBeginGroup(TRUE);
		pGroupSearch->Add(xtpControlButton, ID_EXPLOR_DIRBACK);
		pGroupSearch->Add(xtpControlButton, ID_EXPLOR_UPLOAD)->SetBeginGroup(TRUE);
		pGroupSearch->Add(xtpControlButton, ID_EXPLOR_DOWNLOAD);
		pGroupSearch->Add(xtpControlButton, ID_EXPLOR_DELETE);
		pGroupSearch->Add(xtpControlButton, ID_EXPLOR_FLUSH)->SetBeginGroup(TRUE);


		//快捷按钮
		CXTPControl* pControlExpand = m_pRibbonBar->GetControls()->Add(xtpControlButton, ID_RIBBON_EXPAND);
		pControlExpand->SetFlags(xtpFlagRightAlign);

		CXTPControl* pControlMinimize = m_pRibbonBar->GetControls()->Add(xtpControlButton, ID_RIBBON_MINIMIZE);
		pControlMinimize->SetFlags(xtpFlagRightAlign);

		CXTPControl* pControlSitchreportview = m_pRibbonBar->GetControls()->Add(xtpControlButton, ID_RIBBON_SWITCH_REPORTVIEW);
		pControlSitchreportview->SetFlags(xtpFlagRightAlign);

		CXTPControl* pControlSitchchartview = m_pRibbonBar->GetControls()->Add(xtpControlButton, ID_RIBBON_SWITCH_CHARTVIEW);
		pControlSitchchartview->SetFlags(xtpFlagRightAlign);

		CXTPControlPopup* pControlOptions = (CXTPControlPopup*)m_pRibbonBar->GetControls()->Add(xtpControlSplitButtonPopup, ID_APP_OPTIONS);
		CMenu mnuOptions;
		mnuOptions.LoadMenu(ID_APP_OPTIONS);
		pControlOptions->SetCommandBar(mnuOptions.GetSubMenu(0));
		pControlOptions->SetFlags(xtpFlagRightAlign);

		CXTPControlPopup* pControlSkin = (CXTPControlPopup*)m_pRibbonBar->GetControls()->Add(xtpControlSplitButtonPopup, ID_THEME_SKIN);
		CMenu mnuSkin;
		mnuSkin.LoadMenu(ID_THEME_SKIN);
		pControlSkin->SetCommandBar(mnuSkin.GetSubMenu(0));
		pControlSkin->SetFlags(xtpFlagRightAlign);

		CXTPControlPopup*pControlHelp = (CXTPControlPopup*)m_pRibbonBar->GetControls()->Add(xtpControlButtonPopup, ID_APP_HELP);
		CMenu mnuHelp;
		mnuHelp.LoadMenu(ID_APP_HELP);
		pControlHelp->SetCommandBar(mnuHelp.GetSubMenu(0));
		pControlHelp->SetFlags(xtpFlagRightAlign| xtpFlagManualUpdate);

		m_pRibbonBar->GetQuickAccessControls()->Add(xtpControlButton, ID_FILE_NEW);
		m_pRibbonBar->GetQuickAccessControls()->Add(xtpControlButton, ID_FILE_OPEN);
		m_pRibbonBar->GetQuickAccessControls()->Add(xtpControlButton, ID_FILE_SAVE);
		m_pRibbonBar->GetQuickAccessControls()->Add(xtpControlButton, ID_FILE_IMPORT);
		m_pRibbonBar->GetQuickAccessControls()->Add(xtpControlButton, ID_FILE_EXPORT);
		CXTPControlPopup* pControlUndo = (CXTPControlPopup*)m_pRibbonBar->GetQuickAccessControls()->Add(xtpControlSplitButtonPopup, ID_EDIT_UNDO);

		pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());
		pPopupBar->SetShowGripper(FALSE);

		pControlUndo->SetCommandBar(pPopupBar);

		pControlGallery = new CControlGalleryUndo();
		pControlGallery->SetItems(m_pItemsUndo);
		pControlGallery->SetWidth(120);
		pControlGallery->ShowScrollBar(FALSE);
		pPopupBar->GetControls()->Add(pControlGallery, ID_EDIT_UNDO);

		CXTPControlStatic* pControlListBoxInfo = (CXTPControlStatic*)pPopupBar->GetControls()->Add(new CXTPControlStatic(), ID_EDIT_UNDO);
		pControlListBoxInfo->SetWidth(120);
		pControlListBoxInfo->SetFlags(xtpFlagSkipFocus);

		pPopupBar->InternalRelease();

		m_pRibbonBar->GetQuickAccessControls()->Add(xtpControlSplitButtonPopup, ID_EDIT_REDO);
		m_pRibbonBar->GetQuickAccessControls()->Add(xtpControlButton, ID_FILE_PRINT);
		m_pRibbonBar->GetQuickAccessControls()->Add(xtpControlButton, ID_FILE_PRINT_PREVIEW);
		m_pRibbonBar->GetQuickAccessControls()->CreateOriginalControls();

		m_pRibbonBar->SetCloseable(FALSE);
		m_pRibbonBar->EnableFrameTheme();
		m_pRibbonBar->EnableDocking(0);


		// Call it only if your Ribbon is support customization using Ribbon Customization page.
		m_pRibbonBar->EnableCustomization(TRUE);

		LoadRegSoft(vecTabs);

		LogInfo(L"XTPRibbonBar Create Successful!");
	}
	catch (CException* e)
	{
		LogErrorExp(L"XTPRibbonBar Create Faild!", e);
	}

	return TRUE;
}

void CMainFrame::LoadRibbonIcons()
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	pCommandBars->GetImageManager()->SetIcons(IDR_MAINFRAME);

	UINT uiSystemMenu[] = {ID_FILE_NEW, ID_FILE_OPEN, ID_FILE_SAVE, ID_FILE_SAVE_AS, ID_DATABASE_TABLE, ID_EDIT_PASTE,ID_FILE_PRINT, ID_FILE_PRINT_SETUP, ID_FILE_PRINT_PREVIEW, 
		ID_FILE_PREPARE, ID_FILE_SEND, ID_APP_CLOSE, ID_FILE_SEND_MAIL, ID_FILE_SEND_INTERNETFAX, ID_FILE_DATA,ID_FILE_IMPORT,ID_FILE_EXPORT}; 
	pCommandBars->GetImageManager()->SetIcons(IDP_SYSTEMMENULARGE, uiSystemMenu, _countof(uiSystemMenu), CSize(32, 32));

	UINT uiLargeIcons[] = {ID_DATASET_DATBACKUP,ID_USERMGR_REGUSER,ID_PROGRAM_TASKTRACK,ID_PROGRAM_METHOD,ID_PROGRAM_CHART,ID_PROGRAM_REPORT,
		ID_REPORT_SET_EDITABLE,ID_REPORT_MODIFY_ADD,ID_REPORT_MODIFY_SET,ID_REPORT_MODIFY_DEL,ID_COSTMGR_DAT,ID_CRTSMGR_SLAB_DAT,ID_PROBLEMMGR_DAT,
		ID_POINTSMGR_POINTS,ID_POINTSMGR_LEDGER,ID_REPORT_GENERATOR,ID_EXPLOR_SEARCH,ID_EXPLOR_FLUSH,ID_CRTSMGR_RAIL_MGE,ID_CRTSMGR_RAIL_CAL,
		ID_POINTSMGR_TABLE,ID_COSTMGR_TABLE,ID_MATERMGR_TABLE,ID_FINANCEMGR_TABLE,ID_FINANCEMGR_RATE,ID_FINANCEMGR_LOAN,ID_GALLERY_SHAPES,ID_ZXING_FORMAT}; 
	pCommandBars->GetImageManager()->SetIcons(IDR_LARGEICONS, uiLargeIcons, _countof(uiLargeIcons), CSize(32, 32));

	UINT uiZxingFormat[] = {ID_ZXING_EAN13, ID_ZXING_PDF417, ID_ZXING_QRCODE, ID_ZXING_QRCODEMID};
	pCommandBars->GetImageManager()->SetIcons(ID_ZXING_FORMAT, uiZxingFormat, _countof(uiZxingFormat), CSize(32, 32));

	UINT uiGroups[] = {ID_GROUP_DATABASE, ID_GROUP_CLIPBOARD, ID_GROUP_FONT, ID_GROUP_ALIGNMENT, ID_GROUP_NUMBER, ID_GROUP_STYLES, ID_GROUP_CELL, ID_GROUP_MODIFY}; 
	pCommandBars->GetImageManager()->SetIcons(IDP_RIBBON_GROUPS, uiGroups, _countof(uiGroups), CSize(16, 16));

	UINT uiGroupDatabase[] = {ID_DATABASE_MSSQL, ID_DATABASE_EXCEL, ID_DATABASE_ACCESS};
	pCommandBars->GetImageManager()->SetIcons(ID_GROUP_DATABASE, uiGroupDatabase, _countof(uiGroupDatabase), CSize(16, 16));

	UINT uiGroupClipboard[] = {ID_EDIT_CUT, ID_EDIT_COPY, ID_EDIT_FORMAT_PAINTER};
	pCommandBars->GetImageManager()->SetIcons(ID_GROUP_CLIPBOARD, uiGroupClipboard, _countof(uiGroupClipboard), CSize(16, 16));

	UINT uiGroupFont[] = {ID_FONT_GROW, ID_FONT_SHRINK, ID_FONT_CLEAR, ID_FONT_BOLD, ID_FONT_ITALIC, ID_FONT_UNDERLINE,
		ID_FONT_BORDERS, ID_FONT_SUBSCRIPT,ID_FONT_SUPERSCRIPT, ID_FONT_CHARFIELD, ID_FONT_BACKCOLOR,ID_FONT_FACECOLOR,
	    ID_UNDERLINE_SINGLE,ID_UNDERLINE_DOUBLE,ID_CHARFIELD_SHOW,ID_CHARFIELD_EDIT,ID_CHARFIELD_MODIFY};
	pCommandBars->GetImageManager()->SetIcons(ID_GROUP_FONT, uiGroupFont, _countof(uiGroupFont), CSize(16, 16));

	UINT uiBorders[] = {ID_BORDERS_DOWN,ID_BORDERS_UP,ID_BORDERS_LEFT,ID_BORDERS_RIGHT,
		ID_BORDERS_NO,ID_BORDERS_ALL,ID_BORDERS_OUT,ID_BORDERS_OUTB,ID_BORDERS_DOWND,
		ID_BORDERS_DOWNB,ID_BORDERS_UPDOWN,ID_BORDERS_UPDOWND,ID_BORDERS_UPDOWNB,
		ID_BORDERS_MIDH,ID_BORDERS_MIDV,ID_BORDERS_LEFTS,ID_BORDERS_RIGHTS,ID_BORDERS_MIDHV};
	pCommandBars->GetImageManager()->SetIcons(ID_FONT_BORDERS, uiBorders, _countof(uiBorders), CSize(16, 16));

	UINT uiGroupAlignment[] = {ID_ALIGNMENT_UP, ID_ALIGNMENT_MIDDLE, ID_ALIGNMENT_DOWN, ID_ALIGNMENT_DIRECT, ID_ALIGNMENT_SORT, 
		ID_ALIGNMENT_LEFT, ID_ALIGNMENT_CENTER, ID_ALIGNMENT_RIGHT, ID_ALIGNMENT_DECREASEINDENT, ID_ALIGNMENT_INCREASEINDENT,
		ID_ALIGNMENT_WRAP, ID_ALIGNMENT_MERGE,ID_DIRECT_DECLOCKWISE,ID_DIRECT_INCLOCKWISE,ID_DIRECT_VERTICALTEXT,ID_DIRECT_UPROTATEDTEXT,
		ID_DIRECT_DOWNROTATEDTEXT,ID_DIRECT_CELLALIGNMENT,ID_MERGE_CENTER,ID_MERGE_SPAN,ID_MERGE_CELL,ID_MERGE_UNDO};
	pCommandBars->GetImageManager()->SetIcons(ID_GROUP_ALIGNMENT, uiGroupAlignment, _countof(uiGroupAlignment), CSize(16, 16));

	UINT uiGroupNumber[] = {ID_NUMBER_ACCOUNTANT, ID_NUMBER_PERCENT, ID_NUMBER_KILOBITGAP,
		ID_NUMBER_INPRECISION,ID_NUMBER_ENPRECISION};
	pCommandBars->GetImageManager()->SetIcons(ID_GROUP_NUMBER, uiGroupNumber, _countof(uiGroupNumber), CSize(16, 16));

	UINT uiGroupStyles[] = {ID_STYLES_CONDITION, ID_STYLES_FORMAT, ID_STYLES_CELL,
		ID_CELL_INSERT,ID_CELL_DELETE,ID_CELL_FORMAT,ID_MODIFY_SORTFILTER,ID_MODIFY_FINDSELECT,
		ID_CONDITION_HIGHLIGHT,ID_CONDITION_ITEMSELECT,ID_CONDITION_DATABARS,
		ID_CONDITION_CLRVALEUR,ID_CONDITION_ICONLIST};
	pCommandBars->GetImageManager()->SetIcons(ID_GROUP_STYLES, uiGroupStyles, _countof(uiGroupStyles), CSize(32, 32));

	UINT uiGroupCell[] = {ID_CONDITION_NEW,ID_CONDITION_CLEAR, ID_CONDITION_MANAGE,ID_STYLEFMT_SAVESTYLE,
		ID_STYLEFMT_APPLYSTYLE,ID_STYLECELL_NEW,ID_STYLECELL_MERGE,
		ID_INSERT_CELL, ID_INSERT_ROW, ID_INSERT_COL,ID_INSERT_TABLE,
		ID_DELETE_CELL,ID_DELETE_ROW,ID_DELETE_COL,ID_DELETE_TABLE,ID_FORMAT_ROWHIGHT,ID_FORMAT_COLWIDTH,
		ID_FORMAT_PROTECT,ID_FORMAT_LOCK,ID_FORMAT_MODIFY};
	pCommandBars->GetImageManager()->SetIcons(ID_GROUP_CELL, uiGroupCell, _countof(uiGroupCell), CSize(16, 16));

	UINT uiGroupModify[] = {ID_MODIFY_SUM,ID_MODIFY_FILL,ID_MODIFY_CLEAR,ID_SUM_SUM,ID_FILL_DOWN,ID_FILL_RIGHT,ID_FILL_UP,ID_FILL_LEFT,ID_CLEAR_ALL,ID_CLEAR_FORMAT,
		ID_SORTFILTER_ASCE,ID_SORTFILTER_DESC,ID_SORTFILTER_SORT,ID_SORTFILTER_FILTER,ID_SORTFILTER_CLEAR,ID_SORTFILTER_REAPPLY,
		ID_FINDSELECT_FIND, ID_FINDSELECT_REPLACE, ID_FINDSELECT_GOTO, ID_FINDSELECT_SELOBJECTS,ID_FINDSELECT_SELWINDOW};
	pCommandBars->GetImageManager()->SetIcons(ID_GROUP_MODIFY, uiGroupModify, _countof(uiGroupModify), CSize(16, 16));

	pCommandBars->GetImageManager()->SetIcon(ID_GROUP_BUTTONPOPUP, ID_GROUP_BUTTONPOPUP);

	UINT uiGroupsTip[] = {ID_GROUP_DATABASE_OPTION, ID_GROUP_CLIPBOARD_OPTION, ID_GROUP_FONT_OPTION, ID_GROUP_ALIGNMENT_OPTION, ID_GROUP_NUMBER_OPTION, ID_GROUP_STYLES_OPTION, ID_GROUP_CELL_OPTION, IDP_RIBBON_GEAR};
	pCommandBars->GetImageManager()->SetIcons(IDP_TOOLTIP_GROUPOPTION, uiGroupsTip, _countof(uiGroupsTip), CSize(100, 130));

	UINT uiRibbonMinimize[] = {ID_RIBBON_MINIMIZE, ID_RIBBON_EXPAND, ID_RIBBON_SWITCH_REPORTVIEW, ID_RIBBON_SWITCH_CHARTVIEW};
	pCommandBars->GetImageManager()->SetIcons(IDP_RIBBON_MINIMIZE, uiRibbonMinimize, _countof(uiRibbonMinimize), CSize(16, 16));

	UINT uiGroupsReportTip[] = {ID_GROUP_DATASET_OPTION,ID_GROUP_USERMGR_OPTION,ID_GROUP_PROGRAM_OPTION,ID_GROUP_REPORT_SET_OPTION,
		ID_GROUP_REPORT_MODIFY_OPTION,ID_GROUP_COSTMGR_OPTION,ID_GROUP_MATERMGR_OPTION,
		ID_GROUP_CRTSMGR_SLAB_OPTION,ID_GROUP_PROBLEMMGR_OPTION,ID_GROUP_EXPLOR_OPTION};
	pCommandBars->GetImageManager()->SetIcons(IDP_TOOLTIP_REPORT, uiGroupsReportTip, _countof(uiGroupsReportTip), CSize(100, 130));

	UINT uiGroupsDataset[] = {ID_DATASET_DATBACKUP,ID_DATASET_DATRESTORE,ID_DATASET_DATCLEAR};
	pCommandBars->GetImageManager()->SetIcons(IDP_DATASET, uiGroupsDataset, _countof(uiGroupsDataset), CSize(16, 16));

	UINT uiGroupsUsermgr[] = {ID_USERMGR_SETUSER,ID_USERMGR_DELUSER, ID_USERMGR_USERINFO, ID_USERMGR_AUTOLOGIN};
	pCommandBars->GetImageManager()->SetIcons(IDP_USERMGR, uiGroupsUsermgr, _countof(uiGroupsUsermgr), CSize(16, 16));

	UINT uiGroupsProgram[] = {ID_PROGRAM_TASKTRACK,ID_PROGRAM_METHOD,ID_PROGRAM_CHART};
	pCommandBars->GetImageManager()->SetIcons(IDP_PROGRAM, uiGroupsProgram, _countof(uiGroupsProgram), CSize(16, 16));

	UINT uiGroupsReportmgr[] = {ID_REPORT_SET_EDITABLE,ID_REPORT_SET_ROWHEAD,ID_REPORT_SET_ROWFOOT,
		ID_REPORT_MODIFY_ADD,ID_REPORT_MODIFY_SET,ID_REPORT_MODIFY_DEL,
		ID_REPORT_SET_BACKUP, ID_REPORT_SET_RESTORE, ID_REPORT_SET_DROP,
		ID_REPORT_SET_SUBLIST,ID_REPORT_SET_FILTEREDIT,ID_REPORT_SET_FILTERCLEAR};
	pCommandBars->GetImageManager()->SetIcons(IDP_REPORTMGR, uiGroupsReportmgr, _countof(uiGroupsReportmgr), CSize(16, 16));

	UINT uiGroupsPointsmgr[] = {ID_POINTSMGR_DATAPART,ID_POINTSMGR_DATEEDIT,ID_POINTSMGR_DATEPOPUP,IDR_PANE_PROPERTIES,IDR_PANE_RIBBON,IDR_PANE_POINTS};
	pCommandBars->GetImageManager()->SetIcons(IDP_POINTSMGR, uiGroupsPointsmgr, _countof(uiGroupsPointsmgr), CSize(16, 16));

	UINT uiPointsmgrtable[] = {ID_TABLE_ROAD,ID_TABLE_BRIDGE,ID_TABLE_TUNNEL,ID_TABLE_ORBITAL, ID_TABLE_YARD,ID_TABLE_TEMP};
	pCommandBars->GetImageManager()->SetIcons(ID_POINTSMGR_TABLE, uiPointsmgrtable, _countof(uiPointsmgrtable), CSize(32, 32));

	UINT uiCostmgrtable[] = {ID_TABLE_QTY,ID_TABLE_UP,ID_TABLE_DOWN,ID_TABLE_LIST};
	pCommandBars->GetImageManager()->SetIcons(ID_COSTMGR_TABLE, uiCostmgrtable, _countof(uiCostmgrtable), CSize(32, 32));

	UINT uiMatermgrtable[] = {ID_TABLE_EMPLOY,ID_TABLE_UNITS,ID_TABLE_GOODS, ID_TABLE_PACT,ID_TABLE_BARN,ID_TABLE_CHECK,ID_TABLE_INSTOCK,ID_TABLE_OUTSTOCK,ID_TABLE_MIXSTOCK,ID_TABLE_RETSTOCK};
	pCommandBars->GetImageManager()->SetIcons(ID_MATERMGR_TABLE, uiMatermgrtable, _countof(uiMatermgrtable), CSize(32, 32));

	UINT uiFinancetable[] = {ID_TABLE_MEMBER,ID_TABLE_FUNDS,ID_TABLE_INCOME,ID_TABLE_OUTLAY};
	pCommandBars->GetImageManager()->SetIcons(ID_FINANCEMGR_TABLE, uiFinancetable, _countof(uiFinancetable), CSize(32, 32));

	UINT uiGroupsCostmgr[] = {ID_COSTMGR_SUM,ID_COSTMGR_ALL};
	pCommandBars->GetImageManager()->SetIcons(IDP_COSTMGR, uiGroupsCostmgr, _countof(uiGroupsCostmgr), CSize(16, 16));

	UINT uiGroupsMatermgr[] = {ID_MATERMGR_STOCK,ID_MATERMGR_CHECK};
	pCommandBars->GetImageManager()->SetIcons(IDP_MATERMGR, uiGroupsMatermgr, _countof(uiGroupsMatermgr), CSize(16, 16));

	UINT uiGroupsFinancemgr[] = {ID_FINANCEMGR_MGE,ID_FINANCEMGR_SUM};
	pCommandBars->GetImageManager()->SetIcons(IDP_FINANCEMGR, uiGroupsFinancemgr, _countof(uiGroupsFinancemgr), CSize(16, 16));

	UINT uiGroupsOrbitalmgr[] = {ID_CRTSMGR_SLAB_SUM,ID_CRTSMGR_SLAB_ALL,ID_CRTSMGR_SLAB_CNT,
		ID_CRTSMGR_SLAB_HORIZON,ID_CRTSMGR_SLAB_VERTICAL,ID_CRTSMGR_SLAB_MILEAGE,
		ID_CRTSMGR_SLAB_RIDE_IN,ID_CRTSMGR_SLAB_RIDE_OUT,ID_CRTSMGR_SLAB_RATE,
		ID_CRTSMGR_RAIL_PLATE,ID_CRTSMGR_RAIL_ADJUST,ID_CRTSMGR_RAIL_ALL,
		ID_CRTSMGR_RAIL_WFP,ID_CRTSMGR_RAIL_ZW6,ID_CRTSMGR_RAIL_RIDE,ID_CRTSMGR_RAIL_RATE};
	pCommandBars->GetImageManager()->SetIcons(IDP_CRTSMGR_SLAB, uiGroupsOrbitalmgr, _countof(uiGroupsOrbitalmgr), CSize(16, 16));

	UINT uiGroupsProblemmgr[] = {ID_PROBLEMMGR_SUM,ID_PROBLEMMGR_ALL,ID_PROBLEMMGR_ROAD,ID_PROBLEMMGR_BRIDGE,
		ID_PROBLEMMGR_TUNNEL,ID_PROBLEMMGR_ORBITAL,ID_PROBLEMMGR_BARRIER,ID_PROBLEMMGR_GROUD,
		ID_PROBLEMMGR_MEASURE,ID_PROBLEMMGR_SUM_,ID_PROBLEMMGR_ALL_,ID_PROBLEMMGR_CRT};
	pCommandBars->GetImageManager()->SetIcons(IDP_PROBLEMMGR, uiGroupsProblemmgr, _countof(uiGroupsProblemmgr), CSize(16, 16));

	UINT uiGroupsExplor[] = {ID_EXPLOR_SEARCH,ID_EXPLOR_DIRROOT,ID_EXPLOR_DIRBACK,ID_EXPLOR_UPLOAD,ID_EXPLOR_DOWNLOAD,ID_EXPLOR_DELETE,ID_EXPLOR_FLUSH};
	pCommandBars->GetImageManager()->SetIcons(IDP_EXPLOR, uiGroupsExplor, _countof(uiGroupsExplor), CSize(16, 16));
}

void CMainFrame::CreateSystemMenuPopup()
{
	CXTPRibbonControlSystemButton* pControlFile = m_pRibbonBar->GetSystemButton();

	CMenu menu;
	menu.LoadMenu(IDR_MAINFRAME);

	CXTPPopupBar* pCommandBar = new CXTPRibbonSystemPopupBar();
	pCommandBar->SetCommandBars(GetCommandBars());

	pControlFile->SetCommandBar(pCommandBar);
	pCommandBar->InternalRelease();

	pCommandBar->LoadMenu(menu.GetSubMenu(0));

	pCommandBar->SetIconSize(CSize(32, 32));
}

void CMainFrame::CreateBackstage()
{
	CXTPRibbonControlSystemButton* pButton = m_pRibbonBar->GetSystemButton();

	CXTPRibbonBackstageView* pView = CXTPRibbonBackstageView::CreateBackstageView(GetCommandBars());

	pView->AddCommand(ID_FILE_SAVE);
	pView->AddCommand(ID_FILE_SAVE_AS);
	pView->AddCommand(ID_FILE_OPEN);


	if (!m_pageInfo.GetSafeHwnd())
	{
		m_pageInfo.Create(CBackstagePageInfo::IDD, this);
	}

	if (!m_pageHelp.GetSafeHwnd())
	{
		m_pageHelp.Create(CBackstagePageHelp::IDD, this);
	}

	if (!m_pageSave.GetSafeHwnd())
	{
		m_pageSave.Create(CBackstagePageSave::IDD, this);
	}

	if (!m_pageRecent.GetSafeHwnd())
	{
		m_pageRecent.Create(CBackstagePageRecent::IDD, this);
	}

	if (!m_pagePrint.GetSafeHwnd())
	{
		m_pagePrint.Create(CBackstagePagePrint::IDD, this);
	}

	CXTPRibbonBackstageTab* pTabInfo = pView->AddTab(&m_pageInfo, ID_BACKSTAGE_INFO);
	pTabInfo->SetFlags(xtpFlagManualUpdate); // No Need update with Update handler

	pTabInfo->SetControlPaneMinSize(CSize(650, 400));

	CXTPRibbonBackstageTab* pTabRecent = pView->AddTab(&m_pageRecent, ID_BACKSTAGE_RECENT);
	pTabRecent->SetFlags(xtpFlagManualUpdate); // No Need update with Update handler

	CXTPRibbonBackstageTab* pTabPrint = pView->AddTab(&m_pagePrint, ID_BACKSTAGE_PRINT);
	pTabPrint->SetFlags(xtpFlagManualUpdate); // No Need update with Update handler

	CXTPRibbonBackstageTab* pTabSaveSend = pView->AddTab(&m_pageSave, ID_BACKSTAGE_SAVESEND);	
	pTabSaveSend->SetFlags(xtpFlagManualUpdate); // No Need update with Update handler

	CXTPRibbonBackstageTab* pTabHelp = pView->AddTab(&m_pageHelp, ID_BACKSTAGE_HELP);
	pTabHelp->SetFlags(xtpFlagManualUpdate); // No Need update with Update handler

	pView->AddCommand(ID_APP_OPTIONS);
	pView->AddCommand(ID_APP_EXIT);

	pTabInfo->SetItemDefault(TRUE);


	UINT nIDIcons[] = {ID_FILE_SAVE, ID_FILE_SAVE_AS, ID_FILE_OPEN, ID_FILE_CLOSE, ID_APP_OPTIONS, ID_APP_EXIT};
	pView->GetImageManager()->SetIcons(IDP_BACKSTAGE, nIDIcons, 6, CSize(0, 0));


	pButton->SetCommandBar(pView);

	pView->InternalRelease();
}

void CMainFrame::InitBarToopTip()
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	pCommandBars->GetToolTipContext()->SetStyle(xtpToolTipOffice);

	CXTPToolTipContext* pToolTipContext = pCommandBars->GetToolTipContext();
	//pToolTipContext->SetStyle(xtpToolTipResource);
	pToolTipContext->ShowTitleAndDescription();
	pToolTipContext->ShowImage(TRUE, 0);
	pToolTipContext->SetMargin(CRect(2, 2, 2, 2));
	pToolTipContext->SetMaxTipWidth(180);
	pToolTipContext->SetFont(pCommandBars->GetPaintManager()->GetIconFont());
	pToolTipContext->SetDelayTime(TTDT_INITIAL, 900);

	pCommandBars->GetPaintManager()->m_bAutoResizeIcons = TRUE;
	pCommandBars->GetCommandBarsOptions()->bToolBarAccelTips = TRUE;
	pCommandBars->GetCommandBarsOptions()->bShowKeyboardTips = TRUE;
	pCommandBars->GetShortcutManager()->SetAccelerators(IDR_MAINFRAME);
}



/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CXTPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CXTPFrameWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if (lpCreateControl->nID == ID_GALLERY_COLORS)
	{
		CXTPControlGallery* pControlGallery = new CXTPControlGallery();
		pControlGallery->SetControlSize(CSize(30*10, 30 * 9 + 2));
		pControlGallery->ShowLabels(TRUE);
		pControlGallery->SetItems(m_pItemsColors);
		pControlGallery->ShowScrollBar(FALSE);
		pControlGallery->SetResizable(FALSE,FALSE);

		lpCreateControl->pControl = pControlGallery;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_GALLERY_FONT_BACKCOLOR)
	{
		CXTPControlGallery* pControlGallery = new CXTPControlGallery();
		pControlGallery->SetControlSize(CSize(30 * 5, 30 * 3 + 2));
		pControlGallery->ShowScrollBar(FALSE);
		pControlGallery->SetItems(m_pItemsFontBackColor);
		pControlGallery->ShowScrollBar(FALSE);
		pControlGallery->SetResizable(FALSE,FALSE);

		lpCreateControl->pControl = pControlGallery;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_GALLERY_FONTS)
	{
		CXTPControlGallery* pControlGallery = new CXTPControlGallery();
		pControlGallery->SetControlSize(CSize(250, 60 * 5 + 2 + 20));
		pControlGallery->ShowLabels(TRUE);
		pControlGallery->SetItems(m_pItemsFonts);
		pControlGallery->SetResizable(FALSE,FALSE);

		lpCreateControl->pCommandBar->EnableAnimation();

		lpCreateControl->pControl = pControlGallery;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_CONDITION_HIGHLIGHT)
	{
		CXTPControlPopup* pControlPopup = CXTPControlPopup::CreateControlPopup(xtpControlButtonPopup);
		CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());

		CXTPControlGallery* pControlGallery = new CXTPControlGallery();
		pControlGallery->SetControlSize(CSize(150, 40*7 + 2));
		pControlGallery->SetResizable();
		pControlGallery->SetItems(m_pItemsConditionHighlight);
		pControlGallery->ShowScrollBar(FALSE);
		pControlGallery->SetResizable(FALSE,FALSE);

		pPopupBar->GetControls()->Add(pControlGallery, ID_GALLERY_CONDITION_HIGHLIGHT);
		pPopupBar->GetControls()->Add(xtpControlButton, ID_CONDITION_OTHERSHOW);
		pControlPopup->SetCommandBar(pPopupBar);
		pPopupBar->InternalRelease();

		pControlPopup->SetFlags(xtpFlagManualUpdate);

		lpCreateControl->pControl = pControlPopup;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_CONDITION_ITEMSELECT)
	{
		CXTPControlPopup* pControlPopup = CXTPControlPopup::CreateControlPopup(xtpControlButtonPopup);
		CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());

		CXTPControlGallery* pControlGallery = new CXTPControlGallery();
		pControlGallery->SetControlSize(CSize(150, 40*6 + 2));
		pControlGallery->SetResizable();
		pControlGallery->SetItems(m_pItemsConditionItemSecect);
		pControlGallery->ShowScrollBar(FALSE);
		pControlGallery->SetResizable(FALSE,FALSE);

		pPopupBar->GetControls()->Add(pControlGallery, ID_GALLERY_CONDITION_ITEMSELECT);
		pPopupBar->GetControls()->Add(xtpControlButton, ID_CONDITION_OTHERITEM);
		pControlPopup->SetCommandBar(pPopupBar);
		pPopupBar->InternalRelease();

		pControlPopup->SetFlags(xtpFlagManualUpdate);

		lpCreateControl->pControl = pControlPopup;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_CONDITION_DATABARS)
	{
		CXTPControlPopup* pControlPopup = CXTPControlPopup::CreateControlPopup(xtpControlButtonPopup);
		CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());

		CXTPControlGallery* pControlGallery = new CXTPControlGallery();
		pControlGallery->SetControlSize(CSize(40*3, 40*2+2));
		pControlGallery->SetResizable();
		pControlGallery->SetItems(m_pItemsConditionDataBars);
		pControlGallery->ShowScrollBar(FALSE);
		pControlGallery->SetResizable(FALSE,FALSE);

		pPopupBar->GetControls()->Add(pControlGallery, ID_GALLERY_CONDITION_DATABARS);
		pPopupBar->GetControls()->Add(xtpControlButton, ID_CONDITION_OTHERORDER);
		pControlPopup->SetCommandBar(pPopupBar);
		pPopupBar->InternalRelease();

		pControlPopup->SetFlags(xtpFlagManualUpdate);

		lpCreateControl->pControl = pControlPopup;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_CONDITION_CLRVALEUR)
	{
		CXTPControlPopup* pControlPopup = CXTPControlPopup::CreateControlPopup(xtpControlButtonPopup);
		CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());

		CXTPControlGallery* pControlGallery = new CXTPControlGallery();
		pControlGallery->SetControlSize(CSize(40*4, 40*2+2));
		pControlGallery->SetResizable();
		pControlGallery->SetItems(m_pItemsConditionClrValeur);
		pControlGallery->ShowScrollBar(FALSE);
		pControlGallery->SetResizable(FALSE,FALSE);

		pPopupBar->GetControls()->Add(pControlGallery, ID_GALLERY_CONDITION_CLRVALEUR);
		pPopupBar->GetControls()->Add(xtpControlButton, ID_CONDITION_OTHERCOLOR);
		pControlPopup->SetCommandBar(pPopupBar);
		pPopupBar->InternalRelease();

		pControlPopup->SetFlags(xtpFlagManualUpdate);

		lpCreateControl->pControl = pControlPopup;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_CONDITION_ICONLIST)
	{
		CXTPControlPopup* pControlPopup = CXTPControlPopup::CreateControlPopup(xtpControlButtonPopup);
		CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());

		CXTPControlGallery* pControlGallery = new CXTPControlGallery();
		pControlGallery->SetControlSize(CSize(26*5*2, 26*9+2));
		pControlGallery->SetResizable();
		pControlGallery->SetItems(m_pItemsConditionIconList);
		pControlGallery->ShowScrollBar(FALSE);
		pControlGallery->SetResizable(FALSE,FALSE);

		pPopupBar->GetControls()->Add(pControlGallery, ID_GALLERY_CONDITION_ICONLIST);
		pPopupBar->GetControls()->Add(xtpControlButton, ID_CONDITION_OTHERICON);
		pControlPopup->SetCommandBar(pPopupBar);
		pPopupBar->InternalRelease();

		pControlPopup->SetFlags(xtpFlagManualUpdate);

		lpCreateControl->pControl = pControlPopup;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_CONDITION_CLEAR)
	{
		CXTPControlPopup* pControlPopup = CXTPControlPopup::CreateControlPopup(xtpControlButtonPopup);
		CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());
		pPopupBar->GetControls()->Add(xtpControlButton, ID_CONDITION_CLEARCELL);
		pPopupBar->GetControls()->Add(xtpControlButton, ID_CONDITION_CLEARSHEET);
		pPopupBar->GetControls()->Add(xtpControlButton, ID_CONDITION_CLEARTABLE);
		pPopupBar->GetControls()->Add(xtpControlButton, ID_CONDITION_CLEARTABLE);
		pControlPopup->SetCommandBar(pPopupBar);
		pPopupBar->InternalRelease();

		pControlPopup->SetFlags(xtpFlagManualUpdate);
		pControlPopup->SetHeight(24);
		pControlPopup->SetIconSize(CSize(16,16));

		lpCreateControl->pControl = pControlPopup;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_GALLERY_STYLES_FORMAT)
	{
		CXTPControlGallery* pControlGallery = new CXTPControlGallery();
		pControlGallery->SetControlSize(CSize(65*7+18, 50*10));
		pControlGallery->SetResizable();
		pControlGallery->SetItems(m_pItemsStylesFormat);
		pControlGallery->SetResizable(FALSE,FALSE);

		lpCreateControl->pControl = pControlGallery;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_GALLERY_STYLES_CELL)
	{
		CXTPControlGallery* pControlGallery = new CXTPControlGallery();
		pControlGallery->SetControlSize(CSize(104*4+18, 24*18));
		pControlGallery->SetResizable();
		pControlGallery->SetItems(m_pItemsStylesCell);
		pControlGallery->SetResizable(FALSE,FALSE);

		lpCreateControl->pControl = pControlGallery;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_GALLERY_SHAPES)
	{
		CXTPControlGallery* pControlGallery = new CXTPControlGallery();
		pControlGallery->SetControlSize(CSize(200, 400));
		pControlGallery->SetResizable();
		pControlGallery->ShowLabels(TRUE);
		pControlGallery->SetItems(m_pItemsShapes);
		pControlGallery->SetResizable(FALSE,FALSE);

		lpCreateControl->pControl = pControlGallery;
		return TRUE;
	}

	if (lpCreateControl->nID >= ID_OPTIONS_STYLE_BLUE2007 && lpCreateControl->nID <= ID_OPTIONS_STYLE_SYSTEM)
	{		
		lpCreateControl->controlType = xtpControlRadioButton;
		return TRUE;
	}

	if (lpCreateControl->nID >= ID_OPTIONS_FONT_SYSTEM && lpCreateControl->nID <= ID_OPTIONS_FONT_EXTRALARGE)
	{		
		lpCreateControl->controlType = xtpControlRadioButton;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_FILE_RECENTDOCUMENTS)
	{		
		lpCreateControl->pControl = new CXTPRibbonControlSystemRecentFileList();
		return TRUE;
	}

	if (lpCreateControl->nID == ID_APP_EXIT || lpCreateControl->nID == ID_APP_OPTIONS)
	{		
		lpCreateControl->pControl = new CXTPRibbonControlSystemPopupBarButton();

		return TRUE;
	}

	if (lpCreateControl->nID == ID_FILE_PRINT && lpCreateControl->nIndex == 5)
	{		
		CXTPControlPopup* pControl = CXTPControlPopup::CreateControlPopup(xtpControlSplitButtonPopup);

		CMenu menu;
		menu.LoadMenu(ID_FILE_PRINT);

		CXTPRibbonSystemPopupBarPage* pPopupBar = new CXTPRibbonSystemPopupBarPage();
		pPopupBar->SetCommandBars(GetCommandBars());

		pPopupBar->LoadMenu(menu.GetSubMenu(0));

		CXTPControl* pControlLabel = pPopupBar->GetControls()->Add(xtpControlLabel, ID_FILE_PRINT_LABEL, NULL, 0);
		pControlLabel->SetWidth(296);
		pControlLabel->SetItemDefault(TRUE);
		pControlLabel->SetStyle(xtpButtonCaption);

		pPopupBar->SetShowGripper(FALSE);
		pPopupBar->SetDefaultButtonStyle(xtpButtonCaptionAndDescription);
		pPopupBar->SetIconSize(CSize(32, 32));

		pControl->SetCommandBar(pPopupBar);

		pPopupBar->InternalRelease();

		lpCreateControl->pControl = pControl;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_FILE_SEND)
	{		
		CXTPControlPopup* pControl = CXTPControlPopup::CreateControlPopup(xtpControlButtonPopup);

		CMenu menu;
		menu.LoadMenu(ID_FILE_SEND);

		CXTPRibbonSystemPopupBarPage* pPopupBar = new CXTPRibbonSystemPopupBarPage();
		pPopupBar->SetCommandBars(GetCommandBars());

		pPopupBar->LoadMenu(menu.GetSubMenu(0));

		CXTPControl* pControlLabel = pPopupBar->GetControls()->Add(xtpControlLabel, ID_FILE_SEND_LABEL, NULL, 0);
		pControlLabel->SetWidth(296);
		pControlLabel->SetItemDefault(TRUE);
		pControlLabel->SetStyle(xtpButtonCaption);

		pPopupBar->SetShowGripper(FALSE);
		pPopupBar->SetDefaultButtonStyle(xtpButtonCaptionAndDescription);
		pPopupBar->SetIconSize(CSize(32, 32));

		pControl->SetCommandBar(pPopupBar);

		pPopupBar->InternalRelease();

		pControl->SetFlags(xtpFlagManualUpdate);

		lpCreateControl->pControl = pControl;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_FILE_DATA)
	{		
		CXTPControlPopup* pControl = CXTPControlPopup::CreateControlPopup(xtpControlButtonPopup);

		CMenu menu;
		menu.LoadMenu(ID_FILE_DATA);

		CXTPRibbonSystemPopupBarPage* pPopupBar = new CXTPRibbonSystemPopupBarPage();
		pPopupBar->SetCommandBars(GetCommandBars());

		pPopupBar->LoadMenu(menu.GetSubMenu(0));

		CXTPControl* pControlLabel = pPopupBar->GetControls()->Add(xtpControlLabel, ID_FILE_DATA_LABEL, NULL, 0);
		pControlLabel->SetWidth(296);
		pControlLabel->SetItemDefault(TRUE);
		pControlLabel->SetStyle(xtpButtonCaption);

		pPopupBar->SetShowGripper(FALSE);
		pPopupBar->SetDefaultButtonStyle(xtpButtonCaptionAndDescription);
		pPopupBar->SetIconSize(CSize(32, 32));

		pControl->SetCommandBar(pPopupBar);

		pPopupBar->InternalRelease();

		pControl->SetFlags(xtpFlagManualUpdate);

		lpCreateControl->pControl = pControl;
		return TRUE;
	}

	return FALSE;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	m_strTime = CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));
	m_pStatusBarTime->SetText(m_strTime);
}

void CMainFrame::OnClose() 
{
#ifndef _SAVE_TO_XML
	SaveCommandBars(_T("CommandBars_Ribbon"));
#else

	CXTPPropExchangeXMLNode px(FALSE, 0, _T("Settings"));

	if (px.OnBeforeExchange()) 
	{
		CXTPPropExchangeSection pxCommandBars(px.GetSection(_T("CommandBars")));
		XTP_COMMANDBARS_PROPEXCHANGE_PARAM param; 
		param.bSerializeControls = TRUE; 
		param.bSaveOriginalControls = FALSE;
		param.bSerializeOptions = TRUE;
		GetCommandBars()->DoPropExchange(&pxCommandBars, &param);

		px.SaveToFile(m_strIniFileName);
	}
#endif


	CXTPFrameWnd::OnClose();
}

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_ACTION)
	{
		XTP_DOCKINGPANE_ACTION* pAction = (XTP_DOCKINGPANE_ACTION*)lParam;

		if (pAction->action == xtpPaneActionDocked || pAction->action == xtpPaneActionExpanding)
			XTPMouseManager()->SendTrackLost();
	}

	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

		if (!pPane->IsValid())
		{
			switch (pPane->GetID())
			{
			case IDR_PANE_PROPERTIES:
				{
					if (m_wndPaneProperties.GetSafeHwnd() == NULL)
					{
						m_wndPaneProperties.Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);
						XTPMouseManager()->AddTrustedWindow(m_wndPaneProperties);
					}

					pPane->Attach(&m_wndPaneProperties);
				}
				break;
			case IDR_PANE_RIBBON:
				{
					if (m_wndPaneRibbon.GetSafeHwnd() == NULL)
					{
						m_wndPaneRibbon.Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);
					}

					pPane->Attach(&m_wndPaneRibbon);
				}
				break;
			case IDR_PANE_POINTS:
				{
					if (m_wndPanePoints.GetSafeHwnd() == NULL)
					{
						m_wndPanePoints.Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);
					}

					pPane->Attach(&m_wndPanePoints);
				}
				break;
			}
		}

		RefreshPanes();
		return TRUE;
	}

	return FALSE;
}

LRESULT CMainFrame::OnTrayIconNotify(WPARAM /*wParam*/, LPARAM lParam)
{
	//UINT uID       = (UINT)wParam;  // resource ID of the tray icon.
	UINT uMouseMsg = (UINT)lParam;  // mouse message that was sent.


	// We can let the tray icon control handle our context menu and
	// mouse double click events, but we want handle our balloon tip
	// notifications, so we will return 1 to let the tray icon control
	// know that we have handled these messages already...

	switch( uMouseMsg )
	{
		// Sent when the balloon is shown (balloons are queued).
	case NIN_BALLOONSHOW:
		{
		}
		return 1;

		// Sent when the balloon disappears-for example, when the
		// icon is deleted. This message is not sent if the balloon
		// is dismissed because of a timeout or a mouse click.
	case NIN_BALLOONHIDE:
		{
		}
		return 1;

		// Sent when the balloon is dismissed because of a timeout.
	case NIN_BALLOONTIMEOUT:
		{
		}
		return 1;

		// Sent when the balloon is dismissed because of a mouse click.
	case NIN_BALLOONUSERCLICK:
		{
			xtpchar path[MAX_PATH];
			GetModuleFileName(AfxGetApp()->m_hInstance,path,MAX_PATH);
			CXTPString xml(path);
			xml = xml.Mid(0,xml.ReverseFind('\\'));
			theCon.SetDataPath(xml);
			xml = xml+"\\RibbonReport.xml";
			CXTPTinyXmlDocument doc(xml);
			if(doc.LoadFile())
			{
				CXTPTinyXmlNode* node = doc.RootElement();
				if(!node) return 0;
				CXTPTinyXmlElement* root = node->ToElement();
				if(!root) return 0;
				CXTPString val(root->Attribute("Net"));
				CXTPHyperLink hyperlink;
				hyperlink.GotoURL(val);
				doc.SaveFile();
			}
		}
		return 1;

	case WM_RBUTTONUP:
		{
			CMenu menu;
			if (!menu.LoadMenu(IDR_CONTENT_TRAYICON)) {
				return 0;
			}

			CMenu* pSubMenu = menu.GetSubMenu(0);
			if (pSubMenu == NULL) {
				return 0;
			}

			::SetMenuDefaultItem(pSubMenu->m_hMenu, ID_TRAYICON_MIX_MAX, FALSE);

			// Display the menu at the current mouse location. There's a "bug"
			// (Microsoft calls it a feature) in Windows 95 that requires calling
			// SetForegroundWindow. To find out more, search for Q135788 in MSDN.
			//
			CPoint pos;
			GetCursorPos(&pos);
			::SetForegroundWindow(m_hWnd);

			CXTPCommandBars::TrackPopupMenu(pSubMenu, 0, pos.x, pos.y,this);

			::PostMessage(m_hWnd, WM_NULL, 0, 0);

			menu.DestroyMenu();
		}
		return 1;
	}

	return 0;
}

void CMainFrame::OnTrayIconMixmax()
{
	m_bMinimized = !m_bMinimized;

	if (m_bMinimized)
		m_wndTrayIcon.MinimizeToTray(this);
	else
		m_wndTrayIcon.MaximizeFromTray(this);
}

void CMainFrame::OnTrayIconShowdlg()
{
	// Display the properties dialog.
	CDlgTrayIcon dlg;
	dlg.DoModal(this);
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame Menu Item Update

void CMainFrame::RedrawFrame(CWnd* pWnd)
{
	pWnd->RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);		
}

void CMainFrame::SkinFrameChanged()
{
	GetCommandBars()->SetTheme(xtpThemeNone);
	XTPPaintManager()->RefreshMetrics();

	m_wndPaneManager.GetPaintManager()->RefreshMetrics();
	m_wndPaneManager.RedrawPanes();
	
	if (m_pChartView)
		RedrawFrame(m_pChartView);

	if(::IsWindow(m_wndSkinThemes.m_hWnd))
	{
		m_wndTabClient.GetPaintManager()->RefreshMetrics();
		RedrawFrame(&m_wndSkinThemes);
	}

	if(::IsWindow(m_wndPaneProperties.m_hWnd))
	{
		m_wndPaneManager.GetPaintManager()->RefreshMetrics();
		RedrawFrame(&m_wndPaneProperties);
	}

	if(::IsWindow(m_wndPaneRibbon.m_hWnd))
	{
		m_wndPaneManager.GetPaintManager()->RefreshMetrics();
		RedrawFrame(&m_wndPaneRibbon);
	}

	if(::IsWindow(m_wndPanePoints.m_hWnd))
	{
		m_wndPaneManager.GetPaintManager()->RefreshMetrics();
		RedrawFrame(&m_wndPanePoints);
	}

	if (m_pRibbonReportView)
	{
		m_pRibbonReportView->GetPaintManager()->RefreshMetrics();
		RedrawFrame(&m_pRibbonReportView->GetReportCtrl());
	}

	GetCommandBars()->GetPaintManager()->RefreshMetrics();
	GetCommandBars()->RedrawCommandBars();
}

void CMainFrame::SkinSchemaChanged(int nSchema)
{
	switch (nSchema)
	{
	case 0:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Office2007.cjstyles"), _T("NormalBlue.ini"));
		break;
	case 1:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Office2007.cjstyles"), _T("NormalAqua.ini"));
		break;
	case 2:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Office2007.cjstyles"), _T("NormalSilver.ini"));
		break;
	case 3:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Office2007.cjstyles"), _T("NormalBlack.ini"));
		break;
	case 4:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("WinXP.Luna.cjstyles"), _T("NormalBlue.ini"));
		break;
	case 5:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("WinXP.Luna.cjstyles"), _T("NormalHomestead.ini"));
		break;
	case 6:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("WinXP.Luna.cjstyles"), _T("NormalMetallic.ini"));
		break;
	case 7:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("WinXP.Luna.cjstyles"), _T("NormalAqua.ini"));
		break;
	case 8:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("WinXP.Royale.cjstyles"), _T("NormalRoyale.ini"));
		break;
	case 9:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Vista.cjstyles"), _T("NormalBlue.ini"));
		break;
	case 10:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Vista.cjstyles"), _T("NormalSilver.ini"));
		break;
	case 11:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Vista.cjstyles"), _T("NormalBlack.ini"));
		break;
	case 12:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Vista.cjstyles"), _T("NormalBlack2.ini"));
		break;
	case 13:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Office2010.cjstyles"), _T("NormalBlue.ini"));
		break;
	case 14:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Office2010.cjstyles"), _T("NormalSilver.ini"));
		break;
	case 15:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Office2010.cjstyles"), _T("NormalBlack.ini"));
		break;
	case 16:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Codejock.cjstyles"), _T("NormalBlue.ini"));
		break;
	case 17:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Codejock.cjstyles"), _T("NormalBlack.ini"));
		break;
	case 18:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("iTunes"), _T("Normalitunes.ini"));
		break;
	case 19:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Le5"), _T("NormalBlue.ini"));
		break;
	case 20:
		XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Zune.msstyles"));
		break;
	default:
		XTPSkinManager()->LoadSkin(_T(""), _T(""));
		break;
	}

	if (m_hWnd) SkinFrameChanged();
}

void CMainFrame::SkinThemeChanged(int nTheme)
{
	int _nTheme = (nTheme+1) % 4;

	if (_nTheme == xtpThemeOffice2003)
	{
		if (XTPSkinManager()->GetResourceFile() &&
			XTPSkinManager()->GetResourceFile()->GetResourcePath().Find(_T("Office2007")) > 0)
			_nTheme = xtpThemeOffice2007;
	}

	CXTPPaintManager::SetTheme((XTPPaintTheme)_nTheme);
	XTPPaintManager()->GetIconsInfo()->bUseFadedIcons = FALSE;
	XTPPaintManager()->GetIconsInfo()->bIconsWithShadow = FALSE;
	XTPPaintManager()->RefreshMetrics();

	m_wndPaneManager.SetTheme((XTPDockingPanePaintTheme)_nTheme);
	m_wndPaneManager.GetPaintManager()->RefreshMetrics();
	m_wndPaneManager.RedrawPanes();

	if(::IsWindow(m_wndSkinThemes.m_hWnd))
	{
		m_wndTabClient.GetPaintManager()->SetColor((XTPTabColorStyle)_nTheme);
		m_wndTabClient.GetPaintManager()->RefreshMetrics();
		RedrawFrame(&m_wndSkinThemes);
	}

	if(::IsWindow(m_wndPaneProperties.m_hWnd))
	{
		m_wndPaneManager.GetPaintManager()->RefreshMetrics();
		RedrawFrame(&m_wndPaneProperties);
	}

	if(::IsWindow(m_wndPaneRibbon.m_hWnd))
	{
		m_wndPaneManager.GetPaintManager()->RefreshMetrics();
		RedrawFrame(&m_wndPaneRibbon);
	}

	if(::IsWindow(m_wndPanePoints.m_hWnd))
	{
		m_wndPaneManager.GetPaintManager()->RefreshMetrics();
		RedrawFrame(&m_wndPanePoints);
	}

	if (m_pRibbonReportView)
	{
		m_pRibbonReportView->GetPaintManager()->SetColumnStyle((XTPReportColumnStyle)_nTheme);
		m_pRibbonReportView->GetPaintManager()->RefreshMetrics();
		RedrawFrame(&m_pRibbonReportView->GetReportCtrl());
	}

	if (m_pChartView)
		RedrawFrame(m_pChartView);

	GetCommandBars()->GetPaintManager()->RefreshMetrics();
	GetCommandBars()->RedrawCommandBars();
}

void CMainFrame::OnSkinTheme() 
{
	SkinThemeChanged((m_nTheme+1) % 4);
	if (!::IsWindow(m_wndSkinThemes.GetSafeHwnd()))
	{
		m_wndSkinThemes.Create(this);
		m_wndSkinThemes.CenterWindow(this);
	}
	else
	{
		m_wndSkinThemes.SetFocus();
	}
}

void CMainFrame::OnSkinThemePopup(UINT id)
{
	m_nTheme = id - ID_THEME_OFFICE2007_BLUE;
	SkinSchemaChanged(m_nTheme);
	SkinThemeChanged(m_nTheme);
}

void CMainFrame::OnUpdateSkinThemePopup(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetCheck((int)pCmdUI->m_nID == m_nTheme + ID_THEME_OFFICE2007_BLUE);
}

void CMainFrame::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
{
	// Toggle CommandBars
	GetCommandBars()->OnSetPreviewMode(bPreview);

	// Toggle Tab Client
	m_wndTabClient.ShowWorkspace(!bPreview);

	CXTPFrameWnd::OnSetPreviewMode(bPreview, pState);
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame Menu Item Update

void CMainFrame::OnCustomize()
{
	ShowCustomizeDialog(0);

	// get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars == NULL)
		return;

	// instanciate the customize dialog
	CXTPCustomizeSheet dlg(pCommandBars);

	// add the options page to the customize dialog.
	CXTPCustomizeOptionsPage pageOptions(&dlg);
	dlg.AddPage(&pageOptions);

	// add the commands page to the customize dialog.
	CXTPCustomizeCommandsPage* pPageCommands = dlg.GetCommandsPage();
	pPageCommands->AddCategories(IDR_MAINFRAME);

	// initialize the commands page page.
	pPageCommands->InsertAllCommandsCategory();
	pPageCommands->InsertBuiltInMenus(IDR_MAINFRAME);
	pPageCommands->InsertNewMenuCategory();

	// display the customize dialog.
	dlg.DoModal();
}

void CMainFrame::ShowCustomizeDialog(int nSelectedPage)
{
#if 0
	// Classic Office Customize dialog
	CXTPCustomizeSheet cs(GetCommandBars());

	CXTPRibbonCustomizeQuickAccessPage pageQuickAccess(&cs);
	cs.AddPage(&pageQuickAccess);
	pageQuickAccess.AddCategories(IDR_MAINFRAME);

	CXTPCustomizeKeyboardPage pageKeyboard(&cs);
	cs.AddPage(&pageKeyboard);
	pageKeyboard.AddCategories(IDR_MAINFRAME);

	CXTPCustomizeMenusPage pageMenus(&cs);
	cs.AddPage(&pageMenus);

	CXTPCustomizeOptionsPage pageOptions(&cs);
	cs.AddPage(&pageOptions);

	CXTPCustomizeCommandsPage* pCommands = cs.GetCommandsPage();
	pCommands->AddCategories(IDR_MAINFRAME);

	cs.SetActivePage(nSelectedPage);

	cs.SetResizable();
	cs.DoModal();

#else
	CXTPPropertySheet cs(_T("选项"));
	cs.m_psh.dwFlags |= PSH_NOAPPLYNOW;

	CXTPPropertyPageListNavigator* pList = new CXTPPropertyPageListNavigator();
	pList->SetListStyle(xtpListBoxOffice2007);

	cs.SetNavigator(pList);


	CCustomizePageGeneral pageGeneral;
	cs.AddPage(&pageGeneral);

	CCustomizePageRibbon pageCustomizeRibbon(GetCommandBars());
	cs.AddPage(&pageCustomizeRibbon);
	pageCustomizeRibbon.AddCategories(IDR_MAINFRAME);

	CCustomizePageQuickAccessToolbar pageQuickAccess(GetCommandBars());
	cs.AddPage(&pageQuickAccess);
	pageQuickAccess.AddCategories(IDR_MAINFRAME);

	cs.SetResizable();
	cs.DoModal();

#endif
}

void CMainFrame::OnCustomizeQuickAccess()
{
	ShowCustomizeDialog(2);
}

void CMainFrame::SetReportGeneratorView()
{
	m_wndSplitter.ShowColumn();

	if (m_pReportGeneratorView == NULL)
	{
		if (!m_wndSplitter.GetPane(0,0)->IsKindOf(RUNTIME_CLASS(CReportGeneratorView)))
			m_wndSplitter.ReplaceView(0,0,RUNTIME_CLASS(CReportGeneratorView));

		CWnd* pWnd = m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0,0));
		ASSERT_KINDOF(CReportGeneratorView, pWnd);
		m_pReportGeneratorView = (CReportGeneratorView*)pWnd;
	}
	else
		m_wndSplitter.ReplaceView(0,0,(CView*)m_pReportGeneratorView);

	m_wndSplitter.HideColumn(1);
	SetActiveView(m_pReportGeneratorView);
}

void CMainFrame::SetRibbonReportView()
{
	m_wndSplitter.ShowColumn();

	if (m_pRibbonReportView == NULL)
	{
		if (!m_wndSplitter.GetPane(0,1)->IsKindOf(RUNTIME_CLASS(CRibbonReportView)))
			m_wndSplitter.ReplaceView(0,1,RUNTIME_CLASS(CRibbonReportView));

		CWnd* pWnd = m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0,1));
		ASSERT_KINDOF(CRibbonReportView, pWnd);
		m_pRibbonReportView = (CRibbonReportView*)pWnd;
	}
	else
		m_wndSplitter.ReplaceView(0,1,(CView*)m_pRibbonReportView);

	m_wndSplitter.HideColumn(0);
	SetActiveView(m_pRibbonReportView);
}

void CMainFrame::SetChartView()
{
	m_wndSplitter.ShowColumn();

	if (m_pChartView == NULL)
	{
		if (!m_wndSplitter.GetPane(0,0)->IsKindOf(RUNTIME_CLASS(CChartView)))
			m_wndSplitter.ReplaceView(0,0,RUNTIME_CLASS(CChartView));

		CWnd* pWnd = m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0,0));
		ASSERT_KINDOF(CChartView, pWnd);
		m_pChartView = (CChartView*)pWnd;
	}
	else
		m_wndSplitter.ReplaceView(0,0,(CView*)m_pChartView);

	m_wndSplitter.HideColumn(1);
	SetActiveView(m_pChartView);
}

void CMainFrame::SetWebHtmlView()
{
	m_wndSplitter.ShowColumn();

	if (m_pWebHtmlView == NULL)
	{
		if (!m_wndSplitter.GetPane(0,0)->IsKindOf(RUNTIME_CLASS(CWebHtmlView)))
			m_wndSplitter.ReplaceView(0,0,RUNTIME_CLASS(CWebHtmlView));

		CWnd* pWnd = m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0,0));
		ASSERT_KINDOF(CWebHtmlView, pWnd);
		m_pWebHtmlView = (CWebHtmlView*)pWnd;
	}
	else
		m_wndSplitter.ReplaceView(0,0,(CView*)m_pWebHtmlView);

	m_wndSplitter.HideColumn(1);
	SetActiveView(m_pWebHtmlView);
}

void CMainFrame::SetTaskTrackView()
{
	m_wndSplitter.ShowColumn();

	if (m_pTaskTrackView == NULL)
	{
		if (!m_wndSplitter.GetPane(0,0)->IsKindOf(RUNTIME_CLASS(CTaskTrackView)))
			m_wndSplitter.ReplaceView(0,0,RUNTIME_CLASS(CTaskTrackView));

		CWnd* pWnd = m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0,0));
		ASSERT_KINDOF(CTaskTrackView, pWnd);
		m_pTaskTrackView = (CTaskTrackView*)pWnd;
	}
	else
		m_wndSplitter.ReplaceView(0,0,(CView*)m_pTaskTrackView);

	m_wndSplitter.HideColumn(1);
	SetActiveView(m_pTaskTrackView);
}

void CMainFrame::SetShellListView()
{
	m_wndSplitter.ShowColumn();

	if (m_pShellListView == NULL)
	{
		if (!m_wndSplitter.GetPane(0,1)->IsKindOf(RUNTIME_CLASS(CShellListView)))
			m_wndSplitter.ReplaceView(0,1,RUNTIME_CLASS(CShellListView));

		CWnd* pWnd = m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0,1));
		ASSERT_KINDOF(CShellListView, pWnd);
		m_pShellListView = (CShellListView*)pWnd;
	}
	else
		m_wndSplitter.ReplaceView(0,1,(CView*)m_pShellListView);

	SetActiveView(m_pShellListView);
}

void CMainFrame::SetShellTreeView()
{
	m_wndSplitter.ShowColumn();

	if (m_pShellTreeView == NULL)
	{
		if (!m_wndSplitter.GetPane(0,0)->IsKindOf(RUNTIME_CLASS(CShellTreeView)))
			m_wndSplitter.ReplaceView(0,0,RUNTIME_CLASS(CShellTreeView));

		CWnd* pWnd = m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0,0));
		ASSERT_KINDOF(CShellTreeView, pWnd);
		m_pShellTreeView = (CShellTreeView*)pWnd;
	}
	else
		m_wndSplitter.ReplaceView(0,0,(CView*)m_pShellTreeView);

	SetActiveView(m_pShellTreeView);
	m_pShellTreeView->SelectParentItem();
}

void CMainFrame::SetSearchView()
{
	m_wndSplitter.ShowColumn();

	if (m_pSearchView == NULL)
	{
		if (!m_wndSplitter.GetPane(0,0)->IsKindOf(RUNTIME_CLASS(CSearchView)))
		{
			m_wndSplitter.ReplaceView(0,0,RUNTIME_CLASS(CSearchView));
			CWnd* pWnd = m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0,0));
			ASSERT_KINDOF(CSearchView, pWnd);
			m_pSearchView = (CSearchView*)pWnd;
		}
	}
	else
		m_wndSplitter.ReplaceView(0,0,(CView*)m_pSearchView);

	SetActiveView(m_pSearchView);
}

BOOL CMainFrame::IsActiveView(CView* pView)
{
	if(GetActiveView()->IsKindOf(pView->GetRuntimeClass()))
		return TRUE;

	return FALSE;
}

void CMainFrame::RefreshPanes()
{
	if (m_wndPaneProperties.GetSafeHwnd())
	{
		if(GetActiveView()->IsKindOf(RUNTIME_CLASS(CReportRecordView)))
			m_wndPaneProperties.Refresh(m_pRibbonReportView, m_pActivePane);
		else if(GetActiveView()->IsKindOf(RUNTIME_CLASS(CChartView)))
			m_wndPaneProperties.Refresh(m_pChartView, m_pActivePane);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame Menu Item Update

void CMainFrame::OnShellStyle(UINT id)
{
	m_idShellStyle = id;
	if(!m_pShellListView) return;
	m_pShellListView->SetStyle(id);
}

void CMainFrame::OnUpdateShellStyle(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_idShellStyle ? TRUE : FALSE);
}

void CMainFrame::OnShellSort(UINT id)
{
	m_idShellSort = id;
	if(!m_pShellListView) return;
	m_pShellListView->SetSort(id);
}

void CMainFrame::OnUpdateShellSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_idShellSort ? TRUE : FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame Menu Item Update

void XlsToCsv_xls()
{
	try
	{
		CXTPExcelUtil excel;
		excel.InitExcel();

		TCHAR szFilter[] = _T("Excel 2007|*.xlsx|Excel 2003|*.xls|CSV文件|*.csv|所有文件|*.*||");
		CFileDialog FileDlg(TRUE, _T("xlsx"), NULL, OFN_ALLOWMULTISELECT, szFilter, NULL);

		if(IDOK == FileDlg.DoModal())
		{
			CString strFilePath = FileDlg.GetPathName();
			CString strPath = strFilePath.Mid(0,strFilePath.ReverseFind('\\'))+_T("\\");

			excel.OpenExcel(strFilePath);

			strFilePath = strFilePath.Mid(0,strFilePath.ReverseFind('.'));
			excel.SaveAs(strFilePath, 6);
			msgInf(strFilePath+_T(" CSV文件生成成功!"));
		}

		excel.CloseExcel();
		excel.ReleaseExcel();

		LogInfo(L"XTPExcelUtil SaveAs Successful!");
	}
	catch(CException *e)
	{
		LogErrorExp(L"XTPExcelUtil SaveAs Faild!", e);
	}
}
void WordPic_doc()
{
	try
	{
		TCHAR szFilter[] = _T("JPG图像文件|*.jpg|BMP图像文件|*.bmp|PNG图像文件|*.png|所有文件|*.*||");
		CFileDialog FileDlg(TRUE, _T("jpg"), NULL, OFN_ALLOWMULTISELECT, szFilter, NULL);

		if(IDOK == FileDlg.DoModal())
		{
			CString strFilePath = FileDlg.GetPathName();
			CString strPath = strFilePath.Mid(0,strFilePath.ReverseFind('\\'))+_T("\\");

			CXTPWordUtil word;
			word.InitWord();
			word.CreateWord(strPath+_T("doc_pic.doc"));
			word.SetPageSetup(30,30,50,40);

			word.SetFont(_T("黑体"),25);
			word.SetParagraphFormat(1);
			word.WriteText(_T("你的标题"));

			word.BeginParagraph();
			word.SetFont(_T("仿宋"),13);
			word.SetParagraphFormat(3);

			word.InsertTable(2,3);
			word.WriteText(_T("表格项内容"));
			word.MoveRight(1,1);
			word.WriteText(_T("表格项内容"));
			word.MoveRight(1,1);
			word.WriteText(_T("表格项内容"));
			word.MoveDown(4,1);
			word.WriteText(_T("表格项内容"));
			word.MoveRight(1,1);
			word.WriteText(_T("表格项内容"));
			word.MoveRight(1,1);
			word.WriteText(COleDateTime::GetCurrentTime().Format(_T("打印日期：%Y-%m-%d")));
			word.MoveDown(4,1);
			word.MoveDown(5,1);
			word.WriteText(_T("其他内容"));

			word.BeginParagraph();
			word.InsertTable(2,6);
			word.WriteText(_T("pic1"));
			word.MoveRight(1,1);
			word.WriteText(_T("pic1"));
			word.MoveRight(1,1);
			word.WriteText(_T("pic1"));
			word.MoveRight(1,1);
			word.WriteText(_T("pic1"));
			word.MoveRight(1,1);
			word.WriteText(_T("pic1"));
			word.MoveRight(1,1);
			word.WriteText(_T("pic1"));
			word.MoveRight(1,1);
			word.MoveDown(4,1);

			for (int i=0; i<6;i++)
			{
				word.InsertShapes(strFilePath,80,80);
				word.MoveRight(1,1);
			}
			word.MoveDown(4,1);

			word.SaveAs(strPath+_T("doc_pic.doc"));

			word.CloseWord();
			word.ReleaseWord();
			msgInf(_T("doc_pic WORD图片插入完成!"));
		}

		LogInfo(L"XTPWordUtil Insert Picture Successful!");
	}
	catch(CException *e)
	{
		LogErrorExp(L"XTPWordUtil Insert Picture Faild!", e);
	}

}
void WordGrid_doc()
{
	try
	{
		TCHAR szFilter[] = _T("Word 2003|*.doc|Word 2007|*.docx|RTF文件|*.rtf|所有文件|*.*||");
		CFileDialog FileDlg(TRUE, _T("doc"), NULL, OFN_ALLOWMULTISELECT, szFilter, NULL);

		if(IDOK == FileDlg.DoModal())
		{
			CString strFilePath = FileDlg.GetPathName();
			CString strPath = strFilePath.Mid(0,strFilePath.ReverseFind('\\'))+_T("\\");

			CXTPWordUtil word;
			word.InitWord();
			word.OpenWord(strFilePath);

			word.MoveDown(4,1);
			word.MoveDown(5,1);
			word.WriteText(_T("123456789"));
			word.MoveRight(1,1);
			word.WriteText(_T("李明"));
			word.MoveRight(1,1);
			word.WriteText(_T("25"));
			word.MoveRight(1,1);
			word.WriteText(_T("技术员"));
			word.MoveRight(1,1);
			word.WriteText(_T("本科"));
			word.MoveRight(1,1);
			word.WriteText(_T("上海市虹口区民主路315号"));

			word.MoveDown(4,1);
			word.WriteText(_T("123456789"));
			word.MoveRight(1,1);
			word.WriteText(_T("李明"));
			word.MoveRight(1,1);
			word.WriteText(_T("25"));
			word.MoveRight(1,1);
			word.WriteText(_T("技术员"));
			word.MoveRight(1,1);
			word.WriteText(_T("本科"));
			word.MoveRight(1,1);
			word.WriteText(_T("上海市虹口区民主路315号"));

			word.SaveAs(strPath+_T("doc_grid.doc"));

			word.CloseWord();
			word.ReleaseWord();

			msgInf(_T("doc_grid WORD表格填写完毕!"));
		}

		LogInfo(L"XTPWordUtil Write Table Successful!");
	}
	catch(CException *e)
	{
		LogErrorExp(L"XTPWordUtil Write Table Faild!", e);
	}
}
void ConstLog_xls()
{
	try
	{
		CXTPExcelUtil excel;
		excel.InitExcel();

		TCHAR szFilter[] = _T("Excel 2007|*.xlsx|Excel 2003|*.xls|CSV文件|*.csv|所有文件|*.*||");
		CFileDialog FileDlg(TRUE, _T("xlsx"), NULL, OFN_ALLOWMULTISELECT, szFilter, NULL);

		if(IDOK == FileDlg.DoModal())
		{
			CString strFilePath = FileDlg.GetPathName();
			CString strPath = strFilePath.Mid(0,strFilePath.ReverseFind('\\'))+_T("\\");

			excel.OpenExcel(strFilePath);
			int nst = excel.GetSheetCount();

			if(nst > 0)
			{
				excel.LoadSheet(1);

				CString strTitle = excel.GetCell(1,1).bstrVal;
				long rows = excel.GetRowCount();
				long cols = excel.GetColCount();

				xtp_vector<xls_constlog> logs;
				for(long i=6; i<=cols; i++)
				{
					for(long j=4; j<=rows; j++)
					{
						VARIANT val = excel.GetCell(j,i);
						if(CString(val.bstrVal).IsEmpty()) continue;

						COleDateTime dt = val;
						int id = _ttoi(excel.GetCell(2,i).bstrVal);
						CString work = excel.GetCell(3,i).bstrVal;
						CString posi = excel.GetCell(j,2).bstrVal;
						CString unit = excel.GetCell(3,id+2).bstrVal;
						double qty = _ttof(excel.GetCell(j,id+2).bstrVal);
						logs.push_back(xls_constlog(dt,j,i,work,posi,unit,qty,id));
					}
				}
				sort(logs.begin(), logs.end(),less<xls_constlog>());//升序
				//sort(logs.begin(), logs.end(),greater<xls_constlog>());//降序

				CStdioFile file;
				if(file.Open(strPath+strTitle+_T(".txt"), CFile::modeCreate | CFile::modeReadWrite))
				{
					setlocale(LC_ALL,"");
					file.Seek(CFile::begin,0);

					xtp_vector<xls_constlog>::iterator iter = logs.begin();
					while(iter != logs.end())
					{
						COleDateTime dt = (*iter).val;
						CString* arr_work = new CString[cols-5];
						CString* arr_posi = new CString[cols-5];
						int* arr_num = new int[cols-5];
						for(int i=0; i<cols-5; i++)
						{
							arr_work[i].Empty();
							arr_posi[i].Empty();
							arr_num[i]=0;
						}

						xtp_vector<xls_constlog>::iterator _iter = iter;
						while((_iter = find(_iter, logs.end(), *iter)) != logs.end())
						{
							xls_constlog _val = *_iter++;
							if (_val.id == -1)
								arr_posi[_val.col-6] = arr_posi[_val.col-6]+_val.posi+_T("、");
							else
								arr_posi[_val.col-6] = arr_posi[_val.col-6]+_val.posi+_T(" ")+ToString(_val.qty,2)+_val.unit+_T("、");

							arr_work[_val.col-6] = _val.work;
							arr_num[_val.col-6]++;
						}

						CString str;
						str.Format(_T("施工日期：%s     晴      16-30度  南风3-4级\n"),dt.Format(_T("%Y年%m月%d日")));
						file.WriteString(str);
						file.WriteString(_T("----------------------当日施工进度---------------------\n"));

						int n=1;
						for(int i=0; i<cols-5; i++)
						{
							if(arr_posi[i].GetLength())
							{
								arr_posi[i] = arr_posi[i].Mid(0,arr_posi[i].GetLength()-1);
								str.Format(_T("(%d) %s共%d个，编号为：%s\n"),n++,arr_work[i],arr_num[i],arr_posi[i]);
								file.WriteString(str);
								str.Empty();
							}
						}

						file.WriteString(_T("=======================================================\n"));

						while(++iter != logs.end()) if((*iter).val != dt) break;
					}

					file.Close();
					msgInf(strTitle+_T(" 文件生成成功!"));
				}
			}
		}

		excel.CloseExcel();
		excel.ReleaseExcel();

		LogInfo(L"XTPExcelUtil Read ConstLog Data Successful!");
	}
	catch(CException *e)
	{
		LogErrorExp(L"XTPExcelUtil Read ConstLog Data Faild!", e);
	}
}
void ConstLog_csv()
{
	try
	{
		TCHAR szFilter[] = _T("CSV文件|*.csv|文本文件|*.txt|所有文件|*.*||");
		CFileDialog FileDlg(TRUE, _T("csv"), NULL, OFN_ALLOWMULTISELECT, szFilter, NULL);

		if(IDOK == FileDlg.DoModal())
		{
			CString strFilePath = FileDlg.GetPathName();
			CString strPath = strFilePath.Mid(0,strFilePath.ReverseFind('\\'))+_T("\\");
			CString strTitle;
			xtp_vector<xls_logconst> logs;
			int row = 0;
			int col = 0;

			CStdioFile file;
			if(file.Open(FileDlg.GetPathName(), CFile::modeRead))
			{
				setlocale(LC_ALL,"");

				CString strLine;
				file.ReadString(strLine);
				row++;
				strLine.Trim();
				if(strLine.GetLength() > 0)
				{
					CXTPString xtpstr(strLine);
					vector<CXTPString> strs;
					xtpstr.ParseSubStrByDelimiter(strs,_T(","));
					strTitle = strs[0].t_str();
				}

				vector<CXTPString> ids;
				file.ReadString(strLine);
				row++;
				strLine.Trim();
				if(strLine.GetLength() > 0)
				{
					CXTPString xtpstr(strLine);
					xtpstr.ParseSubStrByDelimiter(ids,_T(","));
				}

				vector<CXTPString> works;
				file.ReadString(strLine);
				row++;
				strLine.Trim();
				if(strLine.GetLength() > 0)
				{
					CXTPString xtpstr(strLine);
					xtpstr.ParseSubStrByDelimiter(works,_T(","));
				}

				while(file.ReadString(strLine))
				{
					row++;
					strLine.Trim();
					if(strLine.GetLength() == 0)
						continue;

					CXTPString xtpstr(strLine);
					vector<CXTPString> strs;
					xtpstr.ParseSubStrByDelimiter(strs,_T(","));

					for(col=5; col<strs.size(); col++)
					{
						if(CString(strs[col].t_str()).IsEmpty()) continue;

						int id = _ttoi(ids[col].t_str());
						CString posi = strs[1].t_str();
						CString work = works[col].t_str();
						CString unit = works[id+1].t_str();
						double qty = _ttof(strs[id+1].t_str());
						logs.push_back(xls_logconst(strs[col].t_str(),row,col+1,work,posi,unit,qty,id));
					}
				}
				sort(logs.begin(), logs.end(),less<xls_logconst>());//升序
				//sort(logs.begin(), logs.end(),greater<xls_logconst>());//降序

				file.Close();
			}

			if(file.Open(strPath+strTitle+_T(".txt"), CFile::modeCreate | CFile::modeReadWrite))
			{
				setlocale(LC_ALL,"");
				file.Seek(CFile::begin,0);

				xtp_vector<xls_logconst>::iterator iter = logs.begin();
				while(iter != logs.end())
				{
					CString val = (*iter).val;
					CString* arr_work = new CString[col-5];
					CString* arr_posi = new CString[col-5];
					int* arr_num = new int[col-5];
					for(int i=0; i<col-5; i++)
					{
						arr_work[i].Empty();
						arr_posi[i].Empty();
						arr_num[i]=0;
					}

					xtp_vector<xls_logconst>::iterator _iter = iter;
					while((_iter = find(_iter, logs.end(), *iter)) != logs.end())
					{
						xls_logconst _val = *_iter++;
						if (_val.id == -1)
							arr_posi[_val.col-6] = arr_posi[_val.col-6]+_val.posi+_T("、");
						else
							arr_posi[_val.col-6] = arr_posi[_val.col-6]+_val.posi+_T(" ")+ToString(_val.qty,2)+_val.unit+_T("、");

						arr_work[_val.col-6] = _val.work;
						arr_num[_val.col-6]++;
					}

					CString str;
					str.Format(_T("施工日期：%s     晴      16-30度  南风3-4级\n"),val);
					file.WriteString(str);
					file.WriteString(_T("----------------------当日施工进度---------------------\n"));

					int n=1;
					for(int i=0; i<col-5; i++)
					{
						if(arr_posi[i].GetLength())
						{
							arr_posi[i] = arr_posi[i].Mid(0,arr_posi[i].GetLength()-1);
							str.Format(_T("(%d) %s共%d个，编号为：%s\n"),n++,arr_work[i],arr_num[i],arr_posi[i]);
							file.WriteString(str);
							str.Empty();
						}
					}

					file.WriteString(_T("=======================================================\n"));

					while(++iter != logs.end()) if((*iter).val.Compare(val) != 0) break;
				}

				file.Close();
				msgInf(strTitle+_T(" 文件生成成功!"));
			}
		}

		LogInfo(L"XTPExcelUtil Read ConstLog Data Successful!");
	}
	catch(CException *e)
	{
		LogErrorExp(L"XTPExcelUtil Read ConstLog Data Faild!", e);
	}
}
void ConstLog_sql()
{
	try
	{
		TCHAR szFilter[] = _T("Excel2007文件|*.xlsx|Excel2003文件|*.xls|所有文件|*.*||");
		CFileDialog FileDlg(TRUE, _T("xlsx"), NULL, 0, szFilter, NULL);
		if(IDOK == FileDlg.DoModal())
		{
			LogInfo(L"FileDialog Open Excel File Successful!");
			CString strFilePath = FileDlg.GetPathName();
			CString strPath = strFilePath.Mid(0,strFilePath.ReverseFind('\\'))+_T("\\");
			CString strTitle;
			vector<CString> ids;
			vector<CString> works;
			xtp_vector<xls_constlog> logs;
			long rs,cs;

			CXTPADOConnection con;
			con.SetDataDriver(CXTPADOConnection::adoACEExcel);
			con.SetDataBase(strFilePath);
			con.InitConnectionString();
			if (con.Open())
			{
				LogInfo(L"XTPADOConnection Open Excel Data Successful!");

				CString sql = _T("select * from [sheet1$]");
				CXTPADORecordset rst(con);
				if(rst.Open(sql,CXTPADORecordset::adoRstStoredProc))
				{
					rs = rst.GetRecordCount();
					cs = rst.GetFieldCount();
					if(rs)
					{
						rst.MoveFirst();
						if(!rst.IsEOF()) rst.GetFieldValue(0,strTitle);
	
						rst.MoveNext();
						if(!rst.IsEOF())
						{
							for (int i=0; i<cs; i++)
							{
								CString id;
								rst.GetFieldValue(i,id);
								ids.push_back(id);
							}
						}

						rst.MoveNext();
						if(!rst.IsEOF())
						{
							for (int i=0; i<cs; i++)
							{
								CString work;
								rst.GetFieldValue(i,work);
								works.push_back(work);
							}
						}
					}
					rst.Close();
				}

				sql = _T("select * from [sheet2$]");
				if(rst.Open(sql,CXTPADORecordset::adoRstStoredProc))
				{
					rs = rst.GetRecordCount();
					cs = rst.GetFieldCount();
					if(rs)
					{
						int row = 0;
						int col = 0;

							
						rst.MoveNext();
						while (!rst.IsEOF())
						{
							row++;

							for(col=5; col<cs; col++)
							{
								CString _dt;
								rst.GetFieldValue(col,_dt);
								if(_dt.IsEmpty()) continue;

								COleDateTime dt;
								rst.GetFieldValue(col,dt);

								int id = _ttoi(ids[col]);
								CString posi;
								rst.GetFieldValue(1,posi);
								CString work = works[col];
								CString unit = works[id+1];
								double qty;
								rst.GetFieldValue(id+1,qty);
								logs.push_back(xls_constlog(dt,row,col+1,work,posi,unit,qty,id));
							}

							rst.MoveNext();
						}
						sort(logs.begin(), logs.end(),less<xls_constlog>());//升序
						//sort(logs.begin(), logs.end(),greater<xls_constlog>());//降序
					}
					rst.Close();
				}
				con.Close();
				LogInfo(L"XTPADOConnection Read JetConcrete Data Successful!");
			}

			CStdioFile file;
			if(file.Open(strPath+strTitle+_T(".txt"), CFile::modeCreate | CFile::modeReadWrite))
			{
				setlocale(LC_ALL,"");
				file.Seek(CFile::begin,0);

				xtp_vector<xls_constlog>::iterator iter = logs.begin();
				while(iter != logs.end())
				{
					COleDateTime dt = (*iter).val;
					CString* arr_work = new CString[cs-5];
					CString* arr_posi = new CString[cs-5];
					int* arr_num = new int[cs-5];
					for(int i=0; i<cs-5; i++)
					{
						arr_work[i].Empty();
						arr_posi[i].Empty();
						arr_num[i]=0;
					}

					xtp_vector<xls_constlog>::iterator _iter = iter;
					while((_iter = find(_iter, logs.end(), *iter)) != logs.end())
					{
						xls_constlog _val = *_iter++;
						if (_val.id == -1)
							arr_posi[_val.col-6] = arr_posi[_val.col-6]+_val.posi+_T("、");
						else
							arr_posi[_val.col-6] = arr_posi[_val.col-6]+_val.posi+_T(" ")+ToString(_val.qty,2)+_val.unit+_T("、");

						arr_work[_val.col-6] = _val.work;
						arr_num[_val.col-6]++;
					}

					CString str;
					str.Format(_T("施工日期：%s     晴      16-30度  南风3-4级\n"),dt.Format(_T("%Y年%m月%d日")));
					file.WriteString(str);
					file.WriteString(_T("----------------------当日施工进度---------------------\n"));

					int n=1;
					for(int i=0; i<cs-5; i++)
					{
						if(arr_posi[i].GetLength())
						{
							arr_posi[i] = arr_posi[i].Mid(0,arr_posi[i].GetLength()-1);
							str.Format(_T("(%d) %s共%d个，编号为：%s\n"),n++,arr_work[i],arr_num[i],arr_posi[i]);
							file.WriteString(str);
							str.Empty();
						}
					}

					file.WriteString(_T("=======================================================\n"));

					while(++iter != logs.end()) if((*iter).val != dt) break;
				}

				file.Close();
				msgInf(strTitle+_T(" 文件生成成功!"));
			}
		}
	}
	catch(CException *e)
	{
		LogErrorExp(L"XTPADOConnection Read JetConcrete Data Faild!", e);
	}
}
void GroutLog_xls()
{
	try
	{
		CXTPExcelUtil excel;
		excel.InitExcel();

		TCHAR szFilter[] = _T("Excel 2007|*.xlsx|Excel 2003|*.xls|CSV文件|*.csv|所有文件|*.*||");
		CFileDialog FileDlg(TRUE, _T("xlsx"), NULL, OFN_ALLOWMULTISELECT, szFilter, NULL);

		if(IDOK == FileDlg.DoModal())
		{
			CString strFilePath = FileDlg.GetPathName();
			CString strPath = strFilePath.Mid(0,strFilePath.ReverseFind('\\'))+_T("\\");

			excel.OpenExcel(strFilePath);
			int nst = excel.GetSheetCount();

			for(int k=0; k<nst; k++)
			{
				xtp_vector<xtp_vector<VARIANT>> vecData;
				excel.LoadSheet(k+1);
				long rows = excel.GetRowCount();
				for(long i=0; i<rows; i++)
				{
					xtp_vector<VARIANT> data;

					for(long j=0; j<8; j++)
					{
						VARIANT val = excel.GetCell(i+1,j+1);
						data.push_back(val);
					}

					vecData.push_back(data);
				}

				CStdioFile file;
				if(file.Open(strPath+vecData[0][0].bstrVal+_T(".txt"), CFile::modeCreate | CFile::modeReadWrite))
				{
					setlocale(LC_ALL,"");
					file.Seek(CFile::begin,0);

					CString str,str1;
					str.Format(_T("%s\n"),vecData[0][0].bstrVal);
					file.WriteString(str);
					file.WriteString(_T("=======================================================\n"));
					int n=1;
					for(int i=2; i<vecData.size(); i=i+2)
					{
						if(i+1 > vecData.size()-1) break;
						str.Format(_T("时间：%s          页码：%d\n"),vecData[i][3].bstrVal, n++);
						file.WriteString(str);
						file.WriteString(_T("-----------------------当日施工情况--------------------\n"));
						if(CString(vecData[i+1][2].bstrVal).IsEmpty())
							str.Format(_T("  %s %s 处钻孔、\n"),vecData[0][0].bstrVal,vecData[i][2].bstrVal);
						else
							str.Format(_T("  %s %s 处、%s 处钻孔、\n"),vecData[0][0].bstrVal,vecData[i][2].bstrVal,vecData[i+1][2].bstrVal);

						file.WriteString(str);
						file.WriteString(_T("  安装注浆管、拱墙防寒径向注浆。\n"));
						file.WriteString(_T("---------------------当日主要监理工作------------------\n"));
						file.WriteString(_T("一、人员配置及机械使用情况：\n"));
						str.Format(_T("  1、人员配置：施工负责人 %s、技术员 %s\n"),vecData[i][6].bstrVal,vecData[i][7].bstrVal);
						file.WriteString(str);
						file.WriteString(_T("  2、机械设备：风枪2台，水泥搅拌机2台，注浆机2台。\n"));
						file.WriteString(_T("二、主要验收记录：\n"));
						str.Format(_T("  1、经测量放样布孔，符合要求。围岩等级 %s级，\n"),vecData[i][1].bstrVal);
						file.WriteString(str);


						if(CString(vecData[i+1][2].bstrVal).IsEmpty())
						{
							int numb = _ttoi(vecData[i][4].bstrVal);
							str.Format(_T("  注浆环向间距2m，梅花型布置，钻孔 %d 个孔，孔深3m、\n"),numb);
						}
						else
						{
							int numb = _ttoi(vecData[i][4].bstrVal)+_ttoi(vecData[i+1][4].bstrVal);
							str.Format(_T("  注浆环向间距2m，梅花型布置，两处共钻孔 %d 个孔，孔深3m、\n"),numb);
						}
						file.WriteString(str);
						file.WriteString(_T("  孔径52mm、管长1m、管径50mm。验收合格，同意转序。\n"));
						if(CString(vecData[i+1][2].bstrVal).IsEmpty())
							str.Format(_T("  2、注浆量为 %s m3。注浆采用1:1水泥浆液，\n"),vecData[i][5].bstrVal);
						else
							str.Format(_T("  2、注浆量分别为 %s m3，%s m3。注浆采用1:1水泥浆液，\n"),vecData[i][5].bstrVal,vecData[i+1][5].bstrVal);

						file.WriteString(str);
						file.WriteString(_T("  符合设计要求。详细情况见旁站记录表。\n"));
						file.WriteString(_T("=======================================================\n"));
					}

					file.Close();
					msgInf(CString(vecData[0][0].bstrVal)+_T(" TXT文件生成成功!"));
				}
			}
		}

		excel.CloseExcel();
		excel.ReleaseExcel();

		LogInfo(L"XTPExcelUtil Read GroutLog Data Successful!");
	}
	catch(CException *e)
	{
		LogErrorExp(L"XTPExcelUtil Read GroutLog Data Faild!", e);
	}
}
void GroutLedger_xls()
{
	try
	{
		CXTPExcelUtil excel;
		excel.InitExcel();

		TCHAR szFilter[] = _T("Excel 2007|*.xlsx|Excel 2003|*.xls|CSV文件|*.csv|所有文件|*.*||");
		CFileDialog FileDlg(TRUE, _T("xlsx"), NULL, OFN_ALLOWMULTISELECT, szFilter, NULL);

		if(IDOK == FileDlg.DoModal())
		{
			CString strFilePath = FileDlg.GetPathName();
			CString strPath = strFilePath.Mid(0,strFilePath.ReverseFind('\\'))+_T("\\");

			excel.OpenExcel(strFilePath);
			int nst = excel.GetSheetCount();

			for(int k=0; k<nst; k++)
			{
				xtp_vector<xtp_vector<VARIANT>> vecData;
				excel.LoadSheet(k+1);
				long rows = excel.GetRowCount();
				for(long i=0; i<rows; i++)
				{
					xtp_vector<VARIANT> data;

					for(long j=0; j<8; j++)
					{
						VARIANT val = excel.GetCell(i+1,j+1);
						data.push_back(val);
					}

					vecData.push_back(data);
				}

				CStdioFile file;
				if(file.Open(strPath+vecData[0][0].bstrVal+_T(".txt"), CFile::modeCreate | CFile::modeReadWrite))
				{
					setlocale(LC_ALL,"");
					file.Seek(CFile::begin,0);

					CString str,str1;
					str.Format(_T("%s\n"),vecData[0][0].bstrVal);
					file.WriteString(str);
					file.WriteString(_T("=======================================================\n"));
					int n=1;
					for(int i=2; i<vecData.size(); i=i+2)
					{
						if(i+1 > vecData.size()-1) break;
						str.Format(_T("--------------时间：%s    页码：%d--------------\n"),vecData[i][3].bstrVal, n++);
						file.WriteString(str);
						file.WriteString(_T("四、拱墙防寒径向注浆\n"));
						if(CString(vecData[i+1][2].bstrVal).IsEmpty())
						{
							int numb = _ttoi(vecData[i][4].bstrVal);
							str.Format(_T("%s 处，围岩 %s 级，钻孔 %d 个，孔深3m、孔径52mm；注浆管长1m、管径50mm；注浆量 %s m3。浆液1;1。\n"),vecData[i][2].bstrVal,vecData[i][1].bstrVal,numb, vecData[i][5].bstrVal);
						}
						else
						{
							int numb = _ttoi(vecData[i][4].bstrVal)+_ttoi(vecData[i+1][4].bstrVal);
							str.Format(_T("%s、%s 处，围岩 %s 级，共钻孔 %d 个，孔深3m、孔径52mm；注浆管长1m、管径50mm；注浆量分别为 %s m3，%s m3。浆液1;1。\n"),vecData[i][2].bstrVal,vecData[i+1][2].bstrVal,vecData[i][1].bstrVal,numb, vecData[i][5].bstrVal,vecData[i+1][5].bstrVal);
						}

						file.WriteString(str);
						file.WriteString(_T("=======================================================\n"));
					}

					file.Close();
					msgInf(CString(vecData[0][0].bstrVal)+_T(" TXT文件生成成功!"));
				}
			}

		}

		excel.CloseExcel();
		excel.ReleaseExcel();

		LogInfo(L"XTPExcelUtil Read GroutLedger Data Successful!");
	}
	catch(CException *e)
	{
		LogErrorExp(L"XTPExcelUtil Read GroutLedger Data Faild!", e);
	}
}
void JetConcrete_sql()
{
	try
	{
		TCHAR szFilter[] = _T("Excel2007文件|*.xlsx|Excel2003文件|*.xls|所有文件|*.*||");
		CFileDialog FileDlg(TRUE, _T("xlsx"), NULL, 0, szFilter, NULL);
		if(IDOK == FileDlg.DoModal())
		{
			LogInfo(L"FileDialog Open Excel File Successful!");
			CString strFilePath = FileDlg.GetPathName();
			CString strPath = strFilePath.Mid(0,strFilePath.ReverseFind('\\'))+_T("\\");

			CXTPADOConnection con;
			con.SetDataDriver(CXTPADOConnection::adoACEExcel);
			con.SetDataBase(strFilePath);
			con.InitConnectionString();
			if (con.Open())
			{
				LogInfo(L"XTPADOConnection Open Excel Data Successful!");

				CString sql = _T("select * from [sheet1$]");

				CXTPADORecordset rst(con);
				if(rst.Open(sql,CXTPADORecordset::adoRstStoredProc))
				{
					CStdioFile file;
					if(file.Open(strPath+_T("喷锚支护.txt"), CFile::modeCreate | CFile::modeReadWrite))
					{
						setlocale(LC_ALL,"");
						file.Seek(CFile::begin,0);

						while (!rst.IsEOF())
						{
							CString str;
							COleDateTime dt,dt1,dt2,dt3,dt4;
							double page,stat;
							CString str1,str2,str3;
							rst.GetFieldValue(0,page);
							rst.GetFieldValue(1,stat);
							rst.GetFieldValue(2,str1);
							rst.GetFieldValue(3,str2);
							rst.GetFieldValue(4,str3);
							str.Format(_T("-----------------------隧道喷锚支护施工记录表  %s  第 %.4d 页-----------------------\n"),str1,(int)page);
							file.WriteString(str);
							str.Format(_T("施工里程: %s %s   围岩级别: %s   衬砌类型: %s\n"),FormatStat(stat,7),str1,str2,str3);
							file.WriteString(str);
							rst.GetFieldValue(20,dt);
							str.Format(_T("喷锚部位开挖时间: %s\n"),dt.Format(_T("%Y-%m-%d %H:%m")));
							file.WriteString(str);
							rst.GetFieldValue(21,dt1);
							rst.GetFieldValue(22,dt2);
							str.Format(_T("喷射混凝土施做开始时间: %s,  终止时间: %s\n"),dt1.Format(_T("%Y-%m-%d %H:%m")),dt2.Format(_T("%Y-%m-%d %H:%m")));
							file.WriteString(str);
							rst.GetFieldValue(23,dt3);
							rst.GetFieldValue(24,dt4);
							str.Format(_T("锚杆施做开始时间: %s,  终止时间: %s\n"),dt3.Format(_T("%Y-%m-%d %H:%m")),dt4.Format(_T("%Y-%m-%d %H:%m")));
							file.WriteString(str);
							rst.GetFieldValue(25,str1);
							rst.GetFieldValue(26,str2);
							rst.GetFieldValue(27,str3);
							str.Format(_T("喷层厚度: %s, 面积: %s, 水泥 %s\n"),str1,str2,str3);
							file.WriteString(str);
							rst.GetFieldValue(28,str1);
							rst.GetFieldValue(29,str2);
							rst.GetFieldValue(30,str3);
							str.Format(_T("锚杆布置: %s, 数量: %s, 水泥: %s\n"),str1,str2,str3);
							file.WriteString(str);

							rst.MoveNext();
							file.WriteString(_T("==========================================================\n"));
						}

						file.Close();
						msgInf(_T("喷锚支护 TXT文件生成成功!"));
					}

					rst.Close();
				}

				con.Close();
			}
		}

		LogInfo(L"XTPADOConnection Read JetConcrete Data Successful!");
	}
	catch(CException *e)
	{
		LogErrorExp(L"XTPADOConnection Read JetConcrete Data Faild!", e);
	}
}
void InitialSupport_sql()
{
	try
	{
		TCHAR szFilter[] = _T("Excel2007文件|*.xlsx|Excel2003文件|*.xls|所有文件|*.*||");
		CFileDialog FileDlg(TRUE, _T("xlsx"), NULL, 0, szFilter, NULL);
		if(IDOK == FileDlg.DoModal())
		{
			LogInfo(L"FileDialog Open Excel File Successful!");
			CString strFilePath = FileDlg.GetPathName();
			CString strPath = strFilePath.Mid(0,strFilePath.ReverseFind('\\'))+_T("\\");

			CXTPADOConnection con;
			con.SetDataDriver(CXTPADOConnection::adoACEExcel);
			con.SetDataBase(strFilePath);
			con.InitConnectionString();
			if (con.Open())
			{
				LogInfo(L"XTPADOConnection Open Excel Data Successful!");

				CString sql = _T("select * from [sheet1$]");

				CXTPADORecordset rst(con);
				if(rst.Open(sql,CXTPADORecordset::adoRstStoredProc))
				{
					CStdioFile file;
					if(file.Open(strPath+_T("初期支护.txt"), CFile::modeCreate | CFile::modeReadWrite))
					{
						setlocale(LC_ALL,"");
						file.Seek(CFile::begin,0);

						while (!rst.IsEOF())
						{
							CString str;
							COleDateTime date;
							double page,stat;
							CString str1,str2,str3,str4,str5;
							rst.GetFieldValue(0,page);
							rst.GetFieldValue(1,date);
							rst.GetFieldValue(2,stat);
							rst.GetFieldValue(3,str1);
							rst.GetFieldValue(4,str2);
							rst.GetFieldValue(5,str3);
							str.Format(_T("-----------------------隧道初期支护工程验收记录表  %s  第 %.4d 页-----------------------\n"),str1,(int)page);
							file.WriteString(str);
							str.Format(_T("施工里程: %s %s   围岩级别: %s   衬砌类型: %s;"),FormatStat(stat,7),str1,str2,str3);
							file.WriteString(str);
							rst.GetFieldValue(6,str1);
							rst.GetFieldValue(7,str2);
							str.Format(_T("  钢架规格: %s,  单元号: %s\n"),str1,str2);
							file.WriteString(str);
							rst.GetFieldValue(8,str1);
							rst.GetFieldValue(9,str2);
							rst.GetFieldValue(10,str3);
							rst.GetFieldValue(11,str4);
							str.Format(_T("钢架安装偏差 间距: %s, %s, %s, %s;"),str1,str2,str3,str4);
							file.WriteString(str);
							rst.GetFieldValue(12,str1);
							rst.GetFieldValue(13,str2);
							rst.GetFieldValue(14,str3);
							rst.GetFieldValue(15,str4);
							str.Format(_T("  横向: %s, %s, %s, %s;"),str1,str2,str3,str4);
							file.WriteString(str);
							rst.GetFieldValue(16,str1);
							rst.GetFieldValue(17,str2);
							rst.GetFieldValue(18,str3);
							rst.GetFieldValue(19,str4);
							str.Format(_T("  高程: %s, %s, %s, %s;"),str1,str2,str3,str4);
							file.WriteString(str);
							rst.GetFieldValue(20,str1);
							rst.GetFieldValue(21,str2);
							rst.GetFieldValue(22,str3);
							rst.GetFieldValue(23,str4);
							str.Format(_T("  垂直度: %s, %s, %s, %s\n"),str1,str2,str3,str4);
							file.WriteString(str);
							rst.GetFieldValue(24,str1);
							rst.GetFieldValue(25,str2);
							str.Format(_T("砂浆锚杆 数量(根): %s  长度(m): %s;"),str1,str2);
							file.WriteString(str);
							rst.GetFieldValue(26,str1);
							rst.GetFieldValue(27,str2);
							rst.GetFieldValue(28,str3);
							rst.GetFieldValue(29,str4);
							str.Format(_T("  安装偏差 间距: %s, %s, %s, %s;"),str1,str2,str3,str4);
							file.WriteString(str);
							rst.GetFieldValue(30,str1);
							rst.GetFieldValue(31,str2);
							rst.GetFieldValue(32,str3);
							rst.GetFieldValue(33,str4);
							str.Format(_T("  嵌入深度: %s, %s, %s, %s\n"),str1,str2,str3,str4);
							file.WriteString(str);
							rst.GetFieldValue(34,str1);
							rst.GetFieldValue(35,str2);
							str.Format(_T("中空锚杆 数量(根): %s  长度(m): %s;"),str1,str2);
							file.WriteString(str);
							rst.GetFieldValue(36,str1);
							rst.GetFieldValue(37,str2);
							rst.GetFieldValue(38,str3);
							rst.GetFieldValue(39,str4);
							str.Format(_T("  安装偏差 间距: %s, %s, %s, %s;"),str1,str2,str3,str4);
							file.WriteString(str);
							rst.GetFieldValue(40,str1);
							rst.GetFieldValue(41,str2);
							rst.GetFieldValue(42,str3);
							rst.GetFieldValue(43,str4);
							str.Format(_T("  嵌入深度: %s, %s, %s, %s\n"),str1,str2,str3,str4);
							file.WriteString(str);
							rst.GetFieldValue(44,str1);
							rst.GetFieldValue(45,str2);
							str.Format(_T("锁脚锚管 数量(根): %s  长度(m): %s;"),str1,str2);
							file.WriteString(str);
							rst.GetFieldValue(46,str1);
							rst.GetFieldValue(47,str2);
							rst.GetFieldValue(48,str3);
							rst.GetFieldValue(49,str4);
							str.Format(_T("  安装偏差 间距: %s, %s, %s, %s;"),str1,str2,str3,str4);
							file.WriteString(str);
							rst.GetFieldValue(50,str1);
							rst.GetFieldValue(51,str2);
							rst.GetFieldValue(52,str3);
							rst.GetFieldValue(53,str4);
							str.Format(_T("  嵌入深度: %s, %s, %s, %s\n"),str1,str2,str3,str4);
							file.WriteString(str);
							rst.GetFieldValue(54,str1);
							rst.GetFieldValue(55,str2);
							rst.GetFieldValue(56,str3);
							rst.GetFieldValue(57,str4);
							rst.GetFieldValue(58,str5);
							str.Format(_T("钢筋网安装偏差 网格尺寸: %s, %s, %s, %s, %s;"),str1,str2,str3,str4,str5);
							file.WriteString(str);
							rst.GetFieldValue(59,str1);
							rst.GetFieldValue(60,str2);
							rst.GetFieldValue(61,str3);
							rst.GetFieldValue(62,str4);
							rst.GetFieldValue(63,str5);
							str.Format(_T("  搭接长度: %s, %s, %s, %s, %s\n"),str1,str2,str3,str4,str5);
							file.WriteString(str);

							rst.MoveNext();
							file.WriteString(_T("==========================================================\n"));
						}

						file.Close();
						msgInf(_T("初期支护 TXT文件生成成功!"));
					}

					rst.Close();
				}

				con.Close();
			}
		}

		LogInfo(L"XTPADOConnection Read InitialSupport Data Successful!");
	}
	catch(CException *e)
	{
		LogErrorExp(L"XTPADOConnection Read InitialSupport Data Faild!", e);
	}
}
void LockAnchor_sql()
{
	try
	{
		TCHAR szFilter[] = _T("Excel2007文件|*.xlsx|Excel2003文件|*.xls|所有文件|*.*||");
		CFileDialog FileDlg(TRUE, _T("xlsx"), NULL, 0, szFilter, NULL);
		if(IDOK == FileDlg.DoModal())
		{
			LogInfo(L"FileDialog Open Excel File Successful!");
			CString strFilePath = FileDlg.GetPathName();
			CString strPath = strFilePath.Mid(0,strFilePath.ReverseFind('\\'))+_T("\\");

			CXTPADOConnection con;
			con.SetDataDriver(CXTPADOConnection::adoACEExcel);
			con.SetDataBase(strFilePath);
			con.InitConnectionString();
			if (con.Open())
			{
				LogInfo(L"XTPADOConnection Open Excel Data Successful!");

				CString sql = _T("select * from [sheet1$]");

				CXTPADORecordset rst(con);
				if(rst.Open(sql,CXTPADORecordset::adoRstStoredProc))
				{
					CStdioFile file;
					if(file.Open(strPath+_T("锁脚注浆.txt"), CFile::modeCreate | CFile::modeReadWrite))
					{
						setlocale(LC_ALL,"");
						file.Seek(CFile::begin,0);

						while (!rst.IsEOF())
						{
							CString str;
							double page,stat;
							COleDateTime date;
							CString str1,str2,str3,str4;
							rst.GetFieldValue(0,page);
							rst.GetFieldValue(1,stat);
							rst.GetFieldValue(2,str1);
							rst.GetFieldValue(3,str2);
							rst.GetFieldValue(4,str3);
							rst.GetFieldValue(7,date);
							str.Format(_T("-----------------------锁脚锚管注浆记录表  %s  第 %.4d 页-----------------------\n"),str1,(int)page);
							file.WriteString(str);
							str.Format(_T("施工里程: %s %s   围岩级别: %s   衬砌类型: %s   施工时间: %s\n"),FormatStat(stat,7),str1,str2,str3,date.Format(_T("%Y-%m-%d %H:%m")));
							file.WriteString(str);
							rst.GetFieldValue(8,str1);
							rst.GetFieldValue(9,str2);
							rst.GetFieldValue(10,str3);
							rst.GetFieldValue(11,str4);
							str.Format(_T("1号管注浆开始时间: %s,  终止时间: %s,  注浆压力: %s,  注浆量: %s\n"),str1,str2,str3,str4);
							file.WriteString(str);
							rst.GetFieldValue(12,str1);
							rst.GetFieldValue(13,str2);
							rst.GetFieldValue(14,str3);
							rst.GetFieldValue(15,str4);
							str.Format(_T("2号管注浆开始时间: %s,  终止时间: %s,  注浆压力: %s,  注浆量: %s\n"),str1,str2,str3,str4);
							file.WriteString(str);
							rst.GetFieldValue(16,str1);
							rst.GetFieldValue(17,str2);
							rst.GetFieldValue(18,str3);
							rst.GetFieldValue(19,str4);
							str.Format(_T("3号管注浆开始时间: %s,  终止时间: %s,  注浆压力: %s,  注浆量: %s\n"),str1,str2,str3,str4);
							file.WriteString(str);
							rst.GetFieldValue(20,str1);
							rst.GetFieldValue(21,str2);
							rst.GetFieldValue(22,str3);
							rst.GetFieldValue(23,str4);
							str.Format(_T("4号管注浆开始时间: %s,  终止时间: %s,  注浆压力: %s,  注浆量: %s\n"),str1,str2,str3,str4);
							file.WriteString(str);
							rst.GetFieldValue(24,str1);
							rst.GetFieldValue(25,str2);
							rst.GetFieldValue(26,str3);
							rst.GetFieldValue(27,str4);
							str.Format(_T("5号管注浆开始时间: %s,  终止时间: %s,  注浆压力: %s,  注浆量: %s\n"),str1,str2,str3,str4);
							file.WriteString(str);
							rst.GetFieldValue(28,str1);
							rst.GetFieldValue(29,str2);
							rst.GetFieldValue(30,str3);
							rst.GetFieldValue(31,str4);
							str.Format(_T("6号管注浆开始时间: %s,  终止时间: %s,  注浆压力: %s,  注浆量: %s\n"),str1,str2,str3,str4);
							file.WriteString(str);
							rst.GetFieldValue(32,str1);
							rst.GetFieldValue(33,str2);
							rst.GetFieldValue(34,str3);
							rst.GetFieldValue(35,str4);
							str.Format(_T("7号管注浆开始时间: %s,  终止时间: %s,  注浆压力: %s,  注浆量: %s\n"),str1,str2,str3,str4);
							file.WriteString(str);
							rst.GetFieldValue(36,str1);
							rst.GetFieldValue(37,str2);
							rst.GetFieldValue(38,str3);
							rst.GetFieldValue(39,str4);
							str.Format(_T("8号管注浆开始时间: %s,  终止时间: %s,  注浆压力: %s,  注浆量: %s\n"),str1,str2,str3,str4);
							file.WriteString(str);

							rst.MoveNext();
							file.WriteString(_T("==========================================================\n"));
						}

						file.Close();
						msgInf(_T("锁脚注浆 TXT文件生成成功!"));
					}

					rst.Close();
				}

				con.Close();
			}
		}

		LogInfo(L"XTPADOConnection Read LockAnchor Data Successful!");
	}
	catch(CException *e)
	{
		LogErrorExp(L"XTPADOConnection Read LockAnchor Data Faild!", e);
	}
}
void AdvanceGrout_sql()
{
	try
	{
		TCHAR szFilter[] = _T("Excel2007文件|*.xlsx|Excel2003文件|*.xls|所有文件|*.*||");
		CFileDialog FileDlg(TRUE, _T("xlsx"), NULL, 0, szFilter, NULL);
		if(IDOK == FileDlg.DoModal())
		{
			LogInfo(L"FileDialog Open Excel File Successful!");
			CString strFilePath = FileDlg.GetPathName();
			CString strPath = strFilePath.Mid(0,strFilePath.ReverseFind('\\'))+_T("\\");

			CXTPADOConnection con;
			con.SetDataDriver(CXTPADOConnection::adoACEExcel);
			con.SetDataBase(strFilePath);
			con.InitConnectionString();
			if (con.Open())
			{
				LogInfo(L"XTPADOConnection Open Excel Data Successful!");

				CString sql = _T("select * from [sheet1$]");

				CXTPADORecordset rst(con);
				if(rst.Open(sql,CXTPADORecordset::adoRstStoredProc))
				{
					CStdioFile file;
					if(file.Open(strPath+_T("小导管注浆.txt"), CFile::modeCreate | CFile::modeReadWrite))
					{
						setlocale(LC_ALL,"");
						file.Seek(CFile::begin,0);

						while (!rst.IsEOF())
						{
							CString str;
							double page,stat;
							COleDateTime date;
							CString part,level,type,str1,str2,str3,str4;
							rst.GetFieldValue(0,page);
							rst.GetFieldValue(1,stat);
							rst.GetFieldValue(2,part);
							rst.GetFieldValue(3,level);
							rst.GetFieldValue(4,type);
							rst.GetFieldValue(6,date);
							str.Format(_T("-----------------------超前小导管注浆记录表  %s  第 %.4d 页-----------------------\n"),part,(int)page);
							file.WriteString(str);
							str.Format(_T("施工里程: %s %s   围岩级别: %s   衬砌类型: %s   施工时间: %s\n"),FormatStat(stat,7),part,level,type,date.Format(_T("%Y-%m-%d %H:%m")));
							file.WriteString(str);
							for (int i=0; i<27; i++)
							{
								rst.GetFieldValue(9+i*4,str1);
								rst.GetFieldValue(10+i*4,str2);
								rst.GetFieldValue(11+i*4,str3);
								rst.GetFieldValue(12+i*4,str4);
								str.Format(_T("%d号管注浆开始时间: %s,  终止时间: %s,  注浆压力: %s,  注浆量: %s\n"),i*2+1,str1,str2,str3,str4);
								file.WriteString(str);
							}
							file.WriteString(_T("==========================================================\n"));
							str.Format(_T("-----------------------超前小导管注浆记录表  %s  第 %.4d 页-----------------------\n"),part,(int)page+1);
							file.WriteString(str);
							str.Format(_T("施工里程: %s %s   围岩级别: %s   衬砌类型: %s   施工时间: %s\n"),FormatStat(stat,7),part,level,type,date.Format(_T("%Y-%m-%d %H:%m")));
							file.WriteString(str);
							for (int i=0; i<27; i++)
							{
								rst.GetFieldValue(9+27*4+i*4,str1);
								rst.GetFieldValue(10+27*4+i*4,str2);
								rst.GetFieldValue(11+27*4+i*4,str3);
								rst.GetFieldValue(12+27*4+i*4,str4);
								str.Format(_T("%d号管注浆开始时间: %s,  终止时间: %s,  注浆压力: %s,  注浆量: %s\n"),54-i*2,str1,str2,str3,str4);
								file.WriteString(str);
							}

							rst.MoveNext();
							file.WriteString(_T("==========================================================\n"));
						}

						file.Close();
						msgInf(_T("小导管注浆 TXT文件生成成功!"));
					}

					rst.Close();
				}

				con.Close();
			}
		}

		LogInfo(L"XTPADOConnection Read AdvanceGrout Data Successful!");
	}
	catch(CException *e)
	{
		LogErrorExp(L"XTPADOConnection Read AdvanceGrout Data Faild!", e);
	}
}

void CMainFrame::OnBnPointsManager()
{
	if(!m_pPopupPoints) return;
	m_pPopupPoints->SetCaption(ID_POINTSMGR_TABLE);
	m_pPopupPoints->SetIconId(ID_POINTSMGR_TABLE);

	CDlgMethod dlg;
	if(dlg.DoModal() == IDOK)
	{
		switch(dlg.m_nIDCheck)
		{
		case IDC_METHOD_RO_XLSCSV:
			XlsToCsv_xls();
			break;
		case IDC_METHOD_RO_WORDPIC:
			WordPic_doc();
			break;
		case IDC_METHOD_RO_WORDGRID:
			WordGrid_doc();
			break;
		case IDC_METHOD_RO_CONSTLOG:
			ConstLog_sql();
			break;
		case IDC_METHOD_RO_GROUTLOG:
			GroutLog_xls();
			break;
		case IDC_METHOD_RO_GROUTLEDGER:
			GroutLedger_xls();
			break;
		case IDC_METHOD_RO_JETCONCRETE:
			JetConcrete_sql();
			break;
		case IDC_METHOD_RO_INITIALSUPPORT:
			InitialSupport_sql();
			break;
		case IDC_METHOD_RO_LOCKANCHOR:
			LockAnchor_sql();
			break;
		case IDC_METHOD_RO_ADVANCEGROUT:
			AdvanceGrout_sql();
			break;
		}
	}
}

void CMainFrame::OnBnCostManager()
{
	if(!m_pPopupCost) return;
	m_pPopupCost->SetCaption(ID_COSTMGR_TABLE);
	m_pPopupCost->SetIconId(ID_COSTMGR_TABLE);

	if(m_pRibbonReportView)
	{
		CString str;
		if(!m_pPopupPoints) return;
		CString* tag = (CString*)(DWORD_PTR)m_pPopupPoints->GetTag();
		if(!tag) return;
		str = _T("tv") + *tag;
		CString lst=str + _T("lst");

		if(!m_pPopupCost) return;
		tag = (CString*)(DWORD_PTR)m_pPopupCost->GetTag();
		if(!tag) return;
		str += *tag;
		m_pRibbonReportView->SetView(str);
		int idx = m_pSearchDatapart->GetCurSel();
		if(idx > -1)
		{
			CXTPControlGalleryItem* pCalleryItem = m_pItemsSearchdatapart->GetItem(idx);
			CString* tag = (CString*)(DWORD_PTR)pCalleryItem->GetData();
			CString sql;
			sql.Format(_T("工点编号='%s'"),*tag);
			if(m_idCostmgr==ID_TABLE_LIST)
				m_pRibbonReportView->DataBindRecordset();
			else
				m_pRibbonReportView->DataBindRecordset(GetRecordset(str,sql));
		}

		switch (m_idCostmgr)
		{
		case ID_TABLE_QTY:
			{
				m_pRibbonReportView->SetRecordItemControl(1, XTP_REPORT_ITEM_CTRL_LINK, _T("tv_points"),TRUE,FALSE);
				m_pRibbonReportView->SetRecordItemControl(2, XTP_REPORT_ITEM_CTRL_LINK, lst,TRUE,FALSE);
				int col[] = {3,4,5};
				xtp_vector<int> cols(col);
				m_pRibbonReportView->SetRecordItemEditable(cols);
			}
			break;
		case ID_TABLE_UP:
			{
				m_pRibbonReportView->SetRecordItemControl(1, XTP_REPORT_ITEM_CTRL_LINK, _T("tv_points"),TRUE,FALSE);
				m_pRibbonReportView->SetRecordItemControl(2, XTP_REPORT_ITEM_CTRL_LINK, lst,TRUE,FALSE);
				m_pRibbonReportView->SetRecordItemControl(3, XTP_REPORT_ITEM_CTRL_LINK, _T("tv_ledger"),TRUE,FALSE);
				int col[] = {4,5,6,7,8,13,14};
				xtp_vector<int> cols(col);
				m_pRibbonReportView->SetRecordItemEditable(cols);
			}
			break;
		case ID_TABLE_DOWN:
			{
				m_pRibbonReportView->SetRecordItemControl(1, XTP_REPORT_ITEM_CTRL_LINK, _T("tv_points"),TRUE,FALSE);
				m_pRibbonReportView->SetRecordItemControl(2, XTP_REPORT_ITEM_CTRL_LINK, lst,TRUE,FALSE);
				m_pRibbonReportView->SetRecordItemControl(3, XTP_REPORT_ITEM_CTRL_LINK, _T("tv_ledger"),TRUE,FALSE);
				m_pRibbonReportView->SetRecordItemControl(4, XTP_REPORT_ITEM_CTRL_LINK, _T("tv_units"),TRUE,FALSE);
				int col[] = {5,6,7,8,9,10,15,16};
				xtp_vector<int> cols(col);
				m_pRibbonReportView->SetRecordItemEditable(cols);
			}
			break;
		}
	}
}

void CMainFrame::OnBnMaterManager()
{
	if(!m_pPopupMater) return;
	m_pPopupMater->SetCaption(ID_MATERMGR_TABLE);
	m_pPopupMater->SetIconId(ID_MATERMGR_TABLE);

	CString str;
	if(!m_pPopupMater) return;
	CString* tag = (CString*)(DWORD_PTR)m_pPopupMater->GetTag();
	if(!tag) return;
	str = _T("tv")+*tag;
	m_pRibbonReportView->SetView(str);
	m_pRibbonReportView->DataBindRecordset();

	switch (m_idMatermgr)
	{
	case ID_TABLE_CHECK:
		{
			m_pRibbonReportView->SetRecordItemControl(2, XTP_REPORT_ITEM_CTRL_LINK, _T("tv_barn"),TRUE,FALSE);
			m_pRibbonReportView->SetRecordItemControl(3, XTP_REPORT_ITEM_CTRL_LINK, _T("tv_goods"),TRUE,FALSE);
			m_pRibbonReportView->SetRecordItemControl(4, XTP_REPORT_ITEM_CTRL_LINK, _T("tv_employee"),TRUE,FALSE);
			int col[] = {5,6,7,8,10,12};
			xtp_vector<int> cols(col);
			m_pRibbonReportView->SetRecordItemEditable(cols);
		}
		break;
	case ID_TABLE_INSTOCK:
	case ID_TABLE_OUTSTOCK:
	case ID_TABLE_MIXSTOCK:
	case ID_TABLE_RETSTOCK:
		{
			m_pRibbonReportView->SetRecordItemControl(3, XTP_REPORT_ITEM_CTRL_LINK, _T("tv_barn"),TRUE,FALSE);
			m_pRibbonReportView->SetRecordItemControl(4, XTP_REPORT_ITEM_CTRL_LINK, _T("tv_goods"),TRUE,FALSE);
			m_pRibbonReportView->SetRecordItemControl(5, XTP_REPORT_ITEM_CTRL_LINK, _T("tv_units"),TRUE,FALSE);
			m_pRibbonReportView->SetRecordItemControl(6, XTP_REPORT_ITEM_CTRL_LINK, _T("tv_employee"),TRUE,FALSE);
			m_pRibbonReportView->SetRecordItemControl(7, XTP_REPORT_ITEM_CTRL_LINK, _T("tv_pact"),TRUE,FALSE);
			int col[] = {8,9,10,11,12,14,15,19,20};
			xtp_vector<int> cols(col);
			m_pRibbonReportView->SetRecordItemEditable(cols);
		}
		break;
	}
}

void CMainFrame::OnBnFinanceManager()
{
	if(!m_pPopupFinance) return;
	m_pPopupFinance->SetCaption(ID_FINANCEMGR_TABLE);
	m_pPopupFinance->SetIconId(ID_FINANCEMGR_TABLE);

	CString str;
	if(!m_pPopupFinance) return;
	CString* tag = (CString*)(DWORD_PTR)m_pPopupFinance->GetTag();
	if(!tag) return;
	str = _T("tv")+*tag;
	m_pRibbonReportView->SetView(str);
	m_pRibbonReportView->DataBindRecordset();

	switch (m_idFinancemgr)
	{
	case ID_TABLE_MEMBER:
		{
			CString* tag = new CString;
			*tag = _T("男;女");
			m_pRibbonReportView->SetRecordItemControl(2, XTP_REPORT_ITEM_CTRL_COMBOBOX,EMPTY,TRUE,TRUE,(DWORD_PTR)tag);
			*tag = _T("党员;团员;农民;学生");
			m_pRibbonReportView->SetRecordItemControl(4, XTP_REPORT_ITEM_CTRL_COMBOBOX,EMPTY,TRUE,TRUE,(DWORD_PTR)tag);
			*tag = _T("爸爸;妈妈;女儿;儿子;爷爷;奶奶");
			m_pRibbonReportView->SetRecordItemControl(5, XTP_REPORT_ITEM_CTRL_COMBOBOX,EMPTY,TRUE,TRUE,(DWORD_PTR)tag);
			*tag = _T("硕士;本科;高中;初中;小学;文盲");
			m_pRibbonReportView->SetRecordItemControl(7, XTP_REPORT_ITEM_CTRL_COMBOBOX,EMPTY,TRUE,TRUE,(DWORD_PTR)tag);
		}
		break;
	case ID_TABLE_FUNDS:
		{
			CString* tag = new CString;
			*tag = _T("工资;存款;补助;借款;还款;服装;餐饮;住宿;交通;医疗;教育;外贸;旅游;娱乐;活动");
			m_pRibbonReportView->SetRecordItemControl(2, XTP_REPORT_ITEM_CTRL_COMBOBOX,EMPTY,TRUE,TRUE,(DWORD_PTR)tag);
			*tag = _T("件;个;月;次;年;套;间");
			m_pRibbonReportView->SetRecordItemControl(4, XTP_REPORT_ITEM_CTRL_COMBOBOX,EMPTY,TRUE,TRUE,(DWORD_PTR)tag);
		}
		break;
	case ID_TABLE_INCOME:
	case ID_TABLE_OUTLAY:
		{
			CString* tag = new CString;
			*tag = _T("银行卡;支付宝;微信;现金");
			m_pRibbonReportView->SetRecordItemControl(9, XTP_REPORT_ITEM_CTRL_COMBOBOX,EMPTY,TRUE,TRUE,(DWORD_PTR)tag);
			m_pRibbonReportView->SetRecordItemControl(2, XTP_REPORT_ITEM_CTRL_LINK, _T("tv_funds"),TRUE,TRUE);
			m_pRibbonReportView->SetRecordItemControl(3, XTP_REPORT_ITEM_CTRL_LINK, _T("tv_member"),TRUE,TRUE);
			int col[] = {4,5,6,8};
			xtp_vector<int> cols(col);
			m_pRibbonReportView->SetRecordItemEditable(cols);
		}
		break;
	}
}

void CMainFrame::OnBnFinanceLoan()
{
	CDlgLoan dlg;
	dlg.DoModal();
}

void CMainFrame::OnBnZxingCommand()
{
	if(!m_pPopupZxing) return;
	m_pPopupZxing->SetCaption(ID_ZXING_FORMAT);
	m_pPopupZxing->SetIconId(ID_ZXING_FORMAT);

	if(m_pRibbonReportView && m_pRibbonReportView->m_pFocusItem)
	{
		CXTPString str = m_pRibbonReportView->m_pFocusItem->GetCaption();
		if(str.IsEmpty()) str = "6923450657713";;
		CXTPString filePath;
		filePath.Format(_T("%s\\Excel\\temp.jpg"), GetCurPath());

		switch (m_idZxingFormat)
		{
		case ID_ZXING_EAN13:
			EnCode_EAN_13A(str.c_ptr(), filePath.c_ptr(), 200, 75, 2);
			break;
		case ID_ZXING_PDF417:
			EnCode_PDF_417W(str.w_ptr(), filePath.w_ptr(), 300, 115, 2);
			break;
		case ID_ZXING_QRCODE:
			EnCode_QR_CODEW(str.w_ptr(), filePath.w_ptr(), 300, 300, 2);
			break;
			break;
		case ID_ZXING_QRCODEMID:
			{
				TCHAR szFilter[] = _T("PNG图片|*.png|JPG图片|*.jpg|BMP图片|*.bmp|所有文件|*.*||");
				CFileDialog FileDlg(TRUE, _T("png"), NULL, OFN_ALLOWMULTISELECT, szFilter, NULL);

				if(IDOK == FileDlg.DoModal())
				{
					CXTPString pngPath = FileDlg.GetPathName();
					EnCode_QR_CODE_MIDW(str.w_ptr(), filePath.w_ptr(), pngPath.w_ptr(), 300, 300, 2);
				}
			}
			break;
		}

		ShellExecuteOpen(CXTPString(filePath));
	}
}



/////////////////////////////////////////////////////////////////////////////
// CMainFrame Menu Item Update

void CMainFrame::OnPointsManager(UINT id)
{
	if(!m_pPopupPoints) return;
	m_idPointsmgr = id;
	m_pPopupPoints->SetCaption(id);
	m_pPopupPoints->SetIconId(id);
	m_pPopupPoints->SetTooltip(_T("当前设置工点数据列表！"));
	CString* tag = new CString;
	switch (id)
	{
	case ID_TABLE_ROAD:
		*tag = _T("_road_");
		break;
	case ID_TABLE_BRIDGE:
		*tag = _T("_bridge_");
		break;
	case ID_TABLE_TUNNEL:
		*tag = _T("_tunnel_");
		break;
	case ID_TABLE_ORBITAL:
		*tag = _T("_orbital_");
		break;
	case ID_TABLE_YARD:
		*tag = _T("_yard_");
		break;
	case ID_TABLE_TEMP:
		*tag = _T("_temp_");
		break;
	}
	m_pPopupPoints->SetTag((DWORD_PTR)tag);

	CString tb_point = g_TablePrix+_T("[tb_points]");
	CString fids = theCon.GetFields(tb_point,0,2);
	CXTPADOData adoName,adoStat;
	long rs = theCon.GetRecordCount(tb_point);
	GetRecord(adoName,_T("tb_points"),fids);
	_variant_t** name = (_variant_t**)adoName.vals[0];
	fids = theCon.GetFields(tb_point,4,7);
	GetRecord(adoStat,_T("tb_points"),fids);
	_variant_t** stat = (_variant_t**)adoStat.vals[0];

	if(m_pItemsSearchdatapart->GetItemCount() > 0)
		m_pItemsSearchdatapart->RemoveAll();

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
		switch (id)
		{
		case ID_TABLE_ROAD:
			if(strType == _T("路基"))
				str.Format(_T("%s  %s～%s"),strName,strBStat,strEStat);
			break;
		case ID_TABLE_BRIDGE:
			#ifdef _UNICODE
				str = strType.Right(1);
			#else
				str = strType.Right(2);
			#endif // _UNICODE
			if(str == _T("桥") || str == _T("涵"))
				str.Format(_T("%s  %s  L=%.3fm"),strName,strCStat,fLength);
			else
				str.Empty();
			break;
		case ID_TABLE_TUNNEL:
			if(strType == _T("隧道"))
				str.Format(_T("%s  %s  L=%.3fm"),strName,strCStat,fLength);
			else
				str.Empty();
			break;
		case ID_TABLE_ORBITAL:
		case ID_TABLE_YARD:
		case ID_TABLE_TEMP:
			if(strType == _T("路基"))
				str.Format(_T("%s  %s～%s"),strName,strBStat,strEStat);
			else
				str.Format(_T("%s  %s  L=%.3fm"),strName,strCStat,fLength);
			break;
		}

		if(!str.IsEmpty())
		{
			CXTPControlGalleryItem* pGalleryItem = m_pItemsSearchdatapart->AddItem(str);
			pGalleryItem->SetData((DWORD_PTR)new CString(strCode));
		}
	}

	m_pSearchDatapart->SetCurSel(0);
}

void CMainFrame::OnUpdatePointsManager(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(pCmdUI->m_nID == m_idPointsmgr);
}

void CMainFrame::OnCostManager(UINT id)
{
	if(!m_pPopupCost) return;
	m_idCostmgr = id;
	m_pPopupCost->SetCaption(id);
	m_pPopupCost->SetIconId(id);
	m_pPopupCost->SetTooltip(_T("当前设置成本管理数据列表！"));
	CString* tag = new CString;
	switch (id)
	{
	case ID_TABLE_QTY:
		*tag = _T("qty");
		break;
	case ID_TABLE_UP:
		*tag = _T("up");
		break;
	case ID_TABLE_DOWN:
		*tag = _T("down");
		break;
	case ID_TABLE_LIST:
		*tag = _T("lst");
		break;
	}

	m_pPopupCost->SetTag((DWORD_PTR)tag);
}

void CMainFrame::OnUpdateCostManager(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(pCmdUI->m_nID == m_idCostmgr);
}

void CMainFrame::OnMaterManager(UINT id)
{
	if(!m_pPopupMater) return;
	m_idMatermgr = id;
	m_pPopupMater->SetCaption(id);
	m_pPopupMater->SetIconId(id);
	m_pPopupMater->SetTooltip(_T("当前设置物资管理数据列表！"));
	CString* tag = new CString;
	switch (id)
	{
	case ID_TABLE_EMPLOY:
		*tag = _T("_employee");
		break;
	case ID_TABLE_UNITS:
		*tag = _T("_units");
		break;
	case ID_TABLE_GOODS:
		*tag = _T("_goods");
		break;
	case ID_TABLE_PACT:
		*tag = _T("_pact");
		break;
	case ID_TABLE_BARN:
		*tag = _T("_barn");
		break;
	case ID_TABLE_CHECK:
		*tag = _T("_check");
		break;
	case ID_TABLE_INSTOCK:
		*tag = _T("_instock");
		break;
	case ID_TABLE_OUTSTOCK:
		*tag = _T("_outstock");
		break;
	case ID_TABLE_MIXSTOCK:
		*tag = _T("_mixstock");
		break;
	case ID_TABLE_RETSTOCK:
		*tag = _T("_retstock");
		break;
	}

	m_pPopupMater->SetTag((DWORD_PTR)tag);
}

void CMainFrame::OnUpdateMaterManager(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(pCmdUI->m_nID == m_idMatermgr);
}

void CMainFrame::OnFinanceManager(UINT id)
{
	if(!m_pPopupFinance) return;
	m_idFinancemgr = id;
	m_pPopupFinance->SetCaption(id);
	m_pPopupFinance->SetIconId(id);
	m_pPopupFinance->SetTooltip(_T("当前设置财务管理数据列表！"));
	CString* tag = new CString;
	switch (id)
	{
	case ID_TABLE_MEMBER:
		*tag = _T("_member");
		break;
	case ID_TABLE_FUNDS:
		*tag = _T("_funds");
		break;
	case ID_TABLE_INCOME:
		*tag = _T("_income");
		break;
	case ID_TABLE_OUTLAY:
		*tag = _T("_outlay");
		break;
	}

	m_pPopupFinance->SetTag((DWORD_PTR)tag);
}

void CMainFrame::OnUpdateFinanceManager(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(pCmdUI->m_nID == m_idFinancemgr);
}

void CMainFrame::OnZxingCommand(UINT id)
{
	if(!m_pPopupZxing) return;
	m_idZxingFormat = id;
	m_pPopupZxing->SetCaption(id);
	m_pPopupZxing->SetIconId(id);
	m_pPopupZxing->SetTooltip(_T("当前设置条码/二维码编码样式！"));
}

void CMainFrame::OnUpdateZxingCommand(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(pCmdUI->m_nID == m_idZxingFormat ? TRUE : FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame Menu Item Update

void CMainFrame::OnHelpHelp()
{
	CString filePath = GetCurPath()+ _T("\\Help\\about.pdf");
	ShellExecuteOpen(filePath);
}

void CMainFrame::OnHelpReg()
{
	CDlgReg dreg;
	dreg.DoModal();
}

void CMainFrame::OnHelpNet()
{
	xtpchar path[MAX_PATH];
	GetModuleFileName(AfxGetApp()->m_hInstance,path,MAX_PATH);
	CXTPString xml(path);
	xml = xml.Mid(0,xml.ReverseFind('\\'));
	theCon.SetDataPath(xml);
	xml = xml+"\\RibbonReport.xml";
	CXTPTinyXmlDocument doc(xml);
	if(doc.LoadFile())
	{
		CXTPTinyXmlNode* node = doc.RootElement();
		if(!node) return;
		CXTPTinyXmlElement* root = node->ToElement();
		if(!root) return;
		CXTPString val(root->Attribute("Net"));
		theCon.SetServer(val);
		ShellExecuteOpen(val);
		doc.SaveFile();
	}
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame Menu Item Update

void CMainFrame::OnResetRibbonBar()
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	pCommandBars->Remove(pCommandBars->GetMenuBar());

	CreateRibbonBar();

	OnOptionsStyle(m_idRibbonStyle);
	pCommandBars->UpdateCommandBars();
	pCommandBars->RedrawCommandBars();
}

void CMainFrame::OnRibbonTabChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMXTPTABCHANGE* pNMTabChanged = (NMXTPTABCHANGE*)pNMHDR;

	if (pNMTabChanged->pTab)
	{
		switch (pNMTabChanged->pTab->GetIndex())
		{
		case 1:
			SetWebHtmlView();
			break;
		case 8:
			SetShellListView();
			SetShellTreeView();
			break;
		default:
			SetRibbonReportView();
			break;
		}
	}

	*pResult = 0;
}

void CMainFrame::OnRibbonTabChanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMXTPTABCHANGE* pNMTabChanging = (NMXTPTABCHANGE*)pNMHDR;


	*pResult = 0;
}


void CMainFrame::OnGalleryColors(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlGallery* pGallery = DYNAMIC_DOWNCAST(CXTPControlGallery, tagNMCONTROL->pControl);

	if (pGallery)
	{
		CXTPControlGalleryItem* pItem = pGallery->GetItem(pGallery->GetSelectedItem());
		if (pItem)
		{
			m_idColors = pItem->GetID();

			m_pItemsFontTextColor->RemoveAll();
			pItem = m_pItemsFontTextColor->AddLabel(0);
			pItem->SetCaption(_T("Theme Colors"));
			CGalleryItemFontColor::AddThemeColors(m_pItemsFontTextColor, m_idColors);

			pItem = m_pItemsFontTextColor->AddLabel(0);
			pItem->SetCaption(_T("Standard Colors"));
			CGalleryItemFontColor::AddStandardColors(m_pItemsFontTextColor);

		}

		*pResult = TRUE; // Handled
	}
}

void CMainFrame::OnUpdateGalleryColors(CCmdUI* pCmdUI)
{
	CXTPControlGallery* pGallery = 	DYNAMIC_DOWNCAST(CXTPControlGallery, CXTPControl::FromUI(pCmdUI));

	if (pGallery)
	{
		pGallery->SetCheckedItem(m_idColors);
	}
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnGalleryFonts(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlGallery* pGallery = DYNAMIC_DOWNCAST(CXTPControlGallery, tagNMCONTROL->pControl);

	if (pGallery)
	{
		CXTPControlGalleryItem* pItem = pGallery->GetItem(pGallery->GetSelectedItem());

		if (pItem)
		{
			m_idFonts = pItem->GetID();
		}

		*pResult = TRUE; // Handled
	}
}

void CMainFrame::OnUpdateGalleryFonts(CCmdUI* pCmdUI)
{
	CXTPControlGallery* pGallery = 	DYNAMIC_DOWNCAST(CXTPControlGallery, CXTPControl::FromUI(pCmdUI));

	if (pGallery)
	{
		pGallery->SetCheckedItem(m_idFonts);
	}
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnGalleryStyleCell(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlGallery* pGallery = DYNAMIC_DOWNCAST(CXTPControlGallery, tagNMCONTROL->pControl);

	if (pGallery)
	{
		CXTPControlGalleryItem* pItem = pGallery->GetItem(pGallery->GetSelectedItem());
		if (pItem)
			m_idStylesCell = pItem->GetID();

		*pResult = TRUE; // Handled
	}
}

void CMainFrame::OnUpdateGalleryStyleCell(CCmdUI* pCmdUI)
{
	CXTPControlGallery* pGallery = 	DYNAMIC_DOWNCAST(CXTPControlGallery, CXTPControl::FromUI(pCmdUI));

	if (pGallery)
		pGallery->SetCheckedItem(m_idStylesCell);

	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnGalleryShapes(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlGallery* pGallery = DYNAMIC_DOWNCAST(CXTPControlGallery, tagNMCONTROL->pControl);

	if (pGallery)
	{
		CXTPControlGalleryItem* pItem = pGallery->GetItem(pGallery->GetSelectedItem());
		if (pItem)
			m_idShape = pItem->GetID();

		*pResult = TRUE; // Handled
	}
}

void CMainFrame::OnUpdateGalleryShapes(CCmdUI* pCmdUI)
{
	CXTPControlGallery* pGallery = 	DYNAMIC_DOWNCAST(CXTPControlGallery, CXTPControl::FromUI(pCmdUI));

	if (pGallery)
		pGallery->SetCheckedItem(m_idShape);

	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnOptionsRightToLeft() 
{
	if (GetExStyle() & WS_EX_LAYOUTRTL)
	{
		GetCommandBars()->SetLayoutRTL(FALSE);
		ModifyStyleEx(WS_EX_LAYOUTRTL, 0);
	}
	else
	{
		GetCommandBars()->SetLayoutRTL(TRUE);
		ModifyStyleEx(0, WS_EX_LAYOUTRTL);
	}	

	m_wndMessageBar.ModifyStyleEx(WS_EX_LAYOUTRTL, GetExStyle() & WS_EX_LAYOUTRTL);

	RecalcLayout(FALSE);
}

void CMainFrame::OnOptionsAnimation() 
{
	m_bAnimation ^= 1;
	GetCommandBars()->GetMenuBar()->EnableAnimation(m_bAnimation);

}

void CMainFrame::OnFrameTheme()
{
	ShowWindow(SW_NORMAL);
	CXTPWindowRect rc(this);
	rc.top += (m_pRibbonBar->IsFrameThemeEnabled() ? -1 : +1) * GetSystemMetrics(SM_CYCAPTION);
	MoveWindow(rc);

	m_pRibbonBar->EnableFrameTheme(!m_pRibbonBar->IsFrameThemeEnabled());
}

void CMainFrame::OnUpdateOptionsRighttoleft(CCmdUI* pCmdUI) 
{
	if (XTPSystemVersion()->IsLayoutRTLSupported())
	{
		pCmdUI->SetCheck(GetExStyle() & WS_EX_LAYOUTRTL ? TRUE : FALSE);	
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdateOptionsAnimation(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bAnimation);	
}

void CMainFrame::OnUpdateFrameTheme(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_pRibbonBar->IsFrameThemeEnabled() ? TRUE : FALSE);
}

void CMainFrame::OnOptionsStyle(UINT nStyle)
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	pCommandBars->SetTheme(xtpThemeRibbon);

	HMODULE hModule = AfxGetInstanceHandle();

	LPCTSTR lpszIniFile = 0;
	switch (nStyle)
	{
	case ID_OPTIONS_STYLE_BLUE2007:
		hModule = LoadLibrary(m_strStylesPath + _T("Office2007.dll"));
		lpszIniFile = _T("OFFICE2007BLUE.INI"); 
		break;

	case ID_OPTIONS_STYLE_BLACK2007: 
		hModule = LoadLibrary(m_strStylesPath + _T("Office2007.dll"));
		lpszIniFile = _T("OFFICE2007BLACK.INI"); 
		break;

	case ID_OPTIONS_STYLE_AQUA2007: 
		hModule = LoadLibrary(m_strStylesPath + _T("Office2007.dll"));
		lpszIniFile = _T("OFFICE2007AQUA.INI"); 
		break;

	case ID_OPTIONS_STYLE_SILVER2007: 
		hModule = LoadLibrary(m_strStylesPath + _T("Office2007.dll"));
		lpszIniFile = _T("OFFICE2007SILVER.INI"); 
		break;

	case ID_OPTIONS_STYLE_SCENIC7: 
		hModule = LoadLibrary(m_strStylesPath + _T("Windows7.dll"));
		lpszIniFile = _T("WINDOWS7BLUE.INI"); 
		break;

	case ID_OPTIONS_STYLE_SILVER2010: 
		hModule = LoadLibrary(m_strStylesPath + _T("Office2010.dll"));
		lpszIniFile = _T("OFFICE2010SILVER.INI"); 
		break;

	case ID_OPTIONS_STYLE_BLUE2010: 
		hModule = LoadLibrary(m_strStylesPath + _T("Office2010.dll"));
		lpszIniFile = _T("OFFICE2010BLUE.INI"); 
		break;

	case ID_OPTIONS_STYLE_BLACK2010: 
		hModule = LoadLibrary(m_strStylesPath + _T("Office2010.dll"));
		lpszIniFile = _T("OFFICE2010BLACK.INI"); 
		break;

	case ID_OPTIONS_STYLE_SYSTEM: 
		hModule = LoadLibrary(m_strStylesPath + _T("Windows7.dll"));
		lpszIniFile = _T("WINDOWS7BLUE.INI");
		break;
	}

	if (hModule != 0)
		((CXTPResourceTheme*)pCommandBars->GetPaintManager())->SetImageHandle(hModule, lpszIniFile);

	if (nStyle >= ID_OPTIONS_STYLE_SCENIC7 && nStyle <= ID_OPTIONS_STYLE_BLACK2010)
	{
		m_pRibbonBar->GetSystemButton()->SetStyle(xtpButtonCaption);
		CreateBackstage();
	}
	else
	{
		m_pRibbonBar->GetSystemButton()->SetStyle(xtpButtonAutomatic);
		CreateSystemMenuPopup();
	}

	pCommandBars->GetPaintManager()->GetIconsInfo()->bUseFadedIcons = FALSE;
	pCommandBars->GetPaintManager()->GetIconsInfo()->bIconsWithShadow = FALSE;
	pCommandBars->GetPaintManager()->GetIconsInfo()->bUseDisabledIcons = TRUE;

	m_pRibbonBar->GetSystemButton()->SetStyle(xtpButtonAutomatic);

	if (XTPSystemVersion()->IsWinVistaOrGreater())
	{
		pCommandBars->GetPaintManager()->m_bUseOfficeFont = TRUE;
		pCommandBars->GetPaintManager()->m_strOfficeFont = _T("Segoe UI");
	}

	m_wndPaneManager.SetTheme(xtpPaneThemeResource);
	m_wndPaneManager.GetPaintManager()->RefreshMetrics();
	m_wndPaneManager.RedrawPanes();

	if (m_pRibbonReportView)
	{
		m_pRibbonReportView->GetPaintManager()->SetColumnStyle(xtpReportColumnResource);
		m_pRibbonReportView->GetPaintManager()->RefreshMetrics();
		RedrawFrame(&m_pRibbonReportView->GetReportCtrl());
	}

	GetCommandBars()->GetPaintManager()->RefreshMetrics();
	GetCommandBars()->GetImageManager()->RefreshAll();
	GetCommandBars()->RedrawCommandBars();

	SendMessage(WM_NCPAINT);
	RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE);

	m_idRibbonStyle = nStyle;
}

void CMainFrame::OnUpdateOptionsStyle(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_idRibbonStyle ? 1 : 0);
}

int nFonts[] = {0, 12, 14, 16};
void CMainFrame::OnOptionsFont(UINT nID)
{
	int nFontHeight = nFonts[nID - ID_OPTIONS_FONT_SYSTEM];
	m_pRibbonBar->SetFontHeight(nFontHeight);
}

void CMainFrame::OnUpdateOptionsFont(CCmdUI* pCmdUI)
{
	int nFontHeight = nFonts[pCmdUI->m_nID - ID_OPTIONS_FONT_SYSTEM];
	pCmdUI->SetCheck(m_pRibbonBar->GetFontHeight() == nFontHeight ? TRUE : FALSE);
}

void CMainFrame::OnToggleGroups() 
{
	m_pRibbonBar->SetRibbonMinimized(!m_pRibbonBar->IsRibbonMinimized());
}

void CMainFrame::OnAutoResizeIcons()
{
	GetCommandBars()->GetPaintManager()->m_bAutoResizeIcons ^= 1;
	GetCommandBars()->RedrawCommandBars();
}

void CMainFrame::OnUpdateAutoResizeIcons(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetCommandBars()->GetPaintManager()->m_bAutoResizeIcons);
}

void CMainFrame::OnUpdateRibbonMinimize(CCmdUI* pCmdUI)
{
	CXTPControl* pControl = CXTPControl::FromUI(pCmdUI);
	if (pControl)
		pControl->SetVisible(!m_pRibbonBar->IsRibbonMinimized());
}

void CMainFrame::OnUpdateRibbonExpand(CCmdUI* pCmdUI)
{
	CXTPControl* pControl = CXTPControl::FromUI(pCmdUI);
	if (pControl)
		pControl->SetVisible(m_pRibbonBar->IsRibbonMinimized());
}

void CMainFrame::OnRibbonSwitchReportView()
{
	if(!GetActiveView()->IsKindOf(RUNTIME_CLASS(CRibbonReportView)))
		SetRibbonReportView();

	RefreshPanes();
}

void CMainFrame::OnRibbonSwitchChartView()
{
	if(!GetActiveView()->IsKindOf(RUNTIME_CLASS(CChartView)))
		SetChartView();

	RefreshPanes();
}


void CMainFrame::OnUpdateRibbonSwitchReportView(CCmdUI* pCmdUI)
{
	CXTPControl* pControl = CXTPControl::FromUI(pCmdUI);
	if (pControl)
		pControl->SetVisible(GetActiveView()->IsKindOf(RUNTIME_CLASS(CRibbonReportView)));
}

void CMainFrame::OnUpdateRibbonSwitchChartView(CCmdUI* pCmdUI)
{
	CXTPControl* pControl = CXTPControl::FromUI(pCmdUI);
	if (pControl)
		pControl->SetVisible(GetActiveView()->IsKindOf(RUNTIME_CLASS(CChartView)));
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CXTPFrameWnd::OnGetMinMaxInfo(lpMMI);

	if (!GetCommandBars())
		return;

	if (m_pRibbonBar && m_pRibbonBar->IsBackstageViewVisible())
	{
		lpMMI->ptMinTrackSize.x = 500;
		lpMMI->ptMinTrackSize.y = 400;

	}
}

void CMainFrame::OnSystemCommand(UINT nID)
{
	switch (nID)
	{
	case ID_DATASET_DATBACKUP:
		{
			bool ret = false;
			msgAsk(_T("确定要备份数据库到 D 盘吗？"), ret);
			if(ret)
			{
				if(DataBackup(_T("ProjectManage"),_T("D:\\ProjectManage.bak")))
				{
					msgInf(_T("数据库备份成功！"));
				}
				else
				{
					msgInf(_T("数据库备份失败！"));
				}
			}
		}
		break;
	case ID_DATASET_DATRESTORE:
		{
			bool ret = false;
			msgAsk(_T("确定要恢复 D 盘恢复数据库吗？"), ret);
			if(ret)
			{
				if(DataRestore(_T("ProjectManager"),_T("D:\\ProjectManager.bak")))
				{
					msgInf(_T("数据库备份成功！"));
				}
				else
				{
					msgInf(_T("数据库备份失败！"));
				}
			}
		}
		break;
	case ID_DATASET_DATCLEAR:
		{
			bool ret = false;
			msgAsk(_T("确定要清空当前数据库吗？"), ret);
			if(ret)
			{
				if(DataClear())
				{
					msgInf(_T("数据库清空成功！"));
				}
				else
				{
					msgInf(_T("数据库清空失败！"));
				}
			}
		}
		break;
	case ID_USERMGR_REGUSER:
		{
			CDlgRegUser dlg;
			if(dlg.DoModal() == IDOK)
			{
				if(dlg.m_strPwd == dlg.m_strPwd2)
				{
					g_SysUser = dlg.m_strUser;
					g_SysPwd = dlg.m_strPwd2;
					if(AddSysUser())
					{
						msgInf(_T("注册成功！"));
					}
					else msgErr(_T("注册失败！"));
				}
				else msgErr(_T("两次输入密码不同！"));
			}
		}
		break;
	case ID_USERMGR_SETUSER:
		{
			CDlgSetPwd dlg;
			dlg.m_strUser = g_SysUser;
			dlg.m_strPwd = g_SysPwd;
			if(dlg.DoModal() == IDOK)
			{
				if(dlg.m_strPwd2 == dlg.m_strPwd3)
				{
					if(dlg.m_strPwd != dlg.m_strPwd2)
					{
						g_SysUser = dlg.m_strUser;
						g_SysPwd = dlg.m_strPwd2;
						if(SetSysPwd())
						{
							msgInf(_T("密码修改成功！"));
						}
						else msgErr(_T("密码修改失败！"));
					}
					else msgErr(_T("原密码与新密码相同！"));
				}
				else msgErr(_T("两次输入密码不同！"));
			}
		}
		break;
	case ID_USERMGR_DELUSER:
		{
			bool ret = false;
			msgAsk(_T("确定要删除当前用户吗？"), ret);
			if(ret) DelSysUser();
		}
		break;
	case ID_USERMGR_AUTOLOGIN:
		{
			g_SysAutoLogin = !g_SysAutoLogin;
			SetSysStatus(g_SysRemberPwd,g_SysAutoLogin,IsAdmin,IsConnected);
		}
		break;
	case ID_USERMGR_USERINFO:
		{
			ado_users src;
			int ret = FindSysUid(src);
			CString info;
			info.Format(_T("编号,用户,密码,密匙\n%s,%s,%s,%s \
				            \n客户端IP,客户端口,服务器IP,服务器端口\n%s,%d,%s,%d \
							\n登录状态,系统权限,记住密码,自动登录\n%s,%s,%s,%s"),
							src.id,src.sysuser,src.password,src.md5pwd,
							src.cip,src.cport,src.sip,src.sport,
							src.state?_T("True"):_T("False"),src.advanced?_T("True"):_T("False"),src.remberpwd?_T("True"):_T("False"),src.autologin?_T("True"):_T("False"));
			msgInf(info);
		}
		break;
	case ID_PROGRAM_TASKTRACK:
		SetTaskTrackView();
		break;
	case ID_PROGRAM_METHOD:
		OnBnPointsManager();
		break;
	case ID_PROGRAM_CHART:
		SetChartView();
		break;
	case ID_PROGRAM_REPORT:
		CString filePath = GetCurPath()+ _T("\\ReportCreator.exe");
		ShellExecuteOpen(filePath);
		break;
	}
}

void CMainFrame::OnExplorCommand(UINT nID)
{
	switch (nID)
	{
	case ID_EXPLOR_SEARCH:
		m_pSearchView;
		break;
	case ID_EXPLOR_DOWNLOAD:
		m_pShellListView->DownLoad();
		break;
	case ID_EXPLOR_UPLOAD:
		m_pShellListView->UpLoad();
		break;
	case ID_EXPLOR_DELETE:
		m_pShellListView->Delete();
		break;
	case ID_EXPLOR_FLUSH:
		m_pShellListView->Flush();
		break;
	}
}

void CMainFrame::OnUpdateGallerySearchDataPart(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateSearchDataPart(CCmdUI* pCmdUI)
{
	CXTPControlComboBox* pControlCombo = (CXTPControlComboBox*)CXTPControl::FromUI(pCmdUI);

	pCmdUI->Enable(TRUE);

	if (pControlCombo && pControlCombo->GetType() == xtpControlComboBox && m_pItemsSearchdatapart)
	{
		int idx = pControlCombo->GetCurSel();
		if(idx > -1 && m_pRibbonReportView)
		{
			CString str = pControlCombo->GetEditText();
			CXTPControlGalleryItem* pCalleryItem = m_pItemsSearchdatapart->GetItem(idx);
			if(pCalleryItem)
			{
				CString cap = pCalleryItem->GetCaption();
				if(cap != str && m_pRibbonReportView)
				{
					pControlCombo->SetEditText(pCalleryItem->GetCaption());
					CString* tag = (CString*)(DWORD_PTR)pCalleryItem->GetData();
					CString sql;
					sql.Format(_T("工点编号='%s'"),*tag);

					//m_pRibbonReportView->m_wndFilterEdit.SetText(*tag);

					CString table = m_pRibbonReportView->IsView() ? m_pRibbonReportView->GetView(): m_pRibbonReportView->GetTable();
					m_pRibbonReportView->DataBindRecordset(GetRecordset(table,sql), FALSE);
					m_pRibbonReportView->SetRecordItemControl();
					m_pRibbonReportView->SetRecordItemEditable();
				}
			}
		}
	}
}

void CMainFrame::OnUpdateSearchDatePopup(CCmdUI* pCmdUI)
{
	//CXTPControlPopup* pControlPopup = DYNAMIC_DOWNCAST(CXTPControlPopup, CXTPControl::FromUI(pCmdUI));
	//if (pControlPopup && m_pSearchDatePicker && m_pSearchDateEdit)
	//{
	//	if(m_pSearchDatePicker->IsSelected(m_oleSearchDateTime))
	//	{
	//		m_pSearchDateEdit->SetFocused(TRUE);
	//		CString st;
	//		if (m_pSearchDatePicker->GetCurSel(m_oleSearchDateTime))
	//			st = m_oleSearchDateTime.Format(_T("%Y-%m-%d"));

	//		if(!st.IsEmpty())
	//		{
	//			m_pSearchDateEdit->SetEditText(st);
	//			m_pSearchDateEdit->SetFocused(FALSE);
	//			m_pSearchDatePicker->Deselect(m_oleSearchDateTime);
	//		}

	//		pControlPopup->SetFocused(FALSE);
	//	}
	//}

	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateSearchDateEdit(CCmdUI* pCmdUI)
{
	CXTPControlEdit* pControlEdit = DYNAMIC_DOWNCAST(CXTPControlEdit, CXTPControl::FromUI(pCmdUI));

	if (pControlEdit && m_pSearchDatapart && m_pItemsSearchdatapart && m_pRibbonReportView)
	{
		CString st;
		if(m_pSearchDatePicker && m_pSearchDatePicker->GetCurSel(m_oleSearchDateTime))
		{
			st = m_oleSearchDateTime.Format(_T("%Y-%m-%d"));
			m_pSearchDateEdit->SetEditText(st);
			m_pSearchDateEdit->SetFocused(TRUE);
			m_pSearchDateEdit->SetFocused(FALSE);
			m_pSearchDatePicker->Deselect(m_oleSearchDateTime);

			//m_pRibbonReportView->m_wndFilterEdit.SetText(st);

			int idx = m_pSearchDatapart->GetCurSel();
			if(idx > -1)
			{
				CXTPControlGalleryItem* pCalleryItem = m_pItemsSearchdatapart->GetItem(m_pSearchDatapart->GetCurSel());
				if(pCalleryItem)
				{
					CString table = m_pRibbonReportView->IsView() ? m_pRibbonReportView->GetView(): m_pRibbonReportView->GetTable();
					CString* tag = (CString*)(DWORD_PTR)pCalleryItem->GetData();
					CString sql;

					if(st.IsEmpty())
						sql.Format(_T("工点编号='%s'"),*tag);
					else
					{
						if(m_idCostmgr==ID_TABLE_UP)
							sql.Format(_T("工点编号='%s' and 对上日期='%s'"),*tag,st);
						else if(m_idCostmgr==ID_TABLE_DOWN)
							sql.Format(_T("工点编号='%s' and 对下日期='%s'"),*tag,st);
					}

					m_pRibbonReportView->DataBindRecordset(GetRecordset(table,sql), FALSE);
					m_pRibbonReportView->SetRecordItemControl();
					m_pRibbonReportView->SetRecordItemEditable();
				}
			}
		}

		if(pControlEdit->HasFocus())
		{
			st = pControlEdit->GetEditText();
			//m_pRibbonReportView->m_wndFilterEdit.SetText(st);

			int idx = m_pSearchDatapart->GetCurSel();
			if(idx > -1)
			{
				CXTPControlGalleryItem* pCalleryItem = m_pItemsSearchdatapart->GetItem(m_pSearchDatapart->GetCurSel());
				if(pCalleryItem)
				{
					CString table = m_pRibbonReportView->IsView() ? m_pRibbonReportView->GetView(): m_pRibbonReportView->GetTable();
					CString* tag = (CString*)(DWORD_PTR)pCalleryItem->GetData();
					CString sql;

					if(st.IsEmpty())
						sql.Format(_T("工点编号='%s'"),*tag);
					else
					{
						if(m_idCostmgr==ID_TABLE_UP)
							sql.Format(_T("工点编号='%s' and 对上日期='%s'"),*tag,st);
						else if(m_idCostmgr==ID_TABLE_DOWN)
							sql.Format(_T("工点编号='%s' and 对下日期='%s'"),*tag,st);
					}

					m_pRibbonReportView->DataBindRecordset(GetRecordset(table,sql), FALSE);
					m_pRibbonReportView->SetRecordItemControl();
					m_pRibbonReportView->SetRecordItemEditable();
				}
			}
		}
	}

	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnSearchDateEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPUPDOWN* tagNMCONTROL = (NMXTPUPDOWN*)pNMHDR;
	CXTPControlEdit* pControlEdit = DYNAMIC_DOWNCAST(CXTPControlEdit, tagNMCONTROL->pControl);

	if (pControlEdit)
	{
		COleDateTime t = m_oleSearchDateTime;
		COleDateTimeSpan ts;
		CString st;

		if(tagNMCONTROL->iDelta > 0)
		{
			if(t.GetMonth() > 1)
				ts = COleDateTime(t.GetYear(),t.GetMonth(),1,0,0,0) - COleDateTime(t.GetYear(),t.GetMonth()-1,1,0,0,0);
			else ts = COleDateTimeSpan(31,0,0,0);

			m_oleSearchDateTime = t-ts;
		}

		if(tagNMCONTROL->iDelta < 0)
		{
			if(t.GetMonth() < 12)
				ts = COleDateTime(t.GetYear(),t.GetMonth()+1,1,0,0,0) - COleDateTime(t.GetYear(),t.GetMonth(),1,0,0,0);
			else ts = COleDateTimeSpan(31,0,0,0);

			m_oleSearchDateTime = t+ts;
		}

		st = m_oleSearchDateTime.Format(_T("%Y-%m-%d"));
		if(!st.IsEmpty() && m_pSearchDatapart && m_pItemsSearchdatapart && m_pRibbonReportView)
		{
			pControlEdit->SetEditText(st);

			//m_pRibbonReportView->m_wndFilterEdit.SetText(st);

			int idx = m_pSearchDatapart->GetCurSel();
			if(idx > -1)
			{
				CXTPControlGalleryItem* pCalleryItem = m_pItemsSearchdatapart->GetItem(m_pSearchDatapart->GetCurSel());
				if(pCalleryItem)
				{
					CString table = m_pRibbonReportView->IsView() ? m_pRibbonReportView->GetView(): m_pRibbonReportView->GetTable();
					CString* tag = (CString*)(DWORD_PTR)pCalleryItem->GetData();
					CString sql;

					if(st.IsEmpty())
						sql.Format(_T("工点编号='%s'"),*tag);
					else
					{
						if(m_idCostmgr==ID_TABLE_UP)
							sql.Format(_T("工点编号='%s' and 对上日期='%s'"),*tag,st);
						else if(m_idCostmgr==ID_TABLE_DOWN)
							sql.Format(_T("工点编号='%s' and 对下日期='%s'"),*tag,st);
					}

					m_pRibbonReportView->DataBindRecordset(GetRecordset(table,sql), FALSE);
					m_pRibbonReportView->SetRecordItemControl();
					m_pRibbonReportView->SetRecordItemEditable();
				}
			}
		}
	}

	*pResult = 1;
}

void CMainFrame::OnSearchDateButton(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPUPDOWN* tagNMCONTROL = (NMXTPUPDOWN*)pNMHDR;
	CXTPControlPopup* pControlPopup = DYNAMIC_DOWNCAST(CXTPControlPopup, tagNMCONTROL->pControl);

	if (pControlPopup)
	{
		CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());

		m_pSearchDatePicker = new CXTPDatePickerControl;
		if (!m_pSearchDatePicker->Create(WS_TABSTOP | WS_CHILD | WS_BORDER | DTS_SHORTDATEFORMAT,
			CRect(0, 0, 205, 205), pPopupBar, ID_POINTSMGR_DATEPICKER)) return;
		m_pSearchDatePicker->SetButtonsVisible(TRUE, FALSE);
		m_pSearchDatePicker->SetShowWeekNumbers(TRUE);
		CXTPControlCustom* pCustomDateTime = CXTPControlCustom::CreateControlCustom(m_pSearchDatePicker);
		pCustomDateTime->SetFlags(xtpFlagManualUpdate);

		pPopupBar->GetControls()->Add(pCustomDateTime, ID_POINTSMGR_DATECTRL);
		pPopupBar->EnableCustomization(FALSE);
		pPopupBar->InternalRelease();
		pPopupBar->Popup(0,0,CRect(0, 0, 205, 205));
	}

	*pResult = 1;
}

void CMainFrame::LoadRegSoft(vector<CXTPRibbonTab*>& vecTabs)
{
	CTime tmBegin(2019,5,1,0,0,0);
	CTime tmEnd = CTime(2019,12,31,0,0,0);
	DelRegSoft(tmBegin);

	vecTabs[0]->SetEnabled(FALSE);
	vecTabs[1]->SetEnabled(FALSE);
	vecTabs[2]->SetEnabled(FALSE);
	vecTabs[3]->SetEnabled(FALSE);
	vecTabs[4]->SetEnabled(FALSE);
	vecTabs[5]->SetEnabled(FALSE);
	vecTabs[6]->SetEnabled(FALSE);

	if(IsAdmin)
	{
		if(IsConnected)
		{
			vecTabs[0]->SetEnabled(TRUE);
			vecTabs[1]->SetEnabled(TRUE);
			vecTabs[2]->SetEnabled(TRUE);
			vecTabs[3]->SetEnabled(TRUE);
			vecTabs[4]->SetEnabled(TRUE);
			vecTabs[5]->SetEnabled(TRUE);
			vecTabs[6]->SetEnabled(TRUE);
		}

		m_pStatusBarLogo->SetText(_T("<TextBlock VerticalAlignment='Center'><Bold> <Run Foreground='#00007F'>施工项目管理系统专业版v3.0</Run></Bold></TextBlock>"));
	}
	else
	{
		if(IsRegSoft())
		{
			if(IsConnected)
			{
				vecTabs[0]->SetEnabled(TRUE);
				vecTabs[1]->SetEnabled(TRUE);
				vecTabs[2]->SetEnabled(TRUE);
				vecTabs[3]->SetEnabled(TRUE);
				vecTabs[4]->SetEnabled(TRUE);
				vecTabs[5]->SetEnabled(TRUE);
				vecTabs[6]->SetEnabled(TRUE);
			}

			m_pStatusBarLogo->SetText(_T("<TextBlock VerticalAlignment='Center'><Bold> <Run Foreground='Black'>施工项目管理系统注册版v3.0</Run></Bold></TextBlock>"));
		}
		else
		{
			CreateMessageBar();
			CTime tm = CTime::GetCurrentTime();
			if(tm > tmBegin && tm < tmEnd)
			{
				if(IsConnected)
				{
					vecTabs[0]->SetEnabled(TRUE);
					vecTabs[1]->SetEnabled(TRUE);
					vecTabs[2]->SetEnabled(TRUE);
				}

				m_pStatusBarLogo->SetText(_T("<TextBlock VerticalAlignment='Center'><Bold> <Run Foreground='#FF0000'>施工项目管理系统试用版v3.0</Run></Bold></TextBlock>"));
			}
			else
			{
				m_pStatusBarLogo->SetText(_T("<TextBlock VerticalAlignment='Center'><Bold><Run Foreground='Black'>您的使用有效期至2017-07-31结束</Run> <Run Foreground='#FF0000'>使用权限已过期请联系管理员注册</Run></Bold></TextBlock>"));
			}
		}
	}
}

BOOL CMainFrame::IsRegSoft() const
{
	HKEY key;
	DWORD size = MAX_PATH;
	xtpchar data[MAX_PATH];
	DWORD type=REG_SZ;
	//CXTPString skey="Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\App Paths\\RibbonReport";
	CXTPString skey="Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\RibbonReport";
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,skey,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,&key) == ERROR_SUCCESS)
	{
		if(RegQueryValueEx(key,_T("RegSerial"),NULL,&type,(BYTE*)data,&size) == ERROR_SUCCESS)
		{
			CXTPString serial = SnKey(DiskSerial().Mid(3,4)+MacAddress().Mid(4,6));
			LPXTPSTR szStr = new xtpchar[size+1];
			for(DWORD i=0; i<size; i++)
			{
				szStr[i] = data[i];
			}
			szStr[size] = '\0';
			CXTPString reg_serial(szStr);
			int index = reg_serial.Find(_T("_"));
			reg_serial = reg_serial.Right(index+1);
			if(RegQueryValueEx(key,_T("IsReged"),NULL,&type,(BYTE*)data,&size) == ERROR_SUCCESS)
			{
				szStr = new xtpchar[2];
				szStr[0] = data[0];
				szStr[1] = '\0';
				CXTPString reg_reged(szStr);

				if(serial == reg_serial && reg_reged == CXTPString("1"))
				{
					RegCloseKey(key);
					return TRUE;
				}
			}
		}
	}

	RegCloseKey(key);
	return FALSE;
}

BOOL CMainFrame::DelRegSoft(CTime tm) const
{
	HKEY key;
	DWORD size = MAX_PATH;
	xtpchar data[MAX_PATH];
	DWORD type=REG_SZ;
	//CXTPString skey="Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\App Paths\\RibbonReport";
	CXTPString skey="Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\RibbonReport";
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,skey,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,&key) == ERROR_SUCCESS)
	{
		if(RegQueryValueEx(key,_T("RegDate"),NULL,&type,(BYTE*)data,&size) == ERROR_SUCCESS)
		{
			LPXTPSTR szStr = new xtpchar[size+1];
			for(DWORD i=0; i<size; i++)
			{
				szStr[i] = data[i];
			}
			szStr[size] = '\0';
			CXTPString reg_date(szStr);

			CXTPString year = reg_date.Lefts(4);
			CXTPString month = reg_date.Mid(5,2);
			CXTPString day = reg_date.Rights(2);
			CTime tm_reged(year.ToInt(),month.ToInt(),day.ToInt(),0,0,0);
			if(tm_reged < tm)
			{
				//skey="Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\App Paths";
				skey="Software\\Microsoft\\Windows\\CurrentVersion\\App Paths";
				if(RegOpenKeyEx(HKEY_CURRENT_USER,skey,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,&key) == ERROR_SUCCESS)
				{
					if(RegDeleteKeyEx(key,_T("RibbonReport"),KEY_ALL_ACCESS,0) == ERROR_SUCCESS)
					{
						RegCloseKey(key);
						return TRUE;
					}
				}
			}
		}
	}

	RegCloseKey(key);
	return FALSE;
}