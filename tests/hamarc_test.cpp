#include <libs/HamArc/HamArc.h>
#include <gtest/gtest.h>

TEST(HammingCodeTestSuite, EncodeTest1) {
    std::vector<bool> data = {1, 0, 1, 1, 0, 1, 1, 0};
    std::vector<bool> ans = {1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0};
    std::vector<bool> hamming_code = HammingCode::EncodeToHammingCode(data);

    for (size_t i = 0; i < ans.size(); ++i) {
        ASSERT_EQ(ans[i], hamming_code[i]);
    }
}

TEST(HammingCodeTestSuite, EncodeTest2) {
    std::vector<bool> data = {1, 1, 0, 0, 0, 1, 1, 0};
    std::vector<bool> ans = {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0};
    std::vector<bool> hamming_code = HammingCode::EncodeToHammingCode(data);

    for (size_t i = 0; i < ans.size(); ++i) {
        ASSERT_EQ(ans[i], hamming_code[i]);
    }
}

TEST(HammingCodeTestSuite, DecodeTest1) {
    std::vector<bool> data = {1, 0, 1, 1, 0, 1, 1, 0};
    std::vector<bool> hamming_code = HammingCode::EncodeToHammingCode(data);
    std::optional<std::vector<bool>> decoded_data = HammingCode::DecodeToData(hamming_code);

    for (size_t i = 0; i < data.size(); ++i) {
        ASSERT_EQ(data[i], (*decoded_data)[i]);
    }
}

TEST(HammingCodeTestSuite, DecodeTest2) {
    std::vector<bool> data = {1, 1, 0, 0, 0, 1, 1, 0};
    std::vector<bool> hamming_code = HammingCode::EncodeToHammingCode(data);
    std::optional<std::vector<bool>> decoded_data = HammingCode::DecodeToData(hamming_code);

    for (size_t i = 0; i < data.size(); ++i) {
        ASSERT_EQ(data[i], (*decoded_data)[i]);
    }
}

TEST(HammingCodeTestSuite, DecodeTest3) {
    std::vector<bool> data = {0, 0, 0, 1, 1, 1, 0, 0};
    std::vector<bool> hamming_code = HammingCode::EncodeToHammingCode(data);
    std::optional<std::vector<bool>> decoded_data = HammingCode::DecodeToData(hamming_code);

    for (size_t i = 0; i < data.size(); ++i) {
        ASSERT_EQ(data[i], (*decoded_data)[i]);
    }
}

TEST(HammingCodeTestSuite, OneErrorTest1) {
    std::vector<bool> data = {1, 0, 1, 1, 0, 1, 1, 0};
    std::vector<bool> hamming_code = HammingCode::EncodeToHammingCode(data);
    std::optional<std::vector<bool>> decoded_data;

    for (size_t i = 0; i < hamming_code.size(); ++i) {
        hamming_code[i] = !hamming_code[i];
        decoded_data = HammingCode::DecodeToData(hamming_code);
        ASSERT_NE(decoded_data, std::nullopt);
        for (size_t j = 0; j < data.size(); ++j) {
            ASSERT_EQ(data[j], (*decoded_data)[j]);
        }
        (*decoded_data).clear();
    }
}

TEST(HammingCodeTestSuite, OneErrorTest2) {
    std::vector<bool> data = {1, 1, 0, 0, 0, 1, 1, 0};
    std::vector<bool> hamming_code = HammingCode::EncodeToHammingCode(data);
    std::optional<std::vector<bool>> decoded_data;

    for (size_t i = 0; i < hamming_code.size(); ++i) {
        hamming_code[i] = !hamming_code[i];
        decoded_data = HammingCode::DecodeToData(hamming_code);
        ASSERT_NE(decoded_data, std::nullopt);
        for (size_t j = 0; j < data.size(); ++j) {
            ASSERT_EQ(data[j], (*decoded_data)[j]);
        }
        (*decoded_data).clear();
    }
}

TEST(HammingCodeTestSuite, TwoErrorsTest1) {
    std::vector<bool> data = {1, 0, 1, 1, 0, 1, 1, 0};
    std::vector<bool> hamming_code = HammingCode::EncodeToHammingCode(data);

    for (size_t i = 0; i < hamming_code.size(); ++i) {
        hamming_code[i] = !hamming_code[i];
        for (size_t j = i + 1; j < hamming_code.size(); ++j) {
            hamming_code[j] = !hamming_code[j];
            std::optional<std::vector<bool>> decoded_data = HammingCode::DecodeToData(hamming_code);
            ASSERT_EQ(decoded_data, std::nullopt);
            hamming_code[j] = !hamming_code[j];
        }
        hamming_code[i] = !hamming_code[i];
    }
}

TEST(HammingCodeTestSuite, TwoErrorsTest2) {
    std::vector<bool> data = {1, 1, 0, 0, 0, 1, 1, 0};
    std::vector<bool> hamming_code = HammingCode::EncodeToHammingCode(data);

    for (size_t i = 0; i < hamming_code.size(); ++i) {
        hamming_code[i] = !hamming_code[i];
        for (size_t j = i + 1; j < hamming_code.size(); ++j) {
            hamming_code[j] = !hamming_code[j];
            std::optional<std::vector<bool>> decoded_data = HammingCode::DecodeToData(hamming_code);
            ASSERT_EQ(decoded_data, std::nullopt);
            hamming_code[j] = !hamming_code[j];
        }
        hamming_code[i] = !hamming_code[i];
    }
}