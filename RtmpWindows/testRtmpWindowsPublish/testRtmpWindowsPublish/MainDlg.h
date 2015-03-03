// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "resource.h"
#include <atlctrls.h>
#include <string>
using std::wstring;

//#include "RtmpManage_JSON\IRtmpManage_JSON.h"
#include "RtmpPublishManager\IRtmpPublishManager.h"
#include "AudioVideoPreview/IAudioVideoPreview.h"
#include "jsonPraser/json.h"

#define					MSG_SHOWMSG									WM_USER+1001
#define					MSG_SHOW_PUBLISH_NETINFO					WM_USER+1002
#define					MSG_SHOW_PUBLISH_PCSTATUS					WM_USER+1003
#define					MSG_SHOW_AUDIO_SPECTRUM						WM_USER+1004

class CMainDlg : public CDialogImpl<CMainDlg>, 
		public CMessageFilter, public CIdleHandler,
		public IRtmpPublishManagerNotify
		, public IAudioVideoPreviewNotify
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
		COMMAND_HANDLER(IDC_BUTTON_START, BN_CLICKED, OnBnClickedButtonStart)
		COMMAND_ID_HANDLER(IDC_COMBO_VIDEO, OnVideoDevice)
		COMMAND_ID_HANDLER(IDC_COMBO_AUDIO, OnAudioDevice)
		COMMAND_ID_HANDLER(IDC_COMBO_AUDIOPLAYDEV, OnAudioPlayDevice)
		COMMAND_HANDLER(IDC_BUTTON_STOP, BN_CLICKED, OnBnClickedButtonStop)
		COMMAND_HANDLER(IDC_BUTTON1, BN_CLICKED, OnBnClickedButton1)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		//COMMAND_HANDLER(IDC_BUTTON_START_PLAY, BN_CLICKED, OnBnClickedButtonStartPlay)
		COMMAND_HANDLER(IDC_BUTTON_STEREO, BN_CLICKED, OnBnClickedButtonStereo)
		COMMAND_HANDLER(IDC_BUTTON_MIC, BN_CLICKED, OnBnClickedButtonMic)
		MESSAGE_HANDLER(MSG_SHOWMSG, OnShowMsg) 
		MESSAGE_HANDLER(MSG_SHOW_PUBLISH_NETINFO, OnShowPublishNetInfo) 
		MESSAGE_HANDLER(MSG_SHOW_PUBLISH_PCSTATUS, OnShowPublishPCStauts) 
		COMMAND_HANDLER(IDC_BUTTON_COPYMSG, BN_CLICKED, OnBnClickedButtonCopymsg)
		COMMAND_HANDLER(IDC_BUTTON_CHANGE_SIZE, BN_CLICKED, OnBnClickedButtonChangeSize)
		COMMAND_HANDLER(IDC_BUTTON2, BN_CLICKED, OnBnClickedButton2)
		COMMAND_HANDLER(IDC_BUTTON_ISRESTART_PROCESS, BN_CLICKED, OnBnClickedButtonIsrestartProcess)
		COMMAND_HANDLER(IDC_BUTTON_OTHERPLAYER_MUTE, BN_CLICKED, OnBnClickedButtonOtherPlayerMute)
		COMMAND_HANDLER(IDC_BUTTON_ENABLERECORD, BN_CLICKED, OnBnClickedButtonEnableRecord)
		COMMAND_HANDLER(IDC_BUTTON_IS_DIRECTCON, BN_CLICKED, OnBnClickedButtonDirectCon)
		COMMAND_HANDLER(IDC_BUTTON_UDP, BN_CLICKED, OnBnClickedButtonUDP)
		COMMAND_HANDLER(IDC_BUTTON_AUDIO_PREVIEW, BN_CLICKED, OnBnClickedButtonAudioPreview)
		COMMAND_HANDLER(IDC_BUTTON_VIDEO_PREVIEW, BN_CLICKED, OnBnClickedButtonVideoPreview)
		COMMAND_HANDLER(IDC_BUTTON_SHOWSPM, BN_CLICKED, OnBnClickedButtonShowSpectrum)
		MESSAGE_HANDLER(MSG_SHOW_AUDIO_SPECTRUM, OnShowSpectrum) 
		COMMAND_HANDLER(IDC_CHECK_AUTOCTRLBITRATE, BN_CLICKED, OnBnClickedCheckAutoctrlbitrate)
		//COMMAND_HANDLER(IDC_BUTTON3, BN_CLICKED, OnBnClickedButton3)
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

		m_ComboBoxVideo.Attach(GetDlgItem(IDC_COMBO_VIDEO).m_hWnd);
		m_ComboBoxAudio.Attach(GetDlgItem(IDC_COMBO_AUDIO).m_hWnd);
		m_ComboBoxThirdPartPlayer.Attach(GetDlgItem(IDC_COMBO_THIRD_PART_PLAYER).m_hWnd);
		m_ComboBoxAudioPlay.Attach(GetDlgItem(IDC_COMBO_AUDIOPLAYDEV).m_hWnd);

		m_MsgList.Attach(GetDlgItem(IDC_LIST_MSG).m_hWnd);
		m_PublishNetInfoList.Attach(GetDlgItem(IDC_EDIT_NETINFO).m_hWnd);
		m_PublishPCStatusList.Attach(GetDlgItem(IDC_EDIT_PCSTATUS).m_hWnd);
		m_sliderVolum.Attach(GetDlgItem(IDC_SLIDER_VOLUM).m_hWnd);
		m_sliderOtherPlayerVolum.Attach(GetDlgItem(IDC_SLIDER_OTHERPLAYER).m_hWnd);
		m_sliderShowSpectrum.Attach(GetDlgItem(IDC_SLIDER_SHOWSPECTRUM).m_hWnd);

		Init();
		return TRUE;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		
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
		DestroyWindow();
		::PostQuitMessage(nVal);
	}
	LRESULT OnBnClickedButtonStart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnVideoDevice(WORD wNotifyCode, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAudioDevice(WORD wNotifyCode, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	//@	IRtmpManageNotify
	virtual bool OnRtmpPublishManagerMsg(eMsgLevel MsgLev, const char *pMsg) ;
	virtual bool OnRtmpPublishManagerNetInfo(RtmpPublishNetInfo &RtmpNetInfo) ;
	virtual bool OnRtmpPublishManagerComputerStatusInfo(RtmpPublishComputerStatusInfo &StatusInfo) ;
	virtual bool OnRtmpPublishNotifyInJson(const char* pszJson);

	//
	virtual bool OnAudioVideoPreviewMsg(eMsgLevel MsgLev, const char *pMsg, const char *pDetail=NULL) ;

private:
	 IRtmpPublishManager * m_pRtmpManage;
	 CComboBox							m_ComboBoxVideo;
	 CComboBox							m_ComboBoxAudio;
 	 CComboBox							m_ComboBoxThirdPartPlayer;
	 CComboBox							m_ComboBoxAudioPlay;

	 wstring									m_wstrCam;
	 wstring									m_wstrMic;
	 wstring									m_wstrAudioPlay;
	 unsigned int							m_uCamID;
	 unsigned int							m_uMicID;
	 void	 Init();
	 void ShowMsg(string &strMsg);
	 void ShowPublishNetInfo(string &strMsg);
	 void PublishPara2Json(string &strRetJson);
	 void PlayPara2Json(string &strRetJson);
	 void Cmd2Json(string &strRetJson, string &strCmd);
	 void SetDefaultPara();
	 void GetPublishPara();
	 string GetSdkVersion();
	 bool ParseCmd(const string &strCmd);
	 void SetShowSpectrum(bool bShow);
	 void SetAutoCtrlBitrate(bool bAuto);

	 //@ server
	 string m_strScheduleSvr;
	 string m_strPublishRoomInfo;
	 string m_strPublishStageInfo;
	 string m_strKey;

	 //@ video paragment
	 int		m_iVdFps;
	 int		m_iVdBitRate;
	 int		m_iVdWidth;
	 int		m_iVdHeight;
	 int		m_iVdQuality;
	 int		m_iVdInterval;
	 int		m_iBFrame;
	 int		m_iQuality;
	 
	 //@ audio para
	 int		m_iAdSampleRate;
	 int		m_iAdBitRate;
	 int		m_iAdChannels;
	 int		m_iAdBitPerSample;
	 int		m_iAdVolum;
	 bool		m_bAdMute;
	 bool		m_bShowSpectrum;

	 //
	 /*CEdit										m_MsgList;*/
	 CListBox										m_MsgList;
	 CEdit										m_PublishNetInfoList;
	 CEdit										m_PublishPCStatusList;
	 CTrackBarCtrl						m_sliderVolum;
	 CTrackBarCtrl						m_sliderOtherPlayerVolum;
	 CTrackBarCtrl				m_sliderShowSpectrum;
	 Json::Value				m_jsonRoot;
	 Json::Value				m_jsonParams;

public:
	LRESULT OnBnClickedButtonStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	LRESULT OnBnClickedButtonStartPlay(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	
	bool					m_bCapStereo;
	bool					m_bCapMic;
	LRESULT OnBnClickedButtonStereo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonMic(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnShowMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnShowPublishNetInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnShowPublishPCStauts(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void CMainDlg::ShowPublishPCStatus(string &strMsg);
	LRESULT OnBnClickedButtonCopymsg(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonChangeSize(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButton2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnShowSpectrum(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//wasapi
	bool						m_bCaptureBackground;
	void						EnableBackgroundPlayer(bool bBackground);
	bool						m_bCapPlayer;
	bool						m_bRestartProcess;
	LRESULT OnBnClickedButtonIsrestartProcess(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonOtherPlayerMute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	int			m_iOtherPlayerVolum;
	bool		m_bOtherPlayerMute;

	string	m_strUserID;
	LRESULT OnBnClickedButtonEnableRecord(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	bool		m_bEnableRecord;
	string	m_strFileName;

	LRESULT OnBnClickedButtonDirectCon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	bool					m_bDirectConSvr;
	LRESULT OnBnClickedButtonUDP(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	bool					m_bUseUDP;

	LRESULT OnBnClickedButtonAudioPreview(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	bool					m_bPreviewAudio;
	LRESULT OnBnClickedButtonVideoPreview(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	bool					m_bPreviewVideo;

	LRESULT OnAudioPlayDevice(WORD wNotifyCode, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	IAudioVideoPreview*									m_pAudioVideoPreview;
	LRESULT OnBnClickedButtonShowSpectrum(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedCheckAutoctrlbitrate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
