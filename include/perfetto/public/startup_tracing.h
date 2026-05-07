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

#ifndef INCLUDE_PERFETTO_PUBLIC_STARTUP_TRACING_H_
#define INCLUDE_PERFETTO_PUBLIC_STARTUP_TRACING_H_

#include "perfetto/public/abi/startup_tracing_abi.h"
#include "perfetto/public/compiler.h"

// Arguments for PerfettoSetupStartupTracing. This struct is not ABI-stable,
// fields can be added and rearranged.
struct PerfettoSetupStartupTracingArgs {
  PerfettoBackendTypes backend;
  uint32_t timeout_ms;

  // If set, this callback is executed when startup tracing was set up.
  void (*on_setup)(struct PerfettoOnStartupTracingSetupCallbackArgs,
                   void* user_arg);

  // If set, this callback is executed if any data sources were aborted.
  void (*on_aborted)(void* user_arg);

  // If set, this callback is executed after all data sources were adopted.
  void (*on_adopted)(void* user_arg);

  // Passed as-is to the callbacks above.
  void* user_arg;
};

// Initializes a PerfettoSetupStartupTracingArgs struct with defaults.
static inline struct PerfettoSetupStartupTracingArgs
PerfettoSetupStartupTracingArgsDefault(void) {
  struct PerfettoSetupStartupTracingArgs args;
  args.backend = 0;  // unspecified
  args.timeout_ms = 10000;
  args.on_setup = PERFETTO_NULL;
  args.on_aborted = PERFETTO_NULL;
  args.on_adopted = PERFETTO_NULL;
  args.user_arg = PERFETTO_NULL;
  return args;
}

// Start a new startup tracing session using client args.
static inline struct PerfettoStartupTracingSessionImpl*
PerfettoSetupStartupTracingArgs(const void* cfg_begin,
                                size_t cfg_len,
                                struct PerfettoSetupStartupTracingArgs args) {
  struct PerfettoSetupStartupTracingOpts* opts =
      PerfettoSetupStartupTracingOptsCreate();

  PerfettoSetupStartupTracingOptsSetBackend(opts, args.backend);
  PerfettoSetupStartupTracingOptsSetTimeoutMs(opts, args.timeout_ms);
  if (args.on_setup) {
    PerfettoSetupStartupTracingOptsSetOnSetup(opts, args.on_setup);
  }
  if (args.on_aborted) {
    PerfettoSetupStartupTracingOptsSetOnAborted(opts, args.on_aborted);
  }
  if (args.on_adopted) {
    PerfettoSetupStartupTracingOptsSetOnAdopted(opts, args.on_adopted);
  }
  PerfettoSetupStartupTracingOptsSetUserArg(opts, args.user_arg);

  struct PerfettoStartupTracingSessionImpl* session =
      PerfettoSetupStartupTracing(cfg_begin, cfg_len, opts);

  PerfettoSetupStartupTracingOptsDestroy(opts);
  return session;
}

// Blocking version of PerfettoSetupStartupTracing using client args.
static inline struct PerfettoStartupTracingSessionImpl*
PerfettoSetupStartupTracingBlockingArgs(
    const void* cfg_begin,
    size_t cfg_len,
    struct PerfettoSetupStartupTracingArgs args) {
  struct PerfettoSetupStartupTracingOpts* opts =
      PerfettoSetupStartupTracingOptsCreate();

  PerfettoSetupStartupTracingOptsSetBackend(opts, args.backend);
  PerfettoSetupStartupTracingOptsSetTimeoutMs(opts, args.timeout_ms);
  if (args.on_setup) {
    PerfettoSetupStartupTracingOptsSetOnSetup(opts, args.on_setup);
  }
  if (args.on_aborted) {
    PerfettoSetupStartupTracingOptsSetOnAborted(opts, args.on_aborted);
  }
  if (args.on_adopted) {
    PerfettoSetupStartupTracingOptsSetOnAdopted(opts, args.on_adopted);
  }
  PerfettoSetupStartupTracingOptsSetUserArg(opts, args.user_arg);

  struct PerfettoStartupTracingSessionImpl* session =
      PerfettoSetupStartupTracingBlocking(cfg_begin, cfg_len, opts);

  PerfettoSetupStartupTracingOptsDestroy(opts);
  return session;
}

#endif  // INCLUDE_PERFETTO_PUBLIC_STARTUP_TRACING_H_
