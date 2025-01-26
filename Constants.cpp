#include "Constants.hpp"

const std::unordered_map<std::string, int> NOTE_VAL_DICT = {
    {"Ab", 8}, {"A", 9},  {"A#", 10},
    {"Bb", 10},{"B", 11}, {"Cb", 11},
    {"C", 0},  {"C#", 1}, {"Db", 1},
    {"D", 2},  {"D#", 3}, {"Eb", 3},
    {"E", 4},  {"F", 5},  {"F#", 6},
    {"Gb", 6}, {"G", 7},  {"G#", 8}
};

const std::map<int, std::vector<std::string>> VAL_NOTE_DICT = {
    {0,  {"C"}},
    {1,  {"Db", "C#"}},
    {2,  {"D"}},
    {3,  {"Eb", "D#"}},
    {4,  {"E"}},
    {5,  {"F"}},
    {6,  {"F#", "Gb"}},
    {7,  {"G"}},
    {8,  {"Ab", "G#"}},
    {9,  {"A"}},
    {10, {"Bb", "A#"}},
    {11, {"B", "Cb"}}
};

const std::unordered_map<int, std::string> SHARPED_SCALE = {
    {0, "C"},  {1, "C#"}, {2, "D"},  {3, "D#"},
    {4, "E"},  {5, "F"},  {6, "F#"}, {7, "G"},
    {8, "G#"}, {9, "A"},  {10, "A#"},{11, "B"}
};

const std::unordered_map<int, std::string> FLATTED_SCALE = {
    {0, "C"},  {1, "Db"}, {2, "D"},  {3, "Eb"},
    {4, "E"},  {5, "F"},  {6, "Gb"}, {7, "G"},
    {8, "Ab"}, {9, "A"},  {10, "Bb"},{11, "B"}
};

const std::unordered_map<std::string, std::unordered_map<int, std::string>> SCALE_VAL_DICT = {
    {"Ab", FLATTED_SCALE}, {"A", SHARPED_SCALE},  {"A#", SHARPED_SCALE},
    {"Bb", FLATTED_SCALE}, {"B", SHARPED_SCALE},  {"Cb", FLATTED_SCALE},
    {"C", FLATTED_SCALE},  {"C#", SHARPED_SCALE}, {"Db", FLATTED_SCALE},
    {"D", SHARPED_SCALE},  {"D#", SHARPED_SCALE}, {"Eb", FLATTED_SCALE},
    {"E", SHARPED_SCALE},  {"F", FLATTED_SCALE},  {"F#", SHARPED_SCALE},
    {"Gb", FLATTED_SCALE}, {"G", SHARPED_SCALE},  {"G#", SHARPED_SCALE}
};

const std::unordered_map<std::string, std::vector<int>> RELATIVE_KEY_DICT = {
    {"maj", {0, 2, 4, 5, 7, 9, 11, 12}}, // Ionian
    {"Dor", {0, 2, 3, 5, 7, 9, 10, 12}},
    {"Phr", {0, 1, 3, 5, 7, 8, 10, 12}},
    {"Lyd", {0, 2, 4, 6, 7, 9, 11, 12}},
    {"Mix", {0, 2, 4, 5, 7, 9, 10, 12}},
    {"min", {0, 2, 3, 5, 7, 8, 10, 12}}, // Aeolian
    {"Loc", {0, 1, 3, 5, 6, 8, 10, 12}}
};

const std::vector<std::pair<std::string, std::vector<int>>> DEFAULT_QUALITIES = {
    // 2-note
    {"5", {0, 7}},
    {"no5", {0, 4}},
    {"omit5", {0, 4}},
    {"m(no5)", {0, 3}},
    {"m(omit5)", {0, 3}},
    // 3-note
    {"", {0, 4, 7}},
    {"maj", {0, 4, 7}},
    {"m", {0, 3, 7}},
    {"min", {0, 3, 7}},
    {"-", {0, 3, 7}},
    {"dim", {0, 3, 6}},
    {"(b5)", {0, 4, 6}},
    {"aug", {0, 4, 8}},
    {"sus2", {0, 2, 7}},
    {"sus4", {0, 5, 7}},
    {"sus", {0, 5, 7}},
    // 4-note
    {"6", {0, 4, 7, 9}},
    {"6b5", {0, 4, 6, 9}},
    {"6-5", {0, 4, 6, 9}},
    {"7", {0, 4, 7, 10}},
    {"7-5", {0, 4, 6, 10}},
    {"7b5", {0, 4, 6, 10}},
    {"7+5", {0, 4, 8, 10}},
    {"7#5", {0, 4, 8, 10}},
    {"7sus4", {0, 5, 7, 10}},
    {"m6", {0, 3, 7, 9}},
    {"m7", {0, 3, 7, 10}},
    {"m7-5", {0, 3, 6, 10}},
    {"m7b5", {0, 3, 6, 10}},
    {"m7+5", {0, 3, 8, 10}},
    {"m7#5", {0, 3, 8, 10}},
    {"dim6", {0, 3, 6, 8}},
    {"dim7", {0, 3, 6, 9}},
    {"M7", {0, 4, 7, 11}},
    {"maj7", {0, 4, 7, 11}},
    {"maj7+5", {0, 4, 8, 11}},
    {"M7+5", {0, 4, 8, 11}},
    {"mmaj7", {0, 3, 7, 11}},
    {"mM7", {0, 3, 7, 11}},
    {"add4", {0, 4, 5, 7}},
    {"majadd4", {0, 4, 5, 7}},
    {"Madd4", {0, 4, 5, 7}},
    {"madd4", {0, 3, 5, 7}},
    {"add9", {0, 4, 7, 14}},
    {"majadd9", {0, 4, 7, 14}},
    {"Madd9", {0, 4, 7, 14}},
    {"madd9", {0, 3, 7, 14}},
    {"sus4add9", {0, 5, 7, 14}},
    {"sus4add2", {0, 2, 5, 7}},
    {"2", {0, 4, 7, 14}},
    {"add11", {0, 4, 7, 17}},
    {"4", {0, 4, 7, 17}},
    // 5-note
    {"m69", {0, 3, 7, 9, 14}},
    {"69", {0, 4, 7, 9, 14}},
    {"9", {0, 4, 7, 10, 14}},
    {"m9", {0, 3, 7, 10, 14}},
    {"M9", {0, 4, 7, 11, 14}},
    {"maj9", {0, 4, 7, 11, 14}},
    {"9sus4", {0, 5, 7, 10, 14}},
    {"7-9", {0, 4, 7, 10, 13}},
    {"7b9", {0, 4, 7, 10, 13}},
    {"7(b9)", {0, 4, 7, 10, 13}},
    {"7+9", {0, 4, 7, 10, 15}},
    {"7#9", {0, 4, 7, 10, 15}},
    {"9-5", {0, 4, 6, 10, 14}},
    {"9b5", {0, 4, 6, 10, 14}},
    {"9+5", {0, 4, 8, 10, 14}},
    {"9#5", {0, 4, 8, 10, 14}},
    {"7#9b5", {0, 4, 6, 10, 15}},
    {"7#9#5", {0, 4, 8, 10, 15}},
    {"m7b9b5", {0, 3, 6, 10, 13}},
    {"7b9b5", {0, 4, 6, 10, 13}},
    {"7b9#5", {0, 4, 8, 10, 13}},
    {"11", {0, 7, 10, 14, 17}},
    {"7+11", {0, 4, 7, 10, 18}},
    {"7#11", {0, 4, 7, 10, 18}},
    {"maj7+11", {0, 4, 7, 11, 18}},
    {"M7+11", {0, 4, 7, 11, 18}},
    {"maj7#11", {0, 4, 7, 11, 18}},
    {"M7#11", {0, 4, 7, 11, 18}},
    {"7b9#9", {0, 4, 7, 10, 13, 15}},
    {"7b9#11", {0, 4, 7, 10, 13, 18}},
    {"7#9#11", {0, 4, 7, 10, 15, 18}},
    {"7-13", {0, 4, 7, 10, 20}},
    {"7b13", {0, 4, 7, 10, 20}},
    {"m7add11", {0, 3, 7, 10, 17}},
    {"maj7add11", {0, 4, 7, 11, 17}},
    {"M7add11", {0, 4, 7, 11, 17}},
    {"mmaj7add11", {0, 3, 7, 11, 17}},
    {"mM7add11", {0, 3, 7, 11, 17}},
    // 6-note
    {"7b9b13", {0, 4, 7, 10, 13, 17, 20}},
    {"9+11", {0, 4, 7, 10, 14, 18}},
    {"9#11", {0, 4, 7, 10, 14, 18}},
    {"m11", {0, 3, 7, 10, 14, 17}},
    {"13", {0, 4, 7, 10, 14, 21}},
    {"13-9", {0, 4, 7, 10, 13, 21}},
    {"13b9", {0, 4, 7, 10, 13, 21}},
    {"13+9", {0, 4, 7, 10, 15, 21}},
    {"13#9", {0, 4, 7, 10, 15, 21}},
    {"13+11", {0, 4, 7, 10, 18, 21}},
    {"13#11", {0, 4, 7, 10, 18, 21}},
    {"maj13", {0, 4, 7, 11, 14, 21}},
    {"M13", {0, 4, 7, 11, 14, 21}},
    {"maj7add13", {0, 4, 7, 9, 11, 14}},
    {"M7add13", {0, 4, 7, 9, 11, 14}},
};
