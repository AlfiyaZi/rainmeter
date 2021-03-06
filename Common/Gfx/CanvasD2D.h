/*
  Copyright (C) 2013 Birunthan Mohanathas

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef RM_GFX_CANVASD2D_H_
#define RM_GFX_CANVASD2D_H_

#include "Canvas.h"
#include "FontCollectionD2D.h"
#include "TextFormatD2D.h"
#include "Util/WICBitmapDIB.h"
#include <string>
#include <GdiPlus.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

namespace Gfx {

// Provides a Direct2D/DirectWrite implementation of Canvas.
class CanvasD2D : public Canvas
{
public:
	CanvasD2D();
	~CanvasD2D();

	virtual void Resize(int w, int h);

	virtual bool BeginDraw();
	virtual void EndDraw();

	virtual Gdiplus::Graphics& BeginGdiplusContext() override;
	virtual void EndGdiplusContext() override;

	virtual HDC GetDC() override;
	virtual void ReleaseDC(HDC dc) override;
	
	virtual FontCollection* CreateFontCollection() override { return new FontCollectionD2D(); }
	virtual TextFormat* CreateTextFormat() override { return new TextFormatD2D(); }

	virtual bool IsTransparentPixel(int x, int y) override;

	virtual void SetTransform(const Gdiplus::Matrix& matrix) override;
	virtual void ResetTransform() override;
	virtual void RotateTransform(float angle, float x, float y, float dx, float dy) override;

	virtual void SetAntiAliasing(bool enable) override;
	virtual void SetTextAntiAliasing(bool enable) override;

	virtual void Clear(const Gdiplus::Color& color) override;

	virtual void DrawTextW(const WCHAR* str, UINT strLen, const TextFormat& format, Gdiplus::RectF& rect, const Gdiplus::SolidBrush& brush) override;
	virtual bool MeasureTextW(const WCHAR* str, UINT strLen, const TextFormat& format, Gdiplus::RectF& rect) override;
	virtual bool MeasureTextLinesW(const WCHAR* str, UINT strLen, const TextFormat& format, Gdiplus::RectF& rect, UINT& lines) override;

	virtual void DrawBitmap(Gdiplus::Bitmap* bitmap, const Gdiplus::Rect& dstRect, const Gdiplus::Rect& srcRect) override;

	virtual void FillRectangle(Gdiplus::Rect& rect, const Gdiplus::SolidBrush& brush) override;

private:
	friend class FontCollectionD2D;
	friend class TextFormatD2D;

	CanvasD2D(const CanvasD2D& other) {}

	static bool Initialize();
	static void Finalize();

	void Dispose();

	bool BeginTargetDraw();
	void EndTargetDraw();

	// Retrieves current GDI+ transform (if any) and converts to a D2D Matrix
	D2D1_MATRIX_3X2_F GetCurrentTransform();

	ID2D1RenderTarget* m_Target;
	Util::WICBitmapDIB m_Bitmap;

	// GDI+ objects that share the pixel data of m_Bitmap.
	Gdiplus::Graphics* m_GdipGraphics;
	Gdiplus::Bitmap* m_GdipBitmap;

	bool m_TextAntiAliasing;

	static UINT c_Instances;
	static ID2D1Factory* c_D2DFactory;
	static IDWriteFactory* c_DWFactory;
	static IDWriteGdiInterop* c_DWGDIInterop;
	static IWICImagingFactory* c_WICFactory;
};

}  // namespace Gfx

#endif