#ifndef MTFIND_FILEWRAPPER_H
#define MTFIND_FILEWRAPPER_H

#include "QueueThreadWrapper.h"
#include <optional>
#include <queue>
#include <string>

enum class EncodingType : uint8_t {
    ASCII = 0,
};

class FileWrapper {
public:
    using PairIndexString = std::pair<uint64_t, std::string>;

    explicit FileWrapper(std::string &path, EncodingType type = EncodingType::ASCII);

    void OutputAllUsageOfString(std::string &substring, std::ostream &os);

private:
    void ReadFromFile();
    void FindAndOutputSubstringInLines(const std::string &substring, std::ostream &os);

    void PushLineToQueue(const uint64_t index, const std::string &str);
    std::optional<PairIndexString> PopLineFromQueue();
    bool QueueIsEmpty() const {return _queueOfLines.IsEmpty();}

    std::string _path;
    EncodingType _encodingType;
    QueueThreadWrapper<PairIndexString> _queueOfLines;
    std::atomic<bool> _firstThreadIsFinished;
};

#endif //MTFIND_FILEWRAPPER_H
