#include "stdafx.h"
//#include "vld/vld.h"
#include "MainDlg.h"
#include ".\maindlg.h"
#include <vector>
#include <string>
#include <assert.h>
using std::vector;
using std::wstring;

#include "jsonPraser/json.h"
#include "ConvertString.h"
#include "atlmisc.h"
//#include "atlstr.h"
//using WTL::CString;

void CheckOnExit(){
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	int mCount = _CrtDumpMemoryLeaks();
	assert( mCount == 0);
}
void	CMainDlg::Init()
{
	m_pRtmpManage = CreateRtmpPublishManager(this);
	string strSdkVersion = GetSdkVersion();

	if (strSdkVersion.empty())
	{	strSdkVersion = "SdkVer:get failed.";	}
	else
	{	strSdkVersion = "SdkVer:" + strSdkVersion;	}

	ShowMsg(strSdkVersion);
	wchar_t** ppCam = NULL;
	unsigned int size = 0;
	m_pRtmpManage->GetCamList(ppCam, size);

	vector<wstring> vecCam ;
	for(unsigned int i=0; i<size; i++)
	{
		vecCam.push_back(ppCam[i]);
		m_ComboBoxVideo.InsertString(m_ComboBoxVideo.GetCount(), ppCam[i]);
	}

	wchar_t** ppMic = NULL;
	m_pRtmpManage->GetMicList(ppMic, size);

	vector<wstring> vecMic ;
	for(unsigned int i=0; i<size; i++)
	{
		vecMic.push_back(ppMic[i]);
		m_ComboBoxAudio.InsertString(m_ComboBoxAudio.GetCount(), ppMic[i]);
	}

	m_pAudioVideoPreview = CreateAudioVideoPreview(this);
	wchar_t** ppAudioPlayDev = NULL;
	//m_pRtmpManage->GetAudioPlayDevList (ppAudioPlayDev, size);
	m_pAudioVideoPreview->GetAudioPlayDevList (ppAudioPlayDev, size);

	vector<wstring> vecAudioPlayDev ;
	for(unsigned int i=0; i<size; i++)
	{
		vecAudioPlayDev.push_back(ppAudioPlayDev[i]);
		m_ComboBoxAudioPlay.InsertString(m_ComboBoxAudioPlay.GetCount(), ppAudioPlayDev[i]);
	}

	vector<string> vecOtherPlayer ;
	if(m_pRtmpManage)
	{
		m_pRtmpManage->GetPlayerList(vecOtherPlayer);
		size = vecOtherPlayer.size();
		for(unsigned int i=0; i<size; i++)
		{
			m_ComboBoxThirdPartPlayer.InsertString(m_ComboBoxThirdPartPlayer.GetCount(), 
				ConvertString::s2ws(vecOtherPlayer[i]).c_str());
		}
		if(!vecOtherPlayer.empty())
		{
			m_ComboBoxThirdPartPlayer.SetCurSel(0);
		}
	}

	m_bCapPlayer = false;
	m_bRestartProcess = true;
	m_bCaptureBackground = false;
	////build json
	//Json::Value root;  
	//Json::Value arrayObj;  
	//Json::Value item;

	//root.clear();
	//arrayObj.clear();
	//item.clear();

	//item["para1"] = 1;
	//item["para2"] = 2;
	//arrayObj.append(item);
	//root["cmd"]="GetCamList";
	//root["para"]=arrayObj;
	//string strjson = root.toStyledString();

	////@ call json interface
	//char *pJson = NULL;
	//m_pRtmpManage->Call(strjson.c_str(), &pJson);
	//string strRetJson = pJson;
	//m_pRtmpManage->JsonFree(pJson);

	////@ prase json result
	//Json::Reader reader;  
	////Json::Value root;  
	//if (reader.parse(strRetJson.c_str(), root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
	//{  
	//	bool bSuc = root["success"].asBool(); 
	//	if(bSuc)
	//	{
	//		Json::Value arrInfo = root["info"];
	//		int size = arrInfo.size();
	//		for ( int index =0; index < size; ++index )
	//		{
	//			wstring wstr = ConvertString::s2ws( arrInfo[index].asString());
	//			m_ComboBoxVideo.InsertString(m_ComboBoxVideo.GetCount(), (wstr).c_str());
	//		}
	//	}
	//}
	//root.clear();
	//arrayObj.clear();
	//item.clear();

	//item["para1"] = 1;
	//item["para2"] = 2;
	//arrayObj.append(item);
	//root["cmd"]="GetMicList";
	//root["para"]=arrayObj;
	//strjson = root.toStyledString();

	//m_pRtmpManage->Call(strjson.c_str(), &pJson);
	//strRetJson = pJson;
	//m_pRtmpManage->JsonFree(pJson);
	////@ prase json result
	////Json::Reader reader;  
	////Json::Value root;  
	//if (reader.parse(strRetJson.c_str(), root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
	//{  
	//	bool bSuc = root["success"].asBool(); 
	//	if(bSuc)
	//	{
	//		Json::Value arrInfo = root["info"];
	//		int size = arrInfo.size();
	//		for ( int index =0; index < size; ++index )
	//		{
	//			wstring wstr = ConvertString::s2ws( arrInfo[index].asString());
	//			m_ComboBoxAudio.InsertString(m_ComboBoxAudio.GetCount(), (wstr).c_str());
	//		}
	//	}
	//}


	//test
	SetDefaultPara();

	//@ Show default config
	GetDlgItem(IDC_EDIT_SERVER).SetWindowText(ConvertString::s2ws(m_strScheduleSvr).c_str());
	GetDlgItem(IDC_EDIT_APP).SetWindowText(ConvertString::s2ws(m_strPublishRoomInfo).c_str());
	GetDlgItem(IDC_EDIT_STAGEID).SetWindowText(ConvertString::s2ws(m_strPublishStageInfo).c_str());
	GetDlgItem(IDC_EDIT_USERID).SetWindowText(ConvertString::s2ws(m_strUserID).c_str());
	GetDlgItem(IDC_EDIT_KEY).SetWindowText(ConvertString::s2ws(m_strKey).c_str());

	wchar_t buf[30]={L'\0'};
	swprintf(buf, L"%d", m_iVdWidth);
	wstring wstrWidth = buf;
	memset(buf, 0, sizeof(buf));
	swprintf(buf, L"%d", m_iVdHeight);
	wstring wstrHeight = buf;
	memset(buf, 0, sizeof(buf));
	swprintf(buf, L"%d", m_iVdFps);
	wstring wstrVdFps = buf;
	memset(buf, 0, sizeof(buf));
	swprintf(buf, L"%d", m_iVdBitRate);
	wstring wstrVdBitRate = buf;
	memset(buf, 0, sizeof(buf));
	swprintf(buf, L"%d", m_iBFrame);
	wstring wstrBframe = buf;
	memset(buf, 0, sizeof(buf));
	swprintf(buf, L"%d", m_iQuality);
	wstring wstrQuality = buf;
	memset(buf, 0, sizeof(buf));
	swprintf(buf, L"%d", m_iVdInterval);
	wstring wstrVdInterval = buf;

	memset(buf, 0, sizeof(buf));
	swprintf(buf, L"%d", m_iAdSampleRate);
	wstring wstrAdSampleRate = buf;
	memset(buf, 0, sizeof(buf));
	swprintf(buf, L"%d", m_iAdBitRate);
	wstring wstrAdBitRate = buf;
	memset(buf, 0, sizeof(buf));
	swprintf(buf, L"%d", m_iAdChannels);
	wstring wstrAdChannels = buf;
	memset(buf, 0, sizeof(buf));
	swprintf(buf, L"%d", m_iAdBitPerSample);
	wstring wstrAdBitperSample = buf;

	GetDlgItem(IDC_EDIT_VIDEOWIDTH).SetWindowText(wstrWidth.c_str());
	GetDlgItem(IDC_EDIT_VIDEOHEIGHT).SetWindowText(wstrHeight.c_str());
	GetDlgItem(IDC_EDIT_VIDEOFRAMERATE).SetWindowText(wstrVdFps.c_str());
	GetDlgItem(IDC_EDIT_VIDEOBITRATE).SetWindowText(wstrVdBitRate.c_str());
	GetDlgItem(IDC_EDIT_VIDEOKEYFRAMEINTERVAL).SetWindowText(wstrVdInterval.c_str());
	GetDlgItem(IDC_EDIT_BFRAME).SetWindowText(wstrBframe.c_str());
	GetDlgItem(IDC_EDIT_QUALITY).SetWindowText(wstrQuality.c_str());
	
	GetDlgItem(IDC_EDIT_SAMPLERATE).SetWindowText(wstrAdSampleRate.c_str());
	GetDlgItem(IDC_EDIT_CHANNELS).SetWindowText(wstrAdChannels.c_str());
	GetDlgItem(IDC_EDIT_BITPERSAMPLE).SetWindowText(wstrAdBitperSample.c_str());
	GetDlgItem(IDC_EDIT_AUDIOBITRATE).SetWindowText(wstrAdBitRate.c_str());

	m_ComboBoxAudio.SetCurSel(0);
	m_ComboBoxVideo.SetCurSel(0);
	m_ComboBoxAudioPlay.SetCurSel(0);

	m_bCapStereo = false;
	m_bCapMic = true;

	m_sliderOtherPlayerVolum.SetPos(m_iOtherPlayerVolum);
	m_sliderVolum.SetPos(m_iAdVolum);

	m_bEnableRecord = false;
	m_strFileName = "d:\\a.flv";
	m_bDirectConSvr = false;
	m_bUseUDP = true;

	m_bPreviewAudio = false;
	m_bPreviewVideo = false;
}

void CMainDlg::PublishPara2Json(string &strRetJson)
{
	Json::Value root;  
	Json::Value arrayObj;  
	Json::Value item;
	Json::Value item1;

	root.clear();
	arrayObj.clear();
	item.clear();
	item1.clear();
	//@ video
	item["fps"] = m_iVdFps;
	item["bitrate"] = m_iVdBitRate;
	item["width"] = m_iVdWidth;
	item["height"] = m_iVdHeight;
	item["quality"] = m_iVdQuality;
	item["vddisplay_wnd"] = (int)(GetDlgItem(IDC_STATIC_DISPLAY_PUBLISH).m_hWnd);

	item1["videopara"] = item;
	//arrayObj.append(item1);

	item.clear();
	//item1.clear();
	//@ audio
	item["samplerate"] = m_iAdSampleRate;
	item["bitrate"] = m_iAdBitRate;
	item["channel"] = m_iAdChannels;
	item["bitpersample"] = m_iAdBitPerSample;
	item["volum"] = m_iAdVolum;
	item["mute"] = m_bAdMute;

	item1["audiopara"] = item;
	//arrayObj.append(item1);

	item.clear();
	//item1.clear();
	//@ stream
	item["schedulesvr"] = m_strScheduleSvr;
	item["site"] = m_strPublishRoomInfo;		//@站点
	item["stage"] = m_strPublishStageInfo;		//@麦id
	
	item1["streampara"] = item;
	//arrayObj.append(item1);

	root["cmd"]="SetPublishPara";
	root["para"]=item1;
	strRetJson = root.toStyledString();
}


void CMainDlg::Cmd2Json(string &strRetJson, string &strCmd)
{
	Json::Value root;  

	//root["cmd"]="StartPublish";
	root["cmd"]=strCmd.c_str();
	strRetJson = root.toStyledString();
}


void CMainDlg::SetDefaultPara()
{
	 //m_strScheduleSvr = "rtmp.aodianyun.com:1936";
	 m_strScheduleSvr = "rtmp://rtmp.aodianyun.com:1935";
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

	 m_bOtherPlayerMute = false;
	 m_iOtherPlayerVolum = 80;
}

void CMainDlg::GetPublishPara()
{


	//m_srvstr = ip;

	wchar_t buf[100] = {L'0'};
	GetDlgItem(IDC_EDIT_SERVER).GetWindowText(buf, sizeof(buf));
	m_strScheduleSvr = ConvertString::ws2s( wstring(buf) );
	GetDlgItem(IDC_EDIT_APP).GetWindowText(buf, sizeof(buf));
	m_strPublishRoomInfo = ConvertString::ws2s( wstring(buf) );
	GetDlgItem(IDC_EDIT_STAGEID).GetWindowText(buf, sizeof(buf));
	m_strPublishStageInfo = ConvertString::ws2s( wstring(buf) );
	GetDlgItem(IDC_EDIT_USERID).GetWindowText(buf, sizeof(buf));
	m_strUserID = ConvertString::ws2s( wstring(buf) );
	GetDlgItem(IDC_EDIT_KEY).GetWindowText(buf, sizeof(buf));
	m_strKey = ConvertString::ws2s( wstring(buf) );

	GetDlgItem(IDC_EDIT_VIDEOWIDTH).GetWindowText(buf, sizeof(buf));
	m_iVdWidth = _wtoi(buf);
	GetDlgItem(IDC_EDIT_VIDEOHEIGHT).GetWindowText(buf, sizeof(buf));
	m_iVdHeight = _wtoi(buf);
	GetDlgItem(IDC_EDIT_VIDEOFRAMERATE).GetWindowText(buf, sizeof(buf));
	m_iVdFps = _wtoi(buf);
	GetDlgItem(IDC_EDIT_VIDEOBITRATE).GetWindowText(buf, sizeof(buf));
	m_iVdBitRate = _wtoi(buf) * 1000;
	GetDlgItem(IDC_EDIT_VIDEOKEYFRAMEINTERVAL).GetWindowText(buf, sizeof(buf));
	m_iVdInterval = _wtoi(buf);
	GetDlgItem(IDC_EDIT_BFRAME).GetWindowText(buf,sizeof(buf));
	m_iBFrame = _wtoi(buf);
	GetDlgItem(IDC_EDIT_QUALITY).GetWindowText(buf,sizeof(buf));
	m_iQuality = _wtoi(buf);
	
	
	GetDlgItem(IDC_EDIT_SAMPLERATE).GetWindowText(buf, sizeof(buf));
	m_iAdSampleRate = _wtoi(buf);
	GetDlgItem(IDC_EDIT_CHANNELS).GetWindowText(buf, sizeof(buf));
	m_iAdChannels = _wtoi(buf);
	GetDlgItem(IDC_EDIT_BITPERSAMPLE).GetWindowText(buf, sizeof(buf));
	m_iAdBitPerSample = _wtoi(buf);
	GetDlgItem(IDC_EDIT_AUDIOBITRATE).GetWindowText(buf, sizeof(buf));
	m_iAdBitRate = _wtoi(buf) * 1000;

}
LRESULT CMainDlg::OnBnClickedButtonStart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	/*m_pRtmpManage->SetCam(m_ComboBoxVideo.GetCurSel());
	m_pRtmpManage->SetMic(m_ComboBoxAudio.GetCurSel());
	m_pRtmpManage->StartPublish();*/
	//atexit(CheckOnExit);
	GetPublishPara();
	CheckDlgButton(IDC_CHECK_AUTOCTRLBITRATE, BST_UNCHECKED);
	////build json
	//Json::Value root;  
	//Json::Value arrayObj;  
	//Json::Value item;

	//root.clear();
	//arrayObj.clear();
	//item.clear();

	//item["id"] = m_ComboBoxVideo.GetCurSel();
	////arrayObj.append(item);
	//root["cmd"]="SetCam";
	//root["para"]=item;
	//string strjson = root.toStyledString();
	//m_pRtmpManage->Call(strjson.c_str());

	//root.clear();
	//arrayObj.clear();
	//item.clear();

	//item["id"] = m_ComboBoxAudio.GetCurSel();
	////arrayObj.append(item);
	//root["cmd"]="SetMic";
	//root["para"]=item;
	//strjson = root.toStyledString();
	//m_pRtmpManage->Call(strjson.c_str());

	//string strPublishJson;
	//PublishPara2Json(strPublishJson);
	//m_pRtmpManage->Call(strPublishJson.c_str());

	//string strcmd = "StartPublish";
	//Cmd2Json(strjson, strcmd);
	//m_pRtmpManage->Call(strjson.c_str());

	if(m_pRtmpManage == NULL)
	{
		m_pRtmpManage = CreateRtmpPublishManager(this);
		vector<string> vecOtherPlayer ;
		m_pRtmpManage->GetPlayerList(vecOtherPlayer);
	}
	if(m_pRtmpManage)
	{
		::MoveWindow(GetDlgItem(IDC_STATIC_DISPLAY_PUBLISH).m_hWnd, 0, 0, 320, 240, TRUE);

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
		m_pRtmpManage->SetPublishVideoPara(m_iVdWidth, m_iVdHeight, m_iVdFps, m_iVdBitRate, GetDlgItem(IDC_STATIC_DISPLAY_PUBLISH).m_hWnd, m_iVdInterval);
		string strApp = m_strPublishRoomInfo+"?"+m_strKey;
		m_pRtmpManage->SetPublishStreamPara(m_strScheduleSvr.c_str(), /*m_strPublishRoomInfo*/strApp.c_str(), m_strPublishStageInfo.c_str());
		m_pRtmpManage->SetPublishOtherPlayerVolum(m_iOtherPlayerVolum, m_bOtherPlayerMute);
		m_pRtmpManage->SetClientUserID(m_strUserID.c_str());
		//m_pRtmpManage->SetRecordFileFullName(m_strFileName.c_str());
		//m_pRtmpManage->EnableRecordFile(true);
		/*BOOL BRET;
		OnBnClickedButtonEnableRecord(0,0,0, BRET);*/
		m_pRtmpManage->StartPublish();
	}
	return 0;
}

//视频设备选择
LRESULT CMainDlg::OnVideoDevice(WORD wNotifyCode, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (CBN_SELCHANGE == wNotifyCode)
	{
		//CString strDevice;
		TCHAR temp[100] = {0};
		m_ComboBoxVideo.GetLBText(m_ComboBoxVideo.GetCurSel(),temp);

		m_wstrCam = temp;
		//m_uMicID = m_ComboBoxVideo.GetSel();
	}
	return 0;
}


LRESULT CMainDlg::OnAudioDevice(WORD wNotifyCode, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (CBN_SELCHANGE == wNotifyCode)
	{
		//CString strDevice;
		TCHAR temp[100] = {0};
		m_ComboBoxAudio.GetLBText(m_ComboBoxAudio.GetCurSel(),temp);

		m_wstrMic = temp;

	}
	return 0;
}

LRESULT CMainDlg::OnAudioPlayDevice(WORD wNotifyCode, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (CBN_SELCHANGE == wNotifyCode)
	{
		//CString strDevice;
		TCHAR temp[100] = {0};
		m_ComboBoxAudioPlay.GetLBText(m_ComboBoxAudioPlay.GetCurSel(),temp);

		m_wstrAudioPlay = temp;

	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	/*string strmsg = "stop";
	ShowMsg(strmsg);*/

	// TODO: 在此添加控件通知处理程序代码
	//@ stop
	/*string strcmd = "StopPublish";
	string strjson;
	Cmd2Json(strjson, strcmd);
	if(m_pRtmpManage)
	m_pRtmpManage->Call(strjson.c_str());*/

	/*if(m_pRtmpManage)
	m_pRtmpManage->StopPublish();*/
	FILE *fp = fopen("d:\\teststop.txt", "at+");
	if(fp)
	{
		fprintf(fp, "click stop:time:%d\n", GetTickCount());
		fclose(fp);
	}
	//@ release
	if(m_pRtmpManage)
	{
		m_pRtmpManage->StopPublish();
		m_pRtmpManage->Release();
		m_pRtmpManage = NULL;
	}
		
	return 0;
}
void CMainDlg::ShowPublishNetInfo(string &strMsg)
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
		 m_PublishNetInfoList.ReplaceSel(ConvertString::s2ws(strMsg+"\r\n").c_str());
	 } 
	 else
	 { 
		 m_PublishNetInfoList.SetSel(0, -1); 
		 m_PublishNetInfoList.Clear(); 
	 }
}

void CMainDlg::ShowPublishPCStatus(string &strMsg)
{	
	 m_PublishPCStatusList.SetSel(0, -1); 
	 m_PublishPCStatusList.Clear(); 
	 int iLineNum = m_PublishPCStatusList.GetLineCount(); 
	 if (iLineNum <= 200) 
	 {
		 m_PublishPCStatusList.SetSel(-1, -1); 
		// m_EditLog.ReplaceSel(str + "\r\n\r\n"); 
		 m_PublishPCStatusList.ReplaceSel(ConvertString::s2ws(strMsg+"\r\n").c_str());
	 } 
	 else
	 { 
		 m_PublishPCStatusList.SetSel(0, -1); 
		 m_PublishPCStatusList.Clear(); 
	 }
}

void CMainDlg::ShowMsg(string &strMsg)
{
	/*int nStartChar = 0;
	int nEndChar = 0;
	m_MsgList.GetSel(nStartChar, nEndChar);
	m_MsgList.ReplaceSel(ConvertString::s2ws(strMsg).c_str());*/
	
	// int iLineNum = m_MsgList.GetLineCount(); 
	 //if (iLineNum <= 200) 
	 //{
		// //m_MsgList.SetSel(-1, -1); 
		// m_MsgList.SetSel(-1); 
		//// m_EditLog.ReplaceSel(str + "\r\n\r\n"); 
		// m_MsgList.ReplaceSel(ConvertString::s2ws(strMsg+"\r\n").c_str());
	 //} 
	 //else
	 //{ 
		// m_MsgList.SetSel(0, -1); 
		// m_MsgList.Clear(); 
	 //}

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

bool CMainDlg::OnRtmpPublishNotifyInJson(const char* pszJson)
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

bool CMainDlg::OnRtmpPublishManagerNetInfo(RtmpPublishNetInfo &RtmpNetInfo) 
{
	/*static bool bs = true;
	if(bs)
	{
		bs = false;
		BOOL BRET;
		OnBnClickedButtonEnableRecord(0,0,0, BRET);
	}*/
	//@ Risk to memory over-boundary
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
bool CMainDlg::OnRtmpPublishManagerMsg(eMsgLevel MsgLev, const char *pMsg)
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
}

bool CMainDlg::OnRtmpPublishManagerComputerStatusInfo(RtmpPublishComputerStatusInfo &RtmpComputerStatusInfo) 
{
	/*static bool bs = true;
	if(bs)
	{
		bs = false;
		BOOL BRET;
		OnBnClickedButtonEnableRecord(0,0,0, BRET);
	}*/
	//@ Note that stack overflow
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
LRESULT CMainDlg::OnShowPublishNetInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
LRESULT CMainDlg::OnShowPublishPCStauts(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
LRESULT CMainDlg::OnBnClickedButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	m_bAdMute = !m_bAdMute;
	if(m_bAdMute)
	{
		GetDlgItem(IDC_BUTTON1).SetWindowText(L"noise");
	}
	else
	{
		GetDlgItem(IDC_BUTTON1).SetWindowText(L"mute");
	}

	////build json
	//Json::Value root;  
	//Json::Value arrayObj;  
	//Json::Value item;

	//root.clear();
	//arrayObj.clear();
	//item.clear();

	//item["bMute"] = m_bAdMute;
	////arrayObj.append(item);
	//root["cmd"]="ChangeMute";
	//root["para"]=item;
	//string strjson = root.toStyledString();
	//if(m_pRtmpManage)
	//m_pRtmpManage->Call(strjson.c_str());

	if(m_pRtmpManage)
	m_pRtmpManage->SetPublishVolum(m_iAdVolum, m_bAdMute);

	/*BOOL BRET;
		OnBnClickedButtonEnableRecord(0,0,0, BRET);*/
	return 0;
}

LRESULT CMainDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if ( HWND(lParam) == GetDlgItem(IDC_SLIDER_VOLUM).m_hWnd )
	{
		m_iAdVolum = m_sliderVolum.GetPos();
		////build json
		//Json::Value root;  
		//Json::Value arrayObj;  
		//Json::Value item;

		//root.clear();
		//arrayObj.clear();
		//item.clear();

		//item["volum"] = m_iAdVolum;
		////arrayObj.append(item);
		//root["cmd"]="ChangeVolum";
		//root["para"]=item;
		//string strjson = root.toStyledString();
		//if(m_pRtmpManage)
		//m_pRtmpManage->Call(strjson.c_str());

		if(m_pRtmpManage)
		m_pRtmpManage->SetPublishVolum(m_iAdVolum, m_bAdMute);
	}
	else if(HWND(lParam) == GetDlgItem(IDC_SLIDER_OTHERPLAYER).m_hWnd )
	{
		m_iOtherPlayerVolum = m_sliderOtherPlayerVolum.GetPos();
		if(m_pRtmpManage)
		m_pRtmpManage->SetPublishOtherPlayerVolum(m_iOtherPlayerVolum, m_bOtherPlayerMute);
	}
	return S_OK;
}
	
void CMainDlg::PlayPara2Json(string &strRetJson)
{
	Json::Value root;  
	Json::Value arrayObj;  
	Json::Value item;
	Json::Value item1;

	root.clear();
	arrayObj.clear();
	item.clear();
	item1.clear();
	//@ video
	//item["vddisplay_wnd"] = (int)(GetDlgItem(IDC_STATIC_DISPLAY_PLAY).m_hWnd);

	item1["videopara"] = item;
	//arrayObj.append(item1);

	item.clear();
	//item1.clear();
	//@ audio
	item["volum"] = m_iAdVolum;
	item["mute"] = m_bAdMute;

	item1["audiopara"] = item;
	item.clear();
	//@ stream
	item["schedulesvr"] = m_strScheduleSvr;
	item["site"] = m_strPublishRoomInfo;		//@站点
	item["stage"] = m_strPublishStageInfo;		//@麦id
	
	item1["streampara"] = item;
	//arrayObj.append(item1);

	root["cmd"]="SetPlayPara";
	root["para"]=item1;
	strRetJson = root.toStyledString();
}

LRESULT CMainDlg::OnBnClickedButtonStartPlay(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	string strPlayJson;
	PlayPara2Json(strPlayJson);
	//m_pRtmpManage->Call(strPlayJson.c_str());

	string strcmd = "StartPlay";
	string strjson;
	Cmd2Json(strjson, strcmd);
	//m_pRtmpManage->Call(strjson.c_str());
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonStereo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	m_bCapStereo = true;
	m_bCapMic = false;
	if(m_pRtmpManage)
	{
		m_pRtmpManage->CapStereo(m_bCapStereo);
	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonMic(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	m_bCapStereo = false;
	m_bCapMic = true;
	if(m_pRtmpManage)
	{
		m_pRtmpManage->CapStereo(m_bCapStereo);
	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonCopymsg(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	//SelItemRange(TRUE, 0, m_listbox.GetCount()-1);

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
		//::MoveWindow(GetDlgItem(IDC_STATIC_DISPLAY_PUBLISH).m_hWnd, 0, 0, 160, 120, TRUE);
		::MoveWindow(GetDlgItem(IDC_STATIC_DISPLAY_PUBLISH).m_hWnd, 0, 0, 240, 180, TRUE);
	}
	else
	{
		::MoveWindow(GetDlgItem(IDC_STATIC_DISPLAY_PUBLISH).m_hWnd, 0, 0, 320, 240, TRUE);
	}
	change = !change;

	return 0;
}

LRESULT CMainDlg::OnBnClickedButton2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	m_bCapPlayer = !m_bCapPlayer;
	if(m_bCapPlayer)
	GetDlgItem(IDC_BUTTON2).SetWindowTextW(L"NotCap");
	else
		GetDlgItem(IDC_BUTTON2).SetWindowTextW(L"Capture");

	if(m_pRtmpManage)
	{
		wchar_t buf[100] = {L'\0'};
		m_ComboBoxThirdPartPlayer.GetWindowText(buf, sizeof(buf)/2);
		m_pRtmpManage->SelectPlayerName(ConvertString::ws2s(wstring(buf)));
		m_pRtmpManage->WhetherToRestartTargetProcess(m_bRestartProcess);
		m_pRtmpManage->EnableHookPlayer(m_bCapPlayer);
	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonIsrestartProcess(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	m_bRestartProcess = !m_bRestartProcess;
	if(m_bRestartProcess)
	GetDlgItem(IDC_BUTTON_ISRESTART_PROCESS).SetWindowTextW(L"重启进程");
	else
		GetDlgItem(IDC_BUTTON_ISRESTART_PROCESS).SetWindowTextW(L"不重启进程");
	return 0;
}
LRESULT CMainDlg::OnBnClickedButtonOtherPlayerMute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_bOtherPlayerMute = !m_bOtherPlayerMute;
	if(m_bOtherPlayerMute)
	{
		GetDlgItem(IDC_BUTTON_OTHERPLAYER_MUTE).SetWindowText(L"noise");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_OTHERPLAYER_MUTE).SetWindowText(L"mute");
	}
	if(m_pRtmpManage)
		m_pRtmpManage->SetPublishOtherPlayerVolum(m_iOtherPlayerVolum, m_bOtherPlayerMute);
	return 0;
}
LRESULT CMainDlg::OnBnClickedButtonEnableRecord(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_bEnableRecord = !m_bEnableRecord;
	m_pRtmpManage->SetRecordFileFullName(m_strFileName.c_str());
	//m_pRtmpManage->EnableRecordFile(true);
	if(m_bEnableRecord)
	{
		if(m_pRtmpManage)
		{
			m_pRtmpManage->SetRecordFileFullName(m_strFileName.c_str());
			m_pRtmpManage->EnableRecordFile(m_bEnableRecord);
		}
		GetDlgItem(IDC_BUTTON_ENABLERECORD).SetWindowText(L"停止");
	}
	else
	{
		if(m_pRtmpManage)
		{
			m_pRtmpManage->EnableRecordFile(m_bEnableRecord);
		}
		GetDlgItem(IDC_BUTTON_ENABLERECORD).SetWindowText(L"录制");
	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonDirectCon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_bDirectConSvr = !m_bDirectConSvr;
	if(m_bDirectConSvr)
	{
		GetDlgItem(IDC_BUTTON_IS_DIRECTCON).SetWindowText(L"直连");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_IS_DIRECTCON).SetWindowText(L"调度");
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

LRESULT CMainDlg::OnBnClickedButtonAudioPreview(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_bPreviewAudio = !m_bPreviewAudio;
	if(m_bPreviewAudio)
	{
		GetDlgItem(IDC_BUTTON_AUDIO_PREVIEW).SetWindowText(L"停止");
		//if(m_pRtmpManage)
		//{
		//	//m_pRtmpManage->SetCam(m_ComboBoxVideo.GetCurSel());
		//	m_pRtmpManage->SetMic(m_ComboBoxAudio.GetCurSel());
		//	m_pRtmpManage->SetAudioPreviewVolumPGWnd(GetDlgItem( IDC_STATIC_VOLUMPG).m_hWnd);
		//	m_pRtmpManage->SetAudioPlayDev(m_ComboBoxAudioPlay.GetCurSel());
		//	m_pRtmpManage->EnableAudioPreview(true);
		//}

		if(m_pAudioVideoPreview)
		{
			//m_pRtmpManage->SetCam(m_ComboBoxVideo.GetCurSel());
			m_pAudioVideoPreview->SetMic(m_ComboBoxAudio.GetCurSel());
			m_pAudioVideoPreview->SetAudioPreviewVolumPGWnd(GetDlgItem( IDC_STATIC_VOLUMPG).m_hWnd);
			m_pAudioVideoPreview->SetAudioPlayDev(m_ComboBoxAudioPlay.GetCurSel());
			m_pAudioVideoPreview->EnableAudioPreview(true);
		}
	}
	else
	{
		GetDlgItem(IDC_BUTTON_AUDIO_PREVIEW).SetWindowText(L"音频预览");
		/*if(m_pRtmpManage)
		{
			m_pRtmpManage->EnableAudioPreview(false);
		}*/
		if(m_pAudioVideoPreview)
		{
			m_pAudioVideoPreview->EnableAudioPreview(false);
		}
	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonVideoPreview(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pAudioVideoPreview == NULL)
	{
		m_pAudioVideoPreview = CreateAudioVideoPreview(this);
	}
	m_bPreviewVideo = !m_bPreviewVideo;
	if(m_bPreviewVideo)
	{
		GetDlgItem(IDC_BUTTON_VIDEO_PREVIEW).SetWindowText(L"停止");
		/*
		if(m_pRtmpManage)
		{
			m_pRtmpManage->SetCam(m_ComboBoxVideo.GetCurSel());
			m_pRtmpManage->SetVideoPreviewWnd(GetDlgItem( IDC_STATIC_VIDEOPREVIEW).m_hWnd);
			m_pRtmpManage->EnableVideoPreview(true);

		}
		*/
		if(m_pAudioVideoPreview)
		{
			m_pAudioVideoPreview->SetCam(m_ComboBoxVideo.GetCurSel());
			m_pAudioVideoPreview->SetVideoPreviewWnd(GetDlgItem( IDC_STATIC_VIDEOPREVIEW).m_hWnd);
			m_pAudioVideoPreview->EnableVideoPreview(true);
		}
	}
	else
	{
		GetDlgItem(IDC_BUTTON_VIDEO_PREVIEW).SetWindowText(L"视频预览");
		/*if(m_pRtmpManage)
		{
			m_pRtmpManage->EnableVideoPreview(false);
		}*/
		if(m_pAudioVideoPreview)
		{
			m_pAudioVideoPreview->EnableVideoPreview(false);
		}
	}
	return 0;
}

bool CMainDlg::OnAudioVideoPreviewMsg(eMsgLevel MsgLev, const char *pMsg, const char *pDetail) 
{
	return true; 
}
LRESULT CMainDlg::OnBnClickedButtonShowSpectrum(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	m_bShowSpectrum = !m_bShowSpectrum;
	SetShowSpectrum(m_bShowSpectrum);

	return 0;
}

void CMainDlg::SetShowSpectrum(bool bShow)
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

LRESULT CMainDlg::OnBnClickedCheckAutoctrlbitrate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_AUTOCTRLBITRATE))
	{	SetAutoCtrlBitrate(true);	}
	else
	{	SetAutoCtrlBitrate(false);	}

	return 0;
}

void CMainDlg::SetAutoCtrlBitrate(bool bAuto)
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


void CMainDlg::EnableBackgroundPlayer(bool bBackground)
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

		root["cmd"] = Json::Value("StartBackground");
		params["bBackground"] = Json::Value(bBackground);
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