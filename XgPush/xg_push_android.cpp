/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "xg_push.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include <slib/core/log.h>
#include <slib/ui/platform.h>
#include <slib/ui/core.h>

namespace slib
{

	void _priv_XgPush_nativeOnDeviceToken(JNIEnv* env, jobject _this, jstring token);
	void _priv_XgPush_nativeOnReceiveMessage(JNIEnv* env, jobject _this, jboolean flagClicked, jstring title, jstring content, jstring data);

	SLIB_JNI_BEGIN_CLASS(JXgPush, "slib/xgpush/XgPush")
		SLIB_JNI_STATIC_METHOD(start, "start", "(Landroid/app/Activity;)V");
		SLIB_JNI_STATIC_METHOD(stop, "stop", "(Landroid/app/Activity;)V");

		SLIB_JNI_NATIVE(nativeOnDeviceToken, "nativeOnDeviceToken", "(Ljava/lang/String;)V", _priv_XgPush_nativeOnDeviceToken);
		SLIB_JNI_NATIVE(nativeOnReceiveMessage, "nativeOnReceiveMessage", "(ZLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", _priv_XgPush_nativeOnReceiveMessage);
	SLIB_JNI_END_CLASS

	sl_bool _g_slib_xgpush_flag_started = sl_false;

	void XgPush::start(sl_uint32 accessId, const String& _accessKey)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (!jactivity) {
			return;
		}
		if (_g_slib_xgpush_flag_started) {
			return;
		}
		_g_slib_xgpush_flag_started = sl_true;
		JXgPush::start.call(sl_null, jactivity);
	}
	
	void XgPush::stop()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (!jactivity) {
			return;
		}
		if (!_g_slib_xgpush_flag_started) {
			return;
		}
		_g_slib_xgpush_flag_started = sl_false;
		JXgPush::stop.call(sl_null, jactivity);
	}

	void _priv_XgPush_nativeOnDeviceToken(JNIEnv* env, jobject _this, jstring _token)
	{
		String token = Jni::getString(_token);
		XgPush::_onUpdateDeviceToken(token);
	}

	void _priv_XgPush_nativeOnReceiveMessage(JNIEnv* env, jobject _this, jboolean flagClicked, jstring title, jstring content, jstring data)
	{
		XgPushMessage message;
		message.title = Jni::getString(title);
		message.content = Jni::getString(content);
		message.data = Json::parseJson(Jni::getString(data));
		message.flagClicked = flagClicked ? sl_true : sl_false;
		XgPush::_onReceiveMessage(message);
	}

}

#endif