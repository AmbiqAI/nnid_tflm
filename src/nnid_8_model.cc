/**
 * @file <nnid_8>_model.c (generated from template.cc)
 * @author autodeploy.py
 * @brief Generated automatically from template code by NS autodeploy script
 * @version 0.1
 * @date 2023-03-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "nnid_8_api.h"
#include "nnid_8_model.h"
#include "nnid_8_model_data.h"
#include "ns_model.h"

// Tensorflow Lite for Microcontroller includes (somewhat boilerplate)
// #include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_profiler.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

#ifdef NS_TFSTRUCTURE_RECENT
    #include "tensorflow/lite/micro/tflite_bridge/micro_error_reporter.h"
#else
    #include "tensorflow/lite/micro/micro_error_reporter.h"
#endif

static constexpr int nnid_8_tensor_arena_size = 1024 * nnid_8_COMPUTED_ARENA_SIZE;
alignas(16) static uint8_t nnid_8_tensor_arena[nnid_8_tensor_arena_size];

// Resource Variable Arena
static constexpr int nnid_8_resource_var_arena_size =
    4 * (0 + 1) * sizeof(tflite::MicroResourceVariables);
alignas(16) static uint8_t nnid_8_var_arena[nnid_8_resource_var_arena_size];

int
nnid_8_init(ns_model_state_t *ms);

int
nnid_8_minimal_init(ns_model_state_t *ms) {
    ms->runtime = TFLM;
    ms->model_array = nnid_8_model;
    ms->arena = nnid_8_tensor_arena;
    ms->arena_size = nnid_8_tensor_arena_size;
    ms->rv_arena = nnid_8_var_arena;
    ms->rv_arena_size = nnid_8_resource_var_arena_size;
    ms->rv_count = 0;
    ms->numInputTensors = 3;
    ms->numOutputTensors = 3;

    ms->tickTimer = NULL;
    ms->mac_estimate = NULL;

    int status = nnid_8_init(ms);
    return status;
}

int
nnid_8_init(ns_model_state_t *ms) {
    ms->state = NOT_READY;

    tflite::MicroErrorReporter micro_error_reporter;
    ms->error_reporter = &micro_error_reporter;

    tflite::InitializeTarget();

    // Map the model into a usable data structure. This doesn't involve any
    // copying or parsing, it's a very lightweight operation.
    ms->model = tflite::GetModel(ms->model_array);
    if (ms->model->version() != TFLITE_SCHEMA_VERSION) {
        TF_LITE_REPORT_ERROR(ms->error_reporter,
                             "Model provided is schema version %d not equal "
                             "to supported version %d.",
                             ms->model->version(), TFLITE_SCHEMA_VERSION);
        return nnid_8_STATUS_FAILURE;
    }

#ifdef NS_TFSTRUCTURE_RECENT
    static tflite::MicroMutableOpResolver<9> resolver;
#else
    static tflite::MicroMutableOpResolver<9> resolver(error_reporter);
#endif
    resolver.AddConv2D();
resolver.AddTanh();
resolver.AddStridedSlice();
resolver.AddUnpack();
resolver.AddFullyConnected();
resolver.AddAdd();
resolver.AddSplit();
resolver.AddLogistic();
resolver.AddMul();


    // Allocate ResourceVariable stuff if needed
    tflite::MicroResourceVariables *resource_variables;
    tflite::MicroAllocator *var_allocator;

    if (ms->rv_count != 0) {
        var_allocator = tflite::MicroAllocator::Create(ms->rv_arena, ms->rv_arena_size, nullptr);
        resource_variables = tflite::MicroResourceVariables::Create(var_allocator, ms->rv_count);
    } else {
        resource_variables = nullptr;
    }

    // Build an interpreter to run the model with.
#ifdef NS_TFSTRUCTURE_RECENT
    static tflite::MicroInterpreter static_interpreter(
        ms->model, resolver, ms->arena, ms->arena_size, resource_variables, ms->profiler);
#else
    static tflite::MicroInterpreter static_interpreter(
        ms->model, resolver, ms->arena, ms->arena_size, ms->error_reporter, nullptr, ms->profiler);
#endif
    ms->interpreter = &static_interpreter;

    // Allocate memory from the tensor_arena for the model's tensors.
    TfLiteStatus allocate_status = ms->interpreter->AllocateTensors();

    if (allocate_status != kTfLiteOk) {
        TF_LITE_REPORT_ERROR(ms->error_reporter, "AllocateTensors() failed");
        return nnid_8_STATUS_FAILURE;
    }

    // Obtain pointers to the model's input and output tensors.
    for (uint32_t t = 0; t <= ms->numInputTensors; t++) {
        ms->model_input[t] = ms->interpreter->input(t);
    }

    for (uint32_t t = 0; t <= ms->numOutputTensors; t++) {
        ms->model_output[t] = ms->interpreter->output(t);
    }

    ms->state = READY;
    return nnid_8_STATUS_SUCCESS;
}
