#pragma once

#include <string>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <liburing.h>
#include <exception>
#include <iostream>
#include <format>
#include <array>
#include <bitset>

namespace ltlog{

template<unsigned Entries, unsigned EntrySize>
class IOURing
{
public:

    explicit IOURing(const std::string& file_path): entry_index_(0), buffers_({})
    {
        fd_ = open(file_path.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd_ < 0) {
            std::cerr << std::format("open file {} return {}", file_path, fd_) << std::endl;
            std::terminate();
        }
        int rc = io_uring_queue_init(Entries, &ring_, 0);

        if(rc < 0)
        {
            std::cerr << std::format("io_uring_queue_init return {}", rc) << std::endl;
            close(fd_);
            std::terminate();
        }
    }

    IOURing(const IOURing &) = delete;
    IOURing &operator=(const IOURing &) = delete;
    IOURing(IOURing &&) noexcept = delete;
    IOURing &operator=(IOURing &&) noexcept = delete;

    ~IOURing()
    {
        Submit();
        WaitAll();

        io_uring_queue_exit(&ring_);
        close(fd_);
    }

    void Write(const std::string& message)
    {
        io_uring_sqe* sqe = nullptr;
        while (!(sqe = io_uring_get_sqe(&ring_))) {
            Submit();
            WaitAll();
        }
        
        size_t size = message.size();
        char* buffer = buffers_.data() + entry_index_ ++ * EntrySize;
        std::memcpy(buffer, message.data(), size);

        io_uring_prep_write(sqe, fd_, buffer, size, 0);
    }
private:

    void Submit()
    {
        int rc = io_uring_submit(&ring_);
        if (rc < 0) {
            std::cerr << "io_uring_submit return " << rc << std::endl;
            io_uring_queue_exit(&ring_);
            close(fd_);
        }
    }

    void WaitAll()
    {
        unsigned count = 0;
        while (count < entry_index_)
        {
            count += Peek() ? 1 : 0;
        }
        entry_index_ = 0;
    }

    bool Peek()
    {
        io_uring_cqe *cqe = nullptr;
        int rc = io_uring_peek_cqe(&ring_, &cqe);
        if (rc != 0) {
            return false;
        }

        if (cqe->res < 0) {
            std::cerr << "Write failed: " << strerror(-cqe->res) << rc << std::endl;
        }
        io_uring_cqe_seen(&ring_, cqe);
        return true;
    }

    int fd_;
    io_uring ring_;
    unsigned entry_index_;
    std::array<char, Entries * EntrySize> buffers_;
};
}
