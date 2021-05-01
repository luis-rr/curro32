#ifndef TTFMODEL_H_
#define TTFMODEL_H_


#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include "model.h"


class TFLiteHandler {

  tflite::ErrorReporter* error_reporter = nullptr;
  const tflite::Model* model = nullptr;
  tflite::MicroInterpreter* interpreter = nullptr;
  TfLiteTensor* input = nullptr;
  TfLiteTensor* output = nullptr;
  int inference_count = 0;

  static const int kTensorArenaSize = 2000;
  uint8_t tensor_arena[kTensorArenaSize];

  // This constant represents the range of x values our model was trained on,
  // which is from 0 to (2 * Pi). We approximate Pi to avoid requiring additional
  // libraries.
  const float kXrange = 2.f * 3.14159265359f;

  // This constant determines the number of inferences to perform across the range
  // of x values defined above. Since each inference takes time, the higher this
  // number, the more time it will take to run through the entire range. The value
  // of this constant can be tuned so that one full cycle takes a desired amount
  // of time. Since different devices take different amounts of time to perform
  // inference, this value should be defined per-device.
  // This is a small number so that it's easy to read the logs
  const int kInferencesPerCycle = 20;

  public:
    bool setup();
    bool loop();
};


#endif  // TTFMODEL_H_
