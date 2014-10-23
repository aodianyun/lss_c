#pragma once
#include <vector>
using std::vector;
#include "../RtmpSDKMsg.h"
#include "string"
using std::string;
using std::wstring;
#include <windows.h>

class IAudioVideoPreviewNotify
{
	public:
	virtual bool OnAudioVideoPreviewMsg(eMsgLevel MsgLev, const char *pMsg, const char *pDetail=NULL) = 0;
};
class IAudioVideoPreview
{
public:
	virtual ~IAudioVideoPreview(){}
	/*virtual bool Start() = 0;
	virtual bool Stop() = 0;*/
	virtual void Release() = 0;
	
	/*
	*	Function:					Get the camera list in your computer. 
	*
	*	Parameter:		
	*		@ppDevice:		 reference type, 
	*		@size:					 sum of cameras	 
	*
	*	Return:					 >=0(success), <0(fail)
	*/
	virtual int GetCamList(wchar_t ** &ppDevice, unsigned int &size) = 0;

	/*
	*	Function:					Get the microphone list in your computer.  
	*
	*	Parameter:		
	*		@ppDevice:		 reference type, 
	*		@size:					 sum of microphones		
	*
	*	Return:					 >=0(success), <0(fail)
	*/
	virtual int GetMicList(wchar_t ** &ppDevice, unsigned int &size) = 0;

	/*
	*	Function:					Set the target camera to capture. 
	*
	*	Parameter:		
	*		@uCamID:			id of cam selected in the cam list.(count from 0)
	*
	*	Return:					 >=0(success), <0(fail)
	*/
	virtual int SetCam(int uCamID) = 0;

	/*
	*	Function:					Set the target microphone to capture.  
	*
	*	Parameter:		
	*		@uMicID:			id of microphone selected in the cam list. (count from 0)	
	*
	*	Return:					 >=0(success), <0(fail)
	*/
	virtual int SetMic(int uMicID) = 0;

	/*
	函 数 名：	SetAudioPreviewVolumPGWnd
	接口功能：	开启/关闭 音频预览功能
	参    数：	hwnd(音频预览的音量频谱显示窗口句柄)
	返 回 值：	
	*/
	virtual void SetAudioPreviewVolumPGWnd(HWND hwnd) = 0;

	/*
	函 数 名：	EnableAudioPreview
	接口功能：	开启/关闭 音频预览功能
	参    数：	bEnable(true:开启, false:关闭)
	返 回 值：	
	*/
	virtual void EnableAudioPreview(bool bEnable) = 0;

	/*
	函 数 名：	SetVideoPreviewWnd
	接口功能：	设置视频预览窗口句柄
	参    数：	hwnd(视频预览句柄)
	返 回 值：	
	*/
	virtual void SetVideoPreviewWnd(HWND hwnd) = 0;

	/*
	函 数 名：	EnableRecordFile
	接口功能：	开启/关闭 视频预览功能
	参    数：	bEnable(true:开启, false:关闭)
	返 回 值：	
	*/
	virtual void EnableVideoPreview(bool bEnable) = 0;

	/*
	*	Function:					Get Audio Play Device List 
	*
	*	Parameter:		
	*		@ppDevice:		Secondary  Pointer to Audio-Play Device Name
	*		@size:					Sum of Audio-Play Devices 
	*
	*	Return:					true(success), false(fail)
	*/
	virtual int GetAudioPlayDevList(wchar_t ** &ppDevice, unsigned int &size) = 0;

	/*
	*	Function:					Set Audio-Play Device  
	*
	*	Parameter:		
	*		@uDevID:			Device id(count from 0)
	*
	*	Return:					>=0(success), <0(fail)
	*/
	virtual int SetAudioPlayDev(int uDevID) = 0;
};

__declspec(dllexport) IAudioVideoPreview * CreateAudioVideoPreview(IAudioVideoPreviewNotify * pNotify);