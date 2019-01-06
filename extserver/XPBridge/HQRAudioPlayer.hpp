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


#ifndef __HQRAUDIOPLAYER_H__
#define __HQRAUDIOPLAYER_H__


#include "XPBridge.hpp"
#include "XBuzzer.hpp"
#include "HQRover.hpp"


#define HQRAudioPlayer_API_setNoteParameter	(1)	/*!< void setNoteParameter(uint8_t beatTime, uint8_t noteGap) */
#define HQRAudioPlayer_API_playNote			(2) /*!< void playNote(uint8_t note, uint8_t scale, uint8_t tone, uint8_t beat) */
#define HQRAudioPlayer_API_playMusic		(3)	/*!< void playMusic(uint8_t id) */
#define HQRAudioPlayer_API_playSound		(4)	/*!< void playSound(uint8_t id) */
#define HQRAudioPlayer_API_stop				(5)	/*!< void stop() */
#define HQRAudioPlayer_API_playTone			(6)	/*!< void playTone() */


#define MUSIC_SHENGRIKUAILE		1
#define MUSIC_XIAOXINXIN		2
#define MUSIC_HUANLESONG		3
#define MUSIC_XIAOPINGGUO		4
#define MUSIC_SHANGXUEGE		5

#define MUSIC_SUM				5


#define SOUND_AMBULANCE			1
#define SOUND_FIRE_ENGINE		2
#define SOUND_POLICE_WAGON		3
#define SOUND_CAR_WHISTLING		4

#define SOUND_SUM				4  //exclude SOUND_CAR_WHISTLING


class HQRAudioPlayer : public XPServer {
public:
	HQRAudioPlayer();
	~HQRAudioPlayer();

    int setup(XBuzzer *buz);

    void playTone(uint16_t frequency, uint32_t duration);

	void setNoteParameter(uint8_t beatTime, uint8_t noteGap);

	int playNote(uint8_t note, uint8_t scale, uint8_t tone, uint8_t beat);

	void playMusic(uint8_t id);

	void playSound(uint8_t id);

	void stop();

private:
	int mBeatTime;	// millisecond, default 500ms
};


#endif //__HQRAUDIOPLAYER_H__
