// testPublishDlg.h : header file
//

#if !defined(AFX_TESTPUBLISHDLG_H__71E55966_2937_4288_8E4E_52FA84481B3D__INCLUDED_)
#define AFX_TESTPUBLISHDLG_H__71E55966_2937_4288_8E4E_52FA84481B3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RtmpPublishManager/IRtmpPublishManager.h"
#include "jsonPraser/json.h"
#include "ConvertString.h"

#define					MSG_SHOWMSG									WM_USER+1001
#define					MSG_SHOW_PUBLISH_NETINFO					WM_USER+1002
#define					MSG_SHOW_PUBLISH_PCSTATUS					WM_USER+1003
#define					MSG_SHOW_AUDIO_SPECTRUM						WM_USER+1004


class CTestPublishDlg : public CDialog,public IRtmpPublishManagerNotify
{
// Construction
public:
	CTestPublishDlg(CWnd* pParent = NULL);	// standard constructor

public:
	virtual bool OnRtmpPublishNotifyInJson(const char* pszJson);
	virtual bool OnRtmpPublishManagerMsg(eMsgLevel MsgLev, const char *pMsg);
	virtual bool OnRtmpPublishManagerNetInfo(RtmpPublishNetInfo &RtmpNetInfo);
	virtual bool OnRtmpPublishManagerComputerStatusInfo(RtmpPublishComputerStatusInfo &RtmpStatusInfo);

private:
	IRtmpPublishManager * m_pRtmpManage;
	CComboBox							m_ComboBoxVideo;
	CComboBox							m_ComboBoxAudio;
	CComboBox							m_ComboBoxThirdPartPlayer;

	void	 Init();
	void ShowMsg(string &strMsg);
	void ShowPublishNetInfo(string &strMsg);
	void SetDefaultPara();
	void GetPublishPara();
	string GetSdkVersion();
	bool ParseCmd(const string &strCmd);
	void SetShowSpectrum(bool bShow);
	void SetAutoCtrlBitrate(bool bAuto);
	void ShowPublishPCStatus(string &strMsg);

	//@ server
	string m_strScheduleSvr;
	string m_strPublishRoomInfo;
	string m_strPublishStageInfo;
	string m_strKey;
	string	m_strUserID;

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

	CListBox										m_MsgList;
	CEdit										m_PublishNetInfoList;
	CEdit										m_PublishPCStatusList;
	CSliderCtrl						m_sliderVolum;
	//CSliderCtrl						m_sliderOtherPlayerVolum;
	CSliderCtrl				m_sliderShowSpectrum;
	Json::Value				m_jsonRoot;
	Json::Value				m_jsonParams;

	bool					m_bCapStereo;
	bool					m_bCapMic;
// Dialog Data
	//{{AFX_DATA(CTestPublishDlg)
	enum { IDD = IDD_TESTPUBLISH_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestPublishDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestPublishDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	afx_msg LRESULT OnShowMsg(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnShowPublishNetInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowPublishPCStauts(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHScroll(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonShowspm();
	afx_msg void OnBnClickedButtonChangeSize();
	afx_msg void OnBnClickedButtonStereo();
	afx_msg void OnBnClickedButtonMic();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTPUBLISHDLG_H__71E55966_2937_4288_8E4E_52FA84481B3D__INCLUDED_)
