


TEST(TestErase, leftRotation)
{
	jod::flat_tree<int, int> t;
	t.insert({44, 440});
	t.insert({17, 170});
	t.insert({62, 620});
	t.insert({32, 320});
	t.insert({50, 500});
	t.insert({78, 780});
	t.insert({48, 480});
	t.insert({54, 540});
	t.insert({88, 880});

    //t.traverse([](jod::flat_tree<int, int>::node p){
	//	std::cout << p.data().second <<  " (" << p.height() << ") "; }); std::cout << '\n';
        
	t.erase(32);

	testing::internal::CaptureStdout();
    t.traverse([](jod::flat_tree<int, int>::node p){
		std::cout << p.data().second <<  " (" << p.height() << ") "; });
	EXPECT_EQ("170 (0) 440 (2) 480 (0) 500 (1) 540 (0) 620 (3) 780 (1) 880 (0) ", 
			testing::internal::GetCapturedStdout());
}

// ---------------------------------------------------------------------------

TEST(TestErase, rightRotation)
{
	jod::flat_tree<int, int> t;
	t.insert({9, 90});
	t.insert({5, 50});
	t.insert({10, 100});
	t.insert({0, 0});
	t.insert({6, 60});
	t.insert({11, 110});
	t.insert({-1, -10});
	t.insert({1, 10});
	t.insert({2, 20});

    //t.traverse([](jod::flat_tree<int, int>::node p){
	//	std::cout << p.data().second <<  " (" << p.height() << ") "; }); std::cout << '\n';

	t.erase(10);

	testing::internal::CaptureStdout();
    t.traverse([](jod::flat_tree<int, int>::node p){
		std::cout << p.data().second <<  " (" << p.height() << ") "; });
	EXPECT_EQ("-10 (0) 0 (1) 10 (3) 20 (0) 50 (1) 60 (0) 90 (2) 110 (0) ", 
			testing::internal::GetCapturedStdout());
}

