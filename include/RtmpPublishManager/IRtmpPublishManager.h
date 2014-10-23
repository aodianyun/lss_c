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
	 * 接    口：JSON回调统一接口 OnAudioPercent
	 * 接口功能：音频数据（百分比）回调，用于上层显示自定义音量条
	 * 参    数：uPercent	音频数据（处理后的）的百分比形式
	 * 调    用：pszJson { "cmd":"OnAudioPercent", "params":{"uPercent":12} }
	 **/

	/**
	 * 接    口：JSON回调统一接口 OnShowVideo
	 * 接口功能：视频显示时触发
	 * 调    用：pszJson { "cmd":"OnShowVideo", "params":{} }
	 **/

	/**
	 * 接口功能：JSON回调统一接口
	 * 参    数：pszJson	JSON格式字符串
	 * 返 回 值：成功/失败
	 * 示    例：{ "cmd":"xxx", "params":{"x":true, "xx":12, "xxx":"abc"} }
	 **/
	virtual bool OnRtmpPublishNotifyInJson(const char* pszJson){ return true; };

	/**
	 * 接口功能：消息回调
	 * 参    数：MsgLev	消息级别
	 *           pMsg	消息主体
	 * 返 回 值：成功/失败
	 **/
	virtual bool OnRtmpPublishManagerMsg(eMsgLevel MsgLev, const char *pMsg) = 0;

	/**
	 * 接口功能：上麦流信息回调
	 * 参    数：RtmpNetInfo	流信息结构体，详见结构体定义
	 * 返 回 值：成功/失败
	 **/
	virtual bool OnRtmpPublishManagerNetInfo(RtmpPublishNetInfo &RtmpNetInfo) = 0;

	/**
	 * 接口功能：系统资源信息回调
	 * 参    数：RtmpStatusInfo	系统资源信息结构体，详见结构体定义
	 * 返 回 值：成功/失败
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
	 * 接    口：JSON调用统一接口 SetShowSpectrum
	 * 接口功能：设置是否显示音频频谱
	 * 调    用：pszCmd { "cmd":"SetShowSpectrum", "params":{"bShow":true} }
	 * 成功返回：ppszRes { "code":0, "data":{} }
	 * 失败返回：ppszRes { "code":1, "data":{"reason":"no."} }
	 **/

	/**
	 * 接    口：JSON调用统一接口 GetSdkVersion
	 * 接口功能：获取当前使用的SDK版本号
	 * 调    用：pszCmd { "cmd":"GetSdkVersion", "params":{} }
	 * 成功返回：ppszRes { "code":0, "data":{"version":"201407171636"} }
	 * 失败返回：ppszRes { "code":1, "data":{"reason":"no version."} }
	 **/

	/**
	 * 接    口：JSON调用统一接口 SetAutoCtrlBitrate
	 * 接口功能：设置是否开启自动调整码率模式
	 * 调    用：pszCmd { "cmd":"SetAutoCtrlBitrate", "params":{"bAuto":true} }
	 * 成功返回：ppszRes { "code":0, "data":{} }
	 * 失败返回：ppszRes { "code":1, "data":{"reason":"no version."} }
	 **/

	/**
	 * 接    口：JSON调用统一接口 GetPlayerList
	 * 接口功能：获取本地安装的播放器列表（必须在 StartPublish 之前调用）
	 * 调    用：pszCmd { "cmd":"GetPlayerList", "params":{} }
	 * 成功返回：ppszRes { "code":0, "data":{"player":["9158", "guagua"]} }
	 * 失败返回：ppszRes { "code":1, "data":{"reason":"no version."} }
	 **/

	/**
	 * 接口功能：JSON调用统一接口
	 * 参    数：pszCmd	JSON格式命令
	 *			 ppszRes JSON格式结果
	 * 返 回 值：成功/失败
	 * 示    例：pszCmd { "cmd":"xxx", "params":{"x":true, "xx":12, "xxx":"abc"} }
	 * 示    例：ppszRes { "code":0, "data":{"reason":"abc"} }
	 * 说    明：code含义 0：success，1：fail
	 **/
	virtual bool CallInJson(const char* pszCmd, char** ppszRes) = 0;

	/*
	*	接口功能:	发布流到服务器
	*	参    数:	无
	*	返 回 值:	true(success), false(fail)
	*/
	virtual bool StartPublish() = 0;

	/*
	*	接口功能:	停止发布
	*	参    数:	无	
	*	返 回 值:	true(success), false(fail)
	*/
	virtual bool StopPublish() = 0;
	
	/*
	*	接口功能:	释放
	*	参    数:	无
	*	返 回 值:	无
	*/
	virtual void Release() = 0;

	/*
	*	接口功能:	获取摄像头列表. （必须在 StartPublish 之前调用）
	*	参    数:		
	*		@ppDevice:	名称数组
	*		@size:		总数
	*	返 回 值:		 >=0(success), <0(fail)
	*/
	virtual int GetCamList(wchar_t ** &ppDevice, unsigned int &size) = 0;

	/*
	*	接口功能:	获取麦克风列表. （必须在 StartPublish 之前调用）
	*	参    数:		
	*		@ppDevice:	名称数组
	*		@size:		总数
	*	返 回 值:					 >=0(success), <0(fail)
	*/
	virtual int GetMicList(wchar_t ** &ppDevice, unsigned int &size) = 0;

	/*
	*	接口功能:		设置摄像头ID. （必须在 StartPublish 之前调用）
	*	参    数:		
	*		@uCamID:	摄像头ID.(count from 0)
	*	返 回 值:		>=0(success), <0(fail)
	*/
	virtual int SetCam(int uCamID) = 0;

	/*
	*	接口功能:		设置麦克风ID.  （必须在 StartPublish 之前调用）
	*	参    数:		
	*		@uMicID:	麦克风ID.(count from 0)
	*	返 回 值:		>=0(success), <0(fail)
	*/
	virtual int SetMic(int uMicID) = 0;
	
	/*
	*	接口功能:		设置混音或者麦克.  （必须在 StartPublish 之后，StartPublish 之前调用）
	*	参    数:		
	*		@bCapStereo:	true(混音), false(麦克)	
	*	返 回 值:			>=0(success), <0(fail)
	*/
	virtual int CapStereo(bool bCapStereo) = 0;

	/*
	*	接口功能:		设置音频参数.  （必须在 StartPublish 之前调用）
	*	参    数:		
	*		@iSampleRate:			采样率. (hz)	
	*		@iChannels:				音频通道数. 	
	*		@iBitPerSample:			音频采样位数. (bit)	
	*		@iBitRate:				码率. (bps)	
	*		@iVolum:				音量. (0-100)	
	*		@bMute:					是否静音. (true/false)	
	*	返 回 值:		无
	*/
	virtual void SetPublishAudioPara(int iSampleRate, int iChannels, int iBitPerSample, int iBitRate,int iVolum, bool bMute) = 0;
	
	/*
	*	接口功能:		设置视频参数.  （必须在 StartPublish 之前调用）
	*	参    数:		
	*		@uWidth:				视频宽. 	
	*		@uHeight:				视频高. 	
	*		@uFrameRate:			视频帧率. 	
	*		@uBitRate:				码率. (bps)	
	*		@hVideo:				视频显示窗口句柄.		
	*	返 回 值:					无
	*/
	virtual void SetPublishVideoPara(int uWidth, int uHeight, int uFrameRate, int uBitRate, HWND hVideo, int uKeyFrameInterval=0) = 0;
	
	/*
	*	接口功能:					设置发布流信息.  （必须在 StartPublish 之前调用）
	*	参    数:		
	*		@pServer:				服务器地址. 	
	*		@pApp:					App string (different from other site). (when use verify policy, AppID= AppID+"?"+key, if appID is "App" and key is "kkyoo", then here is "App?kkyoo")	
	*		@pStream:				Stream. 	
	*	返 回 值:					无
	*/
	virtual void SetPublishStreamPara(const char *pServer, const char *pApp, const char *pStream) = 0;
	
	/*
	*	接口功能:					设置音量.（可以在发布过程中调用）
	*	参    数:		
	*		@uVolum:				音量. 	
	*		@bMute:					是否静音. 	
	*	返 回 值:					无
	*/
	virtual void SetPublishVolum(int uVolum, bool bMute/*, int uPlayerVolum=80*/) = 0;

	/*
	*	接口功能:					设置背景音乐音量 .（可以在上麦过程中调用）
	*	参    数:		
	*		@uVolum:				音量. 	
	*		@bMute:					是否静音. 
	*	返 回 值:					无
	*/
	virtual void SetPublishOtherPlayerVolum(int uVolum, bool bMute) = 0;

	/*
	接口功能：	获取背景音乐播放器列表（必须在 StartPublish 之前调用）
	参    数：	
	返 回 值：	true -- 成功 false -- 失败
	*/
	virtual bool GetPlayerList(vector<string> &vecPlayerList) = 0;

	/*
	接口功能：	选择背景音乐播放器（必须在 StartPublish 之前调用）
	参    数：	
	返 回 值：	true -- 成功 	false -- 失败
	*/
	virtual bool SelectPlayerName(string strPlayerName) = 0;

	/*
	接口功能：	开启背景音乐播放器（必须在 StartPublish 之后，StartPublish 之前调用）
	参    数：	
	返 回 值：	true -- 成功 	false -- 失败
	*/
	virtual bool EnableHookPlayer(bool IsEnableHookPlayer) = 0;

	/*
	接口功能：	开启背景音乐播放器时是否重启播放器进程（必须在 StartPublish 之前调用）
	参    数：	
	返 回 值：	无
	*/
	virtual void WhetherToRestartTargetProcess(bool isRestart) = 0;

	/*
	接口功能：	开启日志（必须在 StartPublish 之后，StartPublish 之前调用）
	参    数：	isEnableLog 是否开启
	返 回 值：	无
	*/
	virtual void EnableLog(bool isEnableLog) = 0;

	/*
	接口功能：	让HookDll 退出进程空间（必须在 StartPublish 之前调用）
	参    数：	
	返 回 值：	无
	*/
	virtual void DetachHookDll() = 0;

	/*
	接口功能：	发送用户ID到服务器（必须在 StartPublish 之前调用）
	参    数：	pszUserID 用户ID
	返 回 值：	无
	*/
	virtual void SetClientUserID(const char *pszUserID) = 0;

	/*
	接口功能：	设置录制文件名
	参    数：	pszFileName 文件名
	返 回 值：	无
	*/
	virtual void SetRecordFileFullName(const char *pszFileName) = 0;

	/*
	接口功能：	开启/关闭 录制功能（可以在上麦过程中调用）
	参    数：	
	返 回 值：	无
	*/
	virtual void EnableRecordFile(bool bEnable) = 0;
};

extern "C" __declspec(dllexport) IRtmpPublishManager * CreateRtmpPublishManager(IRtmpPublishManagerNotify * pNotify);