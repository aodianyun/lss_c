// testPublishDlg.cpp : implementation file
//

#include "stdafx.h"
#include "testPublish.h"
#include "testPublishDlg.h"
#include ".\testpublishdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestPublishDlg dialog

CTestPublishDlg::CTestPublishDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestPublishDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestPublishDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestPublishDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_COMBO_VIDEO,m_ComboBoxVideo);
	DDX_Control(pDX,IDC_COMBO_AUDIO,m_ComboBoxAudio);
	DDX_Control(pDX,IDC_COMBO_THIRD_PART_PLAYER,m_ComboBoxThirdPartPlayer);

	DDX_Control(pDX,IDC_LIST_MSG, m_MsgList);
	DDX_Control(pDX,IDC_EDIT_NETINFO,m_PublishNetInfoList);
	DDX_Control(pDX,IDC_EDIT_PCSTATUS,m_PublishPCStatusList);
	DDX_Control(pDX,IDC_SLIDER_VOLUM, m_sliderVolum);
	DDX_Control(pDX,IDC_SLIDER_SHOWSPECTRUM,m_sliderShowSpectrum);

}

BEGIN_MESSAGE_MAP(CTestPublishDlg, CDialog)
	//{{AFX_MSG_MAP(CTestPublishDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_START, OnBnClickedButtonStart)
	ON_MESSAGE(MSG_SHOWMSG,OnShowMsg)
	ON_MESSAGE(MSG_SHOW_PUBLISH_NETINFO,OnShowPublishNetInfo)
	ON_MESSAGE(MSG_SHOW_PUBLISH_PCSTATUS,OnShowPublishPCStauts)
	ON_MESSAGE(WM_HSCROLL,OnHScroll)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_SHOWSPM, OnBnClickedButtonShowspm)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_SIZE, OnBnClickedButtonChangeSize)
	ON_BN_CLICKED(IDC_BUTTON_STEREO, OnBnClickedButtonStereo)
	ON_BN_CLICKED(IDC_BUTTON_MIC, OnBnClickedButtonMic)
END_MESSAGE_MAP()

//MESSAGE_HANDLER(MSG_SHOW_PUBLISH_NETINFO, OnShowPublishNetInfo) 
//MESSAGE_HANDLER(MSG_SHOW_PUBLISH_PCSTATUS, OnShowPublishPCStauts) 
/////////////////////////////////////////////////////////////////////////////
// CTestPublishDlg message handlers

BOOL CTestPublishDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	Init();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestPublishDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestPublishDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestPublishDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

bool CTestPublishDlg::OnRtmpPublishNotifyInJson(const char* pszJson)
{
   return true;
}

bool CTestPublishDlg::OnRtmpPublishManagerMsg(eMsgLevel MsgLev, const char *pMsg)
{
		if(pMsg == NULL)
		return false;
	if(string(pMsg) == "NetStream.Publish.Start")
	{
		/*BOOL BRET;
		OnBnClickedButtonEnableRecord(0,0,0, BRET);*/
	}
	int ShowMsgLev = 0;
	if(MsgLev >= ShowMsgLev)
	{
		/*string strMsg = pMsg;
		ShowMsg(strMsg);*/
		SYSTEMTIME time;
		GetLocalTime(&time);
		char buf[50] = {0};
		sprintf(buf, "%d:%d:%d:%d  ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
		string strMsg = string(buf) + string(pMsg);

		const char *p = new char [strMsg.length()+1];

		if (p)
		{
			memcpy((void *)p, strMsg.c_str(), strMsg.length()+1);
			PostMessage(MSG_SHOWMSG, 0, (LPARAM)p);
		}
	}
	return true;
}

bool CTestPublishDlg::OnRtmpPublishManagerNetInfo(RtmpPublishNetInfo &RtmpNetInfo)
{
	char buf[50]={0};
	sprintf(buf, "%d  ms", RtmpNetInfo.uPing);
	string strNetInfo = string("Ping:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", RtmpNetInfo.uDelay);
	strNetInfo+= string("Delay:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", RtmpNetInfo.uDiscardBytesSum);
	strNetInfo+= string("丢弃字节数:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	//sprintf(buf, "%.3f  kps", (float)RtmpNetInfo.uAudioBitRate*8/1024);
	sprintf(buf, "%.3f  kps", RtmpNetInfo.fAudioBitRate);
	strNetInfo+= string("音频码率:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	//sprintf(buf, "%.3f  kps", (float)RtmpNetInfo.uVideoBitRate*8/1024);
	sprintf(buf, "%.3f  kps", RtmpNetInfo.fVideoBitRate);
	strNetInfo+= string("视频码率:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	//sprintf(buf, "%.3f  kps", ((float)RtmpNetInfo.uVideoBitRate + (float)RtmpNetInfo.uAudioBitRate)*8/1024);
	sprintf(buf, "%.3f  kps", (RtmpNetInfo.fAudioBitRate + RtmpNetInfo.fVideoBitRate));
	strNetInfo+= string("当前码率:")+string(buf)+"\r\n";

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", RtmpNetInfo.uAudioFrameSum);
	strNetInfo+= string("发送音频总帧数:")+string(buf)+"\r\n";

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", RtmpNetInfo.uVideoFrameSum);
	strNetInfo+= string("发送视频总帧数:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", RtmpNetInfo.uVideoFrameRate);
	strNetInfo+= string("视频帧率:")+string(buf)+"\r\n";

	const char *p = new char [strNetInfo.length()+1];
	if (p)
	{
		memcpy((void *)p, strNetInfo.c_str(), strNetInfo.length()+1);
		PostMessage(MSG_SHOW_PUBLISH_NETINFO, 0, (LPARAM)p);
	}
	return true;
}

bool CTestPublishDlg::OnRtmpPublishManagerComputerStatusInfo(RtmpPublishComputerStatusInfo &RtmpComputerStatusInfo)
{
	char buf[500]={0};
	sprintf(buf, "%d", RtmpComputerStatusInfo.uTotalPhys/1000);
	string strNetInfo = string("总内存:")+string(buf)+"M\r\n";
	unsigned int uUsedMem = RtmpComputerStatusInfo.uTotalPhys-RtmpComputerStatusInfo.uAvailPhys;
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%dM(%d%%)", uUsedMem/1000, uUsedMem*100/RtmpComputerStatusInfo.uTotalPhys);
	strNetInfo+= string("已用内存:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", RtmpComputerStatusInfo.uTotalVirtual/1000);
	strNetInfo+= string("总虚拟内存:")+string(buf)+"M\r\n";
	unsigned int uUsedVirtualMem = RtmpComputerStatusInfo.uTotalVirtual - RtmpComputerStatusInfo.uAvailVirtual;
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%dM(%d%%)", uUsedVirtualMem/1000, uUsedVirtualMem*100/RtmpComputerStatusInfo.uTotalVirtual);
	strNetInfo+= string("已用虚拟内存:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%dMHZ", RtmpComputerStatusInfo.uCPUClockSpeed);
	strNetInfo+= string("cpu频率:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", RtmpComputerStatusInfo.uCPUCores);
	strNetInfo+= string("cpu核心数:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", RtmpComputerStatusInfo.uCPUUsage);
	strNetInfo+= string("cpu使用率:")+string(buf)+"%\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%dKB", RtmpComputerStatusInfo.uUpSpeed);
	strNetInfo+= string("上传流量:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%dKB", RtmpComputerStatusInfo.uDownSpeed);
	strNetInfo+= string("下载流量:")+string(buf)+"\r\n";
	const char *p = new char [strNetInfo.length()+1];

	if (p)
	{
		memcpy((void *)p, strNetInfo.c_str(), strNetInfo.length()+1);
		PostMessage(MSG_SHOW_PUBLISH_PCSTATUS, 0, (LPARAM)p);
	}
	return true;
}

void CTestPublishDlg::OnBnClickedButtonStart()
{
	GetPublishPara();
	CheckDlgButton(IDC_CHECK_AUTOCTRLBITRATE, BST_UNCHECKED);
	if(m_pRtmpManage == NULL)
	{
		m_pRtmpManage = CreateRtmpPublishManager(this);
	}

   	if(m_pRtmpManage)
	{
		::MoveWindow(GetDlgItem(IDC_STATIC_DISPLAY_PUBLISH)->m_hWnd, 0, 0, 320, 240, TRUE);

		m_pRtmpManage->SetCam(m_ComboBoxVideo.GetCurSel());
		m_pRtmpManage->SetMic(m_ComboBoxAudio.GetCurSel());
		/*m_pRtmpManage->SetCam(-1);
		m_pRtmpManage->SetMic(-1);*/
		string strCmd;
		m_jsonRoot.clear();
		m_jsonParams.clear();
		m_jsonRoot["cmd"] = Json::Value("SetBframeCount");
		m_jsonParams["BframeCount"] = Json::Value(m_iBFrame);
		m_jsonRoot["params"] = m_jsonParams;
		strCmd = m_jsonRoot.toStyledString();
		char* pszRes = new char[1024];

		m_pRtmpManage->CallInJson(strCmd.c_str(),&pszRes);
		
		//cqp
		strCmd.clear();
		m_jsonRoot.clear();
		m_jsonParams.clear();
		memset(pszRes,0,1024);
		m_jsonRoot["cmd"] = Json::Value("SetVBRQuality");
		m_jsonParams["Quality"] = Json::Value(m_iQuality);
		m_jsonRoot["params"] = m_jsonParams;
		strCmd = m_jsonRoot.toStyledString();

		m_pRtmpManage->CallInJson(strCmd.c_str(),&pszRes);


		m_pRtmpManage->SetPublishAudioPara(m_iAdSampleRate, m_iAdChannels, m_iAdBitPerSample, m_iAdBitRate, m_iAdVolum, m_bAdMute);
		m_pRtmpManage->SetPublishVideoPara(m_iVdWidth, m_iVdHeight, m_iVdFps, m_iVdBitRate, GetDlgItem(IDC_STATIC_DISPLAY_PUBLISH)->m_hWnd, m_iVdInterval);
		string strApp = m_strPublishRoomInfo+"?"+m_strKey;
		m_pRtmpManage->SetPublishStreamPara(m_strScheduleSvr.c_str(), /*m_strPublishRoomInfo*/strApp.c_str(), m_strPublishStageInfo.c_str());
		//m_pRtmpManage->SetPublishOtherPlayerVolum(m_iOtherPlayerVolum, m_bOtherPlayerMute);
		m_pRtmpManage->SetClientUserID(m_strUserID.c_str());
		//m_pRtmpManage->SetRecordFileFullName(m_strFileName.c_str());
		//m_pRtmpManage->EnableRecordFile(true);
		/*BOOL BRET;
		OnBnClickedButtonEnableRecord(0,0,0, BRET);*/
		m_pRtmpManage->StartPublish();
	}
}

void CTestPublishDlg::Init()
{
	m_pRtmpManage = CreateRtmpPublishManager(this);

	string strSdkVersion = GetSdkVersion();

	if (strSdkVersion.empty())
	{	strSdkVersion = "SdkVer:get failed.";	}
	else
	{	strSdkVersion = "SdkVer:" + strSdkVersion;	}
	SetWindowText(strSdkVersion.c_str())	;
	ShowMsg(strSdkVersion);
	wchar_t** ppCam = NULL;
	unsigned int size = 0;
	m_pRtmpManage->GetCamList(ppCam, size);

	for(unsigned int i=0; i<size; i++)
	{
		m_ComboBoxVideo.InsertString(m_ComboBoxVideo.GetCount(),ConvertString::ws2s(wstring(ppCam[i])).c_str());
	}

	wchar_t** ppMic = NULL;
	m_pRtmpManage->GetMicList(ppMic, size);

	for(unsigned int i=0; i<size; i++)
	{
		m_ComboBoxAudio.InsertString(m_ComboBoxAudio.GetCount(), ConvertString::ws2s(wstring(ppMic[i])).c_str());
	}

	m_ComboBoxAudio.SetCurSel(0);
	m_ComboBoxVideo.SetCurSel(0);


	//test
	SetDefaultPara();

	//@ Show default config
	GetDlgItem(IDC_EDIT_SERVER)->SetWindowText(m_strScheduleSvr.c_str());
	GetDlgItem(IDC_EDIT_APP)->SetWindowText(m_strPublishRoomInfo.c_str());
	GetDlgItem(IDC_EDIT_STAGEID)->SetWindowText(m_strPublishStageInfo.c_str());
	GetDlgItem(IDC_EDIT_USERID)->SetWindowText(m_strUserID.c_str());
	GetDlgItem(IDC_EDIT_KEY)->SetWindowText(m_strKey.c_str());

	char buf[30]={'\0'};
	sprintf(buf, "%d", m_iVdWidth);
	string strWidth = buf;
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", m_iVdHeight);
	string strHeight = buf;
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", m_iVdFps);
	string strVdFps = buf;
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", m_iVdBitRate);
	string strVdBitRate = buf;
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", m_iBFrame);
	string strBframe = buf;
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", m_iQuality);
	string strQuality = buf;
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", m_iVdInterval);
	string strVdInterval = buf;

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", m_iAdSampleRate);
	string strAdSampleRate = buf;
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", m_iAdBitRate);
	string strAdBitRate = buf;
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", m_iAdChannels);
	string strAdChannels = buf;
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", m_iAdBitPerSample);
	string strAdBitperSample = buf;



	GetDlgItem(IDC_EDIT_VIDEOWIDTH)->SetWindowText(strWidth.c_str());
	GetDlgItem(IDC_EDIT_VIDEOHEIGHT)->SetWindowText(strHeight.c_str());
	GetDlgItem(IDC_EDIT_VIDEOFRAMERATE)->SetWindowText(strVdFps.c_str());
	GetDlgItem(IDC_EDIT_VIDEOBITRATE)->SetWindowText(strVdBitRate.c_str());
	GetDlgItem(IDC_EDIT_VIDEOKEYFRAMEINTERVAL)->SetWindowText(strVdInterval.c_str());
	GetDlgItem(IDC_EDIT_BFRAME)->SetWindowText(strBframe.c_str());
	GetDlgItem(IDC_EDIT_QUALITY)->SetWindowText(strQuality.c_str());

	GetDlgItem(IDC_EDIT_SAMPLERATE)->SetWindowText(strAdSampleRate.c_str());
	GetDlgItem(IDC_EDIT_CHANNELS)->SetWindowText(strAdChannels.c_str());
	GetDlgItem(IDC_EDIT_BITPERSAMPLE)->SetWindowText(strAdBitperSample.c_str());
	GetDlgItem(IDC_EDIT_AUDIOBITRATE)->SetWindowText(strAdBitRate.c_str());

	m_bCapStereo = false;
	m_bCapMic = true;
	m_sliderVolum.SetPos(m_iAdVolum);

}

void CTestPublishDlg::SetDefaultPara()
{
	//m_strScheduleSvr = "rtmp.aodianyun.com:1936";
	m_strScheduleSvr = "rtmp://lsspublish.aodianyun.com:1935";
	m_strPublishRoomInfo = "app";
	m_strPublishStageInfo = "stream";
	m_strUserID = "10000";
	m_strKey = "yun";

	//@ video paragment
	m_iVdFps = 10;
	m_iVdBitRate = 160;
	m_iVdWidth = 320;
	m_iVdHeight = 240;
	m_iVdQuality = 80;
	m_iVdInterval = 30;
	m_iBFrame = 0;
	m_iQuality = 0;

	//@ audio para
	m_iAdSampleRate = 44100;
	m_iAdBitRate = 32;
	m_iAdChannels = 2;
	m_iAdBitPerSample = 16;
	m_iAdVolum = 80;
	m_bAdMute = false;
	m_bShowSpectrum = true;

}

void CTestPublishDlg::ShowMsg(string &strMsg)
{
	int iLineNum = m_MsgList.GetCount();
	if (iLineNum <= 200) 
	{
		//m_MsgList.AddString(ConvertString::s2ws(strMsg).c_str());
		m_MsgList.InsertString(0, strMsg.c_str());
	} 
	else
	{ 
		//m_MsgList.DeleteString(0); 
		m_MsgList.DeleteString(iLineNum-1);
	}
}

string CTestPublishDlg::GetSdkVersion()
{
	string strCmd;
	string strSdkVersion;
	Json::Value root;
	Json::Value params;
	Json::Reader reader;
	Json::Value resRoot;
	Json::Value data;
	char* pszRes = new char[1024];

	if (pszRes)
	{
		memset(pszRes, 0, 1024);

		root["cmd"] = Json::Value("GetSdkVersion");
		root["params"] = params;
		strCmd = root.toStyledString();

		if (m_pRtmpManage)
		{
			m_pRtmpManage->CallInJson(strCmd.c_str(), &pszRes);
		}

		if (reader.parse(string(pszRes), resRoot))
		{
			int iCode = resRoot["code"].asInt();

			if (0 == iCode)
			{
				Json::Value data = resRoot["data"];

				if (data.isObject())
				{
					strSdkVersion = data["version"].asString();
				}
			}
		}
	}

	return strSdkVersion;
}

void CTestPublishDlg::ShowPublishNetInfo(string &strMsg)
{
	   	/*int nStartChar = 0;
	int nEndChar = 0;
	m_MsgList.GetSel(nStartChar, nEndChar);
	m_MsgList.ReplaceSel(ConvertString::s2ws(strMsg).c_str());*/
	
	 m_PublishNetInfoList.SetSel(0, -1); 
	 m_PublishNetInfoList.Clear(); 
	 int iLineNum = m_PublishNetInfoList.GetLineCount(); 
	 if (iLineNum <= 200) 
	 {
		 m_PublishNetInfoList.SetSel(-1, -1); 
		// m_EditLog.ReplaceSel(str + "\r\n\r\n"); 
		 m_PublishNetInfoList.ReplaceSel((strMsg+"\r\n").c_str());
	 } 
	 else
	 { 
		 m_PublishNetInfoList.SetSel(0, -1); 
		 m_PublishNetInfoList.Clear(); 
	 }
}

void CTestPublishDlg::GetPublishPara()
{
	char buf[100] = {'0'};
	GetDlgItem(IDC_EDIT_SERVER)->GetWindowText(buf, sizeof(buf));
	m_strScheduleSvr =  string(buf);
	GetDlgItem(IDC_EDIT_APP)->GetWindowText(buf, sizeof(buf));
	m_strPublishRoomInfo =  string(buf);
	GetDlgItem(IDC_EDIT_STAGEID)->GetWindowText(buf, sizeof(buf));
	m_strPublishStageInfo =  string(buf);
	GetDlgItem(IDC_EDIT_USERID)->GetWindowText(buf, sizeof(buf));
	m_strUserID =  string(buf);
	GetDlgItem(IDC_EDIT_KEY)->GetWindowText(buf, sizeof(buf));
	m_strKey =  string(buf);

	GetDlgItem(IDC_EDIT_VIDEOWIDTH)->GetWindowText(buf, sizeof(buf));
	m_iVdWidth = atoi(buf);
	GetDlgItem(IDC_EDIT_VIDEOHEIGHT)->GetWindowText(buf, sizeof(buf));
	m_iVdHeight = atoi(buf);
	GetDlgItem(IDC_EDIT_VIDEOFRAMERATE)->GetWindowText(buf, sizeof(buf));
	m_iVdFps = atoi(buf);
	GetDlgItem(IDC_EDIT_VIDEOBITRATE)->GetWindowText(buf, sizeof(buf));
	m_iVdBitRate = atoi(buf) * 1000;
	GetDlgItem(IDC_EDIT_VIDEOKEYFRAMEINTERVAL)->GetWindowText(buf, sizeof(buf));
	m_iVdInterval = atoi(buf);
	GetDlgItem(IDC_EDIT_BFRAME)->GetWindowText(buf,sizeof(buf));
	m_iBFrame = atoi(buf);
	GetDlgItem(IDC_EDIT_QUALITY)->GetWindowText(buf,sizeof(buf));
	m_iQuality = atoi(buf);


	GetDlgItem(IDC_EDIT_SAMPLERATE)->GetWindowText(buf, sizeof(buf));
	m_iAdSampleRate = atoi(buf);
	GetDlgItem(IDC_EDIT_CHANNELS)->GetWindowText(buf, sizeof(buf));
	m_iAdChannels = atoi(buf);
	GetDlgItem(IDC_EDIT_BITPERSAMPLE)->GetWindowText(buf, sizeof(buf));
	m_iAdBitPerSample = atoi(buf);
	GetDlgItem(IDC_EDIT_AUDIOBITRATE)->GetWindowText(buf, sizeof(buf));
	m_iAdBitRate = atoi(buf) * 1000;
}

bool CTestPublishDlg::ParseCmd(const string &strCmd)
{
	string strMsg;
	string strApi;
	Json::Reader reader;
	Json::Value root;
	Json::Value resRoot;
	Json::Value data;

	if (reader.parse(strCmd, root))
	{
		strApi = root["cmd"].asString();

		if (strApi.empty())
		{
			strMsg = "Error:cmd can not be null.";
			ShowMsg(strMsg);
			return false;
		}
		else
		{
			Json::Value objParams = root["params"];

			if (!objParams.isObject())
			{
				strMsg = "Error:params must be object.";
				ShowMsg(strMsg);
				return false;
			}
			else
			{
				if (strApi == "OnAudioPercent")
				{
					int uPercent = objParams["uPercent"].asInt();
					char* pParam = new char[sizeof(int)];	/* pParam will be free in msg function. */

					if (pParam)
					{
						itoa(uPercent, pParam, 10);
						PostMessage(MSG_SHOW_AUDIO_SPECTRUM, 0, (LPARAM)pParam);
					}
					return true;
				}
				else if (strApi == "OnShowVideo")
				{
					return true;
				}
			}
		}
	}

	strMsg = "Error:cmd must be json format.";
	ShowMsg(strMsg);
	return false;
}

void CTestPublishDlg::SetShowSpectrum(bool bShow)
{
	string strCmd;
	m_jsonRoot.clear();
	m_jsonParams.clear();
	m_jsonRoot["cmd"] = Json::Value("SetShowSpectrum");
	m_jsonParams["bShow"] = Json::Value(bShow);
	m_jsonRoot["params"] = m_jsonParams;
	strCmd = m_jsonRoot.toStyledString();
	char* pszRes = new char[1024];

	if (pszRes)
	{
		memset(pszRes, 0, 1024);
		Json::Reader reader;
		Json::Value resRoot;
		Json::Value data;

		if (m_pRtmpManage)
		{
			m_pRtmpManage->CallInJson(strCmd.c_str(), &pszRes);
		}

		if (reader.parse(string(pszRes), resRoot))
		{
			int iCode = resRoot["code"].asInt();

			if (0 == iCode)
			{
			}
		}
	}
}

void CTestPublishDlg::SetAutoCtrlBitrate(bool bAuto)
{
	string strCmd;
	string strSdkVersion;
	Json::Value root;
	Json::Value params;
	Json::Reader reader;
	Json::Value resRoot;
	Json::Value data;
	char* pszRes = new char[1024];

	if (pszRes)
	{
		memset(pszRes, 0, 1024);

		root["cmd"] = Json::Value("SetAutoCtrlBitrate");
		params["bAuto"] = Json::Value(bAuto);
		root["params"] = params;
		strCmd = root.toStyledString();

		if (m_pRtmpManage)
		{
			m_pRtmpManage->CallInJson(strCmd.c_str(), &pszRes);
		}

		if (reader.parse(string(pszRes), resRoot))
		{
			int iCode = resRoot["code"].asInt();

			if (0 == iCode)
			{
				Json::Value data = resRoot["data"];
			}
		}
	}
}

LRESULT CTestPublishDlg::OnShowMsg(WPARAM wParam, LPARAM lParam)
{
	if(lParam)
	{
		const char *p = (const char *)lParam;
		string strMsg = p;
		delete [] p;
		ShowMsg(strMsg);
	}
	return 0;
}

LRESULT CTestPublishDlg::OnShowPublishNetInfo(WPARAM wParam, LPARAM lParam)
{
	if(lParam)
	{
		const char *p = (const char *)lParam;
		string strMsg = p;
		delete [] p;
		ShowPublishNetInfo(strMsg);
	}
	return 0;
}

LRESULT CTestPublishDlg::OnShowPublishPCStauts(WPARAM wParam, LPARAM lParam)
{
	if(lParam)
	{
		const char *p = (const char *)lParam;
		string strMsg = p;
		delete [] p;
		ShowPublishPCStatus(strMsg);
	}
	return 0;
}

void CTestPublishDlg::ShowPublishPCStatus(string &strMsg)
{
	m_PublishPCStatusList.SetSel(0, -1); 
	m_PublishPCStatusList.Clear(); 
	int iLineNum = m_PublishPCStatusList.GetLineCount(); 
	if (iLineNum <= 200) 
	{
		m_PublishPCStatusList.SetSel(-1, -1); 
		// m_EditLog.ReplaceSel(str + "\r\n\r\n"); 
		m_PublishPCStatusList.ReplaceSel((strMsg+"\r\n").c_str());
	} 
	else
	{ 
		m_PublishPCStatusList.SetSel(0, -1); 
		m_PublishPCStatusList.Clear(); 
	}
}

void CTestPublishDlg::OnBnClickedButtonStop()
{
	if(m_pRtmpManage)
	{
		m_pRtmpManage->StopPublish();
		m_pRtmpManage->Release();
		m_pRtmpManage = NULL;
	}

}

LRESULT CTestPublishDlg::OnHScroll(WPARAM wParam, LPARAM lParam)
{
	if ( HWND(lParam) == GetDlgItem(IDC_SLIDER_VOLUM)->m_hWnd )
	{
		m_iAdVolum = m_sliderVolum.GetPos();

		if(m_pRtmpManage)
			m_pRtmpManage->SetPublishVolum(m_iAdVolum, m_bAdMute);
	}
	return S_OK;
}

void CTestPublishDlg::OnBnClickedButtonShowspm()
{
	m_bShowSpectrum = !m_bShowSpectrum;
	SetShowSpectrum(m_bShowSpectrum);
}

void CTestPublishDlg::OnBnClickedButtonChangeSize()
{
	static bool change = true;
	if (change)
	{
		//::MoveWindow(GetDlgItem(IDC_STATIC_DISPLAY_PUBLISH).m_hWnd, 0, 0, 160, 120, TRUE);
		::MoveWindow(GetDlgItem(IDC_STATIC_DISPLAY_PUBLISH)->m_hWnd, 0, 0, 240, 180, TRUE);
	}
	else
	{
		::MoveWindow(GetDlgItem(IDC_STATIC_DISPLAY_PUBLISH)->m_hWnd, 0, 0, 320, 240, TRUE);
	}
	change = !change;
}

void CTestPublishDlg::OnBnClickedButtonStereo()
{
	m_bCapStereo = true;
	m_bCapMic = false;
	if(m_pRtmpManage)
	{
		m_pRtmpManage->CapStereo(m_bCapStereo);
	}
}

void CTestPublishDlg::OnBnClickedButtonMic()
{
	m_bCapStereo = false;
	m_bCapMic = true;
	if(m_pRtmpManage)
	{
		m_pRtmpManage->CapStereo(m_bCapStereo);
	}
}
