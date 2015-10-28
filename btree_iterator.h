/*
*** 
 */

#ifndef BTREE_ITERATOR_H
#define BTREE_ITERATOR_H

#include <iterator>
using namespace std;

template <typename T> class btree;
template <typename T> class btree_const_iterator;

template <typename T>
class btree_iterator {
    public:
    friend class btree_const_iterator<T>;
    typedef ptrdiff_t                   difference_type;
    typedef bidirectional_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef T&                           reference;
    typedef T*                           pointer;
    
    T&  operator*() const { 
    	return treePointee->elem; 
    }
    
    T* operator->() const { 
    	return treePointee; 
    }
    
    btree_iterator& operator++();
    btree_iterator& operator--();
    
    void operator++(int) { 
    	++(*this); 
    }
    
    void operator--(int) {
     	--(*this); 
     }
    
    bool operator==(const btree_iterator& other) const {
    	if (this->treePointee == other.treePointee) {
   			return true;
 		} else {
 			return false;
 		}
    }
    
    bool operator!=(const btree_iterator& other) const { 
    	return !operator==(other);
    }

    btree_iterator(typename btree<T>::Element *treePointee = Constant::null) : treePointee(treePointee) {}
    btree_iterator(typename btree<T>::Element *treePointee, 
                   const typename btree<T>::treePtr& btptr) : treePointee(treePointee),
                                                        btree_(btptr.btree_) {}
    // converstions from const to non-const version of iterator
    btree_iterator& operator=(const btree_const_iterator<T>& constIt) {
        treePointee = constIt.treePointee;
        btree_ = constIt.btree_;
    }
    bool operator==(const btree_const_iterator<T>& other) const { return this->treePointee == other.treePointee; } 
    bool operator!=(const btree_const_iterator<T>& other) const { return !operator==(other); }

    private:
    typename btree<T>::Element *treePointee;
    const typename btree<T>::BTree* btree_;
};

template <typename ForwardItType>
class btree_reverse_iterator {
    public:
    typedef typename ForwardItType::difference_type      difference_type;
    typedef typename ForwardItType::iterator_category    iterator_category;
    typedef typename ForwardItType::value_type           value_type;
   
    btree_reverse_iterator() : current_() {}
    btree_reverse_iterator(ForwardItType forit) : current_(forit) {}
    btree_reverse_iterator(const btree_reverse_iterator& other) : current_(other.current_) {}

    template <typename OtherItType>
    btree_reverse_iterator(const btree_reverse_iterator<OtherItType>& other) : current_(other.base()) {}

    ForwardItType base() const {
    	return current_;
    }

	btree_reverse_iterator& operator++() {
    	--current_; 
    	return *this;
    } 
    void operator++(int) { 
    	++(*this);
    } 
    btree_reverse_iterator& operator--() {
    	++current_;
    	return *this;
    }
    void operator--(int) {
    	--(*this);
    }

   typename ForwardItType::reference operator*() const { 
        ForwardItType tmp = current_; 
        return *--tmp;
    }
    typename ForwardItType::pointer operator->() const { 
    	return &(operator*());
    }
    
    

    bool operator==(const btree_reverse_iterator& other) const { 
    	return base() == other.base();
    }
    bool operator!=(const btree_reverse_iterator& other) const {
    	return !operator==(other); 
    }


    protected:
    ForwardItType current_;
};


template <typename T>
class btree_const_iterator {
    public:
    friend class btree_iterator<T>;
    typedef ptrdiff_t                   difference_type;
    typedef bidirectional_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef const T*                    pointer;
    typedef const T&                    reference;

    reference operator*() const {
    	return treePointee->elem; 
    }
    pointer operator->() const {
    	return treePointee; 
    }
    btree_const_iterator& operator++();
    void operator++(int) { 
    	++(*this); 
    }
    btree_const_iterator& operator--();
    void operator--(int) {
    	--(*this);
    }
    bool operator==(const btree_const_iterator& other) const { 
    	return this->treePointee == other.treePointee; 
    } 
    bool operator!=(const btree_const_iterator& other) const {
    	return !operator==(other); 
    }

    btree_const_iterator(typename btree<T>::Element *treePointee = Constant::null) : treePointee(treePointee) {}
    btree_const_iterator(typename btree<T>::Element *treePointee, 
                   const typename btree<T>::treePtr& btptr) : treePointee(treePointee),
                                                        btree_(btptr.btree_) {}
	//conversion
    btree_const_iterator(const btree_iterator<T>& nonConstIt) : treePointee(nonConstIt.treePointee),
                                                                btree_(nonConstIt.btree_) {}
    btree_const_iterator& operator=(const btree_iterator<T>& nonConstIt) {
        treePointee = nonConstIt.treePointee;
        btree_ = nonConstIt.btree_;
    }
    bool operator==(const btree_iterator<T>& other) const { 
    	return this->treePointee == other.treePointee;
    } 
    bool operator!=(const btree_iterator<T>& other) const { 
    	return !operator==(other);
    }


    private:
    typename btree<T>::Element *treePointee;
    const typename btree<T>::BTree* btree_;
};


#include "btree_iterator.tem"

#endif
