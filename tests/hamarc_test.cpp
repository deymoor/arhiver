//#include <lib/Array17b.h>
//#include <gtest/gtest.h>
//#include <sstream>
//
//
//using namespace Array17bits;
//
//
//TEST(Array17bTestSuite, OneValueTest) {
//    Array17b array = Array17b::MakeArray(1, 1, 1);
//    array[0][0][0] = 993;
//
//    ASSERT_EQ(array[0][0][0], 993);
//}
//
//TEST(Array17bTestSuite, TwoValueTest) {
//    Array17b array = Array17b::MakeArray(1, 2, 1);
//    array[0][1][0] = 993;
//
//    ASSERT_EQ(array[0][1][0], 993);
//}
//
//TEST(Array17bTestSuite, ThreeValueTest) {
//    Array17b array = Array17b::MakeArray(2, 1, 1);
//    array[1][0][0] = 993;
//
//    ASSERT_EQ(array[1][0][0], 993);
//}
//
//TEST(Array17bTestSuite, MultiValueTest) {
//    Array17b array = Array17b::MakeArray(2, 2, 2);
//    array[0][0][0] = 993;
//    array[0][0][1] = 994;
//    array[0][1][0] = 995;
//    array[0][1][1] = 996;
//
//    ASSERT_EQ(array[0][0][0], 993);
//    ASSERT_EQ(array[0][0][1], 994);
//    ASSERT_EQ(array[0][1][0], 995);
//    ASSERT_EQ(array[0][1][1], 996);
//}
//
//TEST(Array17bTestSuite, FillFullArrayTest) {
//    Array17b array = Array17b::MakeArray(2, 2, 1);
//    array[0][0][0] = 993;
//    array[0][1][0] = 994;
//    array[1][0][0] = 995;
//    array[1][1][0] = 996;
//
//    ASSERT_EQ(array[0][0][0], 993);
//    ASSERT_EQ(array[0][1][0], 994);
//    ASSERT_EQ(array[1][0][0], 995);
//    ASSERT_EQ(array[1][1][0], 996);
//}
//
//TEST(Array17bTestSuite, ReplacementValueTest) {
//    Array17b array = Array17b::MakeArray(1, 1, 1);
//    array[0][0][0] = 993;
//    array[0][0][0] = 1000;
//
//    ASSERT_EQ(array[0][0][0], 1000);
//}
//
//TEST(Array17bTestSuite, ZeroValueTest) {
//    Array17b array = Array17b::MakeArray(1, 1, 1);
//    array[0][0][0] = 0;
//
//    ASSERT_EQ(array[0][0][0], 0);
//}
//
//TEST(Array17bTestSuite, NegativeValueTest) {
//    Array17b array = Array17b::MakeArray(1, 1, 1);
//    array[0][0][0] = -1;
//
//    ASSERT_NE(array[0][0][0], -1);
//}
//
//TEST(Array17bTestSuite, OverflowValueTest) {
//    Array17b array = Array17b::MakeArray(1, 1, 1);
//    array[0][0][0] = 131072;
//
//    ASSERT_EQ(array[0][0][0], 0);
//}
//
//TEST(Array17bTestSuite, AlmostOverflowValueTest) {
//    Array17b array = Array17b::MakeArray(1, 1, 1);
//    array[0][0][0] = 131071;
//
//    ASSERT_EQ(array[0][0][0], 131071);
//}
//
//TEST(Array17bTestSuite, VariableTest) {
//    Array17b array = Array17b::MakeArray(1, 1, 1);
//    array[0][0][0] = 993;
//    uint32_t k = array[0][0][0];
//
//    ASSERT_EQ(k, 993);
//}
//
//TEST(Array17bTestSuite, NegativeVariableTest) {
//    Array17b array = Array17b::MakeArray(1, 1, 1);
//    array[0][0][0] = -1;
//    int32_t k = array[0][0][0];
//
//    ASSERT_NE(k, 1);
//}
//
//TEST(Array17bTestSuite, InputTest) {
//    Array17b array = Array17b::MakeArray(1, 1, 1);
//    std::stringstream input;
//    input << 100 << ' ' << 200;
//    input >> array;
//
//    ASSERT_EQ(array[0][0][0], 100);
//}
//
//TEST(Array17bTestSuite, MultiValueInputTest) {
//    Array17b array = Array17b::MakeArray(1, 2, 1);
//    std::stringstream input;
//    input << 100 << ' ' << 200;
//    input >> array;
//
//    ASSERT_EQ(array[0][0][0], 100);
//    ASSERT_EQ(array[0][1][0], 200);
//}
//
//TEST(Array17bTestSuite, MultiInputTest) {
//    Array17b first_array = Array17b::MakeArray(1, 1, 1);
//    Array17b second_array = Array17b::MakeArray(1, 1, 1);
//    std::stringstream input;
//    input << 100 << ' ' << 200;
//    input >> first_array >> second_array;
//
//    ASSERT_EQ(first_array[0][0][0], 100);
//    ASSERT_EQ(second_array[0][0][0], 200);
//}
//
//TEST(Array17bTestSuite, CheckSizeBoundariesTest) {
//    Array17b array = Array17b::MakeArray(10, 5, 37);
//    ASSERT_EQ(array.GetFirstBoundary(), 10);
//    ASSERT_EQ(array.GetSecondBoundary(), 5);
//    ASSERT_EQ(array.GetThirdBoundary(), 37);
//}
//
//TEST(Array17bTestSuite, SetTest) {
//    Array17b array = Array17b::MakeArray(2, 2, 2);
//    array.FillArray(10);
//
//    for (size_t i = 0; i < array.GetFirstBoundary(); ++i) {
//        for (size_t j = 0; j < array.GetSecondBoundary(); ++j) {
//            for (size_t k = 0; k < array.GetThirdBoundary(); ++k) {
//                ASSERT_EQ(array[i][j][k], 10);
//            }
//        }
//    }
//}
//
//TEST(Array17bTestSuite, ReplacementSetTest) {
//    Array17b array = Array17b::MakeArray(2, 2, 2);
//    array.FillArray(10);
//    array.FillArray(50);
//
//    for (size_t i = 0; i < array.GetFirstBoundary(); ++i) {
//        for (size_t j = 0; j < array.GetSecondBoundary(); ++j) {
//            for (size_t k = 0; k < array.GetThirdBoundary(); ++k) {
//                ASSERT_EQ(array[i][j][k], 50);
//            }
//        }
//    }
//}
//
//TEST(Array17bTestSuite, AddTest) {
//    Array17b first_array = Array17b::MakeArray(2, 2, 2);
//    Array17b second_array = Array17b::MakeArray(2, 2, 2);
//    first_array.FillArray(10);
//    second_array.FillArray(20);
//    Array17b array = first_array + second_array;
//
//    for (size_t i = 0; i < array.GetFirstBoundary(); ++i) {
//        for (size_t j = 0; j < array.GetSecondBoundary(); ++j) {
//            for (size_t k = 0; k < array.GetThirdBoundary(); ++k) {
//                ASSERT_EQ(array[i][j][k], 30);
//            }
//        }
//    }
//}
//
//TEST(Array17bTestSuite, AddOverflowTest) {
//    Array17b first_array = Array17b::MakeArray(2, 2, 2);
//    Array17b second_array = Array17b::MakeArray(2, 2, 2);
//    first_array.FillArray(131071);
//    second_array.FillArray(1);
//    Array17b array = first_array + second_array;
//
//    for (size_t i = 0; i < array.GetFirstBoundary(); ++i) {
//        for (size_t j = 0; j < array.GetSecondBoundary(); ++j) {
//            for (size_t k = 0; k < array.GetThirdBoundary(); ++k) {
//                ASSERT_NE(array[i][j][k], 131072);
//            }
//        }
//    }
//}
//
//TEST(Array17bTestSuite, AddDifferentBoundariesTest) {
//    Array17b first_array = Array17b::MakeArray(2, 1, 2);
//    Array17b second_array = Array17b::MakeArray(2, 2, 3);
//    first_array.FillArray(10);
//    second_array.FillArray(20);
//    ASSERT_DEATH({ first_array + second_array; }, "");
//}
//
//TEST(Array17bTestSuite, SubTest) {
//    Array17b first_array = Array17b::MakeArray(2, 2, 2);
//    Array17b second_array = Array17b::MakeArray(2, 2, 2);
//    first_array.FillArray(20);
//    second_array.FillArray(5);
//    Array17b array = first_array - second_array;
//
//    for (size_t i = 0; i < array.GetFirstBoundary(); ++i) {
//        for (size_t j = 0; j < array.GetSecondBoundary(); ++j) {
//            for (size_t k = 0; k < array.GetThirdBoundary(); ++k) {
//                ASSERT_EQ(array[i][j][k], 15);
//            }
//        }
//    }
//}
//
//TEST(Array17bTestSuite, SubOverflowTest) {
//    Array17b first_array = Array17b::MakeArray(2, 2, 2);
//    Array17b second_array = Array17b::MakeArray(2, 2, 2);
//    first_array.FillArray(15);
//    second_array.FillArray(16);
//    Array17b array = first_array - second_array;
//
//    for (size_t i = 0; i < array.GetFirstBoundary(); ++i) {
//        for (size_t j = 0; j < array.GetSecondBoundary(); ++j) {
//            for (size_t k = 0; k < array.GetThirdBoundary(); ++k) {
//                ASSERT_NE(array[i][j][k], -1);
//            }
//        }
//    }
//}
//
//TEST(Array17bTestSuite, SubDifferentBoundariesTest) {
//    Array17b first_array = Array17b::MakeArray(2, 1, 2);
//    Array17b second_array = Array17b::MakeArray(2, 2, 3);
//    first_array.FillArray(10);
//    second_array.FillArray(20);
//    ASSERT_DEATH({ first_array - second_array; }, "");
//}
//
//TEST(Array17bTestSuite, MultiplyTest) {
//    Array17b first_array = Array17b::MakeArray(2, 2, 2);
//    first_array.FillArray(10);
//    Array17b array = first_array * 5;
//
//    for (size_t i = 0; i < array.GetFirstBoundary(); ++i) {
//        for (size_t j = 0; j < array.GetSecondBoundary(); ++j) {
//            for (size_t k = 0; k < array.GetThirdBoundary(); ++k) {
//                ASSERT_EQ(array[i][j][k], 50);
//            }
//        }
//    }
//}
//
//TEST(Array17bTestSuite, MultiplyNegativeTest) {
//    Array17b first_array = Array17b::MakeArray(2, 2, 2);
//    first_array.FillArray(10);
//    Array17b array = first_array * -1;
//
//    for (size_t i = 0; i < array.GetFirstBoundary(); ++i) {
//        for (size_t j = 0; j < array.GetSecondBoundary(); ++j) {
//            for (size_t k = 0; k < array.GetThirdBoundary(); ++k) {
//                ASSERT_NE(array[i][j][k], -10);
//            }
//        }
//    }
//}
//
//TEST(Array17bTestSuite, MultiplyOverflowTest) {
//    Array17b first_array = Array17b::MakeArray(2, 2, 2);
//    first_array.FillArray(10);
//    Array17b array = first_array * 100000;
//
//    for (size_t i = 0; i < array.GetFirstBoundary(); ++i) {
//        for (size_t j = 0; j < array.GetSecondBoundary(); ++j) {
//            for (size_t k = 0; k < array.GetThirdBoundary(); ++k) {
//                ASSERT_NE(array[i][j][k], 1000000);
//            }
//        }
//    }
//}
//
//TEST(Array17bTestSuite, OutputTest) {
//    Array17b array = Array17b::MakeArray(2, 2, 2);
//    array.FillArray(10);
//    std::stringstream output;
//    output << array;
//    ASSERT_EQ(output.str(),
//              "10 10\n"
//              "10 10"
//              "\n"
//              "\n"
//              "10 10\n"
//              "10 10"
//    );
//}
//
//TEST(Array17bTestSuite, MultiOutputTest) {
//    Array17b first_array = Array17b::MakeArray(2, 2, 2);
//    Array17b second_array = Array17b::MakeArray(2, 2, 2);
//    first_array.FillArray(10);
//    second_array.FillArray(20);
//    std::stringstream output;
//    output << first_array << '\n' << second_array;
//    ASSERT_EQ(output.str(),
//              "10 10\n"
//              "10 10"
//              "\n"
//              "\n"
//              "10 10\n"
//              "10 10"
//              "\n"
//              "20 20\n"
//              "20 20"
//              "\n"
//              "\n"
//              "20 20\n"
//              "20 20"
//    );
//}
//
//TEST(Array17bTestSuite, CopyTest) {
//    Array17b array = Array17b::MakeArray(2, 2, 2);
//    array.FillArray(10);
//    Array17b copied_array = array;
//
//    for (size_t i = 0; i < copied_array.GetFirstBoundary(); ++i) {
//        for (size_t j = 0; j < copied_array.GetSecondBoundary(); ++j) {
//            for (size_t k = 0; k < copied_array.GetThirdBoundary(); ++k) {
//                ASSERT_EQ(copied_array[i][j][k], 10);
//            }
//        }
//    }
//}
//
//TEST(Array17bTestSuite, CopyReplacementTest) {
//    Array17b array = Array17b::MakeArray(2, 2, 2);
//    array.FillArray(10);
//    Array17b copied_array = array;
//    copied_array.FillArray(5);
//
//    for (size_t i = 0; i < array.GetFirstBoundary(); ++i) {
//        for (size_t j = 0; j < array.GetSecondBoundary(); ++j) {
//            for (size_t k = 0; k < array.GetThirdBoundary(); ++k) {
//                ASSERT_EQ(array[i][j][k], 10);
//                ASSERT_EQ(copied_array[i][j][k], 5);
//            }
//        }
//    }
//}
//
//TEST(Array17bTestSuite, AssignmentTest) {
//    Array17b array = Array17b::MakeArray(2, 2, 2);
//    array.FillArray(10);
//    Array17b copied_array;
//    copied_array = array;
//
//    for (size_t i = 0; i < copied_array.GetFirstBoundary(); ++i) {
//        for (size_t j = 0; j < copied_array.GetSecondBoundary(); ++j) {
//            for (size_t k = 0; k < copied_array.GetThirdBoundary(); ++k) {
//                ASSERT_EQ(copied_array[i][j][k], 10);
//            }
//        }
//    }
//}
//
//TEST(Array17bTestSuite, AssignmentReplacementTest) {
//    Array17b array = Array17b::MakeArray(2, 2, 2);
//    array.FillArray(10);
//    Array17b copied_array;
//    copied_array = array;
//    copied_array.FillArray(5);
//
//    for (size_t i = 0; i < copied_array.GetFirstBoundary(); ++i) {
//        for (size_t j = 0; j < copied_array.GetSecondBoundary(); ++j) {
//            for (size_t k = 0; k < copied_array.GetThirdBoundary(); ++k) {
//                ASSERT_EQ(array[i][j][k], 10);
//                ASSERT_EQ(copied_array[i][j][k], 5);
//            }
//        }
//    }
//}