// Copyright (c) 2022 LucaWei. All rights reserved.
#ifndef SRC_LCS_GEOMETRY_HPP_
#define SRC_LCS_GEOMETRY_HPP_

namespace lcs {

class Point {
 public:
    int x_, y_;

    Point() = default;
    virtual ~Point() = default;
    Point(int x, int y) : x_(x), y_(y) {}

    Point(const Point& pt) {
        x_ = pt.x_;
        y_ = pt.y_;
    }

    void operator=(const Point& pt) {
        x_ = pt.x_;
        y_ = pt.y_;
    }

    bool operator==(const Point& pt) const {
        return pt.x_ == x_ && pt.y_ == y_;
    }

    bool operator!=(const Point& pt) const {
        return pt.x_ != x_ || pt.y_ != y_;
    }

    bool operator<(const Point& pt)const {
        return (x_ < pt.x_) || (x_ == pt.x_ && y_ < pt.y_);
    }

    friend std::ostream& operator<<(std::ostream& os, const Point& pt) {
        os << "(" << pt.x_ << ", " << pt.y_ << ")";
        return os;
    }
};

class Rectangle {
 public:
    Point left_bottom_, right_top_;
    int num_;

    Rectangle() = default;

    virtual ~Rectangle() = default;

    Rectangle(Point lb, Point rt) : left_bottom_(lb), right_top_(rt) {}

    Rectangle(const Rectangle& rect) {
        num_ = rect.num_;
        left_bottom_ = rect.left_bottom_;
        right_top_ = rect.right_top_;
    }

    void operator=(const Rectangle& rect) {
        num_ = rect.num_;
        left_bottom_ = rect.left_bottom_;
        right_top_ = rect.right_top_;
    }
};


//                     RT
//                     ^
//                     |
//      ----------------
//      |              | ---> TR
//      |              |
//      |              |
//      | (lower left) |
//  BL <----------------
//      |
//      v
//      LB

enum TileType {
    space, block
};

class Tile {
    using TilePtrType = std::shared_ptr<Tile>;

 public:
    TilePtrType rt_, tr_, bl_, lb_;
    TileType type_;
    Point left_bottom_, right_top_;
    int num_;

    Tile() = default;

    virtual ~Tile() = default;

    Tile(Point lb, Point rt, TileType type)
    : left_bottom_(lb), right_top_(rt), type_(type) {
        rt_ = nullptr;
        tr_ = nullptr;
        bl_ = nullptr;
        lb_ = nullptr;
    }

    bool setPtr(const TilePtrType& rt, const TilePtrType& tr
    , const TilePtrType& bl, const TilePtrType& lb) {
        rt_ = rt;
        tr_ = tr;
        bl_ = bl;
        lb_ = lb;
        return true;
    }


    bool isInTile(const Point& pt) const {
        return
        pt.x_ >= left_bottom_.x_ &&
        pt.x_ < right_top_.x_ &&
        pt.y_ >= left_bottom_.y_ &&
        pt.y_ < right_top_.y_;
    }

    void info(std::ostream& os) {
        if (this == nullptr) {
        os << "null" << std::endl;
        } else {
            os << "Tile " << num_ << ": "
            << left_bottom_.x_ << " "
            << left_bottom_.y_ << "  "
            << right_top_.x_ << " "
            << right_top_.y_
            << std::endl;
        }
    }
};

}  // namespace lcs


#endif  // SRC_LCS_GEOMETRY_HPP_
