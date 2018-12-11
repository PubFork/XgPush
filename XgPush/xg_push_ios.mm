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

#include "iOS/XGPush.h"

#include <slib/core/log.h>
#include <slib/ui/platform.h>

@interface SLibXGPush : NSObject<XGPushDelegate>
{
}
@end

namespace slib
{

	SLibXGPush* _g_slib_xgpush_instance = nil;
	sl_bool _g_slib_xgpush_flag_started = sl_false;

	void XgPush::start(sl_uint32 accessId, const String& _accessKey)
	{
		if (_g_slib_xgpush_instance == nil) {
			_g_slib_xgpush_instance = [SLibXGPush new];
			UIPlatform::registerDidReceiveRemoteNotificationCallback([](NSDictionary* userInfo) {
				[[XGPush defaultManager] reportXGNotificationInfo:userInfo];
			});
		}
		if (_g_slib_xgpush_flag_started) {
			return;
		}
		_g_slib_xgpush_flag_started = sl_true;
		NSString* accessKey = Apple::getNSStringFromString(_accessKey);
		[[XGPush defaultManager] startXGWithAppID:accessId appKey:accessKey delegate:_g_slib_xgpush_instance];
	}
	
	void XgPush::stop()
	{
		if (!_g_slib_xgpush_flag_started) {
			return;
		}
		_g_slib_xgpush_flag_started = sl_false;
		[[XGPush defaultManager] stopXGNotification];
	}

}

@implementation SLibXGPush

- (void)xgPushDidRegisteredDeviceToken:(nullable NSString *)deviceToken error:(nullable NSError *)error
{
	if (error != nil) {
		LogError("XgPush", "Error: %s", slib::Apple::getStringFromNSString([error localizedDescription]));
	} else {
		slib::String token = slib::Apple::getStringFromNSString(deviceToken);
		slib::XgPush::_onUpdateDeviceToken(token);
	}
}

- (void)xgPushDidReceiveRemoteNotification:(id)notification withCompletionHandler:(void (^)(NSUInteger))completionHandler {
	if ([notification isKindOfClass:[NSDictionary class]]) {
		[[XGPush defaultManager] reportXGNotificationInfo:(NSDictionary *)notification];
		completionHandler(UIBackgroundFetchResultNewData);
	} else if ([notification isKindOfClass:[UNNotification class]]) {
		NSDictionary* userInfo = ((UNNotification *)notification).request.content.userInfo;
		[[XGPush defaultManager] reportXGNotificationInfo:userInfo];
		slib::XgPushMessage message;
		if (slib::UIPlatform::parseRemoteNotificationInfo(userInfo, message)) {
			message.flagClicked = sl_false;
			slib::XgPush::_onReceiveMessage(message);
		}
		completionHandler(UNNotificationPresentationOptionBadge | UNNotificationPresentationOptionSound | UNNotificationPresentationOptionAlert);
	}
}

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_10_0
- (void)xgPushUserNotificationCenter:(UNUserNotificationCenter *)center
	  didReceiveNotificationResponse:(UNNotificationResponse *)response
			   withCompletionHandler:(void (^)(void))completionHandler
{
	[[XGPush defaultManager] reportXGNotificationResponse:response];
	slib::XgPushMessage message;
	NSDictionary* userInfo = response.notification.request.content.userInfo;
	if (slib::UIPlatform::parseRemoteNotificationInfo(userInfo, message)) {
		message.flagClicked = sl_true;
		slib::XgPush::_onReceiveMessage(message);
	}
	completionHandler();
}

#endif

@end
