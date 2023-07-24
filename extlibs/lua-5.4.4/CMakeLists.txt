cmake_minimum_required(VERSION 3.10)
include(ExternalProject)

ExternalProject_Add(luat
        URL "https://www.lua.org/ftp/lua-5.4.4.tar.gz"
        CONFIGURE_COMMAND ""
        BUILD_COMMAND make generic
        BUILD_ALWAYS true
        BUILD_IN_SOURCE true
        INSTALL_COMMAND ""
        DOWNLOAD_EXTRACT_TIMESTAMP true
        )

add_library(liblua STATIC IMPORTED)
ExternalProject_Get_property(luat SOURCE_DIR)
message("liblua will be found at \'${SOURCE_DIR}/src\'")