#include "Parser.hpp"
#include "Constants.hpp"
#include <stdexcept>
#include <regex>

static void checkNote(const std::string& note) {
    if (NOTE_VAL_DICT.find(note) == NOTE_VAL_DICT.end()) {
        throw std::runtime_error("Invalid note: " + note);
    }
}

/**
 * Extract a chord’s tokens. We look for root note (possibly 2 chars if it has b/#),
 * then read the remainder as the chord quality name. Then see if there's a slash
 * chord portion. Also check if there's an inversion number like "/9".
 */
ChordTokens parseChord(const std::string& chordExpression) {
    ChordTokens tokens;
    tokens.inversion = 0;

    if (chordExpression.empty()) {
        throw std::runtime_error("Chord expression is empty.");
    }

    // 1) Get root note. If second character is '#' or 'b', root is 2 chars
    size_t idx = 1;
    if (chordExpression.size() > 1 &&
        (chordExpression[1] == 'b' || chordExpression[1] == '#')) {
        idx = 2;
    }
    tokens.root = chordExpression.substr(0, idx);
    checkNote(tokens.root);

    // The rest
    std::string rest = chordExpression.substr(idx);

    // 2) Check for an inversion number with a regex "/(\\d+)"
    // For simplicity, we'll do something like:
    static const std::regex inversionRegex("/(\\d+)");
    std::smatch match;
    if (std::regex_search(rest, match, inversionRegex)) {
        if (match.size() > 1) {
            tokens.inversion = std::stoi(match[1].str());
        }
        // remove that part from rest
        rest = std::regex_replace(rest, inversionRegex, "");
    }

    // 3) Check for slash chord that is not numeric. E.g. "C/G"
    // If slash is found, parse that as slashNote
    auto slashPos = rest.find('/');
    if (slashPos != std::string::npos) {
        tokens.slashNote = rest.substr(slashPos + 1);
        rest = rest.substr(0, slashPos);
        checkNote(tokens.slashNote);
    }

    // 4) The remainder is presumably the chord quality name (unless we want to parse appended)
    tokens.qualityName = rest;

    // We do not parse appended notes deeply yet (the Python code left it as TODO).
    // So we store empty appended for now:
    tokens.appended = {};

    return tokens;
}
