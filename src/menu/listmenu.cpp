/*
** listmenu.cpp
** A simple menu consisting of a list of items
**
**---------------------------------------------------------------------------
** Copyright 2010 Christoph Oelckers
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. The name of the author may not be used to endorse or promote products
**    derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
** IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
** NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
** THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**---------------------------------------------------------------------------
**
*/

#include "v_video.h"
#include "v_font.h"
#include "cmdlib.h"
#include "gstrings.h"
#include "g_level.h"
#include "gi.h"
#include "d_gui.h"
#include "d_event.h"
#include "menu/menu.h"

IMPLEMENT_CLASS(DListMenu, false, false)

IMPLEMENT_POINTERS_START(DListMenu)
IMPLEMENT_POINTER(mFocusControl)
IMPLEMENT_POINTERS_END

//=============================================================================
//
//
//
//=============================================================================

DListMenu::DListMenu(DMenu *parent, FListMenuDescriptor *desc)
: DMenu(parent)
{
	mDesc = NULL;
	if (desc != NULL) Init(parent, desc);
}

//=============================================================================
//
//
//
//=============================================================================

void DListMenu::Init(DMenu *parent, FListMenuDescriptor *desc)
{
	mParentMenu = parent;
	GC::WriteBarrier(this, parent);
	mDesc = desc;
	if (desc->mCenter)
	{
		int center = 160;
		for(unsigned i=0;i<mDesc->mItems.Size(); i++)
		{
			int xpos = mDesc->mItems[i]->GetX();
			int width = mDesc->mItems[i]->GetWidth();
			int curx = mDesc->mSelectOfsX;

			if (width > 0 && mDesc->mItems[i]->Selectable())
			{
				int left = 160 - (width - curx) / 2 - curx;
				if (left < center) center = left;
			}
		}
		for(unsigned i=0;i<mDesc->mItems.Size(); i++)
		{
			int width = mDesc->mItems[i]->GetWidth();

			if (width > 0)
			{
				mDesc->mItems[i]->SetX(center);
			}
		}
	}
}

//=============================================================================
//
//
//
//=============================================================================

DMenuItemBase *DListMenu::GetItem(FName name)
{
	for(unsigned i=0;i<mDesc->mItems.Size(); i++)
	{
		FName nm = mDesc->mItems[i]->GetAction(NULL);
		if (nm == name) return mDesc->mItems[i];
	}
	return NULL;
}

//=============================================================================
//
//
//
//=============================================================================

bool DListMenu::Responder (event_t *ev)
{
	if (ev->type == EV_GUI_Event)
	{
		if (ev->subtype == EV_GUI_KeyDown)
		{
			int ch = tolower (ev->data1);

			for(unsigned i = mDesc->mSelectedItem + 1; i < mDesc->mItems.Size(); i++)
			{
				if (mDesc->mItems[i]->CheckHotkey(ch))
				{
					mDesc->mSelectedItem = i;
					S_Sound(CHAN_VOICE | CHAN_UI, "menu/cursor", snd_menuvolume, ATTN_NONE);
					return true;
				}
			}
			for(int i = 0; i < mDesc->mSelectedItem; i++)
			{
				if (mDesc->mItems[i]->CheckHotkey(ch))
				{
					mDesc->mSelectedItem = i;
					S_Sound(CHAN_VOICE | CHAN_UI, "menu/cursor", snd_menuvolume, ATTN_NONE);
					return true;
				}
			}
		}
	}
	return Super::Responder(ev);
}

//=============================================================================
//
//
//
//=============================================================================

bool DListMenu::MenuEvent (int mkey, bool fromcontroller)
{
	int oldSelect = mDesc->mSelectedItem;
	int startedAt = mDesc->mSelectedItem;
	if (startedAt < 0) startedAt = 0;

	switch (mkey)
	{
	case MKEY_Up:
		do
		{
			if (--mDesc->mSelectedItem < 0) mDesc->mSelectedItem = mDesc->mItems.Size()-1;
		}
		while (!mDesc->mItems[mDesc->mSelectedItem]->Selectable() && mDesc->mSelectedItem != startedAt);
		if (mDesc->mSelectedItem == startedAt) mDesc->mSelectedItem = oldSelect;
		S_Sound (CHAN_VOICE | CHAN_UI, "menu/cursor", snd_menuvolume, ATTN_NONE);
		return true;

	case MKEY_Down:
		do
		{
			if (++mDesc->mSelectedItem >= (int)mDesc->mItems.Size()) mDesc->mSelectedItem = 0;
		}
		while (!mDesc->mItems[mDesc->mSelectedItem]->Selectable() && mDesc->mSelectedItem != startedAt);
		if (mDesc->mSelectedItem == startedAt) mDesc->mSelectedItem = oldSelect;
		S_Sound (CHAN_VOICE | CHAN_UI, "menu/cursor", snd_menuvolume, ATTN_NONE);
		return true;

	case MKEY_Enter:
		if (mDesc->mSelectedItem >= 0 && mDesc->mItems[mDesc->mSelectedItem]->Activate())
		{
			S_Sound (CHAN_VOICE | CHAN_UI, "menu/choose", snd_menuvolume, ATTN_NONE);
		}
		return true;

	default:
		return Super::MenuEvent(mkey, fromcontroller);
	}
}

//=============================================================================
//
//
//
//=============================================================================

bool DListMenu::MouseEvent(int type, int x, int y)
{
	int sel = -1;

	// convert x/y from screen to virtual coordinates, according to CleanX/Yfac use in DrawTexture
	x = ((x - (screen->GetWidth() / 2)) / CleanXfac) + 160;
	y = ((y - (screen->GetHeight() / 2)) / CleanYfac) + 100;

	if (mFocusControl != NULL)
	{
		mFocusControl->MouseEvent(type, x, y);
		return true;
	}
	else
	{
		if ((mDesc->mWLeft <= 0 || x > mDesc->mWLeft) &&
			(mDesc->mWRight <= 0 || x < mDesc->mWRight))
		{
			for(unsigned i=0;i<mDesc->mItems.Size(); i++)
			{
				if (mDesc->mItems[i]->CheckCoordinate(x, y))
				{
					if ((int)i != mDesc->mSelectedItem)
					{
						//S_Sound (CHAN_VOICE | CHAN_UI, "menu/cursor", snd_menuvolume, ATTN_NONE);
					}
					mDesc->mSelectedItem = i;
					mDesc->mItems[i]->MouseEvent(type, x, y);
					return true;
				}
			}
		}
	}
	mDesc->mSelectedItem = -1;
	return Super::MouseEvent(type, x, y);
}

//=============================================================================
//
//
//
//=============================================================================

void DListMenu::Ticker ()
{
	Super::Ticker();
	for(unsigned i=0;i<mDesc->mItems.Size(); i++)
	{
		mDesc->mItems[i]->Ticker();
	}
}

//=============================================================================
//
//
//
//=============================================================================

void DListMenu::Drawer ()
{
	for(unsigned i=0;i<mDesc->mItems.Size(); i++)
	{
		if (mDesc->mItems[i]->mEnabled) mDesc->mItems[i]->Drawer(mDesc->mSelectedItem == (int)i);
	}
	if (mDesc->mSelectedItem >= 0 && mDesc->mSelectedItem < (int)mDesc->mItems.Size())
		mDesc->mItems[mDesc->mSelectedItem]->DrawSelector(mDesc->mSelectOfsX, mDesc->mSelectOfsY, mDesc->mSelector);
	Super::Drawer();
}

//=============================================================================
//
// base class for menu items
//
//=============================================================================
IMPLEMENT_CLASS(DMenuItemBase, true, false)

bool DMenuItemBase::CheckCoordinate(int x, int y)
{
	return false;
}

void DMenuItemBase::Ticker()
{
}

void DMenuItemBase::Drawer(bool selected)
{
}

bool DMenuItemBase::Selectable()
{
	return false;
}

void DMenuItemBase::DrawSelector(int xofs, int yofs, FTextureID tex)
{
	if (tex.isNull())
	{
		if ((DMenu::MenuTime%8) < 6)
		{
			screen->DrawText(ConFont, OptionSettings.mFontColorSelection,
				(mXpos + xofs - 160) * CleanXfac + screen->GetWidth() / 2,
				(mYpos + yofs - 100) * CleanYfac + screen->GetHeight() / 2,
				"\xd",
				DTA_CellX, 8 * CleanXfac,
				DTA_CellY, 8 * CleanYfac,
				TAG_DONE);
		}
	}
	else
	{
		screen->DrawTexture (TexMan(tex), mXpos + xofs, mYpos + yofs, DTA_Clean, true, TAG_DONE);
	}
}

bool DMenuItemBase::Activate()
{
	return false;	// cannot be activated
}

FName DMenuItemBase::GetAction(int *pparam)
{
	return mAction;
}

bool DMenuItemBase::SetString(int i, const char *s)
{
	return false;
}

bool DMenuItemBase::GetString(int i, char *s, int len)
{
	return false;
}

bool DMenuItemBase::SetValue(int i, int value)
{
	return false;
}

bool DMenuItemBase::GetValue(int i, int *pvalue)
{
	return false;
}

void DMenuItemBase::Enable(bool on)
{
	mEnabled = on;
}

bool DMenuItemBase::MenuEvent(int mkey, bool fromcontroller)
{
	return false;
}

bool DMenuItemBase::MouseEvent(int type, int x, int y)
{
	return false;
}

bool DMenuItemBase::CheckHotkey(int c) 
{ 
	return false; 
}

int DMenuItemBase::GetWidth() 
{ 
	return 0; 
}


//=============================================================================
//
// static patch
//
//=============================================================================
IMPLEMENT_CLASS(DListMenuItemStaticPatch, false, false)

DListMenuItemStaticPatch::DListMenuItemStaticPatch(int x, int y, FTextureID patch, bool centered)
: DMenuItemBase(x, y)
{
	mTexture = patch;
	mCentered = centered;
}
	
void DListMenuItemStaticPatch::Drawer(bool selected)
{
	if (!mTexture.Exists())
	{
		return;
	}

	int x = mXpos;
	FTexture *tex = TexMan(mTexture);
	if (mYpos >= 0)
	{
		if (mCentered) x -= tex->GetScaledWidth()/2;
		screen->DrawTexture (tex, x, mYpos, DTA_Clean, true, TAG_DONE);
	}
	else
	{
		int x = (mXpos - 160) * CleanXfac + (SCREENWIDTH>>1);
		if (mCentered) x -= (tex->GetScaledWidth()*CleanXfac)/2;
		screen->DrawTexture (tex, x, -mYpos*CleanYfac, DTA_CleanNoMove, true, TAG_DONE);
	}
}

//=============================================================================
//
// static text
//
//=============================================================================
IMPLEMENT_CLASS(DListMenuItemStaticText, false, false)

DListMenuItemStaticText::DListMenuItemStaticText(int x, int y, const char *text, FFont *font, EColorRange color, bool centered)
: DMenuItemBase(x, y)
{
	mText = text;
	mFont = font;
	mColor = color;
	mCentered = centered;
}
	
void DListMenuItemStaticText::Drawer(bool selected)
{
	if (mText.IsNotEmpty())
	{
		const char *text = mText;
		if (*text == '$') text = GStrings(text+1);
		if (mYpos >= 0)
		{
			int x = mXpos;
			if (mCentered) x -= mFont->StringWidth(text)/2;
			screen->DrawText(mFont, mColor, x, mYpos, text, DTA_Clean, true, TAG_DONE);
		}
		else
		{
			int x = (mXpos - 160) * CleanXfac + (SCREENWIDTH>>1);
			if (mCentered) x -= (mFont->StringWidth(text)*CleanXfac)/2;
			screen->DrawText (mFont, mColor, x, -mYpos*CleanYfac, text, DTA_CleanNoMove, true, TAG_DONE);
		}
	}
}

//=============================================================================
//
// base class for selectable items
//
//=============================================================================
IMPLEMENT_CLASS(DListMenuItemSelectable, false, false)

DListMenuItemSelectable::DListMenuItemSelectable(int x, int y, int height, FName action, int param)
: DMenuItemBase(x, y, action)
{
	mHeight = height;
	mParam = param;
	mHotkey = 0;
}

bool DListMenuItemSelectable::CheckCoordinate(int x, int y)
{
	return mEnabled && y >= mYpos && y < mYpos + mHeight;	// no x check here
}

bool DListMenuItemSelectable::Selectable()
{
	return mEnabled;
}

bool DListMenuItemSelectable::Activate()
{
	M_SetMenu(mAction, mParam);
	return true;
}

FName DListMenuItemSelectable::GetAction(int *pparam)
{
	if (pparam != NULL) *pparam = mParam;
	return mAction;
}

bool DListMenuItemSelectable::CheckHotkey(int c) 
{ 
	return c == tolower(mHotkey); 
}

bool DListMenuItemSelectable::MouseEvent(int type, int x, int y)
{
	if (type == DMenu::MOUSE_Release)
	{
		if (NULL != DMenu::CurrentMenu && DMenu::CurrentMenu->MenuEvent(MKEY_Enter, true))
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
//
// text item
//
//=============================================================================
IMPLEMENT_CLASS(DListMenuItemText, false, false)

DListMenuItemText::DListMenuItemText(int x, int y, int height, int hotkey, const char *text, FFont *font, EColorRange color, EColorRange color2, FName child, int param)
: DListMenuItemSelectable(x, y, height, child, param)
{
	mText = ncopystring(text);
	mFont = font;
	mColor = color;
	mColorSelected = color2;
	mHotkey = hotkey;
}

void DListMenuItemText::OnDestroy()
{
	if (mText != NULL)
	{
		delete [] mText;
	}
}

void DListMenuItemText::Drawer(bool selected)
{
	const char *text = mText;
	if (text != NULL)
	{
		if (*text == '$') text = GStrings(text+1);
		screen->DrawText(mFont, selected ? mColorSelected : mColor, mXpos, mYpos, text, DTA_Clean, true, TAG_DONE);
	}
}

int DListMenuItemText::GetWidth() 
{ 
	const char *text = mText;
	if (text != NULL)
	{
		if (*text == '$') text = GStrings(text+1);
		return mFont->StringWidth(text); 
	}
	return 1;
}


//=============================================================================
//
// patch item
//
//=============================================================================
IMPLEMENT_CLASS(DListMenuItemPatch, false, false)

DListMenuItemPatch::DListMenuItemPatch(int x, int y, int height, int hotkey, FTextureID patch, FName child, int param)
: DListMenuItemSelectable(x, y, height, child, param)
{
	mHotkey = hotkey;
	mTexture = patch;
}

void DListMenuItemPatch::Drawer(bool selected)
{
	screen->DrawTexture (TexMan(mTexture), mXpos, mYpos, DTA_Clean, true, TAG_DONE);
}

int DListMenuItemPatch::GetWidth() 
{
	return mTexture.isValid() 
		? TexMan[mTexture]->GetScaledWidth() 
		: 0;
}

