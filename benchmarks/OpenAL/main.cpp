#include <al/al.h>
#include <al/alc.h>
#define AL_ALEXT_PROTOTYPES
#include <al/alext.h>
#include <stdio.H>
#include <math.h>
#include <time.h>

#define M_PI 3.14159
#define ITERATIONS 200
#define BLOCK_SIZE 1024
#define NUM_SOURCES 150
#define BUFFER_LENGTH 22050

#define ABORT_ON_ERROR(x) if(alGetError() != AL_NO_ERROR || alcGetError(device) != ALC_NO_ERROR) {\
	printf(x);\
	return 1;\
}

static ALuint createSineWave(void)
{
    ALshort data[BUFFER_LENGTH];
    ALuint buffer;
    ALenum err;
    ALuint i;

    for(i = 0;i < BUFFER_LENGTH;i++)
        data[i] = (ALshort)(sin(i/44100.0 * 1000.0 * 2.0*M_PI) * 32767.0);

    /* Buffer the audio data into a new buffer object. */
    buffer = 0;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, AL_FORMAT_MONO16, data, sizeof(data), 44100);

    /* Check if an error occured, and clean up if so. */
    err = alGetError();
    if(err != AL_NO_ERROR)
    {
        fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
        if(alIsBuffer(buffer))
            alDeleteBuffers(1, &buffer);
        return 0;
    }

    return buffer;
}

int main() {
	ALCdevice *device = nullptr;
	ALCcontext *context = nullptr;
	ALCint attrs[] = {
		ALC_MONO_SOURCES, NUM_SOURCES,
		ALC_FREQUENCY, 44100,
		ALC_FORMAT_CHANNELS_SOFT, ALC_STEREO_SOFT,
		ALC_FORMAT_TYPE_SOFT, ALC_SHORT_SOFT,
		ALC_HRTF_SOFT, AL_TRUE,
		0};
	device = alcLoopbackOpenDeviceSOFT(NULL);
	if(device == NULL) {
		printf("Couldn't get device.\n");
		return 1;
	}
	context = alcCreateContext(device, attrs);
	if(context == NULL || alcMakeContextCurrent(context) == ALC_FALSE) {
		printf("Couldn't get context.\n");
		return 1;
	}
	//Sources and buffer.
	auto buffer = createSineWave();
	if(buffer == 0) {
		printf("Couldn't get buffer.\n");
		return 1;
	}
	ALuint sources[NUM_SOURCES] = {0};
	alGenSources(NUM_SOURCES, sources);
	ABORT_ON_ERROR("Couldn't create sources.\n");
	for(int i = 0; i < NUM_SOURCES; i++) {
		alSourcei(sources[i], AL_BUFFER, buffer);
		alSourcei(sources[i], AL_LOOPING, AL_TRUE);
		alSourcePlay(sources[i]);
	}
	ABORT_ON_ERROR("Couldn't set up source properties.\n");
	printf("Measuring. num_sources=%i, block_size=%i, iterations=%i\n", NUM_SOURCES, BLOCK_SIZE, ITERATIONS);
	ALshort block[BLOCK_SIZE*2];
	auto start = clock();
	for(int i = 0; i < ITERATIONS; i++) {
		alcRenderSamplesSOFT(device, block, BLOCK_SIZE);
	}
	auto end = clock();
	double duration = (end-start)/(double)CLOCKS_PER_SEC;
	printf("Took %f seconds.\n", duration);
	double synthesized = BLOCK_SIZE*ITERATIONS/44100.0;
	printf("Synthesized %f seconds of audio.\n", synthesized);
	printf("Estimated maximum of %f sources before realtime synthesis is impossible.\n", synthesized*NUM_SOURCES/duration);
	alDeleteSources(NUM_SOURCES, sources);
	alDeleteBuffers(1, &buffer);
	alcDestroyContext(context);
	alcCloseDevice(device);
}
