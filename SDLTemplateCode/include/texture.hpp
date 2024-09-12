#include "SDL.h"
#include "SDL_image.h"

#include <string>

/**
 * @brief 封装了 SDL 纹理的基类，声明对于纹理的一些通用操作。 
*/
class TextureBisic
{
    private:
        SDL_Texture * texture;      // SDL 纹理
        SDL_Rect    renderPosition; // 纹理的原始长宽，以及在屏幕中的渲染位置
    
    public:
        TextureBisic(void) : texture(nullptr), renderPosition({0}) {}

        /**
         * @brief 设置 SDL 纹理。
        */
        void setTexture(SDL_Texture * __t) { this->texture = __t; }

        /**
         * @brief 获取 SDL 纹理。
        */
        SDL_Texture * getTexture(void) const { return this->texture; }

        /**
         * @brief 获取该纹理的原始长宽的副本，以及该纹理渲染在屏幕的位置。
        */
        SDL_Rect getRenderPosition(void) const { return this->renderPosition; }

        /**
         * @brief 获取该纹理的原始长宽的引用，以及该纹理渲染在屏幕的位置。
        */
        SDL_Rect & getRenderPosition(void) { return this->renderPosition; }

        /**
         * @brief 销毁该纹理，并重置所有数据成员。 
        */
        virtual ~TextureBisic();
};

/**
 * @brief 图片纹理类，用于加载渲染图片纹理。
*/
class TextureImage : public TextureBisic
{
    private:
        std::string textureName;    // 纹理编号
        SDL_Rect    clipPosition;   // 纹理的裁剪范围信息

        /**
         * @brief 设置纹理名
        */
        void setTextureName(std::string __path);

    public:
        TextureImage(void) : TextureBisic(), textureName(), clipPosition({0}) {}

        /**
         * @brief 获取纹理的裁剪位置和裁剪长宽的副本。
        */
        SDL_Rect getClipPosition(void) const { return this->clipPosition; }

        /**
         * @brief 获取纹理的裁剪位置和裁剪长宽的引用。
        */
        SDL_Rect & getClipPosition(void) { return this->clipPosition; }

        /**
         * @brief 设置纹理的颜色值（RGB）
        */
        void setColor(SDL_Color __color) {
            SDL_SetTextureColorMod(this->getTexture(), __color.r, __color.g, __color.b);
        }

        /**
         * @brief 设置纹理的混合模式。
        */
        void setBlendMode(SDL_BlendMode __blending) {
            SDL_SetTextureBlendMode(this->getTexture(), __blending);
        }

        /**
         * @brief 设置纹理的 Alpha 通道值（透明度 Tansparency）。
        */
        void setAlpha(Uint8 __alpha) { 
            SDL_SetTextureAlphaMod(this->getTexture(), __alpha);
        }

        /**
         * @brief 加载一张图片的纹理信息。
         * 
         * @param __path         图片路径
         * @param __render       渲染器
         * 
         * @return 是否加载成功
        */
        bool load(std::string __path, SDL_Renderer * __render);

        /**
         * @brief 指定纹理相对于屏幕的渲染平面坐标（x, y）和 裁剪范围，并交由渲染器渲染。
         * 
         * @param __clips       纹理裁剪的位置和长宽。
         * @param __render      渲染器
        */
        void render(int __x, int __y, SDL_Rect __clips, SDL_Renderer * __render);

        ~TextureImage();
};

/**
 * @brief 矩形纹理类，用于加载和渲染一个矩形纹理 
*/
class RectengleTexture : public TextureBisic
{
    public:
        /**
         * @brief 渲染标志位，表明只渲染矩形边框还是整个矩形。 
        */
        enum RenderFlag {
            BORDER = 0, WHOLE
        };
    private:
        std::string rectengleName;  // 矩形纹理编号

    public:
        /**
         * @brief 加载一个指定长宽的矩形 __name。
         * 
         * @param __name    矩形纹理编号
         * @param __w       矩形的宽
         * @param __h       矩形的高
         * @param __render  渲染器
        */
        bool load(std::string __name, int __w, int __h, SDL_Renderer * __render);

        /**
         * @brief 把加载好的颜色（使用 RGBA）的矩形纹理渲染在屏幕的 (__x, __y) 处。
         * 
         * @param __color       矩形的颜色值
         * @param __render      渲染器
         * @param __renderFlag  渲染标志位
        */
        void render(int __x, int __y, SDL_Color __color, 
                    SDL_Renderer * __render, RenderFlag __renderFlag
        );

        ~RectengleTexture();
};

/**
 * @brief 圆形纹理类，这个类的实现会比较特殊，
 *        不需要依赖 SDL_Texture，而是通过渲染像素点的方式直接绘制出整个圆。
*/
class CircleTexture
{
    public:
        struct CircleInfo {
            int centerX, centerY;   // 圆心坐标
            int radius;             // 圆半径
        };

        /**
         * @brief 判断点 (__x, __y) 是否在这个圆（包括圆上）之内。
         *        方法很简单，求该点与圆形的欧几里得距离 d，
         *        如果大于圆半径就意味着不在圆上。
        */
        static bool isInCircle(int __x, int __y, CircleInfo & __circleInfo);

        /**
         * @brief 显示圆的相关数据（CircleInfo）。
        */
        void showCircleInfo(void);

    private:
        std::string circleName;     // 圆的编号
        CircleInfo  circleInfo;     // 圆的属性
    
    public:
        /**
         * @brief 设置该圆的编号和圆的属性。
         * 
         * @param __name        该圆纹理的编号
         * @param __circleInfo  该圆纹理的属性
        */
        void load(const std::string __name, CircleInfo __circleInfo);

        /**
         * @brief 在圆上寻找 __segments 个点并渲染（__segments 的值越大，圆轮廓越平滑）。
         * 
         * @brief - 方法很简单，使用圆的标准方程就能求出圆上任意一点。
         * 
         * @param __segments 需要在圆上寻找点的数量。
         * @param __color    点的颜色（最后就是圆轮廓的颜色）。
         * @param __render   完成初始化的 SDL 渲染器。
        */
        void render(int __segments, SDL_Color __color, SDL_Renderer * __render);

        /**
         * @brief 将圆用 __color 颜色填充。
         * 
         * @param __color    圆的填充颜色。
         * @param __render   完成初始化的 SDL 渲染器。
        */
        void fill(SDL_Color __color, SDL_Renderer * __render);

        ~CircleTexture();
};