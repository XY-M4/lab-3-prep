#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

// Lab3-prep
#include <utility> //for std::move (resource stealing)
#include <cassert> //for assert statements

template <typename T>  //declare type T
class UniquePtr {
public:
    UniquePtr(T* p = nullptr) : mPtr { p } {
    } //init, default is nullptr, else, arg passed through p

    ~UniquePtr() { //destruct
        if (mPtr != nullptr) { // not necessary since delete nullptr is no-op
            delete mPtr; }
    }

    // DONE: copy and move constructors/assignment operators.
    // Copying forbidden using delete
    UniquePtr(const UniquePtr&) = delete; // Don't allow creating a new UniquePtr by copying another one.
    UniquePtr& operator=(const UniquePtr&) = delete; // Don't allow UniquePtr a = b; only one pointer allowed.

    // Move constructor (creates ptr2)
    // Example usage: UniquePtr<T> ptr2(std::move(ptr1)); 
    UniquePtr(UniquePtr&& other) : mPtr { other.mPtr } {// Bitwise copy the other guy's stuff (e.g. pointers),
        other.mPtr = nullptr;  // then set the other guys resources to "null" resources.
    }  // We have thus "stolen" their stuff.

    // Move assignment operator (changes existing ptr2) 
    // Example usage: ptr2 = std::move(ptr1);
    UniquePtr& operator=(UniquePtr&& other) {
        if (this != &other) { // Swap our stuff with the other guy's stuff.
            std::swap(mPtr, other.mPtr); } // We have thus "stolen" their stuff.
        return *this;
    } // Since they will die soon, they will free our original resources when they destruct.

    // Converting move constructor (to use on ptr of diff data type)
    // Example usage: UniquePtr<T> ptr2(std::move(ptr1));
    template <typename U>  //declare type U
    UniquePtr(UniquePtr<U>&& other) : mPtr { other.release() } {
    } // steal value of U type from another variable

    // Dereference operator
    // Example usage: data_type var_name = *uPtr;
    T& operator*() const { 
        assert(mPtr != nullptr);
        return *mPtr; 
    } // allows for dereference of UniquePtrs.

    // Arrow operator
    // Example usage: uPtr->Method();
    T* operator->() const {
        assert(mPtr != nullptr);
        return mPtr;
    } // returns the underlying pointer, allowing member access (methods)

    // Get underlying raw pointer
    // Example usage: T* var_name = uPtr_name.get();
    T* get() const {
        return mPtr;
    } // returns raw pointer,

    // Comparison operator
    // Example usage: if (ptr1 == ptr2)
    bool operator==(const UniquePtr<T>& other) const{
        return mPtr == other.mPtr;
    } // return true, if both mPtr values are equal, else false

    // Release ownership
    // Example usage: T* var_name = uPtr.release();
    T* release() {
        T* var = mPtr;
        mPtr = nullptr;
        return var;
    } // return pointer

    // Reset ownership
    // Example usage: uPtr.reset(new int(10));
    void reset(T* newPtr = nullptr) {
        assert(mPtr == nullptr || newPtr != mPtr);
        T* current_Ptr = mPtr;
        mPtr = newPtr;
        delete current_Ptr;
    } // reset val of uPtr to new_val

    // Swap owned pointers
    // Example usage: ptr1.swap(ptr2);
    void swap(UniquePtr<T>& other) {
        std::swap(mPtr, other.mPtr);
    } // swap two existing uPtrs

    // Bool conversion
    // Example usage: if (uPtr)
    explicit operator bool() const {
        return mPtr != nullptr;
    } // return false if empty, else true 

private:
    T* mPtr;
};

// Create a Unique Pointer
// Example usage: auto uPtr = makeUnique<int>(5);
template <typename T, typename... Args> 
UniquePtr<T> makeUnique(Args&&... args) {
    return UniquePtr<T>( //pass args using forward
        new T(std::forward<Args>(args)...)
    ); // UniquePtr now manages the new T
} 
// A const UniquePtr<T> should prevent changing the pointer's
// ownership. It does not prevent changing the owned T. 
// To make the owned object const, use UniquePtr<const T>.

#endif
