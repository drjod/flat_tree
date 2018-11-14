
struct TestMemberFixture : testing::Test
{
	jod::flat_tree<int, int> t;

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

TEST_F(TestMemberFixture, member)
{
	
	EXPECT_EQ(t.size(), 8);
	EXPECT_FALSE(t.empty());
	EXPECT_EQ(t.height(), 3);


	jod::flat_tree<int, int>::iterator it = t.find(13);
	EXPECT_EQ(it->second, 130);

	EXPECT_EQ(t.end(), t.find(1000));

	t[15] = -15;
	EXPECT_EQ(t[15], -15);

	t.at(5) = 40;
	EXPECT_EQ(t.at(5), 40);
	EXPECT_THROW(t.at(-5), std::out_of_range);
}

// ---------------------------------------------------------------------------
