/*
 * Copyright (C) 2018 The Android Open Source Project
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

#define LOG_TAG "neuralnetworks_hidl_hal_test"

#include "VtsHalNeuralnetworks.h"

namespace android::hardware::neuralnetworks::V1_0::vts::functional {

// create device test
TEST_P(NeuralnetworksHidlTest, CreateDevice) {}

// status test
TEST_P(NeuralnetworksHidlTest, StatusTest) {
    Return<DeviceStatus> status = kDevice->getStatus();
    ASSERT_TRUE(status.isOk());
    EXPECT_EQ(DeviceStatus::AVAILABLE, static_cast<DeviceStatus>(status));
}

// initialization
TEST_P(NeuralnetworksHidlTest, GetCapabilitiesTest) {
    Return<void> ret =
            kDevice->getCapabilities([](ErrorStatus status, const Capabilities& capabilities) {
                EXPECT_EQ(ErrorStatus::NONE, status);
                EXPECT_LT(0.0f, capabilities.float32Performance.execTime);
                EXPECT_LT(0.0f, capabilities.float32Performance.powerUsage);
                EXPECT_LT(0.0f, capabilities.quantized8Performance.execTime);
                EXPECT_LT(0.0f, capabilities.quantized8Performance.powerUsage);
            });
    EXPECT_TRUE(ret.isOk());
}

}  // namespace android::hardware::neuralnetworks::V1_0::vts::functional
