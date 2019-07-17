#ifndef PICLUASOL_PICLUASOL_HPP
#define PICLUASOL_PICLUASOL_HPP

#include <Pic/Image.hpp>
#include <Stick/Path.hpp>
#include <StickLuaSol/StickLuaSol.hpp>
#include <sol/sol.hpp>

namespace picLua
{
STICK_API void registerPic(sol::state_view _lua, const stick::String & _namespace = "");
STICK_API void registerPic(sol::state_view _lua, sol::table _tbl);

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

    _tbl.new_usertype<BasePixel>(
        stick::String::concat("__", _name, "BasePixel").cString(),
        "new",
        sol::no_constructor,
        sol::meta_function::equal_to,
        &BasePixel::operator==,
        "setChannelValue",
        &BasePixel::setChannelValue,
        "channel",
        sol::resolve<const ValueType & (BasePixel::*)(stick::UInt32)const>(&BasePixel::channel)
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

        _tbl.new_usertype<T>(stick::String::concat("Pixel", _name).cString(),
                             sol::base_classes,
                             sol::bases<typename T::BasePixel>(),
                             "new",
                             sol::constructors<T(ValueType)>(),
                             "setValue",
                             sol::resolve<void (T::*)(ValueType)>(&T::setValue));
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

        _tbl.new_usertype<T>(
            stick::String::concat("Pixel", _name).cString(),
            sol::base_classes,
            sol::bases<typename T::BasePixel>(),
            "new",
            sol::constructors<T(ValueType), T(ValueType, ValueType)>(),
            "setValue",
            sol::overload(sol::resolve<void (T::*)(ValueType)>(&T::setValue),
                          sol::resolve<void (T::*)(ValueType, ValueType)>(&T::setValue)));
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

        _tbl.new_usertype<T>(
            stick::String::concat("Pixel", _name).cString(),
            sol::base_classes,
            sol::bases<typename T::BasePixel>(),
            "new",
            sol::constructors<T(ValueType),
                              T(ValueType, ValueType),
                              T(ValueType, ValueType, ValueType)>(),
            "setValue",
            sol::overload(
                sol::resolve<void (T::*)(ValueType)>(&T::setValue),
                sol::resolve<void (T::*)(ValueType, ValueType)>(&T::setValue),
                sol::resolve<void (T::*)(ValueType, ValueType, ValueType)>(&T::setValue)));
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

        _tbl.new_usertype<T>(
            stick::String::concat("Pixel", _name).cString(),
            sol::base_classes,
            sol::bases<typename T::BasePixel>(),
            "new",
            sol::constructors<T(ValueType),
                              T(ValueType, ValueType),
                              T(ValueType, ValueType, ValueType),
                              T(ValueType, ValueType, ValueType)>(),
            "setValue",
            sol::overload(sol::resolve<void (T::*)(ValueType)>(&T::setValue),
                          sol::resolve<void (T::*)(ValueType, ValueType)>(&T::setValue),
                          sol::resolve<void (T::*)(ValueType, ValueType, ValueType)>(&T::setValue),
                          sol::resolve<void (T::*)(ValueType, ValueType, ValueType, ValueType)>(
                              &T::setValue)));
    }
};

template <class ImageT>
inline void registerImageType(const stick::String & _name, sol::table _namespace)
{
    using PixelType = typename ImageT::Pixel;
    using ValueType = typename PixelType::ValueType;

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

    _tbl.new_usertype<ImageT>(
        stick::String::concat("Image", _name).cString(),
        sol::base_classes,
        sol::bases<pic::Image>(),
        "new",
        sol::constructors<ImageT(),
                          ImageT(stick::Size, stick::Size),
                          ImageT(stick::Size, stick::Size, stick::Size)>(),
        "pixel",
        sol::overload(sol::resolve<PixelType & (ImageT::*)(stick::Size, stick::Size)>(&T::pixel),
                      sol::resolve<PixelType & (ImageT::*)(stick::Size, stick::Size, stick::Size)>(
                          &T::pixel)));
}
} // namespace detail

STICK_API void registerPic(sol::state_view _lua, const stick::String & _namespace)
{
    registerPic(_lua, stickLuaSol::ensureNamespaceTable(_lua, _lua.globals(), _namespace));
}

STICK_API void registerPic(sol::state_view _lua, sol::table _tbl)
{
    using namespace luanatic;
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
        Image,
        sol::base_classes,
        sol::bases<pic::Image>(),
        "new",
        sol::no_constructor,
        "resize",
        sol::overload(sol::resolve<void (Image::*)(Size, Size)>(&Image::resize),
                      sol::resolve<void (Image::*)(Size, Size, Size)>(&Image::resize)),
        "width",
        &Image::width,
        "height",
        &Image::height,
        "deph",
        &Image::deph,
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
        "saveFile",
        &Image::saveFile);

    // register free functions
    // namespaceTable.addWrapper<Image, ImageUniquePtr>();
    namespaceTable.set_function("loadImage", pic::loadImage);

    // register concrete image types (we only register 32 bit and floating point image types for
    // now...)
    detail::registerImageType<pic::ImageGray8>("Gray8", namespaceTable);
    detail::registerImageType<pic::ImageGrayAlpha8>("GrayAlpha8", namespaceTable);
    detail::registerImageType<pic::ImageAlphaGray8>("AlphaGray8", namespaceTable);
    detail::registerImageType<pic::ImageRGB8>("RGB8", namespaceTable);
    detail::registerImageType<pic::ImageBGR8>("BGR8", namespaceTable);
    detail::registerImageType<pic::ImageRGBA8>("RGBA8", namespaceTable);
    detail::registerImageType<pic::ImageBGRA8>("BGRA8", namespaceTable);
    detail::registerImageType<pic::ImageARGB8>("ARGB8", namespaceTable);
    detail::registerImageType<pic::ImageABGR8>("ABGR8", namespaceTable);

    detail::registerImageType<pic::ImageGray32f>("Gray32f", namespaceTable);
    detail::registerImageType<pic::ImageGrayAlpha32f>("GrayAlpha32f", namespaceTable);
    detail::registerImageType<pic::ImageAlphaGray32f>("AlphaGray32f", namespaceTable);
    detail::registerImageType<pic::ImageRGB32f>("RGB32f", namespaceTable);
    detail::registerImageType<pic::ImageBGR32f>("BGR32f", namespaceTable);
    detail::registerImageType<pic::ImageRGBA32f>("RGBA32f", namespaceTable);
    detail::registerImageType<pic::ImageBGRA32f>("BGRA32f", namespaceTable);
    detail::registerImageType<pic::ImageARGB32f>("ARGB32f", namespaceTable);
    detail::registerImageType<pic::ImageABGR32f>("ABGR32f", namespaceTable);
}
} // namespace picLua

#endif // PICLUASOL_PICLUASOL_HPP
