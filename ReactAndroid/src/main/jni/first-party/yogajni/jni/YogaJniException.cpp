/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdexcept>
#include <string>
#include "YogaJniException.h"
#include "common.h"

namespace facebook {
namespace yoga {
namespace vanillajni {

YogaJniException::YogaJniException() {
  jclass cl = getCurrentEnv()->FindClass("Ljava/lang/RuntimeException;");
  static const jmethodID methodId = facebook::yoga::vanillajni::getMethodId(
      getCurrentEnv(), cl, "<init>", "()V");
  auto throwable = getCurrentEnv()->NewObject(cl, methodId);
  throwable_ = make_global_ref(static_cast<jthrowable>(throwable));
}

YogaJniException::YogaJniException(jthrowable throwable) {
  throwable_ = make_global_ref(throwable);
}

YogaJniException::YogaJniException(YogaJniException&& rhs)
    : throwable_(std::move(rhs.throwable_)) {}

YogaJniException::YogaJniException(const YogaJniException& rhs) {
  throwable_ = make_global_ref(rhs.throwable_.get());
}

YogaJniException::~YogaJniException() {
  try {
    throwable_.reset();
  } catch (...) {
    std::terminate();
  }
}

ScopedLocalRef<jthrowable> YogaJniException::getThrowable() const noexcept {
  return make_local_ref(getCurrentEnv(), throwable_.get());
}
} // namespace vanillajni
} // namespace yoga
} // namespace facebook
