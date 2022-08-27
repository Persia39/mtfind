#include "FileWrapper.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <thread>
#include <regex>


FileWrapper::FileWrapper(std::string &path, EncodingType encodingType) : _path(path), _encodingType(encodingType) {}

void FileWrapper::OutputAllUsageOfString(std::string substring, std::ostream &os) {

    if (!std::filesystem::exists(_path)) {
        std::cerr << "File '" << _path << "' doesn't exist" << std::endl;
        return;
    }

    std::thread readerThread(&FileWrapper::ReadFromFile, this);
    std::thread handlerThread(&FileWrapper::FindAndOutputSubstringInLines, this, std::ref(substring), std::ref(os));

    readerThread.join();
    handlerThread.join();
}

void FileWrapper::ReadFromFile() {
    if (_encodingType != EncodingType::ASCII){
        return;
    }

    std::ifstream input_file(_path);
    std::string line;
    uint64_t index_line{1};

    if (input_file.is_open()) {
        while (std::getline(input_file, line)) {
            PushLineToQueue(index_line++, line);
        }
    } else {
        std::cerr << "Couldn't open the file - '" << _path << "'" << std::endl;
    }
    _firstThreadIsFinished.store(true, std::memory_order_relaxed);
}

void FileWrapper::FindAndOutputSubstringInLines(std::string &substring, std::ostream &os) {
    std::replace(substring.begin(), substring.end(), '?', '.');

    std::map<std::pair<uint64_t, uint64_t>, std::string> findings;
    auto regex = std::regex(substring);

    while (!_firstThreadIsFinished.load(std::memory_order_relaxed) || !QueueIsEmpty()) {
        const auto opt_pair = PopLineFromQueue();
        if (opt_pair.has_value()) {
            const std::string &current_line = opt_pair.value().second;
            const uint64_t index_line = opt_pair.value().first;
            for (std::sregex_iterator iter = std::sregex_iterator(std::begin(current_line), std::end(current_line), regex);
                 iter != std::sregex_iterator();
                 ++iter) {
                std::smatch match = *iter;
                findings.insert({{index_line, match.position() + 1}, match.str()});
            }
        }
    }

    for (const auto&[pair, str]: findings)
        os << pair.first << " " << pair.second << " " << str << std::endl;
}

void FileWrapper::PushLineToQueue(const uint64_t index, const std::string &str) {
    _queueOfLines.Push({index, str});
}

std::optional<FileWrapper::PairIndexString> FileWrapper::PopLineFromQueue() {
    if (_queueOfLines.IsEmpty())
        return {};
    else
        return _queueOfLines.Pop();
}
