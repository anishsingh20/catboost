#pragma once

#include "input.h"
#include "output.h"

#include <util/generic/utility.h>

/**
 * Proxy input stream that can read a limited number of characters from a slave
 * stream.
 *
 * This can be useful for breaking up the slave stream into small chunks and
 * treat these as separate streams.
 */
class TLengthLimitedInput: public TInputStream {
public:
    inline TLengthLimitedInput(TInputStream* slave, ui64 length) noexcept
        : Slave_(slave)
        , Length_(length)
    {
    }

    ~TLengthLimitedInput() override = default;

    inline ui64 Left() const noexcept {
        return Length_;
    }

private:
    size_t DoRead(void* buf, size_t len) override;
    size_t DoSkip(size_t len) override;

private:
    TInputStream* Slave_;
    ui64 Length_;
};

/**
 * Proxy input stream that counts the number of characters read.
 */
class TCountingInput: public TInputStream {
public:
    inline TCountingInput(TInputStream* slave) noexcept
        : Slave_(slave)
        , Count_()
    {
    }

    ~TCountingInput() override = default;

    /**
     * \returns                         The total number of characters read from
     *                                  this stream.
     */
    inline ui64 Counter() const noexcept {
        return Count_;
    }

private:
    size_t DoRead(void* buf, size_t len) override;
    size_t DoSkip(size_t len) override;
    size_t DoReadTo(TString& st, char ch) override;
    ui64 DoReadAll(TOutputStream& out) override;

private:
    TInputStream* Slave_;
    ui64 Count_;
};

/**
 * Proxy output stream that counts the number of characters written.
 */
class TCountingOutput: public TOutputStream {
public:
    inline TCountingOutput(TOutputStream* slave) noexcept
        : Slave_(slave)
        , Count_()
    {
    }

    ~TCountingOutput() override = default;

    TCountingOutput(TCountingOutput&&) noexcept = default;
    TCountingOutput& operator=(TCountingOutput&&) noexcept = default;

    /**
     * \returns                         The total number of characters written
     *                                  into this stream.
     */
    inline ui64 Counter() const noexcept {
        return Count_;
    }

private:
    void DoWrite(const void* buf, size_t len) override;

private:
    TOutputStream* Slave_;
    ui64 Count_;
};
