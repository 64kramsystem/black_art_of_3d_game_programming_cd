
// Header File for Library Module BLACK6.C

// D E F I N E S ////////////////////////////////////////////////////////////

// return values for digital sound status function

#define SOUND_STOPPED     0       // no sound is playing
#define SOUND_PLAYING     1       // a sound is playing

// return values for the midi sequence status function

#define SEQUENCE_STOPPED     0    // the current sequence is stopped
#define SEQUENCE_PLAYING     1    // the current sequence is playing
#define SEQUENCE_COMPLETE    2    // the current sequence has completed
#define SEQUENCE_UNAVAILABLE 0    // this sequence is unavailable

// these return values are used to determine what happened when a midi file
// has been registered

#define XMIDI_UNREGISTERED  0 // the midi file couldn't be registered at all
#define XMIDI_BUFFERED      1 // the midi file was registered and buffered
#define XMIDI_UNBUFFERED    2 // the midi file was registered, but was too
										// big to be buffered, hence, the caller
										// needs to keep the midi data resident in
										// memory

// T Y P E D E F S //////////////////////////////////////////////////////////



// the DIGIPAK sound structure

typedef struct SNDSTRUC_typ
		  {

		  unsigned char far *sound;  // a pointer to the raw sound data
		  unsigned short sndlen;      // the length of the sound data in bytes
		  short far *IsPlaying;       // a pointer to a variable that will be
												// used to hold the status of a playing
												// sound
		  short frequency;            // the frequency in hertz that the
												// sound should be played at

		  } SNDSTRUC, *SNDSTRUC_PTR;


// our high level sound structure

typedef struct sound_typ
		  {
		  unsigned char far *buffer;   // pointer to the start of VOC file

		  short status;                // the current status of the sound

		  SNDSTRUC SS;                 // the DIGIPAK sound structure

		  } _sound, *_sound_ptr;


// this holds a midi file

typedef struct music_typ
		  {

		  unsigned char far *buffer;   // pointer to midi data
		  long size;                   // size of midi file in bytes
		  int status;                  // status of song
		  int register_info;           // return value of RegisterXmidiFile

		  } music, *music_ptr;

// E X T E R N A L S ////////////////////////////////////////////////////////


// P R O T O T Y P E S  /////////////////////////////////////////////////////

int Sound_Load(char *filename, _sound_ptr sound,int translate);

void Sound_Unload(_sound_ptr the_sound);

void Sound_Translate(_sound_ptr the_sound);

void Sound_Play(_sound_ptr the_sound);

int Sound_Status(void);

void Sound_Stop(void);



int Music_Load(char *filename, music_ptr the_music);

void Music_Unload(music_ptr the_music);

int Music_Register(music_ptr the_music);

void Music_Stop(void);

int Music_Play(music_ptr the_music,int sequence);

void Music_Resume(void);

int Music_Status(void);



