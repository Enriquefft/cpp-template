add_test([=[SampleTest.sample1]=]  /home/enrique/Documents/EDA/KD-Tree/build/tests/sampleTest [==[--gtest_filter=SampleTest.sample1]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[SampleTest.sample1]=]  PROPERTIES WORKING_DIRECTORY /home/enrique/Documents/EDA/KD-Tree/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] VS_DEBUGGER_WORKING_DIRECTORY)
set(  sampleTest_TESTS SampleTest.sample1)
