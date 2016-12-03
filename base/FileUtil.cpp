//
// Created by mark on 16-12-3.
//

#include <blackpanther/base/FileUtil.h>
#include <blackpanther/base/Logging.h>

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <boost/implicit_cast.hpp>

using namespace blackpanther;

FileUtil::AppendFile::AppendFile(const std::string &filename):
        fp_(::fopen(filename.c_str(), "ae")){
    assert(fp_);
    ::setbuffer(fp_, buffer_, sizeof(buffer_));
}

FileUtil::AppendFile::~AppendFile() {
    ::fclose(fp_);
}

void FileUtil::AppendFile::append(const char *logline, const size_t len){
    size_t n = write(logline, len);
    size_t remain = len - n;

    while(remain > 0){
        size_t x = write(logline+n, remain);
        if(x == 0){
            int err = ferror(fp_);
            if(err){
                fprintf(stderr, "AppendFile::append() failed %s\n", strerror_tl(err));
            }
            break;
        }
        n += x;
        remain = len -n;
    }
    writtenBytes_ += n;
}

void FileUtil::AppendFile::flush(){
    ::fflush(fp_);
}

size_t FileUtil::AppendFile::write(const char *logline, size_t len){
    return fwrite_unlocked(logline, 1, len, fp_);
}

FileUtil::ReadSmallFile::ReadSmallFile(const std::string &filename)
    :fd_(::open(filename.c_str(), O_RDONLY | O_CLOEXEC)),
    err_(0){
    buf_[0] = '\0';
    if(fd_ < 0)
        err_ = errno;
}

FileUtil::ReadSmallFile::~ReadSmallFile() {
    if(fd_ >= 0)
        ::close(fd_);
}

template <typename T>
int FileUtil::ReadSmallFile::readToStirng(int maxSize, T *content, int64_t *fileSize, int64_t *modifyTime,
                                          int64_t *createTime) {
    static_assert(sizeof(off_t) == 8, "_FILE_OFFSET_BITS = 64");
    int err = err_;
    assert(content != nullptr);
    if(fd_ > 0){
        content->clear();
        if(fileSize){
            struct stat statbuf;
            if(::fstat(fd_, &statbuf) == 0){
                if(S_ISREG(statbuf.st_mode)){
                    *fileSize = statbuf.st_size;
                    content->reserve(static_cast<int>(std::min(boost::implicit_cast<int64_t>(maxSize), *fileSize)));
                }
                else if(S_ISDIR(statbuf.st_mode)){
                    err = EISDIR;
                }
                if(modifyTime){
                    *modifyTime = statbuf.st_mtime;
                }
                if(createTime){
                    *createTime = statbuf.st_ctime;
                }
            }
            else{
                err = errno;
            }
        }
        while(true){

        }
    }
}