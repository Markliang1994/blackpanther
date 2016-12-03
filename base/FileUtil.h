//
// Created by mark on 16-12-3.
//

#ifndef BLACKPANTHER_BASE_FILEUTIL_H
#define BLACKPANTHER_BASE_FILEUTIL_H

#include <string>
#include "noncopyable.h"

namespace blackpanther{
    namespace FileUtil{
        class ReadSmallFile{
        public:
            ReadSmallFile(const std::string &filename);
            ~ReadSmallFile();

            template <typename T>
            int readToStirng(int maxSize,
                             T *content,
                             int64_t *fileSize,
                             int64_t *modifyTime,
                             int64_t *createTime);

            int readToBuffer(int *size);

            const char *buffer() const { return buf_; }

            static const int kBufferSize = 64*1024;
        private:
            int fd_;
            int err_;
            char buf_[];
        };

        template <typename T>
        int readFile(const std::string &filename,
                    int maxSize,
                    T *content,
                    int64_t *fileSize = nullptr,
                    int64_t *modifyTime = nullptr,
                    int64_t *createTime = nullptr){
            ReadSmallFile file(filename);
            return file.readToStirng(maxSize, content, fileSize, modifyTime, createTime);
        }

        // Watch out! not thread safe.
        class AppendFile : noncopyable{
        public:
            explicit AppendFile(const std::string &filename);
            ~AppendFile();

            void append(const char *logline, size_t len);
            void flush();
            ssize_t writtenBytes(){ return writtenBytes_; }

        private:
            size_t write(const char *logline, size_t len);

            FILE *fp_;
            char buf[64*1024];
            size_t writtenBytes_;
        };
    }
}
#endif //BLACKPANTHER_FILEUTIL_H
