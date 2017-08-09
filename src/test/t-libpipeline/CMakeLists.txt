add_executable(
  t-libpipeline
  t-libpipeline.cpp
  TestPipelineManager.cpp
)

add_test(
  NAME t-libpipeline
  COMMAND t-libpipeline --gtest_output=xml:t-libpipeline.xml
)

target_link_libraries(
  t-libpipeline
  pipeline
  buffer
  common
  gtest
  gtest_main
  pthread
)
