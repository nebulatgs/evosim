#include <unordered_map>
#include <string>
#include <stack>
#include <numeric>
struct Gene;
enum Protein {vision, color, size, speed, a_res, SetA_Res, FindFood, MoveFood, EatFood, Reproduce, instr_sep = 4};
std::unordered_map<std::string, int> proteinLUT = {
    {"AAA", Protein::FindFood},
    {"AAT", Protein::FindFood},
    {"AAG", Protein::FindFood},
    {"AAC", Protein::FindFood},
    {"ATA", Protein::MoveFood},
    {"ATT", Protein::MoveFood},
    {"ATG", Protein::MoveFood},
    {"ATC", Protein::MoveFood},
    {"AGA", Protein::EatFood},
    {"AGT", Protein::EatFood},
    {"AGC", Protein::EatFood},
    {"AGG", Protein::EatFood},
    {"ACA", Protein::Reproduce},
    {"ACT", Protein::Reproduce},
    {"ACG", Protein::Reproduce},
    {"ACC", Protein::Reproduce},
    {"TAA", Protein::vision},
    {"TAT", Protein::a_res},
    {"TAG", Protein::SetA_Res}
};