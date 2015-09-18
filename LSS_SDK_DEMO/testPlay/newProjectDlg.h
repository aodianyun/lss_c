// newProjectDlg.h : 头文件
//

#pragma once
#include "RtmpPlayManager/IRtmpPlayManager.h"
#include "jsonPraser/json.h"
#include "ConvertString.h"

#define				MSG_SHOWMSG								WM_USER+100
#define				MSG_SHOW_PUBLISH_NETINFO				WM_USER+101
#define				MSG_SHOW_PLAY_PCSTATUS					WM_USER+102
#define				MSG_SHOW_PLAY_AVINFO					WM_USER+103
#define				MSG_SHOW_PLAYQUALITY					WM_USER+104
#define				MSG_SHOW_AUDIO_SPECTRUM					WM_USER+105

// CnewProjectDlg 对话框
class CnewProjectDlg : public CDialog, public IRtmpPlayManagerNotify
{
// 构造
public:
	CnewProjectDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_NEWPROJECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现接口
public:
	virtual bool OnRtmpPlayManagerMsg(eMsgLevel MsgLev, const char *pMsg, const char *pDetail=NULL) ;
	virtual bool OnRtmpPlayManagerNetInfo(RtmpPublishNetInfo &RtmpNetInfo) ;
	virtual bool OnRtmpPlayManagerComputerStatusInfo(RtmpPublishComputerStatusInfo &RtmpComputerStatusInfo) ;
	virtual bool OnRtmpPlayManagerAVInfo(RtmpPlayAVInfo &RtmpPlayAVInfo) ;
	virtual bool OnRtmpPlayManagerPlayQuality(RtmpPlayQuality &RtmpPlayQuality) ;
	virtual bool OnRtmpPlayNotifyInJson(const char* pszJson);

	void ShowMsg(string &strMsg);
	string GetSdkVersion();
	bool ParseCmd(const string &strCmd);
	void OnShowVideo();
public:
	string m_strScheduleSvr;
	string m_strPublishSiteInfo;
	string m_strPublishStageInfo;
	string m_strKey;
	string m_strUserID;
	// playsdk对象
	IRtmpPlayManager	*m_pRtmpPlayManager;
	unsigned long						m_lLastTime;
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

	bool								m_bOutMute;
	int									m_iAdOutVolum;

	bool		m_bEnableRecord;
	string	m_strFileName;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnShowMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowPlayPCStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowPlayAVInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowPlayNetInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowPlayQuality(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowSpectrum(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHScroll(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl						m_sliderShowSpectrum;
	CSliderCtrl						m_sliderOutVolum;
	CComboBox							m_ComboBoxAudioOutDev;
	//CEdit										m_MsgList;
	CListBox									m_MsgList;
	CEdit										m_PlayNetInfoList;
	CEdit										m_PlayPCStatusList;
	CEdit										m_PlayAVInfoList;
	CEdit										m_PlayQualityList;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonBmute();
	afx_msg void OnBnClickedButtonSetBufferTime();
	afx_msg void OnBnClickedButtonEnableRecord();
	afx_msg void OnBnClickedButtonChangeSize();
};
