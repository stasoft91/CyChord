#include "Chord.hpp"
#include "Parser.hpp"
#include "Utils.hpp"
#include "QualityManager.hpp"
#include "Constants.hpp"
#include <stdexcept>
#include <sstream>
#include <algorithm>

Chord::Chord(const std::string& chordName)
    : m_chordName(chordName)
{
    // parse
    ChordTokens tokens = parseChord(chordName);
    m_root    = tokens.root;
    // get a new Quality from manager
    m_quality = QualityManager::Instance().getQuality(tokens.qualityName, tokens.inversion);
    m_appended = tokens.appended;
    m_on      = tokens.slashNote;

    // Possibly adjust slash chord intervals
    applyOnChord();

    // Reconfigure the textual chord name
    reconfigureChord();
}

/**
 * Similar to the Python code: from_note_index(note, quality, scale, diatonic, chromatic).
 * E.g. if you want the I chord of "Cmaj", note=1 => "C" => "C{quality}".
 * If diatonic, we deduce chord quality from the scale's scale degrees.
 */
Chord Chord::fromNoteIndex(int note,
                           const std::string& quality,
                           const std::string& scale,
                           bool diatonic,
                           int chromatic)
{
    if (note < 1 || note > 8) {
        throw std::runtime_error("Invalid scale degree (must be 1..7 or 8).");
    }

    // We'll parse the scale: last 3 chars is the mode, e.g. "maj" or "min"
    if (scale.size() < 4) {
        throw std::runtime_error("Scale must be something like Cmaj or Amin, e.g. length>=4");
    }
    std::string scaleMode = scale.substr(scale.size() - 3);
    std::string scaleRoot = scale.substr(0, scale.size() - 3);

    // check if scaleMode in RELATIVE_KEY_DICT
    auto it = RELATIVE_KEY_DICT.find(scaleMode);
    if (it == RELATIVE_KEY_DICT.end()) {
        throw std::runtime_error("Unknown scale mode: " + scaleMode);
    }
    const auto& pattern = it->second; // scale degrees

    // scale degree index = note-1 => pattern[note-1]
    int semitoneOffset = pattern[note - 1];

    // Now figure out the "root" semitone for the scale root
    int scaleRootVal = noteToVal(scaleRoot);
    // add chromatic shift
    scaleRootVal += chromatic;
    // overall absolute semitone
    int chordRootVal = scaleRootVal + semitoneOffset;

    // convert chordRootVal back to a note name
    std::string chordRoot = valToNote(chordRootVal, scaleRoot);

    // If diatonic is true, we auto-generate the chord quality from triads or sevenths
    // as the python code does. We'll do a minimal approach:
    std::string finalQuality = quality; // default

    if (diatonic) {
        // we figure out a triad or 7th from the scale pattern. This is simplified logic:
        // For example, we can gather intervals: (root, third, fifth, maybe seventh)
        // then see if there's a matching chord in the manager.

        // Let's build a mini chord from the scale steps:
        // indices in pattern => (note-1), (note+1), (note+3), ...
        // note that we have to wrap around the pattern if needed
        // We do a naive approach:
        // For triad: pattern[(note-1)], pattern[(note+1) % 7], pattern[(note+3) % 7]
        // For seventh: pattern[(note+5) % 7], etc.
        // Then we unify them to 0-based intervals. Then we see if we can find a matching quality.

        // We'll handle only triad or seventh as the Python code states.

        bool isSeventh = false;
        if (quality.find("7") != std::string::npos || quality == "M7" || quality == "maj7" || quality == "m7") {
            isSeventh = true;
        }
        // Gather
        std::vector<int> diatonicIntervals;
        diatonicIntervals.reserve(isSeventh ? 4 : 3);

        auto wrap = [&](int x) { return pattern[x % 7] + 12 * (x / 7); };

        diatonicIntervals.push_back(wrap(note - 1));
        diatonicIntervals.push_back(wrap((note - 1) + 2));
        diatonicIntervals.push_back(wrap((note - 1) + 4));
        if (isSeventh) {
            diatonicIntervals.push_back(wrap((note - 1) + 6));
        }

        // Now reorder them so the first is 0
        // subtract diatonicIntervals[0]
        int base = diatonicIntervals[0];
        for (auto &v : diatonicIntervals) {
            v -= base;
        }
        // Look up in manager
        auto found = QualityManager::Instance().findQualityFromComponents(diatonicIntervals);
        if (!found) {
            throw std::runtime_error("Diatonic chord not found for scale " + scale);
        }
        finalQuality = found->getQualityName();
    }

    // Now construct the chord string "root + finalQuality"
    std::string chordExpression = chordRoot + finalQuality;
    return Chord(chordExpression);
}

std::string Chord::chordName() const {
    return m_chordName;
}

std::string Chord::root() const {
    return m_root;
}

std::shared_ptr<Quality> Chord::quality() const {
    return m_quality;
}

std::vector<std::string> Chord::appended() const {
    return m_appended;
}

std::string Chord::on() const {
    return m_on;
}

std::string Chord::info() const {
    std::ostringstream oss;
    oss << m_chordName << "\n"
        << "root=" << m_root << "\n"
        << "quality=" << (m_quality ? m_quality->getQualityName() : "null") << "\n"
        << "on=" << m_on << "\n";
    if (!m_appended.empty()) {
        oss << "appended=[";
        for (auto &app : m_appended) {
            oss << app << ",";
        }
        oss << "]\n";
    }
    return oss.str();
}

void Chord::transpose(int semitones, const std::string& scale) {
    if (semitones == 0) return;
    // transpose root, on chord
    m_root = transposeNote(m_root, semitones, scale);
    if (!m_on.empty()) {
        m_on = transposeNote(m_on, semitones, scale);
    }
    // re-build chord text
    reconfigureChord();
}

std::vector<int> Chord::components(bool visible) const {
    // We rely on Quality::getComponents to get absolute intervals.
    // If visible is false, we return numeric intervals with the root forced to 0-based.
    // So we do:
    auto compsAbs = m_quality->getComponents(m_root, false);
    // Then if we want to normalize so that the root is 0, we subtract the first from all.
    // Actually, the python code used "positions" so that the first is 0, etc.

    if (compsAbs.empty()) return compsAbs;

    // sort them
    std::sort(compsAbs.begin(), compsAbs.end());
    // subtract compsAbs[0]
    int base = compsAbs[0];
    for (auto &v : compsAbs) {
        v -= base;
    }
    return compsAbs;
}

std::vector<std::string> Chord::componentsWithPitch(int rootPitch) const {
    // In Python, we see that if the root note is < 0, we add 12, etc.
    // We'll do a simpler approach: get absolute intervals, then for each, compute an octave offset
    auto compsAbs = m_quality->getComponents(m_root, false);
    std::vector<std::string> result;
    result.reserve(compsAbs.size());
    for (int semitone : compsAbs) {
        // semitone might be negative if it's a slash chord that forced a negative offset
        // We'll do a naive approach: figure out how many 12's fit below or above rootPitch
        // But let's match the python approach exactly:
        // "val_to_note(c, scale=self._root) + std::to_string(root_pitch + c // 12)"
        int octaveOffset = rootPitch + semitone / 12;
        int noteVal = semitone;
        std::string noteName = valToNote(noteVal, m_root);
        result.push_back(noteName + std::to_string(octaveOffset));
    }
    return result;
}

bool Chord::operator==(const Chord& other) const {
    // Compare roots by semitone, so e.g. "C" == "B#" is the same
    if (noteToVal(m_root) != noteToVal(other.m_root)) {
        return false;
    }
    // Compare quality by intervals
    if (!m_quality && !other.m_quality) {
        // both null
        return true;
    } else if ((m_quality && !other.m_quality) || (!m_quality && other.m_quality)) {
        return false;
    } else {
        // compare the actual intervals from root=0
        auto myComps = components(false);
        auto theirComps = other.components(false);
        if (myComps != theirComps) {
            return false;
        }
    }

    // appended
    if (m_appended.size() != other.m_appended.size()) {
        return false;
    }
    if (m_appended != other.m_appended) {
        return false;
    }

    // slash (m_on)
    bool hasOnA = !m_on.empty();
    bool hasOnB = !other.m_on.empty();
    if (hasOnA && hasOnB) {
        if (noteToVal(m_on) != noteToVal(other.m_on)) {
            return false;
        }
    } else if (hasOnA != hasOnB) {
        // one has slash chord, the other doesn't
        return false;
    }

    return true;
}

void Chord::reconfigureChord() {
    // Rebuild textual name from pieces
    std::string qName = m_quality ? m_quality->getQualityName() : "";
    // appended
    std::string appended = displayAppended(m_appended);
    // slash
    std::string slash = displayOn(m_on);

    m_chordName = m_root + qName + appended + slash;
}

void Chord::applyOnChord() {
    if (m_quality && !m_on.empty()) {
        m_quality->appendOnChord(m_on, m_root);
    }
}
