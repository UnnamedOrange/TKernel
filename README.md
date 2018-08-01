# TKernel

## TKernel 是什么

*TKernel* 是一套用于加速 Windows 平台下的 C++ 开发的头文件。

## 如何使用 TKernel

```c++
#include "TKernel.h" // However, you need to input the correct directory of it.
```

## 关于

Powered by: Orange Software

Version: 3

Build: Alpha

## 更新信息

### Alpha Build

No information.

## 指令

更多指令可以通过 Visual Studio 的 IntelliSense 了解。

### `TKernel.h`

包含其它关键文件。

### `TApplication.h`

`TApplication.h` 是一个用于管理你的应用程序的头文件。它包含了一个抽象类：

```c++
class TApplication abstract;
```

你必须继承它。一般你可以只使用默认构造函数，但如果你需要调用某些特定的成员函数，你需要调用以下构造函数：

```c++
TApplication(LPCWSTR lpcAppName, LPCWSTR lpcGUID);
```

但是建议只使用以上构造函数。

#### TApplication 的实例数量是有限制的 

你只能在你的代码中声明一个 TApplication，否则将会抛出异常。

### `TFileInfo.h`

`TFileInfo.h` 是包含一个继承自 `VS_FIXEDFILEINFO` 的类的头文件. `TFileInfo` 类有两个构造函数。

```c++
TFileInfo();
explicit TFileInfo(LPCWSTR lpcFileName);
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

### `TBlock`

用于分配内存块。

#### 可以像使用 STL 容器一样使用它

#### 没有维护内存池

### `TWindow` 和 `TDialog`

用于创建窗口和对话框。

`TWindow` 和 `TDialog` 是抽象类，你必须继承它，并且提供它的纯虚函数的实现（可以通过 Visual Studio 的 IntelliSense 获取帮助）。

#### `Create`

在创建完对象后，调用 `Create` 函数创建窗口。**`Create` 函数将会自动注册窗口类**，如果有其它需求，需要在窗口创建后手动修改。（例如窗口图标需要调用其它函数修改）

为了方便，存在名为 `Createx` 的函数，可以提供更少的参数创建窗口。

对于一个实例，在窗口销毁前仅能调用一次 Create。

##### `TDialog::TDialogParam`

在调用 `TDialog` 的 `Create` 函数时，需要提供类型为 `TDialog::TDialogParam` 的参数。

#### `WndProc`

`WndProc` 是一个纯虚函数，用于实现窗口过程。你需要像使用一般的 WndProc（或 DialogProc） 一样使用它。

无法处理调用 `WM_CREATE`（或 `WM_INITDIALOG`）之前发送到窗口的消息，幸运的是这些信息也不大重要。可以保证的是 `WM_DESTROY` 是最后一条发送给这个函数的消息。

#### 注意事项

使用 TWindow 的窗口不能使用 `GWLP_USERDATA`。