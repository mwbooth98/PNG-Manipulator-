#include "qtvar.h"

bool qtvar::prunable(Node * root, int tol) {
    /* Your code here! */
    return(root->var<tol);
}
