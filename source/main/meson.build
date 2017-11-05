project('xunittest', 'c', 'cpp', default_options : ['c_std=c11', 'cpp_std=c++11'], version : '1.0.0', license : 'MIT')

add_global_arguments('-DMESON_TEST', language : 'cpp')
incdir = include_directories('include/')

sources = [ 'cpp/ut_Utils.cpp', 
            'cpp/ut_TimeConstraint.cpp', 
            'cpp/ut_Thread_Base.cpp', 
            'cpp/ut_TestState.cpp', 
            'cpp/ut_TestRunner.cpp', 
            'cpp/ut_TestResults.cpp', 
            'cpp/ut_TestReporterTeamCity.cpp', 
            'cpp/ut_TestReporterStdout.cpp', 
            'cpp/ut_TestReporter.cpp', 
            'cpp/ut_TestList.cpp', 
            'cpp/ut_Test.cpp', 
            'cpp/ut_StringBuilder.cpp', 
            'cpp/ut_ReportAssert.cpp', 
            'cpp/ut_Checks.cpp', 
            'cpp/ut_AssertException.cpp',
            ]

if host_machine.system() == 'windows'
    sources += [
            'cpp/ut_Utils_Win32.cpp', 
            'cpp/ut_TimeHelpers_Win32.cpp', 
            'cpp/ut_Thread_Win32.cpp', 
            'cpp/ut_Stdout_Win32.cpp', 
            'cpp/ut_Test_Win32.cpp', 
    ]

    add_global_arguments('-DTARGET_PC', language : 'cpp')
    add_global_arguments('-DTARGET_64BIT', language : 'cpp')

elif host_machine.system() == 'darwin'
    sources += [
            'cpp/ut_Utils_OSX.cpp', 
            'cpp/ut_TimeHelpers_OSX.cpp', 
            'cpp/ut_Thread_OSX.cpp', 
            'cpp/ut_Stdout_OSX.cpp', 
            'cpp/ut_Test_OSX.cpp', 
    ]

    add_global_arguments('-DTARGET_MACOS', language : 'cpp')
    add_global_arguments('-DTARGET_64BIT', language : 'cpp')
else

    error('Unknown host machine "' + host_machine.system() + '".')
endif

static_library('xunittest', sources, include_directories: incdir)