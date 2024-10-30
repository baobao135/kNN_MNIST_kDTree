#include "main.hpp"
#include "Dataset.hpp"
/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */
struct kDTreeNode
{
    vector<int> data;
    int label;
    kDTreeNode *left;
    kDTreeNode *right;
    kDTreeNode(vector<int> data, kDTreeNode *left = nullptr, kDTreeNode *right = nullptr)
    {
        this->data = data;
        this->label = 0;
        this->left = left;
        this->right = right;
    }
    kDTreeNode(vector<int> data, int label, kDTreeNode *left = nullptr, kDTreeNode *right = nullptr)
    {
        this->data = data;
        this->label = label;
        this->left = nullptr;
        this->right = nullptr;
    }

    friend ostream &operator<<(ostream &os, const kDTreeNode &node)
    {
        os << "(";
        for (int i = 0; i < node.data.size(); i++)
        {
            os << node.data[i];
            if (i != node.data.size() - 1)
            {
                os << ", ";
            }
        }
        os << ")";
        return os;
    }
};

class kDTree
{
public:
    int k;
    kDTreeNode *root;
    int count;
private:
    void deleteForDestructor(kDTreeNode *temp);
    kDTreeNode* copyTree(const kDTreeNode* temp);
    int heightRec(kDTreeNode *root) const;
    int leafRec(kDTreeNode *root) const;
    void inorderRec(kDTreeNode *root) const;
    void preorderRec(kDTreeNode *root) const;
    void postorderRec(kDTreeNode *root) const;
    kDTreeNode* insertRec(kDTreeNode *temp, const vector<int> &point, int depth);
    bool searchRec(kDTreeNode* temp, const vector<int> &point, int depth);
    kDTreeNode* findminRec(kDTreeNode *temp, int d, int depth);
    kDTreeNode* minNode(kDTreeNode* father, kDTreeNode* toleft, kDTreeNode* toright, int d);
    kDTreeNode* removeRec(kDTreeNode *temp, const vector<int> &point, int depth);
    kDTreeNode* buildTreeRec(const vector<vector<int>> &pointList, int depth);
    void merge (vector<vector<int>> &pointList, int l, int m, int r, int alpha);
    void mergeSort(vector<vector<int>> &pointList, int l, int r, int alpha);
    kDTreeNode* nearestNeighbourRec(kDTreeNode* temp, const vector<int> &target, kDTreeNode *&best, int depth);
    double distancePoint(const vector<int> A, const vector<int> B, int k);
    void kNearestNeighbourRec(kDTreeNode* temp, const vector<int> &target, int k, vector<kDTreeNode *> &bestList, int level);
    kDTreeNode* buildTreeLableRec(const vector<vector<int>> &pointList,const vector<int> &label,int depth);
public:
    kDTree(int k = 2);
    ~kDTree();
    const kDTree &operator=(const kDTree &other);
    kDTree(const kDTree &other);
    int nodeCount() const;
    int height() const;
    int leafCount() const;
    void inorderTraversal() const;
    void preorderTraversal() const;
    void postorderTraversal() const;
    void insert(const vector<int> &point);
    void remove(const vector<int> &point);
    bool search(const vector<int> &point);
    void buildTree(const vector<vector<int>> &pointList);
    void nearestNeighbour(const vector<int> &target, kDTreeNode*& best);
    void kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode*> &bestList);
    void buildTreeLable(const vector<vector<int>> &pointList,const vector<int> &label);
    friend class kNN;
};



class kNN
{
private:
    int k;
    Dataset *X_train;
    Dataset *y_train;
    kDTree  kdtree;
public:
    kNN(int k=5);
    void fit(Dataset &X_train, Dataset &y_train);
    Dataset predict(Dataset &X_test);
    double score(const Dataset &y_test, const Dataset &y_pred);


    //* test case thôi
    void print_Y(const Dataset& y)
    {
        OUTPUT << y.columnName[0] << ": ";
        for (auto it : y.data)
        {
            OUTPUT << it.front() << " ";
        }
        OUTPUT << endl;

    }
};

// Please add more or modify as needed
