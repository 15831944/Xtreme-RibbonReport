#pragma once


// CDlgMethod �Ի���

class CDlgMethod : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMethod)

public:
	CDlgMethod(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMethod();

// �Ի�������
	enum { IDD = IDD_DLG_METHOD };

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	UINT m_nIDCheck;
};
