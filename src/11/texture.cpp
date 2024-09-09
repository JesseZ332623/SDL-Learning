#include "../../include/11/texture.hpp"

#include <cstring>
#include <MyLib/myLogerDef.h>

TextureBisic::~TextureBisic()
{
    if (this->getTexture() != nullptr) {
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
    }
}

void TextureImage::setTextureName(std::string __path)
{
    std::size_t pos = __path.rfind('/');

    if (pos != std::string::npos) { 
        this->textureName = __path.substr(pos); 
    }
    else { this->textureName = __path; }
}

bool TextureImage::load(std::string __path, SDL_Renderer * __render)
{
    using namespace MyLib::MyLoger;

    SDL_Texture * finalTexture = nullptr;

    NOTIFY_LOG("Load image [" + __path + "]\n");
    SDL_Surface * loadSurface = IMG_Load(__path.c_str());

    if (!loadSurface) 
    {
        ERROR_LOG("Unable to load image: " + __path + 
                  " SDL Image ERROR: " + std::string(IMG_GetError()) + '\n'
        );
    }
    else
    {
#if false
        SDL_SetColorKey(
            loadSurface, SDL_TRUE, 
            SDL_MapRGB(loadSurface->format, 0x00, 0xFF, 0xFF)
        );
#endif

        finalTexture = SDL_CreateTextureFromSurface(__render, loadSurface);

        if (!finalTexture) {
            ERROR_LOG(
                "Unable to create texture from " + __path + 
                " SDL ERROR: " + std::string(SDL_GetError()) + '\n'
            );
        }
        else {
            this->getRenderPosition().w = loadSurface->w;
            this->getRenderPosition().h = loadSurface->h;
        }

        SDL_FreeSurface(loadSurface);
    }

    this->setTexture(finalTexture);

    return (this->getTexture() != nullptr);
}

void TextureImage::render(int __x, int __y, SDL_Rect __clips, SDL_Renderer * __render)
{
    this->getRenderPosition().x = __x;
    this->getRenderPosition().y = __y;
    std::memcpy(&this->getClipPosition(), &__clips, sizeof(SDL_Rect));

    SDL_Rect tempRect = {
        this->getRenderPosition().x, this->getRenderPosition().y,
        __clips.w, __clips.h
    };

    SDL_RenderCopy(
        __render, this->getTexture(), 
        &this->getClipPosition(), &tempRect
    );
}