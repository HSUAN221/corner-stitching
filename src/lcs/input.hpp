// Copyright (c) 2022 LucaWei. All rights reserved.
#ifndef SRC_LCS_INPUT_HPP_
#define SRC_LCS_INPUT_HPP_

namespace lcs {

struct InputData {
    int width_, height_;
    std::set<Rectangle, ComRectNum> rect_list_;
    std::vector<Point> point_list_;

    friend std::ostream& operator<<(std::ostream& os, const InputData& data) {
        os << data.width_ << " " << data.height_ << std::endl;
        os << "num. of point= " << data.point_list_.size() << std::endl;
        for (const auto& pt : data.point_list_) {
            os << pt.x_ << " " << pt.y_ << std::endl;
        }
        os << "num. of rect= " << data.rect_list_.size() << std::endl;
        for (auto& rect : data.rect_list_) {
            os << rect.num_ << " : " << rect.left_bottom_ << " " << rect.right_top_ << std::endl;
        }
        return os;
    }
};

class ReadFileInterface {
 public:
    ReadFileInterface() = default;
    virtual ~ReadFileInterface() = default;
    virtual std::shared_ptr<InputData> readFile(const std::string& filename) = 0;
};

class ReadTextfile : public ReadFileInterface {
 public:
    ReadTextfile() = default;
    ~ReadTextfile() = default;

    std::shared_ptr<InputData> readFile(const std::string& filename) override {
        std::fstream file;
        std::shared_ptr<InputData> d = std::make_shared<InputData>();
        file.open(filename);
        if (file.fail()) {
            throw std::invalid_argument(filename + "can't find!!!");
        }
        std::string line;
        std::getline(file, line);
        std::istringstream ss(line);
        ss >> d->width_ >> d->height_;
        while (std::getline(file, line)) {
            std::istringstream  ss(line);
            if (line[0] == 'P') {
                Point temp_pt;
                std::string temp;
                ss >> temp >> temp_pt.x_ >> temp_pt.y_;
                d->point_list_.push_back(temp_pt);
            } else {
                int left_bottom_x, left_bottom_y, width, height, num;
                ss >> num >> left_bottom_x >> left_bottom_y >> width >> height;
                Rectangle temp_rect(Point(left_bottom_x, left_bottom_y),
                Point(left_bottom_x+width, left_bottom_y+height));
                temp_rect.num_ = num;
                d->rect_list_.insert(temp_rect);
            }
        }
        file.close();

        return d;
    }
};

}  // namespace lcs
#endif  // SRC_LCS_INPUT_HPP_
