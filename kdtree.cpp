
#include "kdtree.h"


Kdtree::Kdtree(vector<vector<double> > points){
        root = createBalancedTree(points, 0);
}


void Kdtree::dimensional_partial_sort(vector<vector<double> >& points, unsigned idx){

        std::nth_element(points.begin(), points.begin() + points.size()/2, points.end(), [idx](const vector<double>& a, vector<double>& b) -> bool {
                return a[idx] < b[idx];
        });

}

NodePtr Kdtree::createBalancedTree(vector<vector<double> > points, unsigned depth){

        NodePtr root =make_shared<Node>();

        if(points.size() == 0) {
                return nullptr;
        }

        unsigned current_dimension = depth%dimension;

        unsigned middle = points.size() / 2;
        dimensional_partial_sort(points, current_dimension);
        for (int i=0; i<dimension; i++)
                root->point[i] = points[middle][i];

        vector<vector<double> > left_points(points.begin(), points.begin() + middle);
        vector<vector<double> > right_points(points.begin() + middle + 1, points.end());

        root->left = createBalancedTree(left_points, depth+1);
        root->right = createBalancedTree(right_points, depth+1);

        return root;

}

void Kdtree::searchNNearest_(
        const NodePtr& root,
        const vector<double> point,
        const size_t depth,
        const int N,
        vector<vector<double> >& heap
        ) {

        if (root==nullptr) {
                return;
        }

        using std::placeholders::_1;
        using std::placeholders::_2;

        //initialize the heap
        make_heap(heap.begin(), heap.end(), bind(cmp, point, _1, _2));

        unsigned current_dimension = depth%dimension;
        double dist_to_box = root->point[current_dimension] - point[current_dimension];

        vector<double> farthest_neighbor;
        double farthest_neighbor_distance;

        if(heap.size()>0) {
                pop_heap(heap.begin(), heap.end(), bind(cmp, point, _1, _2));
                farthest_neighbor = heap.back();
                farthest_neighbor_distance = euclideanDist(farthest_neighbor, point);
        }

        if(heap.size() < N) {
                vector<double> temp;
                for(int i = 0; i < dimension; i++)
                        temp.push_back(root->point[i]);
                heap.push_back(temp);

                pop_heap(heap.begin(), heap.end(), bind(cmp, point, _1, _2));
                farthest_neighbor = heap.back();
                farthest_neighbor_distance = euclideanDist(farthest_neighbor, point);
        }else if (euclideanDist(root->point, point) < farthest_neighbor_distance) {
                heap.pop_back();
                vector<double> temp;
                for(int i = 0; i < dimension; i++)
                        temp.push_back(root->point[i]);
                heap.push_back(temp);
        }


        NodePtr close;
        NodePtr far;

        if (dist_to_box > 0) {
                close = root->left;
                far = root->right;
        } else {
                close = root->right;
                far = root->left;
        }

        size_t level = depth + 1;

        searchNNearest_(close, point, level, N, heap);

        //dist_to_box*dist_to_box is the shortest distance to the box that includes all of the points under far node
        //only traverse through these nodes if there is possibility of finding a node with distance shorter than farthest_neighbor_distance
        if (dist_to_box*dist_to_box < farthest_neighbor_distance) {
                searchNNearest_(far, point, level, N, heap);
        }

        return;
};


vector<vector<double> > Kdtree::searchNNearest(
        const vector<double> point,
        const int N
        ){

        vector<vector<double> > res;
        searchNNearest_(root, point, 0, N, res);

        return res;
}

double Kdtree::euclideanDist(const vector<double> point1, const vector<double> point2){

        double squared_dist = 0;
        for(int i = 0; i < dimension; i++)
                squared_dist += (point1[i]-point2[i])*(point1[i]-point2[i]);

        return squared_dist;
}

double Kdtree::euclideanDist(const double point1[], const vector<double> point2){

        double squared_dist = 0;
        for(int i = 0; i < dimension; i++)
                squared_dist += (point1[i]-point2[i])*(point1[i]-point2[i]);

        return squared_dist;
}

vector<vector<double> > Kdtree::bruteForceNNearest(vector<vector<double> > tree, vector<double> point, int N){

        sort(tree.begin(), tree.end(), [point](const vector<double>& a, vector<double>& b){
                return euclideanDist(a, point)<euclideanDist(b, point);
        });

        vector<vector<double> > res(tree.begin(), tree.begin() + min(N, (int)tree.size()));

        return res;
}
