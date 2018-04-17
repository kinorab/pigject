#include "audio.h"
#include <stdexcept>
#include <iostream>

sf::Sound Audio::sound1;
sf::Music Audio::bgmusic;
static sf::SoundBuffer buffer1;
static float bufferBgVolume = 100.0f;
static float bufferVolume1 = 50.0f;

// preload all audio files
void Audio::initialize() {
	try {
		// if memory violation happen, reset the lib connector of project (-d have something bug)
		if (!buffer1.loadFromFile("s1.wav")) {
			throw std::runtime_error("Cannot get the sound file.");
		}
		if (!Audio::bgmusic.openFromFile("bg.wav")) {
			throw std::runtime_error("Cannot get the music file.");
		}
		Audio::sound1.setBuffer(buffer1);
		Audio::sound1.setVolume(bufferVolume1);
	}
	catch (std::runtime_error &ex) {
		std::cout << "Runtime_error: " << ex.what() << std::endl;
	}
}
