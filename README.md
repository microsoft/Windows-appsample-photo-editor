---
topic: sample
languages:
- cppwinrt
products:
- windows
- uwp
statusNotificationTargets:
- codefirst
---

<!---
  category: ControlsLayoutAndText FilesFoldersAndLibraries
-->

# Photo Editor C++/WinRT sample application

The Photo Editor application is a Universal Windows Platform (UWP) sample that showcases development with the [C++/WinRT](https://docs.microsoft.com/windows/uwp/cpp-and-winrt-apis/intro-to-using-cpp-with-winrt) language projection. The sample application allows you to retrieve photos from the **Pictures** library, and then edit the selected image with assorted photo effects. In the sample's source code, you'll see a number of common practices&mdash;such as [data binding](https://docs.microsoft.com/windows/uwp/cpp-and-winrt-apis/binding-property), and [asynchronous actions and operations](https://docs.microsoft.com/windows/uwp/cpp-and-winrt-apis/concurrency)&mdash;performed using the C++/WinRT projection. 

![PhotoEditor sample showing the image collection page, editing page, and editing controls](Screenshots/PhotoEditorBanner.png)


## Features

Photo Editor primarily demonstrates:
    
- Use of Standard C++17 syntax and libraries with Windows Runtime (WinRT) APIs.
- Use of coroutines, including the use of co_await, co_return, [**IAsyncAction**](https://docs.microsoft.com/uwp/api/windows.foundation.iasyncaction), and [**IAsyncOperation&lt;TResult&gt;**](https://docs.microsoft.com/uwp/api/windows.foundation.iasyncoperation_tresult_).
- Creation and use of custom Windows Runtime class (runtime class) projected types and implementation types. For more info about these terms, see [Consume APIs with C++/WinRT](https://docs.microsoft.com/windows/uwp/cpp-and-winrt-apis/consume-apis) and [Author APIs with C++/WinRT](https://docs.microsoft.com/windows/uwp/cpp-and-winrt-apis/author-apis).
- [Event handling](https://docs.microsoft.com/windows/uwp/cpp-and-winrt-apis/handle-events), including the use of auto-revoking event tokens.
- Use of the external Win2D NuGet package, and [Windows::UI::Composition](https://docs.microsoft.com/uwp/api/windows.ui.composition), for image effects.
- XAML data binding, including the [{x:Bind} markup extension](https://docs.microsoft.com/windows/uwp/xaml-platform/x-bind-markup-extension).
- XAML styling and UI customization, including [connected animations](https://docs.microsoft.com/windows/uwp/design/motion/connected-animation).

## Prerequisites

This sample requires:
- [Visual Studio 2017 and the latest version of the Windows 10 SDK (10.0.17134.0)](http://go.microsoft.com/fwlink/?LinkID=280676). You can use the free Visual Studio Community Edition to build and run Windows Universal Platform (UWP) apps. 
- Download and install the [C++/WinRT Visual Studio Extension](https://aka.ms/cppwinrt/vsix).
- You'll need to have a collection of images available in your local **Pictures** folder. If there are no images or photos available at that location, the app will not show anything but the background when you run it.  

To get the latest updates to Windows and the development tools, and to help shape their development, join the [Windows Insider Program](https://insider.windows.com).

## Running the sample

The default project is PhotoEditor and you can Start Debugging (F5) or Start Without Debugging (Ctrl+F5) to try it out, just make sure to set the platform target appropriately. 
The app will run in the emulator or on physical devices. 

## Code at a glance

If you're just interested in code snippets for certain areas, and don't want to browse or run the full sample, 
check out the following files.

A custom runtime class that represents a photo.
* [Photo.idl](PhotoEditor/Photo.idl)
  * [Photo.h](PhotoEditor/Photo.h)
  * [Photo.cpp](PhotoEditor/Photo.cpp)

The first page that loads the photos from the Pictures Library and displays a tiled thumbnail view. 
* [MainPage.xaml](PhotoEditor/MainPage.xaml)
  * [MainPage.idl](PhotoEditor/MainPage.idl)
  * [MainPage.h](PhotoEditor/MainPage.h)
  * [MainPage.cpp](PhotoEditor/MainPage.cpp)

Tapping a photo from the MainPage thumbnail view will take you to the photo editor page, where Win2D effects are toggled, set, and chained together.
* [DetailPage.xaml](PhotoEditor/DetailPage.xaml)
  * [DetailPage.idl](PhotoEditor/DetailPage.idl)
  * [DetailPage.h](PhotoEditor/DetailPage.h)
  * [DetailPage.cpp](PhotoEditor/DetailPage.cpp)

## Related C++/WinRT documentation and blogs.
* [C++/WinRT UWP conceptual documentation](https://docs.microsoft.com/windows/uwp/cpp-and-winrt-apis/)
* [C++/WinRT base.h reference documentation](https://docs.microsoft.com/uwp/cpp-ref-for-winrt/winrt)
* [ModernCpp blog](https://moderncpp.com/)

## Related XAML documentation and code samples.
* [Controls and patterns for UWP apps](https://docs.microsoft.com/windows/uwp/controls-and-patterns/index)
* [Layout for UWP apps](https://docs.microsoft.com/windows/uwp/layout/)
* [Data binding in depth](https://docs.microsoft.com/windows/uwp/data-binding/data-binding-in-depth)
* [UWP style guide](https://docs.microsoft.com/windows/uwp/style/)
* [Visual layer](https://docs.microsoft.com/windows/uwp/composition/visual-layer)
* [ListView and GridView data virtualization](https://docs.microsoft.com/windows/uwp/debug-test-perf/listview-and-gridview-data-optimization)
* [Data virtualization sample](https://github.com/Microsoft/Windows-universal-samples/tree/master/Samples/XamlDataVirtualization)

## External libraries used in this sample

* [Win2D](https://github.com/Microsoft/Win2D)
