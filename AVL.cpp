
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <vector>
using namespace std;

template<typename T>
class AVL{
    private:
        struct Node{
            T data;
            int height = 0;
            Node* left = nullptr;
            Node* right = nullptr;

            Node(T _data) : data(_data){};
        };

        Node* root;

        int height(Node* node){
            if(!node)
                return -1;
            else
                return node->height;
        }

        void update_height(Node* node){
            node->height = max(height(node->left), height(node->right)) + 1;
        }

        void l_rotation(Node*& node){
            Node* temp = node->left;
            node->left = temp->right;
            temp->right = node;
            node->height = max(height(node->left), height(node->right)) + 1;
            temp->height = max(height(temp->left), node->height) + 1;
            node = temp;
        }

        void r_rotation(Node*& node){
            Node* temp = node->right;
            node->right = temp->left;
            temp->left = node;
            node->height = max(height(node->right), height(node->left)) + 1;
            temp->height = max(height(temp->right), node->height) + 1;
            node = temp;
        }

        void lr_rotation(Node*& node){
            l_rotation(node->right);
            r_rotation(node);
        }

        void rl_rotation(Node*&  node){
            r_rotation(node->left);
            l_rotation(node);
        }

        int balance_factor(Node* node){
            return height(node->left) - height(node->right);
        }

        void balance(Node*& node){
            if(!node)
                return;

            if(balance_factor(node) > 1)
                if(balance_factor(node->left) >= 0)
                    l_rotation(node);
                else
                    rl_rotation(node);
            else
                if(balance_factor(node) < -1)
                    if(balance_factor(node->right) <= 0)
                        r_rotation(node);
                    else
                        lr_rotation(node);

            update_height(node);
        }

        T findMin(Node* node){
            if(!node)
                return NULL;
            if(!node->left)
                return node->data;

            return findMin(node->left);
        }

        void insert(Node*& current, T _data){
            if(!current)
                current = new Node(_data);
            else if(_data < current->data)
                insert(current->left, _data);
            else if(_data > current->data)
                insert(current->right, _data);

            balance(current);
        }

        void erase(Node*& current, T item){
            if(!current)
                return;

            if(item < current->data)
                erase(current->left, item);
            else if(item > current->data)
                erase(current->right, item);
            else if(current->left && current->right){
                T min = findMin(current->right);
                current->data = min;
                erase(current->right, min);
            }
            else{
                Node* temp = current;
                if(current->left)
                    current = current->left;
                else
                    current = current->right;
                delete temp;
            }

            balance(current);
        }

        bool search(Node* current, T item){
            if(!current)
                return false;
            else if(item < current->data)
                return search(current->left, item);
            else if(item > current->data)
                return search(current->right, item);
            else
                return true;
        }

        void range_search(Node* current, T begin, T end, vector<T>& v){
            if(!current)
                return;

            if(current->data > begin)
                range_search(current->left, begin, end, v);
            if(current->data >= begin && current->data <= end)
                v.push_back(current->data);
            if(current->data < end)
                range_search(current->right, begin, end, v);
        }

        void pre_order(Node* current){
            if(current){
                cout << current->data << " ";
                pre_order(current->left);
                pre_order(current->right);
            }
        }

        void in_order(Node* current){
            if(current){
                in_order(current->left);
                cout << current->data << " ";
                in_order(current->right);
            }
        }

        void post_order(Node* current){
            if(current){
                post_order(current->left);
                post_order(current->right);
                cout << current->data << " ";
            }
        }

        void breadth_first_search(){
            queue<Node*> q;
            q.push(root);
            while(!q.empty()){
                Node* temp = q.front();
                q.pop();
                cout << temp->data << " ";
                if(temp->left)
                    q.push(temp->left);
                if(temp->right)
                    q.push(temp->right);
            }
        }

        void depth_first_search(){
            stack<Node*> s;
            Node* temp = root;

            while(temp || !s.empty()){
                while(temp){
                    s.push(temp);
                    temp = temp->left;
                }
                temp = s.top();
                s.pop();
                cout << temp->data << " ";
                temp = temp->right;
            }
        }

    public:
        AVL(): root(nullptr){};

        void insert(T _data){
            insert(root, _data);
        }

        void erase(T item){
            erase(root, item);
        }

        bool search(T item){
            return search(root, item);
        }

        vector<T> range_search(T begin, T end){
            vector<T> result;
            range_search(root, begin, end, result);
            return result;
        }

        void print(string traversal = "inorder"){
            if(!root){
                cout << "EMPTY TREE" << endl;
                return;
            }

            if(traversal == "preorder"){
                cout << "PRE ORDER: ";
                pre_order(root);
            }
            else if(traversal == "inorder"){
                cout << "IN ORDER: ";
                in_order(root);
            }
            else if(traversal == "postorder"){
                cout << "POST ORDER: ";
                post_order(root);
            }
            else if(traversal == "BFS"){
                cout << "BFS: ";
                breadth_first_search();
            }
            else if(traversal == "DFS"){
                cout << "DFS: ";
                depth_first_search();
            }
            else{
                cout << "Invalid input. Defaulting to inorder" << "\n";
                cout << "IN ORDER: ";
                in_order(root);
            }

            cout << "\n";
        }
};

int main(){

    AVL<int> avlTree;


    avlTree.insert(10);
    avlTree.insert(20);
    avlTree.insert(5);
    avlTree.insert(6);
    avlTree.insert(15);
    avlTree.insert(30);


    cout << "Árbol después de las inserciones:" << endl;
    avlTree.print("inorder");
    avlTree.print("preorder");
    avlTree.print("postorder");
    avlTree.print("BFS");
    avlTree.print("DFS");


    cout << "\nBuscar 15: " << (avlTree.search(15) ? "Encontrado" : "No encontrado") << endl;
    cout << "Buscar 100: " << (avlTree.search(100) ? "Encontrado" : "No encontrado") << endl;


    avlTree.erase(10);
    cout << "\nÁrbol después de eliminar 10:" << endl;
    avlTree.print("inorder");


    vector<int> rangeResult = avlTree.range_search(5, 20);
    cout << "\nElementos en el rango 5-20: ";
    for (int val : rangeResult) {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}