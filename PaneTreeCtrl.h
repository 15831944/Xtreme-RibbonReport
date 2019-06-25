// PaneTreeCtrl.h: interface for the CPaneTreeCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PANETREECTRL_H__6C481840_FE83_4798_9524_1A01F1FB13DB__INCLUDED_)
#define AFX_PANETREECTRL_H__6C481840_FE83_4798_9524_1A01F1FB13DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPaneTreeCtrl : public CXTPTreeCtrl
{
public:
	enum TreeType
	{
		treeRoot,
		treeCategory,
		treeItem
	};

	void GetChildItemCount(HTREEITEM hItem = NULL);
	TreeType GetItemType(HTREEITEM hItem);

	void ExpandItems(HTREEITEM hItem = NULL);
	void CollapseItems(HTREEITEM hItem = NULL);

	static CLIPFORMAT m_cfItem;
	static int m_nItemCount;

protected:
	LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam);

protected:
	//{{AFX_MSG(CPaneTreeCtrl)
	//}}AFX_MSG
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnItemExpanded(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

private:
	CImageList m_ilTreeIcons;
};

class CActions : public CXTPControlActions
{
public:
	CActions() : CXTPControlActions(NULL) {}
	CXTPControlAction* operator[](int nIndex);

public:
	int InsertAction(CXTPControlAction* pAction);
	CXTPControlAction* AddAction(UINT nID, LPCTSTR lpszKey, LPCTSTR lpszCategory, UINT nImage = -1, LPCTSTR lpszDescription=_T(""), LPCTSTR lpszTooltip=_T(""));
	CXTPControlAction* AddAction(LPCTSTR lpszCaption, UINT nID, LPCTSTR lpszKey, LPCTSTR lpszCategory, UINT nImage = -1, LPCTSTR lpszDescription=_T(""), LPCTSTR lpszTooltip=_T(""));
	int GetIndex(CXTPControlAction* pAction);
	int GetIndex(UINT nID);
	void ReplaceActionId(CXTPControlAction* pAction, int nID);
	void DeleteAction(CXTPControlAction* pAction);
};

#endif // !defined(AFX_PANETREECTRL_H__6C481840_FE83_4798_9524_1A01F1FB13DB__INCLUDED_)




/*

void TreeCtrl控件总结()
{
CTreeCtrl wndTreeCtrl;
//基础操作
//---1插入节点
//---1）插入根节点

//插入根节点
HTREEITEM hRoot,hItem;
CString str=L"ROOT";
hRoot = wndTreeCtrl.InsertItem(str);
//相当于
hRoot=wndTreeCtrl.InsertItem(str,TVI_ROOT,TVI_LAST);


//---2）插入孩子节点
//添加hRoot节点的孩子节点，并且被添加的节点位于hRoot所有孩子节点的末尾
HTREEITEM hChild=wndTreeCtrl.InsertItem(str,hRoot);
//相当于
HTREEITEM hChild=wndTreeCtrl.InsertItem(str,hRoot,TVI_LAST);


//---2获得节点句柄
//获得根节点
HTREEITEM hRootItem;
hRootItem=wndTreeCtrl.GetRootItem();

//获得当前节点
HTREEITEM hCurrentItem;
hCurrentItem=wndTreeCtrl.GetSelectedItem();

//获得hItem的前一个节点
HTREEITEM hPreItem;
hPreItem=wndTreeCtrl.GetNextItem(hItem,TVGN_PREVIOUS);

//获得hItem的下一个节点
HTREEITEM hNextItem;
hNextItem=wndTreeCtrl.GetNextItem(hItem,TVGN_NEXT);


//---3判断某节点是否有孩子节点
//判断某节点是否有孩子节点
if(wndTreeCtrl.ItemHasChildren(hRoot)) return;


//---4展开或收缩子节点
HTREEITEM hParentItem;
if(wndTreeCtrl.ItemHasChildren(hRoot))
	wndTreeCtrl.Expand(hParentItem,TVE_EXPAND);


//---5获得第一个孩子节点的句柄
//判断某节点是否有孩子节点
if(wndTreeCtrl.ItemHasChildren(hRoot))
{
	//获得孩子节点
	HTREEITEM hChild=wndTreeCtrl.GetChildItem(hRoot);
}


//---6遍历hRoot下一层的所有孩子节点
//判断某节点是否有孩子节点
if(wndTreeCtrl.ItemHasChildren(hRoot))
{
	//获得孩子节点
	HTREEITEM hChild=wndTreeCtrl.GetChildItem(hRoot);

	//遍历hRoot下一层的所有孩子节点
	while(hChild)
	{
		hChild=wndTreeCtrl.GetNextItem(hChild,TVGN_NEXT);
	}
}


//---7获得某节点上的文字
//获得某节点上的文字
CString str;
wndTreeCtrl.GetItemText(hRoot);


//---8选择某节点，并让其获得焦点
//首先，TREE控件的样式必须设置为TVS_SHOWSELALWAYS
//其次：选择该节点
wndTreeCtrl.SelectItem(hItem);
//最后，设置焦点

wndTreeCtrl.SetFocus();
//Tree控件设置焦点后，会自动将焦点定位到选择的节点上


//---9清空树控件
wndTreeCtrl.DeleteAllItems();

}

//---10将指定目录下的文件插入节点
void InsertPath(CString path,HTREEITEM hRoot,CTreeCtrl& ctrl)
{
	CFileFind nFindFile;
	CString str=L"";
	CString nPicFileName=L"";
	BOOL IsExist=FALSE;
	HTREEITEM hSubItem;

	nPicFileName.Format(L"%s\\*.*",path);
	IsExist=nFindFile.FindFile(nPicFileName);
	while(IsExist)
	{
		IsExist=nFindFile.FindNextFile();
		if(nFindFile.IsDots())
			continue;
		nPicFileName=nFindFile.GetFileName();

		//路径
		if(nFindFile.IsDirectory())
		{
			hSubItem=ctrl.InsertItem(nPicFileName,hRoot);
			InsertPath(nFindFile.GetFilePath(),hSubItem,ctrl);
		}
		else
		{
			//文件
			str=nPicFileName.Right(4);
			if(!str.CompareNoCase(_T(".jpg"))||!str.CompareNoCase(_T(".tif")))
			{
				ctrl.InsertItem(nPicFileName,hRoot);
			}
		}
	}
	nFindFile.Close();
}

void LoadPath(CString path)//path为指定目录此函数的作用为将path目录下的文件插入树控件中
{
	CTreeCtrl& ctrl=GetTreeCtrl();
	ASSERT(ctrl);
	ctrl.DeleteAllItems();
	HTREEITEM hRoot=ctrl.InsertItem(path);
	InsertPath(path,hRoot,ctrl);
	ctrl.Expand(hRoot,TVE_EXPAND);
}


//---11将文件列表中的文件插入树控件中

void InsetAllFile(list<CString>& filePathList)
{
	CTreeCtrl wndTreeCtrl;
	wndTreeCtrl.DeleteAllItems();

	list<CString>::iterator it=filePathList.begin();
	HTREEITEM hRoot=NULL;
	CString filePath;
	CString treeRootName=L"根目录";//所有的文件都在根目录下即：默认所有的文件都在同一个目录下

	while(it!=filePathList.end())
	{
		filePath=*it;

		if(hRoot==NULL)
			hRoot=wndTreeCtrl.InsertItem(treeRootName);//建立根目录

		if(filePath.Find(treeRootName)==0)//文件第一层目录与根目录相同，则截去文件第一层目录，文件从第二层目录开始
			filePath=filePath.Right(filePath.GetLength()-treeRootName.GetLength()-1);


		LoadPicFiles(wndTreeCtrl,filePath,hRoot);

		it++;
	}
}

void LoadPicFiles(CTreeCtrl& wndTreeCtrl,CString nFilePath,HTREEITEM nRoot)
{
	//判断nPicFolder是目录还是文件
	//如果是文件
	//直接将文件插入到树控件中wndTreeCtrl.InsertItem(nPicFolder,nRoot);
	//如果是目录
	//获取nPicFolder的第一层目录
	//判断nRoot目录下是否已经有此层目录
	//如果有此层目录
	//递归插入其他
	//如果无此层目录
	//插入此层目录，然后递归插入其他


	CString nSubFolder;//首层目录
	CString nSubFilePath;//去掉首层目录后的文件名
	BOOL IsExist=FALSE;

	int nIndex=-1;
	nIndex=nFilePath.Find(L'\\');

	if(nIndex>=0)//目录
	{
		nSubFolder=nFilePath.Left(nIndex);
		nSubFilePath=nFilePath.Right(nFilePath.GetLength()-nIndex-1);

		HTREEITEM nSubRoot=NULL;
		if(wndTreeCtrl.ItemHasChildren(nRoot))
			nSubRoot=wndTreeCtrl.GetChildItem(nRoot);
		CString str;
		BOOL bExist=FALSE;
		while(nSubRoot)
		{
			str=wndTreeCtrl.GetItemText(nSubRoot);

			if(str.CompareNoCase(nSubFolder)==0)
			{

				bExist=TRUE;
				break;
			}

			nSubRoot=wndTreeCtrl.GetNextSiblingItem(nSubRoot);
		}

		if(!bExist)
		{

			nSubRoot=wndTreeCtrl.InsertItem(nSubFolder,nRoot);

			LoadPicFiles(wndTreeCtrl,nSubFilePath,nSubRoot);
		}else{
			LoadPicFiles(wndTreeCtrl,nSubFilePath,nSubRoot);
		}
	}
	else if(nFilePath.Find(L".jpg")!=-1||nFilePath.Find(L".tif")!=-1)
	{
		wndTreeCtrl.InsertItem(nFilePath,nRoot);
	}
}



//二扩展操作
//---1响应TVN_ITEMEXPANDING消息时如何获得将要展开或收缩的那一个节点的句柄

			TVN_ITEMEXPANDING pnmtv=(NM_TREEVIEWFAR*)lParam

			typedef struct_NM_TREEVIEW{
				NMHDR hdr;
				UINT action;
				TV_ITEM itemOld;
				TV_ITEM itemNew;
				POINT ptDrag;
		}NM_TREEVIEW;
		typedef NM_TREEVIEWFAR* LPNM_TREEVIEW;

			typedef struct_TV_ITEM
			{tvi
			UINT mask;
		HTREEITEM hItem;
		UINT state;
		UINT stateMask;
		LPSTR pszText;
		int cchTextMax;
		int iImage;
		int iSelectedImage;
		int cChildren;
		LPARAM lParam;}
		TV_ITEM,FAR*LPTV_ITEM;


//在TV_ITEM的hItem中存放着要展开项的句柄
void CLeftView::OnItemexpanding(NMHDR*pNMHDR,LRESULT*pResult)
{
	LPNMTREEVIEW pNMTreeView=reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	//TODO:在此添加控件通知处理程序代
	HTREEITEM htree=pNMTreeView->itemNew.hItem;//这个就是将要被扩展或收缩节点的句柄
}



//2怎么知道CTreeCtrl的一个节点是展开的还是收缩着的
//方法1
GetItemState(hItem,TVIS_EXPANDED)&TVIS_EXPANDED)!=TVIS_EXPANDED//如果相等，则说明改节点是扩展的，如果不相等，则说明该节点是收缩的


//方法2
//响应TVN_ITEMEXPANDING事件时：
void CExampleDlg::OnItemexpandingTree1(NMHDR* pNMHDR,LRESULT* pResult)
{
	NM_TREEVIEW*pNMTreeView=(NM_TREEVIEW*)pNMHDR;

	if(pNMTreeView->action==TVE_COLLAPSE)//判断action的值
}





//---3判断节点是否被扩展过
if((GetTreeCtrl().GetItemState(hItem,TVIS_EXPANDEDONCE)&TVIS_EXPANDEDONCE)!=0)//判断是否扩展过一次，若！=0则说明被扩展过


//---4使用CImageListm_ImageList;加载位图或图标，并将其与树控件联系在一起，由此便可以设置每个节点的图标
    	CImageList m_ImageList;
		m_ImageList.Create(12,12,ILC_COLORDDB|ILC_MASK,3,1);
		HICON hAdd=::LoadIcon(::AfxGetInstanceHandle(),(LPCTSTR)IDI_ADD);
		HICON hRemove=::LoadIcon(::AfxGetInstanceHandle(),(LPCTSTR)IDI_REMOVE);
		HICON hLeaf=::LoadIcon(::AfxGetInstanceHandle(),(LPCTSTR)IDI_LEAF);
		m_ImageList.Add(hAdd);
		m_ImageList.Add(hRemove);
		m_ImageList.Add(hLeaf);
		GetTreeCtrl().SetImageList(&m_ImageList,TVSIL_NORMAL);//树控件和图像列表相连

		m_treeCtrl.SetItemImage(htree,0,0)//通过SetItemImage(htree,0,0)设置节点的图标



//---5什么时候响应OnItemexpanding消息
//当节点第一次被展开时，才响应此消息。也就是说：当以开后该节点再展开或收缩时，便不再响应此消息了。



//6设置树控件形式为TVS_HASBUTTONS|TVS_LINESATROOT时，树控件节点前才会出现+-号
//以下为综合例子：点击按钮上一个显示该节点的上一个兄弟节点，并更改控件焦点
//设置控件样式：
BOOL CTreePathView::PreCreateWindow(CREATESTRUCT& cs)
{
	//TODO:在此处通过修改
	//CREATESTRUCTcs来修改窗口类或样式

	cs.style|=TVS_HASLINES|TVS_SHOWSELALWAYS;//若是想用CImageList的图标，则不要设置为TVS_HASBUTTONS形式


	return CTreeView::PreCreateWindow(cs);
}

//点击按钮5（焦点移动到上一个兄弟节点）
void NewImageView::OnBnClickedButton5()//上一个图
{
	//TODO:在此添加控件通知处理程序代码

	CTreePathView* pTree=(CTreePathView*)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter.GetPane(0,0));

	CTreeCtrl& wndTreeCtrl=pTree->GetTreeCtrl();

	HTREEITEM hItem=wndTreeCtrl.GetSelectedItem();
	if(hItem!=NULL)
	{
		hItem=wndTreeCtrl.GetNextItem(hItem,TVGN_PREVIOUS);

		if(hItem!=NULL)
		{
			CString str;
			str=pTree->GetFullPath(hItem);
			SetImage(str);
			wndTreeCtrl.SelectItem(hItem);
			wndTreeCtrl.SetFocus();
			InvalidateRect(m_ClientRect);
		}
	}
}


//点击按钮6（焦点移动到下一个兄弟节点）
void NewImageView::OnBnClickedButton6()//下一个
{
	//TODO:在此添加控件通知处理程序代码

	CTreePathView* pTree=(CTreePathView*)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter.GetPane(0,0));
	CTreeCtrl&wndTreeCtrl=pTree->GetTreeCtrl();
	HTREEITEM hItem=wndTreeCtrl.GetSelectedItem();

	if(hItem!=NULL)
	{
		hItem=wndTreeCtrl.GetNextItem(hItem,TVGN_NEXT);

		if(hItem!=NULL)
		{
			CString str;
			str=pTree->GetFullPath(hItem);
			SetImage(str);

			wndTreeCtrl.SelectItem(hItem);
			wndTreeCtrl.SetFocus();
			InvalidateRect(m_ClientRect);
		}
	}
}


//---7遍历树控件的所有节点
//---1）获得根节点句柄
CTreeCtrl& wndTreeCtrl=((CImportTreeView*)m_SplitterWnd.GetPane(0,0))->GetTreeCtrl();

		HTREEITEM hItem;
		//获得根目录节点
		hItem=wndTreeCtrl.GetRootItem();
		//遍历树控件节点
		TreeVisit(&wndTreeCtrl,hItem);


//---2）遍历所有节点
void TreeVisit(CTreeCtrl*pCtrl,HTREEITEM hItem)
{
	if(pCtrl->ItemHasChildren(hItem))
	{
		HTREEITEMhChildItem=pCtrl->GetChildItem(hItem);
		while(hChildItem!=NULL)
		{
			TreeVisit(pCtrl,hChildItem);//递归遍历孩子节点
			hChildItem=pCtrl->GetNextItem(hChildItem,TVGN_NEXT);
		}
	}
	else//对叶子节点进行操作
		Leaf(pCtrl,hItem);
}


//---8获得某Item节点的全路径
	CString m_ParentFolder[10];
		CString m_OldParentFolder[10];

			//--------------------将nParent添加到nParentFolder[10]第一位----------------------
BOOL AddParentFolder(CString nParentFolder[10],CString nParent)
{
	for(int i=9;i>0;i--)
		nParentFolder[i]=nParentFolder[i-1];
	nParentFolder[0]=nParent;
	return TRUE;
}

//---------------------nParentFolder[10]中的有效数据整合(加\)---------------------
CString AllCString(CString nParentFolder[10])
{
	CString nAllCString=L"";
	for(int i=0;i<10;i++)
	{
		if(nParentFolder[i]==L"")break;
		nAllCString+=L"\\"+nParentFolder[i];
	}
	return nAllCString;
}


//---获得Item节点路径的函数
CString GetItemPath(CTreeCtrl* pCtrl,HTREEITEM hItem)
{
	CString nSelItemName=pCtrl->GetItemText(hItem);

	HTREEITEM parentItem=pCtrl->GetParentItem(hItem);

	if(parentItem==NULL)//hItem即为根目录
		return nSelItemName;

	//清空OLD
	for(int i=0;i<10;i++)m_OldParentFolder[i]=L"";

	//m_OldParentFolder记录上一个节点的父节点
	for(int i=0;i<10;i++)
		m_OldParentFolder[i]=m_ParentFolder[i];

	//m_ParentFolder记录当前节点的父亲节点
	for(int i=0;i<10;i++)
		m_ParentFolder[i]=L"";

	CString itemPath;
	CString parentFolder=nSelItemName;

	//将parentFolder添加到m_ParentFolder[0],其他值依次后移
	AddParentFolder(m_ParentFolder,parentFolder);



	//m_PicFolder为根节点对应的名字
	while(parentItem!=NULL&&pCtrl->GetItemText(parentItem).Compare(m_PicFolder))
	{
		parentFolder=pCtrl->GetItemText(parentItem);
		AddParentFolder(m_ParentFolder,parentFolder);
		parentItem=pCtrl->GetParentItem(parentItem);

	}

	itemPath.Format(L"%s%s",m_PicFolder,AllCString(m_ParentFolder));

	//清空OLD
	for(int i=0;i<10;i++)m_OldParentFolder[i]=L"";
	//清空
	for(int i=0;i<10;i++)
		m_ParentFolder[i]=L"";

	return itemPath;
}





//获得叶子节点的函数
void Leaf(CTreeCtrl* pCtrl,HTREEITEM hItem)
{

	CString itemName=pCtrl->GetItemText(hItem);

	//叶子节点是jpg文件或tif文件
	if(nSelItemName.Find(L".jpg")!=-1||nSelItemName.Find(L".tif")!=-1)
	{

		//m_OldParentFolder记录上一个节点的父节点
		for(int i=0;i<10;i++)
			m_OldParentFolder[i]=m_ParentFolder[i];

		//m_ParentFolder记录当前节点的父亲节点
		for(int i=0;i<10;i++)
			m_ParentFolder[i]=L"";

		CString imgPath=L"";
		CString parentFolder=itemName;

		//将parentFolder添加到m_ParentFolder[0],其他值依次后移
		AddParentFolder(m_ParentFolder,parentFolder);

		HTREEITEM parentItem=pCtrl->GetParentItem(hItem);

		//m_imgPath为根节点对应的名字
		while(pCtrl->GetItemText(parentItem).Compare(m_imgPath))
		{
			parentFolder=pCtrl->GetItemText(parentItem);
			AddParentFolder(m_ParentFolder,parentFolder);
			parentItem=pCtrl->GetParentItem(parentItem)

		}

		//获得叶子节点的全路径
		imgPath.Format(L"%s%s",m_imgPath,AllCString(m_ParentFolder));

	}


	//对imgPath所指的文件进行操作
	ShowPic(imgPath);
}
			

//使用栈，依次将本节点-->根节点入栈出栈时顺序便为根节点-->本节点
//1）叶子节点
//本地是否存在此文章
void CMainFrame::PostPath(CTreeCtrl& wndTreeCtrl,HTREEITEM hItem,CString& path)
{
	stack<HTREEITEM> itemStack;
	while(hItem!=wndTreeCtrl.GetRootItem())
	{
		itemStack.push(hItem);
		hItem=wndTreeCtrl.GetParentItem(hItem);
	}
	itemStack.push(wndTreeCtrl.GetRootItem());
	CString itemName;
	while(!itemStack.empty())
	{
		hItem=(HTREEITEM)itemStack.top();
		itemStack.pop();
		itemName=wndTreeCtrl.GetItemText(hItem);
		path+=itemName;
		path+=L"\\";
	}
	path.TrimRight(L"\\");
	path+=L".xml";
}

//2）目录节点
void CMainFrame::DirPath(CTreeCtrl& wndTreeCtrl,HTREEITEM nRoot,CString& path)
{
	stack<HTREEITEM>itemStack;
	while(hItem!=wndTreeCtrl.GetRootItem())
	{
		itemStack.push(hItem);
		hItem=wndTreeCtrl.GetParentItem(hItem);
	}
	itemStack.push(wndTreeCtrl.GetRootItem());
	CString itemName;
	while(!itemStack.empty())
	{
		hItem=(HTREEITEM)itemStack.top();
		itemStack.pop();
		itemName=wndTreeCtrl.GetItemText(hItem);
		path+=itemName;
		path+=L"\\";
	}
}



//---9获得树中所有叶子节点的父目录
//即：树中可能有许多枝干，获取这些枝干的路径
std::vector<CString>m_BookDirectory;//存放所有叶子节点的父目录
void GetBookDirectory(CTreeCtrl* pCtrl,HTREEITEM hItem)
{
	if(pCtrl->ItemHasChildren(hItem))
	{
		HTREEITEM hChildItem=pCtrl->GetChildItem(hItem);
		while(hChildItem!=NULL)
		{
			GetBookDirectory(pCtrl,hChildItem);//递归遍历孩子节点

			if(pCtrl->ItemHasChildren(hChildItem))
				hChildItem=pCtrl->GetNextItem(hChildItem,TVGN_NEXT);
			else
				break;
		}
	}
	else
	{
		HTREEITEM parentItem=pCtrl->GetParentItem(hItem);
		CString bookPath=GetItemPath(pCtrl,parentItem);

		m_BookDirectory.push_back(bookPath);

	}
}

CTreeCtrl& wndTreeCtrl=((CImportTreeView*)m_SplitterWnd.GetPane(0,0))->GetTreeCtrl();
HTREEITEM hItem;
hItem=wndTreeCtrl.GetRootItem();

m_BookDirectory.clear();
GetBookDirectory(&wndTreeCtrl,hItem);//获得几本书及书的路径


//---10利用InsertItem、SetItemData存放与该节点有关的数字信息
HTREEITEM InsertItem(
LPCTSTR lpszItem,
int nImage,//实测范围0-65535
int nSelectedImage,
HTREEITEM hParent=TVI_ROOT,
HTREEITEM hInsertAfter=TVI_LAST
);
//存放65535以上的大数据时用SetItemData
BOOL SetItemData(HTREEITEM hItem, DWORD_PTR dwData);

*/