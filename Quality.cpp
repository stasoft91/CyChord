#include "Quality.hpp"
#include "Utils.hpp"
#include <stdexcept>
#include <algorithm>

Quality::Quality(const std::string& name, const std::vector<int>& components)
    : m_qualityName(name),
      m_components(components)
{
}

std::string Quality::getQualityName() const {
    return m_qualityName;
}

/**
 * Returns either numeric intervals or note names for this chord
 * from the given root.
 */
std::vector<int> Quality::getComponents(const std::string& root, bool visible) const {
    // We'll store the raw intervals and possibly convert if visible=true.
    // But let's keep logic consistent with Python: if visible, we do note-names,
    // but let's do a two-step approach:
    //   1) compute absolute semitones
    //   2) if visible, convert them to note strings
    // Because the function signature returns a vector<int>, we might return
    // semitones. If we want strings, we'd need a vector<std::string>. So either
    // we overload or we do a union approach. For clarity, let's do intervals if !visible,
    // or an empty vector if visible. We'll handle that differently in Chord::components().
    // 
    // We'll simply return the absolute semitones here to match the Python approach
    // where the "visible" transformation was done by the chord function itself.
    // 
    // So ignoring the 'visible' param for now, returning semitones:

    int rootSemitone = noteToVal(root);
    std::vector<int> absValues;
    absValues.reserve(m_components.size());
    for (int c : m_components) {
        absValues.push_back(c + rootSemitone);
    }
    return absValues;
}

/**
 * Reorders intervals so that 'onChord' is the lowest note. 
 * Example from Python: 
 *     if onChordVal is found in the intervals, remove and re-insert it in front, etc.
 */
void Quality::appendOnChord(const std::string& onChord, const std::string& root) {
    int rv = noteToVal(root);
    int onVal = noteToVal(onChord);

    // Convert onVal to be relative to root's semitone (like Python: onVal - rootVal).
    int relOnVal = onVal - rv;

    // We have intervals in m_components. We want to reorder them so that relOnVal is the first.
    // First check if any existing intervals mod 12 match relOnVal mod 12. If so, remove that.
    // Then insert relOnVal at front if not present.
    // If relOnVal is higher, we might reduce by 12 to keep chord tidy.

    std::vector<int> newComponents;
    newComponents.reserve(m_components.size());

    // Remove any occurrence that matches relOnVal mod 12:
    bool foundMatch = false;
    for (int interval : m_components) {
        if ((interval % 12) == (relOnVal % 12)) {
            foundMatch = true;
            // skip adding it to newComponents
        } else {
            newComponents.push_back(interval);
        }
    }
    // Possibly adjust relOnVal to ensure it's the smallest in the new chord
    // In Python code, we do:
    // if on_chord_val > root_val: on_chord_val -= 12
    // But that's a direct interpretation. Typically we want the "lowest possible" slash note.
    // We'll do it similarly:
    if (relOnVal > 0) {
        relOnVal -= 12;
    }

    // Now insert it at front if not found
    // (In Python code we do an unconditional insert. We'll replicate that.)
    newComponents.insert(newComponents.begin(), relOnVal);

    m_components = newComponents;
}

bool Quality::operator==(const Quality& other) const {
    if (m_components.size() != other.m_components.size()) {
        return false;
    }
    // we compare exact intervals
    return (m_components == other.m_components);
}

int Quality::rootVal(const std::string& root) const {
    return noteToVal(root);
}
