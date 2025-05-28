#include "tfmodel.h"
#include <Arduino.h>


bool TFLiteHandler::setup() {
  tflite::InitializeTarget();

  // Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(g_model);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
      TF_LITE_REPORT_ERROR(error_reporter,
                          "Model provided is schema version %d not equal "
                          "to supported version %d.",
                          model->version(), TFLITE_SCHEMA_VERSION);
      return false;
  }

  // This pulls in all the operation implementations we need.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::AllOpsResolver resolver;

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
      TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
      return false;
  }

  // Obtain pointers to the model's input and output tensors.
  input = interpreter->input(0);
  output = interpreter->output(0);

  // Keep track of how many inferences we have performed.
  inference_count = 0;

  return true;
}

bool TFLiteHandler::loop() {
  // Calculate an x value to feed into the model. We compare the current
  // inference_count to the number of inferences per cycle to determine
  // our position within the range of possible x values the model was
  // trained on, and use this to calculate a value.
  float position = static_cast<float>(inference_count) /
                  static_cast<float>(kInferencesPerCycle);
  float x = position * kXrange;
  Serial.println("x");
  Serial.println(x);

  // Quantize the input from floating-point to integer
  int8_t x_quantized = x / input->params.scale + input->params.zero_point;
  Serial.println("x_quantized");
  Serial.println(x_quantized);
  // Place the quantized input in the model's input tensor
  input->data.int8[0] = x_quantized;
  Serial.println("set data");

  // Run inference, and report any error
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
      TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed on x: %f\n",
                          static_cast<double>(x));
      return false;
  }

  // Obtain the quantized output from model's output tensor
  int8_t y_quantized = output->data.int8[0];
  // Dequantize the output from integer to floating-point
  float y = (y_quantized - output->params.zero_point) * output->params.scale;

  // Output the results. A custom HandleOutput function can be implemented
  // for each supported hardware target.
  TF_LITE_REPORT_ERROR(
      error_reporter, "x_value: %f, y_value: %f\n",
      static_cast<double>(x),
      static_cast<double>(y)
  );

  // Increment the inference_counter, and reset it if we have reached
  // the total number per cycle
  inference_count += 1;
  if (inference_count >= kInferencesPerCycle) 
      inference_count = 0;

return true;
}