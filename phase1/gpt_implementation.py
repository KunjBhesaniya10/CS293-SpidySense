def build_suffix_array(text):
    """Build suffix array and LCP array."""
    n = len(text)
    suffixes = sorted((text[i:], i) for i in range(n))
    print(suffixes)
    suffix_array = [suffix[1] for suffix in suffixes]
    
    # Build LCP array
    lcp = [0] * n
    for i in range(1, n):
        lcp[i] = longest_common_prefix(suffixes[i - 1][0], suffixes[i][0])
    
    return suffix_array, lcp

def longest_common_prefix(s1, s2):
    """Calculate the longest common prefix (LCP) between two strings."""
    length = 0
    while length < min(len(s1), len(s2)) and s1[length] == s2[length]:
        length += 1
    return length

def approximate_match(text, pattern, k):
    """Find the longest approximate match of pattern in text with tolerance k."""
    suffix_array, lcp = build_suffix_array(text)
    left, right = 0, len(text) - 1
    best_match_length, best_match_index = 0, -1
    
    # Binary search for approximate pattern matching
    while left <= right:
        mid = (left + right) // 2
        pos = suffix_array[mid]
        mismatches = count_mismatches(text, pattern, pos, k)
        
        if mismatches <= k:
            match_length = min(len(text) - pos, len(pattern))
            if match_length > best_match_length:
                best_match_length = match_length
                best_match_index = pos
            right = mid - 1  # Continue searching for longer matches to the left
        else:
            left = mid + 1  # Move right in search space
    
    return text[best_match_index:best_match_index + best_match_length]

def count_mismatches(text, pattern, start, k):
    """Count mismatches between pattern and text substring from start."""
    mismatches = 0
    for i in range(len(pattern)):
        if start + i >= len(text) or text[start + i] != pattern[i]:
            mismatches += 1
            if mismatches > k:
                return mismatches
    return mismatches 

# Example usage
text = "abracadabra"
pattern = "abrax"
tolerance_k = 2
longest_approximate_match = approximate_match(text, pattern, tolerance_k)
print(longest_approximate_match)
print(f"Longest approximate match: {longest_approximate_match}")
