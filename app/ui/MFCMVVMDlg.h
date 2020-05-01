
// MFCMVVMDlg.h: 头文件
//

#pragma once
#include <mvvm/baseview.h>
#include <logic/logic_define.h>
#include <logic/application.h>

// CMFCMVVMDlg 对话框
class CMFCMVVMDlg : public CDialogEx,
	public MVVM_FRAME::BaseView
{
// 构造
public:
	CMFCMVVMDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCMVVM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	void OnPropertyChanged(int32_t property_id, const ::google::protobuf::Message* new_value) override;
	void OnEventFired(int32_t event_id, const ::google::protobuf::Message* value) override;
	
	// 实现
protected:
	void OnUserNameChange(const ::google::protobuf::Message* new_value);
	void OnPasswdChange(const ::google::protobuf::Message* new_value);

	void OnLogin(const ::google::protobuf::Message* value);
	void OnLogout(const ::google::protobuf::Message* value);

	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString username_;
  CString value_;
	afx_msg void OnEnChangeEditPasswd();
	afx_msg void OnEnChangeEditUsername();
  afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonLogout();
};
