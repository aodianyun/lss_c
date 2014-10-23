#pragma once
#include <windows.h>
#include <string>
using std::string;
#include "../RtmpSDKMsg.h"

class IRtmpPlayManagerNotify
{
public:
	/**
	 * �ӿڹ��ܣ�JSON�ص�ͳһ�ӿ�
	 * ��    ����pszJson	JSON��ʽ�ַ���
	 * �� �� ֵ���ɹ�/ʧ��
	 * ʾ    ����{ "cmd":"xxx", "params":{"x":true, "xx":12, "xxx":"abc"} }
	 **/
	virtual bool OnRtmpPlayNotifyInJson(const char* pszJson){ return true; };

	/**
	 * �ӿڹ��ܣ���Ϣ�ص�
	 * ��    ����MsgLev	��Ϣ����
	 *           pMsg	��Ϣ����
	 *           pDetail��Ϣ��ϸ˵��
	 * �� �� ֵ���ɹ�/ʧ��
	 **/
	virtual bool OnRtmpPlayManagerMsg(eMsgLevel MsgLev, const char *pMsg, const char *pDetail=NULL) = 0;
	
	/**
	 * �ӿڹ��ܣ���������Ϣ�ص�
	 * ��    ����RtmpNetInfo	����Ϣ�ṹ�壬����ṹ�嶨��
	 * �� �� ֵ���ɹ�/ʧ��
	 **/
	virtual bool OnRtmpPlayManagerNetInfo(RtmpPublishNetInfo &RtmpNetInfo) = 0;
	
	/**
	 * �ӿڹ��ܣ�ϵͳ��Դ��Ϣ�ص�
	 * ��    ����RtmpStatusInfo	ϵͳ��Դ��Ϣ�ṹ�壬����ṹ�嶨��
	 * �� �� ֵ���ɹ�/ʧ��
	 **/
	virtual bool OnRtmpPlayManagerComputerStatusInfo(RtmpPublishComputerStatusInfo &RtmpStatusInfo) = 0;
	
	/**
	 * �ӿڹ��ܣ�����Ƶ������Ϣ�ص�
	 * ��    ����RtmpPlayAVInfo	����Ƶ������Ϣ�ṹ�壬����ṹ�嶨��
	 * �� �� ֵ���ɹ�/ʧ��
	 **/
	virtual bool OnRtmpPlayManagerAVInfo(RtmpPlayAVInfo &RtmpPlayAVInfo) = 0;
	
	/**
	 * �ӿڹ��ܣ�����������Ϣ�ص�
	 * ��    ����RtmpPlayQuality	����������Ϣ�ṹ�壬����ṹ�嶨��
	 * �� �� ֵ���ɹ�/ʧ��
	 **/
	virtual bool OnRtmpPlayManagerPlayQuality(RtmpPlayQuality &RtmpPlayQuality){return true;};
	
	/**
	 * ��    �ڣ�OnShowVideo
	 * �ӿڹ��ܣ���һ����ʾ��Ƶǰ����
	 * ��    �ã�pszCmd { "cmd":"OnShowVideo", "params":{} }
	 **/

	/**
	 * ��    �ڣ�OnAudioPercent
	 * �ӿڹ��ܣ���Ƶ���ݣ��ٷֱȣ��ص��������ϲ���ʾ�Զ���������
	 * ��    ����uPercent	��Ƶ���ݣ������ģ��İٷֱ���ʽ
	 * ��    �ã�pszCmd { "cmd":"OnAudioPercent", "params":{"uPercent":12} }
	 **/
};

class IRtmpPlayManager
{
public:
	/**
	 * ��    �ڣ�JSON����ͳһ�ӿ� GetSdkVersion
	 * �ӿڹ��ܣ���ȡ��ǰʹ�õ�SDK�汾��
	 * ��    �ã�pszCmd { "cmd":"GetSdkVersion", "params":{} }
	 * �ɹ����أ�ppszRes { "code":0, "data":{"version":"201407171636"} }
	 * ʧ�ܷ��أ�ppszRes { "code":1, "data":{"reason":"no version."} }
	 **/

	/**
	 * ��    �ڣ�JSON����ͳһ�ӿ� SetSYNBufTime
	 * �ӿڹ��ܣ����ò��Ż�����ʱ����
	 * ��    �ã�pszCmd { "cmd":"SetSYNBufTime", "params":{"dwDatalenOffset":1500, "dwMaxTimestampleInqueue":3000, "dwTimeCheckSynbuf":5000} }
	 * ���أ���
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
	*	�ӿڹ���:		��ʼ����
	*	��    ��:		��
	*	�� �� ֵ:		true(success), false(fail)
	*/
	virtual bool StartPlay() = 0;

	/*
	*	�ӿڹ���:		ֹͣ����
	*	��    ��:		��
	*	�� �� ֵ:		true(success), false(fail)
	*/
	virtual bool StopPlay() = 0;

	/*
	*	�ӿڹ���:		�ͷŶ���
	*	��    ��:		��
	*	�� �� ֵ:		��
	*/
	virtual void Release() = 0;

	/*
	*	�ӿڹ���:		��ȡ�����豸�б� �������� StartPlay ֮ǰ���ã�
	*	��    ��:		
	*		@ppDevice:	�豸�����б�
	*                   ָ��һ��wchar_t *�����ָ�룬����Ԫ�أ�wchar_t�ַ�����ָ�룬��L'\0'��β����
	*                   �ڴ���dll���ڷ����ͷţ��ϲ㲻�ô������ﴦ��ʱ���ƴ���Ķ���ָ���ʼ��null����ֹ�ڴ�й©��
	*		@size:		�豸����
	*	�� �� ֵ:		true(success), false(fail)
	*/
	virtual int GetAudioPlayDevList(wchar_t ** &ppDevice, unsigned int &size) = 0;

	/*
	*	�ӿڹ���:		���ò����豸ID  �������� StartPlay ֮ǰ���ã�
	*	��    ��:		
	*		@uDevID:	�����豸ID(count from 0)
	*	�� �� ֵ:		>=0(success), <0(fail)
	*/
	virtual int SetAudioPlayDev(int uDevID) = 0;

	/*
	*	�ӿڹ���:		���ò�������Ϣ  �������� StartPlay ֮ǰ���ã�
	*	��    ��:		
	*		@pURL:		��������ַ
	*		@pAppID:	��App����(���������֤���� AppID= AppID+"?"+key, if appID is "App" and key is "kkyoo", then here is "App?kkyoo")
	*		@pStreamID:	Stream����
	*	�� �� ֵ:							true(success), false(fail)
	*/
	virtual bool SetStreamInfo(const char *pURL, const char *pAppID, const char *pStreamID) = 0;

	/*
	*	�ӿڹ���:		������Ƶ��ʾ���ھ��  �������� StartPlay ֮ǰ���ã�
	*	��    ��:		
	*		@hwnd:		���ھ��
	*	�� �� ֵ:		void
	*/
	virtual void SetVdDisplayWnd(HWND hwnd) = 0;

	/*
	*	�ӿڹ���:		��������  �������ڲ��Ź����е��ã�
	*	��    ��:		
	*		@uOutVolum:	����
	*		@bMute:	    �Ƿ���
	*	�� �� ֵ:					void
	*/
	virtual void SetAdPara(unsigned int uOutVolum, bool bMute) = 0;

	/*
	*	�ӿڹ���:		�����û�ID �������� StartPlay ֮ǰ���ã�
	*	��    ��:		
	*		@pszUserID:	�û�ID
	*	�� �� ֵ:		��
	*/
	virtual void SetClientUserID(const char *pszUserID) = 0;

	/*
	�ӿڹ��ܣ�	����¼���ļ����������� StartPlay ֮ǰ���ã�
	��    ����	
	*		@pszFileName:¼���ļ�·��
	�� �� ֵ��	��
	*/
	virtual void SetRecordFileFullName(const char *pszFileName) = 0;

	/*
	�� �� ����	EnableRecordFile
	�ӿڹ��ܣ�	����/�ر� ¼������Ƶ���ܣ������ڲ��Ź����е��ã�
	��    ����	bEnable �Ƿ���
	�� �� ֵ��	��
	*/
	virtual void EnableRecordFile(bool bEnable) = 0;
};

extern "C" __declspec(dllexport) IRtmpPlayManager * CreateRtmpPlayManager(IRtmpPlayManagerNotify * pNotify);