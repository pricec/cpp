add_executable(
  t-libbuffer
  t-libbuffer.cpp
  TestBuffer.cpp
  TestLocklessRingBuffer.cpp
)

add_test(
  NAME t-libbuffer
  COMMAND t-libbuffer --gtest_output=xml:t-libmessage.xml
)

target_link_libraries(
  t-libbuffer
  buffer
  gtest
  gtest_main
  pthread
)
