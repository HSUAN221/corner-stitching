// Copyright (c) 2022 LucaWei. All rights reserved.
#include <lcs/lcs.hpp>
#include <lcs/functional.hpp>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        throw std::invalid_argument("plz set the input file name and output file name!!!");
    }
    const std::string input_file_name = argv[1];
    const std::string output_file_name = argv[2];

    lcs::Timer timer;
    std::ofstream ans_file;
    ans_file.open(output_file_name);
    timer.start();
    auto layout = lcs::functional::writeAnswer(ans_file, input_file_name);
    timer.stop();
    ans_file.close();
    std::cout << "Total Elapsed Time= " << timer.elapsedTime() << std::endl;

    /// draw block layout
    // std::ofstream for_fig_file;
    // for_fig_file.open("docs/debug.txt");
    // lcs::functional::writeFigureOutput(for_fig_file, layout);
    // for_fig_file.close();

    /// test case
    // std::string no_path = output_file_name;
    // no_path = no_path.substr(no_path.rfind("/")+1);  // only for Linux
    // bool isSame = lcs::functional::compareFiles("data/ans_by_ta/"+no_path, output_file_name);
    // if (isSame) {
    //     std::cout << "my " + no_path + " is same as TA's " + no_path << std::endl;
    // } else {
    //     std::cout << "my " + no_path + " isn't same as TA's " + no_path << std::endl;
    // }
    return 0;
}
