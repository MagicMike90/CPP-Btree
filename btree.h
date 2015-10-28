/*
*** 
 */

#ifndef BTREE_H
#define BTREE_H

#include <utility>
#include <set>
#include <vector>
#include <cstddef>
#include <iostream>

//Using a namespace
namespace Constant {
    const int null = 0;
}

#include "btree_iterator.h"

template <typename T> std::ostream& operator<<(std::ostream& os, const btree<T>& tree);
template <typename T> class btree;

template <typename T> 
class btree {
    public:
    	//Friends
        friend class btree_const_iterator<T>;
        friend class btree_iterator<T>;
        
        //Iterator Typedefs
        typedef btree_iterator<T> iterator;
        typedef btree_const_iterator<T> const_iterator;
        typedef btree_reverse_iterator<const_iterator>  constRevIt;
        typedef btree_reverse_iterator<iterator>        revIt;

		//Constructors
        btree(size_t ElementMaximum = 40);
        btree(const btree<T>& original);
        btree<T>& operator=(const btree<T>& rightHandSide);

		//BTree output
        friend std::ostream& operator<< <T> (std::ostream& os, const btree<T>& tree);

		//Iterator functions
        iterator begin() { 
        	return iterator(btree_->head(), this->btree_);
    	} 
        
        const_iterator begin() const { 
        	return const_iterator(btree_->head(), this->btree_); 
        } 
        
        iterator end() {
        	return iterator(Constant::null, this->btree_); 
        }
        
        const_iterator end() const { 
        	return const_iterator(Constant::null, this->btree_); 
        }

        revIt rbegin() { 
        	return revIt(end());
        }
        
        constRevIt rbegin() const { 
        	return constRevIt(end()); 
        }
        
        revIt rend() { 
        	return revIt(begin()); 
        }
        
        constRevIt rend() const { 
        	return constRevIt(begin());
        }
         
        iterator find(const T& elem);
        const_iterator find(const T& elem) const;

        std::pair<iterator, bool> insert(const T& elem);

		~btree();
    
    private:
		typedef std::pair<iterator, bool> insert_res_type;
        struct treePtr;
        treePtr btree_;
        size_t ElementMaximum_;

		//Element object
        struct Element {
            Element(const T& e) : elem(e) {
            	count = 0;
            }

            T elem; //element itself
            treePtr owner_; // the btree this Element belongs to. 
            treePtr left_; //element on the left
            treePtr right_; // element on the right
            mutable int count;

            // destructor
            ~Element() {
                clear();
            }

            // functions
            friend std::ostream& operator<<(std::ostream& os, const Element& n) {
                os << n.elem;
                return os;
            }

			//Overloaded operator <
            friend bool operator< (const Element& l, const Element& r) {
                return l.elem < r.elem;
            }

            Element* findNextUp(); 
            Element* findPrevUp(); 

			//Clear function
            void clear() {
                right_ = treePtr();    
                left_ = treePtr();    
                owner_ = treePtr();    
            }

            void recurClear() {
                if (!left_.isNull()) left_->recurClear();
                if (!right_.isNull()) right_->recurClear();
                clear();
            }
        };

        struct ElementPtr {

            // constructors
            ElementPtr(Element* n) : n(n) { ++n->count; }
            ElementPtr(const ElementPtr& rightHandSide) : n(rightHandSide.n) { if (!isNull()) ++n->count; }
			ElementPtr() : n(Constant::null) {}
            
            // destructor
            ~ElementPtr() { 
                if (!isNull() && --n->count == 0) { 
                    delete n;
                } 
            }

            // members
            Element* n;

            // functions
            Element* operator->() {
            	return n;
        	}
            
            Element* operator->() const { 
            	return n;
            }
            
            Element& operator*() { 
            	return *n;
        	}
            
            Element& operator*() const { 
            	return *n;
            }

            ElementPtr& operator=(const ElementPtr& rightHandSide) {
                if (n == rightHandSide.n) 
                    return *this;
                if (!isNull() && --n->count == 0)
                    delete n;

                n = rightHandSide.n;
                if (!isNull()) ++n->count;
                return *this;
            }

            friend bool operator< (const ElementPtr& l, const ElementPtr& r) {
                return *l < *r;
            }

            friend std::ostream& operator<<(std::ostream& os, const ElementPtr& np) {
                os << *np;
                return os;
            }

            bool isNull() const { return n == Constant::null; }
        };

        struct BTree {
            // Types
            typedef set<ElementPtr>                    Elements_type;
            typedef typename Elements_type::iterator   Elements_iterator_type;
           
            // constructor
            BTree(size_t max) : ElementMaximum_(max), count(0) {
            	Elements_type Elements();
        	}

            // destructor
            ~BTree() { 
                recurClear();
            }

            // Members
            ElementPtr top_left_;
            ElementPtr top_right_;

            Elements_type  Elements_;
            size_t      ElementMaximum_;
            mutable int count;

            // Functions
            insert_res_type insert(const T& elem);
            size_t ElementElems() { return Elements_.size(); }
            friend std::ostream& operator<<(std::ostream& os, const BTree& bt) {
                for (typename Elements_type::const_iterator it = bt.Elements_.begin();
                     it != bt.Elements_.end(); ++it) {
                    os << *it;
                }
                return os;
            }

            Element* find(const T& elem) const ;
            Element* head() const;
            Element* tail() const;
            void outputBF(vector<string>& strs, size_t level) const;
            BTree* clone() const;

            void clear() {
                Elements_.clear();
            }

            void recurClear() {
                for (Elements_iterator_type it = Elements_.begin(); it != Elements_.end(); ++it) {
                    (*it)->recurClear();
                }
                clear();
            }
            
        };

        struct treePtr {

            // constructor
            treePtr() : btree_(Constant::null) {}
            treePtr(BTree* bt) : btree_(bt) { if (!isNull()) ++btree_->count; } 
            treePtr(const treePtr& rightHandSide) : btree_(rightHandSide.btree_) { ++btree_->count; }

            // destructor
            ~treePtr() { 
                if (!isNull() && --btree_->count == 0) {
                    delete btree_;
                }  
            }
            
            // members
            BTree* btree_;

            // functions
            BTree& operator*() { 
            	return *btree_; 
            }
            BTree& operator*() const { 
            	return *btree_; 
            }
			BTree* operator->() { 
            	return btree_; 
            }
            BTree* operator->() const { 
            	return btree_; 
            }
            
            treePtr& operator=(const treePtr& rightHandSide) {
                if (!isNull() && btree_ == rightHandSide.btree_)  {
                    return *this;
                }
                if (btree_ != Constant::null && --btree_->count == 0) {
                    delete btree_;
                }

                btree_ = rightHandSide.btree_;
                if (!isNull() ) ++btree_->count;
                return *this;
            }
            
            bool isNull() const { return btree_ == Constant::null; }
        };
};

#include "btree.tem"
#endif
