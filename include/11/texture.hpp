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
         * @brief 获取该纹理的原始长宽，以及该纹理渲染在屏幕的位置。
        */
        SDL_Rect getRenderPosition(void) const { return this->renderPosition; }
        SDL_Rect & getRenderPosition(void) { return this->renderPosition; }

        /**
         * @brief 销毁该纹理，并重置所有数据成员。 
        */
        virtual ~TextureBisic();
};

class TextureImage : public TextureBisic
{
    private:
        std::string textureName;
        SDL_Rect    clipPosition;

        /**
         * @brief 设置纹理名
        */
        void setTextureName(std::string __path);

    public:
        TextureImage(void) : TextureBisic(), textureName(), clipPosition({0}) {}

        /**
         * @brief 获取纹理的裁剪位置和裁剪长宽。
        */
        SDL_Rect getClipPosition(void) const { return this->clipPosition; }
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
        */
        void render(int __x, int __y, SDL_Rect __clips, SDL_Renderer * __render);
};