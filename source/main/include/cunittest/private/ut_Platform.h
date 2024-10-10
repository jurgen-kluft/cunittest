#ifndef __CUNITTEST_DETECT_PLATFORM_H__
#define __CUNITTEST_DETECT_PLATFORM_H__

namespace UnitTest
{
#if !defined(TARGET_PC) && !defined(TARGET_MAC)
#    if defined(_WIN32)
#        define TARGET_PC 1
#        define TARGET_OS_WINDOWS 1
#        ifndef NOMINMAX
#            define NOMINMAX
#        endif
#        if defined(__MINGW32__)
#            define TARGET_OS_MINGW 1
#        endif
#    elif defined(__APPLE__)
#        define TARGET_OS_APPLE 1
#        include "TargetConditionals.h"
#        if defined(TARGET_OS_MAC)
#            define TARGET_MAC 1
#            define TARGET_OS_MACOSX 1
#            if defined(TARGET_OS_IPHONE)
#                define TARGET_OS_IOS 1
#            endif
#        endif
#    endif
#elif defined(__unix__)
#    define TARGET_LINUX 1
#    define TARGET_OS_LINUX 1
#endif
} // namespace UnitTest

#endif ///< __CUNITTEST_DETECT_PLATFORM_H__
