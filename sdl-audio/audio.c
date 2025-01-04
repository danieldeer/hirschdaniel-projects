#include <SDL2/SDL.h>
#include <stdio.h>

int main()
{
	SDL_AudioSpec spec;
	Uint8* audio_buf;
	Uint32 audio_len;
	SDL_InitSubSystem(SDL_INIT_AUDIO);	
	SDL_AudioSpec* return_spec = SDL_LoadWAV("sample.wav", &spec, &audio_buf, &audio_len);
	if(return_spec == NULL)
	{
		printf("Error opening the specified file\n");
	}
	else
	{
		printf("Audio file was successfully loaded.\n");
	}
	
	SDL_AudioDeviceID dev = SDL_OpenAudioDevice(
                          NULL,
                          0,
                          return_spec,
                          return_spec,
                          0);
	printf("Device ID is %d\n", dev);
	int res = SDL_QueueAudio(dev, audio_buf, audio_len);
	if(res == 0)
	{
		printf("Success queueing the audio.\n");
	}
	else
	{
		printf("Failure queueing the audio.\n");
	}
	SDL_PauseAudioDevice(dev, 0);
	SDL_Delay(10000);

	SDL_CloseAudioDevice(dev);
	SDL_FreeWAV(audio_buf);
	SDL_Quit();
}
