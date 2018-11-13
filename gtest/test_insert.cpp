
TEST(TestInsert, leftLeft)
{
	jod::flat_tree<int, int> t;
	t.insert({13, 13});
	t.insert({10, 10});
	t.insert({15, 15});
	t.insert({5, 5});
	t.insert({11, 11});
	t.insert({16, 16});
	t.insert({4, 4});
	t.insert({6, 6});

	//t.traverse([](jod::flat_tree<int, int>::node p){ 
	//	std::cout << p.data().second <<  " (" << p.height() << ") "; }); std::cout << '\n';

	t.insert({3, 3});

	testing::internal::CaptureStdout();
	t.traverse([](jod::flat_tree<int, int>::node p){ 
		std::cout << p.data().second <<  " (" << p.height() << ") "; });
	EXPECT_EQ("3 (0) 4 (1) 5 (2) 6 (0) 10 (1) 11 (0) 13 (3) 15 (1) 16 (0) ", 
			testing::internal::GetCapturedStdout());
}

// ---------------------------------------------------------------------------

TEST(TestInsert, rightRight)
{
	jod::flat_tree<int, int> t;
	t.insert({30, 30});
	t.insert({5, 5});
	t.insert({35, 35});
	t.insert({32, 32});
	t.insert({40, 40});

	//t.traverse([](jod::flat_tree<int, int>::node p){ 
	//	std::cout << p.data().second <<  " (" << p.height() << ") "; }); std::cout << '\n';

	t.insert({45, 45});

	testing::internal::CaptureStdout();
	t.traverse([](jod::flat_tree<int, int>::node p){ 
		std::cout << p.data().second <<  " (" << p.height() << ") "; });
	EXPECT_EQ("5 (0) 30 (1) 32 (0) 35 (2) 40 (1) 45 (0) ", 
			testing::internal::GetCapturedStdout());
}

// ---------------------------------------------------------------------------

TEST(TestInsert, leftRight)
{
	jod::flat_tree<int, int> t;
	t.insert({13, 13});
	t.insert({10, 10});
	t.insert({15, 15});
	t.insert({5, 5});
	t.insert({11, 11});
	t.insert({16, 16});
	t.insert({4, 4});
	t.insert({6, 6});

	//t.traverse([](jod::flat_tree<int, int>::node p){ 
	//	std::cout << p.data().second <<  " (" << p.height() << ") "; }); std::cout << '\n';

	t.insert({7, 7});

	testing::internal::CaptureStdout();
	t.traverse([](jod::flat_tree<int, int>::node p){ 
		std::cout << p.data().second <<  " (" << p.height() << ") "; });
	EXPECT_EQ("4 (0) 5 (1) 6 (2) 7 (0) 10 (1) 11 (0) 13 (3) 15 (1) 16 (0) ", 
			testing::internal::GetCapturedStdout());
}

// ---------------------------------------------------------------------------

TEST(TestInsert, rightLeft)
{
	jod::flat_tree<int, int> t;
	t.insert({5, 5});
	t.insert({2, 2});
	t.insert({7, 7});
	t.insert({1, 1});
	t.insert({4, 4});
	t.insert({6, 6});
	t.insert({9, 9});
	t.insert({3, 3});
	t.insert({16, 16});

	//t.traverse([](jod::flat_tree<int, int>::node p){ 
	//	std::cout << p.data().second <<  " (" << p.height() << ") "; }); std::cout << '\n';

	t.insert({15, 15});

	testing::internal::CaptureStdout();
	t.traverse([](jod::flat_tree<int, int>::node p){ 
		std::cout << p.data().second <<  " (" << p.height() << ") "; });
	EXPECT_EQ("1 (0) 2 (2) 3 (0) 4 (1) 5 (3) 6 (0) 7 (2) 9 (0) 15 (1) 16 (0) ", 
			testing::internal::GetCapturedStdout());
}


// ---------------------------------------------------------------------------
