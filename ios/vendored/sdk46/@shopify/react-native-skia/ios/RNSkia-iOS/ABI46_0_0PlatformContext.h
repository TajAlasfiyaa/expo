#pragma once

#import <ABI46_0_0React/ABI46_0_0RCTBridge.h>

#include <functional>
#include <memory>
#include <string>

#include <ABI46_0_0DisplayLink.h>
#include <ABI46_0_0RNSkPlatformContext.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#include <SkStream.h>

#pragma clang diagnostic pop

#include <ABI46_0_0jsi/ABI46_0_0jsi.h>

namespace ABI46_0_0facebook {
  namespace ABI46_0_0React {
    class CallInvoker;
  }
}

namespace ABI46_0_0RNSkia {

using namespace ABI46_0_0facebook;

static void handleNotification(CFNotificationCenterRef center, void *observer, CFStringRef name, const void *object, CFDictionaryRef userInfo);

class ABI46_0_0PlatformContext : public ABI46_0_0RNSkPlatformContext {
public:
  ABI46_0_0PlatformContext(jsi::Runtime *runtime,
                  std::shared_ptr<ABI46_0_0React::CallInvoker> callInvoker)
      : ABI46_0_0RNSkPlatformContext(runtime, callInvoker, [[UIScreen mainScreen] scale]) {
        // We need to make sure we invalidate when modules are freed
        CFNotificationCenterAddObserver(
              CFNotificationCenterGetLocalCenter(),
              this,
              &handleNotification,
              (__bridge CFStringRef)ABI46_0_0RCTBridgeWillInvalidateModulesNotification,
              NULL,
              CFNotificationSuspensionBehaviorDeliverImmediately
          );
      }

  ~ABI46_0_0PlatformContext() {
    CFNotificationCenterRemoveEveryObserver(CFNotificationCenterGetLocalCenter(), this);
  }

  void startDrawLoop() override;
  void stopDrawLoop() override;

  virtual void performStreamOperation(
      const std::string &sourceUri,
      const std::function<void(std::unique_ptr<SkStreamAsset>)> &op) override;

  void raiseError(const std::exception &err) override;
  
  void willInvalidateModules() {
    // We need to do some house-cleaning here!
    invalidate();
  }
    
private:
  ABI46_0_0DisplayLink *_displayLink;
};

static void handleNotification(CFNotificationCenterRef center, void *observer, CFStringRef name,
                               const void *object, CFDictionaryRef userInfo) {
  (static_cast<ABI46_0_0PlatformContext*>(observer))->willInvalidateModules();
}

} // namespace ABI46_0_0RNSkia
