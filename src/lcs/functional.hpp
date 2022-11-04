// Copyright (c) 2022 LucaWei. All rights reserved.
#ifndef SRC_LCS_FUNCTIONAL_HPP_
#define SRC_LCS_FUNCTIONAL_HPP_
#include <lcs/lcs.hpp>
namespace lcs {
namespace functional {

std::shared_ptr<Layout> writeAnswer(std::ostream& output, const std::string& input_filename) {
    auto layout = std::make_unique<Layout>();
    layout->readFile(ReadTextfile(), input_filename);
    layout->initialize();
    std::fstream file;
    file.open(input_filename);
    if (file.fail()) {
        throw std::invalid_argument(input_filename + "can't find!!!");
    }
    int width, height;
    std::string line;
    std::getline(file, line);
    std::istringstream ss(line);
    ss >> width >> height;
    std::vector<Point> left_right_finding_tile_list;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        if (line[0] == 'P') {
            Point temp_pt;
            std::string temp;
            ss >> temp >> temp_pt.x_ >> temp_pt.y_;
            const auto& tile = layout->PointFinding(temp_pt);
            left_right_finding_tile_list.push_back(tile->left_bottom_);
        } else {
            int left_bottom_x, left_bottom_y, width, height, num;
            ss >> num >> left_bottom_x >> left_bottom_y >> width >> height;
            layout->createTile(Point(left_bottom_x, left_bottom_y),
            Point(left_bottom_x+width, left_bottom_y+height), num);
        }
    }

    /// Write Answer
    const auto& result = layout->block_tile_list_;
    output << layout->enumerate(Point(0, 0), Point(width, height)).size() << std::endl;

    for (const auto& tile : result) {
        std::map<TileType, int> cnt;
        const auto& neighbors = layout->getNeighbors(tile);
        for (const auto& nebor_tile : neighbors) {
            if (nebor_tile->type_ == TileType::block) {
                cnt[TileType::block]++;
            } else if (nebor_tile->type_ == TileType::space) {
                cnt[TileType::space]++;
            }
        }
        output << tile->num_
        << " " << cnt[TileType::block]
        << " " << cnt[TileType::space]
        << std::endl;
    }

    for (const auto& pt : left_right_finding_tile_list) {
        output << pt.x_ << " " << pt.y_ << std::endl;
    }
    return std::move(layout);
}

void writeFigureOutput(std::ostream& os, const std::shared_ptr<Layout>& layout) {
    const auto& input_data = layout->input_data_;
    os << layout->num_of_tiles_ << std::endl;
    os << input_data->width_ << " " << input_data->height_ << std::endl;

    std::set<std::shared_ptr<Tile>> all_tiles;
    const auto& result = layout->block_tile_list_;
    for (const auto& tile : result) {
        const auto& neighbors = layout->getNeighbors(tile);
        for (const auto& n : neighbors) {
            all_tiles.insert(n);
        }
    }
    for (auto& tile : all_tiles) {
        if (tile->type_ == TileType::space) {
            os << "-1 " << tile->left_bottom_.x_ << " " << tile->left_bottom_.y_
            << " " << tile->right_top_.x_ - tile->left_bottom_.x_
            << " " << tile->right_top_.y_ - tile->left_bottom_.y_
            << std::endl;
        }
    }
    for (const auto& tile : result) {
        if (tile->type_ == TileType::block) {
            os << "1 " << tile->left_bottom_.x_ << " " << tile->left_bottom_.y_
            << " " << tile->right_top_.x_ - tile->left_bottom_.x_
            << " " << tile->right_top_.y_ - tile->left_bottom_.y_
            << std::endl;
        }
    }
}

bool compareFiles(const std::string& p1, const std::string& p2) {
    std::ifstream f1(p1, std::ifstream::binary|std::ifstream::ate);
    std::ifstream f2(p2, std::ifstream::binary|std::ifstream::ate);

    if (f1.fail() || f2.fail()) {
        return false;
    }

    if (f1.tellg() != f2.tellg()) {
        return false;
    }

    f1.seekg(0, std::ifstream::beg);
    f2.seekg(0, std::ifstream::beg);
    return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                    std::istreambuf_iterator<char>(),
                    std::istreambuf_iterator<char>(f2.rdbuf()));
}

}  // namespace functional
}  // namespace lcs
#endif  // SRC_LCS_FUNCTIONAL_HPP_
