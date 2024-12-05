#include <iostream>
#include "bst.h"

int main(int, char**){
    BinarySearchTree<int>* bst = BinarySearchTree<int>::buildTree("(10(7(2()())(3()()))(12(11()())(15()())))");

    // bst->inorder();

    // std::cout << bst->sumOfElements() << std::endl; 

    delete bst;
}
