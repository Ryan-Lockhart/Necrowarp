# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/ryanj/source/repos/necrowarp/out/build/x86_64-windows-debug/_deps/bleak-src")
  file(MAKE_DIRECTORY "C:/Users/ryanj/source/repos/necrowarp/out/build/x86_64-windows-debug/_deps/bleak-src")
endif()
file(MAKE_DIRECTORY
  "C:/Users/ryanj/source/repos/necrowarp/out/build/x86_64-windows-debug/_deps/bleak-build"
  "C:/Users/ryanj/source/repos/necrowarp/out/build/x86_64-windows-debug/_deps/bleak-subbuild/bleak-populate-prefix"
  "C:/Users/ryanj/source/repos/necrowarp/out/build/x86_64-windows-debug/_deps/bleak-subbuild/bleak-populate-prefix/tmp"
  "C:/Users/ryanj/source/repos/necrowarp/out/build/x86_64-windows-debug/_deps/bleak-subbuild/bleak-populate-prefix/src/bleak-populate-stamp"
  "C:/Users/ryanj/source/repos/necrowarp/out/build/x86_64-windows-debug/_deps/bleak-subbuild/bleak-populate-prefix/src"
  "C:/Users/ryanj/source/repos/necrowarp/out/build/x86_64-windows-debug/_deps/bleak-subbuild/bleak-populate-prefix/src/bleak-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/ryanj/source/repos/necrowarp/out/build/x86_64-windows-debug/_deps/bleak-subbuild/bleak-populate-prefix/src/bleak-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/ryanj/source/repos/necrowarp/out/build/x86_64-windows-debug/_deps/bleak-subbuild/bleak-populate-prefix/src/bleak-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
