add_library(perfect INTERFACE)

target_sources(perfect
  INTERFACE
    FILE_SET HEADERS
    FILES
    perfect.hpp
)
