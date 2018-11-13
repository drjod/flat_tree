
struct entity
{
	int nr;
	entity(int _nr) : nr(_nr) {}
	int get_nr() const { return nr; }
};

struct TestIteratorFixture : testing::Test
{
	jod::flat_tree<int, int> t;
	jod::flat_tree<int, entity> te;

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

		te.insert({1, entity(1)});
	}
	virtual void TearDown() {}

};

// -----------------------------------------------------------------------------

TEST_F(TestIteratorFixture, iterator)
{
	testing::internal::CaptureStdout();
	for(auto it: t) std::cout << it.first << " " << it.second << ", ";
	EXPECT_EQ("4 40, 5 50, 6 60, 10 100, 11 110, 13 130, 15 150, 16 160, ", 
			testing::internal::GetCapturedStdout());

	testing::internal::CaptureStdout();
	for(jod::flat_tree<int, int>::iterator it=t.begin(); it != t.end(); ++it)
	{
		it->second *= 10;
		std::cout << it->first << " " << it->second << ", ";
	}
	EXPECT_EQ("4 400, 5 500, 6 600, 10 1000, 11 1100, 13 1300, 15 1500, 16 1600, ", 
			testing::internal::GetCapturedStdout());

	EXPECT_TRUE(t.end() == nullptr);
	EXPECT_EQ(te.begin()->second.get_nr(), 1);
}


TEST_F(TestIteratorFixture, const_iterator)
{
	testing::internal::CaptureStdout();
	for(const auto& cit: t) std::cout << cit.first << " " << cit.second << ", ";
	EXPECT_EQ("4 40, 5 50, 6 60, 10 100, 11 110, 13 130, 15 150, 16 160, ", 
			testing::internal::GetCapturedStdout());

	testing::internal::CaptureStdout();
	for(jod::flat_tree<int, int>::const_iterator cit=t.cbegin(); cit != t.cend(); ++cit)
	{
		std::cout << cit->first << " " << cit->second << ", ";
	}
	EXPECT_EQ("4 40, 5 50, 6 60, 10 100, 11 110, 13 130, 15 150, 16 160, ", 
			testing::internal::GetCapturedStdout());

	EXPECT_TRUE(t.cend() == nullptr);
	EXPECT_EQ(te.cbegin()->second.get_nr(), 1);
}


TEST_F(TestIteratorFixture, reverse_iterator)
{
	testing::internal::CaptureStdout();
	for(jod::flat_tree<int, int>::reverse_iterator rit=t.rbegin(); rit != t.rend(); ++rit)
	{
		rit->second *= 10;
		std::cout << rit->first << " " << rit->second << ", ";
	}
	EXPECT_EQ("16 1600, 15 1500, 13 1300, 11 1100, 10 1000, 6 600, 5 500, 4 400, ", 
			testing::internal::GetCapturedStdout());

	EXPECT_TRUE(t.rend() == nullptr);
	EXPECT_EQ(te.rbegin()->second.get_nr(), 1);
}


TEST_F(TestIteratorFixture, const_reverse_iterator)
{
	testing::internal::CaptureStdout();
	for(jod::flat_tree<int, int>::const_reverse_iterator crit=t.crbegin(); crit != t.crend(); ++crit)
	{
		std::cout << crit->first << " " << crit->second << ", ";
	}
	EXPECT_EQ("16 160, 15 150, 13 130, 11 110, 10 100, 6 60, 5 50, 4 40, ", 
			testing::internal::GetCapturedStdout());

	EXPECT_TRUE(t.crend() == nullptr);
	EXPECT_EQ(te.crbegin()->second.get_nr(), 1);
}


// ---------------------------------------------------------------------------
