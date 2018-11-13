#include "tree.h"


TEST(TestDelete, leftLeft)
{
	Tree tree;
	tree.insert(44);
	tree.insert(17);
	tree.insert(62);
	tree.insert(32);
	tree.insert(50);
	tree.insert(78);
	tree.insert(48);
	tree.insert(54);
	tree.insert(88);

	tree.traverse(true);
	tree.remove(32);

	testing::internal::CaptureStdout();
	tree.traverse(true);
	EXPECT_EQ("3 (0) 4 (1) 5 (2) 6 (0) 10 (1) 11 (0) 13 (3) 15 (1) 16 (0) \n", 
			testing::internal::GetCapturedStdout());
}

// ---------------------------------------------------------------------------
/*
TEST(TestDelete, rightRight)
{
	Tree tree;
	tree.insert(30);
	tree.insert(5);
	tree.insert(35);
	tree.insert(32);
	tree.insert(40);

	//tree.traverse(true);
	tree.insert(45);

	testing::internal::CaptureStdout();
	tree.traverse(true);
	EXPECT_EQ("5 (0) 30 (1) 32 (0) 35 (2) 40 (1) 45 (0) \n", 
			testing::internal::GetCapturedStdout());
}

// ---------------------------------------------------------------------------

TEST(TestDelete, leftRight)
{
	Tree tree;
	tree.insert(13);
	tree.insert(10);
	tree.insert(15);
	tree.insert(5);
	tree.insert(11);
	tree.insert(16);
	tree.insert(4);
	tree.insert(6);

	tree.traverse(true);
	tree.insert(7);

	testing::internal::CaptureStdout();
	tree.traverse(true);
	EXPECT_EQ("4 (0) 5 (1) 6 (2) 7 (0) 10 (1) 11 (0) 13 (3) 15 (1) 16 (0) \n", 
			testing::internal::GetCapturedStdout());
}

// ---------------------------------------------------------------------------

TEST(TestDelete, rightLeft)
{
	Tree tree;
	tree.insert(5);
	tree.insert(2);
	tree.insert(7);
	tree.insert(1);
	tree.insert(4);
	tree.insert(6);
	tree.insert(9);
	tree.insert(3);
	tree.insert(16);

	tree.traverse(true);
	tree.insert(15);

	testing::internal::CaptureStdout();
	tree.traverse(true);
	EXPECT_EQ("1 (0) 2 (2) 3 (0) 4 (1) 5 (3) 6 (0) 7 (2) 9 (0) 15 (1) 16 (0) \n", 
			testing::internal::GetCapturedStdout());
}
*/
// ---------------------------------------------------------------------------
