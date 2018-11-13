
struct TestCompareFixture : testing::Test
{
	jod::flat_tree<int, int, std::greater<int> > t;

	virtual void SetUp()
	{
		t.insert({13, 130});
		t.insert({10, 100});
		t.insert({15, 150});
		t.insert({5, 50});
		t.insert({11, 110});
		t.insert({16, 160});
		t.insert({4, 40});
		t.insert({6, 60});

	}
	virtual void TearDown() {}

};

// -----------------------------------------------------------------------------

TEST_F(TestCompareFixture, compare)
{

    testing::internal::CaptureStdout();
    t.traverse([](jod::flat_tree<int, int, std::greater<int> >::node p){
        std::cout << p.data().second <<  " (" << p.height() << ") "; });
    EXPECT_EQ("160 (0) 150 (1) 130 (3) 110 (0) 100 (2) 60 (0) 50 (1) 40 (0) ",
            testing::internal::GetCapturedStdout());

}

// ---------------------------------------------------------------------------
