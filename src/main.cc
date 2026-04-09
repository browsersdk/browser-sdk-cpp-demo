#include "stdafx.h"

namespace {
void PrintHelp() {
  std::cout
      << "Commands:\n"
      << "  help\n"
      << "  init <userSig> [workDir] [port] [debug]\n"
      << "  init-file <json-file>\n"
      << "  info\n"
      << "  browser-info\n"
      << "  browser-install-file <json-file>\n"
      << "  open <envId> [url1] [url2] ...\n"
      << "  open-file <json-file>\n"
      << "  close <envId> [envId] ...\n"
      << "  close-file <json-file>\n"
      << "  env-create-file <json-file>\n"
      << "  env-update-file <json-file>\n"
      << "  env-page-file <json-file>\n"
      << "  env-destroy-file <json-file>\n"
      << "  token-update <userSig>\n"
      << "  token-update-file <json-file>\n"
      << "  wait-open <envId> [timeoutSeconds]\n"
      << "  wait-close <envId> [timeoutSeconds]\n"
      << "  events\n"
      << "  clear-events\n"
      << "  shutdown\n"
      << "  quit | exit | q\n"
      << std::endl;
}

void PrintCode(const SDKWrapper &sdk, const std::string &label, int32_t code) {
  std::cout << label << ": " << sdk.DescribeCode(code) << std::endl;
}

void PrintJsonResponse(const std::string &body) {
  if (!body.empty())
    std::cout << Utils::PrettyJson(body) << std::endl;
}

bool CheckJsonFile(const std::string &path_text, stl::path *path) {
  if (!path)
    return false;
  *path = stl::path(path_text);
  if (std::filesystem::exists(*path))
    return true;
  std::cerr << "json file not found: " << path->string() << std::endl;
  return false;
}

bool ParseTimeout(const std::vector<std::string> &tokens, size_t index,
                  int *timeout_seconds) {
  if (!timeout_seconds)
    return false;
  if (index >= tokens.size()) {
    *timeout_seconds = 60;
    return true;
  }
  return Utils::TryParseInt(tokens[index], timeout_seconds);
}

bool RunCommand(SDKWrapper &sdk, const std::vector<std::string> &tokens,
                bool *exit_requested) {
  if (!exit_requested)
    return false;
  *exit_requested = false;
  if (tokens.empty())
    return true;

  const std::string command = Utils::ToLowerCopy(tokens[0]);

  if (command == "help") {
    PrintHelp();
    return true;
  }

  if (command == "q" || command == "quit" || command == "exit") {
    *exit_requested = true;
    return true;
  }

  if (command == "init") {
    if (tokens.size() < 2) {
      std::cerr << "usage: init <userSig> [workDir] [port] [debug]" << std::endl;
      return false;
    }
    DemoInitOptions options;
    options.user_sig = tokens[1];
    if (tokens.size() >= 3)
      options.work_dir = tokens[2];
    if (tokens.size() >= 4) {
      int parsed_port = 0;
      if (!Utils::TryParseInt(tokens[3], &parsed_port) || parsed_port < 0 ||
          parsed_port > 65535) {
        std::cerr << "invalid port: " << tokens[3] << std::endl;
        return false;
      }
      options.port = static_cast<uint16_t>(parsed_port);
    }
    if (tokens.size() >= 5 && !Utils::StringToBool(tokens[4], &options.debug)) {
      std::cerr << "invalid debug flag: " << tokens[4] << std::endl;
      return false;
    }
    std::string response;
    const int32_t code = sdk.Init(options, &response);
    PrintCode(sdk, "sdk_init", code);
    PrintJsonResponse(response);
    return true;
  }

  if (command == "init-file") {
    if (tokens.size() != 2) {
      std::cerr << "usage: init-file <json-file>" << std::endl;
      return false;
    }
    stl::path file;
    if (!CheckJsonFile(tokens[1], &file))
      return false;
    std::string response;
    const int32_t code = sdk.InitWithJsonFile(file, &response);
    PrintCode(sdk, "sdk_init", code);
    PrintJsonResponse(response);
    return true;
  }

  if (command == "info") {
    std::string response;
    const int32_t code = sdk.Info(&response);
    PrintCode(sdk, "sdk_info", code);
    PrintJsonResponse(response);
    return true;
  }

  if (command == "browser-info") {
    std::string response;
    const int32_t code = sdk.BrowserInfo(&response);
    PrintCode(sdk, "sdk_browser_info", code);
    PrintJsonResponse(response);
    return true;
  }

  if (command == "browser-install-file") {
    if (tokens.size() != 2) {
      std::cerr << "usage: browser-install-file <json-file>" << std::endl;
      return false;
    }
    stl::path file;
    if (!CheckJsonFile(tokens[1], &file))
      return false;
    const int32_t code = sdk.BrowserInstallWithJsonFile(file);
    PrintCode(sdk, "sdk_browser_install", code);
    return true;
  }

  if (command == "open") {
    if (tokens.size() < 2) {
      std::cerr << "usage: open <envId> [url1] [url2] ..." << std::endl;
      return false;
    }
    DemoBrowserOpenOptions options;
    options.env_id = tokens[1];
    for (size_t i = 2; i < tokens.size(); ++i)
      options.urls.push_back(tokens[i]);
    const int32_t code = sdk.BrowserOpen(options);
    PrintCode(sdk, "sdk_browser_open", code);
    return true;
  }

  if (command == "open-file") {
    if (tokens.size() != 2) {
      std::cerr << "usage: open-file <json-file>" << std::endl;
      return false;
    }
    stl::path file;
    if (!CheckJsonFile(tokens[1], &file))
      return false;
    const int32_t code = sdk.BrowserOpenWithJsonFile(file);
    PrintCode(sdk, "sdk_browser_open", code);
    return true;
  }

  if (command == "close") {
    if (tokens.size() < 2) {
      std::cerr << "usage: close <envId> [envId] ..." << std::endl;
      return false;
    }
    std::vector<std::string> env_ids(tokens.begin() + 1, tokens.end());
    const int32_t code = sdk.BrowserClose(env_ids);
    PrintCode(sdk, "sdk_browser_close", code);
    return true;
  }

  if (command == "close-file") {
    if (tokens.size() != 2) {
      std::cerr << "usage: close-file <json-file>" << std::endl;
      return false;
    }
    stl::path file;
    if (!CheckJsonFile(tokens[1], &file))
      return false;
    const int32_t code = sdk.BrowserCloseWithJsonFile(file);
    PrintCode(sdk, "sdk_browser_close", code);
    return true;
  }

  if (command == "env-create-file" || command == "env-update-file" ||
      command == "env-page-file" || command == "env-destroy-file") {
    if (tokens.size() != 2) {
      std::cerr << "usage: " << command << " <json-file>" << std::endl;
      return false;
    }
    stl::path file;
    if (!CheckJsonFile(tokens[1], &file))
      return false;
    std::string response;
    int32_t code = 0;
    if (command == "env-create-file")
      code = sdk.EnvCreateWithJsonFile(file, &response);
    else if (command == "env-update-file")
      code = sdk.EnvUpdateWithJsonFile(file, &response);
    else if (command == "env-page-file")
      code = sdk.EnvPageWithJsonFile(file, &response);
    else
      code = sdk.EnvDestroyWithJsonFile(file, &response);
    PrintCode(sdk, command, code);
    PrintJsonResponse(response);
    return true;
  }

  if (command == "token-update") {
    if (tokens.size() != 2) {
      std::cerr << "usage: token-update <userSig>" << std::endl;
      return false;
    }
    const int32_t code = sdk.TokenUpdate(tokens[1]);
    PrintCode(sdk, "sdk_token_update", code);
    return true;
  }

  if (command == "token-update-file") {
    if (tokens.size() != 2) {
      std::cerr << "usage: token-update-file <json-file>" << std::endl;
      return false;
    }
    stl::path file;
    if (!CheckJsonFile(tokens[1], &file))
      return false;
    const int32_t code = sdk.TokenUpdateWithJsonFile(file);
    PrintCode(sdk, "sdk_token_update", code);
    return true;
  }

  if (command == "wait-open" || command == "wait-close") {
    if (tokens.size() < 2) {
      std::cerr << "usage: " << command << " <envId> [timeoutSeconds]"
                << std::endl;
      return false;
    }
    int timeout_seconds = 60;
    if (!ParseTimeout(tokens, 2, &timeout_seconds)) {
      std::cerr << "invalid timeout value" << std::endl;
      return false;
    }
    DemoAsyncEvent event;
    const bool matched =
        command == "wait-open"
            ? sdk.WaitForBrowserOpenResult(tokens[1], timeout_seconds, &event)
            : sdk.WaitForBrowserCloseResult(tokens[1], timeout_seconds, &event);
    if (!matched) {
      std::cerr << "timeout waiting for " << command << " " << tokens[1]
                << std::endl;
      return false;
    }
    std::cout << "matched event: " << event.type << std::endl;
    PrintJsonResponse(event.payload);
    return true;
  }

  if (command == "events") {
    const auto events = sdk.Events();
    std::cout << "event-count: " << events.size() << std::endl;
    for (size_t i = 0; i < events.size(); ++i) {
      const auto &event = events[i];
      std::cout << "[" << i << "] " << event.received_at
                << " code=" << sdk.DescribeCode(event.callback_code);
      if (!event.type.empty())
        std::cout << " type=" << event.type;
      if (!event.req_id.empty())
        std::cout << " reqId=" << event.req_id;
      if (!event.env_id.empty())
        std::cout << " envId=" << event.env_id;
      std::cout << std::endl;
    }
    return true;
  }

  if (command == "clear-events") {
    sdk.ClearEvents();
    std::cout << "event history cleared." << std::endl;
    return true;
  }

  if (command == "shutdown") {
    const int32_t code = sdk.Shutdown();
    PrintCode(sdk, "sdk_shutdown", code);
    return true;
  }

  std::cerr << "unknown command: " << tokens[0] << std::endl;
  PrintHelp();
  return false;
}
} // namespace

int main(int argc, char **argv) {
  SDKWrapper sdk;
  std::cout << "BroSDK C++ Demo" << std::endl;
  if (sdk.Ready()) {
    std::cout << "Loaded runtime: " << sdk.ModulePath().string() << std::endl;
    std::cout << "Type \"help\" to list commands." << std::endl;
  } else {
    std::cerr << "Failed to load BroSDK runtime: " << sdk.LastError()
              << std::endl;
    return 1;
  }

  if (argc > 1) {
    std::vector<std::string> tokens;
    for (int i = 1; i < argc; ++i)
      tokens.emplace_back(argv[i]);
    bool exit_requested = false;
    return RunCommand(sdk, tokens, &exit_requested) ? 0 : 1;
  }

  PrintHelp();

  std::string input;
  while (true) {
    std::cout << "brosdk> " << std::flush;
    if (!std::getline(std::cin, input))
      break;
    const auto tokens = Utils::SplitCommandLine(input);
    if (tokens.empty())
      continue;
    bool exit_requested = false;
    RunCommand(sdk, tokens, &exit_requested);
    if (exit_requested)
      break;
  }

  return 0;
}
