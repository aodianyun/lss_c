#pragma once

#include <string>
#include <vector>
using std::string;
using std::vector;
#include "../RtmpSDKMsg.h"

class IRtmpPublishManagerNotify
{
public:
	/**
	 * ��    �ڣ�JSON�ص�ͳһ�ӿ� OnAudioPercent
	 * �ӿڹ��ܣ���Ƶ���ݣ��ٷֱȣ��ص��������ϲ���ʾ�Զ���������
	 * ��    ����uPercent	��Ƶ���ݣ������ģ��İٷֱ���ʽ
	 * ��    �ã�pszJson { "cmd":"OnAudioPercent", "params":{"uPercent":12} }
	 **/

	/**
	 * ��    �ڣ�JSON�ص�ͳһ�ӿ� OnShowVideo
	 * �ӿڹ��ܣ���Ƶ��ʾʱ����
	 * ��    �ã�pszJson { "cmd":"OnShowVideo", "params":{} }
	 **/

	/**
	 * �ӿڹ��ܣ�JSON�ص�ͳһ�ӿ�
	 * ��    ����pszJson	JSON��ʽ�ַ���
	 * �� �� ֵ���ɹ�/ʧ��
	 * ʾ    ����{ "cmd":"xxx", "params":{"x":true, "xx":12, "xxx":"abc"} }
	 **/
	virtual bool OnRtmpPublishNotifyInJson(const char* pszJson){ return true; };

	/**
	 * �ӿڹ��ܣ���Ϣ�ص�
	 * ��    ����MsgLev	��Ϣ����
	 *           pMsg	��Ϣ����
	 * �� �� ֵ���ɹ�/ʧ��
	 **/
	virtual bool OnRtmpPublishManagerMsg(eMsgLevel MsgLev, const char *pMsg) = 0;

	/**
	 * �ӿڹ��ܣ���������Ϣ�ص�
	 * ��    ����RtmpNetInfo	����Ϣ�ṹ�壬����ṹ�嶨��
	 * �� �� ֵ���ɹ�/ʧ��
	 **/
	virtual bool OnRtmpPublishManagerNetInfo(RtmpPublishNetInfo &RtmpNetInfo) = 0;

	/**
	 * �ӿڹ��ܣ�ϵͳ��Դ��Ϣ�ص�
	 * ��    ����RtmpStatusInfo	ϵͳ��Դ��Ϣ�ṹ�壬����ṹ�嶨��
	 * �� �� ֵ���ɹ�/ʧ��
	 **/
	virtual bool OnRtmpPublishManagerComputerStatusInfo(RtmpPublishComputerStatusInfo &RtmpStatusInfo) = 0;
};
typedef struct DevInfo
{
	DevInfo()
		: ppDevice(NULL)
		, size(0)
	{}
	~DevInfo()
	{
		delete [] ppDevice;
	}
	wchar_t ** ppDevice;
	int size;
};
class IRtmpPublishManager
{
public:

	/**
	 * ��    �ڣ�JSON����ͳһ�ӿ� SetShowSpectrum
	 * �ӿڹ��ܣ������Ƿ���ʾ��ƵƵ��
	 * ��    �ã�pszCmd { "cmd":"SetShowSpectrum", "params":{"bShow":true} }
	 * �ɹ����أ�ppszRes { "code":0, "data":{} }
	 * ʧ�ܷ��أ�ppszRes { "code":1, "data":{"reason":"no."} }
	 **/

	/**
	 * ��    �ڣ�JSON����ͳһ�ӿ� GetSdkVersion
	 * �ӿڹ��ܣ���ȡ��ǰʹ�õ�SDK�汾��
	 * ��    �ã�pszCmd { "cmd":"GetSdkVersion", "params":{} }
	 * �ɹ����أ�ppszRes { "code":0, "data":{"version":"201407171636"} }
	 * ʧ�ܷ��أ�ppszRes { "code":1, "data":{"reason":"no version."} }
	 **/

	/**
	 * ��    �ڣ�JSON����ͳһ�ӿ� SetAutoCtrlBitrate
	 * �ӿڹ��ܣ������Ƿ����Զ���������ģʽ
	 * ��    �ã�pszCmd { "cmd":"SetAutoCtrlBitrate", "params":{"bAuto":true} }
	 * �ɹ����أ�ppszRes { "code":0, "data":{} }
	 * ʧ�ܷ��أ�ppszRes { "code":1, "data":{"reason":"no version."} }
	 **/

	/**
	 * ��    �ڣ�JSON����ͳһ�ӿ� GetPlayerList
	 * �ӿڹ��ܣ���ȡ���ذ�װ�Ĳ������б������� StartPublish ֮ǰ���ã�
	 * ��    �ã�pszCmd { "cmd":"GetPlayerList", "params":{} }
	 * �ɹ����أ�ppszRes { "code":0, "data":{"player":["9158", "guagua"]} }
	 * ʧ�ܷ��أ�ppszRes { "code":1, "data":{"reason":"no version."} }
	 **/

	/**
	 * �ӿڹ��ܣ�JSON����ͳһ�ӿ�
	 * ��    ����pszCmd	JSON��ʽ����
	 *			 ppszRes JSON��ʽ���
	 * �� �� ֵ���ɹ�/ʧ��
	 * ʾ    ����pszCmd { "cmd":"xxx", "params":{"x":true, "xx":12, "xxx":"abc"} }
	 * ʾ    ����ppszRes { "code":0, "data":{"reason":"abc"} }
	 * ˵    ����code���� 0��success��1��fail
	 **/
	virtual bool CallInJson(const char* pszCmd, char** ppszRes) = 0;

	/*
	*	�ӿڹ���:	��������������
	*	��    ��:	��
	*	�� �� ֵ:	true(success), false(fail)
	*/
	virtual bool StartPublish() = 0;

	/*
	*	�ӿڹ���:	ֹͣ����
	*	��    ��:	��	
	*	�� �� ֵ:	true(success), false(fail)
	*/
	virtual bool StopPublish() = 0;
	
	/*
	*	�ӿڹ���:	�ͷ�
	*	��    ��:	��
	*	�� �� ֵ:	��
	*/
	virtual void Release() = 0;

	/*
	*	�ӿڹ���:	��ȡ����ͷ�б�. �������� StartPublish ֮ǰ���ã�
	*	��    ��:		
	*		@ppDevice:	��������
	*		@size:		����
	*	�� �� ֵ:		 >=0(success), <0(fail)
	*/
	virtual int GetCamList(wchar_t ** &ppDevice, unsigned int &size) = 0;

	/*
	*	�ӿڹ���:	��ȡ��˷��б�. �������� StartPublish ֮ǰ���ã�
	*	��    ��:		
	*		@ppDevice:	��������
	*		@size:		����
	*	�� �� ֵ:					 >=0(success), <0(fail)
	*/
	virtual int GetMicList(wchar_t ** &ppDevice, unsigned int &size) = 0;

	/*
	*	�ӿڹ���:		��������ͷID. �������� StartPublish ֮ǰ���ã�
	*	��    ��:		
	*		@uCamID:	����ͷID.(count from 0)
	*	�� �� ֵ:		>=0(success), <0(fail)
	*/
	virtual int SetCam(int uCamID) = 0;

	/*
	*	�ӿڹ���:		������˷�ID.  �������� StartPublish ֮ǰ���ã�
	*	��    ��:		
	*		@uMicID:	��˷�ID.(count from 0)
	*	�� �� ֵ:		>=0(success), <0(fail)
	*/
	virtual int SetMic(int uMicID) = 0;
	
	/*
	*	�ӿڹ���:		���û����������.  �������� StartPublish ֮��StartPublish ֮ǰ���ã�
	*	��    ��:		
	*		@bCapStereo:	true(����), false(���)	
	*	�� �� ֵ:			>=0(success), <0(fail)
	*/
	virtual int CapStereo(bool bCapStereo) = 0;

	/*
	*	�ӿڹ���:		������Ƶ����.  �������� StartPublish ֮ǰ���ã�
	*	��    ��:		
	*		@iSampleRate:			������. (hz)	
	*		@iChannels:				��Ƶͨ����. 	
	*		@iBitPerSample:			��Ƶ����λ��. (bit)	
	*		@iBitRate:				����. (bps)	
	*		@iVolum:				����. (0-100)	
	*		@bMute:					�Ƿ���. (true/false)	
	*	�� �� ֵ:		��
	*/
	virtual void SetPublishAudioPara(int iSampleRate, int iChannels, int iBitPerSample, int iBitRate,int iVolum, bool bMute) = 0;
	
	/*
	*	�ӿڹ���:		������Ƶ����.  �������� StartPublish ֮ǰ���ã�
	*	��    ��:		
	*		@uWidth:				��Ƶ��. 	
	*		@uHeight:				��Ƶ��. 	
	*		@uFrameRate:			��Ƶ֡��. 	
	*		@uBitRate:				����. (bps)	
	*		@hVideo:				��Ƶ��ʾ���ھ��.		
	*	�� �� ֵ:					��
	*/
	virtual void SetPublishVideoPara(int uWidth, int uHeight, int uFrameRate, int uBitRate, HWND hVideo, int uKeyFrameInterval=0) = 0;
	
	/*
	*	�ӿڹ���:					���÷�������Ϣ.  �������� StartPublish ֮ǰ���ã�
	*	��    ��:		
	*		@pServer:				��������ַ. 	
	*		@pApp:					App string (different from other site). (when use verify policy, AppID= AppID+"?"+key, if appID is "App" and key is "kkyoo", then here is "App?kkyoo")	
	*		@pStream:				Stream. 	
	*	�� �� ֵ:					��
	*/
	virtual void SetPublishStreamPara(const char *pServer, const char *pApp, const char *pStream) = 0;
	
	/*
	*	�ӿڹ���:					��������.�������ڷ��������е��ã�
	*	��    ��:		
	*		@uVolum:				����. 	
	*		@bMute:					�Ƿ���. 	
	*	�� �� ֵ:					��
	*/
	virtual void SetPublishVolum(int uVolum, bool bMute/*, int uPlayerVolum=80*/) = 0;

	/*
	*	�ӿڹ���:					���ñ����������� .����������������е��ã�
	*	��    ��:		
	*		@uVolum:				����. 	
	*		@bMute:					�Ƿ���. 
	*	�� �� ֵ:					��
	*/
	virtual void SetPublishOtherPlayerVolum(int uVolum, bool bMute) = 0;

	/*
	�ӿڹ��ܣ�	��ȡ�������ֲ������б������� StartPublish ֮ǰ���ã�
	��    ����	
	�� �� ֵ��	true -- �ɹ� false -- ʧ��
	*/
	virtual bool GetPlayerList(vector<string> &vecPlayerList) = 0;

	/*
	�ӿڹ��ܣ�	ѡ�񱳾����ֲ������������� StartPublish ֮ǰ���ã�
	��    ����	
	�� �� ֵ��	true -- �ɹ� 	false -- ʧ��
	*/
	virtual bool SelectPlayerName(string strPlayerName) = 0;

	/*
	�ӿڹ��ܣ�	�����������ֲ������������� StartPublish ֮��StartPublish ֮ǰ���ã�
	��    ����	
	�� �� ֵ��	true -- �ɹ� 	false -- ʧ��
	*/
	virtual bool EnableHookPlayer(bool IsEnableHookPlayer) = 0;

	/*
	�ӿڹ��ܣ�	�����������ֲ�����ʱ�Ƿ��������������̣������� StartPublish ֮ǰ���ã�
	��    ����	
	�� �� ֵ��	��
	*/
	virtual void WhetherToRestartTargetProcess(bool isRestart) = 0;

	/*
	�ӿڹ��ܣ�	������־�������� StartPublish ֮��StartPublish ֮ǰ���ã�
	��    ����	isEnableLog �Ƿ���
	�� �� ֵ��	��
	*/
	virtual void EnableLog(bool isEnableLog) = 0;

	/*
	�ӿڹ��ܣ�	��HookDll �˳����̿ռ䣨������ StartPublish ֮ǰ���ã�
	��    ����	
	�� �� ֵ��	��
	*/
	virtual void DetachHookDll() = 0;

	/*
	�ӿڹ��ܣ�	�����û�ID���������������� StartPublish ֮ǰ���ã�
	��    ����	pszUserID �û�ID
	�� �� ֵ��	��
	*/
	virtual void SetClientUserID(const char *pszUserID) = 0;

	/*
	�ӿڹ��ܣ�	����¼���ļ���
	��    ����	pszFileName �ļ���
	�� �� ֵ��	��
	*/
	virtual void SetRecordFileFullName(const char *pszFileName) = 0;

	/*
	�ӿڹ��ܣ�	����/�ر� ¼�ƹ��ܣ���������������е��ã�
	��    ����	
	�� �� ֵ��	��
	*/
	virtual void EnableRecordFile(bool bEnable) = 0;
};

extern "C" __declspec(dllexport) IRtmpPublishManager * CreateRtmpPublishManager(IRtmpPublishManagerNotify * pNotify);