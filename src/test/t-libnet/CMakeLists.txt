add_executable(
  t-libnet
  t-libnet.cpp
  TestMacAddress.cpp
)

add_test(
  NAME t-libnet
  COMMAND t-libnet --gtest_output=xml:t-libnet.xml
)

target_link_libraries(
  t-libnet
  net
  gtest
  gtest_main
  pthread
)
