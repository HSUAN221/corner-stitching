// Copyright (c) 2022 LucaWei. All rights reserved.
#ifndef SRC_LCS_LAYOUT_HPP_
#define SRC_LCS_LAYOUT_HPP_

namespace lcs {

class Layout {
    using TilePtrType = std::shared_ptr<Tile>;

 private:
    TilePtrType newTile(Point left_bottom, Point right_top, TileType type);

    TilePtrType splitTopSpaceTile(TilePtrType top_space_tile,
    Point left_bottom, Point right_top);

    TilePtrType splitBottomSpaceTile(TilePtrType bottom_space_tile,
    Point left_bottom, Point right_top);

    void splitCenterSpaceTile(TilePtrType center_space_tile, TilePtrType new_block_tile,
    TilePtrType top_space_tile, TilePtrType bottom_space_tile, TilePtrType& rt_of_left_tile);

    bool isTopEdgeOnTile(TilePtrType top_space_tile,
    Point left_bottom, Point right_top) {
        return right_top.y_ == top_space_tile->right_top_.y_
        || right_top.y_ == top_space_tile->left_bottom_.y_;
    }

    bool isBottomEdgeOnTile(TilePtrType bottom_space_tile,
    Point left_bottom, Point right_top) {
        return left_bottom.y_ == bottom_space_tile->right_top_.y_ ||
        left_bottom.y_ == bottom_space_tile->left_bottom_.y_;
    }

    void mergeRightTiles(std::vector<TilePtrType> all_right_tiles);

    void mergeLeftTiles(std::vector<TilePtrType> all_left_tiles);

    void removeTile(TilePtrType tile); /*only developer cat use it*/

    void RProcedure(TilePtrType tile, std::vector<TilePtrType>& tile_lsit, Point left_bottom, Point right_top);

    void mergeTiles(TilePtrType tile);

 public:
    std::shared_ptr<InputData> input_data_;
    int num_of_tiles_;
    TilePtrType back_tile_;
    std::set<TilePtrType, ComTileNum> block_tile_list_;

    Layout() = default;

    virtual ~Layout() = default;

    void readFile(ReadFileInterface&& readfile_object, const std::string& filename);

    void initialize();

    TilePtrType PointFinding(Point target);

    bool AreaSearching(Point left_bottom, Point right_top);

    bool createTile(Point left_bottom, Point right_top, int num);

    std::vector<TilePtrType> NeighborFinding(TilePtrType tile);

    std::vector<TilePtrType> NeighborFindingLeft(TilePtrType tile);

    std::vector<TilePtrType> NeighborFindingTop(TilePtrType tile);

    std::vector<TilePtrType> NeighborFindingBottom(TilePtrType tile);

    std::vector<TilePtrType> getNeighbors(TilePtrType tile);

    std::vector<TilePtrType> enumerate(Point left_bottom, Point right_top);
};

// ************************************** //
//                implement               //
// ************************************** //

inline void Layout::readFile(ReadFileInterface&& readfile_object, const std::string& filename) {
    input_data_ = readfile_object.readFile(filename);
}

inline void Layout::initialize() {
    num_of_tiles_ = 0;
    // first space tile
    back_tile_ = newTile(Point(0, 0), Point(input_data_->width_,
    input_data_->height_), TileType::space);
}

std::vector<Layout::TilePtrType> Layout::enumerate(Point left_bottom, Point right_top) {
    std::vector<TilePtrType> tiles;
    std::queue<TilePtrType> tile_q;

    // Use the PointFinding algorithm to locate the tile at the top left corner of the area of interest.
    // Then step down through all the tiles along the left edge.
    Point top_point(left_bottom.x_, right_top.y_);
    auto iter_tile = PointFinding(top_point);
    if (!iter_tile) {
        iter_tile = PointFinding(Point(left_bottom.x_, right_top.y_-1));
    }
    while (iter_tile && iter_tile->right_top_.y_ > left_bottom.y_) {
        tile_q.push(iter_tile);
        Point next_pt(left_bottom.x_, iter_tile->left_bottom_.y_-1);
        iter_tile = PointFinding(next_pt);
    }

    while (!tile_q.empty()) {
        auto tile = tile_q.front();
        tile_q.pop();
        if (tile->left_bottom_.y_ < right_top.y_) {
            RProcedure(tile, tiles, left_bottom, right_top);
        }
    }

    return tiles;
}

void Layout::RProcedure(Layout::TilePtrType current_tile, std::vector<Layout::TilePtrType>& tile_list,
Point left_bottom, Point right_top) {
    // R1: enumerate the tile
    tile_list.push_back(current_tile);

    // R2: If the right edge of the tile is outside of the search area, then return form RProcedure()
    if (current_tile->right_top_.x_ > right_top.x_) {
        return;
    }

    // R3: Use NeighborFinding() to locate all the tiles that touch the right side of the current tile
    // and also intersect the search area.
    auto neighbors = NeighborFinding(current_tile);

    for (auto& neighbor : neighbors) {
        if (neighbor->left_bottom_.y_ >= right_top.y_ || neighbor->right_top_.y_ <= left_bottom.y_ ||
        neighbor->left_bottom_.x_ >= right_top.x_) {
            continue;
        }

        // R4 For each of these neighbors, if the bottom left corner of the neighbor touches the current tile
        // then call RProcedure()
        if (neighbor->bl_ == current_tile) {
            RProcedure(neighbor, tile_list, left_bottom, right_top);
            continue;
        }

        // R5 If the bottom edge of the search area cuts both the current tile and the neighbor, then call
        // RProcedure()
        if (current_tile->left_bottom_.y_ <= left_bottom.y_ && neighbor->left_bottom_.y_ <= left_bottom.y_) {
            RProcedure(neighbor, tile_list, left_bottom, right_top);
        }
    }
}

inline std::vector<Layout::TilePtrType> Layout::getNeighbors(TilePtrType tile) {
    std::vector<TilePtrType> neighbors;
    auto right = NeighborFinding(tile);
    for (const auto& r : right) {
        neighbors.push_back(r);
    }

    auto left = NeighborFindingLeft(tile);
    for (const auto& l : left) {
        neighbors.push_back(l);
    }

    auto top = NeighborFindingTop(tile);
    for (const auto& t : top) {
        neighbors.push_back(t);
    }

    auto bottom = NeighborFindingBottom(tile);
    for (const auto& b : bottom) {
        neighbors.push_back(b);
    }
    return neighbors;
}

inline Layout::TilePtrType Layout::PointFinding(Point target) {
    auto current_tile = back_tile_;
    while (current_tile && !current_tile->isInTile(target)) {
        while (current_tile && target.y_ < current_tile->left_bottom_.y_) {
            current_tile = current_tile->lb_;
        }
        while (current_tile && target.y_ >= current_tile->right_top_.y_) {
            current_tile = current_tile->rt_;
        }

        while (current_tile && target.x_ < current_tile->left_bottom_.x_) {
            current_tile = current_tile->bl_;
        }
        while (current_tile && target.x_ >= current_tile->right_top_.x_) {
            current_tile = current_tile->tr_;
        }
    }

    if (current_tile) {
        back_tile_ = current_tile;
    }

    return current_tile;
}

inline bool Layout::AreaSearching(Point left_bottom, Point right_top) {
    Point start_point(left_bottom.x_, right_top.y_);
    auto current_tile = PointFinding(start_point);
    if (!current_tile) {
        current_tile = PointFinding(Point(left_bottom.x_, right_top.y_-1));
    }

    if (current_tile->left_bottom_.y_ == right_top.y_) {
        Point next_point(left_bottom.x_, current_tile->left_bottom_.y_ - 1);
        current_tile = PointFinding(next_point);
    }

    while (current_tile && current_tile->right_top_.y_ > left_bottom.y_) {
        if (current_tile->type_ == TileType::block || current_tile->right_top_.x_ < right_top.x_) {
            /// find blcok tile or find a space tile's right_top.x small than searching region right_top.x
            return true;
        }
        Point next_point(left_bottom.x_, current_tile->left_bottom_.y_ - 1);
        current_tile = PointFinding(next_point);
    }
    /// return false == Nothing is found
    return false;
}

inline void Layout::removeTile(TilePtrType tile) {
    // If u kill the back_tile_, the back_tile_ need to move to other tile.
    if (tile == back_tile_) {
        if (back_tile_->rt_) {
            back_tile_ = back_tile_->rt_;
        } else if (back_tile_->tr_) {
            back_tile_ = back_tile_->tr_;
        } else if (back_tile_->lb_) {
            back_tile_ = back_tile_->lb_;
        } else if (back_tile_->bl_) {
            back_tile_ = back_tile_->bl_;
        } else {
            std::cout << "warning: back_tile_ will be killed!!!" << std::endl;
        }
    }


    tile.reset();
    tile = nullptr;
    num_of_tiles_--;
}

// |------|------| first tr_, and then go down (lb_),
// |      |   B  | until B's right_top_.y_ < A->left_bottom_.y_
// |   A  |------|
// |      |  B1  |
// |------|------|
inline std::vector<Layout::TilePtrType> Layout::NeighborFinding(TilePtrType tile) {
    std::vector<TilePtrType> neighbors;
    TilePtrType current_tile = tile->tr_;
    while (current_tile && current_tile->right_top_.y_ > tile->left_bottom_.y_) {
        neighbors.push_back(current_tile);
        current_tile = current_tile->lb_;
    }
    return neighbors;
}

std::vector<Layout::TilePtrType> Layout::NeighborFindingLeft(TilePtrType tile) {
    std::vector<TilePtrType> left_neighbors;
    TilePtrType current_tile = tile->bl_;
    while (current_tile && current_tile->left_bottom_.y_ < tile->right_top_.y_) {
        left_neighbors.push_back(current_tile);
        current_tile = current_tile->rt_;
    }
    return left_neighbors;
}

std::vector<Layout::TilePtrType> Layout::NeighborFindingTop(TilePtrType tile) {
    std::vector<TilePtrType> top_neighbors;
    TilePtrType current_tile = tile->rt_;
    while (current_tile && current_tile->right_top_.x_ > tile->left_bottom_.x_) {
        top_neighbors.push_back(current_tile);
        current_tile = current_tile->bl_;
    }
    return top_neighbors;
}

std::vector<Layout::TilePtrType> Layout::NeighborFindingBottom(TilePtrType tile) {
    std::vector<TilePtrType> bottom_neighbors;
    TilePtrType current_tile = tile->lb_;
    while (current_tile && current_tile->left_bottom_.x_ < tile->right_top_.x_) {
        bottom_neighbors.push_back(current_tile);
        current_tile = current_tile->tr_;
    }
    return bottom_neighbors;
}

inline bool Layout::createTile(Point left_bottom, Point right_top, int num) {
    /// if find any tiles in researching region, return true
    if (AreaSearching(left_bottom, right_top)) {
        std::cout << "Waring: In the bonding box has a tile" << std::endl;
        return false;
    }
    std::shared_ptr<Tile> rt_of_left_tile = std::make_shared<Tile>();

    /// STEP1 split space tiles containing the Top and Bottom edges of the new tile
    Point top_point(left_bottom.x_, right_top.y_);
    auto top_space_tile = PointFinding(top_point);

    if (!top_space_tile) {
        top_space_tile = PointFinding(Point(left_bottom.x_, right_top.y_-1));
    }

    // new_top_space_tile is come from top_space_tile
    std::shared_ptr<Tile> new_top_space_tile = nullptr;
    if (right_top.y_ == top_space_tile->left_bottom_.y_) {
        top_point.y_--;
        top_space_tile = PointFinding(top_point);
    }
    if (isTopEdgeOnTile(top_space_tile, left_bottom, right_top)) {
        rt_of_left_tile = PointFinding(Point(left_bottom.x_-1, right_top.y_));
    } else {
        new_top_space_tile = splitTopSpaceTile(top_space_tile, left_bottom, right_top);
        rt_of_left_tile = new_top_space_tile;
    }

    auto bottom_space_tile = PointFinding(left_bottom);
    // new_bottom_space_tile is come from bottom_space_tile
    std::shared_ptr<Tile> new_bottom_space_tile = nullptr;
    if (!isBottomEdgeOnTile(bottom_space_tile, left_bottom, right_top)) {
        new_bottom_space_tile = splitBottomSpaceTile(bottom_space_tile, left_bottom, right_top);
    }

    /// STEP2 walk the left and right edges (traverse all center tiles), then split tiles
    auto out_of_center_space_tile = PointFinding(Point(left_bottom.x_, left_bottom.y_-1));
    auto center_space_tile = top_space_tile;
    auto new_block_tile = newTile(left_bottom, right_top, TileType::block);
    std::queue<std::shared_ptr<Tile>> center_space_tile_quene;
    // collect all center_space_tiles including top_space_tile and bottom_space_tile
    // out_of_center_space_tile means out of center_space_tiles
    while (center_space_tile != out_of_center_space_tile) {
        center_space_tile_quene.push(center_space_tile);
        top_point.y_ = center_space_tile->left_bottom_.y_ - 1;
        center_space_tile = PointFinding(top_point);
    }
    // traverse all center tiles, and split
    while (!center_space_tile_quene.empty()) {
        splitCenterSpaceTile(center_space_tile_quene.front(),
        new_block_tile, top_space_tile, bottom_space_tile, rt_of_left_tile);
        center_space_tile_quene.pop();
    }

    /// STEP3 merge space-like tiles vertically wherever possible
    mergeTiles(new_block_tile);


    /// Final: push_back to block_tile_list_
    new_block_tile->num_ = num;
    block_tile_list_.insert(new_block_tile);
    return true;
}

// ************************************** //
//           private functions            //
// ************************************** //

inline void Layout::mergeTiles(TilePtrType tile) {
    auto all_right_tiles = NeighborFinding(tile);
    auto all_left_tiles = NeighborFindingLeft(tile);
    if (!all_right_tiles.empty()) {
        mergeRightTiles(all_right_tiles);
    }
    if (!all_left_tiles.empty()) {
        mergeLeftTiles(all_left_tiles);
    }
}

inline Layout::TilePtrType Layout::newTile(Point left_bottom, Point right_top, TileType type) {
    auto nt = std::make_shared<Tile>(left_bottom, right_top, type);
    num_of_tiles_++;
    return nt;
}

inline Layout::TilePtrType Layout::splitTopSpaceTile(TilePtrType top_space_tile,
Point left_bottom, Point right_top) {
    auto new_tile = newTile(Point(top_space_tile->left_bottom_.x_, right_top.y_),
    top_space_tile->right_top_, TileType::space);
    new_tile->setPtr(top_space_tile->rt_, top_space_tile->tr_, top_space_tile->bl_, top_space_tile);
    while (new_tile->bl_ && new_tile->bl_->right_top_.y_ <= new_tile->left_bottom_.y_) {
        new_tile->bl_ = new_tile->bl_->rt_;
    }

    // Define new_tile->bl
    auto iter_tile = top_space_tile->bl_;
    while (iter_tile && iter_tile->tr_ == top_space_tile) {
        if (iter_tile->right_top_.y_ > new_tile->left_bottom_.y_) {
            iter_tile->tr_ = new_tile;
        }
        iter_tile = iter_tile->rt_;
    }

    // Define new_tile->tr
    iter_tile = top_space_tile->tr_;
    while (iter_tile && iter_tile->bl_ == top_space_tile) {
        if (iter_tile->left_bottom_.y_ >= new_tile->left_bottom_.y_) {
            iter_tile->bl_ = new_tile;
        }
        iter_tile = iter_tile->lb_;
    }

    // Use new_tile replace top_space_tile
    iter_tile = top_space_tile->rt_;
    while (iter_tile && iter_tile->lb_ == top_space_tile) {
        iter_tile->lb_ = new_tile;
        iter_tile = iter_tile->bl_;
    }

    // change top_space_tile's right_top_ point and rt_
    top_space_tile->rt_ = new_tile;
    top_space_tile->right_top_.y_ = right_top.y_;
    while (top_space_tile->tr_ &&
    top_space_tile->tr_->left_bottom_.y_ >= top_space_tile->right_top_.y_) {
        top_space_tile->tr_ = top_space_tile->tr_->lb_;
    }
    return new_tile;
}

inline Layout::TilePtrType Layout::splitBottomSpaceTile(TilePtrType bottom_space_tile,
Point left_bottom, Point right_top) {
    auto new_tile = newTile(bottom_space_tile->left_bottom_,
    Point(bottom_space_tile->right_top_.x_, left_bottom.y_), TileType::space);
    new_tile->setPtr(bottom_space_tile, bottom_space_tile->tr_,
    bottom_space_tile->bl_, bottom_space_tile->lb_);
    while (new_tile->tr_ && new_tile->tr_->left_bottom_.y_ >= new_tile->right_top_.y_) {
        new_tile->tr_ = new_tile->tr_->lb_;
    }

    // rt
    auto iter_tile = bottom_space_tile->lb_;
    while (iter_tile && iter_tile->rt_ == bottom_space_tile) {
        iter_tile->rt_ = new_tile;
        iter_tile = iter_tile->tr_;
    }

    // tr
    iter_tile = bottom_space_tile->bl_;
    while (iter_tile && iter_tile->tr_ == bottom_space_tile) {
        if (iter_tile->right_top_.y_ <= new_tile->right_top_.y_) {
            iter_tile->tr_ = new_tile;
        }
        iter_tile = iter_tile->rt_;
    }

    // bl
    iter_tile = bottom_space_tile->tr_;
    while (iter_tile && iter_tile->bl_ == bottom_space_tile) {
        if (iter_tile->left_bottom_.y_ < new_tile->right_top_.y_) {
            iter_tile->bl_ = new_tile;
        }
        iter_tile = iter_tile->lb_;
    }

    // fix bottom space
    bottom_space_tile->lb_ = new_tile;
    bottom_space_tile->left_bottom_.y_ = left_bottom.y_;
    while (bottom_space_tile->bl_ &&
    bottom_space_tile->bl_->right_top_.y_ <= bottom_space_tile->left_bottom_.y_) {
        bottom_space_tile->bl_ = bottom_space_tile->bl_->rt_;
    }
    return new_tile;
}

inline void Layout::mergeLeftTiles(std::vector<Layout::TilePtrType> all_left_tiles) {
    auto first_tile = *(all_left_tiles.begin());
    if (first_tile->lb_ &&
    first_tile->left_bottom_.x_ == first_tile->lb_->left_bottom_.x_ &&
    first_tile->right_top_.x_ == first_tile->lb_->right_top_.x_) {
        all_left_tiles.insert(all_left_tiles.begin(), first_tile->lb_);
    }

    auto last_tile = all_left_tiles[all_left_tiles.size()-1];
    if (last_tile->rt_ &&
    last_tile->left_bottom_.x_ == last_tile->rt_->left_bottom_.x_ &&
    last_tile->right_top_.x_ == last_tile->rt_->right_top_.x_) {
        all_left_tiles.push_back(last_tile->rt_);
    }

    TilePtrType iter_tile;
    for (SizeType i = 0; i < all_left_tiles.size()-1; ++i) {
        auto& current = all_left_tiles[i];
        auto& next = all_left_tiles[i+1];
        bool is_need_to_merge =
        current->left_bottom_.x_ == next->left_bottom_.x_ &&
        current->right_top_.x_ == next->right_top_.x_ &&
        current->type_ == TileType::space &&
        next->type_ == TileType::space;

        if (is_need_to_merge) {
            // |--------|
            // |  next  |
            // |--------|
            // |--------|
            // |  curr  |
            // |--------|
            // |----|---|
            // |    |   | iter_tile
            // |----|---|
            iter_tile = current->lb_;
            while (iter_tile && iter_tile->rt_ == current) {
                iter_tile->rt_ = next;
                iter_tile = iter_tile->tr_;
            }

            // |--------|
            // |  next  |
            // |--------|
            // |--------|-------|
            // |  curr  |       | iter_tile
            // |        |-------|
            // |        |       |
            // |        |       |
            // |--------|-------|
            iter_tile = current->tr_;
            while (iter_tile && iter_tile->bl_ == current) {
                iter_tile->bl_ = next;
                iter_tile = iter_tile->lb_;
            }

            //                    |--------|
            //                    |  next  |
            //                    |--------|
            //            |-------|--------|
            // iter_tile  |       |  curr  |
            //            |-------|        |
            //            |       |        |
            //            |       |        |
            //            |-------|--------|
            iter_tile = current->bl_;
            while (iter_tile && iter_tile->tr_ == current) {
                iter_tile->tr_ = next;
                iter_tile = iter_tile->rt_;
            }

            next->left_bottom_ = current->left_bottom_;
            next->lb_ = current->lb_;
            next->bl_ = current->bl_;
            removeTile(current);
        }
    }
}

inline void Layout::mergeRightTiles(std::vector<Layout::TilePtrType> all_right_tiles) {
    auto first_tile = *(all_right_tiles.begin());
    if (first_tile->rt_ &&
    first_tile->left_bottom_.x_ == first_tile->rt_->left_bottom_.x_ &&
    first_tile->right_top_.x_ == first_tile->rt_->right_top_.x_) {
        all_right_tiles.insert(all_right_tiles.begin(), first_tile->rt_);
    }

    auto last_tile = all_right_tiles.back();
    if (last_tile->lb_ &&
    last_tile->left_bottom_.x_ == last_tile->lb_->left_bottom_.x_ &&
    last_tile->right_top_.x_ == last_tile->lb_->right_top_.x_) {
        all_right_tiles.push_back(last_tile->lb_);
    }

    TilePtrType iter_tile;
    for (SizeType i = 0; i < all_right_tiles.size()-1; ++i) {
        auto& current = all_right_tiles[i];
        auto& next = all_right_tiles[i+1];
        bool is_need_to_merge =
        current->left_bottom_.x_ == next->left_bottom_.x_ &&
        current->right_top_.x_ == next->right_top_.x_ &&
        current->type_ == TileType::space &&
        next->type_ == TileType::space;

        if (is_need_to_merge) {
            // |----|---|
            // |    |   | iter_tile
            // |----|---|
            // |--------|
            // |  curr  |
            // |--------|
            // |--------|
            // |  next  |
            // |--------|
            iter_tile = current->rt_;
            while (iter_tile && iter_tile->lb_ == current) {
                iter_tile->lb_ = next;
                iter_tile = iter_tile->bl_;
            }

            // |--------|----|
            // |  curr  |    |
            // |        |----| iter_tile
            // |        |    |
            // |        |    |
            // |        |    |
            // |--------|----|
            // |--------|
            // |  next  |
            // |--------|
            iter_tile = current->tr_;
            while (iter_tile && iter_tile->bl_ == current) {
                iter_tile->bl_ = next;
                iter_tile = iter_tile->lb_;
            }

            //           |----|--------|
            //           |    |  curr  |
            // iter_tile |----|        |
            //           |    |        |
            //           |    |        |
            //           |    |        |
            //           |----|--------|
            //                |--------|
            //                |  next  |
            //                |--------|
            iter_tile = current->bl_;
            while (iter_tile && iter_tile->tr_ == current) {
                iter_tile->tr_ = next;
                iter_tile = iter_tile->rt_;
            }
            next->right_top_ = current->right_top_;
            next->rt_ = current->rt_;
            next->tr_ = current->tr_;
            removeTile(current);
        }
    }
}

inline void Layout::splitCenterSpaceTile(TilePtrType center_space_tile, TilePtrType new_block_tile,
TilePtrType top_space_tile, TilePtrType bottom_space_tile, TilePtrType& rt_of_left_tile) {
    bool no_left_space = center_space_tile->left_bottom_.x_ == new_block_tile->left_bottom_.x_;
    bool no_right_space = center_space_tile->right_top_.x_ == new_block_tile->right_top_.x_;
    std::shared_ptr<Tile> iter_tile;

    if (no_left_space && no_right_space) {
        // new_block_tile replace center_space_tile by left
        iter_tile = center_space_tile->bl_;
        while (iter_tile && iter_tile->tr_ == center_space_tile) {
            iter_tile->tr_ = new_block_tile;
            iter_tile = iter_tile->rt_;
        }

        // new_block_tile replace center_space_tile by right
        iter_tile = center_space_tile->tr_;
        while (iter_tile && iter_tile->bl_ == center_space_tile) {
            iter_tile->bl_ = new_block_tile;
            iter_tile = iter_tile->lb_;
        }

        // new_block_tile replace center_space_tile by top
        iter_tile = center_space_tile->rt_;
        while (iter_tile && iter_tile->lb_ == center_space_tile) {
            iter_tile->lb_ = new_block_tile;
            iter_tile = iter_tile->bl_;
        }

        // new_block_tile replace center_space_tile by bottom
        iter_tile = center_space_tile->lb_;
        while (iter_tile && iter_tile->rt_ == center_space_tile) {
            if (center_space_tile == bottom_space_tile)
                iter_tile->rt_ = new_block_tile;
            else
                iter_tile->rt_ = nullptr;
            iter_tile = iter_tile->tr_;
        }

        // Define new_block_tile pointers
        if (center_space_tile == top_space_tile) {
            new_block_tile->tr_ = center_space_tile->tr_;
            new_block_tile->rt_ = center_space_tile->rt_;
        }

        // Define new_block_tile pointers
        if (center_space_tile == bottom_space_tile) {
            new_block_tile->bl_ = center_space_tile->bl_;
            new_block_tile->lb_ = center_space_tile->lb_;
        }

        rt_of_left_tile = center_space_tile->bl_;
        removeTile(center_space_tile);

    } else if (no_right_space) {
        // center_space_tile   new_block_tile
        // |-------------|-------|----|
        // |             |       |    |
        // |-------------|       |    | iter_tile
        // |             |       |    |
        // |-------------|-------|----|
        iter_tile = center_space_tile->tr_;
        while (iter_tile && iter_tile->bl_ == center_space_tile) {
            iter_tile->bl_ = new_block_tile;
            iter_tile = iter_tile->lb_;
        }

        iter_tile = center_space_tile->lb_;
        while (iter_tile && iter_tile->rt_ == center_space_tile) {
            if (iter_tile->right_top_.x_ > new_block_tile->left_bottom_.x_ &&
            iter_tile->right_top_.x_ <= new_block_tile->right_top_.x_) {
                // need consider which is bottom_block_tile
                if (center_space_tile == bottom_space_tile) {
                    iter_tile->rt_ = new_block_tile;
                } else {
                    iter_tile->rt_ = nullptr;
                }
            }
            iter_tile = iter_tile->tr_;
        }

        iter_tile = center_space_tile->rt_;
        while (iter_tile && iter_tile->lb_ == center_space_tile) {
            // i think it only work on top_block_tile
            if (iter_tile->left_bottom_.x_ >= new_block_tile->left_bottom_.x_) {
                iter_tile->lb_ = new_block_tile;
            }
            iter_tile = iter_tile->bl_;
        }

        // Define new_block_tile pointers
        if (center_space_tile == top_space_tile) {
            new_block_tile->tr_ = center_space_tile->tr_;
            new_block_tile->rt_ = center_space_tile->rt_;
        }

        // Define new_block_tile pointers
        if (center_space_tile == bottom_space_tile) {
            new_block_tile->bl_ = center_space_tile;
            // hard to define new_block_tile->lb_, need to draw a figure
            iter_tile = center_space_tile->lb_;
            while (iter_tile && iter_tile->left_bottom_.x_ <= new_block_tile->left_bottom_.x_) {
                if (iter_tile->right_top_.x_ > new_block_tile->left_bottom_.x_) {
                    new_block_tile->lb_ = iter_tile;
                }
                iter_tile = iter_tile->tr_;
            }
        }

        center_space_tile->right_top_.x_ = new_block_tile->left_bottom_.x_;
        center_space_tile->tr_ = new_block_tile;
        center_space_tile->rt_ = rt_of_left_tile;
        rt_of_left_tile = center_space_tile;

    } else if (no_left_space) {
        //            new_block_tile
        //  iter_tile |------|
        //      |-----|      |***********|
        //      |     |      |***********| center_space_tile
        //      |     |      |***********|
        //      |-----|------|
        iter_tile = center_space_tile->bl_;
        while (iter_tile && iter_tile->tr_ == center_space_tile) {
            iter_tile->tr_ = new_block_tile;
            iter_tile = iter_tile->rt_;
        }

        iter_tile = center_space_tile->rt_;
        while (iter_tile && iter_tile->lb_ == center_space_tile) {
            if (iter_tile->left_bottom_.x_ < new_block_tile->right_top_.x_) {
                iter_tile->lb_ = new_block_tile;
            }
            iter_tile = iter_tile->bl_;
        }

        iter_tile = center_space_tile->lb_;
        while (iter_tile && iter_tile->rt_ == center_space_tile) {
            if (iter_tile->right_top_.x_ <= new_block_tile->right_top_.x_) {
                if (center_space_tile == bottom_space_tile) {
                    iter_tile->rt_ = new_block_tile;
                } else {
                    iter_tile == nullptr;
                }
            } else if ( iter_tile->right_top_.x_ > new_block_tile->right_top_.x_ &&
                iter_tile->right_top_.x_ <= center_space_tile->right_top_.x_) {
                iter_tile->rt_ = center_space_tile;
            }
            iter_tile = iter_tile->tr_;
        }

        // Define new_block_tile pointers
        if (center_space_tile == top_space_tile) {
            new_block_tile->tr_ = center_space_tile;
            iter_tile = center_space_tile->rt_;
            while (iter_tile && iter_tile->right_top_.x_ >= new_block_tile->right_top_.x_) {
                if (iter_tile->left_bottom_.x_ < new_block_tile->right_top_.x_) {
                    new_block_tile->rt_ = iter_tile;
                }
                iter_tile = iter_tile->bl_;
            }
        }

        // Define new_block_tile pointers
        if (center_space_tile == bottom_space_tile) {
            new_block_tile->bl_ = center_space_tile->bl_;
            new_block_tile->lb_ = center_space_tile->lb_;
        }
        rt_of_left_tile = center_space_tile->bl_;
        center_space_tile->left_bottom_.x_ = new_block_tile->right_top_.x_;
        center_space_tile->bl_ = new_block_tile;
        while (center_space_tile->lb_ &&
        center_space_tile->lb_->right_top_.x_ <= center_space_tile->left_bottom_.x_) {
            center_space_tile->lb_ = center_space_tile->lb_->tr_;
        }

    } else {
        auto new_right_space_tile = newTile(
        Point(new_block_tile->right_top_.x_, center_space_tile->left_bottom_.y_),
        center_space_tile->right_top_, TileType::space);

        new_right_space_tile->setPtr(
        center_space_tile->rt_,
        center_space_tile->tr_,
        new_block_tile,
        center_space_tile->lb_);

        while (new_right_space_tile->lb_ &&
        new_right_space_tile->lb_->right_top_.x_ <= new_right_space_tile->left_bottom_.x_) {
            new_right_space_tile->lb_ = new_right_space_tile->lb_->tr_;
        }

        // insert new_right_space_tile
        // between center_space_tile and center_space_tile->tr_
        iter_tile = center_space_tile->tr_;
        while (iter_tile && iter_tile->bl_ == center_space_tile) {
            iter_tile->bl_ = new_right_space_tile;
            iter_tile = iter_tile->lb_;
        }

        // insert new_right_space_tile
        // and define center_space_tile->lb_'s rt_
        iter_tile = center_space_tile->lb_;
        while (iter_tile && iter_tile->rt_ == center_space_tile) {
            if (iter_tile->right_top_.x_ > new_block_tile->right_top_.x_) {
                iter_tile->rt_ = new_right_space_tile;
            } else if (iter_tile->right_top_.x_ > new_block_tile->left_bottom_.x_) {
                if (center_space_tile == bottom_space_tile) {
                    iter_tile->rt_ = new_block_tile;
                } else {
                    iter_tile->rt_ = nullptr;
                }
            }
            iter_tile = iter_tile->tr_;
        }

        // insert new_right_space_tile
        // and define center_space_tile->rt_'s lb_
        iter_tile = center_space_tile->rt_;
        while (iter_tile && iter_tile->lb_ == center_space_tile) {
            if (iter_tile->left_bottom_.x_ >= new_block_tile->right_top_.x_) {
                iter_tile->lb_ = new_right_space_tile;
            } else if (iter_tile->left_bottom_.x_ >= new_block_tile->left_bottom_.x_) {
                if (center_space_tile == top_space_tile) {
                    iter_tile->lb_ = new_block_tile;
                } else {
                    iter_tile->lb_ = nullptr;
                }
            }
            iter_tile = iter_tile->bl_;
        }

        // Define new_block_tile's neighbor (tr_, rt_)
        if (center_space_tile == top_space_tile) {
            new_block_tile->tr_ = new_right_space_tile;
            iter_tile = new_right_space_tile->rt_;
            while (iter_tile &&
            iter_tile->right_top_.x_ >= new_block_tile->right_top_.x_) {
                if (iter_tile->left_bottom_.x_ < new_block_tile->right_top_.x_) {
                    new_block_tile->rt_ = iter_tile;
                }
                iter_tile = iter_tile->bl_;
            }
        }

        // Define new_block_tile's neighbor (bl_, lb_)
        if (center_space_tile == bottom_space_tile) {
            new_block_tile->bl_ = center_space_tile;
            iter_tile = center_space_tile->lb_;
            while (iter_tile &&
            iter_tile->left_bottom_.x_ <= new_block_tile->left_bottom_.x_) {
                if (iter_tile->right_top_.x_ > new_block_tile->left_bottom_.x_) {
                    new_block_tile->lb_ = iter_tile;
                }
                iter_tile = iter_tile->tr_;
            }
        }

        center_space_tile->right_top_.x_ = new_block_tile->left_bottom_.x_;
        center_space_tile->tr_ = new_block_tile;
        center_space_tile->rt_ = rt_of_left_tile;
        rt_of_left_tile = center_space_tile;
    }
}

}  // namespace lcs
#endif  // SRC_LCS_LAYOUT_HPP_
