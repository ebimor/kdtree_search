#ifndef KD_TREE_H
#define KD_TREE_H

#include <memory>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;
const int dimension = 3;

/**
   @brief Node class used for holding data points in kdtree data structure
 */

class Node
{
public:
    using NodePtr = shared_ptr<Node>;
    double point[dimension];
    NodePtr left;
    NodePtr right;

    Node() = default;
    ~Node() = default;
};
using NodePtr = shared_ptr<Node>;

/**
   @brief k-dtree structure class used for find nearest neighbor search
 */

class Kdtree {

private:
    NodePtr root;
    /**
     * @brief Helper function for partially sorting the dataset and finding the median
     */

    void dimensional_partial_sort(vector<vector<double> >& points, unsigned idx);

    /**
     * @brief Function to create a balance k-d tree from the datapoints
     */
    NodePtr createBalancedTree(vector<vector<double> > points, unsigned depth = 0);

    /**
       @brief main function that traverse through the tree to find N nearest points to the input "point"
     */
    void searchNNearest_(
            const NodePtr& root,
            const vector<double> point,
            const size_t depth,
            const int N,
            vector<vector<double> >& heap
            );

public:
    using KdtreePtr = shared_ptr<Kdtree>;

    Kdtree() = default;
    ~Kdtree() = default;
    explicit Kdtree(vector<vector<double> > points);
    /**
     * @brief Wrapper for calling the n nearest neighbor search
     */
    vector<vector<double> > searchNNearest(
            const vector<double> point,
            const int N
            );
    /**
    @brief (overloaded) function for calculating squared euclidean distance between points
    */
    static double euclideanDist(const vector<double> point1, const vector<double> point2);

    /**
    @brief (overloaded) function for calculating squared euclidean distance between points
    */
    static double euclideanDist(const double point1[], const vector<double> point2);

    /**
    @brief compare function used in the heap
    */
    static bool cmp(const vector<double> point, const vector<double>& a, const vector<double>& b){
      return euclideanDist(a, point)<euclideanDist(b, point);
    };

    /**
     * @brief Helper function for debuging and verification
     */
    static vector<vector<double> > bruteForceNNearest(vector<vector<double> > tree, vector<double> point, int N);
};
#endif
