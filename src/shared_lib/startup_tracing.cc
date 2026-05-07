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

#include "perfetto/public/abi/startup_tracing_abi.h"

#include "perfetto/tracing/tracing.h"
#include "protos/perfetto/config/trace_config.gen.h"

struct PerfettoSetupStartupTracingOpts {
  PerfettoBackendTypes backend = 0;
  uint32_t timeout_ms = 10000;
  void (*on_setup)(struct PerfettoOnStartupTracingSetupCallbackArgs,
                   void*) = nullptr;
  void (*on_aborted)(void*) = nullptr;
  void (*on_adopted)(void*) = nullptr;
  void* user_arg = nullptr;
};

namespace {

perfetto::Tracing::SetupStartupTracingOpts ConvertOpts(
    const struct PerfettoSetupStartupTracingOpts* opts) {
  perfetto::Tracing::SetupStartupTracingOpts cpp_opts;
  if (!opts) {
    return cpp_opts;
  }
  cpp_opts.backend = static_cast<perfetto::BackendType>(opts->backend);
  cpp_opts.timeout_ms = opts->timeout_ms;

  if (opts->on_setup) {
    cpp_opts.on_setup =
        [cb = opts->on_setup, user_arg = opts->user_arg](
            perfetto::Tracing::OnStartupTracingSetupCallbackArgs args) {
          PerfettoOnStartupTracingSetupCallbackArgs c_args;
          c_args.num_data_sources_started = args.num_data_sources_started;
          cb(c_args, user_arg);
        };
  }
  if (opts->on_aborted) {
    cpp_opts.on_aborted = [cb = opts->on_aborted, user_arg = opts->user_arg]() {
      cb(user_arg);
    };
  }
  if (opts->on_adopted) {
    cpp_opts.on_adopted = [cb = opts->on_adopted, user_arg = opts->user_arg]() {
      cb(user_arg);
    };
  }
  return cpp_opts;
}

}  // namespace

struct PerfettoSetupStartupTracingOpts* PerfettoSetupStartupTracingOptsCreate(
    void) {
  return new PerfettoSetupStartupTracingOpts();
}

void PerfettoSetupStartupTracingOptsSetBackend(
    struct PerfettoSetupStartupTracingOpts* opts,
    PerfettoBackendTypes backend) {
  opts->backend = backend;
}

void PerfettoSetupStartupTracingOptsSetTimeoutMs(
    struct PerfettoSetupStartupTracingOpts* opts,
    uint32_t timeout_ms) {
  opts->timeout_ms = timeout_ms;
}

void PerfettoSetupStartupTracingOptsSetOnSetup(
    struct PerfettoSetupStartupTracingOpts* opts,
    void (*on_setup)(struct PerfettoOnStartupTracingSetupCallbackArgs, void*)) {
  opts->on_setup = on_setup;
}

void PerfettoSetupStartupTracingOptsSetOnAborted(
    struct PerfettoSetupStartupTracingOpts* opts,
    void (*on_aborted)(void*)) {
  opts->on_aborted = on_aborted;
}

void PerfettoSetupStartupTracingOptsSetOnAdopted(
    struct PerfettoSetupStartupTracingOpts* opts,
    void (*on_adopted)(void*)) {
  opts->on_adopted = on_adopted;
}

void PerfettoSetupStartupTracingOptsSetUserArg(
    struct PerfettoSetupStartupTracingOpts* opts,
    void* user_arg) {
  opts->user_arg = user_arg;
}

void PerfettoSetupStartupTracingOptsDestroy(
    struct PerfettoSetupStartupTracingOpts* opts) {
  delete opts;
}

struct PerfettoStartupTracingSessionImpl* PerfettoSetupStartupTracing(
    const void* cfg_begin,
    size_t cfg_len,
    const struct PerfettoSetupStartupTracingOpts* opts) {
  perfetto::TraceConfig cfg;
  if (cfg_begin && cfg_len > 0) {
    cfg.ParseFromArray(cfg_begin, cfg_len);
  }
  auto session = perfetto::Tracing::SetupStartupTracing(cfg, ConvertOpts(opts));
  return reinterpret_cast<struct PerfettoStartupTracingSessionImpl*>(
      session.release());
}

struct PerfettoStartupTracingSessionImpl* PerfettoSetupStartupTracingBlocking(
    const void* cfg_begin,
    size_t cfg_len,
    const struct PerfettoSetupStartupTracingOpts* opts) {
  perfetto::TraceConfig cfg;
  if (cfg_begin && cfg_len > 0) {
    cfg.ParseFromArray(cfg_begin, cfg_len);
  }
  auto session =
      perfetto::Tracing::SetupStartupTracingBlocking(cfg, ConvertOpts(opts));
  return reinterpret_cast<struct PerfettoStartupTracingSessionImpl*>(
      session.release());
}

void PerfettoStartupTracingSessionAbort(
    struct PerfettoStartupTracingSessionImpl* session) {
  auto* s = reinterpret_cast<perfetto::StartupTracingSession*>(session);
  s->Abort();
}

void PerfettoStartupTracingSessionAbortBlocking(
    struct PerfettoStartupTracingSessionImpl* session) {
  auto* s = reinterpret_cast<perfetto::StartupTracingSession*>(session);
  s->AbortBlocking();
}

void PerfettoStartupTracingSessionDestroy(
    struct PerfettoStartupTracingSessionImpl* session) {
  auto* s = reinterpret_cast<perfetto::StartupTracingSession*>(session);
  delete s;
}
