
/**
 *
 * quadtree (pa3)
 * quadtree.cpp
 * This file will be used for grading.
 *
 */

#include "quadtree.h"
using namespace std;


// Node constructor, given.
quadtree::Node::Node(pair<int,int> ul, int d,RGBAPixel a, double v)
	:upLeft(ul),dim(d),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
	{}

// quadtree destructor, given.
quadtree::~quadtree() {
	clear();
}
// quadtree copy constructor, given.
quadtree::quadtree(const quadtree & other) {
	copy(other);
}
// quadtree assignment operator, given.
quadtree & quadtree::operator=(const quadtree & rhs) {
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}


quadtree::quadtree(PNG & imIn) {
    pair<int,int> ul(0,0);
    stats statsObject = stats(imIn);
    int dim;
    
    if(log2(imIn.width()) > (log2(imIn.height()))){
        dim = log2(imIn.height());
    } else {
        dim = log2(imIn.width());
    }
    // printf("Dimension = %d \n",dim);
    double var = statsObject.getVar(ul,dim);
    RGBAPixel a = statsObject.getAvg(ul,dim);
    int childDim = dim-1;
    root = new Node(ul,dim,a,var);
    edge = pow(2,dim);
    
    pair<int,int> NWUL = make_pair(ul.first,ul.second);
    pair<int,int> NEUL = make_pair(ul.first + (edge/2),ul.second);
    pair<int,int> SEUL = make_pair(ul.first + (edge/2),ul.second +(edge/2));
    pair<int,int> SWUL = make_pair(ul.first,ul.second +(edge/2));

// printf("Before entering BuildTree Recursive\n");

    
    root->NW = buildTree (statsObject,NWUL,childDim);
    // printf("finished NW\n");
    root->NE = buildTree (statsObject,NEUL,childDim);
    // printf("finished NE\n");
    root->SE = buildTree (statsObject,SEUL,childDim);
    // printf("finished SE\n");
    root->SW = buildTree (statsObject,SWUL,childDim);
    // printf("finished SW\n");


    

}


quadtree::Node * quadtree::buildTree(stats & s, pair<int,int> & ul, int dim) {
    //Check if current Node is pixel of size smaller than 1
    // printf("Dimension in recursive build = %d \n",dim);
    if(dim < 0){
        // printf("FOUND A DIM <0\n");
        return NULL;
        // printf("should have exited");
    }
    // printf("setting up recursive node\n");
    
    
    
    double var = s.getVar(ul,dim);
    RGBAPixel a = s.getAvg(ul,dim);
    Node *current = new Node(ul,dim,a,var);
    int childDim = dim-1;
    // printf("set node\n");
    
    
    
    pair<int,int> NWUL(ul);
    // printf("NWUL SET CORRECTLY\n");
    pair<int,int> NEUL = make_pair(ul.first + pow(2,childDim),ul.second);
    pair<int,int> SEUL = make_pair(ul.first + pow(2,childDim),ul.second +pow(2,childDim));
    pair<int,int> SWUL = make_pair(ul.first,ul.second + pow(2,childDim));


    //Possibly childDim is being modified by the recursion??!!??!
    // printf("setting children\n");
    // printf("DIMENSION BEFORE ENTERING RECURSIVE NW TREE IS: %d\n", childDim);
    current->NW = buildTree (s,NWUL,childDim);
    // printf("finished NE Recursive child\n");
    // printf("DIMENSION BEFORE ENTERING RECURSIVE NE TREE IS: %d\n", childDim);
    current->NE = buildTree (s,NEUL,childDim);
    // printf("finished NE Recursive child\n");
    current->SE = buildTree (s,SEUL,childDim);
    // printf("finished SE Recursive child\n");
    current->SW = buildTree (s,SWUL,childDim);
    // printf("finished SW Recursive child\n");
    return current;
    
    
        
            

        }




PNG quadtree::render() {
        PNG *output = new PNG(edge,edge);
        renderHelper(root,output);
        return *output;
        

}
void quadtree::renderHelper(Node *curr, PNG* png) {
        if(curr->NW ==NULL){ //found a leaf
            for(int x = curr->upLeft.first;x < pow(2,curr->dim) + curr->upLeft.first; x++){
                for(int y = curr->upLeft.second; y < pow(2,curr->dim) + curr->upLeft.second; y++){
                    RGBAPixel * toSet = png->getPixel(x,y);
                    *toSet = curr->avg;

                }
            }
        } else { //recursively search for leaf
        renderHelper(curr->NW ,png);
        renderHelper(curr->NE ,png);
        renderHelper(curr->SE ,png);
        renderHelper(curr->SW ,png);

        }
        

}


int quadtree::idealPrune(int leaves){
    return idealPruneHelper(leaves, 0, 10000);
}

int quadtree::idealPruneHelper(int leaves, int min, int max){
    if(min==max){
        return min-1;
    }
    int mid=int((min+max)/2);
    if(pruneSize(mid)<leaves){
        return idealPruneHelper(leaves, min, mid);
    }else{
        return idealPruneHelper(leaves, mid+1, max);
    }
}

int quadtree::pruneSize(int tol){
        return pruneSizeHelper(tol,root);
}

int quadtree::pruneSizeHelper(int tol, Node * curr){
    if (curr==NULL){
        return 0;
    }if(curr->NW==NULL){
        return 1;
    }if (prunable(curr, tol)){
        return 1;
    }else{
        return pruneSizeHelper(tol, curr->NW)+pruneSizeHelper(tol, curr->NE)+pruneSizeHelper(tol, curr->SE)+pruneSizeHelper(tol, curr->SW);
    }
}
        



void quadtree::prune(int tol){
    pruneHelper(tol,root);

}

void quadtree::pruneHelper(int tol, Node *curr){
    if (curr->NW == NULL) return;
    if(prunable(curr,tol)){
        clearHelper(curr->NW);
        clearHelper(curr->NE);
        clearHelper(curr->SW);
        clearHelper(curr->SE);
    } else {
        pruneHelper(tol, curr->NW);
        pruneHelper(tol, curr->NE);
        pruneHelper(tol, curr->SW);
        pruneHelper(tol, curr->SE);
    }

}

void quadtree::clear() {
    if(root->NW == NULL){
        delete root;
        root=NULL;
    } else {
        clearHelper(root);
    }
}

void quadtree::clearHelper(Node* &current){
if(current->NW == NULL){
    delete current;
    current=NULL;
} else {
    clearHelper(current->NW);
    clearHelper(current->NE);
    clearHelper(current->SW);
    clearHelper(current->SE);
    delete current;
    current=NULL;
}
}

void quadtree::copy(const quadtree & orig){
    root = copyHelper(orig.root);
    edge = orig.edge;
    }

   //Copy Constructor helper function
    quadtree::Node *quadtree :: copyHelper (const Node* newRoot)
    { 
    if (newRoot != NULL)
    {
        Node *node = new Node(newRoot->upLeft, newRoot->dim, newRoot->avg, newRoot->var);
        node->NW = copyHelper(newRoot->NW);
        node->NE = copyHelper(newRoot->NE);
        node->SW = copyHelper(newRoot->SW);
        node->SE = copyHelper(newRoot->SE);

        return node;    
    }
    else
        return NULL; 
     }


