/* Copyright (C) Marco Heisig 2014 - GNU GPLv3 or later */

#define DEFINE_EXCEPTION(name, msg)                                     \
    struct name : std::exception {                                      \
        const char* what() const noexcept {                             \
            return msg;                                                 \
        }                                                               \
    }

#define THROW_EXCEPTION(msg)                    \
    do {                                        \
        DEFINE_EXCEPTION(oops, msg);            \
        throw oops();                           \
    } while(0)
