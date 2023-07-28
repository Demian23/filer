#ifndef __ManagedPtr_h__
#define __ManagedPtr_h__


#include <memory>

class IDeleter{
public:
    virtual void dispose() = 0;
    virtual ~IDeleter(){}
};

class NoDel : public IDeleter{
public: 
    void dispose()override{}
};

template <typename T>
class ManagedPtr{
public:
    ManagedPtr(T* ptr, std::unique_ptr<IDeleter>&& d) : ptr_(ptr), d_(std::move(d)), 
        owner_(true){}
    ManagedPtr() : ptr_(nullptr), d_(), owner_(false){}

    static void passOwn(ManagedPtr<T>& f, ManagedPtr<T>& s){
        s.owner_ = f.owner_;
        f.owner_ = false;
        s.d_ = std::move(f.d_);
    }
    
    ManagedPtr(const ManagedPtr& mptr){
        ptr_ = mptr.ptr_;     
        owner_ = false;
    }

    ManagedPtr& operator=(const ManagedPtr& mptr){
        if(this != &mptr){
            if(owner_) d_->dispose();
            ptr_ = mptr.ptr_;
            d_.reset();
            owner_ = false;
        }
        return *this;
    }

    T* operator->(){return ptr_;}
    T& operator*(){return *ptr_;}

    bool operator!(){return !ptr_;}

    void free(){
        if(owner_){
            d_->dispose();
            owner_ = false;
        }
        d_.reset();
        ptr_ = nullptr;
    }

    ~ManagedPtr(){
        if(owner_)
            d_->dispose();
    }
private:
    T* ptr_;
    std::unique_ptr<IDeleter> d_;
    bool owner_;
};

#endif
