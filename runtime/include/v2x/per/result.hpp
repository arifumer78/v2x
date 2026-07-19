#pragma once

#include <cassert>
#include <optional>
#include <type_traits>
#include <utility>

#include "v2x/per/error.hpp"

namespace v2x::per {

// Hand-rolled replacement for std::expected<T, E> — deliberately avoids depending
// on a C++23 stdlib feature with no published MISRA C++:2023 guidance (see
// docs/design/per-runtime-design.md §7). Also avoids std::variant: every T that
// flows through this kernel is small and trivially-copyable, so a manual tagged
// union costs little and removes one more third-party-qualification surface.
template <typename T, typename E = Error>
class Result {
public:
    static Result Ok(T value) {
        Result r(ok_tag{});
        ::new (static_cast<void*>(&r.storage_.value_)) T(std::move(value));
        return r;
    }

    static Result Err(E error) {
        Result r(err_tag{});
        ::new (static_cast<void*>(&r.storage_.error_)) E(std::move(error));
        return r;
    }

    Result(const Result& other) : ok_(other.ok_) {
        if (ok_) {
            ::new (static_cast<void*>(&storage_.value_)) T(other.storage_.value_);
        } else {
            ::new (static_cast<void*>(&storage_.error_)) E(other.storage_.error_);
        }
    }

    Result(Result&& other) noexcept(std::is_nothrow_move_constructible_v<T> &&
                                     std::is_nothrow_move_constructible_v<E>)
        : ok_(other.ok_) {
        if (ok_) {
            ::new (static_cast<void*>(&storage_.value_)) T(std::move(other.storage_.value_));
        } else {
            ::new (static_cast<void*>(&storage_.error_)) E(std::move(other.storage_.error_));
        }
    }

    Result& operator=(const Result& other) {
        if (this != &other) {
            destroy();
            ok_ = other.ok_;
            if (ok_) {
                ::new (static_cast<void*>(&storage_.value_)) T(other.storage_.value_);
            } else {
                ::new (static_cast<void*>(&storage_.error_)) E(other.storage_.error_);
            }
        }
        return *this;
    }

    Result& operator=(Result&& other) noexcept(std::is_nothrow_move_constructible_v<T> &&
                                                std::is_nothrow_move_constructible_v<E>) {
        if (this != &other) {
            destroy();
            ok_ = other.ok_;
            if (ok_) {
                ::new (static_cast<void*>(&storage_.value_)) T(std::move(other.storage_.value_));
            } else {
                ::new (static_cast<void*>(&storage_.error_)) E(std::move(other.storage_.error_));
            }
        }
        return *this;
    }

    ~Result() { destroy(); }

    [[nodiscard]] bool ok() const { return ok_; }
    explicit operator bool() const { return ok_; }

    // Precondition: ok(). Asserted in debug; UB in release per the no-exceptions posture.
    const T& value() const& {
        assert(ok_);
        return storage_.value_;
    }
    T&& value() && {
        assert(ok_);
        return std::move(storage_.value_);
    }

    // Precondition: !ok().
    const E& error() const& {
        assert(!ok_);
        return storage_.error_;
    }

private:
    struct ok_tag {};
    struct err_tag {};

    explicit Result(ok_tag) : ok_(true) {}
    explicit Result(err_tag) : ok_(false) {}

    void destroy() {
        if (ok_) {
            storage_.value_.~T();
        } else {
            storage_.error_.~E();
        }
    }

    union Storage {
        Storage() {}
        ~Storage() {}
        T value_;
        E error_;
    };

    bool ok_;
    Storage storage_;
};

// Void-returning equivalent of Result, for primitives with no output value.
template <typename E = Error>
class Status {
public:
    static Status Ok() { return Status(std::optional<E>{}); }
    static Status Err(E error) { return Status(std::optional<E>(std::move(error))); }

    [[nodiscard]] bool ok() const { return !error_.has_value(); }
    explicit operator bool() const { return ok(); }

    // Precondition: !ok().
    const E& error() const& {
        assert(!ok());
        return *error_;
    }

private:
    explicit Status(std::optional<E> error) : error_(std::move(error)) {}
    std::optional<E> error_;
};

} // namespace v2x::per
