/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

// extern "C" void app_main(void) {
//   setup();
//   while (true) {
//     loop();
//   }
// }


/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/


#define BUFFER_LENGTH 1024

#include <Arduino.h>

#include "tfmodel.h"
#include "mpu.h"
#include "motor.h"

#include "board.h"


TFLiteHandler tfliteHandler;
MPUHandler mpuHandler;
MotorHandler motorHandler;


void setupSerial() {
  Serial.begin(115200);
  Serial.println("setup serial");
}


// The name of this function is important for Arduino compatibility.
void setup() {

  setupSerial();

  // if(!tfliteHandler.setup())
    // return;

  // mpuHandler.setup();

  if(!motorHandler.setup())
    return;
}

void loop() {
  // tfliteHandler.loop();
  // mpuHandler.loop();

  motorHandler.loop();
}
