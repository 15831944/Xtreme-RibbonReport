// common.h : header file
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

#if !defined(AFX_COMMON_H__97381D7E_4FD5_48BB_AAA0_91D67C0D9FE5__INCLUDED_)
#define AFX_COMMON_H__97381D7E_4FD5_48BB_AAA0_91D67C0D9FE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// common.h 公共变量定义

struct ado_users
{
public:
	CString id;
	CString sysuser;
	CString password;
	CString md5pwd;
	CString cip;
	int cport;
	CString sip;
	int sport;
	int state;
	bool advanced;
	bool remberpwd;
	bool autologin;

	ado_users()
	{
		id = _T("SYS00");
		sysuser = _T("admin");
		password = _T("admin");
		md5pwd = _T("");
		cip = _T("127.0.0.1");
		cport = 51000;
		sip = _T("127.0.0.1");
		sport = 50000;
		state = 12;
		advanced = false;
		remberpwd = false;
		autologin = false;
	}

	ado_users(const ado_users& src)
	{
		id = src.id;
		sysuser = src.sysuser;
		password = src.password;
		md5pwd = src.md5pwd;
		cip = src.cip;
		cport = src.cport;
		sip = src.sip;
		sport = src.sport;
		state = src.state;
		advanced = src.advanced;
		remberpwd = src.remberpwd;
		autologin = src.autologin;
	}

	ado_users(CXTPADORecordset& rst)
	{
		if(rst.GetRecordCount() && rst.GetFieldCount() == 12)
		{
			rst.GetFieldValue(0,id);
			rst.GetFieldValue(1,sysuser);
			rst.GetFieldValue(2,password);
			rst.GetFieldValue(3,md5pwd);
			rst.GetFieldValue(4,cip);
			rst.GetFieldValue(5,cport);
			rst.GetFieldValue(6,sip);
			rst.GetFieldValue(7,sport);
			rst.GetFieldValue(8,state);
			rst.GetFieldValue(9,advanced);
			rst.GetFieldValue(10,remberpwd);
			rst.GetFieldValue(11,autologin);
		}
	}

	ado_users& operator = (const ado_users& src)
	{
		id = src.id;
		sysuser = src.sysuser;
		password = src.password;
		md5pwd = src.md5pwd;
		cip = src.cip;
		cport = src.cport;
		sip = src.sip;
		sport = src.sport;
		state = src.state;
		advanced = src.advanced;
		remberpwd = src.remberpwd;
		autologin = src.autologin;
		return *this;
	}

	ado_users& operator = (CXTPADORecordset& rst)
	{
		if(rst.GetRecordCount() && rst.GetFieldCount() == 12)
		{
			rst.GetFieldValue(0,id);
			rst.GetFieldValue(1,sysuser);
			rst.GetFieldValue(2,password);
			rst.GetFieldValue(3,md5pwd);
			rst.GetFieldValue(4,cip);
			rst.GetFieldValue(5,cport);
			rst.GetFieldValue(6,sip);
			rst.GetFieldValue(7,sport);
			rst.GetFieldValue(8,state);
			rst.GetFieldValue(9,advanced);
			rst.GetFieldValue(10,remberpwd);
			rst.GetFieldValue(11,autologin);
		}

		return *this;
	}
};

struct xls_variant
{
public:
	int row,col;
	int id;
	bool valid;
	VARIANT val;

	xls_variant()
	{
		row=0;
		col=0;
		id=0;
		valid=false;
		val.vt=VT_EMPTY;
	}

	xls_variant(VARIANT _val, int _id=0, int _row=0,int _col=0,bool _valid=true)
	{
		row=_row;
		col=_col;
		id=_id;
		valid=_valid;
		val=_val;
	}

	xls_variant(const xls_variant& src)
	{
		row=src.row;
		col=src.col;
		id=src.id;
		valid=src.valid;
		val=src.val;
	}

	xls_variant& operator = (const xls_variant& src)
	{
		row=src.row;
		col=src.col;
		id=src.id;
		valid=src.valid;
		val=src.val;
		return *this;
	}

	bool operator<(const xls_variant& src) const
	{
		if (id < src.id)
			return true;

		return false;
	}

	bool operator>(const xls_variant& src) const
	{
		if (id > src.id)
			return true;

		return false;
	}

	bool operator==(const xls_variant& src) const
	{
		if (id == src.id)
			return true;

		return false;
	}

	bool operator<=(const xls_variant& src) const
	{
		if (id <= src.id)
			return true;

		return false;
	}

	bool operator>=(const xls_variant& src) const
	{
		if (id >= src.id)
			return true;

		return false;
	}

	bool operator!=(const xls_variant& src) const
	{
		if (id != src.id)
			return true;

		return false;
	}
};

struct xls_int
{
public:
	int row,col;
	int id;
	bool valid;
	int val;

	xls_int()
	{
		row=0;
		col=0;
		id=0;
		valid=false;
		val=0;
	}

	xls_int(int _val, int _id=0, int _row=0,int _col=0,bool _valid=true)
	{
		row=_row;
		col=_col;
		id=_id;
		valid=false;
		val=_val;
	}

	xls_int(const xls_int& src)
	{
		row=src.row;
		col=src.col;
		id=src.id;
		valid=src.valid;
		val=src.val;
	}

	xls_int& operator = (const xls_int& src)
	{
		row=src.row;
		col=src.col;
		id=src.id;
		valid=src.valid;
		val=src.val;
		return *this;
	}

	bool operator<(const xls_int& src) const
	{
		if (val < src.val)
			return true;

		return false;
	}

	bool operator>(const xls_int& src) const
	{
		if (val > src.val)
			return true;

		return false;
	}

	bool operator==(const xls_int& src) const
	{
		if (val == src.val)
			return true;

		return false;
	}

	bool operator<=(const xls_int& src) const
	{
		if (val <= src.val)
			return true;

		return false;
	}

	bool operator>=(const xls_int& src) const
	{
		if (val >= src.val)
			return true;

		return false;
	}

	bool operator!=(const xls_int& src) const
	{
		if (val != src.val)
			return true;

		return false;
	}
};

struct xls_double
{
public:
	int row,col;
	int id;
	bool valid;
	double val;

	xls_double()
	{
		row=0;
		col=0;
		id=0;
		valid=false;
		val=0;
	}

	xls_double(double _val, int _id=0, int _row=0,int _col=0,bool _valid=true)
	{
		row=_row;
		col=_col;
		id=_id;
		valid=_valid;
		val=_val;
	}

	xls_double(const xls_double& src)
	{
		row=src.row;
		col=src.col;
		id=src.id;
		valid=src.valid;
		val=src.val;
	}

	xls_double& operator = (const xls_double& src)
	{
		row=src.row;
		col=src.col;
		id=src.id;
		valid=src.valid;
		val=src.val;
		return *this;
	}

	bool operator<(const xls_double& src) const
	{
		if (abs(val-src.val) < 0.0000001)
			return false;
		else if(val < src.val)
			return true;

		return false;
	}

	bool operator>(const xls_double& src) const
	{
		if (abs(val-src.val) < 0.0000001)
			return false;
		else if(val > src.val)
			return true;

		return false;
	}

	bool operator==(const xls_double& src) const
	{
		if (abs(val-src.val) < 0.0000001)
			return true;

		return false;
	}

	bool operator<=(const xls_double& src) const
	{
		if (abs(val-src.val) < 0.0000001 || val < src.val)
			return true;

		return false;
	}

	bool operator>=(const xls_double& src) const
	{
		if (abs(val-src.val) < 0.0000001 || val > src.val)
			return true;

		return false;
	}

	bool operator!=(const xls_double& src) const
	{
		if (abs(val-src.val) > 0.0000001)
			return true;

		return false;
	}
};

struct xls_string
{
public:
	int row,col;
	int id;
	bool valid;
	CString val;

	xls_string()
	{
		row=0;
		col=0;
		id=0;
		valid=false;
		val.Empty();
	}

	xls_string(CString _val, int _id=0, int _row=0,int _col=0,bool _valid=true)
	{
		row=_row;
		col=_col;
		id=_id;
		valid=_valid;
		val=_val;
	}

	xls_string(const xls_string& src)
	{
		row=src.row;
		col=src.col;
		id=src.id;
		valid=src.valid;
		val=src.val;
	}

	xls_string& operator = (const xls_string& src)
	{
		row=src.row;
		col=src.col;
		id=src.id;
		valid=src.valid;
		val=src.val;
		return *this;
	}

	bool operator<(const xls_string& src) const
	{
		return val.Compare(src.val) == -1;
	}

	bool operator>(const xls_string& src) const
	{
		return val.Compare(src.val) == 1;
	}

	bool operator==(const xls_string& src) const
	{
		return val.Compare(src.val) == 0;
	}

	bool operator<=(const xls_string& src) const
	{
		return val.Compare(src.val) < 1;
	}

	bool operator>=(const xls_string& src) const
	{
		return val.Compare(src.val) > -1;
	}

	bool operator!=(const xls_string& src) const
	{
		return val.Compare(src.val) != 0;
	}
};

struct xls_datetime
{
public:
	int row,col;
	int id;
	bool valid;
	COleDateTime val;

	xls_datetime()
	{
		row=0;
		col=0;
		id=0;
		valid=false;
		val=COleDateTime::GetCurrentTime();
	}

	xls_datetime(COleDateTime _val, int _id=0, int _row=0,int _col=0,bool _valid=true)
	{
		row=_row;
		col=_col;
		id=_id;
		valid=_valid;
		val=_val;
	}

	xls_datetime(const xls_datetime& src)
	{
		row=src.row;
		col=src.col;
		id=src.id;
		valid=src.valid;
		val=src.val;
	}

	xls_datetime& operator = (const xls_datetime& src)
	{
		row=src.row;
		col=src.col;
		id=src.id;
		valid=src.valid;
		val=src.val;
		return *this;
	}

	bool operator<(const xls_datetime& src) const
	{
		return val < src.val;
	}

	bool operator>(const xls_datetime& src) const
	{
		return val > src.val;
	}

	bool operator==(const xls_datetime& src) const
	{
		return val == src.val;
	}

	bool operator<=(const xls_datetime& src) const
	{
		return val <= src.val;
	}

	bool operator>=(const xls_datetime& src) const
	{
		return val >= src.val;
	}

	bool operator!=(const xls_datetime& src) const
	{
		return val != src.val;
	}
};

struct xls_constlog
{
public:
	int row,col;
	int id;
	bool valid;
	COleDateTime val;
	CString work,posi,unit;
	double qty;

	xls_constlog()
	{
		row=0;
		col=0;
		id=0;
		valid=false;
		val=COleDateTime::GetCurrentTime();
		posi.Empty();
		unit.Empty();
		qty=0;
	}

	xls_constlog(COleDateTime _val, int _row,int _col,CString _work,CString _posi,CString _unit=_T(""),double _qty=0,int _id=-1,bool _valid=true)
	{
		row=_row;
		col=_col;
		id=_id;
		valid=_valid;
		val=_val;
		work=_work;
		posi=_posi;
		unit=_unit;
		qty=_qty;
	}

	xls_constlog(const xls_constlog& src)
	{
		row=src.row;
		col=src.col;
		id=src.id;
		valid=src.valid;
		val=src.val;
		work=src.work;
		posi=src.posi;
		unit=src.unit;
		qty=src.qty;
	}

	xls_constlog& operator = (const xls_constlog& src)
	{
		row=src.row;
		col=src.col;
		id=src.id;
		valid=src.valid;
		val=src.val;
		work=src.work;
		posi=src.posi;
		unit=src.unit;
		qty=src.qty;
		return *this;
	}

	bool operator<(const xls_constlog& src) const
	{
		return val < src.val;
	}

	bool operator>(const xls_constlog& src) const
	{
		return val > src.val;
	}

	bool operator==(const xls_constlog& src) const
	{
		return val == src.val;
	}

	bool operator<=(const xls_constlog& src) const
	{
		return val <= src.val;
	}

	bool operator>=(const xls_constlog& src) const
	{
		return val >= src.val;
	}

	bool operator!=(const xls_constlog& src) const
	{
		return val != src.val;
	}
};

struct xls_logconst
{
public:
	int row,col;
	int id;
	bool valid;
	CString val;
	CString work,posi,unit;
	double qty;

	xls_logconst()
	{
		row=0;
		col=0;
		id=0;
		valid=false;
		val.Empty();
		work.Empty();
		posi.Empty();
		unit.Empty();
		qty=0;
	}

	xls_logconst(CString _val, int _row,int _col,CString _work,CString _posi,CString _unit=_T(""),double _qty=0,int _id=-1,bool _valid=true)
	{
		row=_row;
		col=_col;
		id=_id;
		valid=_valid;
		val=_val;
		posi=_posi;
		work=_work;
		unit=_unit;
		qty=_qty;
	}

	xls_logconst(const xls_logconst& src)
	{
		row=src.row;
		col=src.col;
		id=src.id;
		valid=src.valid;
		val=src.val;
		work=src.work;
		posi=src.posi;
		unit=src.unit;
		qty=src.qty;
	}

	xls_logconst& operator = (const xls_logconst& src)
	{
		row=src.row;
		col=src.col;
		id=src.id;
		valid=src.valid;
		val=src.val;
		work=src.work;
		posi=src.posi;
		unit=src.unit;
		qty=src.qty;
		return *this;
	}

	bool operator<(const xls_logconst& src) const
	{
		return val.Compare(src.val) == -1;
	}

	bool operator>(const xls_logconst& src) const
	{
		return val.Compare(src.val) == 1;
	}

	bool operator==(const xls_logconst& src) const
	{
		return val.Compare(src.val) == 0;
	}

	bool operator<=(const xls_logconst& src) const
	{
		return val.Compare(src.val) < 1;
	}

	bool operator>=(const xls_logconst& src) const
	{
		return val.Compare(src.val) > -1;
	}

	bool operator!=(const xls_logconst& src) const
	{
		return val.Compare(src.val) != 0;
	}
};

extern CString g_ServerIp;
extern CString g_SysUid;
extern CString g_SysUser;
extern CString g_SysPwd;
extern bool g_SysRemberPwd;
extern bool g_SysAutoLogin;
extern CString g_DataBase;
extern CString g_TablePrix;
extern CString g_CurDirectory;
extern CString g_RootDirectory;

extern void InitCommon();
extern const char* ToColumnLabel(int col);
extern const char* ToRangeLabel(long row_begin, int col_begin, long row_end, int col_end);
extern double Round(double fA, int nB = 3);
extern BOOL Bound(double fA, double fB, double fC, int nType = 0);
extern void ArraySort(xtp_vector<xls_variant>& arr, int nSortType=1);
extern int ArraySearch(xtp_vector<xls_variant>& arr,int id);
extern int ArrayInsert(xtp_vector<xls_variant>& arr,xls_variant val);
//1、K#+###.###  2、+###.###  3、K#  4、+#  5、K#+### 6、K#+###.##  7、K#+###.#  8、+###.##  9、+###.#
extern CString FormatStat(double fStat, int mode = 1, CString prefix = _T("D"));
extern CString ToString(double fValue, int nPrec = 3);
extern CString NumToBinary(int nNum);
extern CString GetCurPath();
extern BOOL FileExist(CString strFilePath);
extern BOOL DirExist(CString strDirPath);
extern BOOL ModifyDir(CString strDirPath, BOOL bCreate=TRUE);
extern void CreateArray(CStringArray& array, LPCTSTR lpszList, char ch = ';');
extern CString LoadResourceString(UINT nID);
extern CString GetDefineID(UINT nID);
extern HINSTANCE ShellExecuteOpen(CXTPString fileName, CXTPString exeName = "");
extern CXTPString EnCodeStr(CXTPString enCode);
extern CXTPString DeCodeStr(CXTPString deCode);
extern CXTPString RegGetString(HKEY hKey, const xtpchar* subKey, const xtpchar* name);
extern int RegPutString(HKEY hKey, const xtpchar* subKey, const xtpchar* name, const xtpchar* val);
extern int RegGetInt(HKEY hKey, const xtpchar* subKey, const xtpchar* name);
extern int RegPutInt(HKEY hKey, const xtpchar* subKey, const xtpchar* name, int val);
extern CXTPString CpuSerial();
extern CXTPString DiskSerial();
extern CXTPString MacAddress();
extern CXTPString SnKey(CXTPString SnCode);

//数据库处理
extern BOOL DataBackup(CString data, CString path);
extern BOOL DataRestore(CString data, CString path);
extern int DataClear();

//用户信息
extern int Login();
extern int FindSysUid(ado_users& src);
extern int SetSysStatus(bool remberpwd = true, bool autologin = false, bool advanced = false, bool stat = true);
extern int AddSysUser();
extern int SetSysPwd();
extern int DelSysUser();

// 获取数据信息
extern BOOL ExistsTable(CString table);
extern long CountField(CString table, CString field, CString cond1, CString cond2=_T(""));
extern XTPREPORTMSADODB::_RecordsetPtr GetRecordset(CString table, CString sql = _T(""));
extern void RequeryRecordset(XTPREPORTMSADODB::_RecordsetPtr rst);
extern void ResyncRecordset(XTPREPORTMSADODB::_RecordsetPtr rst);
extern COleVariant GetAutoCode(CString table);
extern CString GetFieldName(CString table, int idx = 1);
extern xtp_vector<CString> GetTableName(CXTPADOConnection con);
extern xtp_vector<CString> GetFieldName(CXTPADOConnection con, CString table);
extern CString TableToView(CString table);
extern CString ViewToTable(CString view);
extern double SumTable(CString table, int idx);
extern int AddRecordset(CString table);
extern int AddRecordset(CString table, CString& code);
extern void ResetCode(CString table);
extern void DropTable(CString table);
extern BOOL CopyTable(CString tblOld, CString tblNew);
extern int DelRecordset(CString table, CString val);
extern void SetRecordset(CString table, CXTPADOData data);
extern int SetRecordset(CString table, CString fid, CString code, COleVariant val);
extern void GetRecord(CXTPADOData& data, CString table, CString fids);
extern void GetRecord(CXTPADOData& data, CString table, CString code, CString fids);
extern void GetRecord(CXTPADOData& data, CString table, CString code1, CString code2, CString fids);
extern void SumRecord(CXTPADOData& data, CString tbl, int idx, CString*& fids);
extern void FindRecord(CXTPADOData& data, CString tbl, CString strFind, int nSearchDirection);

extern void LogDebug(CString debug);
extern void LogDebugExp(CString debug, CException *e);
extern void SetConfig(CString config);
extern void LogInfo(CString info);
extern void LogInfoExp(CString info, CException *e);
extern void LogWarn(CString warn);
extern void LogWarnExp(CString warn, CException *e);
extern void LogError(CString error);
extern void LogErrorExp(CString error, CException *e);
extern void LogFatal(CString fatal);
extern void LogFatalExp(CString fatal, CException *e);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMON_H__97381D7E_4FD5_48BB_AAA0_91D67C0D9FE5__INCLUDED_)
