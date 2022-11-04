// Copyright (c) 2022 LucaWei. All rights reserved.
#ifndef SRC_LCS_UTILITY_HPP_
#define SRC_LCS_UTILITY_HPP_

namespace lcs {

class MaxX {
 public:
    bool operator()(const Rectangle& r1, const Rectangle& r2) const {
        return r1.right_top_.x_ < r2.right_top_.x_;
    }
};

class MinX {
 public:
    bool operator()(const Rectangle& r1, const Rectangle& r2) const {
        return r1.left_bottom_.x_ > r2.left_bottom_.x_;
    }
};

class MaxY {
 public:
    bool operator()(const Rectangle& r1, const Rectangle& r2) const {
        return r1.right_top_.y_ < r2.right_top_.y_;
    }
};

class MinY {
 public:
    bool operator()(const Rectangle& r1, const Rectangle& r2) const {
        return r1.left_bottom_.y_ > r2.left_bottom_.y_;
    }
};

class ComRectNum {
 public:
    bool operator()(const Rectangle& r1, const Rectangle& r2) const {
        return r1.num_ < r2.num_;
    }
};

class ComTileNum {
 public:
    bool operator()(const std::shared_ptr<lcs::Tile>& t1, const std::shared_ptr<lcs::Tile>& t2) const {
        return t1->num_ < t2->num_;
    }
};

class Timer {
 public:
    void start() {
        t_start = std::chrono::high_resolution_clock::now();
    }

    void stop() {
        t_stop = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> d = t_stop - t_start;
        total = d.count();
    }

    double elapsedTime() { return total;}

    static double resolution() {
        auto tmp = std::chrono::high_resolution_clock::period();
        return static_cast<double>(tmp.num) / tmp.den;
    }

 private:
    std::chrono::high_resolution_clock::time_point t_start, t_stop;
    double total = 0;
};


}  // namespace lcs
#endif  //  SRC_LCS_UTILITY_HPP_
