# Copyright (c) 2018 Cisco and/or its affiliates.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at:
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

##############################################################################
# vat plugin shared library
##############################################################################
add_vpp_library(vatplugin
  SOURCES plugin_api.c
  LINK_LIBRARIES vppinfra
)

##############################################################################
# vpp_api_test
##############################################################################
add_vpp_executable(vpp_api_test ENABLE_EXPORTS
  SOURCES
  api_format.c
  main.c
  plugin.c
  json_format.c
  types.c
  ip_types_api.c
  ip_types.c
  protocols.def

  DEPENDS api_headers

  LINK_LIBRARIES
  vlibmemoryclient
  svm
  vatplugin
  vppinfra
  Threads::Threads
  dl
)

##############################################################################
#  vpp_json_test
##############################################################################
add_vpp_executable(vpp_json_test ENABLE_EXPORTS NO_INSTALL
  SOURCES json_format.c json_test.c
  LINK_LIBRARIES vppinfra m
)

##############################################################################
# vat headers
##############################################################################
install(
  FILES vat.h json_format.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vat
  COMPONENT vpp-dev
)

##############################################################################
# restart
##############################################################################
add_vpp_executable(vpp_restart
  SOURCES restart.c
  LINK_LIBRARIES svm svmdb vppinfra Threads::Threads rt
)
