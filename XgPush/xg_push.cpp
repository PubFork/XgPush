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

namespace slib
{
	
#if !defined(SLIB_PLATFORM_IS_IOS)
	void XgPush::setAccessId_iOS(sl_uint32 accessId)
	{
	}
	
	void XgPush::setAccessKey_iOS(const String& accessKey)
	{
	}
#endif
	
	SLIB_STATIC_ZERO_INITIALIZED(AtomicFunction<void(String)>, _g_slib_xg_push_device_token_callback);

	Function<void(String)> XgPush::getDeviceTokenCallback()
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_slib_xg_push_device_token_callback)) {
			return sl_null;
		}
		return _g_slib_xg_push_device_token_callback;
	}
	
	void XgPush::setDeviceTokenCallback(const Function<void(String)>& token)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_slib_xg_push_device_token_callback)) {
			return;
		}
		_g_slib_xg_push_device_token_callback = token;
	}
	
	SLIB_STATIC_ZERO_INITIALIZED(AtomicString, _g_slib_xg_push_device_token);
	
	String XgPush::getDeviceToken()
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_slib_xg_push_device_token)) {
			return sl_null;
		}
		return _g_slib_xg_push_device_token;
	}
	
	SLIB_STATIC_ZERO_INITIALIZED(AtomicFunction<void(XgPushMessage& message)>, _g_slib_xg_push_received_message_callback);

	Function<void(XgPushMessage& message)> XgPush::getReceivedMessageCallback()
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_slib_xg_push_received_message_callback)) {
			return sl_null;
		}
		return _g_slib_xg_push_received_message_callback;
	}
	
	void XgPush::setReceivedMessageCallback(const Function<void(XgPushMessage& message)>& callback)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_slib_xg_push_received_message_callback)) {
			return;
		}
		_g_slib_xg_push_received_message_callback = callback;
	}
	
	void XgPush::_onUpdateDeviceToken(const String& token)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_slib_xg_push_device_token)) {
			return;
		}
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_slib_xg_push_device_token_callback)) {
			return;
		}
		_g_slib_xg_push_device_token = token;
		_g_slib_xg_push_device_token_callback(token);
		
	}
	
	void XgPush::_onReceiveMessage(XgPushMessage& message)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_slib_xg_push_received_message_callback)) {
			return;
		}
		_g_slib_xg_push_received_message_callback(message);
	}
	
}
