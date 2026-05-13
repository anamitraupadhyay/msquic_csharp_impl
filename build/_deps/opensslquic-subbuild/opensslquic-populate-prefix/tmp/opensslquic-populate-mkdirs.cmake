# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/vadmin/Downloads/msquic_csharp_impl/msquic_csharp_impl/msquic/submodules")
  file(MAKE_DIRECTORY "/home/vadmin/Downloads/msquic_csharp_impl/msquic_csharp_impl/msquic/submodules")
endif()
file(MAKE_DIRECTORY
  "/home/vadmin/Downloads/msquic_csharp_impl/build/_deps/opensslquic-build"
  "/home/vadmin/Downloads/msquic_csharp_impl/build/_deps/opensslquic-subbuild/opensslquic-populate-prefix"
  "/home/vadmin/Downloads/msquic_csharp_impl/build/_deps/opensslquic-subbuild/opensslquic-populate-prefix/tmp"
  "/home/vadmin/Downloads/msquic_csharp_impl/build/_deps/opensslquic-subbuild/opensslquic-populate-prefix/src/opensslquic-populate-stamp"
  "/home/vadmin/Downloads/msquic_csharp_impl/build/_deps/opensslquic-subbuild/opensslquic-populate-prefix/src"
  "/home/vadmin/Downloads/msquic_csharp_impl/build/_deps/opensslquic-subbuild/opensslquic-populate-prefix/src/opensslquic-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/vadmin/Downloads/msquic_csharp_impl/build/_deps/opensslquic-subbuild/opensslquic-populate-prefix/src/opensslquic-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/vadmin/Downloads/msquic_csharp_impl/build/_deps/opensslquic-subbuild/opensslquic-populate-prefix/src/opensslquic-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
