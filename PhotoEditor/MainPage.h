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

#pragma once
#include "MainPage.g.h"

namespace winrt::PhotoEditor::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();

        // Retreives collection of Photo objects for thumbnail view.
        Windows::Foundation::Collections::IVector<Windows::Foundation::IInspectable> Photos() const
        {
            return m_photos;
        }

        // Event handlers for loading and rendering images.
        Windows::Foundation::IAsyncAction OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs);
        Windows::Foundation::IAsyncAction OnContainerContentChanging(Windows::UI::Xaml::Controls::ListViewBase, Windows::UI::Xaml::Controls::ContainerContentChangingEventArgs);

        // Animation for navigation back from DetailPage view.
        Windows::Foundation::IAsyncAction StartConnectedAnimationForBackNavigation();

        // Property changed notifications.
        event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const&);
        void PropertyChanged(event_token const&);

        // Event handler.
        void ImageGridView_ItemClick(Windows::Foundation::IInspectable const, Windows::UI::Xaml::Controls::ItemClickEventArgs const);

    private:
        // Functions for image loading and animation.
        Windows::Foundation::IAsyncAction GetItemsAsync();
        Windows::UI::Composition::CompositionAnimationGroup CreateOffsetAnimation();
        Windows::Foundation::IAsyncOperation<PhotoEditor::Photo> LoadImageInfoAsync(Windows::Storage::StorageFile);

        // Backing field for Photo collection.
        Windows::Foundation::Collections::IVector<IInspectable> m_photos{ nullptr };

        // Field to store selected Photo for later back navigation.
        PhotoEditor::Photo m_persistedItem{ nullptr };

        // Collection of animations for element visuals for reorder animation.
        Windows::UI::Composition::ImplicitAnimationCollection m_elementImplicitAnimation{ nullptr };

        // Field to store page Compositor for creation of types in the Windows.UI.Composition namespace.
        Windows::UI::Composition::Compositor m_compositor{ nullptr };

        // Event
        event<Windows::Foundation::TypedEventHandler<Windows::UI::Xaml::Controls::ListViewBase, Windows::UI::Xaml::Controls::ContainerContentChangingEventArgs>> m_handler;

        // Property changed notifications.
        void RaisePropertyChanged(hstring const&);
        event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

    };
}

namespace winrt::PhotoEditor::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}