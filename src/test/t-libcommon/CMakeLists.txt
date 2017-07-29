add_executable(
  t-libcommon
  t-libcommon.cpp
  TestEvent.cpp
)

add_test(
  NAME t-libcommon
  COMMAND t-libcommon --gtest_output=xml:t-libcommon.xml
)

target_link_libraries(
  t-libcommon
  common
  gtest
  gtest_main
  pthread
)
