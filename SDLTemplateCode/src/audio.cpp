#include "../include/audio.hpp"
#include "../include/fmtTime.hpp"

#include <sndfile.hh>

static std::string setAudioName(std::string __path)
{
    std::size_t pos = __path.rfind('/');

    if (pos != std::string::npos) { 
        return __path.substr(pos + 1); 
    }
    else { 
        return __path; 
    }
}

Audio::Audio(AudioAttribution __attr) : Audio()
{
    this->attribution = __attr;
}

#if false
double Audio::getAudioDuration(std::string & __path)
{
    using namespace fmt;

    SF_INFO soundFileInfo;
    SNDFILE * file = SF_NULL;
    double duration = 0.0;

    try
    {
        file = sf_open(__path.c_str(), SFM_READ, &soundFileInfo);

        if (!file)
        {
            throw std::runtime_error(
                "Faild to open file: [" + __path + "].\n"
            );
        }

        /**
         * @brief 音频持续时间 = 采样点数 / 采样率
        */
        duration = static_cast<double>(soundFileInfo.frames) / 
                          static_cast<double>(soundFileInfo.samplerate);
        
        sf_close(file);
        file = nullptr;
    }
    catch (const std::runtime_error & __except)
    {
        print(
            fg(terminal_color::red) | emphasis::bold, 
            "{}", __except.what()
        );

        if (file) { sf_close(file); }
    }

    return duration;
}
#endif

void Audio::init(AudioAttribution __attr)
{
    this->attribution = __attr;

    if (
        Mix_OpenAudio(
            attribution.frequency, attribution.format, 
            attribution.channels, attribution.chunkSize
        ) < 0
    )
    {
        throw std::runtime_error(
            "Unable to init SDL_mixer moudle, SDL MIXER ERROR: " + 
            std::string(Mix_GetError()) + '\n'
        );
    }
}

void Audio::init(void)
{
    using namespace fmt;

    try {
        this->init(this->attribution);
    }
    catch (const std::runtime_error & __except)
    {
        print(
            fg(terminal_color::red) | emphasis::bold, 
            "{} {}", CurrentTime(), __except.what()
        );
    }
}

void Audio::load(std::string __path)
{
    using namespace fmt;

    print(
        fg(terminal_color::bright_cyan), 
        "{} Load audio [{}].\n", CurrentTime(), __path
    );

    this->music = Mix_LoadMUS(__path.c_str());

    if (!this->music) {
        throw std::runtime_error(
            "Unable to load music: " + __path + 
            " SDL MIXER ERROR: " + std::string(Mix_GetError())
        );
    }
    else {
        duration = Mix_MusicDuration(this->music); //this->getAudioDuration(__path);
        this->audioName = setAudioName(__path);
    }
}

void Audio::play(int __playOption)
{
#if false
    using namespace fmt;
    print(
        fg(terminal_color::blue), 
        "{} Play music: [{}].\n", CurrentTime(), this->audioName
    );
#endif

    if (Mix_PlayMusic(this->music, __playOption) < 0)
    {
        throw std::runtime_error(
            "Unable to load music: " + this->audioName + 
            " SDL MIXER ERROR: " + std::string(Mix_GetError())
        );
    }
}

void Audio::stop(void)
{
    using namespace fmt;

#if false
    print(
        fg(terminal_color::yellow), 
        "Stop all playing music.\n"
    );
#endif

    Mix_HaltMusic();
}

Audio::~Audio()
{
    using namespace fmt;

    print(
        fg(color::green), 
        "{} Close music: [{}].\n", CurrentTime(), this->audioName
    );

    Mix_FreeMusic(this->music);
}