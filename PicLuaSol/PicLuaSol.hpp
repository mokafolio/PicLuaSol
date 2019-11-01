#ifndef PICLUASOL_PICLUASOL_HPP
#define PICLUASOL_PICLUASOL_HPP

#include <Pic/Image.hpp>
#include <Stick/Path.hpp>
#include <StickLuaSol/StickLuaSol.hpp>
#include <sol/sol.hpp>

namespace picLuaSol
{
STICK_API void registerPic(sol::state_view _lua, const stick::String & _namespace = "");
STICK_API void registerPic(sol::state_view _lua, sol::table _tbl);

template <typename TO, typename FROM, class C>
stick::UniquePtr<TO, C> staticUniquePtrCast(stick::UniquePtr<FROM, C> && old)
{
    return stick::UniquePtr<TO, C>{ static_cast<TO *>(old.release()) };
}

namespace detail
{
// inline sol::object luaLoadImage(const char * _uri)
// {
//     // auto * luanaticState = luanatic::detail::luanaticState(_state);
//     // STICK_ASSERT(luanaticState);
//     // return luanatic::pushValueType(_state,
//     // pic::loadImage(luanatic::convertToValueTypeAndCheck<stick::URI>(_state, 1),
//     //                                        *luanaticState->m_allocator));

//     auto img = pic::loadImage(_uri);
// }

template <class PixelT>
inline void registerBasePixel(const stick::String & _name, sol::table _namespace)
{
    using BasePixel = typename PixelT::BasePixel;
    using ValueType = typename BasePixel::ValueType;

    stick::String name = stick::String::concat("__", _name, "BasePixel");
    _namespace.new_usertype<BasePixel>(
        name.cString(),
        "new",
        sol::no_constructor,
        sol::meta_function::equal_to,
        &BasePixel::operator==,
        "setChannelValue",
        &BasePixel::setChannelValue,
        "channel",
        sol::resolve<const ValueType & (stick::UInt32)const>(&BasePixel::channel)
    );
}

template <class T, class Enable = void>
struct PixelRegistrator
{
    static void registerPixel(const stick::String & _name, sol::table _namespace);
};

template <class T>
struct PixelRegistrator<T, typename std::enable_if<T::ChannelLayout::channelCount() == 1>::type>
{
    static void registerPixel(const stick::String & _name, sol::table _namespace)
    {
        using ValueType = typename T::ValueType;

        registerBasePixel<T>(_name, _namespace);

        // luanatic::ClassWrapper<T> pixelCW(stick::String::concat("Pixel", _name));

        // pixelCW.template addConstructor<ValueType>()
        //     .template addBase<typename T::BasePixel>()
        //     .addMemberFunction("setValue",
        //                        LUANATIC_FUNCTION_OVERLOAD(void (T::*)(ValueType), &T::setValue));

        // _namespace.registerClass(pixelCW);

        stick::String name = stick::String::concat("Pixel", _name);
        _namespace.new_usertype<T>(name.cString(),
                                   sol::base_classes,
                                   sol::bases<typename T::BasePixel>(),
                                   sol::call_constructor,
                                   sol::constructors<T(ValueType)>(),
                                   "setValue",
                                   sol::resolve<void(ValueType)>(&T::setValue));
    }
};

template <class T>
struct PixelRegistrator<T, typename std::enable_if<T::ChannelLayout::channelCount() == 2>::type>
{
    static void registerPixel(const stick::String & _name, sol::table _namespace)
    {
        using ValueType = typename T::ValueType;

        registerBasePixel<T>(_name, _namespace);

        // luanatic::ClassWrapper<T> pixelCW(stick::String::concat("Pixel", _name));
        // pixelCW.template addBase<typename T::BasePixel>()
        //     .template addConstructor<ValueType>()
        //     .template addConstructor<ValueType, ValueType>()
        //     .addMemberFunction("setValue",
        //                        LUANATIC_FUNCTION_OVERLOAD(void (T::*)(ValueType), &T::setValue))
        //     .addMemberFunction(
        //         "setValue",
        //         LUANATIC_FUNCTION_OVERLOAD(void (T::*)(ValueType, ValueType), &T::setValue));

        // _namespace.registerClass(pixelCW);

        stick::String name = stick::String::concat("Pixel", _name);
        _namespace.new_usertype<T>(
            name.cString(),
            sol::base_classes,
            sol::bases<typename T::BasePixel>(),
            sol::call_constructor,
            sol::constructors<T(ValueType), T(ValueType, ValueType)>(),
            "setValue",
            sol::overload(sol::resolve<void(ValueType)>(&T::setValue),
                          sol::resolve<void(ValueType, ValueType)>(&T::setValue)));
    }
};

template <class T>
struct PixelRegistrator<T, typename std::enable_if<T::ChannelLayout::channelCount() == 3>::type>
{
    static void registerPixel(const stick::String & _name, sol::table _namespace)
    {
        using ValueType = typename T::ValueType;

        registerBasePixel<T>(_name, _namespace);

        // luanatic::ClassWrapper<T> pixelCW(stick::String::concat("Pixel", _name));
        // pixelCW.template addBase<typename T::BasePixel>()
        //     .template addConstructor<ValueType>()
        //     .template addConstructor<ValueType, ValueType>()
        //     .template addConstructor<ValueType, ValueType, ValueType>()
        //     .addMemberFunction("setValue",
        //                        LUANATIC_FUNCTION_OVERLOAD(void (T::*)(ValueType), &T::setValue))
        //     .addMemberFunction(
        //         "setValue",
        //         LUANATIC_FUNCTION_OVERLOAD(void (T::*)(ValueType, ValueType), &T::setValue))
        //     .addMemberFunction("setValue",
        //                        LUANATIC_FUNCTION_OVERLOAD(
        //                            void (T::*)(ValueType, ValueType, ValueType), &T::setValue));

        // _namespace.registerClass(pixelCW);

        stick::String name = stick::String::concat("Pixel", _name);
        _namespace.new_usertype<T>(
            name.cString(),
            sol::base_classes,
            sol::bases<typename T::BasePixel>(),
            sol::call_constructor,
            sol::constructors<T(ValueType),
                              T(ValueType, ValueType),
                              T(ValueType, ValueType, ValueType)>(),
            "setValue",
            sol::overload(sol::resolve<void(ValueType)>(&T::setValue),
                          sol::resolve<void(ValueType, ValueType)>(&T::setValue),
                          sol::resolve<void(ValueType, ValueType, ValueType)>(&T::setValue)));
    }
};

template <class T>
struct PixelRegistrator<T, typename std::enable_if<T::ChannelLayout::channelCount() == 4>::type>
{
    static void registerPixel(const stick::String & _name, sol::table _namespace)
    {
        using ValueType = typename T::ValueType;

        registerBasePixel<T>(_name, _namespace);

        // luanatic::ClassWrapper<T> pixelCW(stick::String::concat("Pixel", _name));
        // pixelCW.template addBase<typename T::BasePixel>()
        //     .template addConstructor<ValueType>()
        //     .template addConstructor<ValueType, ValueType>()
        //     .template addConstructor<ValueType, ValueType, ValueType>()
        //     .template addConstructor<ValueType, ValueType, ValueType, ValueType>()
        //     .addMemberFunction("setValue",
        //                        LUANATIC_FUNCTION_OVERLOAD(void (T::*)(ValueType), &T::setValue))
        //     .addMemberFunction(
        //         "setValue",
        //         LUANATIC_FUNCTION_OVERLOAD(void (T::*)(ValueType, ValueType), &T::setValue))
        //     .addMemberFunction("setValue",
        //                        LUANATIC_FUNCTION_OVERLOAD(
        //                            void (T::*)(ValueType, ValueType, ValueType), &T::setValue))
        //     .addMemberFunction(
        //         "setValue",
        //         LUANATIC_FUNCTION_OVERLOAD(void (T::*)(ValueType, ValueType, ValueType,
        //         ValueType),
        //                                    &T::setValue));

        // _namespace.registerClass(pixelCW);

        stick::String name = stick::String::concat("Pixel", _name);
        _namespace.new_usertype<T>(
            name.cString(),
            sol::base_classes,
            sol::bases<typename T::BasePixel>(),
            sol::call_constructor,
            sol::constructors<T(ValueType),
                              T(ValueType, ValueType),
                              T(ValueType, ValueType, ValueType),
                              T(ValueType, ValueType, ValueType)>(),
            "setValue",
            sol::overload(
                sol::resolve<void(ValueType)>(&T::setValue),
                sol::resolve<void(ValueType, ValueType)>(&T::setValue),
                sol::resolve<void(ValueType, ValueType, ValueType)>(&T::setValue),
                sol::resolve<void(ValueType, ValueType, ValueType, ValueType)>(&T::setValue)));
    }
};

template <class ImageT>
inline void registerImageType(const stick::String & _name, sol::table _namespace)
{
    using PixelType = typename ImageT::Pixel;
    // using ValueType = typename PixelType::ValueType;

    // register the pixel type
    PixelRegistrator<PixelType>::registerPixel(_name, _namespace);

    // register the image type
    // luanatic::ClassWrapper<ImageT> imageCW(stick::String::concat("Image", _name));

    // imageCW.template addBase<pic::Image>()
    //     .template addConstructor<>()
    //     .template addConstructor<stick::Size, stick::Size>()
    //     .template addConstructor<stick::Size, stick::Size, stick::Size>()
    //     .addMemberFunction("pixel",
    //                        LUANATIC_FUNCTION_OVERLOAD(
    //                            PixelType & (ImageT::*)(stick::Size, stick::Size),
    //                            &ImageT::pixel))
    //     .addMemberFunction(
    //         "pixel",
    //         LUANATIC_FUNCTION_OVERLOAD(
    //             PixelType & (ImageT::*)(stick::Size, stick::Size, stick::Size), &ImageT::pixel));

    // _namespace.registerClass(imageCW);

    stick::String name = stick::String::concat("Image", _name);
    _namespace.new_usertype<ImageT>(
        name.cString(),
        sol::base_classes,
        sol::bases<pic::Image>(),
        sol::call_constructor,
        sol::constructors<ImageT(),
                          ImageT(stick::Size, stick::Size),
                          ImageT(stick::Size, stick::Size, stick::Size)>(),
        "pixel",
        sol::overload(
            sol::resolve<PixelType &(stick::Size, stick::Size)>(&ImageT::pixel),
            sol::resolve<PixelType &(stick::Size, stick::Size, stick::Size)>(&ImageT::pixel)));
}
} // namespace detail

#ifdef PICLUASOL_IMPLEMENTATION
STICK_API void registerPic(sol::state_view _lua, const stick::String & _namespace)
{
    registerPic(_lua, stickLuaSol::ensureNamespaceTable(_lua, _lua.globals(), _namespace));
}

STICK_API void registerPic(sol::state_view _lua, sol::table _tbl)
{
    using namespace pic;
    using namespace stick;

    // register image base
    // ClassWrapper<Image> imageCW("Image");
    // imageCW
    //     .addMemberFunction("resize",
    //                        LUANATIC_FUNCTION_OVERLOAD(void (Image::*)(Size, Size),
    //                        &Image::resize))
    //     .addMemberFunction(
    //         "resize",
    //         LUANATIC_FUNCTION_OVERLOAD(void (Image::*)(Size, Size, Size, Size), &Image::resize))
    //     .addMemberFunction("width", LUANATIC_FUNCTION(&Image::width))
    //     .addMemberFunction("height", LUANATIC_FUNCTION(&Image::height))
    //     .addMemberFunction("depth", LUANATIC_FUNCTION(&Image::depth))
    //     .addMemberFunction("channelCount", LUANATIC_FUNCTION(&Image::channelCount))
    //     .addMemberFunction("pixelCount", LUANATIC_FUNCTION(&Image::pixelCount))
    //     .addMemberFunction("pixelTypeID", LUANATIC_FUNCTION(&Image::pixelTypeID))
    //     .addMemberFunction("channelLayoutTypeID", LUANATIC_FUNCTION(&Image::channelLayoutTypeID))
    //     .addMemberFunction("valueTypeID", LUANATIC_FUNCTION(&Image::valueTypeID))
    //     .addMemberFunction("byteCount", LUANATIC_FUNCTION(&Image::byteCount))
    //     .addMemberFunction("bytesPerRow", LUANATIC_FUNCTION(&Image::bytesPerRow))
    //     .addMemberFunction("bitsPerChannel", LUANATIC_FUNCTION(&Image::bitsPerChannel))
    //     .addMemberFunction("bitsPerPixel", LUANATIC_FUNCTION(&Image::bitsPerPixel))
    //     .addMemberFunction("hasAlpha", LUANATIC_FUNCTION(&Image::hasAlpha))
    //     .addMemberFunction("isFloatingPoint", LUANATIC_FUNCTION(&Image::isFloatingPoint))
    //     .addMemberFunction("alphaPosition", LUANATIC_FUNCTION(&Image::alphaPosition))
    //     .addMemberFunction("flipRows", LUANATIC_FUNCTION(&Image::flipRows))
    //     .addMemberFunction("flipColumns", LUANATIC_FUNCTION(&Image::flipColumns))
    //     .addMemberFunction("flipLayers", LUANATIC_FUNCTION(&Image::flipLayers))
    //     .addMemberFunction("flip", LUANATIC_FUNCTION(&Image::flip))
    //     .addMemberFunction("swapChannels", LUANATIC_FUNCTION(&Image::swapChannels))
    //     .addMemberFunction("saveFile", LUANATIC_FUNCTION(&Image::saveFile));
    // namespaceTable.registerClass(imageCW);
    _tbl.new_usertype<Image>(
        "Image",
        "new",
        sol::no_constructor,
        "resize",
        sol::overload(sol::resolve<void(Size, Size)>(&Image::resize),
                      sol::resolve<void(Size, Size, Size, Size)>(&Image::resize),
                      [](Image * _self, Size _w, Size _h, Size _d) { _self->resize(_w, _h, _d); }),
        "width",
        &Image::width,
        "height",
        &Image::height,
        "depth",
        &Image::depth,
        "channelCount",
        &Image::channelCount,
        "pixelCount",
        &Image::pixelCount,
        "bytesPerRow",
        &Image::bytesPerRow,
        "bitsPerChannel",
        &Image::bitsPerChannel,
        "bitsPerPixel",
        &Image::bitsPerPixel,
        "hasAlpha",
        &Image::hasAlpha,
        "isFloatingPoint",
        &Image::isFloatingPoint,
        "alphaPosition",
        &Image::alphaPosition,
        "flipRows",
        &Image::flipRows,
        "flipColumns",
        &Image::flipColumns,
        "flipLayers",
        &Image::flipLayers,
        "flip",
        &Image::flip,
        "swapChannels",
        &Image::swapChannels,
        "save",
        &Image::save);

    // register free functions
    // namespaceTable.addWrapper<Image, ImageUniquePtr>();
    // _tbl.set_function("loadImage", [](const char * _uri, sol::this_state _s) {
    //     sol::state_view lua(_s);
    //     auto res = pic::loadImage(_uri);
    //     if (res)
    //     {
    //         auto imgptr = std::move(res.get());
    //         if (imgptr->pixelTypeID() == PixelRGB8::TypeInfo::typeID())
    //         {
    //             printf("pushing rgb8 %lu %lu\n", imgptr->width(), imgptr->height());
    //             return sol::make_object(lua, static_cast<ImageRGB8 *>(imgptr.get()));
    //         }
    //         else if (imgptr->pixelTypeID() == PixelRGBA8::TypeInfo::typeID())
    //         {
    //             printf("pushing rgba8 %lu %lu\n", imgptr->width(), imgptr->height());
    //             // return sol::make_object(lua, static_cast<ImageRGBA8 *>(imgptr.get()));
    //             return sol::make_object(lua, std::move(staticUniquePtrCast<ImageRGBA8>(std::move(imgptr))));
    //         }
    //         else
    //         {
    //             printf("psuhing error\n");
    //             return sol::make_object(lua, res.error());
    //         }
    //     }
    //     else
    //         return sol::make_object(lua, res.error());
    // });
    
    _tbl.set_function("loadImage", [](const char * _path, sol::this_state _s)
    {
        sol::state_view lua(_s);
        auto res = pic::loadImage(_path);
        if(res)
            return sol::make_object(lua, std::move(res.get()));
        return sol::make_object(lua, res.error());
    });
    
    _tbl.set_function("upCastImage", [](pic::ImageUniquePtr & _img, sol::this_state _s) {
        sol::state_view lua(_s);
        //@TODO: complete this
        if (_img->pixelTypeID() == pic::PixelRGB8::pixelTypeID())
        {
            return sol::make_object(lua, std::move(staticUniquePtrCast<ImageRGB8>(std::move(_img))));
        }
        else if (_img->pixelTypeID() == pic::PixelRGBA8::pixelTypeID())
        {
            return sol::make_object(lua, std::move(staticUniquePtrCast<ImageRGBA8>(std::move(_img))));
        }
        return sol::make_object(lua, std::move(_img));
    });

    // register concrete image types (we only register 32 bit and floating point image types for
    // now...)
    // detail::registerImageType<pic::ImageGray8>("Gray8", _tbl);
    // detail::registerImageType<pic::ImageGrayAlpha8>("GrayAlpha8", _tbl);
    // detail::registerImageType<pic::ImageAlphaGray8>("AlphaGray8", _tbl);
    detail::registerImageType<pic::ImageRGB8>("RGB8", _tbl);
    // detail::registerImageType<pic::ImageBGR8>("BGR8", _tbl);
    detail::registerImageType<pic::ImageRGBA8>("RGBA8", _tbl);
    // detail::registerImageType<pic::ImageBGRA8>("BGRA8", _tbl);
    // detail::registerImageType<pic::ImageARGB8>("ARGB8", _tbl);
    // detail::registerImageType<pic::ImageABGR8>("ABGR8", _tbl);

    // detail::registerImageType<pic::ImageGray32f>("Gray32f", _tbl);
    // detail::registerImageType<pic::ImageGrayAlpha32f>("GrayAlpha32f", _tbl);
    // detail::registerImageType<pic::ImageAlphaGray32f>("AlphaGray32f", _tbl);
    // detail::registerImageType<pic::ImageRGB32f>("RGB32f", _tbl);
    // detail::registerImageType<pic::ImageBGR32f>("BGR32f", _tbl);
    // detail::registerImageType<pic::ImageRGBA32f>("RGBA32f", _tbl);
    // detail::registerImageType<pic::ImageBGRA32f>("BGRA32f", _tbl);
    // detail::registerImageType<pic::ImageARGB32f>("ARGB32f", _tbl);
    // detail::registerImageType<pic::ImageABGR32f>("ABGR32f", _tbl);
}
#endif // PICLUASOL_IMPLEMENTATION

} // namespace picLuaSol

#endif // PICLUASOL_PICLUASOL_HPP
