// Project: Smart Pointer Implementation
// Author: Rudy Orre
// Class: CS30
// Professor: Solomon Russell
// Date: 4/2/2020 10:49pm (only an hour before submission)

#include <iostream>
using namespace std;

class null_ptr_exception : public runtime_error
{
public:
	null_ptr_exception(const char* s) : runtime_error(s)
	{
	}
};

template <typename T>
class smart_ptr
{
public:
	// default constructor //
	smart_ptr() noexcept : ptr_(nullptr), ref_(nullptr)
	{
	}
	// Create a smart_ptr that is initialized to nullptr. The reference count
	// should be initialized to nullptr.

	// l-value constructor //
	explicit smart_ptr(T*& raw_ptr) : ptr_(raw_ptr), ref_(nullptr)
	{
		int* ref = new int{ 1 };
		ref_ = ref;
	}
	// Create a smart_ptr that is initialized to raw_ptr. The reference count
	// should be one. No change is made to raw_ptr.

	// r-value constructor //
	explicit smart_ptr(T*&& raw_ptr) : ptr_(nullptr), ref_(nullptr)
	{
		try
		{
			ptr_ = raw_ptr;
			ref_ = new int{ 1 };
		}
		catch (...) // if there's a bad_alloc exceotion or something else, delete raw_ptr
		{
			delete raw_ptr;
			throw;
		}
	}
	// Create a smart_ptr that is initialized to raw_ptr. The reference count
	// should be one. If the constructor fails raw_ptr is deleted.	

	// l-value copy constructor //
	smart_ptr(const smart_ptr& rhs) noexcept : ptr_(rhs.ptr_), ref_(rhs.ref_)
	{
		(*ref_)++; // incrementing the reference count
	}
	// Copy construct a pointer from rhs. The reference count should be 
	// incremented by one.

	// r-value copy constructor //
	smart_ptr(smart_ptr&& rhs) noexcept
	{
		ptr_ = rhs.ptr_;
		ref_ = rhs.ref_;
		rhs.ptr_ = nullptr; // rhs loses it's data
		rhs.ref_ = nullptr; // since it's being "stolen" by the LHS
	}
	// Move construct a pointer from rhs.

	// l-value assignment operator //
	smart_ptr& operator=(const smart_ptr& rhs) noexcept
	{
		if (ptr_ != rhs.ptr_ || ref_ != rhs.ref_) // if the smart_ptrs are not the same
		{
			if (ptr_ != nullptr) // removing existing data of this smart_ptr if it isn't nullptr
			{
				if (*ref_ == 1) // if it is the last (delete prev pointer)
				{
					delete ptr_;
					delete ref_;
				}
				else // if it isn't the last (decrementing the prev pointer)
				{
					(*ref_)--; // since LHS won't be associated with its current ptr_ at the end of this function
				}
			}
			ptr_ = rhs.ptr_;
			ref_ = rhs.ref_;
			if (rhs.ref_ != nullptr) // when RHS isn't a nullptr
			{
				(*ref_)++; // if RHS isn't nullptr, then it has its own reference count that is increasing
			}
		}
		return *this;
	}
	// This assignment should make a shallow copy of the right-hand side's
	// pointer data. The reference count should be incremented as appropriate.

	// r-value assignment operator //
	smart_ptr& operator=(smart_ptr&& rhs) noexcept
	{
		if (ptr_ != rhs.ptr_ || ref_ != rhs.ref_) // only do this when they aren't the same
		{
			if (ptr_ != nullptr) // removing existing data of this smart_ptr if it isn't nullptr
			{
				if (*ref_ == 1) // if it is the last (delete prev pointer)
				{
					delete ptr_;
					delete ref_;
				}
				else // if it isn't the last (decrementing the prev pointer)
				{
					(*ref_)--;
				}
			}
			ptr_ = rhs.ptr_; // stealing
			ref_ = rhs.ref_; // data
			rhs.ptr_ = nullptr; // from
			rhs.ref_ = nullptr; // RHS
		}
		return rhs;
	}
	// This move assignment should steal the right-hand side's pointer data.

	// clone //
	bool clone()
	{
		if (ptr_ == nullptr || *ref_ == 1)
		{
			return false;
		}
		else
		{
			(*ref_)--;
			T* tempPtr = new T{ *ptr_ }; // creating a new template pointer with a deep copy of ptr_'s object
			int* tempRef = nullptr;
			try
			{
				tempRef = new int{ 1 }; // if this fails, it'll make sure to delete tempPtr, so no memory leaks
			}
			catch (std::bad_alloc& e)
			{
				delete tempPtr;
				(*ref_)++; // strong guarantee, so we want to revert ref count back to what it was
				throw;
			}
			ptr_ = tempPtr; // copying the data from the temp pointers
			ref_ = tempRef;
			tempPtr = nullptr;
			tempRef = nullptr;
		}
		return true;
	}
	// If the smart_ptr is either nullptr or has a reference count of one, this
	// function will do nothing and return false. Otherwise, the referred to
	// object's reference count will be decreased and a new deep copy of the
	// object will be created. This new copy will be the object that this
	// smart_ptr points and its reference count will be one. 

	// reference count //
	int ref_count() const noexcept	
	{
		if (ref_ == nullptr) // if reference count pointer is nullptr, return 0
		{
			return 0;
		}
		return *ref_;
	}
	// Returns the reference count of the pointed to data.

	// dereference operator //
	T& operator*()
	{
		if (ptr_ == nullptr) // if the ptr_ is nullptr, we can't dereference since it's not pointing to anything
		{
			throw null_ptr_exception("Illegal nullptr access with dereference operator.");
		}
		return *ptr_;
	}
	// The dereference operator shall return a reference to the referred object.
	// Throws null_ptr_exception on invalid access. 

	// arrow operator //
	T* operator->()
	{
		if (ptr_ == nullptr) // again, can't dereference a nullptr
		{
			throw null_ptr_exception("Illegal nullptr access with arrow operator.");
		}
		return ptr_;
	}
	// The arrow operator shall return the pointer ptr_. Throws null_ptr_exception
	// on invalid access.

	// destructor //
	~smart_ptr()
	{
		if (ref_ == nullptr || *ref_ == 1) // delete data if it nullptr or is the only pointer
		{
			delete ref_;
			delete ptr_;
		}
		else // decrement reference count if it is not the only pointer
		{
			(*ref_)--;
			ptr_ = nullptr;
		}
	}
	// deallocate all dynamic memory

private:
	T* ptr_;               // pointer to the referred object
	int* ref_;             // pointer to a reference count
}; 