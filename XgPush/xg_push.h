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

#ifndef CHECKHEADER_SLIB_EXT_XGPUSH_XG_PUSH_H
#define CHECKHEADER_SLIB_EXT_XGPUSH_XG_PUSH_H

#include <slib/core.h>

#include <slib/ui/notification.h>

namespace slib
{
	
	class XgPushMessage : public PushNotificationMessage
	{
	};
	
	class XgPush
	{
	public:
		static void setAccessId_iOS(sl_uint32 accessId);

		static void setAccessKey_iOS(const String& accessKey);

		static void start();
		
		static void stop();
		
		static void setEnableDebug(sl_bool flag);
		
		static Function<void(String)> getDeviceTokenCallback();
		
		static void setDeviceTokenCallback(const Function<void(String)>& token);
		
		static String getDeviceToken();
		
		static Function<void(XgPushMessage& message)> getReceivedMessageCallback();
		
		static void setReceivedMessageCallback(const Function<void(XgPushMessage& message)>& callback);
		
	public:
		static void _onUpdateDeviceToken(const String& token);
		
		static void _onReceiveMessage(XgPushMessage& message);
		
	};
	
}

#endif
