/*
 * Copyright (C) 2026 The Android Open Source Project
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

#ifndef INCLUDE_PERFETTO_PUBLIC_ABI_STARTUP_TRACING_ABI_H_
#define INCLUDE_PERFETTO_PUBLIC_ABI_STARTUP_TRACING_ABI_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "perfetto/public/abi/backend_type.h"
#include "perfetto/public/abi/export.h"

#ifdef __cplusplus
extern "C" {
#endif

// Opaque pointer to the internal representation of a startup tracing session.
struct PerfettoStartupTracingSessionImpl;

// Opaque pointer to an object that stores the startup tracing options.
struct PerfettoSetupStartupTracingOpts;

// Callback arguments for when startup tracing was set up.
struct PerfettoOnStartupTracingSetupCallbackArgs {
  int num_data_sources_started;
};

// Creates an object to store the startup tracing options.
PERFETTO_SDK_EXPORT struct PerfettoSetupStartupTracingOpts*
PerfettoSetupStartupTracingOptsCreate(void);

// Sets the backend.
PERFETTO_SDK_EXPORT void PerfettoSetupStartupTracingOptsSetBackend(
    struct PerfettoSetupStartupTracingOpts*,
    PerfettoBackendTypes backend);

// Sets the timeout in milliseconds.
PERFETTO_SDK_EXPORT void PerfettoSetupStartupTracingOptsSetTimeoutMs(
    struct PerfettoSetupStartupTracingOpts*,
    uint32_t timeout_ms);

// Sets the on_setup callback.
PERFETTO_SDK_EXPORT void PerfettoSetupStartupTracingOptsSetOnSetup(
    struct PerfettoSetupStartupTracingOpts*,
    void (*on_setup)(struct PerfettoOnStartupTracingSetupCallbackArgs, void*));

// Sets the on_aborted callback.
PERFETTO_SDK_EXPORT void PerfettoSetupStartupTracingOptsSetOnAborted(
    struct PerfettoSetupStartupTracingOpts*,
    void (*on_aborted)(void*));

// Sets the on_adopted callback.
PERFETTO_SDK_EXPORT void PerfettoSetupStartupTracingOptsSetOnAdopted(
    struct PerfettoSetupStartupTracingOpts*,
    void (*on_adopted)(void*));

// Sets the user_arg.
PERFETTO_SDK_EXPORT void PerfettoSetupStartupTracingOptsSetUserArg(
    struct PerfettoSetupStartupTracingOpts*,
    void* user_arg);

// Destroys the options object.
PERFETTO_SDK_EXPORT void PerfettoSetupStartupTracingOptsDestroy(
    struct PerfettoSetupStartupTracingOpts*);

// Start a new startup tracing session.
PERFETTO_SDK_EXPORT struct PerfettoStartupTracingSessionImpl*
PerfettoSetupStartupTracing(const void* cfg_begin,
                            size_t cfg_len,
                            const struct PerfettoSetupStartupTracingOpts* opts);

// Blocking version of PerfettoSetupStartupTracing.
PERFETTO_SDK_EXPORT struct PerfettoStartupTracingSessionImpl*
PerfettoSetupStartupTracingBlocking(
    const void* cfg_begin,
    size_t cfg_len,
    const struct PerfettoSetupStartupTracingOpts* opts);

// Abort any active but still unbound data source instances that belong to
// this startup tracing session.
PERFETTO_SDK_EXPORT void PerfettoStartupTracingSessionAbort(
    struct PerfettoStartupTracingSessionImpl*);

// Same as above, but blocks the current thread until aborted.
PERFETTO_SDK_EXPORT void PerfettoStartupTracingSessionAbortBlocking(
    struct PerfettoStartupTracingSessionImpl*);

// Destroy and clean up the startup tracing session wrapper object.
PERFETTO_SDK_EXPORT void PerfettoStartupTracingSessionDestroy(
    struct PerfettoStartupTracingSessionImpl*);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_PERFETTO_PUBLIC_ABI_STARTUP_TRACING_ABI_H_
