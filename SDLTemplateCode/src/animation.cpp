#include "../include/animation.hpp"
#include "../include/fmtTime.hpp"

#include <stdexcept>

void GIFAnimation::load(std::string __path, SDL_Renderer * __render)
{
    using namespace fmt;

    print(
        fg(terminal_color::blue), 
        "{} Load GIF Animation: [{}].\n", CurrentTime(), __path
    );

    this->animation = IMG_LoadAnimation(__path.c_str());

    if (!animation)
    {
        throw std::runtime_error(
            "Unable to load animation: " + __path + 
            "SDL Image Error: " + IMG_GetError() + '\n'
        );
    }

    for (int index = 0; index < this->animation->count; ++index)
    {
        SDL_Texture * tempTexture = SDL_CreateTextureFromSurface(__render, (this->animation->frames)[index]);

        if (!tempTexture)
        {
            throw std::runtime_error(
                "Unable to convert surface to texture, Frame: [" + std::to_string(index) + "]\n"
                "SDL Image Error: " + IMG_GetError() + '\n'
            );
        }

        this->animationTextures.push_back(tempTexture); 
    }

    this->animationPath = __path;
}

void GIFAnimation::frameExtraction(int __frameIndex)
{
    if (__frameIndex < 1 || __frameIndex > this->animation->count) { return; }

    SDL_DestroyTexture(*(this->animationTextures.begin() + __frameIndex - 1));

    this->animationTextures.erase(this->animationTextures.begin() + __frameIndex - 1);
}

void GIFAnimation::frameInsert(int __frameIndex, SDL_Texture * __frameTexure)
{
    if (__frameIndex < 1 || __frameIndex > this->animation->count) { return; }
    if (!__frameTexure) { return; }

    this->animationTextures.insert(
        this->animationTextures.begin() + __frameIndex - 1, __frameTexure
    );
}

void GIFAnimation::setFrameColor(int __frameIndex, SDL_Color __color)
{
    if (__frameIndex < 0 || __frameIndex > this->animation->count) { return; }

    if (__frameIndex == ALL_FRAMES) 
    {
        for (auto & texture : this->animationTextures)
        {
            SDL_SetTextureColorMod(texture, __color.r, __color.g, __color.b);
        }
    }
    else 
    {
        SDL_SetTextureColorMod(
            this->animationTextures.at(__frameIndex - 1), 
            __color.r, __color.g, __color.b
        );
    }
}

void GIFAnimation::setFrameAlpha(int __frameIndex, Uint8 __alpha)
{
    if (__frameIndex < 0 || __frameIndex > this->animation->count) { return; }

    if (__frameIndex == ALL_FRAMES) 
    {
        for (auto & texture : this->animationTextures)
        {
            SDL_SetTextureAlphaMod(texture, __alpha);
        }
    }
    else 
    {
        SDL_SetTextureAlphaMod(this->animationTextures.at(__frameIndex - 1), __alpha);
    }
}

void GIFAnimation::setBlendMode(int __frameIndex, SDL_BlendMode __blending)
{
    if (__frameIndex < 0 || __frameIndex > this->animation->count) { return; }

    if (__frameIndex == ALL_FRAMES) 
    {
        for (auto & texture : this->animationTextures)
        {
            SDL_SetTextureBlendMode(texture, __blending);
        }
    }
    else 
    {
        SDL_SetTextureBlendMode(this->animationTextures.at(__frameIndex - 1), __blending);
    }
}

GIFAnimation::~GIFAnimation()
{
    using namespace fmt;

    print(
        fg(terminal_color::bright_green), 
        "{} Destory GIF animation: [{}].\n", CurrentTime(), this->animationPath
    );

    IMG_FreeAnimation(this->animation);
    this->animation = nullptr;

    for (auto & texture : this->animationTextures)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}