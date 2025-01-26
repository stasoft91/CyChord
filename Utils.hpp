#pragma once

#include <string>
#include <vector>

/**
 * Utilities for note conversion, transposition, and display.
 */

int noteToVal(const std::string& note);
std::string valToNote(int val, const std::string& scaleRoot = "C");
std::string transposeNote(const std::string& note, int semitones, const std::string& scale = "C");

// For chord name rendering
std::string displayAppended(const std::vector<std::string>& appended);
std::string displayOn(const std::string& onNote);
