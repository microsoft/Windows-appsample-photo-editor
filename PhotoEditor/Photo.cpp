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
#include "Photo.h"
#include <sstream>

using namespace winrt;
using namespace std;
using namespace Windows::UI::Xaml;
using namespace Windows::Storage;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Storage::Streams;

namespace winrt::PhotoEditor::implementation
{
    IAsyncOperation<BitmapImage> Photo::GetImageThumbnailAsync() const
    {
        auto thumbnail = co_await m_imageFile.GetThumbnailAsync(FileProperties::ThumbnailMode::PicturesView);
        BitmapImage bitmapImage{};
        bitmapImage.SetSource(thumbnail);
        thumbnail.Close();
        co_return bitmapImage;
    }

    IAsyncOperation<BitmapImage> Photo::GetImageSourceAsync() const
    {
        IRandomAccessStream stream{ co_await ImageFile().OpenAsync(FileAccessMode::Read) };
        BitmapImage bitmap{};
        bitmap.SetSource(stream);
        co_return bitmap;
    }

    hstring Photo::ImageDimensions() const
    {
        wstringstream stringStream;
        stringStream << m_imageProperties.Width() << " x " << m_imageProperties.Height();
        wstring str = stringStream.str();
        return static_cast<hstring>(str);
    }

    void Photo::ImageTitle(hstring const& value)
    {
        if (m_imageProperties.Title() != value)
        {
            m_imageProperties.Title(value);
            auto ignoreResult = m_imageProperties.SavePropertiesAsync();
            RaisePropertyChanged(L"ImageTitle");
        }
    }
}