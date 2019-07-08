# Handmade Hero Day019, Improving Audio Synchronization

* day19-20Ԥ�ƽ�������windows layer�������������
* ��Ҫ�����Ǵ�����֮֡���ֵ�������Ч�����⣬sound gap

## Debug Method

ֱ��Ouput����Ļ
* ��¼���15��������PlayCursor�Լ�WriteCursor��λ��
* ��SoundBuffer��PlayCursor�Լ�WriteCursor�������Ļ����
* ��SoundBuferӳ�䵽RenderBuffer�����²�������
   
�����Log��
* ÿһ��Frame��ӡ������SoundBuffer����ز�������
	* LastPlayCursor, ��һ�ν��м����ʱ��ʹ�õ�PlayCursorλ��
	* ByteToLock, ��ǰ��Ҫ��ʼд���λ��
	* TargetCursor, ����PlayCursor�����ӳٵĲ��Ÿı�λ��
	* PlayCursor, ��ӡ�������ʱ���PlayCursor
	* WriteCursor,  ��ӡ�������ʱ���WriteCursor
		
* ���������Ĺ���ֻ����Լ���Ҫ����־��ֻ�������������

## �ܽ�

* ǰ��һ�󲿷�
	
  
# day020 Debugging the Audio Sync

* ���������ԭ��

![day020-01](day020-01.png)


```c++
if(!SoundIsValid)
{
	GlobalSoundOutput.RunningSampleIndex = WriteCursor / GlobalSoundOutput.BytesPerSample;
	SoundIsValid = true;
}
DWORD ByteToLock = ((GlobalSoundOutput.RunningSampleIndex*GlobalSoundOutput.BytesPerSample) %
	                 GlobalSoundOutput.SecondaryBufferSize);

DWORD ExpectedSoundBytesPerFrame =
	(GlobalSoundOutput.SamplesPerSecond*GlobalSoundOutput.BytesPerSample) / GameUpdateHz;
float SecondsLeftUntilFlip = (TargetSecondsPerFrame - FromBeginToAudioSeconds);
DWORD ExpectedBytesUntilFlip = (DWORD)((SecondsLeftUntilFlip/TargetSecondsPerFrame)*(float)ExpectedSoundBytesPerFrame);

// �������������ķ���һ֡������
DWORD ExpectedFrameBoundaryByte = PlayCursor + ExpectedSoundBytesPerFrame;
                        
DWORD SafeWriteCursor = WriteCursor;
// ����wc��pcʹ wc > pc
if(SafeWriteCursor < PlayCursor)
{
	SafeWriteCursor += GlobalSoundOutput.SecondaryBufferSize;
}
Assert(SafeWriteCursor >= PlayCursor);
// add samplepersecond / 30 / 3
SafeWriteCursor += GlobalSoundOutput.SafetyBytes;

// game��������һ֡�ĵط��������ݣ������ǰ��дָ���λ�ô��ڵ�ǰ������д��ַ����ô�ͻ���һ���ֵ�����������д��
bool AudioCardIsLowLatency = (SafeWriteCursor < ExpectedFrameBoundaryByte);                        

// �����lowlantency˵����֪����һ֡�����µ�����֮ǰ���������ݶ��Ѿ�д�룬��ô����д��
// ����ֻ�ܴӵ�ǰ��wc��ʼд�룬����һ֡���ݼ���
DWORD TargetCursor = 0;
if(AudioCardIsLowLatency)
{
	TargetCursor = (ExpectedFrameBoundaryByte + ExpectedSoundBytesPerFrame);
}
else
{
	TargetCursor = (WriteCursor + ExpectedSoundBytesPerFrame +
	                GlobalSoundOutput.SafetyBytes);
}
TargetCursor = (TargetCursor % GlobalSoundOutput.SecondaryBufferSize);
```
