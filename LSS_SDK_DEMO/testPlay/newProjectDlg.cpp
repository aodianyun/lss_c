// newProjectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "newProject.h"
#include "newProjectDlg.h"
#include ".\newprojectdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CnewProjectDlg �Ի���



CnewProjectDlg::CnewProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CnewProjectDlg::IDD, pParent)
	, m_strScheduleSvr("rtmp://lssplay.aodianyun.com:1935")
	, m_strPublishSiteInfo("app")
	, m_strPublishStageInfo("stream")
	, m_strKey("yun")
	, m_strUserID("10000")
	, m_pRtmpPlayManager(NULL)
	, m_lLastTime(0)
	, m_lTimes(0)
	, m_dwDnsResolveTime(0)
	, m_dwTcpConnectTime(0)
	, m_dwRtmpInitTime(0)
	, m_uSendRtmpTimes(0)
	, m_uTimeToFirstBuffer(0)
	, m_dwTotalBufEmptyTimes(0)
	, m_lPingSum(0)
	, m_iAdOutVolum(80)
	, m_bOutMute(false)
	, m_bEnableRecord(false)
	, m_strFileName("d:\\a.flv")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CnewProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_SLIDER_ShowSpectrum,m_sliderShowSpectrum);
	DDX_Control(pDX,IDC_SLIDER_OUTVOLUM, m_sliderOutVolum);
	DDX_Control(pDX,IDC_LIST_MSG, m_MsgList);
	DDX_Control(pDX,IDC_EDIT_PLAYNETINFO, m_PlayNetInfoList);
	DDX_Control(pDX,IDC_EDIT_PLAYPCSTATUS, m_PlayPCStatusList);
	DDX_Control(pDX,IDC_EDIT_AVINFO, m_PlayAVInfoList);
	DDX_Control(pDX,IDC_EDIT_PLAYQUALITY, m_PlayQualityList);
	DDX_Control(pDX,IDC_COMBO_VDOUTDEV, m_ComboBoxAudioOutDev);
}

BEGIN_MESSAGE_MAP(CnewProjectDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(MSG_SHOWMSG, OnShowMsg) 
	ON_MESSAGE(MSG_SHOW_PUBLISH_NETINFO, OnShowPlayNetInfo)
	ON_MESSAGE(MSG_SHOW_PLAY_PCSTATUS, OnShowPlayPCStatus)
	ON_MESSAGE(MSG_SHOW_PLAY_AVINFO, OnShowPlayAVInfo)
	ON_MESSAGE(MSG_SHOW_PLAYQUALITY, OnShowPlayQuality)
	ON_MESSAGE(MSG_SHOW_AUDIO_SPECTRUM, OnShowSpectrum) 
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_BMUTE, OnBnClickedButtonBmute)
	ON_MESSAGE(WM_HSCROLL,OnHScroll)
	ON_BN_CLICKED(IDC_BUTTON_SET_BUFFER_TIME, OnBnClickedButtonSetBufferTime)
	ON_BN_CLICKED(IDC_BUTTON_ENABLE_RECORD, OnBnClickedButtonEnableRecord)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_SIZE, OnBnClickedButtonChangeSize)
END_MESSAGE_MAP()


// CnewProjectDlg ��Ϣ�������

BOOL CnewProjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_sliderOutVolum.SetPos(80);

	GetDlgItem(IDC_EDIT_SERVER)->SetWindowText(m_strScheduleSvr.c_str());
	GetDlgItem(IDC_EDIT_SITE)->SetWindowText(m_strPublishSiteInfo.c_str());
	GetDlgItem(IDC_EDIT_STAGE)->SetWindowText(m_strPublishStageInfo.c_str());
	GetDlgItem(IDC_EDIT_USERID)->SetWindowText(m_strUserID.c_str());
	GetDlgItem(IDC_EDIT_KEY)->SetWindowText(m_strKey.c_str());

	// ��ȡ���������豸 ����SartPlay֮ǰ
	wchar_t **ppAdOutDev = NULL;
	unsigned int size = 0;
	m_pRtmpPlayManager = CreateRtmpPlayManager(this);
	//ASSERT(m_pRtmpPlayManager == NULL);
	if (!m_pRtmpPlayManager)
	{
		MessageBox("player create faild!");
	}

	m_pRtmpPlayManager->GetAudioPlayDevList(ppAdOutDev, size);

	for (unsigned int i=0; i < size; i++)
	{
		m_ComboBoxAudioOutDev.InsertString(m_ComboBoxAudioOutDev.GetCount(), ConvertString::ws2s(wstring(ppAdOutDev[i])).c_str());
	}
	m_ComboBoxAudioOutDev.SetCurSel(0);

	string strSdkVersion = GetSdkVersion();

	if (strSdkVersion.empty())
	{	strSdkVersion = "SdkVer:get failed.";	}
	else
	{	strSdkVersion = "SdkVer:" + strSdkVersion;	}

	SetWindowText(strSdkVersion.c_str());
	ShowMsg(strSdkVersion);


	GetDlgItem(IDC_EDIT_dwDatalenOffset)->SetWindowText("3000");
	GetDlgItem(IDC_EDIT_dwMaxTimestampleInqueue)->SetWindowText("10000");
	GetDlgItem(IDC_EDIT_dwTimeCheckSynbuf)->SetWindowText("300000");

	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CnewProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CnewProjectDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CnewProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CnewProjectDlg::OnRtmpPlayManagerMsg(eMsgLevel MsgLev, const char *pMsg, const char *pDetail/* =NULL */)
{
	if (pMsg == NULL)
	{
		return false;
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
}

bool CnewProjectDlg::OnRtmpPlayManagerNetInfo(RtmpPublishNetInfo &RtmpNetInfo)
{
	char buf[50]={0};
	sprintf(buf, "%d ms", RtmpNetInfo.uPing);
	string strNetInfo = string("Ping:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d ms", RtmpNetInfo.uDelay);
	strNetInfo+= string("Delay:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", RtmpNetInfo.uDiscardBytesSum);
	strNetInfo+= string("DiscardBytesSum:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%.3f kbps", RtmpNetInfo.fAudioBitRate);
	strNetInfo+= string("��Ƶ����:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%.3f kbps", RtmpNetInfo.fVideoBitRate);
	strNetInfo+= string("��Ƶ����:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%f kbps", RtmpNetInfo.fAudioBitRate + RtmpNetInfo.fVideoBitRate);
	strNetInfo+= string("��ǰ����:")+string(buf)+"\r\n";


	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", RtmpNetInfo.uVideoFrameRate);
	strNetInfo+= string("��ǰ֡��:")+string(buf)+"\r\n";

	const char *p = new char [strNetInfo.length()+1];

	if (p)
	{
		memcpy((void *)p, strNetInfo.c_str(), strNetInfo.length()+1);
		PostMessage(MSG_SHOW_PUBLISH_NETINFO, 0, (LPARAM)p);
	}
	return true;
}
bool CnewProjectDlg::OnRtmpPlayManagerComputerStatusInfo(RtmpPublishComputerStatusInfo &RtmpComputerStatusInfo)
{
	char buf[500]={0};
	sprintf(buf, "%d", RtmpComputerStatusInfo.uTotalPhys/1000);
	string strNetInfo = string("���ڴ�:")+string(buf)+"M\r\n";
	unsigned int uUsedMem = RtmpComputerStatusInfo.uTotalPhys-RtmpComputerStatusInfo.uAvailPhys;
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%dM(%d%%)", uUsedMem/1000, 
		RtmpComputerStatusInfo.uTotalPhys>0 ? uUsedMem*100/RtmpComputerStatusInfo.uTotalPhys : 0);
	strNetInfo+= string("�����ڴ�:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", RtmpComputerStatusInfo.uTotalVirtual/1000);
	strNetInfo+= string("�������ڴ�:")+string(buf)+"M\r\n";
	unsigned int uUsedVirtualMem = RtmpComputerStatusInfo.uTotalVirtual - RtmpComputerStatusInfo.uAvailVirtual;
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%dM(%d%%)", uUsedVirtualMem/1000, 
		RtmpComputerStatusInfo.uTotalVirtual>0 ? uUsedVirtualMem*100/RtmpComputerStatusInfo.uTotalVirtual : 0) ;
	strNetInfo+= string("���������ڴ�:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%dMHZ", RtmpComputerStatusInfo.uCPUClockSpeed);
	strNetInfo+= string("cpuƵ��:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", RtmpComputerStatusInfo.uCPUCores);
	strNetInfo+= string("cpu������:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", RtmpComputerStatusInfo.uCPUUsage);
	strNetInfo+= string("cpuʹ����:")+string(buf)+"%\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%dKB", RtmpComputerStatusInfo.uUpSpeed);
	strNetInfo+= string("�ϴ�����:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%dKB", RtmpComputerStatusInfo.uDownSpeed);
	strNetInfo+= string("��������:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d ms", RtmpComputerStatusInfo.uPublishPing);
	strNetInfo+= string("����ping:")+string(buf)+"\r\n";

	const char *p = new char [strNetInfo.length()+1];

	if (p)
	{
		memcpy((void *)p, strNetInfo.c_str(), strNetInfo.length()+1);
		PostMessage(MSG_SHOW_PLAY_PCSTATUS, 0, (LPARAM)p);
	}

	return true;
}
bool CnewProjectDlg::OnRtmpPlayManagerAVInfo(RtmpPlayAVInfo &RtmpPlayAVInfo)
{
	char buf[500]={0};
	sprintf(buf, "%.3f s", (float)RtmpPlayAVInfo.uTotalBufTime/1000);
	string strNetInfo = string("������ʱ��:")+string(buf)+"\r\n";

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%.3f s", (float)RtmpPlayAVInfo.uAudioBufTime/1000);
	strNetInfo+= string("��Ƶ����ʱ��:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%.3f s", (float)RtmpPlayAVInfo.uVideoBufTime/1000);
	strNetInfo+= string("��Ƶ����ʱ��:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", RtmpPlayAVInfo.uVideoHeight) ;
	strNetInfo+= string("��Ƶԭʼ�߶�:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", RtmpPlayAVInfo.uVideoWidth);
	strNetInfo+= string("��Ƶԭʼ���:")+string(buf)+"\r\n";

	const char *p = new char [strNetInfo.length()+1];

	if (p)
	{
		memcpy((void *)p, strNetInfo.c_str(), strNetInfo.length()+1);
		PostMessage(MSG_SHOW_PLAY_AVINFO, 0, (LPARAM)p);
	}
	return true;
}
bool CnewProjectDlg::OnRtmpPlayManagerPlayQuality(RtmpPlayQuality &RtmpPlayQuality)
{
	char buf[500]={0};
	//sprintf(buf, "%d ms", RtmpPlayQuality.uShowVideoSucTime);����,�����µ� �㷨
	sprintf(buf, "%d.ms",m_dwReceiveFirstDataTime - m_dwButtonStart);
	string strPlayQuality = string("��ʾ��Ƶ����ʱ��:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d (��)", RtmpPlayQuality.uBufferEmptyCount);
	strPlayQuality+= string("��������:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d (��)", RtmpPlayQuality.uBufferFullCount);
	strPlayQuality+= string("��������:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d (ms)", RtmpPlayQuality.uMaxPlayBuffer);
	strPlayQuality+= string("���������ֵ:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d (ms)", RtmpPlayQuality.uMinPlayBuffer);
	strPlayQuality+= string("��������Сֵ:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d (ms)", RtmpPlayQuality.uAvgPlayBuffer);
	strPlayQuality+= string("������ƽ��ֵ:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%.2f (s)", RtmpPlayQuality.fTotalPlayTime);
	strPlayQuality+= string("�ܲ���ʱ��:")+string(buf)+"\r\n";


	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d (ms)", m_dwTotalTimesBeforeShow);
	strPlayQuality+= string("��ʾ��Ƶʱ��:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d (ms)", m_dwDnsResolveTime);
	strPlayQuality+= string("DNS����ʱ��:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d (ms)", m_dwTcpConnectTime);
	strPlayQuality+= string("TCP����ʱ��:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d (ms)", m_dwRtmpInitTime);
	strPlayQuality+= string("RTMP��ʼ��ʱ��:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d (ms)", m_uSendRtmpTimes);
	strPlayQuality+= string("��������ʱ��:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d (ms)", m_uTimeToFirstBuffer);
	strPlayQuality+= string("��һ�λ���ʱ��:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d (ms)", m_dwTotalBufEmptyTimes);
	strPlayQuality+= string("�ٻ�����ʱ��:")+string(buf)+"\r\n";
	const char *p = new char [strPlayQuality.length()+1];

	if (p)
	{
		memcpy((void *)p, strPlayQuality.c_str(), strPlayQuality.length()+1);
		PostMessage(MSG_SHOW_PLAYQUALITY, 0, (LPARAM)p);
	}
	return true;
}
bool CnewProjectDlg::OnRtmpPlayNotifyInJson(const char* pszJson)
{
	if (NULL == pszJson)
	{	return false;	}

	string strCmd = pszJson;
	ParseCmd(strCmd);
	return true;
}

bool CnewProjectDlg::ParseCmd(const string &strCmd)
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
				if (strApi == "OnShowVideo")
				{
					OnShowVideo();
					return true;
				}
				else if (strApi == "OnAudioPercent")
				{
					int uPercent = objParams["uPercent"].asInt();
					char* pParam = new char[sizeof(int)];	/* pParam will be free in msg function. */
					itoa(uPercent, pParam, 10);
					PostMessage(MSG_SHOW_AUDIO_SPECTRUM, 0, (LPARAM)pParam);
					return true;
				}else if (strApi == "OnRtmpPlayManagerPlayQualityAdd")
				{
					m_dwTotalTimesBeforeShow = objParams["uTotalTimesBeforeShow"].asInt();
					m_dwDnsResolveTime = objParams["uDnsResolveTime"].asInt();
					m_dwTcpConnectTime = objParams["uTcpConnectTime"].asInt();
					m_dwRtmpInitTime = objParams["uRtmpInitTime"].asInt();
					m_uSendRtmpTimes= objParams["uSendRtmpTimes"].asInt();
					m_uTimeToFirstBuffer = objParams["uTimeToFirstBuffer"].asInt();
					m_dwTotalBufEmptyTimes = objParams["uTotalBufEmptyTimes"].asInt();
					m_dwReceiveFirstDataTime = objParams["uReceiveFirstDataTime"].asInt();
					return true;
				}
			}
		}
	}

	strMsg = "Error:cmd must be json format.";
	ShowMsg(strMsg);
	return true;
}

void CnewProjectDlg::OnShowVideo()
{
	string strMsg("Msg:OnShowVideo.");

	SYSTEMTIME time;
	GetLocalTime(&time);
	char buf[50] = {0};
	sprintf(buf, "%d:%d:%d:%d  ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	strMsg = string(buf) + strMsg;
	ShowMsg(strMsg);
}

void CnewProjectDlg::ShowMsg(string &strMsg)
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

string CnewProjectDlg::GetSdkVersion()
{
	string strCmd;
	string strSdkVersion;
	Json::Value root;
	Json::Value params;
	Json::Reader reader;
	Json::Value resRoot;
	Json::Value data;
	char* pszRes = new char[1024];
	memset(pszRes, 0, 1024);

	root["cmd"] = Json::Value("GetSdkVersion");
	root["params"] = params;
	strCmd = root.toStyledString();

	if (m_pRtmpPlayManager && pszRes)
	{
		m_pRtmpPlayManager->CallInJson(strCmd.c_str(), &pszRes);
	}

	if (reader.parse(pszRes, resRoot))
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

	if (pszRes)
	{
		delete[] pszRes;
		pszRes = NULL;
	}

	return strSdkVersion;
}

LRESULT CnewProjectDlg::OnShowMsg(WPARAM wParam, LPARAM lParam)
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
LRESULT CnewProjectDlg::OnShowPlayPCStatus(WPARAM wParam, LPARAM lParam)
{
	if(lParam)
	{
		const char *p = (const char *)lParam;
		m_PlayPCStatusList.SetWindowText(p);
		delete [] p;
	}
	return 0;
}
LRESULT CnewProjectDlg::OnShowPlayAVInfo(WPARAM wParam, LPARAM lParam)
{
	if(lParam)
	{
		const char *p = (const char *)lParam;
		string strMsg = p;
		delete [] p;
		m_PlayAVInfoList.SetWindowText(strMsg.c_str());
	}
	return true;
}
LRESULT CnewProjectDlg::OnShowPlayNetInfo(WPARAM wParam, LPARAM lParam)
{
	if(lParam)
	{
		const char *p = (const char *)lParam;
		m_PlayNetInfoList.SetWindowText(p);
		delete [] p;
	}
	return 0;
}
LRESULT CnewProjectDlg::OnShowPlayQuality(WPARAM wParam, LPARAM lParam)
{
	if(lParam)
	{
		const char *p = (const char *)lParam;
		m_PlayQualityList.SetWindowText(p);
		delete [] p;
	}
	return 0;
}
LRESULT CnewProjectDlg::OnShowSpectrum(WPARAM wParam, LPARAM lParam)
{
	if(lParam)
	{
		const char *p = (const char *)lParam;
		int uPercent = atoi(p);
		delete [] p;
		m_sliderShowSpectrum.SetPos(uPercent);
	}
	return 0;
}
void CnewProjectDlg::OnBnClickedButton1()
{
	char szTime[64] = {0};
	SYSTEMTIME st;
	GetLocalTime(&st);
	m_dwButtonStart = GetTickCount();
	sprintf(szTime,"%d:%d:%d.%d ",st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);


	string strMsg = string(szTime) + "ClickStartButton";
	ShowMsg(strMsg);

	char buf[100] = {'0'};
	GetDlgItem(IDC_EDIT_SERVER)->GetWindowText(buf, sizeof(buf));
	m_strScheduleSvr =  buf;
	GetDlgItem(IDC_EDIT_SITE)->GetWindowText(buf, sizeof(buf));
	m_strPublishSiteInfo = buf;
	GetDlgItem(IDC_EDIT_STAGE)->GetWindowText(buf, sizeof(buf));
	m_strPublishStageInfo = buf;
	GetDlgItem(IDC_EDIT_USERID)->GetWindowText(buf, sizeof(buf));
	m_strUserID = buf;
	GetDlgItem(IDC_EDIT_KEY)->GetWindowText(buf, sizeof(buf));
	m_strKey = buf;
	if (!m_pRtmpPlayManager)
	{
		m_pRtmpPlayManager = CreateRtmpPlayManager(this);
	}

	if(m_pRtmpPlayManager)
	{
		::MoveWindow(GetDlgItem(IDC_STATIC)->m_hWnd, 0, 0, 320, 240, TRUE);

		string strApp = m_strPublishSiteInfo+"?"+m_strKey;
		m_pRtmpPlayManager->SetStreamInfo(m_strScheduleSvr.c_str(), /*m_strPublishSiteInfo*/strApp.c_str(), m_strPublishStageInfo.c_str()); 
		m_pRtmpPlayManager->SetAudioPlayDev(m_ComboBoxAudioOutDev.GetCurSel());
		m_pRtmpPlayManager->SetVdDisplayWnd(GetDlgItem(IDC_STATIC)->m_hWnd);
		m_pRtmpPlayManager->SetAdPara(m_iAdOutVolum, m_bOutMute);
		m_pRtmpPlayManager->SetClientUserID(m_strUserID.c_str());
		m_pRtmpPlayManager->StartPlay();
	}
}

void CnewProjectDlg::OnBnClickedButtonStop()
{
	char szTime[64] = {0};
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf(szTime,"%d:%d:%d.%d ",st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	string strMsg = string(szTime) + "ClickStopButton";
	ShowMsg(strMsg);

	if(m_pRtmpPlayManager){
		m_pRtmpPlayManager->StopPlay();
		m_pRtmpPlayManager->Release();
		m_pRtmpPlayManager = NULL;
	}
}

void CnewProjectDlg::OnBnClickedButtonBmute()
{
	m_bOutMute = !m_bOutMute;
	if(m_bOutMute)
	{
		GetDlgItem(IDC_BUTTON_BMUTE)->SetWindowText("noise");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_BMUTE)->SetWindowText("mute");
	}
	if(m_pRtmpPlayManager)
		m_pRtmpPlayManager->SetAdPara(m_iAdOutVolum, m_bOutMute);
}

LRESULT CnewProjectDlg::OnHScroll(WPARAM wParam, LPARAM lParam)
{
	if ( HWND(lParam) == GetDlgItem(IDC_SLIDER_OUTVOLUM)->m_hWnd )
	{
		m_iAdOutVolum = m_sliderOutVolum.GetPos();

		if(m_pRtmpPlayManager)
			m_pRtmpPlayManager->SetAdPara(m_iAdOutVolum, m_bOutMute);
	}
	return S_OK;
}

void CnewProjectDlg::OnBnClickedButtonSetBufferTime()
{
		unsigned int dwDatelenOffset = GetDlgItemInt(IDC_EDIT_dwDatalenOffset);
	unsigned int dwMaxTimestampleInqueue = GetDlgItemInt(IDC_EDIT_dwMaxTimestampleInqueue);
	unsigned int dwTimeCheckSynbuf = GetDlgItemInt(IDC_EDIT_dwTimeCheckSynbuf);


	string strCmd;
	string strSdkVersion;
	Json::Value root;
	Json::Value params;
	Json::Reader reader;
	Json::Value resRoot;
	Json::Value data;
	char* pszRes = new char[1024];
	memset(pszRes, 0, 1024);

	params["dwDatalenOffset"] = Json::Value(dwDatelenOffset);
	params["dwMaxTimestampleInqueue"] = Json::Value(dwMaxTimestampleInqueue);
	params["dwTimeCheckSynbuf"] = Json::Value(dwTimeCheckSynbuf);
	root["cmd"] = Json::Value("SetSYNBufTime");
	root["params"] = params;
	strCmd = root.toStyledString();

	//* ��    �ã�pszCmd { "cmd":"SetSYNBufTime", "params":{"dwDatalenOffset":1500, "dwMaxTimestampleInqueue":3000, "dwTimeCheckSynbuf":5000} }
	if (m_pRtmpPlayManager && pszRes)
	{
		m_pRtmpPlayManager->CallInJson(strCmd.c_str(), &pszRes);
	}

	if (pszRes)
	{
		delete[] pszRes;
		pszRes = NULL;
	}
}

void CnewProjectDlg::OnBnClickedButtonEnableRecord()
{
	m_bEnableRecord = !m_bEnableRecord;
	//m_pRtmpPlayManager->SetRecordFileFullName(m_strFileName.c_str());
	//m_pRtmpManage->EnableRecordFile(true);
	if(m_bEnableRecord)
	{
		if(m_pRtmpPlayManager)
		{
			m_pRtmpPlayManager->SetRecordFileFullName(m_strFileName.c_str());
			m_pRtmpPlayManager->EnableRecordFile(m_bEnableRecord);
		}
		GetDlgItem(IDC_BUTTON_ENABLE_RECORD)->SetWindowText("ֹͣ");
	}
	else
	{
		if(m_pRtmpPlayManager)
		{
			m_pRtmpPlayManager->EnableRecordFile(m_bEnableRecord);
		}
		GetDlgItem(IDC_BUTTON_ENABLE_RECORD)->SetWindowText("¼��");
	}
}

void CnewProjectDlg::OnBnClickedButtonChangeSize()
{
	static bool change = true;
	if (change)
	{
		::MoveWindow(GetDlgItem(IDC_STATIC)->m_hWnd, 0, 0, 160, 120, TRUE);
	}
	else
	{
		::MoveWindow(GetDlgItem(IDC_STATIC)->m_hWnd, 0, 0, 320, 240, TRUE);
	}
	change = !change;
}
