//  ---------------------------------------------------------------------------------
//  Copyright (c) Microsoft Corporation.  All rights reserved.
// 
//  The MIT License (MIT)
// 
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
// 
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
// 
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE
//  ---------------------------------------------------------------------------------

#include "pch.h"
#include "MainPage.h"
#include "Photo.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::Search;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Composition;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Hosting;
using namespace Windows::UI::Xaml::Media::Animation;
using namespace Windows::UI::Xaml::Media::Imaging;

namespace winrt::PhotoEditor::implementation
{
    // Page constructor.
    MainPage::MainPage() : 
        m_photos(winrt::single_threaded_observable_vector<IInspectable>()),
        m_compositor(Window::Current().Compositor())
    {
        InitializeComponent();
        ParaView().Source(ForegroundElement());
    }

    // Loads collection of Photos from users Pictures library.
    IAsyncAction MainPage::OnNavigatedTo(NavigationEventArgs e)
    {
        // Load photos if they haven't previously been loaded.
        if (Photos().Size() == 0)
        {
            m_elementImplicitAnimation = m_compositor.CreateImplicitAnimationCollection();

            // Define trigger and animation that should play when the trigger is triggered. 
            m_elementImplicitAnimation.Insert(L"Offset", CreateOffsetAnimation());

            co_await GetItemsAsync();
        }
    }

    IAsyncAction MainPage::OnContainerContentChanging(ListViewBase sender, ContainerContentChangingEventArgs args)
    {
        auto elementVisual = ElementCompositionPreview::GetElementVisual(args.ItemContainer());
        auto image = args.ItemContainer().ContentTemplateRoot().as<Image>();

        if (args.InRecycleQueue())
        {
            elementVisual.ImplicitAnimations(nullptr);

            image.Source(nullptr);
        }

        if (args.Phase() == 0)
        {
            //Add implicit animation to each visual.
            elementVisual.ImplicitAnimations(m_elementImplicitAnimation);

            args.RegisterUpdateCallback([&](auto sender, auto args)
            {
                OnContainerContentChanging(sender, args);
            });

            args.Handled(true);
        }

        if (args.Phase() == 1)
        {
            // It's phase 1, so show this item's image.
            image.Opacity(100);

            auto item = unbox_value<PhotoEditor::Photo>(args.Item());
            Photo* impleType = get_self<Photo>(item);

            try
            {
                auto thumbnail = co_await impleType->GetImageThumbnailAsync();
                image.Source(thumbnail);
            }
            catch (winrt::hresult_error)
            {
                // File could be corrupt, or it might have an image file
                // extension, but not really be an image file.
                BitmapImage bitmapImage{};
                Uri uri{ image.BaseUri().AbsoluteUri(), L"Assets/StoreLogo.png" };
                bitmapImage.UriSource(uri);
                image.Source(bitmapImage);
            }
        }
    }

    // Called by the Loaded event of the ImageGridView for animation after back navigation from DetailPage view.
    IAsyncAction MainPage::StartConnectedAnimationForBackNavigation()
    {
        // Run the connected animation for navigation back to the main page from the detail page.
        if (m_persistedItem)
        {
            ImageGridView().ScrollIntoView(m_persistedItem);
            auto animation = ConnectedAnimationService::GetForCurrentView().GetAnimation(L"backAnimation");
            if (animation)
            {
                co_await ImageGridView().TryStartConnectedAnimationAsync(animation, m_persistedItem, L"ItemImage");
            }
        }
    }

    // Registers property changed event handler.
    event_token MainPage::PropertyChanged(PropertyChangedEventHandler const& value)
    {
        return m_propertyChanged.add(value);
    }

    // Unregisters property changed event handler.
    void MainPage::PropertyChanged(event_token const& token)
    {
        m_propertyChanged.remove(token);
    }

    // Loads images from the user's Pictures library.
    IAsyncAction MainPage::GetItemsAsync()
    {
        // Show the loading progress bar.
        LoadProgressIndicator().Visibility(Windows::UI::Xaml::Visibility::Visible);
        NoPicsText().Visibility(Windows::UI::Xaml::Visibility::Collapsed);

        // File type filter.
        QueryOptions options{};
        options.FolderDepth(FolderDepth::Deep);
        options.FileTypeFilter().Append(L".jpg");
        options.FileTypeFilter().Append(L".png");
        options.FileTypeFilter().Append(L".gif");

        // Get the Pictures library.
        StorageFolder picturesFolder = KnownFolders::PicturesLibrary();
        auto result = picturesFolder.CreateFileQueryWithOptions(options);
        auto imageFiles = co_await result.GetFilesAsync();
        auto unsupportedFilesFound = false;

        // Populate Photos collection.
        for (auto&& file : imageFiles)
        {
            // Only files on the local computer are supported. 
            // Files on OneDrive or a network location are excluded.
            if (file.Provider().Id() == L"computer")
            {
                auto image = co_await LoadImageInfoAsync(file);
                Photos().Append(image);
            }
            else
            {
                unsupportedFilesFound = true;
            }
        }

        if (Photos().Size() == 0)
        {
            // No pictures were found in the library, so show message.
            NoPicsText().Visibility(Windows::UI::Xaml::Visibility::Visible);
        }

        // Hide the loading progress bar.
        LoadProgressIndicator().Visibility(Windows::UI::Xaml::Visibility::Collapsed);

        if (unsupportedFilesFound)
        {
            ContentDialog unsupportedFilesDialog{};
            unsupportedFilesDialog.Title(box_value(L"Unsupported images found"));
            unsupportedFilesDialog.Content(box_value(L"This sample app only supports images stored locally on the computer. We found files in your library that are stored in OneDrive or another network location. We didn't load those images."));
            unsupportedFilesDialog.CloseButtonText(L"Ok");

            co_await unsupportedFilesDialog.ShowAsync();
        }
    }

    // Creates a Photo from Storage file for adding to Photo collection.
    IAsyncOperation<PhotoEditor::Photo> MainPage::LoadImageInfoAsync(StorageFile file)
    {
        auto properties = co_await file.Properties().GetImagePropertiesAsync();
        auto info = winrt::make<Photo>(properties, file, file.DisplayName(), file.DisplayType());
        co_return info;
    }

    CompositionAnimationGroup MainPage::CreateOffsetAnimation()
    {
        //Define Offset Animation for the Animation group.
        Vector3KeyFrameAnimation offsetAnimation = m_compositor.CreateVector3KeyFrameAnimation();
        offsetAnimation.InsertExpressionKeyFrame(1.0f, L"this.FinalValue");
        TimeSpan span{ std::chrono::milliseconds{400} };
        offsetAnimation.Duration(span);

        //Define Animation Target for this animation to animate using definition. 
        offsetAnimation.Target(L"Offset");

        //Add Animations to Animation group. 
        CompositionAnimationGroup animationGroup = m_compositor.CreateAnimationGroup();
        animationGroup.Add(offsetAnimation);

        return animationGroup;
    }

    // Photo clicked event handler for navigation to DetailPage view.
    void MainPage::ImageGridView_ItemClick(IInspectable const sender, ItemClickEventArgs const e)
    {
        // Prepare the connected animation for navigation to the detail page.
        m_persistedItem = e.ClickedItem().as<PhotoEditor::Photo>();
        ImageGridView().PrepareConnectedAnimation(L"itemAnimation", e.ClickedItem(), L"ItemImage");

        auto m_suppress = SuppressNavigationTransitionInfo();
        Frame().Navigate(xaml_typename<PhotoEditor::DetailPage>(), e.ClickedItem(), m_suppress);
    }

    // Triggers property changed notification.
    void MainPage::RaisePropertyChanged(hstring const& propertyName)
    {
        m_propertyChanged(*this, PropertyChangedEventArgs(propertyName));
    }
}