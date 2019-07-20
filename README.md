[TOC]

# TKernel

## TKernel 是什么

*TKernel* 是一套用于加速 Windows 平台下的 C++ 开发的头文件。

## 如何使用 TKernel

```c++
#include "TKernel.hpp" // However, you need to input the correct directory of it.
```

## 关于

Powered by: Orange Software

Version: 3

Build: Alpha 4

## 更新信息

### Alpha Build

No information.

## 指令

更多指令可以通过 Visual Studio 的 IntelliSense 了解。

### `TKernel.hpp`

包含其它关键文件。

### `TApplication.hpp`

`TApplication.hpp` 是一个用于管理你的应用程序的头文件。它包含了一个抽象类：

```c++
class TApplication abstract;
```

你必须继承它。一般你可以只使用默认构造函数。还可以调用以下构造函数：

```c++
TApplication(LPCWSTR lpcAppName, LPCWSTR lpcGUID);
```

建议只使用该构造函数。使用默认构造函数将无法使用部分该类的方法。

#### TApplication 的实例数量是有限制的 

你只能在你的代码中定义一个 TApplication，否则将会抛出异常。

### `TFileInfo.hpp`

`TFileInfo.hpp` 是包含一个继承自 `VS_FIXEDFILEINFO` 的类的头文件. `TFileInfo` 类有两个构造函数。

```c++
TFileInfo() { GetFileInfo(); }
explicit TFileInfo(LPCWSTR lpcFileName) { GetFileInfo(lpcFileName); }
explicit TFileInfo(HINSTANCE hInstance) { GetFileInfo(hInstance); }
```

如果你调用第二个构造函数，将会自动获取指定文件名的文件版本信息。如果你调用第一个函数，将会获取你的应用程序对应的文件版本信息（在内部会委托第二个构造函数），如果你需要得到你应用程序对应的版本号，直接使用第一个构造函数。

#### `GerVerx`

作为参考，以下四个成员函数可以帮助你得到正确的产品版本。

```c++
DWORD GetVer1() const { return (DWORD)HIWORD(dwProductVersionMS); }
DWORD GetVer2() const { return (DWORD)LOWORD(dwProductVersionMS); }
DWORD GetVer3() const { return (DWORD)HIWORD(dwProductVersionLS); }
DWORD GetVer4() const { return (DWORD)LOWORD(dwProductVersionLS); }
```

### `TBlock.hpp`

用于分配内存块。

#### 可以像使用 STL 容器一样使用它

#### 没有维护内存池

### `TWindow.hpp`

> 在 Alpha 4 中，增加了 `CenterizeWindow` 方法。
>
> 在 Alpha 4 中，对于非对话框的窗口，第一个收到的消息改为 `WM_NCCREATE`。在 Alpha 3 及以前版本，第一个收到的消息为 `WM_CREATE`。

用于创建窗口和对话框。目前可以使用五个抽象类：`TWindowHost`、`TWindowPopup`、`TWindowChild`、`TDialogBox`、`TCreateDialog`。

所有这些类都是抽象类，你必须继承，并且提供其纯虚函数的实现（可以通过 Visual Studio 的 IntelliSense 获取帮助）。

#### 外部接口

调用 `Create()` 来创建窗口。

对于需要注册窗口类的窗口，无需手动注册，类的内部会自动创建。如果需要手动注册，调用 `RegisterClasses()`。如果有其它需求，需要在窗口创建后手动修改。（例如窗口图标需要调用其它函数修改）

对于一个实例，在窗口销毁前仅能调用一次 `Create`。

#### `WndProc`

`WndProc` 是一个纯虚函数，用于实现窗口过程。你需要像使用一般的 WndProc（或 DialogProc） 一样使用它。

无法处理调用 `WM_NCCREATE`（或 `WM_INITDIALOG`）之前发送到窗口的消息，幸运的是这些信息也不大重要。可以保证的是 `WM_DESTROY` 是最后一条发送给这个函数的消息。

#### 注意事项

使用 `TWindow.hpp` 中提供的类的窗口不能使用 `GWLP_USERDATA`。

更多信息，请借助 IntelliSense 参看源代码。

由于需求更新频繁，因此暂时不保证接口不会改变。

### `TDPI.hpp`

用于换算当前 DPI 下对应的尺寸大小。

```c++
template <typename T>
static T dpi(T in);
```

不能再创建实例，直接调用 `TDPI::dpi()` 即可。

### `TGdiplus.hpp`

> 在 Alpha 4 中，支持在多处定义。Alpha 3 及以前版本仅支持在一处定义。

用于自动加载与卸载 Gdiplus。

直接在任意处作如下定义：

```c++
TGdiplus _unused;
```

### `TPrivateFont.hpp`

用于从数据块中加入私有字体。

#### `TPrivateFont`

向构造函数提供对应的内存块进行创建，可以使用 `placement new`。

#### `TPrivateFontPlus`

用于 Gdiplus，可以获得相应的 `FontFamily`。初始化方法与 `TPrivateFont` 一致。

```c++
const Gdiplus::FontFamily& TPrivateFontPlus::operator()();
```

该类继承自 `TPrivateFont`，因此无需同时使用。

### `TTimer.hpp`

用于创建时钟。有两种方式指定回调函数：

```c++
TTimer(std::function<void(DWORD dwTime)> func) noexcept;
VOID operator=(std::function<void(DWORD dwTime)> func);
```

指定回调函数后，调用 `set` 方法，或 `kill` 方法。注意，需要保证时钟的实例始终存在。

### `TMessage.hpp`

用于自定义消息的自动编码。使用 `TMessage::Register(L"name")` 的形式获取对应的消息值。