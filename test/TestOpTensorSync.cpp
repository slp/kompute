
#include "gtest/gtest.h"

#include "kompute/Kompute.hpp"

TEST(TestOpTensorSync, SyncToDeviceMemorySingleTensor)
{

    kp::Manager mgr;

    std::vector<float> testVecPreA{ 0, 0, 0 };
    std::vector<float> testVecPostA{ 9, 8, 7 };

    std::shared_ptr<kp::Tensor> tensorA = mgr.tensor(testVecPreA);

    EXPECT_TRUE(tensorA->isInit());

    tensorA->setData(testVecPostA);

    mgr.sequence()->eval<kp::OpTensorSyncDevice>({ tensorA });

    mgr.sequence()->eval<kp::OpTensorSyncLocal>({ tensorA });

    EXPECT_EQ(tensorA->data(), testVecPostA);
}

TEST(TestOpTensorSync, SyncToDeviceMemoryMultiTensor)
{

    kp::Manager mgr;

    std::vector<float> testVec{ 9, 8, 7 };

    std::shared_ptr<kp::Tensor> tensorA = mgr.tensor({ 0, 0, 0 });
    std::shared_ptr<kp::Tensor> tensorB = mgr.tensor({ 0, 0, 0 });
    std::shared_ptr<kp::Tensor> tensorC = mgr.tensor({ 0, 0, 0 });

    EXPECT_TRUE(tensorA->isInit());
    EXPECT_TRUE(tensorB->isInit());
    EXPECT_TRUE(tensorC->isInit());

    tensorA->setData(testVec);

    mgr.sequence()->eval<kp::OpTensorSyncDevice>({ tensorA });

    mgr.sequence()->eval<kp::OpTensorCopy>({ tensorA, tensorB, tensorC });

    mgr.sequence()->eval<kp::OpTensorSyncLocal>({ tensorA, tensorB, tensorC });

    EXPECT_EQ(tensorA->data(), testVec);
    EXPECT_EQ(tensorB->data(), testVec);
    EXPECT_EQ(tensorC->data(), testVec);
}
