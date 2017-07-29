add_executable(
  t-libmessage
  t-libmessage.cpp
  TestMessage.cpp
)

add_test(
  NAME t-libmessage
  COMMAND t-libmessage --gtest_output=xml:t-libmessage.xml
)

target_link_libraries(
  t-libmessage
  message
  gtest
  gtest_main
  pthread
)
