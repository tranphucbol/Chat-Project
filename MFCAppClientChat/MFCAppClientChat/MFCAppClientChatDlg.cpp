
// MFCAppClientChatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCAppClientChat.h"
#include "MFCAppClientChatDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CMFCAppClientChatDlg dialog



CMFCAppClientChatDlg::CMFCAppClientChatDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPCLIENTCHAT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCAppClientChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IP, ip);
	DDX_Control(pDX, IDC_PORT, port);
	DDX_Control(pDX, IDC_SENDMSG, SendMsg);
	DDX_Control(pDX, IDC_RECVMSG, RecvMsg);
	DDX_Control(pDX, IDC_USERNAME, Username);
	DDX_Control(pDX, IDC_PASSWORD, Password);
	DDX_Control(pDX, IDC_NEWPASS, ChangePassword);
	DDX_Text(pDX, IDC_ADDUSER, AddUser);
	DDX_Control(pDX, IDC_LISTROOM, ListRoom);
	DDX_Text(pDX, IDC_PATHFILE, PathFile);
	DDX_Text(pDX, IDC_USERSENDFILE, UserSendFile);
}

BEGIN_MESSAGE_MAP(CMFCAppClientChatDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCAppClientChatDlg::OnClickedEnter)
	ON_BN_CLICKED(IDC_CONNECTBTN, &CMFCAppClientChatDlg::OnBnClickedConnectbtn)
	ON_BN_CLICKED(IDC_SENDBTN, &CMFCAppClientChatDlg::OnBnClickedSendbtn)
	ON_BN_CLICKED(IDC_CLOSEBTN, &CMFCAppClientChatDlg::OnBnClickedClosebtn)
	ON_BN_CLICKED(IDC_SIGNIN, &CMFCAppClientChatDlg::OnBnClickedSignInbtn)
	ON_BN_CLICKED(IDC_SIGNUP, &CMFCAppClientChatDlg::OnBnClickedSignUpbtn)
	ON_BN_CLICKED(IDC_CHANGEPASSWORD, &CMFCAppClientChatDlg::OnBnClickedChangePasswordbtn)
	ON_BN_CLICKED(IDC_SIGNOUT, &CMFCAppClientChatDlg::OnBnClickedSignOutbtn)
	ON_BN_CLICKED(IDC_ADDUSERBTN, &CMFCAppClientChatDlg::OnBnClickedAdduserbtn)
	ON_LBN_SELCHANGE(IDC_LISTROOM, &CMFCAppClientChatDlg::OnLbnSelchangeListroom)
	ON_BN_CLICKED(IDC_SENDFILE, &CMFCAppClientChatDlg::OnBnClickedSendfilebtn)
	ON_BN_CLICKED(IDC_REMOVEUSERBTN, &CMFCAppClientChatDlg::OnBnClickedRemoveuserbtn)
END_MESSAGE_MAP()


// CMFCAppClientChatDlg message handlers

BOOL CMFCAppClientChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	ip.SetWindowTextW(_T("127.0.0.1"));
	port.SetWindowTextW(_T("54000"));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCAppClientChatDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCAppClientChatDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCAppClientChatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCAppClientChatDlg::checkValid(char msg[]) {

	if (msg[strlen(msg) - 1] == '1') {
		client.valid = true;
		if (msg[strlen(msg) - 3] == 'd') {
			// SIGN IN
			if (msg[strlen(msg) - 2] == 'n')
			{
				AfxMessageBox(_T("Sign in successfully. Here we go :)"));
			}
			else if (msg[strlen(msg) - 2] == 'k')
			{
				AfxMessageBox(_T("Sign up successfully. Let's start :)"));
			}
			else if (msg[strlen(msg) - 2] == 'm')
			{
				AfxMessageBox(_T("Change your password successfully. Keeping secret!"));
			}
			msg[strlen(msg) - 1] = 0;
			msg[strlen(msg) - 2] = 0;
			msg[strlen(msg) - 3] = 0;
		}
	}
	else if (msg[strlen(msg) - 1] == '0') {
		client.valid = false;
		if (msg[strlen(msg) - 3] == 'd') {
			// SIGN IN
			if (msg[strlen(msg) - 2] == 'n')
			{
				AfxMessageBox(_T("Sign in unsuccessfully. Username or password are wrong OR this account is being online!"));
			}
			else if (msg[strlen(msg) - 2] == 'k')
			{
				AfxMessageBox(_T("Sign up unsuccessfully. Username already exists!"));
			}
			else if (msg[strlen(msg) - 2] == 'm')
			{
				AfxMessageBox(_T("Change your pass word unsuccessfully. Please try again!"));
			}
			msg[strlen(msg) - 1] = 0;
			msg[strlen(msg) - 2] = 0;
			msg[strlen(msg) - 3] = 0;
		}
	}
}

void CMFCAppClientChatDlg::ShowMsg(char msg[]) {
	// Check Valid
	checkValid(msg);
	if (msg[0] == 0)	// Just annoucement code
		return;

	//ep kieu char* to CString
	wchar_t wtext[4096];
	mbstowcs(wtext, msg, strlen(msg) + 1);//Plus null
	LPWSTR ptr = wtext;

	RecvMsg.AddString(ptr);
}

string splitFileName(string pathfile) {
	int index = -1;
	for (int i = pathfile.length() - 1; i >= 0; i--) {
		if (pathfile[i] == (char)47) {
			index = i;
			cout << index << endl;
			break;
		}
	}
	return pathfile.substr(index + 1, pathfile.length() - (index + 1));
}

void Recv(CMFCAppClientChatDlg* Dlg, ClientChat& clientChat) {
	while (Dlg->running) {
		char* buf = new char[clientChat.size];
		ZeroMemory(buf, clientChat.size);
		int len = recv(clientChat.client, buf, clientChat.size, 0);
		if (len <= 0) {
			Dlg->running = false;
			break;
		}
		buf[len] = 0;

		if (buf[len - 4] == 'n' && buf[len - 3] == 'o' && buf[len - 2] == 't') {
			AfxMessageBox(CString(buf, len - 4));
		}
		else if (buf[len - 3] == 'r') {
			int idRoom = decryptIdRoom(buf, len);

			//check if client have this room or not?
			int index = -1;
			for (int i = 1; i < clientChat.room.size(); i++) {
				if (idRoom == clientChat.room[i]) {
					index = i;
					break;
				}
			}
			if (index == -1) {
				//Create room
				bool isExistEmptyRoom = false;
				int idRoom = decryptIdRoom(buf, len);
				for (int i = 0; i < clientChat.room.size(); i++)
					if (clientChat.room[i] == -1) {
						isExistEmptyRoom = true;
						clientChat.room[i] = idRoom;
					}
				if (!isExistEmptyRoom)
					clientChat.room.push_back(idRoom);
				Dlg->AddRoom(idRoom);
			}
			else {
				//send message to room
				buf[len - 3] = 0;

				string str(buf, len - 3);

				int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
				wstring wstrTo(size_needed, 0);
				MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);

				if (Dlg->Sel == index) {
					Dlg->RecvMsg.AddString(wstrTo.c_str());
				}
				else {
					Dlg->Content[index].push_back(wstrTo.c_str());
				}
			}
		}
		else if (buf[len - 3] == 'c') {
			int idRoom = decryptIdRoom(buf, 3);
			int index = -1;
			for (int i = 0; i < clientChat.room.size(); i++) {
				if (clientChat.room[i] == idRoom) {
					index = i;
					break;
				}
			}
			if (index == -1) {
				AfxMessageBox(_T("Not eixst room to remove"));
			}
			else {
				clientChat.room[index] = -1;
				Dlg->RemoveRoom(idRoom);
			}
		}
		//Send File
		else if (buf[len - 4] == 's' && buf[len - 3] == 'u' && buf[len - 2] == 'o') {
			if (clientChat.sendf != NULL)
				clientChat.sendf->StateUserOnline = (buf[0] == '1') ? true : false;
		}
		else if (buf[len - 4] == 'n' && buf[len - 3] == 'o' && buf[len - 2] == 'k') {
			if (clientChat.sendf != NULL)
				clientChat.sendf->StateSendFile = true;
		}
		else if (buf[len - 4] == 'n' && buf[len - 3] == 'o' && buf[len - 2] == 'f') {
			clientChat.sendf = new SendFile();
			string filename(buf, strlen(buf) - 3);
			clientChat.sendf->filename = filename;
		}
		else if (buf[len - 4] == 's' && buf[len - 3] == 'o' && buf[len - 2] == 'f') {
			string tmp(buf, strlen(buf) - 3);
			int size = atoi(tmp.c_str());
			clientChat.sendf->size = size;
			clientChat.sendf->CreateContent();
			clientChat.size = SIZE + 6;
			clientChat.sendf->StateSendFile = true;
		}
		else if (buf[0] == 'b' && buf[1] == 'o' && buf[2] == 'f' && clientChat.sendf->StateSendFile) {
			bool finished = clientChat.sendf->writefile(buf);
			if (finished) {
				clientChat.size = 4096;
				delete clientChat.sendf;
				clientChat.sendf = NULL;
			}
		}
		//End Send File
		else {
			Dlg->ShowMsg(buf);
		}
	}
}

void DoThread(CMFCAppClientChatDlg* Dlg) {
	while (Dlg->runningThread) {
		if (!Dlg->running)
			Dlg->doR.join();
	}
}

void CMFCAppClientChatDlg::OnBnClickedConnectbtn()
{
	//port ip
	CString ipA, portA;
	ip.GetWindowTextW(ipA);
	string ipStd = CT2A(ipA.GetString());
	port.GetWindowTextW(portA);
	string portAStd = CT2A(portA.GetString());
	int portStd = atoi(portAStd.c_str());

	if (client.StartConnect(ipStd, portStd)) {
		running = true;
		doR = thread(Recv, this, ref(client));
		//doThread = thread(DoThread, this);
		//add new room
		ListRoom.AddString(_T("New Room"));
		ListRoom.SetCurSel(0);
		Sel = 0;
		vector<CString> tmp;
		Content.push_back(tmp);
		client.room.push_back(-2);
	}
}


void CMFCAppClientChatDlg::OnBnClickedSendbtn()
{
	//check if select == "New Room", prevent
	CString slt;
	int index = ListRoom.GetCurSel();
	ListRoom.GetText(index, slt);
	if (slt == "New Room") {
		AfxMessageBox(_T("Please choose a client to send!"));
	}
	else {
		CString msg((client.name + ": ").c_str());
		CString tmp;
		SendMsg.GetWindowTextW(tmp);
		SendMsg.SetWindowTextW(_T(""));
		msg.Append(tmp);

		RecvMsg.AddString(msg);

		wchar_t* utf16_string = msg.GetBuffer();
		wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
		string str = convert.to_bytes(utf16_string);

		//ep kieu CString to char*
		char buf[4096];
		strcpy(buf, str.c_str());

		//format msg id room
		char format[4] = "r00";

		//detect id room
		char room[20];
		wcstombs(room, slt.GetBuffer(), 20);
		string roomStr(room);
		roomStr = roomStr.substr(5, roomStr.length() - 5);
		encryptIdRoom(format, atoi(roomStr.c_str()));

		int len = strlen(buf);
		buf[len] = format[0];
		buf[len + 1] = format[1];
		buf[len + 2] = format[2];

		client.SendMess(buf, len + 3);
	}
}


void CMFCAppClientChatDlg::OnBnClickedClosebtn()
{
	doR.join();
}


void CMFCAppClientChatDlg::OnBnClickedSignInbtn()
{
	if (client.valid == true) {
		AfxMessageBox(_T("You have to sign out before signing in!"));
		return;
	}

	//Name 
	CString CStrName;
	Username.GetWindowTextW(CStrName);
	char name[20];
	wcstombs(name, CStrName.GetBuffer(), 20);
	client.name = name;
	//Pass
	CString CStrPass;
	Password.GetWindowTextW(CStrPass);
	char pass[21];
	wcstombs(pass, CStrPass.GetBuffer(), 21);
	client.pass = pass;

	// SEND TO SERVER
	client.SendMess(encryptMsgLogin(name, pass));

	// RECV SIGNAL CHECKING FROM SERVER
}


void CMFCAppClientChatDlg::OnBnClickedSignUpbtn()
{
	if (client.valid == true) {
		AfxMessageBox(_T("You have to sign out before signing up!"));
		return;
	}

	//Name 
	CString CStrName;
	Username.GetWindowTextW(CStrName);
	char name[20];
	wcstombs(name, CStrName.GetBuffer(), 20);
	client.name = name;


	//Pass
	CString CStrPass;
	Password.GetWindowTextW(CStrPass);
	char pass[21];
	wcstombs(pass, CStrPass.GetBuffer(), 21);
	client.pass = pass;

	//SEND TO SERVER
	client.SendMess(encryptMsgSignUp(name, pass));
	// RECV SIGNAL CHECKING FROM SERVER
}


void CMFCAppClientChatDlg::OnBnClickedChangePasswordbtn()
{
	if (client.valid == false) {
		AfxMessageBox(_T("You have to sign out or sign in before changing password!"));
		return;
	}
	CString CStrNPass;
	ChangePassword.GetWindowTextW(CStrNPass);
	char npass[21];
	wcstombs(npass, CStrNPass.GetBuffer(), 21);
	client.pass = npass;

	// SEND TO SERVER
	client.SendMess(encryptMsgChangeP(client.name, client.pass));
}


void CMFCAppClientChatDlg::OnBnClickedSignOutbtn()
{
	if (client.valid == true) {
		client.SendMess(client.name + "dx1");
		client.name = "";
		client.pass = "";
		client.valid = false;
		AfxMessageBox(_T("Sign out successfully. Good bye!"));
	}
	else {
		AfxMessageBox(_T("You have to sign in before signing out!"));
	}
}


void CMFCAppClientChatDlg::OnBnClickedAdduserbtn()
{
	CString uname;
	UpdateData();
	uname = AddUser;
	UpdateData(FALSE);

	char format[4] = "u00";

	CString slt;
	int index = ListRoom.GetCurSel();
	ListRoom.GetText(index, slt);

	if (slt == "New Room") {
		format[1] = 255;
		format[2] = 255;
	}
	else {
		char room[20];
		wcstombs(room, slt.GetBuffer(), 20);
		string roomStr(room);
		roomStr = roomStr.substr(5, roomStr.length() - 5);
		encryptIdRoom(format, atoi(roomStr.c_str()));
	}

	char buf[24];
	wcstombs(buf, uname.GetBuffer(), 24);
	int len = strlen(buf);
	//strcat(buf, format);
	buf[len] = format[0];
	buf[len + 1] = format[1];
	buf[len + 2] = format[2];

	client.SendMess(buf, len + 3);
}

void CMFCAppClientChatDlg::AddRoom(int idRoom) {
	char room[6];
	itoa(idRoom, room, 10);
	CString nameRoom("Room ");
	nameRoom.Append(CString(room, strlen(room)));
	ListRoom.AddString(nameRoom);
	vector<CString> tmp;
	Content.push_back(tmp);
	ListRoom.SetCurSel(ListRoom.GetCount() - 1);
	OnLbnSelchangeListroom();
}


void CMFCAppClientChatDlg::OnLbnSelchangeListroom()
{
	for (int i = Content[Sel].size(); i < RecvMsg.GetCount(); i++) {
		CString tmp;
		RecvMsg.GetText(i, tmp);
		Content[Sel].push_back(tmp);
	}

	RecvMsg.ResetContent();
	Sel = ListRoom.GetCurSel();

	for (int i = 0; i < Content[Sel].size(); i++) {
		CString tmp;
		RecvMsg.AddString(Content[Sel][i]);
	}
}

void CMFCAppClientChatDlg::GetOff() {
	if (client.valid == true) {
		client.SendMess(client.name + "dx1");
		client.name = "";
		client.pass = "";
		client.valid = false;
	}
}

CMFCAppClientChatDlg::~CMFCAppClientChatDlg() {
	if (client.valid == true) {
		client.SendMess(client.name + "dx1");
		client.name = "";
		client.pass = "";
		client.valid = false;
	}
}

void CMFCAppClientChatDlg::OnBnClickedSendfilebtn()
{
	UpdateData();
	UpdateData(false);

	char username[20];
	wcstombs(username, UserSendFile.GetBuffer(), 20);

	char pathfile[100];
	wcstombs(pathfile, PathFile.GetBuffer(), 100);

	if (!client.sendFile(username, pathfile)) {
		AfxMessageBox(_T("Unsuccessfully!"));
	}
	else {
		AfxMessageBox(_T("Successfully!"));
	}
	
}

void CMFCAppClientChatDlg::RemoveRoom(int idRoom, int index) {
	if (index == -1) {
		char room[20] = "Room ";
		char strIdRoom[10];
		itoa(idRoom, strIdRoom, 10);
		strcat(room, strIdRoom);

		index = ListRoom.FindString(0, CString(room, strlen(room)));
	}
	ListRoom.DeleteString(index);
	int count = ListRoom.GetCount();
	if (count == index)
		ListRoom.SetCurSel(count - 1);
	else
		ListRoom.SetCurSel(index);
	OnLbnSelchangeListroom();
}

void CMFCAppClientChatDlg::OnBnClickedRemoveuserbtn()
{
	int index = ListRoom.GetCurSel();
	CString nameRoom;
	ListRoom.GetText(index, nameRoom);
	if (nameRoom == "New Room") {
		AfxMessageBox(_T("You can't cloese the new room, please choose other room!"));
	}
	else {
		client.room[index] = -1;
		RemoveRoom(0, index);
		//format msg id room
		char format[4] = "c00";

		//detect id room
		char room[20];
		wcstombs(room, nameRoom.GetBuffer(), 20);
		string roomStr(room);
		roomStr = roomStr.substr(5, roomStr.length() - 5);
		encryptIdRoom(format, atoi(roomStr.c_str()));

		client.SendMess(format, 3);
	}
}

void CMFCAppClientChatDlg::OnClickedEnter() {
	OnBnClickedSendbtn();
}