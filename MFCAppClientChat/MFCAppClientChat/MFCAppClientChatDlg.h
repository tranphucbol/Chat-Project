
// MFCAppClientChatDlg.h : header file
//

#pragma once
#include "ClientChat.h"
#include <sstream>
#include <thread>
#include <codecvt>
#include <string>

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

														// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};


// CMFCAppClientChatDlg dialog
class CMFCAppClientChatDlg : public CDialogEx
{
// Construction
public:
	CMFCAppClientChatDlg(CWnd* pParent = nullptr);	// standard constructor
	~CMFCAppClientChatDlg();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPCLIENTCHAT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	ClientChat client;
	CEdit port, SendMsg, ip, Username, Password, ChangePassword, SendFile;
	CString AddUser, PathFile, UserSendFile;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void GetOff();
	afx_msg void OnBnClickedConnectbtn();
	afx_msg void OnBnClickedSendbtn();
	void ShowMsg(char msg[]);
	void checkValid(char msg[]);
	afx_msg void OnBnClickedClosebtn();
	thread doR, doThread;
	bool running, runningThread;
	afx_msg void OnBnClickedSignInbtn();
	afx_msg void OnBnClickedSignUpbtn();
	afx_msg void OnBnClickedChangePasswordbtn();
	afx_msg void OnBnClickedSignOutbtn();
	afx_msg void OnBnClickedAdduserbtn();
	void AddRoom(int idRoom);
	void RemoveRoom(int idRoom, int index = -1);
	int Sel;
	afx_msg void OnLbnSelchangeListroom();
	CListBox RecvMsg, ListRoom;
	vector<vector<CString>> Content;
	afx_msg void OnBnClickedSendfilebtn();
	afx_msg void OnBnClickedRemoveuserbtn();
	afx_msg void OnClickedEnter();
};


void Recv(CMFCAppClientChatDlg* Dlg, ClientChat& clientChat);