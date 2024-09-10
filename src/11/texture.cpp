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
        this->textureName = __path.substr(pos + 1); 
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
        this->setTextureName(__path);
#if true
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

TextureImage::~TextureImage()
{
    using namespace MyLib::MyLoger;

    CORRECT_LOG("Destory image texture [" + this->textureName + "]\n");
}

bool RectengleTexture::load(
    std::string __name, int __w, int __h, SDL_Renderer * __render)
{
    using namespace MyLib::MyLoger;

    NOTIFY_LOG(
        "Load rectangle, w = " + std::to_string(__w) + 
        " h = " + std::to_string(__h) + '\n'
    );

    SDL_Texture * rectangleTexture = SDL_CreateTexture(
        __render, SDL_PIXELFORMAT_RGB888, 
        SDL_TEXTUREACCESS_STATIC, __w, __h
    );

    if (!rectangleTexture)
    {
        ERROR_LOG(
            "loadRectangle(): Unable to create texture, SDL ERROR: " + 
            std::string(SDL_GetError()) + '\n'
        );
    }
    else {
        this->textureName = __name;
        this->getRenderPosition().w = __w;
        this->getRenderPosition().h = __h;
    }

    this->setTexture(rectangleTexture);

    return (this->getTexture() != nullptr);
}

void RectengleTexture::render(
    int __x, int __y, SDL_Color __color, SDL_Renderer * __render)
{
    this->getRenderPosition().x = __x;
    this->getRenderPosition().y = __y;

    SDL_SetRenderDrawColor(
        __render, __color.r, __color.g, __color.b, 0xFF
    );
    SDL_RenderFillRect(__render, &this->getRenderPosition());
}

RectengleTexture::~RectengleTexture()
{
    using namespace MyLib::MyLoger;

    CORRECT_LOG(
        "Destory rectengle texture: [" + this->textureName + "]\n"
    );
}