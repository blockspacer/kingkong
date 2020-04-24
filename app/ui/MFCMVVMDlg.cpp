
// MFCMVVMDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCMVVM.h"
#include "MFCMVVMDlg.h"
#include "afxdialogex.h"
#include <string_util.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()

};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// CMFCMVVMDlg 对话框



CMFCMVVMDlg::CMFCMVVMDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCMVVM_DIALOG, pParent)
	, username_(_T(""))
  , value_(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCMVVMDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDIT_USERNAME, username_);
  DDX_Text(pDX, IDC_EDIT_PASSWD, value_);
}

void CMFCMVVMDlg::OnPropertyChanged(int32_t property_id, const boost::any& before_value, const boost::any& after_value) {
	BEGIN_HANDLE_PROPERTY(property_id)
		HANDLE_PROPERTY(Main::kPropertyUserName, OnUserNameChange);
		HANDLE_PROPERTY(Main::kPropertyPasswd, OnPasswdChange);
	END_HANDLE_PROPERTY()
}

void CMFCMVVMDlg::OnUserNameChange(const boost::any& before_value, const boost::any& after_value) {
	std::wstring wstr_value = BASE_STRING_UTIL::Utf8ToUnicode(boost::any_cast<std::string>(after_value));
	username_ = wstr_value.c_str();
	UpdateData(FALSE);
}

void CMFCMVVMDlg::OnPasswdChange(const boost::any& before_value, const boost::any& after_value) {
	const Main::UserNameParam* value = boost::any_cast<Main::UserNameParam>(&after_value);
	value_ = BASE_STRING_UTIL::Utf8ToUnicode(value->school).c_str();
  UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CMFCMVVMDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT_PASSWD, &CMFCMVVMDlg::OnEnChangeEditPasswd)
	ON_EN_CHANGE(IDC_EDIT_USERNAME, &CMFCMVVMDlg::OnEnChangeEditUsername)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CMFCMVVMDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_LOGOUT, &CMFCMVVMDlg::OnBnClickedButtonLogout)
END_MESSAGE_MAP()


// CMFCMVVMDlg 消息处理程序

BOOL CMFCMVVMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	Application::get_mutable_instance().Init();
	BindVM(kViewModelMain);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCMVVMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCMVVMDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCMVVMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCMVVMDlg::OnEnChangeEditPasswd()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	std::string utf8_value = BASE_STRING_UTIL::UnicodeToUtf8(value_.GetString());
	boost::any abc(utf8_value);
	vm_->NotifyPropertyChanged(Main::kPropertyPasswd, abc);
}


void CMFCMVVMDlg::OnEnChangeEditUsername() {
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 __super::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  UpdateData(TRUE);
  std::string utf8_value = BASE_STRING_UTIL::UnicodeToUtf8(username_.GetString());
	Main::UserNameParam param;
	param.school = "abc";
	vm_->NotifyPropertyChanged(Main::kPropertyUserName, param);
}


void CMFCMVVMDlg::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	vm_->HandleEvent(Main::kEventLogin, nullptr);
}


void CMFCMVVMDlg::OnBnClickedButtonLogout()
{
	// TODO: 在此添加控件通知处理程序代码
	vm_->HandleEvent(Main::kEventLogout, nullptr);
}
