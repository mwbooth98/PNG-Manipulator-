#include "qtcount.h"

bool qtcount::prunable(Node * root, int tol) {
    /* Your code here */
    if (root==NULL){
        return false;
    }
    bool flag = true;
    unsigned char r = root->avg.r;
    unsigned char g = root->avg.g;
    unsigned char b = root->avg.b;
    helper(root, tol, flag, r, g, b);
    return flag;
}

void qtcount::helper(Node * curr, int tol, bool& flag, unsigned char& r, unsigned char& g, unsigned char& b){
    if(curr->NW==NULL){
        if(((curr->avg.r-r)*(curr->avg.r-r))+((curr->avg.g-g)*(curr->avg.g-g)+((curr->avg.b-b)*(curr->avg.b-b)))>tol){
            flag=false;
            return;
        }
        return;
    }else{
        helper(curr->NW, tol, flag, r, g, b);
        helper(curr->NE, tol, flag, r, g, b);
        helper(curr->SW, tol, flag, r, g, b);
        helper(curr->SE, tol, flag, r, g, b);
    }
}