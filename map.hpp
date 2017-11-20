#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"
#include <cstdio>
using namespace std;
int mmax(int x, int y){
    return x>y ? x:y;
}
namespace sjtu {

template<class Key, class T, class Compare = std::less<Key>>
class map {
public:
	typedef pair<Key, T> value_type;
    struct fnode{
        fnode * next;
        fnode * pre;
        fnode * left;
        fnode * right;
        fnode * parent;
        value_type* data;
        int colour;//1 for black and 0 for red;
        fnode():next(NULL), pre(NULL), left(NULL), right(NULL), colour(0), data(NULL), parent(NULL){}
        fnode(const value_type& x, int c=0):next(NULL), pre(NULL), left(NULL), right(NULL), colour(c), parent(NULL){data = new value_type(x);}
        ~fnode(){
            if (data != NULL){
                delete data;
            }
            data = NULL;
        }
    };
	class const_iterator;
	class iterator {
	private:
        fnode *p;
        const map* owner;
	public:
		iterator() {
		    p = new fnode();
		}
		iterator(const value_type& x, const map* o){
            p = new fnode(x);
            owner = o;
		}
		iterator(fnode * f, const map*o){
            p = f;
            owner = o;
		}
		iterator(const map* o){
            owner = o;
            p = new fnode();
        }
		iterator(const iterator &other) {
            p = &other.return_node();
            owner = &other.return_owner();
		}
		~iterator(){}
		void clear(){
            if (p != NULL) {
                delete p;
            }
            p = NULL;
            owner = NULL;
		}
		void recover(const map* o){
            p = new fnode();
            owner = o;
		}
        iterator operator+(const int &n) const {
            if (n<0) {
                int x = -n;
                iterator tmp = *this;
                while (x--){
                    tmp--;
                }
                return tmp;
            }
            int x = n;
            iterator tmp = *this;
            while (x--){
                tmp++;
            }
            return tmp;
        }
        iterator operator-(const int &n) const {
            if (n<0) {
                int x = -n;
                iterator tmp = *this;
                while (x--){
                    tmp++;
                }
                return tmp;
            }
            int x = n;
            iterator tmp = *this;
            while (x--)
            {
                tmp--;
            }
            return tmp;
        }
		iterator operator++(int) {
            iterator tmp = *this;
            if (p->next == NULL) throw index_out_of_bound();
            p = p->next;
            return tmp;
		}
		iterator & operator++() {
            if (p->next == NULL) throw index_out_of_bound();
            p = p->next;
            return *this;
		}
		iterator operator--(int) {
            iterator tmp = *this;
            if (p->pre->pre == NULL) throw index_out_of_bound();
            p = p->pre;
            return tmp;
		}
		iterator & operator--() {
            if (p->pre->pre == NULL) throw index_out_of_bound();
            p = p->pre;
            return *this;
		}
		void changeroot(fnode * t){
            p = t;
		}
		value_type & operator*() const {
            if (p->next == NULL || p->pre == NULL) throw invalid_iterator();
            return *(p->data);
		}
		bool operator==(const iterator &rhs) const {
		    if (p->data == NULL && rhs.nodeptr()->data == NULL) return ownerptr() == rhs.ownerptr();
            return p->data == rhs.nodeptr()->data;
		}
		bool operator==(const const_iterator &rhs) const {
            if (p->data == NULL && rhs.nodeptr()->data == NULL) return ownerptr() == rhs.ownerptr();
            return p->data == rhs.nodeptr()->data;
		}
		bool operator!=(const iterator &rhs) const {
            if (p->data == NULL && rhs.nodeptr()->data == NULL) return ownerptr() != rhs.ownerptr();
            return p->data != rhs.nodeptr()->data;
		}
		bool operator!=(const const_iterator &rhs) const {
            if (p->data == NULL && rhs.nodeptr()->data == NULL) return ownerptr() != rhs.ownerptr();
            return p->data != rhs.nodeptr()->data;
		}
		value_type* operator->() const noexcept {
            return p->data;
		}
		fnode& return_node()const{
            if (p == NULL) throw invalid_iterator();
            return *p;
		}
		const map& return_owner()const{
            return *owner;
		}
		fnode * nodeptr()const{
            if (p == NULL) throw invalid_iterator();
            return p;
		}
		const map* ownerptr()const{
            return owner;
		}
	};
	class const_iterator {
		private:
			fnode * p;
			const map * owner;
		public:
			const_iterator() {
				p = new fnode();
			}
			const_iterator(fnode *f, const map* o){
                p = f;
                owner = o;
			}
			const_iterator(const const_iterator &other) {
				p = &other.return_node();
				owner = &other.return_owner();
			}
			const_iterator(const iterator &other) {
				p = &other.return_node();
				owner = &other.return_owner();
			}
			~const_iterator(){}
			void clear(){
                if (p != NULL) {
                    delete p;
                }
                p = NULL;
                owner = NULL;
            }
            void recover(const map* o){
                p = new fnode();
                owner = o;
            }
            const_iterator operator+(const int &n) const {
                if (n<0) {
                    int x = -n;
                    const_iterator tmp = *this;
                    while (x--){
                        tmp--;
                    }
                    return tmp;
                }
                int x = n;
                const_iterator tmp = *this;
                while (x--){
                    tmp++;
                }
                return tmp;
            }
            const_iterator operator-(const int &n) const {
                if (n<0) {
                    int x = -n;
                    const_iterator tmp = *this;
                    while (x--){
                        tmp++;
                    }
                    return tmp;
                }
                int x = n;
                const_iterator tmp = *this;
                while (x--)
                {
                    tmp--;
                }
                return tmp;
            }
            const_iterator operator++(int) {
                const_iterator tmp = *this;
                if (p->next == NULL) throw index_out_of_bound();
                p = p->next;
                return tmp;
            }
            const_iterator & operator++() {
                if (p->next == NULL) throw index_out_of_bound();
                p = p->next;
                return *this;
            }
            const_iterator operator--(int) {
                const_iterator tmp = *this;
                if (p->pre->pre == NULL) throw index_out_of_bound();
                p = p->pre;
                return tmp;
            }
            const_iterator & operator--() {
                if (p->pre->pre == NULL) throw index_out_of_bound();
                p = p->pre;
                return *this;
            }
            bool operator==(const iterator &rhs) const {
                if (p->data == NULL && rhs.nodeptr()->data == NULL) return ownerptr() == rhs.ownerptr();
                return p->data == rhs.nodeptr()->data;
            }
            bool operator==(const const_iterator &rhs) const {
               if (p->data == NULL && rhs.nodeptr()->data == NULL) return ownerptr() == rhs.ownerptr();
                return p->data == rhs.nodeptr()->data;
            }
            bool operator!=(const iterator &rhs) const {
                if (p->data == NULL && rhs.nodeptr()->data == NULL) return ownerptr() != rhs.ownerptr();
                return p->data != rhs.nodeptr()->data;
            }
            bool operator!=(const const_iterator &rhs) const {
                if (p->data == NULL && rhs.nodeptr()->data == NULL) return ownerptr() != rhs.ownerptr();
                return p->data != rhs.nodeptr()->data;
            }
            value_type & operator*() const {
                if (p->next == NULL || p->pre == NULL) throw invalid_iterator();
                return *(p->data);
            }
            value_type* operator->() const noexcept {
                return p->data;
            }
            fnode& return_node()const{
                if (p == NULL) throw invalid_iterator();
                return *p;
            }
            const map& return_owner()const{
                return *owner;
            }
            fnode * nodeptr()const{
                if (p == NULL) throw invalid_iterator();
                return p;
            }
            const map* ownerptr()const{
                return owner;
            }
	};
private:
    iterator head;
	iterator tail;
	fnode* root;
	size_t len;
    void left_rotate(fnode *& t, fnode *& x){
        fnode * y = x->right;
        x->right = y->left;
        if (y->left != NULL){
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == NULL){
            t = y;
        }
        else{
            if (x->parent->left == x){
                x->parent->left = y;
            }
            else {
                x->parent->right = y;
            }
        }
        y->left = x;
        x->parent = y;
    }
    void right_rotate(fnode *& t, fnode *& y){
        fnode * x = y->left;
        y->left = x->right;
        if (x->right != NULL){
            x->right->parent = y;
        }
        x->parent = y->parent;
        if (y->parent == NULL){
            t = x;
        }
        else{
            if (y == y->parent->right){
                y->parent->right = x;
            }
            else{
                y->parent->left = x;
            }
        }
        x->right = y;
        y->parent = x;
    }
	void insertFixUp(fnode *& t, fnode *& node){
        fnode * p, * gp;
        //父节点存在，而且父节点红色;
        while (node->parent != NULL && node->parent->colour == 0){
            p = node->parent;
            gp = p->parent;//父节点为红色，故一定有祖父节点;
            if (p == gp->left){
                fnode * uncle = gp->right;
                //Case1:叔节点红色
                if (uncle != NULL && uncle->colour == 0){
                    uncle->colour = 1;
                    p->colour = 1;
                    gp->colour = 0;
                    node = gp;
                    continue;
                }
                //Case2:叔节点黑色，且当前节点为右孩子;
                if (p->right == node){
                    fnode * tmp;
                    left_rotate(t, p);
                    tmp = p;
                    p = node;
                    node = tmp;
                }
                //Case3:叔节点黑色，且当前节点为左孩子;
                p->colour = 1;
                gp->colour = 0;
                right_rotate(t, gp);
            }
            else{
                fnode * uncle = gp->left;
                //Case1:叔节点红色;
                if (uncle != NULL && uncle->colour == 0){
                    uncle->colour = 1;
                    p->colour = 1;
                    gp->colour = 0;
                    node = gp;
                    continue;
                }
                //Case2:叔节点黑色，且当前节点为左孩子;
                if (p->left == node){
                    fnode * tmp;
                    right_rotate(t, p);
                    tmp = p;
                    p = node;
                    node = tmp;
                }
                //Case3:叔节点黑色，且当前节点为右孩子;
                p->colour = 1;
                gp->colour = 0;
                left_rotate(t, gp);
            }
        }
        t->colour = 1;
	}
	pair<iterator, bool> insert1(const value_type &value, fnode *& t){
	    Compare com;
        fnode * y = NULL;
        fnode * x = t;
        while (x != NULL){
            y = x;
            if (com(value.first, x->data->first)){
                x = x->left;
            }
            else if (com(x->data->first, value.first)){
                x = x->right;
            }
            else{
                len--;
                iterator tmp(x, this);
                pair<iterator, bool> re(tmp, false);
                return re;
            }
        }
        fnode * node = new fnode(value, 0);
        node->parent = y;
        if (com(value.first, y->data->first)){
            y->left = node;
            node->pre = y->pre;
            node->next = y;
            y->pre->next = node;
            y->pre = node;
        }
        else{
            y->right = node;
            node->next = y->next;
            node->pre = y;
            y->next->pre = node;
            y->next = node;
        }
        x = node;
        insertFixUp(t, node);
        iterator tmp(x, this);
        pair<iterator, bool> re(tmp, true);
        return re;
	}
	void removeFixUp(fnode *& t, fnode *& node, fnode *& p){
        fnode * other;//node的兄弟节点;
        while ((node == NULL || node->colour == 1) && node != t){
            if (p->left == node){
                other = p->right;
                if (other->colour == 0){
                    other->colour = 1;
                    p->colour = 0;
                    left_rotate(t, p);
                    other = p->right;
                }
                if ((other->left == NULL || other->left->colour == 1) && (other->right == NULL || other->right->colour == 1)){
                    other->colour = 0;
                    node = p;
                    p = node->parent;
                }
                else{
                    if (other->right == NULL || other->right->colour == 1){
                        other->left->colour = 1;
                        other->colour = 0;
                        right_rotate(t, other);
                        other = p->right;
                    }
                    other->colour = p->colour;
                    p->colour = 1;
                    other->right->colour = 1;
                    left_rotate(t, p);
                    node = t;
                    break;
                }
            }
            else{
                other = p->left;
                if (other->colour == 0){
                    other->colour = 1;
                    p->colour = 0;
                    right_rotate(t, p);
                    other = p->left;
                }
                if ((other->left == NULL || other->left->colour == 1) && (other->right == NULL || other->right->colour == 1)){
                    other->colour = 0;
                    node = p;
                    p = node->parent;
                }
                else{
                    if (other->left == NULL || other->left->colour == 1){
                        other->right->colour = 1;
                        other->colour = 0;
                        left_rotate(t, other);
                        other = p->left;
                    }
                    other->colour = p->colour;
                    p->colour = 1;
                    other->left->colour = 1;
                    right_rotate(t, p);
                    node = root;
                    break;
                }
            }
        }
        if (node != NULL){
            node->colour = 1;
        }
	}
	void erase1(value_type& value, fnode *&t){
	    Compare com;
	    fnode * child, * p;
	    int color;
        fnode * node;
        node = &(find1(value.first, root).return_node());
        if (node->left != NULL && node->right != NULL){
            fnode * replace_node = node->next;
            if (node->parent != NULL){
                if (node->parent->left == node){
                    node->parent->left = replace_node;
                }
                else{
                    node->parent->right = replace_node;
                }
            }
            else t = replace_node;
            child = replace_node->right;
            p = replace_node->parent;
            color = replace_node->colour;
            if (p == node){
                p = replace_node;
            }
            else{
                if (child != NULL){
                    child->parent = p;
                }
                p->left = child;
                replace_node->right = node->right;
                node->right->parent = replace_node;
            }
            replace_node->parent = node->parent;
            replace_node->colour = node->colour;//important!
            replace_node->left = node->left;
            node->left->parent = replace_node;
            if (color == 1){
                removeFixUp(t, child, p);
            }
            node->pre->next = node->next;
            node->next->pre = node->pre;
            delete node;
            return;
        }
        if (node->left != NULL){
            child = node->left;
        }
        else{
            child = node->right;
        }
        p = node->parent;
        color = node->colour;
        if (child != NULL){
            child->parent = p;
        }
        if (p != NULL){
            if (p->left == node){
                p->left = child;
            }
            else{
                p->right = child;
            }
        }
        else{
            t = child;
        }
        if (color == 1){
            removeFixUp(t, child, p);
        }
        node->pre->next = node->next;
        node->next->pre = node->pre;
        delete node;
	}
	iterator find1(const Key &key, fnode*t)const{
	    Compare com;
        if (t == NULL) return tail;
        if (com(key, t->data->first)){
            return find1(key, t->left);
        }
        else if (com(t->data->first, key)){
            return find1(key, t->right);
        }
        iterator tmp(t, this);
        return tmp;
	}
	const_iterator find2(const Key &key, fnode*t)const{
	    Compare com;
        if (t == NULL){
            const_iterator tmp(tail);
            return tmp;
        }
        if (com(key, t->data->first)){
            return find2(key, t->left);
        }
        else if (com(t->data->first, key)){
            return find2(key, t->right);
        }
        const_iterator tmp(t, this);
        return tmp;
	}
public:
	map():head(this), tail(this), root(NULL) {
        head.return_node().next = &tail.return_node();
        head.return_node().pre = NULL;
        tail.return_node().pre = &head.return_node();
        tail.return_node().next = NULL;
        len = 0;
	}
	map(const map &other):head(this), tail(this), root(NULL){
        head.return_node().next = &tail.return_node();
        head.return_node().pre = NULL;
        tail.return_node().pre = &head.return_node();
        tail.return_node().next = NULL;
        len = 0;
        if (!other.empty()){
            iterator tmp = other.begin();
            while(tmp != other.end()){
                insert(*tmp);
                tmp++;
            }
        }
	}
	map & operator=(const map &other) {
        if (this == &other) return *this;
        if (!empty()) clear();
        if (!other.empty()){
            iterator tmp = other.begin();
            while(tmp != other.end()){
                insert(*tmp);
                tmp++;
            }
        }
        return *this;
    }
	~map() {
        clear();
        head.clear();
        tail.clear();
	}
	T & at(const Key &key) {
        iterator tmp = find(key);
        if (tmp == tail) throw index_out_of_bound();
        return (*tmp).second;
	}
	const T & at(const Key &key) const {
        const_iterator tmp = find(key);
        if (tmp == cend()) throw index_out_of_bound();
        return (*tmp).second;
	}
	T & operator[](const Key &key) {//insert when none
        iterator tmp = find(key);
        if (tmp == tail){
            T t;
            pair<Key, T> tm(key, t);
            insert(tm);
            tmp = find(key);
        }
        return (*tmp).second;
	}
	const T & operator[](const Key &key) const {
        const_iterator tmp = find(key);
        if (tmp == cend()) throw index_out_of_bound();
        return (*tmp).second;
	}
	iterator begin() const{
        return head+1;
	}
	const_iterator cbegin() const {
        const_iterator tmp(head+1);
        return tmp;
	}
	iterator end()const {
        return tail;
	}
	const_iterator cend() const {
        const_iterator tmp(tail);
        return tmp;
	}
	bool empty() const {
        return len == 0;
    }
	size_t size() const {
        return len;
	}
	void clear() {
        iterator tmp = head+1;
        while (tmp != tail){
            tmp++;
            (tmp-1).clear();
        }
        head.return_node().next = &tail.return_node();
        head.return_node().pre = NULL;
        tail.return_node().pre = &head.return_node();
        tail.return_node().next = NULL;
        len = 0;
        root = NULL;
	}
	pair<iterator, bool> insert(const value_type &value) {
	    len++;
	    if (len == 1){
            root = new fnode(value, 1);
            root->next = &tail.return_node();
            tail.return_node().pre = root;
            root->pre = &head.return_node();
            head.return_node().next = root;
            iterator it(root, this);
            pair<iterator, bool> re(it, true);
            return re;
	    }
        else{
            return insert1(value, root);
        }
	}
	void erase(iterator pos) {
        if (&pos.return_owner() != this) throw invalid_iterator();
        if (pos == end()) throw invalid_iterator();
        erase1(*pos, root);
        len--;
	}
	size_t count(const Key &key) const {
        if (find(key) == end()) return 0;
        return 1;
	}
	iterator find(const Key &key) {
        if (len == 0) return tail;
        return find1(key, root);
	}
	const_iterator find(const Key &key) const {
        if (len == 0){
            const_iterator tmp(tail);
            return tmp;
        }
        return find2(key, root);
	}
};

}

#endif
