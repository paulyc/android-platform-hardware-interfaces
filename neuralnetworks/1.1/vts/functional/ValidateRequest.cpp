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

#include "1.0/Callbacks.h"
#include "1.0/Utils.h"
#include "GeneratedTestHarness.h"
#include "VtsHalNeuralnetworks.h"

namespace android::hardware::neuralnetworks::V1_1::vts::functional {

using V1_0::ErrorStatus;
using V1_0::IPreparedModel;
using V1_0::Request;
using V1_0::implementation::ExecutionCallback;

///////////////////////// UTILITY FUNCTIONS /////////////////////////

// Primary validation function. This function will take a valid request, apply a
// mutation to it to invalidate the request, then pass it to interface calls
// that use the request. Note that the request here is passed by value, and any
// mutation to the request does not leave this function.
static void validate(const sp<IPreparedModel>& preparedModel, const std::string& message,
                     Request request, const std::function<void(Request*)>& mutation) {
    mutation(&request);
    SCOPED_TRACE(message + " [execute]");

    sp<ExecutionCallback> executionCallback = new ExecutionCallback();
    Return<ErrorStatus> executeLaunchStatus = preparedModel->execute(request, executionCallback);
    ASSERT_TRUE(executeLaunchStatus.isOk());
    ASSERT_EQ(ErrorStatus::INVALID_ARGUMENT, static_cast<ErrorStatus>(executeLaunchStatus));

    executionCallback->wait();
    ErrorStatus executionReturnStatus = executionCallback->getStatus();
    ASSERT_EQ(ErrorStatus::INVALID_ARGUMENT, executionReturnStatus);
}

///////////////////////// REMOVE INPUT ////////////////////////////////////

static void removeInputTest(const sp<IPreparedModel>& preparedModel, const Request& request) {
    for (size_t input = 0; input < request.inputs.size(); ++input) {
        const std::string message = "removeInput: removed input " + std::to_string(input);
        validate(preparedModel, message, request,
                 [input](Request* request) { hidl_vec_removeAt(&request->inputs, input); });
    }
}

///////////////////////// REMOVE OUTPUT ////////////////////////////////////

static void removeOutputTest(const sp<IPreparedModel>& preparedModel, const Request& request) {
    for (size_t output = 0; output < request.outputs.size(); ++output) {
        const std::string message = "removeOutput: removed Output " + std::to_string(output);
        validate(preparedModel, message, request,
                 [output](Request* request) { hidl_vec_removeAt(&request->outputs, output); });
    }
}

///////////////////////////// ENTRY POINT //////////////////////////////////

void validateRequest(const sp<IPreparedModel>& preparedModel, const Request& request) {
    removeInputTest(preparedModel, request);
    removeOutputTest(preparedModel, request);
}

}  // namespace android::hardware::neuralnetworks::V1_1::vts::functional
