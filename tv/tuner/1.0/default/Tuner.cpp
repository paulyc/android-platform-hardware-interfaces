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

#define LOG_TAG "android.hardware.tv.tuner@1.0-Tuner"

#include "Tuner.h"
#include <android/hardware/tv/tuner/1.0/IFrontendCallback.h>
#include <utils/Log.h>
#include "Demux.h"
#include "Descrambler.h"
#include "Frontend.h"
#include "Lnb.h"

namespace android {
namespace hardware {
namespace tv {
namespace tuner {
namespace V1_0 {
namespace implementation {

using ::android::hardware::tv::tuner::V1_0::DemuxId;

Tuner::Tuner() {
    // Static Frontends array to maintain local frontends information
    // Array index matches their FrontendId in the default impl
    mFrontendSize = 8;
    mFrontends.resize(mFrontendSize);
    mFrontends[0] = new Frontend(FrontendType::DVBT, 0, this);
    mFrontends[1] = new Frontend(FrontendType::ATSC, 1, this);
    mFrontends[2] = new Frontend(FrontendType::DVBC, 2, this);
    mFrontends[3] = new Frontend(FrontendType::DVBS, 3, this);
    mFrontends[4] = new Frontend(FrontendType::DVBT, 4, this);
    mFrontends[5] = new Frontend(FrontendType::ISDBT, 5, this);
    mFrontends[6] = new Frontend(FrontendType::ANALOG, 6, this);
    mFrontends[7] = new Frontend(FrontendType::ATSC, 7, this);
}

Tuner::~Tuner() {}

Return<void> Tuner::getFrontendIds(getFrontendIds_cb _hidl_cb) {
    ALOGV("%s", __FUNCTION__);

    vector<FrontendId> frontendIds;
    frontendIds.resize(mFrontendSize);
    for (int i = 0; i < mFrontendSize; i++) {
        frontendIds[i] = mFrontends[i]->getFrontendId();
    }

    _hidl_cb(Result::SUCCESS, frontendIds);
    return Void();
}

Return<void> Tuner::openFrontendById(uint32_t frontendId, openFrontendById_cb _hidl_cb) {
    ALOGV("%s", __FUNCTION__);

    if (frontendId >= mFrontendSize || frontendId < 0) {
        ALOGW("[   WARN   ] Frontend with id %d isn't available", frontendId);
        _hidl_cb(Result::UNAVAILABLE, nullptr);
        return Void();
    }

    _hidl_cb(Result::SUCCESS, mFrontends[frontendId]);
    return Void();
}

Return<void> Tuner::openDemux(openDemux_cb _hidl_cb) {
    ALOGV("%s", __FUNCTION__);

    DemuxId demuxId = mLastUsedId + 1;
    mLastUsedId += 1;
    sp<Demux> demux = new Demux(demuxId, this);
    mDemuxes[demuxId] = demux;

    _hidl_cb(Result::SUCCESS, demuxId, demux);
    return Void();
}

Return<void> Tuner::getDemuxCaps(getDemuxCaps_cb _hidl_cb) {
    ALOGV("%s", __FUNCTION__);

    DemuxCapabilities caps;

    _hidl_cb(Result::SUCCESS, caps);
    return Void();
}

Return<void> Tuner::openDescrambler(openDescrambler_cb _hidl_cb) {
    ALOGV("%s", __FUNCTION__);

    sp<IDescrambler> descrambler = new Descrambler();

    _hidl_cb(Result::SUCCESS, descrambler);
    return Void();
}

Return<void> Tuner::getFrontendInfo(FrontendId /* frontendId */, getFrontendInfo_cb _hidl_cb) {
    ALOGV("%s", __FUNCTION__);

    FrontendInfo info;

    _hidl_cb(Result::SUCCESS, info);
    return Void();
}

Return<void> Tuner::getLnbIds(getLnbIds_cb _hidl_cb) {
    ALOGV("%s", __FUNCTION__);

    vector<LnbId> lnbIds;

    _hidl_cb(Result::SUCCESS, lnbIds);
    return Void();
}

Return<void> Tuner::openLnbById(LnbId /* lnbId */, openLnbById_cb _hidl_cb) {
    ALOGV("%s", __FUNCTION__);

    sp<ILnb> lnb = new Lnb();

    _hidl_cb(Result::SUCCESS, lnb);
    return Void();
}

sp<Frontend> Tuner::getFrontendById(uint32_t frontendId) {
    ALOGV("%s", __FUNCTION__);

    return mFrontends[frontendId];
}

void Tuner::setFrontendAsDemuxSource(uint32_t frontendId, uint32_t demuxId) {
    mFrontendToDemux[frontendId] = demuxId;
}

void Tuner::frontendStopTune(uint32_t frontendId) {
    map<uint32_t, uint32_t>::iterator it = mFrontendToDemux.find(frontendId);
    uint32_t demuxId;
    if (it != mFrontendToDemux.end()) {
        demuxId = it->second;
        mDemuxes[demuxId]->stopBroadcastInput();
    }
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace tuner
}  // namespace tv
}  // namespace hardware
}  // namespace android
