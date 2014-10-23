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
	�� �� ����	SetAudioPreviewVolumPGWnd
	�ӿڹ��ܣ�	����/�ر� ��ƵԤ������
	��    ����	hwnd(��ƵԤ��������Ƶ����ʾ���ھ��)
	�� �� ֵ��	
	*/
	virtual void SetAudioPreviewVolumPGWnd(HWND hwnd) = 0;

	/*
	�� �� ����	EnableAudioPreview
	�ӿڹ��ܣ�	����/�ر� ��ƵԤ������
	��    ����	bEnable(true:����, false:�ر�)
	�� �� ֵ��	
	*/
	virtual void EnableAudioPreview(bool bEnable) = 0;

	/*
	�� �� ����	SetVideoPreviewWnd
	�ӿڹ��ܣ�	������ƵԤ�����ھ��
	��    ����	hwnd(��ƵԤ�����)
	�� �� ֵ��	
	*/
	virtual void SetVideoPreviewWnd(HWND hwnd) = 0;

	/*
	�� �� ����	EnableRecordFile
	�ӿڹ��ܣ�	����/�ر� ��ƵԤ������
	��    ����	bEnable(true:����, false:�ر�)
	�� �� ֵ��	
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