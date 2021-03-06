/**
 * @file      test.Object.cpp
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2020, Sergey Baigudin, Baigudin Software
 *
 * @brief Unit tests of `Object`.
 */
#include "System.hpp"
#include "Object.hpp"
#include "lib.Types.hpp"

namespace eoos
{
namespace test
{
    
class test_Object : public ::testing::Test
{

protected:
    
    System eoos;    
};    
    
namespace 
{

Object<> createObject()
{
    Object<> const obj{};
    return obj;
}

class TestObject : public ::eoos::Object<>
{
    using Parent = ::eoos::Object<>;

public:

    void setConstructed(bool_t const flag)
    {
        Parent::setConstructed(flag);
    }    
};

} // namespace

TEST_F(test_Object, Constructor)
{
    Object<> const obj {};
    EXPECT_TRUE(obj.isConstructed())        << "Error: Object is not conctructed";    
}

TEST_F(test_Object, CopyConstructor)
{
    Object<> const obj1 {};
    EXPECT_TRUE(obj1.isConstructed())       << "Error: Object 1 is not conctructed";
    Object<> const obj2 { obj1 };
    EXPECT_TRUE(obj1.isConstructed())       << "Error: Object 2 is not conctructed";
}

TEST_F(test_Object, MoveConstructor)
{
    // Test if compiler moves an obj to obj1
    Object<> obj1 { createObject() };
    EXPECT_TRUE(obj1.isConstructed())   << "Error: An object is not moved to object 1 by compiler";
    // Test if cast moves obj1 to obj2
    Object<> const obj2 { lib::move(obj1) };
    EXPECT_TRUE(obj2.isConstructed())   << "Error: Object 1 is not move casted to object 2";
    EXPECT_FALSE(obj1.isConstructed())  << "Error: Object 1 is conctructed after movement to object 2";
}

TEST_F(test_Object, CopyAssignment)
{
    Object<> const obj1 {};
    EXPECT_TRUE(obj1.isConstructed())   << "Error: Object 1 is not conctructed";
    Object<> obj2 {};
    EXPECT_TRUE(obj2.isConstructed())   << "Error: Object 2 is not conctructed";
    obj2 = obj1;
    EXPECT_TRUE(obj2.isConstructed())   << "Error: Object 2 is not assigned with object 1";
}

TEST_F(test_Object, MoveAssignment)
{
    Object<> obj1 {};
    Object<> obj2 {};
    // Test if an obj moved to rvalue, and the rvalue assigned to obj1
    obj1 = createObject();
    EXPECT_TRUE(obj1.isConstructed())   << "Error: An object is not moved to rvalue, and the rvalue is not assigned to object 1";
    // Test if obj1 moved with lvalue to obj2
    obj2 = lib::move(obj1);  
    EXPECT_TRUE(obj2.isConstructed())   << "Error: An object 2 is not constructed with lvalue";
    EXPECT_FALSE(obj1.isConstructed())  << "Error: An object 1 is constructed but it was moved with lvalue";
    // Test if an obj1 cannot be recovered
    obj1 = lib::move(Object<>());
    EXPECT_FALSE(obj1.isConstructed())   << "Error: An object 1 is re-constructed but it was moved"; 
    // Test if an obj moved with rvalue to obj1
    obj2 = lib::move(Object<>());
    EXPECT_TRUE(obj2.isConstructed())   << "Error: An object 2 is not constructed with rvalue of a moved object";
}

TEST_F(test_Object, MemoryAllocation)
{
    Object<>* obj {new Object<>()};
    ASSERT_NE(obj, NULLPTR)             << "Error: Object is not allocated";
    EXPECT_TRUE(obj->isConstructed())   << "Error: Object is not conctructed, but allocated";
    delete obj;
    obj = NULLPTR;
    TestObject mem{};
    mem.setConstructed(false);
    EXPECT_FALSE(mem.isConstructed())   << "Error: Some memory is not ready to be used";
    obj = new (&mem) Object<>();
    EXPECT_NE(obj, NULLPTR)             << "Error: Object is not put on memory";
    EXPECT_TRUE(obj->isConstructed())   << "Error: Object is not conctructed, but put on memory";
}

TEST_F(test_Object, isConstructed)
{
    Object<> const obj{};
    EXPECT_TRUE(obj.isConstructed())  << "Error: Object is not conctructed";
    obj.~Object();
    EXPECT_FALSE(obj.isConstructed()) << "Error: Object is conctructed after destruction";
}

TEST_F(test_Object, isConstructed_obj)
{
    Object<>* obj {new Object<>()};
    EXPECT_TRUE(Object<>::isConstructed(obj))   << "Error: Object is not conctructed";
    delete obj;
    obj = NULLPTR;
    EXPECT_FALSE(Object<>::isConstructed(obj))  << "Error: Pointer to NULLPTR object is conctructed";
}

TEST_F(test_Object, setConstructed)
{
    TestObject obj{};
    EXPECT_TRUE(obj.isConstructed())    << "Error: Object is not conctructed";
    obj.setConstructed(true);
    EXPECT_TRUE(obj.isConstructed())    << "Error: Object is not set as conctructed";
    obj.setConstructed(false);
    EXPECT_FALSE(obj.isConstructed())   << "Error: Object is not set as unconctructed";
    obj.setConstructed(true);
    EXPECT_FALSE(obj.isConstructed())   << "Error: Object is set as conctructed if it is unconctructed";
}

} // namespace test
} // namespace eoos
