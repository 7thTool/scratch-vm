/**
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */



#include "HQRAudioPlayer.hpp"


#if 0
#define LOGV(x)     printf x
#define LOGI(x)     printf x
#define LOGW(x)     printf x
#define LOGE(x)     printf x
#else
#define LOGV(x)
#define LOGI(x)
#define LOGW(x)
#define LOGE(x)
#endif


HQRAudioPlayer::HQRAudioPlayer() :
    XPServer(HQRSVR_TYPE_AUDIOPLAYER),
	mBeatTime(500)
{
    LOGI(("HQRAudioPlayer::HQRAudioPlayer()\n"));
}

HQRAudioPlayer::~HQRAudioPlayer()
{
    LOGI(("HQRAudioPlayer::~HQRAudioPlayer()\n"));
}

int HQRAudioPlayer::setup(XBuzzer *buz)
{
    uint8_t param = buz->uid();

    LOGI(("HQRAudioPlayer::setup(%d)\n", buz->uid()));

    return XPServer::setup(&param, 1);
}

void HQRAudioPlayer::playTone(uint16_t frequency, uint32_t duration)
{
    uint8_t param[6], *p=param;

    LOGI(("HQRAudioPlayer::playTone(%d, %d)\n", frequency, duration));

    p = fillU16(p, frequency);
    p = fillU32(p, duration);
    access(HQRAudioPlayer_API_playTone, (const uint8_t *)param, 6, NULL, 0, (int)duration);
}

void HQRAudioPlayer::setNoteParameter(uint8_t beatTime, uint8_t noteGap)
{
    uint8_t param[2], *p=param;

    LOGI(("HQRAudioPlayer::setNoteParameter(%d, %d)\n", beatTime, noteGap));

    p = fillU8(p, beatTime);
    p = fillU8(p, noteGap);
    access(HQRAudioPlayer_API_setNoteParameter, (const uint8_t *)param, 2, NULL, 0);
	mBeatTime = beatTime * 10;
}

int HQRAudioPlayer::playNote(uint8_t note, uint8_t scale, uint8_t tone, uint8_t beat)
{
    uint8_t param[4], *p=param;
	int timeout = 0;

    LOGI(("HQRAudioPlayer::playNote(%d, %d, %d, %d)\n", note, scale, tone, beat));

	if (beat == 1) { // 1/8节拍
		timeout = mBeatTime / 8;
	} else if (beat == 2) {	// 1/4节拍
		timeout = mBeatTime / 4;
	} else if (beat == 4) {	// 1/2节拍
		timeout = mBeatTime / 2;
	} else if (beat == 8) {	// 1节拍
		timeout = mBeatTime;
	} else if (beat == 16) {	// 2节拍
		timeout = mBeatTime * 2;
	}

    p = fillU8(p, note);
    p = fillU8(p, scale);
    p = fillU8(p, tone);
    p = fillU8(p, beat);
    return access(HQRAudioPlayer_API_playNote, (const uint8_t *)param, 4, NULL, 0, timeout);
}

void HQRAudioPlayer::playMusic(uint8_t id)
{
    LOGI(("HQRAudioPlayer::playMusic(%d)\n", id));

    access(HQRAudioPlayer_API_playMusic, (const uint8_t *)&id, 1, NULL, 0);
}

void HQRAudioPlayer::playSound(uint8_t id)
{
    LOGI(("HQRAudioPlayer::playSound(%d)\n", id));

    access(HQRAudioPlayer_API_playSound, (const uint8_t *)&id, 1, NULL, 0);
}

void HQRAudioPlayer::stop()
{
    LOGI(("HQRAudioPlayer::stop()\n"));

    access(HQRAudioPlayer_API_stop, NULL, 0, NULL, 0);
}
