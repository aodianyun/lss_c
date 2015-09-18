#pragma once
#include <windows.h>
#include <string>
using std::string;
#include "../RtmpSDKMsg.h"

/************************************************************************/
/*                          接口调用说明                                */
/*必用接口:	CreateRtmpPlayManager创建对象								*/
/*			GetAudioPlayDevList											*/
/*			SetAudioPlayDev												*/
/*			SetVdDisplayWnd												*/
/*			SetStreamInfo												*/
/*			StartPlay													*/
/*			StopPlay													*/
/*			Release														*/
/************************************************************************/


//////////////////////////////////////////////////////////////////////////
// 接口回调，抽象函数必须实现
class IRtmpPlayManagerNotify
{
public:
	/**
	 * 接口功能：JSON回调统一接口
	 * 参    数：pszJson	JSON格式字符串
	 * 返 回 值：成功/失败
	 * 示    例：{ "cmd":"xxx", "params":{"x":true, "xx":12, "xxx":"abc"} }
	 **/
	virtual bool OnRtmpPlayNotifyInJson(const char* pszJson){ return true; };

	/**
	 * 接口功能：消息回调
	 * 参    数：MsgLev	消息级别
	 *           pMsg	消息主体
	 *           pDetail消息详细说明
	 * 返 回 值：成功/失败
	 **/
	virtual bool OnRtmpPlayManagerMsg(eMsgLevel MsgLev, const char *pMsg, const char *pDetail=NULL) = 0;
	
	/**
	 * 接口功能：上麦流信息回调
	 * 参    数：RtmpNetInfo	流信息结构体，详见结构体定义
	 * 返 回 值：成功/失败
	 **/
	virtual bool OnRtmpPlayManagerNetInfo(RtmpPublishNetInfo &RtmpNetInfo) = 0;
	
	/**
	 * 接口功能：系统资源信息回调
	 * 参    数：RtmpStatusInfo	系统资源信息结构体，详见结构体定义
	 * 返 回 值：成功/失败
	 **/
	virtual bool OnRtmpPlayManagerComputerStatusInfo(RtmpPublishComputerStatusInfo &RtmpStatusInfo) = 0;
	
	/**
	 * 接口功能：音视频数据信息回调
	 * 参    数：RtmpPlayAVInfo	音视频数据信息结构体，详见结构体定义
	 * 返 回 值：成功/失败
	 **/
	virtual bool OnRtmpPlayManagerAVInfo(RtmpPlayAVInfo &RtmpPlayAVInfo) = 0;
	
	/**
	 * 接口功能：播放质量信息回调
	 * 参    数：RtmpPlayQuality	播放质量信息结构体，详见结构体定义
	 * 返 回 值：成功/失败
	 **/
	virtual bool OnRtmpPlayManagerPlayQuality(RtmpPlayQuality &RtmpPlayQuality){return true;};
	
	/**
	 * 接    口：OnShowVideo
	 * 接口功能：第一次显示视频前触发
	 * 调    用：pszCmd { "cmd":"OnShowVideo", "params":{} }
	 **/

	/**
	 * 接    口：OnAudioPercent
	 * 接口功能：音频数据（百分比）回调，用于上层显示自定义音量条
	 * 参    数：uPercent	音频数据（处理后的）的百分比形式
	 * 调    用：pszCmd { "cmd":"OnAudioPercent", "params":{"uPercent":12} }
	 **/
};

class IRtmpPlayManager
{
public:

	/**
	 * 接    口：JSON调用统一接口 StopConnectRlib
	 * 接口功能：获取当前使用的SDK版本号
	 * 调    用：pszCmd { "cmd":"StopConnectRlib", "params":{} }
	 * 成功返回：ppszRes { "code":0, "data":{} }
	 * 失败返回：ppszRes { "code":1, "data":{} }
	 **/

	/**
	 * 接    口：JSON调用统一接口 GetSdkVersion
	 * 接口功能：获取当前使用的SDK版本号
	 * 调    用：pszCmd { "cmd":"GetSdkVersion", "params":{} }
	 * 成功返回：ppszRes { "code":0, "data":{"version":"201407171636"} }
	 * 失败返回：ppszRes { "code":1, "data":{"reason":"no version."} }
	 **/

	/**
	 * 接    口：JSON调用统一接口 SetSYNBufTime
	 * 接口功能：设置播放缓冲延时设置
	 * 调    用：pszCmd { "cmd":"SetSYNBufTime", "params":{"dwDatalenOffset":1500, "dwMaxTimestampleInqueue":3000, "dwTimeCheckSynbuf":5000} }
	 * 返回：无
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
	*	接口功能:		开始播放
	*	参    数:		无
	*	返 回 值:		true(success), false(fail)
	*/
	virtual bool StartPlay() = 0;

	/*
	*	接口功能:		停止播放
	*	参    数:		无
	*	返 回 值:		true(success), false(fail)
	*/
	virtual bool StopPlay() = 0;

	/*
	*	接口功能:		释放对象
	*	参    数:		无
	*	返 回 值:		无
	*/
	virtual void Release() = 0;

	/*
	*	接口功能:		获取播放设备列表 （必须在 StartPlay 之前调用）
	*	参    数:		
	*		@ppDevice:	设备名称列表
	*                   指向一个wchar_t *数组的指针，数组元素（wchar_t字符数组指针，以L'\0'结尾），
	*                   内存在dll库内分配释放，上层不用处理。这里处理时限制传入的二级指针初始化null，防止内存泄漏。
	*		@size:		设备总数
	*	返 回 值:		 >=0(success), <0(fail)
	*/
	virtual int GetAudioPlayDevList(wchar_t ** &ppDevice, unsigned int &size) = 0;

	/*
	*	接口功能:		设置播放设备ID  （必须在 StartPlay 之前调用）
	*	参    数:		
	*		@uDevID:	播放设备ID(count from 0)
	*	返 回 值:		>=0(success), <0(fail)
	*/
	virtual int SetAudioPlayDev(int uDevID) = 0;

	/*
	*	接口功能:		设置播放流信息  （必须在 StartPlay 之前调用）
	*	参    数:		
	*		@pURL:		播放流地址
	*		@pAppID:	流App名称(如果启用验证，则 AppID= AppID+"?"+key, if appID is "App" and key is "kkyoo", then here is "App?kkyoo")
	*		@pStreamID:	Stream名称
	*	返 回 值:							true(success), false(fail)
	*/
	virtual bool SetStreamInfo(const char *pURL, const char *pAppID, const char *pStreamID) = 0;

	/*
	*	接口功能:		设置视频显示窗口句柄  （必须在 StartPlay 之前调用）
	*	参    数:		
	*		@hwnd:		窗口句柄
	*	返 回 值:		void
	*/
	virtual void SetVdDisplayWnd(HWND hwnd) = 0;

	/*
	*	接口功能:		设置音量  （可以在播放过程中调用）
	*	参    数:		
	*		@uOutVolum:	音量
	*		@bMute:	    是否静音
	*	返 回 值:					void
	*/
	virtual void SetAdPara(unsigned int uOutVolum, bool bMute) = 0;

	/*
	*	接口功能:		设置用户ID （必须在 StartPlay 之前调用）
	*	参    数:		
	*		@pszUserID:	用户ID
	*	返 回 值:		无
	*/
	virtual void SetClientUserID(const char *pszUserID) = 0;

	/*
	接口功能：	设置录制文件名（必须在 StartPlay 之前调用）
	参    数：	
	*		@pszFileName:录制文件路径
	返 回 值：	无
	*/
	virtual void SetRecordFileFullName(const char *pszFileName) = 0;

	/*
	函 数 名：	EnableRecordFile
	接口功能：	开启/关闭 录制音视频功能（可以在播放过程中调用）
	参    数：	bEnable 是否开启
	返 回 值：	无
	*/
	virtual void EnableRecordFile(bool bEnable) = 0;
};

extern "C" __declspec(dllexport) IRtmpPlayManager * CreateRtmpPlayManager(IRtmpPlayManagerNotify * pNotify);