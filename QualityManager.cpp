#include "QualityManager.hpp"
#include "Constants.hpp"
#include <stdexcept>
#include <algorithm>

QualityManager& QualityManager::Instance() {
    static QualityManager instance;
    return instance;
}

QualityManager::QualityManager() {
    loadDefaultQualities();
}

void QualityManager::loadDefaultQualities() {
    m_qualities.clear();
    for (const auto& pair : DEFAULT_QUALITIES) {
        const std::string& qname = pair.first;
        const std::vector<int>& comps = pair.second;
        m_qualities[qname] = std::make_shared<Quality>(qname, comps);
    }
}

/**
 * If name is known, returns a new copy of that Quality.
 * If 'inversion' > 0, we shift intervals accordingly.
 */
std::shared_ptr<Quality> QualityManager::getQuality(const std::string& name, int inversion) {
    auto it = m_qualities.find(name);
    if (it == m_qualities.end()) {
        throw std::runtime_error("Unknown quality: " + name);
    }
    // Create a new copy
    auto q = std::make_shared<Quality>(*it->second);

    // Inversion: "rotate" intervals n times
    for (int i = 0; i < inversion; ++i) {
        if (!q->getComponents("C").empty()) {
            // We want to rotate the front interval to the back, adding 12 if needed
            // For clarity, let's do something like:
            auto comps = q->getComponents("C", false); // absolute intervals from root "C"
            // But actually we stored the raw intervals in m_components. We can do direct manipulation:
            // We'll do a small workaround: let's define a function to get the "raw" intervals from the
            // python approach. But we haven't stored that in the public interface.
            // For simplicity in C++: let's replicate the rotation logic as in Python:

            // We'll actually store the intervals from the original q. Then reassign them:
            std::vector<int> intervals = it->second->getComponents("C", false);
            // That won't reflect the current state if we invert multiple times.
            // So let's do something simpler: We'll store a local copy of the q's intervals from root=0 approach.
            // Because each rotation is incremental.
            auto baseComps = q->getComponents("C", false);

            // We'll shift the actual underlying data. Let's approximate it:
            // We do something like:
            // front = baseComps[0], remove it from the front, add 12 until it's above the last item
            // then push it to the back.

            // This is actually somewhat more involved to do robustly. For now, let's keep it simpler:
            // We'll do a single rotation that the Python code did:
            //    n = q.components[0]
            //    while n < q.components[-1]:
            //        n += 12
            //    q.components = q.components[1:] + (n,)

            // We'll simulate that by direct access to m_qualities, but that's private.
            // For a simpler approach, we can interpret "inversion" as shifting the first
            // interval up by an octave and moving it to the back. We'll approximate:

            // Not 100% identical to Python if repeated. But let's do a single shift:

            // We can do a manual approach: we know the internal stored intervals are
            // (0, x, y, z). We'll read them from it->second, but that won't incorporate
            // prior inversions. Let's do it on q itself:

            std::vector<int> newAbs = q->getComponents("C", false);
            if (newAbs.size() < 2) continue;

            int first = newAbs[0];
            int last = newAbs[newAbs.size() - 1];
            // Increase 'first' until it is > last:
            while (first <= last) {
                first += 12;
            }
            // Then remove front, push back:
            newAbs.erase(newAbs.begin());
            newAbs.push_back(first);

            // Now we re-set q by constructing a fresh Quality with these intervals minus the root
            // Actually, to get them back into relative form, we subtract the (new) smallest from all:
            // The new root is newAbs[0], which might not be 0 any more. Let's store that offset:
            int offset = newAbs[0];
            for (auto &val : newAbs) {
                val -= offset;
            }

            // We create a new Quality with the name the same but new intervals
            auto newQ = std::make_shared<Quality>(q->getQualityName(), newAbs);
            q = newQ;
        }
    }

    return q;
}

void QualityManager::setQuality(const std::string& name, const std::vector<int>& components) {
    m_qualities[name] = std::make_shared<Quality>(name, components);
}

std::shared_ptr<Quality> QualityManager::findQualityFromComponents(const std::vector<int>& components) {
    // Normalize input so the first interval is 0
    if (components.empty()) {
        return nullptr;
    }
    // Copy and shift so that components[0] is zero
    std::vector<int> normalized = components;
    std::sort(normalized.begin(), normalized.end());
    int base = normalized[0];
    for (auto &val : normalized) {
        val -= base;
    }

    // 1) First pass: Exact match
    for (const auto& kv : m_qualities) {
        auto qualityPtr = kv.second; // shared_ptr<Quality>
        // Retrieve this chord quality's intervals from root=0
        std::vector<int> refIntervals = qualityPtr->getComponents("C", false);
        std::sort(refIntervals.begin(), refIntervals.end());
        if (!refIntervals.empty()) {
            int shift = refIntervals[0];
            for (auto &v : refIntervals) {
                v -= shift;
            }
        }
        // Compare for exact equality
        if (refIntervals == normalized) {
            // Return a new copy of that quality
            return std::make_shared<Quality>(*qualityPtr);
        }
    }

    // 2) Second pass: Subset match
    //    If all intervals in "normalized" appear in the chord's interval set,
    //    we treat it as a match (i.e., chord is missing some tones).
    auto isSubset = [&](const std::vector<int>& small, const std::vector<int>& big) {
        // Both sorted. We'll do a standard subset check
        size_t i = 0, j = 0;
        while (i < small.size() && j < big.size()) {
            if (small[i] == big[j]) {
                ++i; 
                ++j;
            } else if (small[i] < big[j]) {
                // Can't match
                return false;
            } else {
                // small[i] > big[j], so advance big
                ++j;
            }
        }
        return (i == small.size());
    };

    for (const auto& kv : m_qualities) {
        auto qualityPtr = kv.second;
        // Retrieve intervals from root=0 (same approach as above)
        std::vector<int> refIntervals = qualityPtr->getComponents("C", false);
        std::sort(refIntervals.begin(), refIntervals.end());
        if (!refIntervals.empty()) {
            int shift = refIntervals[0];
            for (auto &v : refIntervals) {
                v -= shift;
            }
        }
        // Check if our normalized intervals are a subset of refIntervals
        if (isSubset(normalized, refIntervals)) {
            // Found a subset match: "missing tones" chord
            return std::make_shared<Quality>(*qualityPtr);
        }
    }

    // 3) If still not found, return nullptr
    return nullptr;
}


/* original (old) one
std::shared_ptr<Quality> QualityManager::findQualityFromComponents(const std::vector<int>& components) {
    // We do an exact match of intervals
    // The python code was a direct list compare. We'll replicate that:
    // But first we want to see if the root is at 0? The Python code expected them to start at 0.
    // We'll assume they've been normalized so that the first note is 0. We attempt direct match.
    for (const auto& kv : m_qualities) {
        // kv.second is a shared_ptr<Quality>
        // We'll compare kv.second->m_components to components
        // but we only have getComponents(...) which returns absolute intervals from a root
        // so let's store the quality's raw intervals from root=0:
        auto baseComps = kv.second->getComponents("C", false); 
        // subtract baseComps[0] from each to ensure the first is zero:
        if (!baseComps.empty()) {
            int shift = baseComps[0];
            for (auto &x : baseComps) {
                x -= shift;
            }
        }
        if (baseComps == components) {
            // Found it
            // Return a new copy
            return std::make_shared<Quality>(*kv.second);
        }
    }
    return nullptr;
}
*/