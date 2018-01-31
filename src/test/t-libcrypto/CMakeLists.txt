add_executable(
  t-libcrypto
  t-libcrypto.cpp
  TestHash.cpp
)

add_test(
  NAME t-libcrypto
  COMMAND t-libcrypto --gtest_output=xml:t-libcrypto.xml
)

target_link_libraries(
  t-libcrypto
  crypt
  gtest
  gtest_main
  pthread
)
