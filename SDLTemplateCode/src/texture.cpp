#include "../include/texture.hpp"
#include "../include/fmtTime.hpp"

#include "SDL_image.h"

#include <cmath>

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
    using namespace fmt;

    SDL_Texture * finalTexture = nullptr;

    SDL_Surface * loadSurface = IMG_Load(__path.c_str());

    print(
        fg(terminal_color::bright_cyan), 
        "{} Load image: [{}] Complete.\n", CurrentTime(), __path
    );

    if (!loadSurface) 
    {
        print(
            fg(color::red) | emphasis::bold,
            "Unable to load image: [{}] SDL Image ERROR: {}\n",
            __path, IMG_GetError()
        );
    }
    else
    {
#if true
        /**
         * @brief SDL_SetColorKey() 函数的作用是将一张图片的指定像素设置成透明像素，
         *        并在之后的渲染中忽视这些像素，函数需要如下几个参数。
         *        
         *        1. 一张图片的平面信息，通过 IMG_Load() 获得
         *        2. 一个 SDL 布尔类型，表明是否启用该色键
         *        3. 像素点键值，使用 SDL_MapRGB() 或 SDL_MapRGBA() 来获取，
         *           上面两个函数通过传入平面的像素格式与颜色值，
         *           将这些参数映射成像素值交给外部去处理。
        */
        SDL_SetColorKey(
            loadSurface, SDL_TRUE, 
            SDL_MapRGB(loadSurface->format, 0x00, 0xFF, 0xFF)
        );
#endif
         /**
         * @brief 这里有必要说明一下，为什么 SDL_CreateTextureFromSurface() 
         *        在平面信息转纹理信息的时候，需要渲染器的参与：
         *        首先：平面信息（Surface）通常指存储在内存中的图像信息，
         *        而纹理（Texture）通常指存储在 GPU 显存里面的图像信息，
         *        所以两者的转换就必须有渲染器作为中间人，
         *        以正确的 GPU 格式进行平面信息的转换，和纹理等相关资源的管理。
        */
        finalTexture = SDL_CreateTextureFromSurface(__render, loadSurface);

        if (!finalTexture) {
            print(
                fg(color::red) | emphasis::bold,
                "Unable to create texture from surface: [{}] SDL ERROR: {}\n",
                __path, IMG_GetError()
            );
            
        }
        else {
            this->getRenderPosition().w = loadSurface->w;
            this->getRenderPosition().h = loadSurface->h;
        }

        SDL_FreeSurface(loadSurface);
    }

    this->setTextureName(__path);
    this->setTexture(finalTexture);

    return (this->getTexture() != nullptr);
}

void TextureImage::render(
    int __x, int __y, SDL_Renderer * __render,
    SDL_Rect __clips, FilpAttribution __flip
)
{
    this->getRenderPosition().x = __x;
    this->getRenderPosition().y = __y;
    this->getClipPosition() = __clips;
    this->flipAttribution   = __flip;

    SDL_Rect tempRect = {
        this->getRenderPosition().x, this->getRenderPosition().y,
        __clips.w, __clips.h
    };

    SDL_RenderCopyEx(
        __render, this->getTexture(), 
        &this->getClipPosition(), &tempRect, 
        flipAttribution.angle, &flipAttribution.center, flipAttribution.flipFlag
    );
}

TextureImage::~TextureImage()
{
    using namespace fmt;

    print(
        fg(terminal_color::bright_green),
        "{} Destory image texture [{}]\n",
        CurrentTime(), this->textureName
    );
}

bool RectangleTexture::load(
    std::string __name, int __w, int __h, SDL_Renderer * __render)
{
    using namespace fmt;

    print(
        fg(terminal_color::bright_cyan), 
        "{} Load rectangle, w = {}, h = {}\n", 
        CurrentTime(), __w, __h
    );

    SDL_Surface * rectSurface = SDL_CreateRGBSurfaceWithFormat(
        0, __w, __h, 32, SDL_PIXELFORMAT_RGBA8888
    );

    if (!rectSurface)
    {
        print(
            fg(terminal_color::red) | emphasis::bold,
            "Unable to create rectengle surface, SDL ERROR: {}\n", SDL_GetError()
        );
    }
    else
    {
#if false
        SDL_FillRect(
            rectSurface, nullptr, 
            SDL_MapRGBA(
                rectSurface->format, 
                __color.r, __color.g, __color.b, __color.a
            )
        );
#endif

        SDL_Texture * rectangleTexture = SDL_CreateTextureFromSurface(__render, rectSurface);

        if (!rectangleTexture)
        {
            print(
                fg(terminal_color::red) | emphasis::bold,
                "Unable to create rectangle texture, SDL ERROR: {}\n",
                SDL_GetError()
            );
        }
        else {
            this->rectangleName = __name;

            this->getRenderPosition().w = __w;
            this->getRenderPosition().h = __h;

            this->setTexture(rectangleTexture);
        }
    }

    SDL_FreeSurface(rectSurface);

    return (this->getTexture() != nullptr);
}

void RectangleTexture::render(int __x, int __y, SDL_Color __color, SDL_Renderer * __render, RenderFlag __renderFlag)
{
    this->getRenderPosition().x = __x;
    this->getRenderPosition().y = __y;

    SDL_SetRenderDrawColor(__render, __color.r, __color.g, __color.b, __color.a);

    if (__renderFlag == RenderFlag::WHOLE) {
        SDL_RenderFillRect(__render, &this->getRenderPosition());
    }
#if true
    else if (__renderFlag == RenderFlag::BORDER) {
        SDL_RenderDrawRect(__render, &this->getRenderPosition());
    }
#endif
}

RectangleTexture::~RectangleTexture()
{
    using namespace fmt;

    print(
        fg(terminal_color::bright_green),
        "{} Destory rectengle texture: [{}]\n",
        CurrentTime(), this->rectangleName
    );
}

bool CircleTexture::isInCircle(int __x, int __y, CircleInfo & __circleInfo)
{
    return (std::pow(__x - __circleInfo.centerX, 2) + std::pow(__y - __circleInfo.centerY, 2)) < std::pow(__circleInfo.radius, 2);
}

void CircleTexture::showCircleInfo(void)
{
    using namespace fmt;

    print(
        "Circle name: {}\n"
        "Center coordinate (x, y) = ({}, {}), radius = {}\n",
        this->circleName.c_str(), 
        this->circleInfo.centerX, this->circleInfo.centerY, 
        this->circleInfo.radius
    );
}

void CircleTexture::load(
    const std::string __name, CircleInfo __circleInfo)
{
    using namespace fmt;

    this->circleName = __name;

    print(
        fg(terminal_color::bright_cyan),
        "Load circle: [{}]\n", this->circleName
    );
    this->circleInfo = __circleInfo;

    this->showCircleInfo();
}

void CircleTexture::render(
    int __segments, SDL_Color __color, 
    SDL_Renderer * __render, RenderFlag __renderFlag
)
{
    if (__renderFlag == RenderFlag::WHOLE) {
        this->fill(__color, __render);
    }
    else if (__renderFlag == RenderFlag::BORDER) {
        this->drawOutline(__segments, __color, __render);
    }
}

void CircleTexture::drawOutline(int __segments, SDL_Color & __color, SDL_Renderer * __render)
{
    // 求每次渲染的角度增量
    const float angleStep = 2.0f * M_PI / __segments;
    float angle = 0.0f;

    SDL_SetRenderDrawColor(__render, __color.r, __color.g, __color.b, 0xFF);

    for (int count = 0; count <= __segments / 2; ++count)
    {
        /**
         * 圆的标准方程，但是三角函数乘半径这部分要四舍五入后截断成整数。
        */
        int x = static_cast<int>(std::round(std::cos(angle) * circleInfo.radius)) + circleInfo.centerX;
        int y = static_cast<int>(std::round(std::sin(angle) * circleInfo.radius)) + circleInfo.centerY;

        /**
         * 求圆上某一点的对称点也很容易。
        */
        int symmetryX = 2 * circleInfo.centerX - x;
        int symmetryY = 2 * circleInfo.centerY - y;

        SDL_RenderDrawPoint(__render, x, y);                    // 绘制圆上一点
        SDL_RenderDrawPoint(__render, symmetryX, symmetryY);    // 绘制圆上一点的对称点

        angle += angleStep; // 角度偏移量增加
    }
}

void CircleTexture::fill(SDL_Color & __color, SDL_Renderer * __render)
{
    SDL_SetRenderDrawColor(__render, __color.r, __color.g, __color.b, 0xFF);

    for (int y = 0; y <= this->circleInfo.radius; ++y)
    {
        for (int x = 0; x <= this->circleInfo.radius; ++x)
        {
            if (this->isInCircle(x + circleInfo.centerX, y + circleInfo.centerY, this->circleInfo))
            {
                SDL_RenderDrawPoint(__render, x + circleInfo.centerX, y + circleInfo.centerY);
                SDL_RenderDrawPoint(__render, x + circleInfo.centerX, -y + circleInfo.centerY);
                SDL_RenderDrawPoint(__render, -x + circleInfo.centerX, y + circleInfo.centerY);
                SDL_RenderDrawPoint(__render, -x + circleInfo.centerX, -y + circleInfo.centerY);
            }
        }
    }
}

CircleTexture::~CircleTexture()
{
    using namespace fmt;

    print(fg(terminal_color::bright_green),
        "{} Destory circle texture: [{}]\n",
        CurrentTime(), this->circleName
    );
}