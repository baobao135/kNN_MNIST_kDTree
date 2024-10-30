#include "72_BTL2_DSA.hpp"
kDTree::kDTree(int k){
    this->k = k;
    this->root = nullptr;
    this->count = 0;
}
void kDTree::deleteForDestructor(kDTreeNode *temp){
    if(temp==NULL)   return;
    deleteForDestructor(temp->left);
    deleteForDestructor(temp->right);
    delete temp;
}
kDTree::~kDTree(){
    deleteForDestructor(root);
}
kDTreeNode* kDTree::copyTree(const kDTreeNode* temp){
    if(temp==NULL) return nullptr;
    kDTreeNode* newnode = new kDTreeNode(temp->data);
    newnode->left=copyTree(temp->left);
    newnode->right=copyTree(temp->right);
    return newnode;
}
const kDTree& kDTree:: operator=(const kDTree &other){
    this->k=other.k;
    this->count=other.count;
    this->root=copyTree(other.root);
    return *this;
}
kDTree::kDTree(const kDTree &other){
    this->k=other.k;
    this->count=other.count;
    this->root=copyTree(other.root);
}
int kDTree:: nodeCount() const{
    return this->count;
}
int kDTree::heightRec(kDTreeNode* root) const{
    if (root==NULL) return 0; 
    return 1+max(heightRec(root->left),heightRec(root->right));
}
int kDTree::height() const{
    return this-> heightRec(root);
}
int kDTree::leafRec(kDTreeNode* root) const{
    if(root==NULL) return 0;
    if(root->left==NULL && root->right==NULL) return 1;
    return leafRec(root->left)+leafRec(root->right);
}
int kDTree::leafCount() const{
    return this->leafRec(root);
}
void kDTree::inorderRec(kDTreeNode* root) const{
    if(root==NULL) return;
    inorderRec(root->left);
    OUTPUT<<*root<<" ";
    inorderRec(root->right);
}
void kDTree::inorderTraversal() const{
    this-> inorderRec(root);
}
void kDTree::preorderRec(kDTreeNode* root) const{
    if(root==NULL) return;
    OUTPUT<<*root<<" ";
    preorderRec(root->left);
    preorderRec(root->right);
}
void kDTree::preorderTraversal() const{
    this-> preorderRec(root);
}
void kDTree::postorderRec(kDTreeNode* root) const{
    if(root==NULL) return;
    postorderRec(root->left);
    postorderRec(root->right);
    OUTPUT<<*root<<" ";
}
void kDTree::postorderTraversal() const{
    this-> postorderRec(root);
}

kDTreeNode* kDTree::insertRec(kDTreeNode *temp, const vector<int> &point, int depth){
    if(temp==NULL) return new kDTreeNode(point);
    int alpha=depth%k;
    if(temp->data[alpha]>point[alpha]) temp->left=insertRec(temp->left,point,depth+1);
    else temp->right=insertRec(temp->right,point,depth+1);

    return temp;
}
void kDTree::insert(const vector<int> &point){
    this->root= insertRec(root,point,0);
    this->count++;
}
bool kDTree::searchRec(kDTreeNode* temp, const vector<int> &point, int depth){
    if(temp==NULL)  return false;
    if(temp->data==point) return true;
    int alpha=depth % k;
    if(point[alpha]<temp->data[alpha]) return searchRec(temp->left,point,depth+1);
    else if(temp->data[alpha]<=point[alpha]) return searchRec(temp->right,point,depth+1);
    return false;
}
bool kDTree::search(const vector<int> &point){
    return searchRec(root, point, 0);
}
kDTreeNode* kDTree::minNode(kDTreeNode* father, kDTreeNode* toleft, kDTreeNode* toright, int d){
    kDTreeNode* temp= father;
    if(toleft!=NULL && toleft->data[d] < temp->data[d])
        temp=toleft;
    if(toright!=NULL && toright->data[d] < temp->data[d])
        temp=toright;
    return temp;
}
kDTreeNode* kDTree::findminRec(kDTreeNode* temp, int d, int depth){
    if(temp==NULL) return NULL;
    int alpha=depth % k;
    
    if(alpha==d){
        if(temp->left==NULL)
            return temp;
        else if(temp->data[d]>temp->left->data[d])
            return findminRec(temp->left, d, depth+1);
    }
    return minNode(temp,findminRec(temp->left,d,depth+1),findminRec(temp->right,d,depth+1),d);
}

kDTreeNode* kDTree::removeRec(kDTreeNode *temp, const vector<int> &point, int depth) {
    if(temp==NULL) return NULL;     
    int alpha=depth % k;

    if(temp->data==point){   
        if(temp->right!=NULL){
            kDTreeNode* min=findminRec(temp->right, alpha, depth+1);
            temp->data=min->data;
            temp->right=removeRec(temp->right,min->data, depth+1);
        }
        else if(temp->left!=NULL){
            kDTreeNode* min=findminRec(temp->left, alpha, depth+1);
            temp->data=min->data;
            temp->right=temp->left;
            temp->left=NULL;                                       
            temp->right=removeRec(temp->right,min->data, depth+1);
        }
        else{
            delete temp;
            return NULL;
        }
        return temp;
    }

    if(temp->data[alpha]>point[alpha]) temp->left= removeRec(temp->left,point,depth+1);
    else temp->right= removeRec(temp->right,point,depth+1);
    
    return temp;
}
void kDTree::remove(const vector<int> &point){
    if(search(point)==false) return;
    this->root= removeRec(root,point,0);
    this->count--;
}
void kDTree::merge(vector<vector<int>> &pointList, int l, int m, int r,int alpha){
    int n1= m-l+1;
    int n2= r-m;
    vector<vector<int>> templeft(n1);  
    vector<vector<int>> tempright(n2); 

    for (int i = 0; i < n1; i++)
        templeft[i] = pointList[l +i];
    for (int j = 0; j < n2; j++)
        tempright[j] = pointList[m + 1 + j];

    int i=0, j=0;
    while(i < templeft.size() && j < tempright.size()){
        if(templeft[i][alpha]<=tempright[j][alpha]){
            pointList[l]=templeft[i];
            ++l;
            ++i;
        }
        else{
            pointList[l]=tempright[j];
            ++l;
            ++j;
        }
    }
    while(i<templeft.size()){
        pointList[l]=templeft[i];
        ++l;
        ++i;
    }
    while(j<tempright.size()){
        pointList[l]=tempright[j];
        ++l;
        ++j;
    }
}
void kDTree::mergeSort(vector<vector<int>> &pointList, int l, int r, int alpha){
    if(l>=r) return;
    int m=(l+r)/2;
    mergeSort(pointList,l,m,alpha);
    mergeSort(pointList,m+1,r,alpha);
    merge(pointList,l,m,r,alpha);
}
kDTreeNode* kDTree::buildTreeRec(const vector<vector<int>> &pointList, int depth){
    vector<vector<int>> point=pointList;
    if(pointList.size()==0) return NULL;
    int alpha=depth % this->k;
    mergeSort(point, 0, point.size()-1, alpha);
    int mid=(point.size()-1)/2;
    while(mid-1>=0){
        if(point[mid][alpha]==point[mid-1][alpha])
            mid=mid-1;
        else break;
    }
    kDTreeNode* root=new kDTreeNode(point[mid]);
    this->count++;
    vector<vector<int>> templeft(point.begin(),point.begin()+mid);
    vector<vector<int>> tempright(point.begin()+mid+1,point.end());

    root->left=buildTreeRec(templeft,depth+1);
    root->right=buildTreeRec(tempright,depth+1);
    return root;
}
void kDTree::buildTree(const vector<vector<int>> &pointList){
    this->root=this->buildTreeRec(pointList,0);
}
double kDTree::distancePoint(const vector<int> A, const vector<int> B, int k){
    double d=0;
    for(int i=0;i< k;i++){
        d+=pow(A[i]-B[i],2);
    }
    return sqrt(d);
}
kDTreeNode* kDTree::nearestNeighbourRec(kDTreeNode* temp, const vector<int> &target, kDTreeNode* &best, int depth){
    if(temp==NULL) return NULL; 
    int alpha=depth % this->k;
    if(temp->left==NULL && temp->right==NULL){
        if(best==NULL){
            best=temp;
        }
        else{
            double d=distancePoint(temp->data, target, this->k);
            double R=distancePoint(target, best->data, this->k);
            if(d<R){
                best=temp;
            }
        }
        return best;
    }
    if(target[alpha] < temp->data[alpha]){
        if(temp->left!=NULL)
            best=nearestNeighbourRec(temp->left, target, best, depth+1);
        if(best==NULL)
        {
            best=temp;
        }
        double r=abs(target[alpha] - temp->data[alpha]);
        double d=distancePoint(temp->data, target, this->k);
        double R=distancePoint(target, best->data, this->k);
        if(d<R){
            best=temp;
        }
        
        if(r<R){
            if(temp->right!=NULL)
                best=nearestNeighbourRec(temp->right, target, best, depth+1);
        }
    
        
    }
    else{
        if(temp->right!=NULL)
            best=nearestNeighbourRec(temp->right, target, best, depth+1);
        if(best==NULL)
        {
            best=temp;
        }
        double r=abs(target[alpha] - temp->data[alpha]);
        double d=distancePoint(temp->data, target, this->k);
        double R=distancePoint(target, best->data, this->k);
        if(d<R){
            best=temp;
        }
        if(r<R){
            if(temp->left!=NULL)
                best=nearestNeighbourRec(temp->left, target, best, depth+1);
        }
        
        
    }
    return best;
}
void kDTree::nearestNeighbour(const vector<int> &target, kDTreeNode *&best){
    best=NULL;
    nearestNeighbourRec(this->root, target, best, 0);
}
void kDTree::kNearestNeighbourRec(kDTreeNode* temp, const vector<int> &target, int k, vector<kDTreeNode *> &bestList, int depth){
    if(temp==NULL) return; 
    int alpha= depth % this->k;
    if(temp->left==NULL && temp->right==NULL){
        bestList.push_back(temp);
        int n=bestList.size()-1;
        while(n>0){
            if(distancePoint(bestList[n]->data, target, this->k) < distancePoint(bestList[n-1]->data, target, this->k)){
                swap(bestList[n-1], bestList[n]);
            }
            else break;
            n=n-1;
        }
        if(bestList.size()>k){
            bestList.pop_back();
        }
        return;
    }

    if(target[alpha] < temp->data[alpha]){
        kNearestNeighbourRec(temp->left, target, k, bestList, depth+1);
        bestList.push_back(temp);
        int n=bestList.size()-1;
        while(n>0){
            if(distancePoint(bestList[n]->data, target, this->k) < distancePoint(bestList[n-1]->data, target, this->k)){
                swap(bestList[n-1], bestList[n]);
            }
            else break;
            n=n-1;
        }
        if(bestList.size()>k){
            bestList.pop_back();
        }
        
        double r=abs(target[alpha] - temp->data[alpha]);
        double R=distancePoint(target, bestList[bestList.size()-1]->data, this->k);
        if(r<R || bestList.size()!=k){
            kNearestNeighbourRec(temp->right, target, k, bestList, depth+1);
        }
     }
    else{
        kNearestNeighbourRec(temp->right, target, k, bestList, depth+1);
        bestList.push_back(temp);
        int n=bestList.size()-1;
        while(n>0){
            if(distancePoint(bestList[n]->data, target, this->k) < distancePoint(bestList[n-1]->data, target, this->k)){
                swap(bestList[n-1], bestList[n]);
            }
            else break;
            n=n-1;
        }
        if(bestList.size()>k){
            bestList.pop_back();
        }
        
        double r=abs(target[alpha] - temp->data[alpha]);
        double R=distancePoint(target, bestList[bestList.size()-1]->data, this->k);
        if(r<R || bestList.size()!=k){
            kNearestNeighbourRec(temp->left, target, k ,bestList, depth+1);
        }
    }
}
void kDTree::kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode *> &bestList){
    kNearestNeighbourRec(this->root, target, k, bestList, 0);
}
void mergeLabel(vector<vector<int>> &pointList, vector<int> &label, int l, int m, int r,int alpha){
    int n1= m-l+1;
    int n2= r-m;
    vector<vector<int>> templeft(n1);  
    vector<vector<int>> tempright(n2); 
    vector<int> labelleft(n1);
    vector<int> labelright(n2);

    for (int i = 0; i < n1; i++)
        templeft[i] = pointList[l +i];
    for (int j = 0; j < n2; j++)
        tempright[j] = pointList[m + 1 + j];
    for (int i = 0; i < n1; i++)
        labelleft[i] = label[l +i];
    for (int j = 0; j < n2; j++)
        labelright[j] = label[m + 1 + j];
    int i=0, j=0;
    while(i < templeft.size() && j < tempright.size()){
        if(templeft[i][alpha]<=tempright[j][alpha]){
            pointList[l]=templeft[i];
            label[l]=labelleft[i];
            ++l;
            ++i;
        }
        else{
            pointList[l]=tempright[j];
            label[l]=labelright[j];
            ++l;
            ++j;
        }
    }
    while(i<templeft.size()){
        pointList[l]=templeft[i];
        label[l]=labelleft[i];
        ++l;
        ++i;
    }
    while(j<tempright.size()){
        pointList[l]=tempright[j];
        label[l]=labelright[j];
        ++l;
        ++j;
    }
}
void mergeSortLabel(vector<vector<int>> &pointList, vector<int> &label, int l, int r, int alpha){
    if(l>=r) return;
    int m=(l+r)/2;
    mergeSortLabel(pointList, label,l,m,alpha);
    mergeSortLabel(pointList,label,m+1,r,alpha);
    mergeLabel(pointList,label,l,m,r,alpha);
}
kDTreeNode* kDTree::buildTreeLableRec(const vector<vector<int>> &pointList,const vector<int> &label, int depth){
    vector<vector<int>> point=pointList;
    vector<int> templabel= label;
    if(pointList.size()==0) return NULL;
    int alpha=depth % this->k;
    mergeSortLabel(point, templabel, 0, point.size()-1, alpha);
    int mid=(point.size()-1)/2;
    while(mid-1>=0){
        if(point[mid][alpha]==point[mid-1][alpha])
            mid=mid-1;
        else break;
    }
    kDTreeNode* root=new kDTreeNode(point[mid], templabel[mid], NULL, NULL);

    vector<vector<int>> templeft(point.begin(),point.begin()+mid);
    vector<vector<int>> tempright(point.begin()+mid+1,point.end());
    vector<int> labelleft(templabel.begin(),templabel.begin()+mid);
    vector<int>labelright(templabel.begin()+mid+1,templabel.end());

    root->left=buildTreeLableRec(templeft,labelleft,depth+1);
    root->right=buildTreeLableRec(tempright,labelright,depth+1);
    return root;
}
void kDTree::buildTreeLable(const vector<vector<int>> &pointList,const vector<int> &label){
    this->count = pointList.size();
    this->root= this->buildTreeLableRec(pointList, label, 0);
}
kNN::kNN(int k){
    this->k=k;
}
void kNN::fit(Dataset &X_train, Dataset &y_train){
    this->X_train=&X_train; 
    this->y_train=&y_train;
    if(X_train.data.size()){
        int dim=X_train.data.front().size();
        kdtree.k=dim;
        vector<vector<int>> pointList;
        vector<int> label;
        for(auto i:X_train.data){
            vector<int> temp;
            for(auto j:i)
                temp.push_back(j);
            pointList.push_back(temp);
        }
        for(auto i:y_train.data)
            label.push_back(i.front());
        kdtree.buildTreeLable(pointList,label);
    }

}
Dataset kNN::predict(Dataset &X_test){
    Dataset result;
    result.columnName.push_back("label");
    for(auto i:X_test.data){
        vector<kDTreeNode*> bestList;
        list<int> tempbest;
        vector<int> X;
        for(auto j:i){
            X.push_back(j);
        }
        kdtree.kNearestNeighbour(X, this->k, bestList);
        int arr[2]={0,0};
            for(int l=0;l<bestList.size();l++){
                int cnt=0;
                for(int j=0;j<bestList.size();j++){
                    if(bestList[l]->label==bestList[j]->label){
                        cnt++;
                    }
                }
                if(arr[0]<cnt){
                    arr[0]=cnt;
                    arr[1]=bestList[l]->label;
                }
                else if(arr[0]==cnt){
                    if(bestList[l]->label<arr[1]) 
                        arr[1]=bestList[l]->label;
                }
            }
            
        tempbest.push_back(arr[1]);
        result.data.push_back(tempbest);
    }
    
    return result;
}
double kNN::score(const Dataset &y_test, const Dataset &y_pred){
    if(y_pred.data.size()==0||y_test.data.size()==0) return -1;
        if(y_pred.data.size()!=y_test.data.size()) return -1;
    double count=0;
    vector<int> y1;
    vector<int> y2;
    for(auto i:y_pred.data)
        y1.push_back(i.front());
    for(auto j:y_test.data)
        y2.push_back(j.front());
    for(int i=0;i<y_test.data.size();i++){
        if(y1[i]==y2[i])
            count++;
    }
    return double(count/y_test.data.size());
}