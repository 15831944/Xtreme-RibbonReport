// DlgLogin.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RibbonReport.h"
#include "DlgLogin.h"
#include "DlgSetPwd.h"
#include "DlgRegUser.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgLogin �Ի���

CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLogin::IDD, pParent)
	, m_strUser(_T(""))
	, m_strPwd(_T(""))
{
	m_hIcon= AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LOGIN_ET_USER, m_strUser);
	DDX_Text(pDX, IDC_LOGIN_ET_PWD, m_strPwd);
	DDX_Check(pDX, IDC_LOGIN_CK_LOGIN, m_bAutoLogin);
	DDX_Check(pDX, IDC_LOGIN_CK_PWD, m_bRemberPwd);
	DDX_Control(pDX, IDC_LOGIN_BN_USER, m_bnUser);
	DDX_Control(pDX, IDC_LOGIN_BN_PWD, m_bnPwd);
}

BEGIN_MESSAGE_MAP(CDlgLogin, CDialog)
	ON_BN_CLICKED(IDC_LOGIN_BN_USER, &CDlgLogin::OnBnClickedLoginBnUser)
	ON_BN_CLICKED(IDC_LOGIN_BN_PWD, &CDlgLogin::OnBnClickedLoginBnPwd)
	ON_BN_CLICKED(IDC_LOGIN_CK_PWD, &CDlgLogin::OnBnClickedLoginCkPwd)
	ON_BN_CLICKED(IDC_LOGIN_CK_LOGIN, &CDlgLogin::OnBnClickedLoginCkLogin)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLogin ��Ϣ�������
BOOL CDlgLogin::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UpdateData(FALSE);
	return TRUE;
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgLogin::OnBnClickedLoginBnUser()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgRegUser dlg;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_strPwd == dlg.m_strPwd2)
		{
			g_SysUser = dlg.m_strUser;
			g_SysPwd = dlg.m_strPwd2;
			if(AddSysUser())
			{
				((CEdit*)GetDlgItem(IDC_LOGIN_ET_USER))->SetWindowText(g_SysUser);
				((CEdit*)GetDlgItem(IDC_LOGIN_ET_PWD))->SetWindowText(g_SysPwd);
				msgInf(_T("ע��ɹ���"));
			}
			else msgErr(_T("ע��ʧ�ܣ�"));
		}
		else msgErr(_T("�����������벻ͬ��"));
	}
}

void CDlgLogin::OnBnClickedLoginBnPwd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgSetPwd dlg;
	dlg.m_strUser = m_strUser;
	dlg.m_strPwd = m_strPwd;
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
					((CEdit*)GetDlgItem(IDC_LOGIN_ET_USER))->SetWindowText(g_SysUser);
					((CEdit*)GetDlgItem(IDC_LOGIN_ET_PWD))->SetWindowText(g_SysPwd);
					msgInf(_T("�����޸ĳɹ���"));
				}
				else msgErr(_T("�����޸�ʧ�ܣ�"));
			}
			else msgErr(_T("ԭ��������������ͬ��"));
		}
		else msgErr(_T("�����������벻ͬ��"));
	}
}

void CDlgLogin::OnBnClickedLoginCkPwd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	g_SysRemberPwd = m_bRemberPwd ? true: false;
	g_SysAutoLogin = false;
	((CButton*)GetDlgItem(IDC_LOGIN_CK_LOGIN))->SetCheck(FALSE);
}

void CDlgLogin::OnBnClickedLoginCkLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	g_SysRemberPwd = true;
	g_SysAutoLogin = m_bAutoLogin ? true: false;
	((CButton*)GetDlgItem(IDC_LOGIN_CK_PWD))->SetCheck(TRUE);
}

void CDlgLogin::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(TRUE);
	g_SysUser = m_strUser;
	g_SysPwd = m_strPwd;
	g_SysRemberPwd = m_bRemberPwd ? true: false;
	g_SysAutoLogin = m_bAutoLogin ? true: false;
	SetSysStatus(g_SysRemberPwd,g_SysAutoLogin);
	CDialog::OnOK();
}
