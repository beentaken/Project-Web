template <typename T>
class xor_list 
{
    public:
        class xor_node 
        {
            public:
                T* data_;
                xor_node* ptx;
        };
         
        xor_list();
        ~xor_list();
         
        int push_front(T* data);
        int push_back(T* data);
        int pop_front();
        int pop_back();
        int insert(T* data, int pos);
        int displayList(const char* msg);
        int size();
       
    private:

        xor_node* head_;
        xor_node* tail_;
        int size_;
         
        xor_node* XOR(xor_node* a, xor_node* b);
        xor_node* makeNode(T* data);
};

#include "xor_list.cpp"