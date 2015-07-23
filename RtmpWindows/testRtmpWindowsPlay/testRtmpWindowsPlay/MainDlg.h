// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "resource.h"
#include <atlctrls.h>
#include <string>
#include <vector>
using std::wstring;
using std::vector;

#include "RtmpPlayManager/IRtmpPlayManager.h"
#include "ConvertString.h"
#include "jsonPraser/json.h"
#include "CrashCatch.h"


#define				MSG_SHOWMSG								WM_USER+100
#define				MSG_SHOW_PUBLISH_NETINFO				WM_USER+101
#define				MSG_SHOW_PLAY_PCSTATUS					WM_USER+102
#define				MSG_SHOW_PLAY_AVINFO					WM_USER+103
#define				MSG_SHOW_PLAYQUALITY					WM_USER+104
#define				MSG_SHOW_AUDIO_SPECTRUM					WM_USER+105

class CMainDlg : public CDialogImpl<CMainDlg>, 
		public CMessageFilter, public CIdleHandler
		,public IRtmpPlayManagerNotify
{
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}


	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOk)
		COMMAND_HANDLER(IDC_BUTTON1, BN_CLICKED, OnBnClickedButton1)
		//COMMAND_HANDLER(IDC_BUTTON2, BN_CLICKED, OnBnClickedButton2)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		COMMAND_HANDLER(IDC_BUTTON_BMUTE, BN_CLICKED, OnBnClickedButtonBmute)
		COMMAND_HANDLER(IDC_BUTTON_STOP, BN_CLICKED, OnBnClickedButtonStop)
		COMMAND_HANDLER(IDC_BUTTON_ISDIRECTCON, BN_CLICKED, OnBnClickedButtonDirectCon)
		COMMAND_HANDLER(IDC_BUTTON_UDP, BN_CLICKED, OnBnClickedButtonUDP)
		MESSAGE_HANDLER(MSG_SHOWMSG, OnShowMsg) 
		MESSAGE_HANDLER(MSG_SHOW_PUBLISH_NETINFO, OnShowPlayNetInfo)
		MESSAGE_HANDLER(MSG_SHOW_PLAY_PCSTATUS, OnShowPlayPCStatus)
		MESSAGE_HANDLER(MSG_SHOW_PLAY_AVINFO, OnShowPlayAVInfo)
		MESSAGE_HANDLER(MSG_SHOW_PLAYQUALITY, OnShowPlayQuality)

		COMMAND_HANDLER(IDC_BUTTON_COPYMSG, BN_CLICKED, OnBnClickedButtonCopymsg)
		COMMAND_HANDLER(IDC_BUTTON_CHANGE_SIZE, BN_CLICKED, OnBnClickedButtonChangeSize)
		COMMAND_HANDLER(IDC_BUTTON_ENABLE_RECORD, BN_CLICKED, OnBnClickedButtonRecord)
		MESSAGE_HANDLER(MSG_SHOW_AUDIO_SPECTRUM, OnShowSpectrum) 
		COMMAND_HANDLER(IDC_BUTTON_SET_BUFFER_TIME, BN_CLICKED, OnBnClickedButtonSetBufferTime)
		COMMAND_HANDLER(IDC_BUTTON_TEST, BN_CLICKED, OnBnClickedButtonTest)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall, FALSE);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		//m_MsgList.Attach(GetDlgItem(IDC_EDIT_MSG).m_hWnd); 
		m_MsgList.Attach(GetDlgItem(IDC_LIST_MSG).m_hWnd);
		m_PlayNetInfoList.Attach(GetDlgItem(IDC_EDIT_PLAYNETINFO).m_hWnd);
		m_PlayPCStatusList.Attach(GetDlgItem(IDC_EDIT_PLAYPCSTATUS).m_hWnd);
		m_PlayAVInfoList.Attach(GetDlgItem(IDC_EDIT_AVINFO).m_hWnd);
		m_PlayQualityList.Attach(GetDlgItem(IDC_EDIT_PLAYQUALITY).m_hWnd);
		m_ComboBoxAudioOutDev.Attach(GetDlgItem(IDC_COMBO_VDOUTDEV).m_hWnd);

		m_sliderOutVolum.Attach(GetDlgItem(IDC_SLIDER_OUTVOLUM).m_hWnd);
		m_sliderOutVolum.SetPos(80);
		m_sliderShowSpectrum.Attach(GetDlgItem(IDC_SLIDER_ShowSpectrum).m_hWnd);

		Init_CrashCatch();
		//@ Show default config
		GetDlgItem(IDC_EDIT_SERVER).SetWindowText(ConvertString::s2ws(m_strScheduleSvr).c_str());
		GetDlgItem(IDC_EDIT_SITE).SetWindowText(ConvertString::s2ws(m_strPublishSiteInfo).c_str());
		GetDlgItem(IDC_EDIT_STAGE).SetWindowText(ConvertString::s2ws(m_strPublishStageInfo).c_str());
		GetDlgItem(IDC_EDIT_USERID).SetWindowText(ConvertString::s2ws(m_strUserID).c_str());
		GetDlgItem(IDC_EDIT_KEY).SetWindowText(ConvertString::s2ws(m_strKey).c_str());
		
		wstring wstrTmp;
		TCHAR tbuf[128] = L"0";
		wsprintf(tbuf,L"%i",m_iTimes);
		wstrTmp = tbuf;
		GetDlgItem(IDC_EDIT_TIMES).SetWindowText(wstrTmp.c_str());
		memset(tbuf,0,sizeof(tbuf));
		wsprintf(tbuf,L"%i",m_iDelay);
		wstrTmp = tbuf;	
		GetDlgItem(IDC_EDIT_DELAY).SetWindowText(wstrTmp.c_str());
		//UIAddChildWindowContainer(m_hWnd);
		wchar_t** ppAdOutDev = NULL;
		unsigned int size = 0;
		m_pRtmpPlayManager = CreateRtmpPlayManager(this);
		//m_pRtmpPlayManager2 = CreateRtmpPlayManager(this);
		m_pRtmpPlayManager->GetAudioPlayDevList(ppAdOutDev, size);

		vector<wstring> vecAdOutDev ;

		for(unsigned int i=0; i<size; i++)
		{
			vecAdOutDev.push_back(ppAdOutDev[i]);
			m_ComboBoxAudioOutDev.InsertString(m_ComboBoxAudioOutDev.GetCount(), ppAdOutDev[i]);
		}

		string strSdkVersion = GetSdkVersion();

		if (strSdkVersion.empty())
		{	strSdkVersion = "SdkVer:get failed.";	}
		else
		{	strSdkVersion = "SdkVer:" + strSdkVersion;	}
		wstring wstrtmp = ConvertString::s2ws(strSdkVersion);
		SetWindowText(wstrtmp.c_str());
		ShowMsg(strSdkVersion);
		m_ComboBoxAudioOutDev.SetCurSel(0);

		GetDlgItem(IDC_EDIT_dwDatalenOffset).SetWindowText(L"3000");
		GetDlgItem(IDC_EDIT_dwMaxTimestampleInqueue).SetWindowText(L"10000");
		GetDlgItem(IDC_EDIT_dwTimeCheckSynbuf).SetWindowText(L"300000");


		return TRUE;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		//CAboutDlg dlg;
		//dlg.DoModal();
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		CloseDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}

	void CloseDialog(int nVal)
	{
		if(m_hThread)
		{
			m_bBreak = !m_bBreak;
			WaitForSingleObject(m_hThread,INFINITE);
			CloseHandle(m_hThread);
		}
		DestroyWindow();
		::PostQuitMessage(nVal);
	}
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButton2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonRecord(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	bool		m_bEnableRecord;
	string	m_strFileName;

	LRESULT OnShowPlayPCStatus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void ShowPlayPCStatus(string &strMsg);
	LRESULT OnShowPlayAVInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnShowPlayQuality(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void ShowPlayQuality(string &strMsg);
	IRtmpPlayManager	*m_pRtmpPlayManager;
	IRtmpPlayManager	*m_pRtmpPlayManager2;
	CComboBox							m_ComboBoxAudioOutDev;
	//CEdit										m_MsgList;
	CListBox										m_MsgList;
	CEdit										m_PlayNetInfoList;
	CEdit										m_PlayPCStatusList;
	CEdit										m_PlayAVInfoList;
	CEdit										m_PlayQualityList;

	CTrackBarCtrl						m_sliderOutVolum;
	bool											m_bOutMute;
	int												m_iAdOutVolum;
	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual bool OnRtmpPlayManagerMsg(eMsgLevel MsgLev, const char *pMsg, const char *pDetail=NULL) ;
	virtual bool OnRtmpPlayManagerNetInfo(RtmpPublishNetInfo &RtmpNetInfo) ;
	virtual bool OnRtmpPlayManagerComputerStatusInfo(RtmpPublishComputerStatusInfo &RtmpStatusInfo) ;
	virtual bool OnRtmpPlayManagerAVInfo(RtmpPlayAVInfo &RtmpPlayAVInfo) ;
	virtual bool OnRtmpPlayManagerPlayQuality(RtmpPlayQuality &RtmpPlayQuality) ;
	virtual bool OnRtmpPlayNotifyInJson(const char* pszJson);


	void ShowMsg(string &strMsg);
	LRESULT OnBnClickedButtonBmute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	CMainDlg();
	LRESULT OnBnClickedButtonStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonDirectCon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonUDP(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	string m_strScheduleSvr;
	string m_strPublishSiteInfo;
	string m_strPublishStageInfo;
	string m_strKey;

	LRESULT OnShowMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnShowPlayNetInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void ShowPlayNetInfo(string &strMsg);
	unsigned long								m_lLastTime;
	long								m_lPingSum;
	long								m_lTimes;
	int									m_dwTotalTimesBeforeShow;
	int									m_dwDnsResolveTime;
	int									m_dwTcpConnectTime;
	int									m_dwRtmpInitTime;
	int									m_uSendRtmpTimes;
	int									m_uTimeToFirstBuffer;
	int									m_dwTotalBufEmptyTimes;
	int									m_dwReceiveFirstDataTime;
	DWORD								m_dwButtonStart;
	int									m_iTimes;
	int									m_iDelay;
	bool								m_bBreak;
	HANDLE								m_hThread;
	static DWORD WINAPI TestFun(LPVOID arg);
	void DoTestMain();
	LRESULT OnBnClickedButtonCopymsg(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonChangeSize(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnShowSpectrum(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	string GetSdkVersion();
	void OnShowVideo();
	bool ParseCmd(const string &strCmd);

	string								m_strUserID;
	bool								m_bDirectConSvr;
	bool								m_bUseUDP;
	CTrackBarCtrl						m_sliderShowSpectrum;
	LRESULT OnBnClickedButtonSetBufferTime(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonTest(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
