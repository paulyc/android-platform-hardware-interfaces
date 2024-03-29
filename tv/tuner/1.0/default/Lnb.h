/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_HARDWARE_TV_TUNER_V1_0_LNB_H_
#define ANDROID_HARDWARE_TV_TUNER_V1_0_LNB_H_

#include <android/hardware/tv/tuner/1.0/ILnb.h>
#include <android/hardware/tv/tuner/1.0/ITuner.h>

using namespace std;

namespace android {
namespace hardware {
namespace tv {
namespace tuner {
namespace V1_0 {
namespace implementation {

using ::android::hardware::tv::tuner::V1_0::FrontendLnbPosition;
using ::android::hardware::tv::tuner::V1_0::FrontendLnbTone;
using ::android::hardware::tv::tuner::V1_0::FrontendLnbVoltage;
using ::android::hardware::tv::tuner::V1_0::Result;

class Lnb : public ILnb {
  public:
    Lnb();

    virtual Return<Result> setVoltage(FrontendLnbVoltage voltage) override;

    virtual Return<Result> setTone(FrontendLnbTone tone) override;

    virtual Return<Result> setSatellitePosition(FrontendLnbPosition position) override;

    virtual Return<Result> sendDiseqcMessage(const hidl_vec<uint8_t>& diseqcMessage) override;

    virtual Return<Result> close() override;

  private:
    virtual ~Lnb();
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace tuner
}  // namespace tv
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_TV_TUNER_V1_0_LNB_H_