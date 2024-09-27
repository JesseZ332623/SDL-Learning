#include "../include/audio.hpp"
#include "../include/fmtTime.hpp"

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

void Audio::init(AudioAttribution & __attr)
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
            "Unable to open default device, SDL MIXER ERROR: " + 
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

    if (this->music) {
        Mix_FreeMusic(this->music);
        this->music = nullptr;
    }

    print(
        fg(terminal_color::bright_cyan), 
        "{} Load audio [{}].\n", CurrentTime(), __path
    );

    this->music = Mix_LoadMUS(__path.c_str());

    if (!this->music) 
    {
        throw std::runtime_error(
            "Unable to load music: " + __path + '\n' +
            "SDL MIXER ERROR: " + std::string(Mix_GetError())
        );
    }
    else 
    {
        this->duration  = Mix_MusicDuration(this->music);
        this->audioName = setAudioName(__path);
    }
}

void Audio::play(int __loops)
{
    using namespace fmt;

    print(
        fg(terminal_color::blue), 
        "{} Play music: [{}].\n", CurrentTime(), this->audioName
    );

    if (Mix_PlayMusic(this->music, __loops) < 0)
    {
        throw std::runtime_error(
            "Unable to play music: " + this->audioName + '\n' +
            "SDL MIXER ERROR: " + std::string(Mix_GetError())
        );
    }
}

int Audio::fastForward(int __seconds)
{
    // 获取当前音乐播放到第几秒
    double musicCurrentPos = Mix_GetMusicPosition(this->music);
    double newPlayPos      = musicCurrentPos + __seconds;

    double correctNewPlayPos = std::max(0.0, std::min(newPlayPos, this->getDuration()));

    // 设置快进后的播放位置。
    Mix_SetMusicPosition(correctNewPlayPos);

    if (!Mix_PlayingMusic()) 
    { 
        if (Mix_PlayMusic(this->music, 0) < 0)
        {
            throw std::runtime_error(
                "Unable to play music: " + this->audioName + '\n' +
                "SDL MIXER ERROR: " + std::string(Mix_GetError())
            );
        }
    }

    return (correctNewPlayPos == this->getDuration()) ? -1 : 0;
}

int Audio::fastRewind(int __seconds)
{
    // 获取当前音乐播放到第几秒
    double musicCurrentPos = Mix_GetMusicPosition(this->music);
    double newPlayPos      = musicCurrentPos - __seconds;

    double correctNewPlayPos = std::max(0.0, std::min(newPlayPos, this->getDuration()));

    // 设置快退后的播放位置。
    Mix_SetMusicPosition(correctNewPlayPos);

    if (!Mix_PlayingMusic()) 
    { 
        if (Mix_PlayMusic(this->music, 0) < 0)
        {
            throw std::runtime_error(
                "Unable to play music: " + this->audioName + '\n' +
                "SDL MIXER ERROR: " + std::string(Mix_GetError())
            );
        }
    }

    return (correctNewPlayPos == 0.0) ? -1 : 0;
}

void Audio::pause(void)
{
    if (!this->ifPause)
    {
        if (Mix_PlayingMusic()) 
        { 
            Mix_PauseMusic();
            this->ifPause = true;
        }
    }
}

void Audio::resume(void)
{
    if (this->ifPause && Mix_PausedMusic())
    {
        Mix_ResumeMusic();
        this->ifPause = false;
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

SoundEffects::SoundEffects(AudioAttribution __attr) : SoundEffects()
{
    this->attribution = __attr;
}

void SoundEffects::init(AudioAttribution & __attr)
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
            "Unable to open default device, SDL MIXER ERROR: " + 
            std::string(Mix_GetError()) + '\n'
        );
    }
}

void SoundEffects::init(bool __ifInit)
{
    if (__ifInit) {
        this->init(this->attribution);
    }  
}

void SoundEffects::load(std::string __path)
{
    using namespace fmt;

    if (this->soundEffect) {
        Mix_FreeChunk(this->soundEffect);
        this->soundEffect = nullptr;
    }

    print(
        fg(terminal_color::bright_cyan), 
        "{} Load sound effect [{}].\n", CurrentTime(), __path
    );

    this->soundEffect = Mix_LoadWAV(__path.c_str());

    if (!this->soundEffect) 
    {
        throw std::runtime_error(
            "Unable to load sound effect: " + __path + '\n' +
            "SDL MIXER ERROR: " + std::string(Mix_GetError())
        );
    }
    else {
        this->effectName = setAudioName(__path);
    }
}

void SoundEffects::play(int channel, int __loops)
{
    using namespace fmt;

    this->playChannel = Mix_PlayChannel(channel, this->soundEffect, __loops);

    if (this->playChannel != -1)
    {
        print(
            fg(terminal_color::bright_cyan), 
            "{} Play sound effect: [{}] in channel: [{}].\n", 
            CurrentTime(), this->effectName, this->playChannel
        );
    }

# if false
    else
    {
        throw std::runtime_error(
            "Unable to play sound effect: " + this->effectName + '\n' +
            "SDL MIXER ERROR: " + std::string(Mix_GetError())
        );
    }
#endif
}

SoundEffects::~SoundEffects()
{
    using namespace fmt;

    print(
        fg(color::green), 
        "{} Close sound effect: [{}].\n", CurrentTime(), this->effectName
    );

    Mix_FreeChunk(this->soundEffect);
}