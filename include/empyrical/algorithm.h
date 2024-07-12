/*
 * 自定义的一些算法及函数
 */

#include <vector>
#include <array>

/*
 * 基于递归的快速排序算法
 */
void quick_sorted ( std::int64_t first , std::int64_t last , double *data )
{
    if (first>=last){return;}
    std::int64_t lower, upper ;
    double ftemp, split ;

    split = data[(first+last)/2] ;
    lower = first ;
    upper = last ;

    do {
        while ( split > data[lower] )
            ++lower ;
        while ( split < data[upper] )
            --upper ;
        if (lower == upper) {
            ++lower ;
            --upper ;
        }
        else if (lower < upper) {
            ftemp = data[lower] ;
            data[lower++] = data[upper] ;
            data[upper--] = ftemp ;
        }
    } while ( lower <= upper ) ;

    if (first < upper)
        quick_sorted ( first , upper , data ) ;
    if (lower < last)
        quick_sorted ( lower , last , data ) ;
}


/*
 * 使用vector作为参数和返回值的基于递归的快速排序算法
 */
void quick_sorted(std::vector<double>& data, std::int64_t first = 0, std::int64_t last = -1) {
    if (last == -1) last = static_cast<std::int64_t>(data.size() - 1); // 初始化 last 索引
    if (first >= last) return; // 处理空向量或单个元素的情况

    std::int64_t lower, upper;
    double ftemp, split;

    split = data[(first + last) / 2];
    lower = first;
    upper = last;

    do {
        while (split > data[lower])
            ++lower;
        while (split < data[upper])
            --upper;
        if (lower == upper) {
            ++lower;
            --upper;
        } else if (lower < upper) {
            ftemp = data[lower];
            data[lower++] = data[upper];
            data[upper--] = ftemp;
        }
    } while (lower <= upper);

    if (first < upper) {
        quick_sorted(data, first, upper);
    }
    if (lower < last) {
        quick_sorted(data, lower, last);
    }
}

/*
 * 冒泡排序算法,使用数组作为参数
 * 通过不断交换相邻的元素，将较大的元素逐渐“冒泡”到数组的末尾。
 */
void bubble_sort(double arr[], std::size_t n, bool reverse=false) {
    if (n==0){return;}
    if (!reverse){
        for (std::size_t i = 0; i < n-1; i++) {
            for (std::size_t j = 0; j < n-i-1; j++) {
                if (arr[j] > arr[j+1]) {
                    std::swap(arr[j], arr[j+1]);
                }
            }
        }
    }else{
        for (std::size_t i = 0; i < n-1; i++) {
            for (std::size_t j = 0; j < n-i-1; j++) {
                if (arr[j] < arr[j+1]) {
                    std::swap(arr[j], arr[j+1]);
                }
            }
        }
    }

}


/*
 * 冒泡排序算法，使用vector作为参数
 * 通过不断交换相邻的元素，将较大的元素逐渐“冒泡”到数组的末尾。
 */
void bubble_sort(std::vector<double>& arr, bool reverse=false) {
    std::size_t n = arr.size();
    if (n==0){return;}
    if (!reverse){
        for (std::size_t i = 0; i < n-1; i++) {
            for (std::size_t j = 0; j < n-i-1; j++) {
                if (arr[j] > arr[j+1]) {
                    std::swap(arr[j], arr[j+1]);
                }
            }
        }
    }else{
        for (std::size_t i = 0; i < n-1; i++) {
            for (std::size_t j = 0; j < n-i-1; j++) {
                if (arr[j] < arr[j+1]) {
                    std::swap(arr[j], arr[j+1]);
                }
            }
        }
    }

}

/*
 * 选择排序：每次从未排序的部分选择最小的元素，放到已排序部分的末尾。
 */
void selection_sort(double arr[], std::size_t n, bool reverse=false) {
    if (n==0){return;}
    if(!reverse){
        for (std::size_t i = 0; i < n-1; i++) {
            std::size_t minIndex = i;
            for (std::size_t j = i+1; j < n; j++) {
                if (arr[j] < arr[minIndex]) {
                    minIndex = j;
                }
            }
            std::swap(arr[i], arr[minIndex]);
        }
    }else{
        for (std::size_t i = 0; i < n-1; i++) {
            std::size_t minIndex = i;
            for (std::size_t j = i+1; j < n; j++) {
                if (arr[j] > arr[minIndex]) {
                    minIndex = j;
                }
            }
            std::swap(arr[i], arr[minIndex]);
        }
    }

}

/*
 * 选择排序：每次从未排序的部分选择最小的元素，放到已排序部分的末尾。
 * 传入的参数使用vector
 */
void selection_sort(std::vector<double>& vec, bool reverse=false) {
    std::size_t n = vec.size();
    if (n<=1){return;}
    if (!reverse){
        for (std::size_t i = 0; i < n-1; i++) {
            std::size_t minIndex = i;
            for (std::size_t j = i+1; j < n; j++) {
                if (vec[j] < vec[minIndex]) {
                    minIndex = j;
                }
            }
            std::swap(vec[i], vec[minIndex]);
        }
    }else{
        for (std::size_t i = 0; i < n-1; i++) {
            std::size_t minIndex = i;
            for (std::size_t j = i+1; j < n; j++) {
                if (vec[j] > vec[minIndex]) {
                    minIndex = j;
                }
            }
            std::swap(vec[i], vec[minIndex]);
        }
    }

}
/*
 * 插入排序：将未排序的元素逐个插入到已排序部分的正确位置。
 * 参数使用数组
 */
void insertion_sort(double arr[], std::size_t n, bool reverse=false) {
    if (n==0){return;}
    if (!reverse){
        for (std::size_t i = 1; i < n; i++) {
            double key = arr[i];
            auto j = static_cast<std::int64_t>(i - 1);
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }else{
        for (std::size_t i = 1; i < n; i++) {
            double key = arr[i];
            auto j = static_cast<std::int64_t>(i - 1);
            while (j >= 0 && arr[j] < key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }

}

/*
 * 插入排序：将未排序的元素逐个插入到已排序部分的正确位置。
 * 参数使用vector
 */
void insertion_sort(std::vector<double>& arr, bool reverse=false) {
    std::size_t n = arr.size();
    if (n == 0) { return; }
    if (!reverse) {
        for (std::size_t i = 1; i < n; i++) {
            double key = arr[i];
            auto j = static_cast<std::int64_t>(i - 1);
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    } else {
        for (std::size_t i = 1; i < n; i++) {
            double key = arr[i];
            auto j = static_cast<std::int64_t>(i - 1);
            while (j >= 0 && arr[j] < key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }

    }
}

/*
 * 归并排序
 * 基本思想：将数组分成两个子数组，分别排序，然后合并。
 * 时间复杂度：O(n log n)
 */
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int L[n1], R[n2];
    for (int i = 0; i < n1; i++) {
        L[i] = arr[l + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[m + 1 + j];
    }
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < n1) {
        arr[k++] = L[i++];
    }
    while (j < n2) {
        arr[k++] = R[j++];
    }
}

void merge_sort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort(arr, l, m);
        merge_sort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}


/*
 * 堆排序
 * 基本思想：利用堆这种数据结构进行排序。
 * 时间复杂度：O(n log n)
 */
void heapify(int arr[], int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < n && arr[l] > arr[largest]) {
        largest = l;
    }
    if (r < n && arr[r] > arr[largest]) {
        largest = r;
    }
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heap_sort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}



