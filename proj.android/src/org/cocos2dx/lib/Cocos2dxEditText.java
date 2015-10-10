/****************************************************************************
Copyright (c) 2012 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.lib;

import com.xw.BullfightGame.BullfightGame;

import android.app.Service;
import android.content.Context;
import android.media.AudioManager;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.widget.EditText;

public class Cocos2dxEditText extends EditText {
	// ===========================================================
	// Constants
	// ===========================================================

	// ===========================================================
	// Fields
	// ===========================================================

	private Cocos2dxGLSurfaceView mCocos2dxGLSurfaceView;

	// ===========================================================
	// Constructors
	// ===========================================================

	public Cocos2dxEditText(final Context context) {
		super(context);
	}

	public Cocos2dxEditText(final Context context, final AttributeSet attrs) {
		super(context, attrs);
	}

	public Cocos2dxEditText(final Context context, final AttributeSet attrs,
			final int defStyle) {
		super(context, attrs, defStyle);
	}

	// ===========================================================
	// Getter & Setter
	// ===========================================================

	public void setCocos2dxGLSurfaceView(
			final Cocos2dxGLSurfaceView pCocos2dxGLSurfaceView) {
		this.mCocos2dxGLSurfaceView = pCocos2dxGLSurfaceView;
	}

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	@Override
	public boolean onKeyDown(final int pKeyCode, final KeyEvent pKeyEvent) {
		super.onKeyDown(pKeyCode, pKeyEvent);
		AudioManager audio = (AudioManager) BullfightGame.game
				.getSystemService(Service.AUDIO_SERVICE);
		int currentVolume = audio.getStreamVolume(AudioManager.STREAM_MUSIC);
		/* Let GlSurfaceView get focus if back key is input. */
		if (pKeyCode == KeyEvent.KEYCODE_BACK) {
			this.mCocos2dxGLSurfaceView.requestFocus();
		} else if (pKeyCode == KeyEvent.KEYCODE_VOLUME_DOWN) {
			audio.setStreamVolume(AudioManager.STREAM_MUSIC, currentVolume - 1,
					1);// 2
		} else if (pKeyCode == KeyEvent.KEYCODE_VOLUME_UP) {
			audio.setStreamVolume(AudioManager.STREAM_MUSIC, currentVolume + 1,
					1);// 3
		}

		return true;
	}

	// ===========================================================
	// Methods
	// ===========================================================

	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================
}
