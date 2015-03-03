#include "StdAfx.h"
#include <time.h>
#include ".\maindlg.h"

//#include "../../include/RtmpLibForSdk/IRtmpLib.h"
//class TestRtmp
//	: public IRtmpLibNotify
//{
//public:
//	TestRtmp(){
//		m_server = CreateRtmpLib(this);
//
//	}
//	~TestRtmp(){
//		m_server->Release();
//		m_server = NULL;
//	};
//
//	IRtmpLib *m_server;
//    void OnRtmpDead()
//	{
//	}
//    void OnRtmpPublishSuccess()
//	{
//	}
//    void OnRtmpPublishFaild()
//	{
//	}
//    void OnRtmpStatus(FLASHCmdPacket *status)
//	{
//		const char *p = status->strParamOne.c_str();
//		std::cout<< "On Status :" << status->strParamOne <<std::endl;
//	}
//    void OnRtmpNetInfo(FLASHCmdPacket *flashCmd)
//	{
//	}
//    bool OnRtmpCheckAudioBuffer()
//	{
//		return true;
//	}
//};
//TestRtmp *rtmp = NULL;

LRESULT CMainDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码

	
	return 0;
}

LRESULT CMainDlg::OnBnClickedButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	//m_ComboBoxAudioOutDev.GetCurSel();
	//m_pRtmpPlayManager = CreateRtmpPlayManager(this);
	
	//rtmp://lssplay.aodianyun.com:1935

	//::ShowWindow(GetDlgItem(IDC_STATIC).m_hWnd, SW_SHOW);
	char szTime[64] = {0};
	time_t curTime;
	time(&curTime);
	tm *pTime = NULL;
	pTime = localtime(&curTime);

	if (pTime)
	{
		sprintf(szTime,
			"%d:%d:%d.000 ",
			pTime->tm_hour,
			pTime->tm_min,
			pTime->tm_sec);
	}
	string strMsg = string(szTime) + "ClickStartButton";
	ShowMsg(strMsg);

	wchar_t buf[100] = {L'0'};
	GetDlgItem(IDC_EDIT_SERVER).GetWindowText(buf, sizeof(buf));
	m_strScheduleSvr = ConvertString::ws2s( wstring(buf) );
	GetDlgItem(IDC_EDIT_SITE).GetWindowText(buf, sizeof(buf));
	m_strPublishSiteInfo = ConvertString::ws2s( wstring(buf) );
	GetDlgItem(IDC_EDIT_STAGE).GetWindowText(buf, sizeof(buf));
	m_strPublishStageInfo = ConvertString::ws2s( wstring(buf) );
	GetDlgItem(IDC_EDIT_USERID).GetWindowText(buf, sizeof(buf));
	m_strUserID = ConvertString::ws2s( wstring(buf) );
	GetDlgItem(IDC_EDIT_KEY).GetWindowText(buf, sizeof(buf));
	m_strKey = ConvertString::ws2s( wstring(buf) );

	if(m_pRtmpPlayManager)
	{
		::MoveWindow(GetDlgItem(IDC_STATIC).m_hWnd, 0, 0, 320, 240, TRUE);
		
		string strApp = m_strPublishSiteInfo+"?"+m_strKey;
		m_pRtmpPlayManager->SetStreamInfo(m_strScheduleSvr.c_str(), /*m_strPublishSiteInfo*/strApp.c_str(), m_strPublishStageInfo.c_str()); 
		m_pRtmpPlayManager->SetAudioPlayDev(m_ComboBoxAudioOutDev.GetCurSel());
		m_pRtmpPlayManager->SetVdDisplayWnd(GetDlgItem(IDC_STATIC).m_hWnd);
		m_pRtmpPlayManager->SetAdPara(m_iAdOutVolum, m_bOutMute);
		m_pRtmpPlayManager->SetClientUserID(m_strUserID.c_str());
		m_pRtmpPlayManager->StartPlay();
	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedButton2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码

	return 0;
}
LRESULT CMainDlg::OnBnClickedButtonRecord(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
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
		GetDlgItem(IDC_BUTTON_ENABLE_RECORD).SetWindowText(L"停止");
	}
	else
	{
		if(m_pRtmpPlayManager)
		{
			m_pRtmpPlayManager->EnableRecordFile(m_bEnableRecord);
		}
		GetDlgItem(IDC_BUTTON_ENABLE_RECORD).SetWindowText(L"录制");
	}
	return 0;
}

LRESULT CMainDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if ( HWND(lParam) == GetDlgItem(IDC_SLIDER_OUTVOLUM).m_hWnd )
	{
		m_iAdOutVolum = m_sliderOutVolum.GetPos();
		
		if(m_pRtmpPlayManager)
		m_pRtmpPlayManager->SetAdPara(m_iAdOutVolum, m_bOutMute);
	}
	return S_OK;
}
bool CMainDlg::OnRtmpPlayManagerComputerStatusInfo(RtmpPublishComputerStatusInfo &RtmpComputerStatusInfo) 
{
	//@ Note that stack overflow
	char buf[500]={0};
	sprintf(buf, "%d", RtmpComputerStatusInfo.uTotalPhys/1000);
	string strNetInfo = string("总内存:")+string(buf)+"M\r\n";
	unsigned int uUsedMem = RtmpComputerStatusInfo.uTotalPhys-RtmpComputerStatusInfo.uAvailPhys;
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%dM(%d%%)", uUsedMem/1000, 
		RtmpComputerStatusInfo.uTotalPhys>0 ? uUsedMem*100/RtmpComputerStatusInfo.uTotalPhys : 0);
	strNetInfo+= string("已用内存:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", RtmpComputerStatusInfo.uTotalVirtual/1000);
	strNetInfo+= string("总虚拟内存:")+string(buf)+"M\r\n";
	unsigned int uUsedVirtualMem = RtmpComputerStatusInfo.uTotalVirtual - RtmpComputerStatusInfo.uAvailVirtual;
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%dM(%d%%)", uUsedVirtualMem/1000, 
		RtmpComputerStatusInfo.uTotalVirtual>0 ? uUsedVirtualMem*100/RtmpComputerStatusInfo.uTotalVirtual : 0) ;
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
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d ms", RtmpComputerStatusInfo.uPublishPing);
	strNetInfo+= string("上行ping:")+string(buf)+"\r\n";
	
	const char *p = new char [strNetInfo.length()+1];

	if (p)
	{
		memcpy((void *)p, strNetInfo.c_str(), strNetInfo.length()+1);
		PostMessage(MSG_SHOW_PLAY_PCSTATUS, 0, (LPARAM)p);
	}

	return true;
}

bool CMainDlg::OnRtmpPlayNotifyInJson(const char* pszJson)
{
	if (NULL == pszJson)
	{	return false;	}

	string strCmd = pszJson;
	ParseCmd(strCmd);
	return true;
}

bool CMainDlg::ParseCmd(const string &strCmd)
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
				}
			}
		}
	}

	strMsg = "Error:cmd must be json format.";
	ShowMsg(strMsg);
	return false;
}

bool CMainDlg::OnRtmpPlayManagerAVInfo(RtmpPlayAVInfo &RtmpPlayAVInfo) 
{
	//@ Note that stack overflow
	char buf[500]={0};
	sprintf(buf, "%.3f s", (float)RtmpPlayAVInfo.uTotalBufTime/1000);
	string strNetInfo = string("缓冲区时间:")+string(buf)+"\r\n";
	
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%.3f s", (float)RtmpPlayAVInfo.uAudioBufTime/1000);
	strNetInfo+= string("音频缓冲时间:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%.3f s", (float)RtmpPlayAVInfo.uVideoBufTime/1000);
	strNetInfo+= string("视频缓冲时间:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", RtmpPlayAVInfo.uVideoHeight) ;
	strNetInfo+= string("视频原始高度:")+string(buf)+"\r\n";
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", RtmpPlayAVInfo.uVideoWidth);
	strNetInfo+= string("视频原始宽度:")+string(buf)+"\r\n";

	const char *p = new char [strNetInfo.length()+1];

	if (p)
	{
		memcpy((void *)p, strNetInfo.c_str(), strNetInfo.length()+1);
		PostMessage(MSG_SHOW_PLAY_AVINFO, 0, (LPARAM)p);
	}
	return true;
}
LRESULT CMainDlg::OnShowPlayAVInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(lParam)
	{
		const char *p = (const char *)lParam;
		string strMsg = p;
		delete [] p;
		m_PlayAVInfoList.SetWindowText(ConvertString::s2ws(strMsg/*+"\r\n"*/).c_str());
	}
	return true;
}
LRESULT CMainDlg::OnShowPlayPCStatus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(lParam)
	{
		const char *p = (const char *)lParam;
		string strMsg = p;
		delete [] p;
		ShowPlayPCStatus(strMsg);
	}
	return 0;
}
LRESULT CMainDlg::OnShowPlayQuality(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(lParam)
	{
		const char *p = (const char *)lParam;
		string strMsg = p;
		delete [] p;
		ShowPlayQuality(strMsg);
	}
	return 0;
}
void CMainDlg::ShowPlayPCStatus(string &strMsg)
{
	m_PlayPCStatusList.SetWindowText(ConvertString::s2ws(strMsg/*+"\r\n"*/).c_str());
}
void CMainDlg::ShowPlayQuality(string &strMsg)
{
	m_PlayQualityList.SetWindowText(ConvertString::s2ws(strMsg/*+"\r\n"*/).c_str());
}
bool CMainDlg::OnRtmpPlayManagerNetInfo(RtmpPublishNetInfo &RtmpNetInfo) 
{
	//DWORD dwCurTime = timeGetTime();
	//DWORD dwCurTime = GetTickCount();
	//if(m_lLastTime==0)
	//{
	//	m_lLastTime = dwCurTime;
	//	m_lPingSum += RtmpNetInfo.uPing;
	//	m_lTimes =0 ;
	//}
	//else if(m_lLastTime> 0 && dwCurTime-m_lLastTime < 1000)
	//{
	//	m_lPingSum += RtmpNetInfo.uPing;
	//	m_lTimes++;
	//}
	//else
	//{
	//	m_lLastTime = dwCurTime;
	//	m_lPingSum += RtmpNetInfo.uPing;
	//	m_lTimes++;
	//	//@ Risk to memory over-boundary
	//	char buf[50]={0};
	//	sprintf(buf, "%d", m_lPingSum/m_lTimes);
	//	m_lTimes = 0;
	//	m_lPingSum = 0;
	//	string strNetInfo = string("Ping:")+string(buf)+"\r\n";
	//	memset(buf, 0, sizeof(buf));
	//	sprintf(buf, "%d", RtmpNetInfo.uAudioBitSum);
	//	strNetInfo+= string("AudioBitSum:")+string(buf)+"\r\n";
	//	memset(buf, 0, sizeof(buf));
	//	sprintf(buf, "%d", RtmpNetInfo.uAudioFrameSum);
	//	strNetInfo+= string("AudioFrameSum:")+string(buf)+"\r\n";
	//	memset(buf, 0, sizeof(buf));
	//	sprintf(buf, "%d", RtmpNetInfo.uDelay);
	//	strNetInfo+= string("Delay:")+string(buf)+"\r\n";
	//	memset(buf, 0, sizeof(buf));
	//	sprintf(buf, "%d", RtmpNetInfo.uVideoBitSum);
	//	strNetInfo+= string("VideoBitSum:")+string(buf)+"\r\n";
	//	memset(buf, 0, sizeof(buf));
	//	sprintf(buf, "%d", RtmpNetInfo.uVideoFrameSum);
	//	strNetInfo+= string("VideoFrameSum:")+string(buf)+"\r\n";
	//	memset(buf, 0, sizeof(buf));
	//	sprintf(buf, "%d", RtmpNetInfo.uDiscardBytesSum);
	//	strNetInfo+= string("DiscardBytesSum:")+string(buf)+"\r\n";
	//	const char *p = new char [strNetInfo.length()+1];
	//	memcpy((void *)p, strNetInfo.c_str(), strNetInfo.length()+1);
	//	PostMessage(MSG_SHOW_PUBLISH_NETINFO, 0, (LPARAM)p);
	//}
	//

	//@ Risk to memory over-boundary
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
		strNetInfo+= string("音频码率:")+string(buf)+"\r\n";
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%.3f kbps", RtmpNetInfo.fVideoBitRate);
		strNetInfo+= string("视频码率:")+string(buf)+"\r\n";
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%f kbps", RtmpNetInfo.fAudioBitRate + RtmpNetInfo.fVideoBitRate);
		strNetInfo+= string("当前码率:")+string(buf)+"\r\n";
		
		
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%d", RtmpNetInfo.uVideoFrameRate);
		strNetInfo+= string("当前帧率:")+string(buf)+"\r\n";
		
		const char *p = new char [strNetInfo.length()+1];

		if (p)
		{
			memcpy((void *)p, strNetInfo.c_str(), strNetInfo.length()+1);
			PostMessage(MSG_SHOW_PUBLISH_NETINFO, 0, (LPARAM)p);
		}
	return true;
}
bool CMainDlg::OnRtmpPlayManagerMsg(eMsgLevel MsgLev, const char *pMsg, const char *pDetail) 
{
	if(pMsg == NULL)
		return false;
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
bool CMainDlg::OnRtmpPlayManagerPlayQuality(RtmpPlayQuality &RtmpPlayQuality) 
{
	//@ Risk to memory over-boundary
		char buf[500]={0};
		sprintf(buf, "%d ms", RtmpPlayQuality.uShowVideoSucTime);
		string strPlayQuality = string("显示视频花费时间:")+string(buf)+"\r\n";
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%d (次)", RtmpPlayQuality.uBufferEmptyCount);
		strPlayQuality+= string("缓冲区空:")+string(buf)+"\r\n";
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%d (次)", RtmpPlayQuality.uBufferFullCount);
		strPlayQuality+= string("缓冲区满:")+string(buf)+"\r\n";
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%d (ms)", RtmpPlayQuality.uMaxPlayBuffer);
		strPlayQuality+= string("缓冲区最大值:")+string(buf)+"\r\n";
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%d (ms)", RtmpPlayQuality.uMinPlayBuffer);
		strPlayQuality+= string("缓冲区最小值:")+string(buf)+"\r\n";
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%d (ms)", RtmpPlayQuality.uAvgPlayBuffer);
		strPlayQuality+= string("缓冲区平均值:")+string(buf)+"\r\n";
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%.2f (s)", RtmpPlayQuality.fTotalPlayTime);
		strPlayQuality+= string("总播放时间:")+string(buf)+"\r\n";
			
		const char *p = new char [strPlayQuality.length()+1];

		if (p)
		{
			memcpy((void *)p, strPlayQuality.c_str(), strPlayQuality.length()+1);
			PostMessage(MSG_SHOW_PLAYQUALITY, 0, (LPARAM)p);
		}
	return true;
}

void CMainDlg::OnShowVideo()
{
	string strMsg("Msg:OnShowVideo.");
	ShowMsg(strMsg);
	return;
}

LRESULT CMainDlg::OnShowSpectrum(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

void CMainDlg::ShowMsg(string &strMsg)
{
	/*int nStartChar = 0;
	int nEndChar = 0;
	m_MsgList.GetSel(nStartChar, nEndChar);
	m_MsgList.ReplaceSel(ConvertString::s2ws(strMsg).c_str());*/
	
	//@ CEdit
	// int iLineNum = m_MsgList.GetLineCount(); 
	// if (iLineNum <= 200) 
	// {
	//	 m_MsgList.SetSel(-1); 
	//	// m_EditLog.ReplaceSel(str + "\r\n\r\n"); 
	//	 m_MsgList.ReplaceSel(ConvertString::s2ws(strMsg+"\r\n").c_str());
	// } 
	//else
	// { 
	//	 m_MsgList.SetSel(0, -1); 
	//	 m_MsgList.Clear(); 
	// }

	//@CListBox
	int iLineNum = m_MsgList.GetCount();
	 if (iLineNum <= 200) 
	 {
		 //m_MsgList.AddString(ConvertString::s2ws(strMsg).c_str());
		 m_MsgList.InsertString(0, ConvertString::s2ws(strMsg).c_str());
	 } 
	 else
	 { 
		 //m_MsgList.DeleteString(0); 
		 m_MsgList.DeleteString(iLineNum-1);
	 }
}
LRESULT CMainDlg::OnBnClickedButtonBmute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	m_bOutMute = !m_bOutMute;
	if(m_bOutMute)
	{
		GetDlgItem(IDC_BUTTON_BMUTE).SetWindowText(L"noise");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_BMUTE).SetWindowText(L"mute");
	}
	if(m_pRtmpPlayManager)
		m_pRtmpPlayManager->SetAdPara(m_iAdOutVolum, m_bOutMute);
	return 0;
}
CMainDlg::CMainDlg()
: m_bOutMute(false)
, m_iAdOutVolum(80)
, m_strScheduleSvr("rtmp://lssplay.aodianyun.com:1935")
, m_strPublishSiteInfo("app")
, m_strPublishStageInfo("stream")
, m_strKey("yun")
, m_lLastTime(0)
, m_lTimes(0)
, m_lPingSum(0)
, m_pRtmpPlayManager2(NULL)
, m_strUserID("10000")
, m_bEnableRecord(false)
, m_strFileName("d:\\a.flv")
, m_bDirectConSvr(false)
, m_bUseUDP(false)
{
}

LRESULT CMainDlg::OnBnClickedButtonStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	char szTime[64] = {0};
	time_t curTime;
	time(&curTime);
	tm *pTime = NULL;
	pTime = localtime(&curTime);

	if (pTime)
	{
		sprintf(szTime,
			"%d:%d:%d.000 ",
			pTime->tm_hour,
			pTime->tm_min,
			pTime->tm_sec);
	}
	string strMsg = string(szTime) + "ClickStopButton";
	ShowMsg(strMsg);

	if(m_pRtmpPlayManager)
		m_pRtmpPlayManager->StopPlay();
	//::ShowWindow(GetDlgItem(IDC_STATIC).m_hWnd, SW_HIDE);
	/*if(m_pRtmpPlayManager2)
		m_pRtmpPlayManager2->StopPlay();*/
	return 0;
}

LRESULT CMainDlg::OnShowMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
void CMainDlg::ShowPlayNetInfo(string &strMsg)
{
	//@ Cover show
	m_PlayNetInfoList.SetWindowText(ConvertString::s2ws(strMsg/*+"\r\n"*/).c_str());
}
LRESULT CMainDlg::OnShowPlayNetInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(lParam)
	{
		const char *p = (const char *)lParam;
		string strMsg = p;
		delete [] p;
		ShowPlayNetInfo(strMsg);
	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonCopymsg(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	int msgsum = m_MsgList.GetCount();
	wstring wstrMsg;
	for(int i= 0; i<msgsum; i++)
	{
		//m_MsgList.GetItemText();
		int strLen = m_MsgList.GetTextLen(i);
		wchar_t *pDat = new wchar_t[strLen];

		if (pDat)
		{
			m_MsgList.GetText(i, pDat);
			wstrMsg += wstring(pDat) + wstring(L"\r\n");
		}
	}

	HGLOBAL clipbuffer = GlobalAlloc(GMEM_DDESHARE, 2*(wstrMsg.length() + 1));
	TCHAR* buffer = (TCHAR*) GlobalLock(clipbuffer);
	StrCpy((LPWSTR)buffer, (LPCWSTR)wstrMsg.c_str());
	GlobalUnlock(clipbuffer);

	OpenClipboard();// 打开剪贴板
	EmptyClipboard();// 清空剪贴板的内容
	SetClipboardData(CF_UNICODETEXT, clipbuffer);// 将数据放到剪贴板里
	CloseClipboard(); // 关闭剪贴板
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonChangeSize(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static bool change = true;
	if (change)
	{
		::MoveWindow(GetDlgItem(IDC_STATIC).m_hWnd, 0, 0, 160, 120, TRUE);
	}
	else
	{
		::MoveWindow(GetDlgItem(IDC_STATIC).m_hWnd, 0, 0, 320, 240, TRUE);
	}
	change = !change;

	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonDirectCon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_bDirectConSvr = !m_bDirectConSvr;
	if(m_bDirectConSvr)
	{
		GetDlgItem(IDC_BUTTON_ISDIRECTCON).SetWindowText(L"直连");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_ISDIRECTCON).SetWindowText(L"调度");
	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonUDP(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_bUseUDP = !m_bUseUDP;
	if(m_bUseUDP)
	{
		GetDlgItem(IDC_BUTTON_UDP).SetWindowText(L"UDP");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_UDP).SetWindowText(L"TCP");
	}
	return 0;
}

string CMainDlg::GetSdkVersion()
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

	if (pszRes)
	{
		delete[] pszRes;
		pszRes = NULL;
	}

	return strSdkVersion;
}

LRESULT CMainDlg::OnBnClickedButtonSetBufferTime(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	
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

	//* 调    用：pszCmd { "cmd":"SetSYNBufTime", "params":{"dwDatalenOffset":1500, "dwMaxTimestampleInqueue":3000, "dwTimeCheckSynbuf":5000} }
	if (m_pRtmpPlayManager && pszRes)
	{
		m_pRtmpPlayManager->CallInJson(strCmd.c_str(), &pszRes);
	}

	if (pszRes)
	{
		delete[] pszRes;
		pszRes = NULL;
	}

	return 0;
}
