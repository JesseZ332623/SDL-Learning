#include "../include/texture.hpp"
#include "../include/fmtTime.hpp"

#include "SDL_image.h"

#include <cmath>

TextureBasic::~TextureBasic()
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

void TextureImage::load(std::string __path, SDL_Color __transparentColor, SDL_bool __ifEnble, SDL_Renderer * __render)
{
    using namespace fmt;

    if (this->getTexture() != nullptr) { 
        SDL_DestroyTexture(this->getTexture()); 
    }

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
            loadSurface, __ifEnble, 
            SDL_MapRGB(
                loadSurface->format, 
                __transparentColor.r, __transparentColor.g, __transparentColor.b
            )
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

    if (!this->getTexture()) {
        throw std::runtime_error(
            "Failed to load image: " + this->textureName + '\n'
        );
    }
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
        "{} Destory image texture [{}].\n",
        CurrentTime(), this->textureName
    );
}

void RectangleTexture::load(
    std::string __name, int __w, int __h, SDL_Color __color, SDL_Renderer * __render)
{
    using namespace fmt;

    // 若存在未销毁的纹理数据，先销毁
    if (this->getTexture()) { 
        SDL_DestroyTexture(this->getTexture()); 
    }

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
#if true
        SDL_FillRect(
            rectSurface, nullptr, 
            SDL_MapRGBA(
                rectSurface->format, 
                __color.r, __color.g, __color.b, __color.a
            )
        );       
#endif
        this->rectName  = __name;
        print(
            fg(terminal_color::bright_cyan), 
            "{} Load rectangle: [{}], w = {}, h = {}\n", 
            CurrentTime(), this->rectName, __w, __h
        );

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
            this->rectColor = __color;

            this->getRenderPosition().w = __w;
            this->getRenderPosition().h = __h;

            this->setTexture(rectangleTexture);
        }
    }

    SDL_FreeSurface(rectSurface);

    if (!this->getTexture()) {
        throw std::runtime_error(
            "Failed to load rectangle: " + this->rectName + '\n'
        );
    }
}

SDL_FPoint RectangleTexture::rotatePointAround(
    const SDL_FPoint & __pivot, const SDL_FPoint & __point, double __theta)
{
    double cosTheta = std::cos(__theta);  // 计算 theta 的正弦
    double sinTheta = std::sin(__theta);  // 计算 theta 的余弦

    // 平移（Translation）矩形上一点至原点
    double translateX = __point.x - __pivot.x;
    double translateY = __point.y - __pivot.y;

    // 开始旋转
    double rotatedX = cosTheta * translateX - sinTheta * translateY;
    double rotatedY = sinTheta * translateX - cosTheta * translateY;

    // 将旋转后的坐标反向平移回旋转中心点的位置之后返回结果
    return {__pivot.x + rotatedX, __pivot.y + rotatedY};
}

void RectangleTexture::computeAndDrawRectBorder(SDL_Renderer * __render)
{
    SDL_Color originalColor;
    SDL_GetRenderDrawColor(__render, &originalColor.r, &originalColor.g, &originalColor.b, &originalColor.a);
    SDL_SetRenderDrawColor(__render, 0, 0, 0, 0xFF);

    SDL_FPoint rotateCenter = {(float)this->rectFlip.center.x, (float)this->rectFlip.center.y};
    SDL_FPoint renderPos    = {(float)this->getRenderPosition().x, (float)this->getRenderPosition().y};

    SDL_FPoint renderPoint = this->rotatePointAround(
        rotateCenter, renderPos, this->rectFlip.angle * (M_PI / 180)
    );

    SDL_FRect renderRect = {
        this->getRenderPosition().x, this->getRenderPosition().h,
        renderPoint.x, renderPoint.y
    };
    
    SDL_RenderDrawRectF(__render, &renderRect);

    SDL_SetRenderDrawColor(__render, originalColor.r, originalColor.g, originalColor.b, originalColor.a);
}

void RectangleTexture::render(
    SDL_Renderer * __render, int __x, int __y, 
    RenderFlag __renderFlag, FilpAttribution __flip) 
{
    SDL_Rect & renderPos = this->getRenderPosition();
    renderPos.x = __x;
    renderPos.y = __y;

    // 边框的渲染范围需要微调一下。
    SDL_Rect borderPos = {
        renderPos.x - 1, renderPos.y - 1, 
        renderPos.w + 2, renderPos.h + 2
    };

    // 计算未旋转的情况下，矩形四个顶点的坐标并存储。
#if false
    this->rectPoint[0] = {(float)renderPos.x, (float)renderPos.y};
    this->rectPoint[1] = {(float)renderPos.x + (float)renderPos.w, (float)renderPos.y};
    this->rectPoint[2] = {(float)renderPos.x + (float)renderPos.w, (float)renderPos.y + (float)renderPos.h};
    this->rectPoint[3] = {(float)renderPos.x, (float)renderPos.y + (float)renderPos.h};
#endif

    if (__flip.empty())     // 不旋转
    {
        switch (__renderFlag)
        {
            // 边框渲染默认为黑色 1 像素
            case BORDER:
                SDL_SetRenderDrawColor(__render, 0, 0, 0, 0xFF);
                SDL_RenderDrawRect(__render, &borderPos);
                break;
            
            case FILLED:
                SDL_RenderCopy(__render, this->getTexture(), nullptr, &renderPos);
                break;
            
            case WHOLE:
                SDL_SetRenderDrawColor(__render, 0, 0, 0, 0xFF);
                SDL_RenderDrawRect(__render, &borderPos);
                SDL_RenderCopy(__render, this->getTexture(), nullptr, &renderPos);
                break;
            
            default:
                break;
        }
    }
    else    // 需要旋转
    {
        this->rectFlip = __flip;
        switch (__renderFlag)
        {
            // 边框渲染默认为黑色 1 像素
            case BORDER:
                //computeAndDrawRectBorder(__render);
                break;
            
            case FILLED:
                SDL_RenderCopyEx(
                    __render, this->getTexture(), 
                    nullptr, &renderPos, 
                    this->rectFlip.angle, &this->rectFlip.center, this->rectFlip.flipFlag
                );
                break;
#if false
            case WHOLE:
                computeAndDrawRectBorder(__render);
                SDL_RenderCopyEx(
                    __render, this->getTexture(), 
                    nullptr, &renderPos, 
                    this->rectFlip.angle, &this->rectFlip.center, this->rectFlip.flipFlag
                );
                break;
#endif
            
            default:
                break;
        }
    }
}

RectangleTexture::~RectangleTexture()
{
    using namespace fmt;

    print(
        fg(terminal_color::bright_green),
        "{} Destory rectengle texture: [{}].\n",
        CurrentTime(), this->rectName
    );
}

void FontsTexture::openFontFile(std::string & __path, int __fontSize)
{
    this->font = TTF_OpenFont(__path.c_str(), __fontSize);

    if (!font) {
        throw std::runtime_error(
            "Unable to load font file: " + __path + 
            " SDL TTF Error: " + TTF_GetError() + '\n'
        );
    }
}

void FontsTexture::open(std::string __path, int __fontSize)
{
    using namespace fmt;

    try { 
        this->openFontFile(__path, __fontSize); 
    } catch (const std::runtime_error & __except) {
        print(
            fg(terminal_color::red) | emphasis::bold, "{} {}", CurrentTime(), __except.what()
        );
    }

    this->fontPath = __path;
}

void FontsTexture::load(std::string __textContent, SDL_Color __textColor, SDL_Renderer * __render)
{
    using namespace fmt;

    // 若存在未销毁的纹理数据，先销毁
    if (this->getTexture()) { 
        SDL_DestroyTexture(this->getTexture());
    }

    SDL_Texture * finalTexture = nullptr;
    SDL_Surface * textSurface = TTF_RenderText_Blended(
                                    this->font, __textContent.c_str(), __textColor
                                );
    
    if (!textSurface) {
        print(
            fg(terminal_color::bright_red) | emphasis::bold,
            "Unable to create text surface, SDL TTF ERROR: {}.\n",
            TTF_GetError()
        );
    }
    else
    {
        this->renderContent = __textContent;
        finalTexture = SDL_CreateTextureFromSurface(__render, textSurface);

        if (!finalTexture) {
            print(
                fg(terminal_color::bright_red) | emphasis::bold,
                "Unable to create texture from surface, SDL ERROR {}: ",
                SDL_GetError()
            );
        }
        else
        {
            this->getRenderPosition().w = textSurface->w;
            this->getRenderPosition().h = textSurface->h;
        }

        SDL_FreeSurface(textSurface);
        this->setTexture(finalTexture);
    }

    if (!this->getTexture()) {
        throw std::runtime_error(
            "Failed to load string texture: " + this->renderContent + '\n'
        );
    }
}

void FontsTexture::render(
    int __x, int __y, SDL_Renderer * __render, FilpAttribution __flip
) {
    this->getRenderPosition().x = __x;
    this->getRenderPosition().y = __y;
    this->flipAttribution       = __flip;

    SDL_RenderCopyEx(
        __render, this->getTexture(), 
        nullptr, &this->getRenderPosition(),
        flipAttribution.angle, &flipAttribution.center, flipAttribution.flipFlag
    );
}

FontsTexture::~FontsTexture()
{
    using namespace fmt;

    print(
        fg(terminal_color::bright_green),
        "{} Close font file: [{}].\n", CurrentTime(), this->fontPath
    );
    TTF_CloseFont(this->font);

    print(
        fg(terminal_color::bright_green),
        "{} Destory font texture.\n", CurrentTime()
    );
}

bool CircleTexture::isInCircle(int __x, int __y, CircleInfo & __circleInfo) {
    return (std::pow(__x - __circleInfo.centerX, 2) + std::pow(__y - __circleInfo.centerY, 2)) < std::pow(__circleInfo.radius, 2);
}

void CircleTexture::showCircleInfo(void)
{
    using namespace fmt;

    print(
        "Circle Center coordinate (x, y) = ({}, {}), radius = {}\n",
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
        "{} Load circle: [{}].\n", CurrentTime(), this->circleName
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
        "{} Destory circle texture: [{}].\n",
        CurrentTime(), this->circleName
    );
}