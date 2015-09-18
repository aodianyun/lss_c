#pragma once
typedef enum eMsgLevel{MSG_DEBUG=0, MSG_WARNING, MSG_ERR};
struct RtmpPublishNetInfo
{
	unsigned int uPing;
	unsigned int uVideoBitSum;
	//unsigned int uVideoBitRate;
	float			fVideoBitRate;
	unsigned int uAudioBitSum;
	//unsigned int uAudioBitRate;
	float			fAudioBitRate;
	unsigned int uDelay;
	unsigned int uDiscardBytesSum;
	unsigned int uVideoFrameSum;
	unsigned int uVideoFrameRate;
	unsigned int uAudioFrameSum;
};
struct RtmpPublishComputerStatusInfo
{
	unsigned int uTotalPhys;
	unsigned int uAvailPhys;
	unsigned int uTotalVirtual;
	unsigned int uAvailVirtual;
	unsigned int uCPUClockSpeed;
	unsigned int uCPUCores;
	unsigned int uCPUUsage;
	unsigned int uUpSpeed;
	unsigned int uDownSpeed;
	unsigned int uCurProcCPUUsage;
	unsigned int		uPublishPing;
};

struct RtmpPlayAVInfo
{
	unsigned int uTotalBufTime;
	unsigned int uAudioBufTime;
	unsigned int uVideoBufTime;
	unsigned int uVideoWidth;
	unsigned int uVideoHeight;
};
struct RtmpPlayQuality
{
	unsigned int uShowVideoSucTime;
	unsigned int uBufferEmptyCount;
	unsigned int uBufferFullCount;
	unsigned int uMaxPlayBuffer;
	unsigned int uMinPlayBuffer;
	unsigned int uAvgPlayBuffer;
	float fTotalPlayTime;
};