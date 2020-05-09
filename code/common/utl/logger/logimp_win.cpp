
/*
 * UTL : The universal utility library
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */
// TODO: move this in dcore?

#include <Windows.h>

#include "logger.h"

namespace utl {
namespace {

class fileSink final : public logBase {
    FILE* handle{nullptr};
    size_t bytes_written{0};

public:
    explicit fileSink(const std::wstring& filename) {
        _wfopen_s(&handle, filename.c_str(), L"w");
    }

    void close() {
        if (handle)
            fclose(handle);
    }

    const char* getName() override {
        return "fileOut";
    }

    void write(const logEntry& entry) override {
        constexpr std::size_t MAX_BYTES_WRITTEN = 50 * 1024L * 1024L;

        // prevent spam
        if (!handle || bytes_written > MAX_BYTES_WRITTEN)
            return;

        auto msg = formatLogEntry(entry).append(1, '\n');
        bytes_written += fwrite(static_cast<const void*>(msg.c_str()), msg.length(), 1, handle);

        if (entry.log_level >= logLevel::Error) {
            fflush(handle);
        }
    }
};

static void PrintMessage(const logEntry& entry) {
    auto str = formatLogEntry(entry);
    puts(str.c_str());
}

class consoleSink final : public logBase {

public:
    const char* getName() override {
        return "conOut";
    }

    void write(const logEntry& entry) override {
        HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
        if (console_handle == INVALID_HANDLE_VALUE) {
            return;
        }

        CONSOLE_SCREEN_BUFFER_INFO original_info = {};
        GetConsoleScreenBufferInfo(console_handle, &original_info);

        WORD color = 0;
        switch (entry.log_level) {
        case logLevel::Trace: // Grey
            color = FOREGROUND_INTENSITY;
            break;
        case logLevel::Debug: // Cyan
            color = FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        case logLevel::Info: // Bright gray
            color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        case logLevel::Warning: // Bright yellow
            color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case logLevel::Error: // Bright red
            color = FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case logLevel::Critical: // Bright magenta
            color = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        default:
            break;
        }

        SetConsoleTextAttribute(console_handle, color);
        PrintMessage(entry);
        SetConsoleTextAttribute(console_handle, original_info.wAttributes);
    }
};

class debugSink final : public logBase {
public:
    const char* getName() override {
        return "dbgOut";
    }

    void write(const logEntry& entry) override {
        auto str = formatLogEntry(entry).append(1, '\n');
        OutputDebugStringA(str.c_str());
    }
};
} // namespace

void createLogger(bool createConsole) {
    if (createConsole) {

        // check if there is already an existing console
        // that we can attach to
        DWORD pid = GetCurrentProcessId();
        if (!::AttachConsole(pid)) {
            ::AllocConsole();
            ::AttachConsole(pid);
            ::SetConsoleTitleW(PRJ_NAME_WIDE L" - console");

            FILE* file = nullptr;
            freopen_s(&file, "CON", "w", stdout);
            freopen_s(&file, "CONIN$", "r", stdin);
        }

        addLogSink(new consoleSink());
    }

    // attach the sinks to the log system
    addLogSink(new fileSink(PRJ_NAME_WIDE L".log"));

    if (IsDebuggerPresent())
        addLogSink(new debugSink());

    // attempt to properly close log file in case of a crash
    std::atexit([]() {
        auto* sink = static_cast<fileSink*>(getLogSink("fileOut"));
        if (sink)
            sink->close();
    });
}
}